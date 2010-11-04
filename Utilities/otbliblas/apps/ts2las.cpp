// $Id$
//
// ts2las translates TerraSolid .bin file to ASPRS LAS file.
//
// TerraSolid format: http://cdn.terrasolid.fi/tscan.pdf
//
// (C) Copyright Howard Butler 2009, hobu.inc@gmail.com
//
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include "ts2las.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace liblas;

std::istream* OpenInput(std::string filename) 
{
    std::ios::openmode const mode = std::ios::in | std::ios::binary;
    std::istream* istrm;
    if (compare_no_case(filename.c_str(),"STDIN",5) == 0)
    {
        istrm = &std::cin;
    }
    else 
    {
        istrm = new std::ifstream(filename.c_str(), mode);
    }
    
    if (!istrm->good())
    {
        delete istrm;
        throw std::runtime_error("Reading stream was not able to be created");
        exit(1);
    }
    return istrm;
}

std::ostream* OpenOutput(std::string filename)
{
    std::ostream* ostrm;
    std::ios::openmode m;
    m = std::ios::out | std::ios::binary | std::ios::ate;
            
    if (compare_no_case(filename.c_str(),"STOUT",5) == 0)
    {
        ostrm = &std::cout;
    }
    else 
    {
        ostrm = new std::ofstream(filename.c_str(), m);
    }

    
    if (!ostrm->good())
    {
        delete ostrm;
        throw std::runtime_error("Writing stream was not able to be created");
        exit(1);
    }
    
    return ostrm;
}

liblas::Header CreateHeader(ScanHdr* hdr)
{
    liblas::Header header;
    
    // Checks for time and color values
    liblas::PointFormatName format = liblas::ePointFormat0;
    
    if (hdr->Time) {
        if (hdr->Color) {
            format = liblas::ePointFormat3;
        } else {
            format = liblas::ePointFormat1;
        }
    } else if (hdr->Color) {
        format = liblas::ePointFormat2;
    } 
    header.SetVersionMinor(2);
    header.SetDataFormatId(format);
    // header.SetPointRecordsCount(hdr->PntCnt);
    std::cout << "units: " << hdr->Units << std::endl;
    std::cout << "format: " << format << std::endl;
    double scale = 1.0/(double)hdr->Units;
    std::cout << "scale: " << scale << std::endl;
    header.SetScale(scale, scale, scale);
    header.SetOffset(hdr->OrgX*scale, hdr->OrgY*scale, hdr->OrgZ*scale);
    std::cout << "offset x: " << header.GetOffsetX() 
              << " offset y: " << header.GetOffsetY() 
              << " offset z: " <<header.GetOffsetZ() << std::endl;

    return header;
}

bool ReadHeader(ScanHdr* hdr, std::istream* istrm)
{
    try
    {
        liblas::detail::read_n(*hdr, *istrm, sizeof(ScanHdr));
        
        if (hdr->Tunniste != 970401) return false;
        if (memcmp(hdr->Magic,"CXYZ",4)) return false;
        
        int version = hdr->HdrVersion;
        if (version == 970404) return true;
        if (version == 20010129) return true;
        if (version == 20010712) return true;
        if (version == 20020715) return true;
        if (( version > 20020715) && (version < 20051231)) return true;
        return false;
    }
    catch (std::exception const)
    {
        return false;
    }    

    return true;
}

