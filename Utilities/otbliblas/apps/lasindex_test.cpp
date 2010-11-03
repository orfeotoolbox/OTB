// lasindex_test.cpp : Defines the entry point for the console application.
//

#include <boost/cstdint.hpp>

#include <iosfwd>
#include <iostream>
#include <cstdio>
#include <bitset>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream> // std::stringstream
#include <string>
#include <stack>
#include <typeinfo>
#include <vector>

#include <liblas/lasindex.hpp>

using namespace liblas;
using namespace boost;

#ifdef _WIN32
#define compare_no_case(a,b,n)  _strnicmp( (a), (b), (n) )
#else
#define compare_no_case(a,b,n)  strncasecmp( (a), (b), (n) )
#endif

#define lasindex_test_version	"1.0"
#define SAMPLE_AUTHOR	"Gary Huber"
#define SAMPLE_COMMENT	"This is a test of the LAS file index system in LibLAS"
#define SAMPLE_DATE	"7/22/10"
void usage(FILE *debugger)
{
    fprintf(debugger,"----------------------------------------------------------\n");
    fprintf(debugger,"    lasindex_test (version %s) usage:\n", lasindex_test_version);
    fprintf(debugger,"----------------------------------------------------------\n");
    fprintf(debugger,"\n");

    fprintf(debugger,"Create index for in file, resave to out file, temporary storage file, max memory usage, sort with Z bin interval:\n");
    fprintf(debugger,"  lasindex_test -i in.las -o out.las -t tempfile -z interval -m maxmemory\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-i or --infile (required):\n");
    fprintf(debugger,"  complete input file name and path\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-o or --outfile (required unless readonly):\n");
    fprintf(debugger,"  complete output file name and path\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-n or --indexfile (required for reading standalone index):\n");
    fprintf(debugger,"  complete output file name and path\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-t or --tempfile (required):\n");
    fprintf(debugger,"  complete temporary file name and path used during index building\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-b or --zbinheight (optional):\n");
    fprintf(debugger,"  elevation sorting bin height, no z sorting if omitted\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-m or --maxmem (optional):\n");
    fprintf(debugger,"  maximum memory to use for index building, defaults to no limit if omitted\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-a or --author (optional):\n");
    fprintf(debugger,"  author field for index file record header, 512 max length\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-c or --comment (optional):\n");
    fprintf(debugger,"  comment field for index file record header, 512 max length\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-d or --date (optional):\n");
    fprintf(debugger,"  date of index creation for index file record header, 512 max length\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-r or --readonly (optional):\n");
    fprintf(debugger,"  if set, index is to be read only, not created\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-s or --standalone (optional):\n");
    fprintf(debugger,"  if set, index is to be saved in separate index file, not in las file\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-x (at least one dimension required for filter):\n");
    fprintf(debugger,"  follow with low and high values for X filter\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-y (at least one dimension required for filter):\n");
    fprintf(debugger,"  follow with low and high values for Y filter\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger,"-z (at least one dimension required for filter):\n");
    fprintf(debugger,"  follow with low and high values for Z filter\n");
    fprintf(debugger,"\n");
    
    fprintf(debugger, "\nFor more information, see the full documentation for lasindex_test at:\n"
                    " http://liblas.org/browser/trunk/doc/lasindex_test.txt\n");
    fprintf(debugger,"----------------------------------------------------------\n");

}

std::istream* OpenInput(std::string filename) 
{
    std::ios::openmode const mode = std::ios::in | std::ios::binary;
    std::istream* istrm = 0;
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
        throw std::runtime_error("Reading stream was not able to be created.");
    }
    return istrm;
}

std::ostream* OpenOutput(std::string filename) 
{
    std::ios::openmode const mode = std::ios::out | std::ios::binary;
    std::ostream* ostrm = 0;
    if (compare_no_case(filename.c_str(),"STDOUT",5) == 0)
    {
        ostrm = &std::cout;
    }
    else 
    {
        ostrm = new std::ofstream(filename.c_str(), mode);
    }
    
    if (!ostrm->good())
    {
        delete ostrm;
        throw std::runtime_error("Writing stream was not able to be created.");
    }
    return ostrm;
}

