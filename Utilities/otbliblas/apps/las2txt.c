/***************************************************************************
 * $Id: las2txt.c 879 2008-09-25 18:26:15Z hobu $
 * $Date: 2008-09-25 13:26:15 -0500 (Thu, 25 Sep 2008) $
 *
 * Project: libLAS -- C/C++ read/write library for LAS LIDAR data
 * Purpose: LAS translation to ASCII text with optional configuration
 * Author:  Martin Isenburg isenburg@cs.unc.edu 
 ***************************************************************************
 * Copyright (c) 2007, Martin Isenburg isenburg@cs.unc.edu 
 *
 * See LICENSE.txt in this source distribution for more information.
 **************************************************************************/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "liblas.h"

void print_header(FILE *file, LASHeaderH header, const char* file_name);

void usage()
{
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"    las2txt (version %s) usage:\n", LAS_GetVersion());
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Convert a las file to text on stdout:\n");
    fprintf(stderr,"  las2txt lidar.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Convert using stdin and stdout:\n");
    fprintf(stderr,"  las2txt --stdin --stdout < lidar.las > lidar.txt\n");
    fprintf(stderr,"\n");
    
    fprintf(stderr,"Convert to text with a specified format :\n");
    fprintf(stderr,"  las2txt --parse xyziar lidar.las lidar.txt\n");
    fprintf(stderr,"\n");

    
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr," The '--parse txyz' flag specifies how to format each\n");
    fprintf(stderr," each line of the ASCII file. For example, 'txyzia'\n");
    fprintf(stderr," means that the first number of each line should be the\n");
    fprintf(stderr," gpstime, the next three numbers should be the x, y, and\n");
    fprintf(stderr," z coordinate, the next number should be the intensity\n");
    fprintf(stderr," and the next number should be the scan angle.\n");
    fprintf(stderr," The supported entries are:\n");
    fprintf(stderr,"   a - scan angle\n");
    fprintf(stderr,"   i - intensity\n");
    fprintf(stderr,"   n - number of returns for given pulse\n");
    fprintf(stderr,"   r - number of this return\n");
    fprintf(stderr,"   c - classification\n");
    fprintf(stderr,"   u - user data (does not currently work)\n");
    fprintf(stderr,"   p - point source ID\n");
    fprintf(stderr,"   e - edge of flight line\n");
    fprintf(stderr,"   d - direction of scan flag\n\n");

    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr," The '--sep space' flag specifies what separator to use. The\n");
    fprintf(stderr," default is a space but 'tab', 'komma', 'colon', 'hyphen',\n");
    fprintf(stderr," 'dot', or 'semicolon' are other possibilities.\n\n");
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr," The '--header pound' flag results in the header information\n");
    fprintf(stderr," being printed at the beginning of the ASCII file in form of\n");
    fprintf(stderr," a comment that starts with the special character '#'. Also\n");
    fprintf(stderr," possible are 'percent', 'dollar', 'semicolon', 'komma',\n");
    fprintf(stderr," 'star', 'colon', or 'semicolon' as that special character.\n");

    fprintf(stderr, "\nFor more information, see the full documentation for las2txt at:\n"
                    " http://liblas.org/browser/trunk/doc/las2txt.txt\n");
    fprintf(stderr,"----------------------------------------------------------\n");

}


static int lidardouble2string(char* string, double value)
{
    int len;
    len = sprintf(string, "%f", value) - 1;
    while (string[len] == '0') len--;
    if (string[len] != '.') len++;
    string[len] = '\0';
    return len;
}