bool WritePoints(liblas::Writer* writer, std::istream* strm, ScanHdr* hdr) 
{
    ScanPnt* point = new ScanPnt;
    ScanRow* row = new ScanRow;
    while (true)
    {

///std::cout << "We have header version" << std::endl;

            try
            {
                // std::cout << "stream position is: " << strm->tellg() << std::endl;
                if (hdr->HdrVersion == 20020715) {
                    detail::read_n(*point, *strm, sizeof(ScanPnt));
                } else{
                    detail::read_n(*row, *strm, sizeof(ScanRow));
                    point->Pnt.x = row->x;
                    point->Pnt.y = row->y;
                    point->Pnt.z = row->z;
                    point->Code = row->Code;
                    point->Line = row->Line;
                    point->Intensity = row->EchoInt & 0x3FFF;
                    point->Echo = (row->EchoInt >> 14);
                }
                Point p;

                p.SetCoordinates(writer->GetHeader(),
                                    point->Pnt.x,
                                    point->Pnt.y,
                                    point->Pnt.z);

                // std::cout << "read x: " << point->Pnt.x << " y: "<< point->Pnt.y << " z: " <<point->Pnt.z<< std::endl;
                // std::cout << "wrote x: " << p.GetX() << " y: "<< p.GetY() << " z: " <<p.GetZ()<< std::endl;
                // std::cout << "Code: " << point->Code << " Intensity: "<< point->Intensity << std::endl;
                p.SetClassification(point->Code);
                p.SetIntensity(point->Intensity);
                if (hdr->Time) {
                    liblas::uint32_t t = 0xFFFFFFFF;
                    detail::read_n(t, *strm, sizeof(t));

                    // Time stamps are assumed to be GPS week seconds. The 
                    // storage format is a 32 bit unsigned integer where 
                    // each integer step is 0.0002 seconds.

                    p.SetTime(t*0.0002);
                }
                if (hdr->Color) {
                    liblas::uint8_t r, g, b, a = 0;
                    liblas::Color color;
                    detail::read_n(r, *strm, sizeof(r));
                    detail::read_n(b, *strm, sizeof(b));
                    detail::read_n(g, *strm, sizeof(g));
                    
                    // TS .bin says to read 4 bytes here for some reason.  Maybe 
                    // this is an alpha value or something
                    detail::read_n(a, *strm, sizeof(a));
                    
                    color.SetGreen(g);
                    color.SetBlue(b);
                    color.SetRed(r);
                    p.SetColor(color);
                }
                
                // Set return number
                /* 
                    TerraScan uses two bits for storing echo information. The possible values are: 
                    0 Only echo 
                    1 First of many echo 
                    2 Intermediate echo 
                    3 Last of many echo
                */
                if (point->Echo == 0) { 
                    p.SetNumberOfReturns(1);
                    p.SetReturnNumber(1);
                } else if (point->Echo == 1) {
                    p.SetReturnNumber(1);
                } else if (point->Echo == 3) {
                    p.SetReturnNumber(2);
                    p.SetNumberOfReturns(2);
                } else {
                    // I don't know what the hell to do here without cumulating
                    // through all of the points.  Why wouldn't you store the return 
                    // number?!
                    p.SetReturnNumber(2);
                    p.SetNumberOfReturns(3);
                }

                try {
                    writer->WritePoint(p);
                } catch (std::exception) 
                {
                    std::cout << "Point writing failed!" << std::endl; 
                }
             
            }
            catch (std::out_of_range) // we reached the end of the file
            {
                std::cout << "catching out of range error!" ;
                return true;
                // break;
            }
            catch (std::exception const& e) // we reached the end of the file
            {
                std::cout << e.what() << std::endl;
                break;
            }   

    }
    delete point;
    delete row;
    return true;    

}

void usage() {}

int main(int argc, char* argv[])
{
    int rc = 0;

    std::string input;
    std::string output;
    
    for (int i = 1; i < argc; i++)
    {
        if (    strcmp(argv[i],"-h") == 0 ||
                strcmp(argv[i],"--help") == 0
            )
        {
            usage();
            exit(0);
        }
        else if (   strcmp(argv[i],"--input") == 0  ||
                    strcmp(argv[i],"-input") == 0   ||
                    strcmp(argv[i],"-i") == 0       ||
                    strcmp(argv[i],"-in") == 0
                )
        {
            i++;
            input = std::string(argv[i]);
        }
        else if (   strcmp(argv[i],"--output") == 0  ||
                    strcmp(argv[i],"--out") == 0     ||
                    strcmp(argv[i],"-out") == 0     ||
                    strcmp(argv[i],"-o") == 0       
                )
        {
            i++;
            output = std::string(argv[i]);
        }
        else if (i == argc - 2 && output.empty() && input.empty())
        {
            input = std::string(argv[i]);
        }
        else if (i == argc - 1 && output.empty() && input.empty())
        {
            input = std::string(argv[i]);
        }
        else if (i == argc - 1 && output.empty() && input.empty())
        {
            output = std::string(argv[i]);
        }
        else 
        {
            usage();
            exit(1);
        }
    }
    
    std::cout << "input: " << input<<  " output: " <<output<<std::endl;
    
    ScanHdr* hdr = new ScanHdr;
    std::istream* istrm = OpenInput(input);
    std::ostream* ostrm = OpenOutput(output);
    
    bool success;
    success = ReadHeader(hdr, istrm);
    if (!success) 
    {
        std::cout<<"header was not read! exiting" << std::cout; exit(1);
    }
    
    // std::cout << "stream position is: " << istrm->tellg() << std::endl;
    liblas::Header header = CreateHeader(hdr);
    liblas::Writer* writer = new liblas::Writer(*ostrm, header);
    
    success = WritePoints(writer, istrm, hdr);
    delete writer;
    delete ostrm;
    delete istrm;
    
    std::cout << "Point Count: " << header.GetPointRecordsCount() <<std::endl;
    std::cout << "success: " << success << std::endl;
    return rc;
}