void IndexInitError(FILE *debugger)
{
    fprintf(debugger, "Unable to initialize index.\n");
}

void IndexFilterNoPoints(FILE *debugger)
{
    fprintf(debugger, "No points found in search area.\n");
}

void IndexFilterInitError(FILE *debugger)
{
    fprintf(debugger, "Unable to initialize index filter. Invalid values.\n");
}


int main(int argc, char* argv[])
{
	char *tmpfilenme = 0;
	char *lasinfilenme = 0;
	char *lasoutfilenme = 0;
	char *idxinfilenme = 0;
	char *authorname = 0;
	char *commentfield = 0;
	char *datefield = 0;
	double zbinheight = 0.0;
	double oLowFilterX = 0.0, oHighFilterX = 0.0, oLowFilterY = 0.0, oHighFilterY = 0.0, oLowFilterZ = 0.0, oHighFilterZ = 0.0;
	uint32_t maxmem = 0;
	int debuglevel = 3;
	bool readonly = 0;
	bool forcenewindex = 0;
	bool boundssetbyuser = 0;
	bool writestandaloneindex = 0;
	FILE *debugger = stderr;
	
	// temporary until argv[] starts to work
	// uncomment only one of these blocks
	/*------------------------------N1440375----------------------------------*/
	/*------------------------build embedded index------------------------------
	const char* arggv[] = {"foo", "-t", "C:\\LibLAS\\Samples\\N1440375.tmp",
		 "-i", "C:\\LibLAS\\Samples\\N1440375.las", "-a", SAMPLE_AUTHOR, "-c", SAMPLE_COMMENT, "-d", SAMPLE_DATE,
		 "-o", "C:\\LibLAS\\Samples\\N1440375_idx.las"};
	argc = 13;
	*/
	/*-----------------build index in standalone file---------------------------
	const char* arggv[] = {"foo", "-t", "C:\\LibLAS\\Samples\\N1440375.tmp",
		 "-i", "C:\\LibLAS\\Samples\\N1440375.las", "-a", SAMPLE_AUTHOR, "-c", SAMPLE_COMMENT, "-d", SAMPLE_DATE,
		 "-o", "C:\\LibLAS\\Samples\\N1440375_idx.ldx", "-s"};
	argc = 14;
	*/
	/*------------------filter with embedded index------------------------------
	const char* arggv[] = {"foo",
		 "-i", "C:\\LibLAS\\Samples\\N1440375_idx.las", "-r"};
	argc = 4;
	*/
	/*-------------------filter from standalone file----------------------------
	const char* arggv[] = {"foo", "-i", "C:\\LibLAS\\Samples\\N1440375.las",
		 "-n", "C:\\LibLAS\\Samples\\N1440375_idx.ldx", "-r"};
	argc = 6;
	*/
	/*-----------------build embedded index, filter with user bounds------------
	const char* arggv[] = {"foo", "-t", "C:\\LibLAS\\Samples\\N1440375.tmp",
		 "-i", "C:\\LibLAS\\Samples\\N1440375.las", "-a", SAMPLE_AUTHOR, "-c", SAMPLE_COMMENT, "-d", SAMPLE_DATE,
		 "-o", "C:\\LibLAS\\Samples\\N1440375_idx.las",
		 "-x", "1443000.00", "1444000.00", "-y", "376000.02", "379000.00", "-z", "850.00", "950.00"};
	argc = 22;
	*/	
	/*---------------------Serpent Mound Model LAS Data-----------------------*/
	/*----------------------------build index-----------------------------------
	const char* arggv[] = {"foo", "-t", "C:\\LibLAS\\Samples\\Serpent Mound Model LAS Data.tmp",
		 "-i", "C:\\LibLAS\\Samples\\Serpent Mound Model LAS Data.las", "-a", SAMPLE_AUTHOR, "-c", SAMPLE_COMMENT, "-d", SAMPLE_DATE,
		 "-o", "C:\\LibLAS\\Samples\\Serpent Mound Model LAS Data_idx.las"};
	argc = 13;
	*/
	/*------------------filter with embedded index------------------------------
	const char* arggv[] = {"foo",
		 "-i", "C:\\LibLAS\\Samples\\Serpent Mound Model LAS Data_idx.las", "-r"};
	argc = 4;
	*/
	/*---------------------Mount St Helens Oct 4 2004-------------------------*/
	/*----------------------------build index-----------------------------------
	const char* arggv[] = {"foo", "-t", "C:\\LibLAS\\Mount St Helens Oct 4 2004.tmp",
		 "-i", "C:\\LibLAS\\Samples\\Mount St Helens Oct 4 2004.las", "-a", SAMPLE_AUTHOR, "-c", SAMPLE_COMMENT, "-d", SAMPLE_DATE,
		 "-o", "C:\\LibLAS\\Samples\\Mount St Helens Oct 4 2004_idx.las"};
	argc = 13;
	*/
	/*------------------filter with embedded index------------------------------
	const char* arggv[] = {"foo",
		 "-i", "C:\\LibLAS\\Samples\\Mount St Helens Oct 4 2004_idx.las", "-r"};
	argc = 4;
	*/
	/*----------------------------Lincoln-------------------------------------*/
	/*--------------------------build index-------------------------------------
	const char* arggv[] = {"foo", "-t", "C:\\LibLAS\\Samples\\Lincoln.tmp",
		 "-i", "C:\\LibLAS\\Samples\\Lincoln.las", "-a", SAMPLE_AUTHOR, "-c", SAMPLE_COMMENT, "-d", SAMPLE_DATE,
		 "-o", "C:\\LibLAS\\Samples\\Lincoln_idx.las"};
	argc = 13;
	*/
	/*------------------filter with embedded index------------------------------
	const char* arggv[] = {"foo",
		 "-i", "C:\\LibLAS\\Samples\\Lincoln_idx.las", "-r"};
	argc = 4;
	*/
	/*------------------------------flatDataset-------------------------------*/
	///*------------------------------build index---------------------------------
	const char* arggv[] = {"foo", "-t", "C:\\LibLAS\\flatDataset.tmp",
		 "-i", "C:\\LibLAS\\Samples\\flatDataset.las", "-a", SAMPLE_AUTHOR, "-c", SAMPLE_COMMENT, "-d", SAMPLE_DATE,
		 "-o", "C:\\LibLAS\\Samples\\flatDataset_idx.las"};
	argc = 13;
	//*/
	/*------------------filter with embedded index------------------------------
	const char* arggv[] = {"foo",
		 "-i", "C:\\LibLAS\\Samples\\flatDataset_idx.las", "-r"};
	argc = 4;
	*/
	/*------------------------------------------------------------------------*/
	
	for (int i = 1; i < argc; i++)
    {
        if (    strcmp((const char *)arggv[i],"-h") == 0 ||
                strcmp((const char *)arggv[i],"--help") == 0
            )
        {
            usage(debugger);
            exit(0);
        }
        else if (   strcmp((const char *)arggv[i],"-t") == 0 ||
                    strcmp((const char *)arggv[i],"--tempfile") == 0
            )
        {
            i++;
            tmpfilenme = (char *)arggv[i];
        }
        else if (   strcmp((const char *)arggv[i],"-i") == 0 ||
                    strcmp((const char *)arggv[i],"--infile") == 0
            )
        {
            i++;
            lasinfilenme = (char *)arggv[i];
        }
        else if (   strcmp((const char *)arggv[i],"-o") == 0 ||
                    strcmp((const char *)arggv[i],"--outfile") == 0
            )
        {
            i++;
            lasoutfilenme = (char *)arggv[i];
        }
        else if (   strcmp((const char *)arggv[i],"-n") == 0 ||
                    strcmp((const char *)arggv[i],"--indexfile") == 0
            )
        {
            i++;
            idxinfilenme = (char *)arggv[i];
        }
        else if (   strcmp((const char *)arggv[i],"-b") == 0 ||
                    strcmp((const char *)arggv[i],"--zbinheight") == 0
            )
        {
            i++;
            zbinheight = atof((const char *)arggv[i]);
        }
        else if (   strcmp((const char *)arggv[i],"-m") == 0 ||
                    strcmp((const char *)arggv[i],"--maxmem") == 0
            )
        {
            i++;
            maxmem = atoi((const char *)arggv[i]);
        }
        else if (   strcmp((const char *)arggv[i],"-a") == 0 ||
                    strcmp((const char *)arggv[i],"--author") == 0
            )
        {
            i++;
            authorname = (char *)arggv[i];
        }
        else if (   strcmp((const char *)arggv[i],"-c") == 0 ||
                    strcmp((const char *)arggv[i],"--comment") == 0
            )
        {
            i++;
            commentfield = (char *)arggv[i];
        }
        else if (   strcmp((const char *)arggv[i],"-d") == 0 ||
                    strcmp((const char *)arggv[i],"--date") == 0
            )
        {
            i++;
            datefield = (char *)arggv[i];
        }
        else if (   strcmp((const char *)arggv[i],"-r") == 0 ||
                    strcmp((const char *)arggv[i],"--readonly") == 0
            )
        {
            readonly = true;
        }
        else if (   strcmp((const char *)arggv[i],"-s") == 0 ||
                    strcmp((const char *)arggv[i],"--standalone") == 0
            )
        {
            writestandaloneindex = true;
        }
        else if (   strcmp((const char *)arggv[i],"-x") == 0
            )
        {
            i++;
            oLowFilterX = atof((const char *)arggv[i]);
            i++;
            oHighFilterX = atof((const char *)arggv[i]);
            boundssetbyuser = true;
        }
        else if (   strcmp((const char *)arggv[i],"-y") == 0
            )
        {
            i++;
            oLowFilterY = atof((const char *)arggv[i]);
            i++;
            oHighFilterY = atof((const char *)arggv[i]);
            boundssetbyuser = true;
        }
        else if (   strcmp((const char *)arggv[i],"-z") == 0
            )
        {
            i++;
            oLowFilterZ = atof((const char *)arggv[i]);
            i++;
            oHighFilterZ = atof((const char *)arggv[i]);
            boundssetbyuser = true;
        }
	} // for

	fprintf(debugger, "%s\n", lasinfilenme);
	
	IndexData ParamSrc;
	
	if (lasinfilenme)
	{
		// open the las file for reading
		if (std::istream* istrm = OpenInput(std::string(lasinfilenme)))
		{
			if (Reader *reader = new Reader(*istrm))
			{
				Reader *idxreader = NULL;
				std::istream* idxstrm = NULL;
				if (idxinfilenme)
				{
					idxstrm = OpenInput(std::string(idxinfilenme));
					if (idxstrm)
					{
						idxreader = new Reader(*idxstrm);
					} // if
				} // if
				if (idxreader || ! idxinfilenme)
				{
					// open the output stream here whether for embedded index or standalone
					std::ostream* ostrm = ! readonly ? OpenOutput(std::string(lasoutfilenme)): 0;
					if (ostrm || readonly)
					{
						// there are many versions of this method tailored to specific index tasks. Look for them
						// together in lasindex.hpp. This is version the most generic
						if (ParamSrc.SetInitialValues(0, reader, ostrm, idxreader, tmpfilenme, authorname, commentfield, datefield,
							zbinheight, maxmem, debuglevel, readonly, writestandaloneindex, forcenewindex, debugger))
						{
							// Another way to initiate an index would be to
							// create a simple index with Index() and then initialize it with the Prep command.
							// It would look like this:
							// Index index();  if (index.Prep(ParamSrc)) {}
							// Prep returns true only if an index is ready to use for filtering
							// But here's the other way to do it in which the constructor does the heavy lifting
							Index index(ParamSrc);
							if (index.IndexReady())
							{
								// for testing filter bounds set by user
								if (boundssetbyuser)
								{
									Bounds<double> filterBounds(oLowFilterX, oLowFilterY, oLowFilterZ,
										oHighFilterX, oHighFilterY, oHighFilterZ);
									if (ParamSrc.SetFilterValues(filterBounds, index))
									{
										const std::vector<uint32_t>& FilterResult = index.Filter(ParamSrc);
										if (FilterResult.size())
										{
											// do something with the list of points
											fprintf(debugger, "Points within filter area %zu of %d, %s\n", FilterResult.size(), 
												index.GetPointRecordsCount(), "User-defined gilter bounds");
										}
										else
											IndexFilterNoPoints(debugger);
									}
									else
										IndexFilterInitError(debugger);
								} // if
								else
								{
									// run 6 programmed filter tests of stored data
									double RangeZ, RangeY, RangeX;
									const char *CovgStr;
									Bounds<double> indexBounds = index.GetBounds();
									Bounds<double> filterBounds;
									RangeX = index.GetRangeX();
									RangeY = index.GetRangeY();
									RangeZ = index.GetRangeZ();
									for (int loopct = 0; loopct < 6; ++loopct)
									{
										switch (loopct)
										{
											case 0:
											{
												CovgStr = "entire range, all 3 axes";
												filterBounds = indexBounds;
												break;
											} // 0
											case 1:
											{
												CovgStr = "middle half of all 3 axes";
												filterBounds = Bounds<double>
													(indexBounds.min(0) + .25 * RangeX,
													indexBounds.min(1) + .25 * RangeY,
													indexBounds.min(2) + .25 * RangeZ,
													indexBounds.max(0) - .25 * RangeX,
													indexBounds.max(1) - .25 * RangeY,
													indexBounds.max(2) - .25 * RangeZ);
												break;
											} // 1
											case 2:
											{
												CovgStr = "upper left, all Z range";
												filterBounds = Bounds<double>
													(indexBounds.min(0),
													indexBounds.min(1) + .5 * RangeY,
													indexBounds.min(2),
													indexBounds.min(0) + .5 * RangeX,
													indexBounds.max(1),
													indexBounds.max(2));
												break;
											} // 2
											case 3:
											{
												CovgStr = "upper right, all Z range";
												filterBounds = Bounds<double>
													(indexBounds.min(0) + .5 * RangeX,
													indexBounds.min(1) + .5 * RangeY,
													indexBounds.min(2),
													indexBounds.max(0),
													indexBounds.max(1),
													indexBounds.max(2));
												break;
											} // 3
											case 4:
											{
												CovgStr = "lower left, all Z range";
												filterBounds = Bounds<double>
													(indexBounds.min(0),
													indexBounds.min(1),
													indexBounds.min(2),
													indexBounds.min(0) + .5 * RangeX,
													indexBounds.min(1) + .5 * RangeY,
													indexBounds.max(2));
												break;
											} // 4
											case 5:
											{
												CovgStr = "lower right, all Z range";
												filterBounds = Bounds<double>
													(indexBounds.min(0)+ .5 * RangeX,
													indexBounds.min(1),
													indexBounds.min(2),
													indexBounds.max(0),
													indexBounds.min(1) + .5 * RangeY,
													indexBounds.max(2));
												break;
											} // 5
										} // switch
										if (ParamSrc.SetFilterValues(filterBounds, index))
										{
											const std::vector<uint32_t>& FilterResult = index.Filter(ParamSrc);
											if (FilterResult.size())
											{
												// do something with the list of points
												#ifdef VISUAL_8
												fprintf(debugger, "Points within filter area %d of %d, %s\n", FilterResult.size(), 
													index.GetPointRecordsCount(), CovgStr);
												#else // VISUAL_8
												fprintf(debugger, "Points within filter area %zu of %d, %s\n", FilterResult.size(), 
													index.GetPointRecordsCount(), CovgStr);
												#endif // VISUAL_8
											}
											else
												IndexFilterNoPoints(debugger);
										}
										else
											IndexFilterInitError(debugger);
									} // for
								} // else
							} // if
							else
								IndexInitError(debugger);
						} // if
						else
							IndexInitError(debugger);
						if (ostrm)
						{
							if (static_cast<std::ofstream&>(*ostrm))
								static_cast<std::ofstream&>(*ostrm).close();
						} // if
					} // if
				} // if
				if (idxstrm)
				{
					if (static_cast<std::ifstream&>(*idxstrm))
						static_cast<std::ifstream&>(*idxstrm).close();
				} // if
				if (idxreader)
					delete idxreader;
				delete reader;
			} // if reader
		
        if (static_cast<std::ifstream&>(*istrm))
            static_cast<std::ifstream&>(*istrm).close();
        delete istrm;
		} // if istrm
	} // if input file name
	return 0;
}

// } // namespace liblas