int main(int argc, char *argv[])
{
    int i;
    int use_stdin = FALSE;
    int use_stdout = FALSE;
    int skip_invalid = FALSE;

    int verbose = FALSE;
    char* file_name_in = 0;
    char* file_name_out = 0;
    char separator_sign = ' ';
    char header_comment_sign = '\0';
    char* parse_string = "xyz";
    char printstring[256];
    LASReaderH reader = NULL;
    LASHeaderH header = NULL;
    LASPointH p = NULL;
    FILE* file_out = NULL;
    int len;
  
    for (i = 1; i < argc; i++)
    {
        if (    strcmp(argv[i],"-h") == 0 ||
                strcmp(argv[i],"--help") == 0
            )
        {
            usage();
            exit(0);
        }
        else if (   strcmp(argv[i],"-v") == 0 ||
                    strcmp(argv[i],"--verbose") == 0
            )
        {
            verbose = TRUE;
        }
        else if (   strcmp(argv[i],"-s") == 0 ||
                    strcmp(argv[i],"--skip_invalid") == 0
            )
        {
            skip_invalid = TRUE;
        }   
        else if (   strcmp(argv[i], "--parse") == 0 ||
                    strcmp(argv[i], "-parse") == 0 
                )
        {
            i++;
            parse_string = argv[i];
        }
        else if (   strcmp(argv[i], "--sep") == 0 ||
                    strcmp(argv[i], "-sep") == 0 
                )
        {
            i++;

            if (strcmp(argv[i],"komma") == 0)
            {
                separator_sign = ',';
            }
            else if (strcmp(argv[i],"tab") == 0)
            {
                separator_sign = '\t';
            }
            else if (   strcmp(argv[i],"dot") == 0 || 
                        strcmp(argv[i],"period") == 0
                    )
            {
                separator_sign = '.';
            }
            else if (strcmp(argv[i],"colon") == 0)
            {
                separator_sign = ':';
            }
            else if (   strcmp(argv[i],"scolon") == 0 || 
                        strcmp(argv[i],"semicolon") == 0
                    )
            {
                separator_sign = ';';
            }
            else if (   strcmp(argv[i],"hyphen") == 0 || 
                        strcmp(argv[i],"minus") == 0
                    )
            {
                separator_sign = '-';
            }
            else if (strcmp(argv[i],"space") == 0)
            {
                separator_sign = ' ';
            }
            else
            {
                fprintf(stderr, "ERROR: unknown seperator '%s'\n",argv[i]);
                usage();
                exit(1);
            }
        }
        else if (   strcmp(argv[i], "--header") == 0            || 
                    strcmp(argv[i], "--comment") == 0           || 
                    strcmp(argv[i], "-header") == 0             || 
                    strcmp(argv[i], "-comment") == 0            || 
                    strcmp(argv[i], "-head") == 0  
                )
        {
            i++;
            if (strcmp(argv[i],"komma") == 0)
            {
                header_comment_sign = ',';
            }
            else if (strcmp(argv[i],"colon") == 0)
            {
                header_comment_sign = ':';
            }
            else if (   strcmp(argv[i],"scolon") == 0 || 
                        strcmp(argv[i],"semicolon") == 0
                    )
            {
                header_comment_sign = ';';
            }
            else if (   strcmp(argv[i],"pound") == 0 || 
                        strcmp(argv[i],"hash") == 0
                    )
            {
                header_comment_sign = '#';
            }
            else if (strcmp(argv[i],"percent") == 0)
            {
                header_comment_sign = '%';
            }
            else if (strcmp(argv[i],"dollar") == 0)
            {
                header_comment_sign = '$';
            }
            else if (strcmp(argv[i],"star") == 0)
            {
                header_comment_sign = '*';
            }
            else
            {
                fprintf(stderr, "ERROR: unknown comment symbol '%s'\n",argv[i]);
                usage();
                exit(1);
            }
        }
        else if (   strcmp(argv[i], "--stdin") == 0 ||
                    strcmp(argv[i], "-ilas") == 0
                ) 
        {
            use_stdin = TRUE;
        }
        else if (   strcmp(argv[i], "--stdout") == 0 
                ) 
        {
            use_stdout = TRUE;
        }
        else if (   strcmp(argv[i],"--input") == 0  ||
                    strcmp(argv[i],"-input") == 0   ||
                    strcmp(argv[i],"-i") == 0       ||
                    strcmp(argv[i],"-in") == 0
                )
        {
            i++;
            file_name_in = argv[i];
        }
        else if (   strcmp(argv[i],"--output") == 0  ||
                    strcmp(argv[i],"--out") == 0     ||
                    strcmp(argv[i],"-out") == 0     ||
                    strcmp(argv[i],"-o") == 0       
                )
        {
            i++;
            file_name_out = argv[i];
        }
        else if (i == argc - 2 && file_name_in == 0 && file_name_out == 0)
        {
            file_name_in = argv[i];
        }
        else if (i == argc - 1 && file_name_in == 0 && file_name_out == 0)
        {
            file_name_in = argv[i];
        }
        else if (i == argc - 1 && file_name_in && file_name_out == 0)
        {
            file_name_out = argv[i];
        }
        else 
        {
            fprintf(stderr, "ERROR: unknown argument '%s'\n",argv[i]);
            usage();
            exit(1);
        }
    } /* end looping through argc/argv */


    if (use_stdin) file_name_in = "stdin";
    reader = LASReader_Create(file_name_in);
    if (!reader) {
        LASError_Print("Unable to read file");
        exit(1); 
    } 
      
    header = LASReader_GetHeader(reader);
    if (!header) { 
        LASError_Print("Unable to fetch header for file");
        exit(1); 
    } 


  


    if (use_stdout)
    {
        file_out = stdout;
    }
    else
    {
        if (file_name_out == NULL)
        {
            if (file_name_in == NULL)
            {
                LASError_Print("No input filename was specified");
                usage();
                exit(1);
            }

            len = (int)strlen(file_name_in);
            file_name_out = strdup(file_name_in);
            if (file_name_out[len-3] == '.' && file_name_out[len-2] == 'g' && file_name_out[len-1] == 'z')
            {
                len = len - 4;
            }
            while (len > 0 && file_name_out[len] != '.')
            {
                len--;
            }
            file_name_out[len] = '.';
            file_name_out[len+1] = 't';
            file_name_out[len+2] = 'x';
            file_name_out[len+3] = 't';
            file_name_out[len+4] = '\0';
        }
        file_out = fopen(file_name_out, "w");

        if (file_out == 0)
        {
            LASError_Print("Could not open file for write");
            usage();
            exit(1);
        }
    }



    if (verbose)
    {
        print_header(stderr, header, file_name_in);
    }

    if (header_comment_sign)
    {

        fprintf(file_out, 
                "%c file signature:            '%s'\012", 
                header_comment_sign, 
                LASHeader_GetFileSignature(header)
                );
        fprintf(file_out, 
                "%c file source ID:            %d\012", 
                header_comment_sign, 
                LASHeader_GetFileSourceId(header)
                );
        fprintf(file_out, 
                "%c reserved:                  %d\012", 
                header_comment_sign, 
                LASHeader_GetReserved(header)
                );
        fprintf(file_out, 
                "%c project ID GUID:           %s\012", 
                header_comment_sign, 
                LASHeader_GetProjectId(header)
                );
        fprintf(file_out, 
                "%c version major.minor:       %d.%d\012", 
                header_comment_sign, 
                LASHeader_GetVersionMajor(header), 
                LASHeader_GetVersionMinor(header)
                );
        fprintf(file_out, 
                "%c system_identifier:         '%s'\012", 
                header_comment_sign, 
                LASHeader_GetSystemId(header)
                );
        fprintf(file_out, 
                "%c generating_software:       '%s'\012", 
                header_comment_sign, 
                LASHeader_GetSoftwareId(header)
                );
        fprintf(file_out,
                "%c file creation day/year:    %d/%d\012", 
                header_comment_sign, 
                LASHeader_GetCreationDOY(header), 
                LASHeader_GetCreationYear(header)
                );
        fprintf(file_out, 
                "%c header size                %d\012", 
                header_comment_sign, 
                LASHeader_GetHeaderSize(header)
                );
        fprintf(file_out, 
                "%c offset to point data       %d\012", 
                header_comment_sign, 
                LASHeader_GetDataOffset(header)
                );
        fprintf(file_out, 
                "%c number var. length records %d\012", 
                header_comment_sign, 
                LASHeader_GetRecordsCount(header)
                );
        fprintf(file_out, 
                "%c point data format          %d\012", 
                header_comment_sign, 
                LASHeader_GetDataFormatId(header)
                );
        fprintf(file_out, 
                "%c point data record length   %d\012", 
                header_comment_sign, 
                LASHeader_GetDataRecordLength(header)
                );
        fprintf(file_out,   
                "%c number of point records    %d\012", 
                header_comment_sign, 
                LASHeader_GetPointRecordsCount(header)
                );
        fprintf(file_out, 
                "%c number of points by return %d %d %d %d %d\012", 
                header_comment_sign, 
                LASHeader_GetPointRecordsByReturnCount(header, 0), 
                LASHeader_GetPointRecordsByReturnCount(header, 1), 
                LASHeader_GetPointRecordsByReturnCount(header, 2), 
                LASHeader_GetPointRecordsByReturnCount(header, 3), 
                LASHeader_GetPointRecordsByReturnCount(header, 4)
                );
        fprintf(file_out, 
                "%c  scale factor x y z         %.6f %.6f %.6f\n", 
                header_comment_sign, 
                LASHeader_GetScaleX(header), 
                LASHeader_GetScaleY(header), 
                LASHeader_GetScaleZ(header)
                );
        fprintf(file_out, 
                "%c  offset x y z               %.6f %.6f %.6f\n", 
                header_comment_sign, 
                LASHeader_GetOffsetX(header), 
                LASHeader_GetOffsetY(header), 
                LASHeader_GetOffsetZ(header)
                );
        fprintf(file_out, 
                "%c  min x y z                  %.6f %.6f %.6f\n", 
                header_comment_sign, 
                LASHeader_GetMinX(header), 
                LASHeader_GetMinY(header), 
                LASHeader_GetMinZ(header)
                );
        fprintf(file_out,   
                "%c  max x y z                  %.6f %.6f %.6f\n", 
                header_comment_sign, 
                LASHeader_GetMaxX(header), 
                LASHeader_GetMaxY(header), 
                LASHeader_GetMaxZ(header)
                );

    }



    p = LASReader_GetNextPoint(reader);
    while (p)
    {
        if (skip_invalid && !LASPoint_IsValid(p)) {
            if (verbose) {
                LASError_Print("Skipping writing invalid point...");
            }
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        
        i = 0;
        for (;;)
        {
            switch (parse_string[i])
            {
            /* // the x coordinate */      
            case 'x': 
                lidardouble2string(printstring, LASPoint_GetX(p)); fprintf(file_out, printstring);
                break;
            /* // the y coordinate */
            case 'y': 
                lidardouble2string(printstring, LASPoint_GetY(p)); fprintf(file_out, printstring);
                break;
            /* // the z coordinate */ 
            case 'z': 
                lidardouble2string(printstring, LASPoint_GetZ(p)); fprintf(file_out, printstring);
                break;
            /* // the gps-time */
            case 't': 
                lidardouble2string(printstring,LASPoint_GetTime(p)); fprintf(file_out, printstring);
                break;
            /* // the intensity */
            case 'i': 
                fprintf(file_out, "%d", LASPoint_GetIntensity(p));
                break;
            /* the scan angle */
            case 'a':
                fprintf(file_out, "%d", LASPoint_GetScanAngleRank(p));
                break;
            /* the number of the return */
            case 'r': 
                fprintf(file_out, "%d", LASPoint_GetReturnNumber(p));
                break;
            /* the classification */
            case 'c': 
                fprintf(file_out, "%d", LASPoint_GetClassification(p));
                break;
            /* the user data */
            case 'u': 
                fprintf(file_out, "%d", LASPoint_GetUserData(p));
                break;
            /* the number of returns of given pulse */
            case 'n': 
                fprintf(file_out, "%d", LASPoint_GetNumberOfReturns(p));
                break;
/*
      case 'p': // the point source ID
        fprintf(file_out, "%d", lasreader->point.point_source_ID);
        break;
*/
   
            /* the edge of flight line flag */
            case 'e': 
                fprintf(file_out, "%d", LASPoint_GetFlightLineEdge(p));
                break;
            /* the direction of scan flag */
            case 'd': 
                fprintf(file_out, "%d", LASPoint_GetScanDirection(p));
                break;
            }
            i++;
            if (parse_string[i])
            {
                fprintf(file_out, "%c", separator_sign);
            }
            else
            {
                fprintf(file_out, "\012");
                break;
            }
        }

        p = LASReader_GetNextPoint(reader);
    }


    LASReader_Destroy(reader);
    LASHeader_Destroy(header);



    fclose(file_out);

    return 0;
}
