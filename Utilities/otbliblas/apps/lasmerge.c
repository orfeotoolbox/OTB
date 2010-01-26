/***************************************************************************
 * $Id: lasmerge.c 879 2008-09-25 18:26:15Z hobu $
 * $Date: 2008-09-25 13:26:15 -0500 (Thu, 25 Sep 2008) $
 *
 * Project: libLAS -- C/C++ read/write library for LAS LIDAR data
 * Purpose: LAS file merging
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

LASPointSummary* SummarizePoints(LASReaderH reader);
void print_point_summary(FILE *file, LASPointSummary* summary, LASHeaderH header);
void print_header(FILE *file, LASHeaderH header, const char* file_name, int bSkipVLR);

void usage()
{

    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"    lasmerge (version %s) usage:\n", LAS_GetVersion());
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Merge using a list of files:\n");
    fprintf(stderr,"  lasmerge -i lasfiles.txt -o out.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Manually list files to merge:\n");
    fprintf(stderr,"  lasmerge -i file1.las -i file2.las -i file3.las -o out.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Merge to stdout:\n");
    fprintf(stderr,"  lasmerge -i file1.las -i file2.las --stdout\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Merge and set scale:\n");
    fprintf(stderr, "  lasmerge --input lasfiles.txt --scale 0.01 --output out.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Merge and set scale individually:\n");
    fprintf(stderr, "  lasmerge --input lasfiles.txt --xyz_scale 0.01 0.01 0.1 --output out.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Merge and set offset individually:\n");
    fprintf(stderr, "  lasmerge --input lasfiles.txt --xyz_offset 0.01 0.01 0.1 --output out.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Print this output:\n");
    fprintf(stderr, "  lasmerge --help\n");
    fprintf(stderr,"\n");
    
    fprintf(stderr, "For more information, see the full documentation for lasmerge at:\n"
                    " http://liblas.org/browser/trunk/doc/lasmerge.txt\n");
    fprintf(stderr,"----------------------------------------------------------\n");

}

void ptime(const char *const msg)
{
    float t= ((float)clock())/CLOCKS_PER_SEC;
    fprintf(stderr, "cumulative CPU time thru %s = %f\n", msg, t);
}

int main(int argc, char *argv[])
{
    int i;
    int verbose = FALSE;
    int skip_vlr = FALSE;
    int num_file_name_in = 0;
    int alloced_file_name_in = 32;
    char** file_names_in = (char**)malloc(sizeof(char*)*alloced_file_name_in);
    char* file_name_out = 0;
    int use_stdout = FALSE;
    double xyz_scale[3] = {0.0, 0.0, 0.0};
    double xyz_offset[3] = {0.0, 0.0, 0.0};

    double minx, miny, maxx, maxy, minz, maxz;
    double x_scale_factor, y_scale_factor, z_scale_factor;

  
    LASHeaderH merged_header=NULL;
    LASHeaderH header = NULL;
    LASReaderH reader = NULL;
    LASWriterH writer = NULL;
    LASPointH p = NULL;
  
    FILE* file = NULL;

    int smallest_int = (1<<31)+10;
    int largest_int = smallest_int-1-20;

    int same = TRUE;
    LASPointSummary* summary = NULL;

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
        else if (   strcmp(argv[i],"--input") == 0  ||
                    strcmp(argv[i],"-input") == 0   ||
                    strcmp(argv[i],"-i") == 0       ||
                    strcmp(argv[i],"-in") == 0
                )
        {
            i++;
            if (num_file_name_in == alloced_file_name_in)
            {
                alloced_file_name_in *= 2;
                file_names_in = (char**)realloc(file_names_in,sizeof(char*)*alloced_file_name_in);
            }
            file_names_in[num_file_name_in] = argv[i];
            num_file_name_in++;
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
        else if (   strcmp(argv[i],"--scale") == 0  ||
                    strcmp(argv[i],"-scale") == 0     ||
                    strcmp(argv[i],"-s") == 0       
                )
        {

            i++;
            sscanf(argv[i], "%lf", &(xyz_scale[2]));
            xyz_scale[0] = xyz_scale[1] = xyz_scale[2];
        }
        else if (   strcmp(argv[i],"--xyz_scale") == 0  ||
                    strcmp(argv[i],"-xyz_scale") == 0    
                )

        {
            i++;
            sscanf(argv[i], "%lf", &(xyz_scale[0]));
            i++;
            sscanf(argv[i], "%lf", &(xyz_scale[1]));
            i++;
            sscanf(argv[i], "%lf", &(xyz_scale[2]));
        }
        else if (   strcmp(argv[i],"--xyz_offset") == 0  ||
                    strcmp(argv[i],"-xyz_offset") == 0    
                )
        {
            i++;
            sscanf(argv[i], "%lf", &(xyz_offset[0]));
            i++;
            sscanf(argv[i], "%lf", &(xyz_offset[1]));
            i++;
            sscanf(argv[i], "%lf", &(xyz_offset[2]));
        }
        else if (   strcmp(argv[i],"--stdout") == 0  ||
                    strcmp(argv[i],"-olas") == 0    
                )
        {
            use_stdout = TRUE;
            file_name_out = "stdout";
        }
        else if (   strcmp(argv[i],"--skip_vlr") == 0   ||
                    strcmp(argv[i],"--no_vlr") == 0)
        {
            skip_vlr = TRUE;
        } 
        else if (i == argc - 2 && num_file_name_in == 0 && file_name_out == 0)
        {
            file_names_in[0] = argv[i];
            num_file_name_in = 1;
        }
        else if (i == argc - 1 && num_file_name_in == 0 && file_name_out == 0)
        {
            file_names_in[0] = argv[i];
            num_file_name_in = 1;
        }
        else if (i == argc - 1 && num_file_name_in != 0 && file_name_out == 0)
        {
            file_name_out = argv[i];
        }
        else 
        {
            fprintf(stderr, "ERROR: unknown argument '%s'\n",argv[i]);
            usage();
            exit(1);
        }
    }

    if (num_file_name_in == 0)
    {
        fprintf(stderr, "ERROR: no input specified\n");
        usage();
        exit(1);
    }

    if (    num_file_name_in == 1 && 
            strstr(file_names_in[0],".las") == 0 && 
            strstr(file_names_in[0],".LAS") == 0
       )
    {
        char line[512];

        num_file_name_in = 0;
        file = fopen(file_names_in[0], "r");
        while (fgets(line, sizeof(char) * 512, file))
            {
                if (strstr(line,".las") || strstr(line,".LAS") )
                {
                    if (num_file_name_in == alloced_file_name_in)
                    {
                        alloced_file_name_in *= 2;
                        file_names_in = (char**)realloc(file_names_in,sizeof(char*)*alloced_file_name_in);
                    }
                    file_names_in[num_file_name_in] = strdup(line);
                    i = (int)strlen(file_names_in[num_file_name_in]) - 1;
                    while ( i && 
                            file_names_in[num_file_name_in][i] != 's' && 
                            file_names_in[num_file_name_in][i] != 'S' && 
                            file_names_in[num_file_name_in][i] != 'z' && 
                            file_names_in[num_file_name_in][i] != 'Z') 
                    i--;
                    if (i)
                    {
                        file_names_in[num_file_name_in][i+1] = '\0';
                        num_file_name_in++;
                    }
                    else
                    {
                        fprintf(stderr, "WARNING: cannot parse line '%s'\n",line);
                    }
                }
                else
                {
                    fprintf(stderr, "WARNING: no a valid LAS file name '%s'\n",line);
                }
            }
    }


    if (verbose) ptime("starting first pass.");
    fprintf(stderr, "first pass ... reading headers of %d LAS files...\n", num_file_name_in);

    for (i = 0; i < num_file_name_in; i++)
    {
        reader = LASReader_Create(file_names_in[i]);
        if (!reader) { 
                LASError_Print("Could not get LASReader "); 
                exit(1);
            } 
      
            header = LASReader_GetHeader(reader);
            if (!header) { 
                LASError_Print("Could not get LASHeader "); 
                exit(1);
            } 


        if (i == 0)
        {
            merged_header = LASReader_GetHeader(reader);
        }
        else
        {
            LASHeader_SetPointRecordsCount( merged_header, 
                                            LASHeader_GetPointRecordsCount(merged_header) + 
                                            LASHeader_GetPointRecordsCount(header));
            LASHeader_SetPointRecordsByReturnCount( merged_header, 
                                                    0, 
                                                    LASHeader_GetPointRecordsByReturnCount(merged_header,0) + 
                                                    LASHeader_GetPointRecordsByReturnCount(header,0));
            LASHeader_SetPointRecordsByReturnCount( merged_header, 
                                                    1, 
                                                    LASHeader_GetPointRecordsByReturnCount(merged_header,1) + 
                                                    LASHeader_GetPointRecordsByReturnCount(header,1));
            LASHeader_SetPointRecordsByReturnCount( merged_header, 
                                                    2, 
                                                    LASHeader_GetPointRecordsByReturnCount(merged_header,2) + 
                                                    LASHeader_GetPointRecordsByReturnCount(header,2));
            LASHeader_SetPointRecordsByReturnCount( merged_header, 
                                                    3, 
                                                    LASHeader_GetPointRecordsByReturnCount(merged_header,3) + 
                                                    LASHeader_GetPointRecordsByReturnCount(header,3));
            LASHeader_SetPointRecordsByReturnCount( merged_header, 
                                                    4, 
                                                    LASHeader_GetPointRecordsByReturnCount(merged_header,4) + 
                                                    LASHeader_GetPointRecordsByReturnCount(header,4));

            LASHeader_SetSystemId(merged_header, "MERGED");

            minx = LASHeader_GetMinX(merged_header);
            maxx = LASHeader_GetMaxX(merged_header);
            miny = LASHeader_GetMinY(merged_header);
            maxy = LASHeader_GetMaxY(merged_header);
            minz = LASHeader_GetMinZ(merged_header);
            maxz = LASHeader_GetMaxZ(merged_header);
        
            if (minx > LASHeader_GetMinX(header)) minx = LASHeader_GetMinX(header);
            if (maxx < LASHeader_GetMaxX(header)) maxx = LASHeader_GetMaxX(header);
            if (miny > LASHeader_GetMinY(header)) miny = LASHeader_GetMinY(header);
            if (maxy < LASHeader_GetMaxY(header)) maxy = LASHeader_GetMaxY(header);
            if (minz > LASHeader_GetMinZ(header)) minz = LASHeader_GetMinZ(header);
            if (maxz < LASHeader_GetMaxZ(header)) maxz = LASHeader_GetMaxZ(header);
        
            LASHeader_SetMin(merged_header, minx, miny, minz);
            LASHeader_SetMax(merged_header, maxx, maxy, maxz);
        }
        
        LASHeader_Destroy(header);
        LASReader_Destroy(reader);
    }

    if (verbose)
    {
        fprintf(stderr, "  number_of_point_records %d\n", 
                LASHeader_GetPointRecordsCount(merged_header));
        fprintf(stderr, "  number of points by return %d %d %d %d %d\n", 
                        LASHeader_GetPointRecordsByReturnCount(merged_header, 0),
                        LASHeader_GetPointRecordsByReturnCount(merged_header, 1), 
                        LASHeader_GetPointRecordsByReturnCount(merged_header, 2), 
                        LASHeader_GetPointRecordsByReturnCount(merged_header, 3), 
                        LASHeader_GetPointRecordsByReturnCount(merged_header, 4));
        fprintf(stderr, "  min x y z                  %.6f %.6f %.6f\n", 
                        LASHeader_GetMinX(merged_header), 
                        LASHeader_GetMinY(merged_header), 
                        LASHeader_GetMinZ(merged_header));
        fprintf(stderr, "  max x y z                  %.6f %.6f %.6f\n", 
                        LASHeader_GetMaxX(merged_header), 
                        LASHeader_GetMaxY(merged_header), 
                        LASHeader_GetMaxZ(merged_header));
    }

    if (xyz_scale)
    {
        LASHeader_SetScale( merged_header, 
                            xyz_scale[0], 
                            xyz_scale[1], 
                            xyz_scale[2]);
    }

    if (xyz_offset)
    {
        LASHeader_SetOffset(merged_header, 
                            xyz_offset[0], 
                            xyz_offset[1], 
                            xyz_offset[2] );
    }

    x_scale_factor = LASHeader_GetScaleX(merged_header);
    if (((LASHeader_GetMaxX(merged_header) - LASHeader_GetOffsetX(merged_header)) / LASHeader_GetScaleX(merged_header)) > largest_int ||
        ((LASHeader_GetMinX(merged_header) - LASHeader_GetOffsetX(merged_header)) / LASHeader_GetScaleX(merged_header)) < smallest_int )
    {
        x_scale_factor = 0.0000001;
        while (((LASHeader_GetMaxX(merged_header) - LASHeader_GetOffsetX(merged_header)) / x_scale_factor) > largest_int ||
            ((LASHeader_GetMinX(merged_header) - LASHeader_GetOffsetX(merged_header)) / x_scale_factor) < smallest_int )
        {
            x_scale_factor *= 10;
        }
        fprintf(stderr, "x_scale_factor of merged_header changed to %g\n", x_scale_factor);
    }

    y_scale_factor = LASHeader_GetScaleY(merged_header);
    if (((LASHeader_GetMaxY(merged_header) - LASHeader_GetOffsetY(merged_header)) / y_scale_factor) > largest_int ||
        ((LASHeader_GetMinY(merged_header) - LASHeader_GetOffsetY(merged_header)) / y_scale_factor) < smallest_int )
    {
        y_scale_factor = 0.0000001;
        while (((LASHeader_GetMaxY(merged_header) - LASHeader_GetOffsetY(merged_header)) / y_scale_factor) > largest_int ||
            ((LASHeader_GetMinY(merged_header) - LASHeader_GetOffsetY(merged_header)) / y_scale_factor) < smallest_int )
        {
            y_scale_factor *= 10;
        }
        fprintf(stderr, "y_scale_factor of merged_header changed to %g\n", y_scale_factor);
    }
  
    z_scale_factor = LASHeader_GetScaleZ(merged_header);
    if (((LASHeader_GetMaxZ(merged_header) - LASHeader_GetOffsetZ(merged_header)) / z_scale_factor) > largest_int ||
        ((LASHeader_GetMinZ(merged_header) - LASHeader_GetOffsetZ(merged_header)) / z_scale_factor) < smallest_int )
    {
        z_scale_factor = 0.0000001;
        while (((LASHeader_GetMaxZ(merged_header) - LASHeader_GetOffsetZ(merged_header)) / z_scale_factor) > largest_int ||
            ((LASHeader_GetMinZ(merged_header) - LASHeader_GetOffsetZ(merged_header)) / z_scale_factor) < smallest_int )\
        {
            z_scale_factor *= 10;
        }
        fprintf(stderr, "z_scale_factor of merged_header changed to %g\n", z_scale_factor);
    }
  
    LASHeader_SetScale( merged_header, 
                        x_scale_factor, 
                        y_scale_factor, 
                        z_scale_factor);

    if (file_name_out == NULL && !use_stdout)
    {
        LASError_Print("no output was specified "); 
        exit(1);
    }

    writer = LASWriter_Create(file_name_out, merged_header, LAS_MODE_WRITE);
    if (!writer) { 
        LASError_Print("Could not create LASWriter "); 
        exit(1);
    } 

    if (verbose) ptime("starting second pass.");
    fprintf(stderr, "second pass ... merge %d LAS files into one ...\n", num_file_name_in);

    for (i = 0; i < num_file_name_in; i++)
    {
        reader = LASReader_Create(file_names_in[i]);
        if (!reader) { 
            LASError_Print("Could not get LASReader ");
            exit(1);
        } 
        header = LASReader_GetHeader(reader);
        if (!header) { 
            LASError_Print("Could not get LASHeader ");
            exit(1);
        } 

        same = TRUE;
        if (LASHeader_GetOffsetX(merged_header) != LASHeader_GetOffsetX(header)) same = FALSE;
        if (LASHeader_GetOffsetY(merged_header) != LASHeader_GetOffsetY(header)) same = FALSE;
        if (LASHeader_GetOffsetZ(merged_header) != LASHeader_GetOffsetZ(header)) same = FALSE;

        if (LASHeader_GetScaleX(merged_header) != LASHeader_GetScaleX(header)) same = FALSE;
        if (LASHeader_GetScaleY(merged_header) != LASHeader_GetScaleY(header)) same = FALSE;
        if (LASHeader_GetScaleZ(merged_header) != LASHeader_GetScaleZ(header)) same = FALSE;
    

        if (same)
        {
            p = LASReader_GetNextPoint(reader);
            while (p)
            {
                LASWriter_WritePoint(writer, p);
                p = LASReader_GetNextPoint(reader);
            }
        }
        else
        {
            p = LASReader_GetNextPoint(reader);
            while (p)
            {
                LASPoint_SetX(p,(0.5 + (LASPoint_GetX(p) - LASHeader_GetOffsetX(merged_header)) / LASHeader_GetScaleX(merged_header)));
                LASPoint_SetY(p,(0.5 + (LASPoint_GetY(p) - LASHeader_GetOffsetY(merged_header)) / LASHeader_GetScaleY(merged_header)));
                LASPoint_SetZ(p,(0.5 + (LASPoint_GetZ(p) - LASHeader_GetOffsetZ(merged_header)) / LASHeader_GetScaleZ(merged_header)));
                LASWriter_WritePoint(writer, p);
                p = LASReader_GetNextPoint(reader);
            }
        }

        LASHeader_Destroy(header);
        LASReader_Destroy(reader);
        reader = NULL;
    }
    
    LASHeader_Destroy(merged_header);
    LASWriter_Destroy(writer);

    if (verbose) ptime("done.");

    if (verbose) {
        reader = LASReader_Create(file_name_out);
        if (!reader) { 
            LASError_Print("Could not open file ");
            exit(1);
        } 
      
        header = LASReader_GetHeader(reader);
        if (!header) { 
            LASError_Print("Could not get LASHeader ");
            exit(1);
        } 

        print_header(stderr, header, file_name_out, skip_vlr);        
        summary = SummarizePoints(reader);
        print_point_summary(stderr, summary, header);
        
        LASHeader_Destroy(header);
        header = NULL;            
        LASReader_Destroy(reader);
        reader = NULL;
        
        LASPoint_Destroy(summary->pmin);
        LASPoint_Destroy(summary->pmax);
        free(summary);
    }
    
    return 0;
}
