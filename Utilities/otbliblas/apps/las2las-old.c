/***************************************************************************
 * $Id$
 * $Date$
 *
 * Project: libLAS -- C/C++ read/write library for LAS LIDAR data
 * Purpose: LAS translation with optional configuration
 * Author:  Martin Isenburg isenburg@cs.unc.edu 
 ***************************************************************************
 * Copyright (c) 2007, Martin Isenburg isenburg@cs.unc.edu 
 *
 * See LICENSE.txt in this source distribution for more information.
 **************************************************************************/

#include "liblas.h"
/* std */
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define compare_no_case(a,b,n)  _strnicmp( (a), (b), (n) )
#else
#define compare_no_case(a,b,n)  strncasecmp( (a), (b), (n) )
#endif

LASPointSummary* SummarizePoints(LASReaderH reader);
void print_point_summary(FILE *file, LASPointSummary* summary, LASHeaderH header);
void print_point(FILE *file, LASPointH point);
void print_header(FILE *file, LASHeaderH header, const char* file_name, int bSkipVLR);
void repair_header(FILE *file, LASHeaderH header, LASPointSummary* summary) ;

#define LAS_FORMAT_10 0
#define LAS_FORMAT_11 1
#define LAS_FORMAT_12 2

void do_bulk_copy(const char* infile, size_t in_start_point, const char* outfile)
{
    /* bulk copy assumes that the header has already been written to outfile 
       as it is supposed to be, and that we're just going to copy all of the 
       points in infile as they are.  
    */
    FILE* file_out = 0;
    FILE* file_in = 0;
    
    size_t read = 0;
    size_t written = 0;
    size_t size = 1000;

    char *buffer = 0;
    
    buffer = (char*) malloc(size * sizeof(char));
    
    if (buffer == 0) {
        LASError_Print("unable to allocate buffer copy");
        exit(1);
    }
    file_in = fopen(infile, "rb");
    fseek(file_in, in_start_point, SEEK_SET);
    
    if (file_in == 0) {
        LASError_Print("input filename not valid for bulk copy");
        exit(1);
    }
    file_out = fopen(outfile, "ab+");
    if (file_out == 0) {
        LASError_Print("output filename not valid for bulk copy");
        exit(1);
    }
    
    while (feof(file_in) == 0) {
        read = fread(buffer, 1, size, file_in);
        written = fwrite(buffer, 1, read, file_out);
        
        if (read != written) {
            LASError_Print("unable to write data in bulk copy");
            exit(1);
        }
    }
    
    fclose(file_in);
    fclose(file_out);
    free(buffer);
}


void usage()
{
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"    las2las-old (version %s) usage:\n", LAS_GetFullVersion());
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Clip las file to a bounding box, throwing out points that have invalid data:\n");
    fprintf(stderr,"  las2las-old -i in.las -clip 63000000 483450000 63050000 483500000 -o out.las --skip_invalid\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Read from stdin, eliminate intensities below 2000, and write to stdout:\n");
    fprintf(stderr,"  las2las-old --eliminate_intensity_below 2000 --stdin --stdout < in.las > out.las \n");
    fprintf(stderr,"\n");
    
    fprintf(stderr,"Eliminate return number two:\n");
    fprintf(stderr,"  las2las-old -i in.las -eliminate_return 2 -o out.las\n");
    fprintf(stderr,"\n");
    
    fprintf(stderr,"Eliminate scan angles above 15:\n");
    fprintf(stderr,"  las2las-old -i in.las -eliminate_scan_angle_above 15 -o out.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Eliminate intensities below 1000:\n");
    fprintf(stderr,"  las2las-old -i in.las -eliminate_intensity_below 1000 --stdout > out.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Eliminate intensities below 1000 and classifications that equal 2 (ground):\n");
    fprintf(stderr,"  las2las-old -i in.las --eliminate_class 2 --out out.las\n");
    fprintf(stderr,"\n");
        
    fprintf(stderr,"Capture only first returns and clip to bounding box:\n");
    fprintf(stderr,"  las2las-old -i in.las -first_only -clip 63000000 483450000 63050000 483500000 -o out.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Capture only last returns, eliminate intensities below 2000, and write to stdout:\n");
    fprintf(stderr,"  las2las-old --input in.las --last_only --eliminate_intensity_below 2000 --stdout > out.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Reproject the data (requires GDAL support):\n");
    fprintf(stderr,"  las2las-old -v in.las output.las -xyz_offset 0 0 0 "
                   "-t_srs EPSG:4326 --xyz_scale 0.0000001 0.0000001 0.0000001"
                   "-s_srs EPSG:26915\n");
    fprintf(stderr,"\n");

    fprintf(stderr, "\nFor more information, see the full documentation for las2las-old at:\n"
                    " http://liblas.org/utilities/las2las-old.html\n");
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
    int use_stdin = FALSE;
    int use_stdout = FALSE;
    char* file_name_in = 0;
    char* file_name_out = 0;
    char* proj4_text = NULL;
    double *clip_xy_min = NULL;
    double *clip_xy_max = NULL;
    int clip = FALSE;
    int remove_extra_header = FALSE;
    int elim_return = 0;
    int elim_scan_angle_above = 0;
    int elim_intensity_below = 0;
    int elim_class = 0;
    int clsidx = 0;
    int first_only = FALSE;
    int last_only = FALSE;
    int skip_invalid = FALSE;
    int format = LAS_FORMAT_12;
    int bulk_copy = FALSE;
    
    LASReaderH reader = NULL;
    LASHeaderH header = NULL;
    LASHeaderH surviving_header = NULL;
    LASPointH p = NULL;
    LASWriterH writer = NULL;
    
    LASSRSH in_srs = NULL;
    LASSRSH out_srs = NULL;
    int use_min_offset = FALSE;
    int do_reprojection = FALSE;
    int do_set_offset = FALSE;
    int do_set_scale = FALSE;
    int do_pad_header = FALSE;
    
    int header_pad = 0;
    
    int first_surviving_point = TRUE;
    unsigned int surviving_number_of_point_records = 0;
    unsigned int surviving_number_of_points_by_return[] = {0,0,0,0,0,0,0,0};
    LASPointH surviving_point_min = NULL;
    LASPointH surviving_point_max = NULL;
    double surviving_gps_time_min;
    double surviving_gps_time_max;
    int verticalCSType = -1, verticalDatum = -1, verticalUnits = 9001;
    const char *verticalCitation = "";

    int clipped = 0;
    int eliminated_return = 0;
    int eliminated_scan_angle = 0;
    int eliminated_intensity = 0;
    int eliminated_class = 0;
    int eliminated_first_only = 0;
    int eliminated_last_only = 0; 
    int thinned = 0;
    
    int bThin = FALSE;
    int nThin = 0;

    double xyz_scale[3] = {0.0, 0.0, 0.0};
    double xyz_offset[3] = {0.0, 0.0, 0.0};
    
    double minx, maxx, miny, maxy, minz, maxz;
    
    LASPointSummary* summary = NULL;
    
    int ret = 0;
    
    for (i = 1; i < argc; i++) {
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
        else if (   strcmp(argv[i],"-b") == 0 ||
                    strcmp(argv[i],"--bulk") == 0
            )
        {
            bulk_copy = TRUE;
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

        else if (   strcmp(argv[i],"--stdout") == 0  ||
                    strcmp(argv[i],"-olas") == 0    
                )
        {
            use_stdout = TRUE;
            file_name_out = "stdout";
        }
        else if (   strcmp(argv[i],"--clip") == 0   ||
                    strcmp(argv[i],"-clip") == 0    ||
                    strcmp(argv[i],"-clip_xy") == 0 
                )
        {
            clip_xy_min = (double*) malloc (2 * sizeof(double));
            clip_xy_max = (double*) malloc( 2 * sizeof(double));
            i++;
            clip_xy_min[0] = atof(argv[i]);
            i++;
            clip_xy_min[1] = atof(argv[i]);
            i++;
            clip_xy_max[0] = atof(argv[i]);
            i++;
            clip_xy_max[1] = atof(argv[i]);
            clip = TRUE;
        }
        else if (   strcmp(argv[i],"--format") == 0   ||
                    strcmp(argv[i],"-f") == 0    ||
                    strcmp(argv[i],"-format") == 0 
                )
        {
            i++;
            if (strcmp(argv[i], "1.0") == 0) {
                format = LAS_FORMAT_10;
            }
            else if (strcmp(argv[i], "1.1") == 0) {
                format = LAS_FORMAT_11;
            } 
            else if (strcmp(argv[i], "1.2") == 0) {
                format = LAS_FORMAT_12;
            }
            else {
                LASError_Print("Format must be specified as 1.0, 1.1, or 1.2");
            }

        }

        else if (   strcmp(argv[i],"--eliminate_return") == 0  ||
                    strcmp(argv[i],"-eliminate_return") == 0   ||
                    strcmp(argv[i],"-elim_return") == 0       ||
                    strcmp(argv[i],"-elim_ret") == 0
                )        
        {
            i++;
            elim_return |= (1 << atoi(argv[i]));
        }
        else if (   strcmp(argv[i],"--eliminate_scan_angle_above") == 0  ||
                    strcmp(argv[i],"-eliminate_scan_angle_above") == 0   ||
                    strcmp(argv[i],"-elim_scan_angle_above") == 0       ||
                    strcmp(argv[i],"--clip_scan_above") == 0
                )        
        {
            i++;
            elim_scan_angle_above = atoi(argv[i]);
        }
        else if (   strcmp(argv[i],"--eliminate_class") == 0  ||
                    strcmp(argv[i],"-eliminate_class") == 0   ||
                    strcmp(argv[i],"-elim_class") == 0       ||
                    strcmp(argv[i],"--class") == 0
                )        
        {
            i++;
            elim_class = atoi(argv[i]);
        }
        else if (   strcmp(argv[i],"--eliminate_intensity_below") == 0  ||
                    strcmp(argv[i],"-eliminate_intensity_below") == 0   ||
                    strcmp(argv[i],"-elim_intensity_below") == 0       ||
                    strcmp(argv[i],"--clip_intensity_below") == 0
                )        
        {
            i++;
            elim_intensity_below = atoi(argv[i]);
        }
        else if (   strcmp(argv[i], "--stdin") == 0 ||
                    strcmp(argv[i], "-ilas") == 0
                ) 
        {
            use_stdin = TRUE;
        }
        else if (   strcmp(argv[i], "--first_only") == 0 ||
                    strcmp(argv[i], "-first_only") == 0
                ) 
        {
            first_only = TRUE;
        }
        else if (   strcmp(argv[i], "--last_only") == 0 ||
                    strcmp(argv[i], "-last_only") == 0
                ) 
        {
            last_only = TRUE;
        }
        else if (   strcmp(argv[i], "--remove_extra_header") == 0 ||
                    strcmp(argv[i], "-remove_extra_header") == 0
                ) 
        {
            remove_extra_header = TRUE;
        }
        else if (   strcmp(argv[i],"--s_srs") == 0  ||
                    strcmp(argv[i],"-s_srs") == 0    
                )
        {
            ++i;
            if (LAS_IsGDALEnabled()) {
                in_srs = LASSRS_Create();
                ret = LASSRS_SetFromUserInput(in_srs, argv[i]);
                if (ret) {
                    LASError_Print("Unable to import assigned SRS");
                    exit(1);
                }
            }
        }
        else if (   strcmp(argv[i],"--t_srs") == 0  ||
                    strcmp(argv[i],"-t_srs") == 0    ||
                    strcmp(argv[i],"-t") == 0 
                )
        {
            ++i;
            if (LAS_IsGDALEnabled()) {
                out_srs = LASSRS_Create();
                ret = LASSRS_SetFromUserInput(out_srs, argv[i]);
                if (ret) {
                    LASError_Print("Unable to import output SRS");
                    exit(1);
                }
                do_reprojection = TRUE;
            }
        }
        else if (   strcmp(argv[i],"--a_srs") == 0  ||
                    strcmp(argv[i],"-a_srs") == 0 
                )
        {
            ++i;
            if (LAS_IsGDALEnabled()) {
                out_srs = LASSRS_Create();
                ret = LASSRS_SetFromUserInput(out_srs, argv[i]);
                if (ret) {
                    LASError_Print("Unable to import output SRS");
                    exit(1);
                }
            }
        }
        else if (   strcmp(argv[i],"--a_vertcs") == 0  ||
                    strcmp(argv[i],"-a_vertcs") == 0 
                )
        {
            ++i;
            verticalCSType = atoi(argv[i]);
            ++i;
            if( i < argc && argv[i][0] != '-' )
            {
                verticalCitation = argv[i];
                ++i;

                if( i < argc && argv[i][0] != '-' )
                {
                    verticalDatum = atoi(argv[i]);
                    ++i;
                    if( i < argc && argv[i][0] != '-' )
                    {
                        verticalUnits = atoi(argv[i]);
                        ++i;
                    }
                }
            }
        }
        else if (   strcmp(argv[i],"--scale") == 0  ||
                    strcmp(argv[i],"-scale") == 0    
                )
        {

            i++;
            sscanf(argv[i], "%lf", &(xyz_scale[2]));
            xyz_scale[0] = xyz_scale[1] = xyz_scale[2];
            do_set_scale = TRUE;
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
            do_set_scale = TRUE;
        }
        else if (   strcmp(argv[i],"--xyz_offset") == 0  ||
                    strcmp(argv[i],"-xyz_offset") == 0    
                )
        {
            i++;
            if (!compare_no_case(argv[i], "min", 3)) {
                use_min_offset = TRUE;
                do_set_offset = TRUE;
            } else
            {
                sscanf(argv[i], "%lf", &(xyz_offset[0]));
                i++;
                sscanf(argv[i], "%lf", &(xyz_offset[1]));
                i++;
                sscanf(argv[i], "%lf", &(xyz_offset[2]));
                do_set_offset = TRUE;
            }
        }
        else if (   strcmp(argv[i],"--pad-header") == 0  ||
                    strcmp(argv[i],"-pad-header") == 0   ||
                    strcmp(argv[i],"-ph") == 0 
                )        
        {
            i++;
            do_pad_header = TRUE;
            header_pad = atoi(argv[i]);
        }
        else if (   strcmp(argv[i],"--thin") == 0  ||
                    strcmp(argv[i],"-y") == 0    
                )
        {
            i++;
            bThin = TRUE;
            nThin = atoi(argv[i]);
        }
        else if (file_name_in == NULL && file_name_out == NULL)
        {
            file_name_in = argv[i];
        }
        else if (file_name_in != NULL && file_name_out == NULL)
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

    if (use_stdin) file_name_in="stdin";
    if (file_name_in)
    {
        reader = LASReader_Create(file_name_in);
        if (!reader) { 
            LASError_Print("Could not open file to read");
            exit(1);
        }
    }
    else
    {
        LASError_Print("no input specified");
        usage();
        exit(1);
    }

        
    header = LASReader_GetHeader(reader);
    if (!header) { 
        LASError_Print("Could not fetch header");
        exit(1);
    } 

    if (verbose) {
        ptime("start.");
        fprintf(stderr, "first pass reading %d points ...\n", LASHeader_GetPointRecordsCount(header));
    }

    p  = LASReader_GetNextPoint(reader);

    if (!p) {
        if (LASError_GetLastErrorNum()) 
            LASError_Print("Not able to fetch a point.  LASReader is invalid");
        else
            LASError_Print("File does not contain any points to read.");
        exit(1);
    }
    
    i = 0;
    while (p)
    {
        if (bThin && ((i % nThin) != 0)) {
            thinned++;
            i++;
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (last_only && LASPoint_GetReturnNumber(p) != LASPoint_GetNumberOfReturns(p))
        {
            eliminated_last_only++;
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (first_only && LASPoint_GetReturnNumber(p) != 1)
        {
            eliminated_first_only++;
            p = LASReader_GetNextPoint(reader);
            continue;
        }
    
        if (clip && (LASPoint_GetX(p) < clip_xy_min[0] || LASPoint_GetY(p) < clip_xy_min[1]))
        {
            clipped++;
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (clip && (LASPoint_GetX(p) > clip_xy_max[0] || LASPoint_GetY(p) > clip_xy_max[1]))
        {
            clipped++;
            p = LASReader_GetNextPoint(reader);
            continue;
        } 
        if (elim_return && (elim_return & (1 << LASPoint_GetReturnNumber(p))))
        {
            eliminated_return++;
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (elim_scan_angle_above && (LASPoint_GetScanAngleRank(p) > elim_scan_angle_above || LASPoint_GetScanAngleRank(p) < -elim_scan_angle_above))
        {
            eliminated_scan_angle++;
            p = LASReader_GetNextPoint(reader);
            continue;
        }

        clsidx = LASPoint_GetClassification(p);
        clsidx = (clsidx & 31); /* 31 is max index in classification lookup table */
        assert(clsidx <= 31);
        if (elim_class && (elim_class == clsidx))
        {
            eliminated_class++;
            p = LASReader_GetNextPoint(reader);
            continue;
        }

        if (elim_intensity_below && LASPoint_GetIntensity(p) < elim_intensity_below)
        {
            eliminated_intensity++;
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        surviving_number_of_point_records++;

        if (LASPoint_GetReturnNumber(p))
            surviving_number_of_points_by_return[LASPoint_GetReturnNumber(p)-1]++;
        else
            surviving_number_of_points_by_return[LASPoint_GetReturnNumber(p)]++;        

        if (first_surviving_point)
        {
            surviving_point_min = LASPoint_Copy(p);
            surviving_point_max = LASPoint_Copy(p);
            surviving_gps_time_min = LASPoint_GetTime(p);
            surviving_gps_time_max = LASPoint_GetTime(p);
            first_surviving_point = FALSE;
        }
        else
        {
            if (LASPoint_GetX(p) < LASPoint_GetX(surviving_point_min))
                LASPoint_SetX(surviving_point_min,LASPoint_GetX(p));
            else if (LASPoint_GetX(p) > LASPoint_GetX(surviving_point_max)) 
                LASPoint_SetX(surviving_point_max,LASPoint_GetX(p));

            if (LASPoint_GetY(p) < LASPoint_GetY(surviving_point_min)) 
                LASPoint_SetY(surviving_point_min,LASPoint_GetY(p));
            else if (LASPoint_GetY(p) > LASPoint_GetY(surviving_point_max)) 
                LASPoint_SetY(surviving_point_max,LASPoint_GetY(p));

            if (LASPoint_GetZ(p) < LASPoint_GetZ(surviving_point_min))
                LASPoint_SetZ(surviving_point_min,LASPoint_GetZ(p));
            else if (LASPoint_GetZ(p) > LASPoint_GetZ(surviving_point_max))   
                LASPoint_SetZ(surviving_point_max,LASPoint_GetZ(p));

            if (LASPoint_GetIntensity(p) < LASPoint_GetIntensity(surviving_point_min)) 
                LASPoint_SetIntensity(surviving_point_min,LASPoint_GetIntensity(p));
            else if (LASPoint_GetIntensity(p) > LASPoint_GetIntensity(surviving_point_max)) 
                LASPoint_SetIntensity(surviving_point_max,LASPoint_GetIntensity(p));

            if (LASPoint_GetFlightLineEdge(p) < LASPoint_GetFlightLineEdge(surviving_point_min)) 
                LASPoint_SetFlightLineEdge(surviving_point_min,LASPoint_GetFlightLineEdge(p));
            else if (LASPoint_GetFlightLineEdge(p) > LASPoint_GetFlightLineEdge(surviving_point_max)) 
                LASPoint_SetFlightLineEdge(surviving_point_max,LASPoint_GetFlightLineEdge(p));

            if (LASPoint_GetScanDirection(p) < LASPoint_GetScanDirection(surviving_point_min)) 
                LASPoint_SetScanDirection(surviving_point_min,LASPoint_GetScanDirection(p));
            else if (LASPoint_GetScanDirection(p) > LASPoint_GetScanDirection(surviving_point_max)) 
                LASPoint_SetScanDirection(surviving_point_max,LASPoint_GetScanDirection(p));

            if (LASPoint_GetNumberOfReturns(p) < LASPoint_GetNumberOfReturns(surviving_point_min)) 
                LASPoint_SetNumberOfReturns(surviving_point_min,LASPoint_GetNumberOfReturns(p));
            else if (LASPoint_GetNumberOfReturns(p) > LASPoint_GetNumberOfReturns(surviving_point_max)) 
                LASPoint_SetNumberOfReturns(surviving_point_max,LASPoint_GetNumberOfReturns(p));

            if (LASPoint_GetReturnNumber(p) < LASPoint_GetReturnNumber(surviving_point_min)) 
                LASPoint_SetReturnNumber(surviving_point_min,LASPoint_GetReturnNumber(p));
            else if (LASPoint_GetReturnNumber(p) > LASPoint_GetReturnNumber(surviving_point_max)) 
                LASPoint_SetReturnNumber(surviving_point_max,LASPoint_GetReturnNumber(p));

            if (LASPoint_GetClassification(p) < LASPoint_GetClassification(surviving_point_min)) 
                LASPoint_SetClassification(surviving_point_min,LASPoint_GetClassification(p));
            else if (LASPoint_GetReturnNumber(p) > LASPoint_GetClassification(surviving_point_max)) 
                LASPoint_SetClassification(surviving_point_max,LASPoint_GetClassification(p));

            if (LASPoint_GetScanAngleRank(p) < LASPoint_GetScanAngleRank(surviving_point_min)) 
                LASPoint_SetScanAngleRank(surviving_point_min,LASPoint_GetScanAngleRank(p));
            else if (LASPoint_GetScanAngleRank(p) > LASPoint_GetScanAngleRank(surviving_point_max)) 
                LASPoint_SetScanAngleRank(surviving_point_max,LASPoint_GetScanAngleRank(p));

            if (LASPoint_GetUserData(p) < LASPoint_GetUserData(surviving_point_min)) 
                LASPoint_SetUserData(surviving_point_min,LASPoint_GetUserData(p));
            else if (LASPoint_GetUserData(p) > LASPoint_GetUserData(surviving_point_max)) 
                LASPoint_SetUserData(surviving_point_max,LASPoint_GetUserData(p));

            if (LASPoint_GetTime(p) < LASPoint_GetTime(surviving_point_min)) 
                LASPoint_SetTime(surviving_point_min,LASPoint_GetTime(p));
            else if (LASPoint_GetTime(p) > LASPoint_GetTime(surviving_point_max)) 
                LASPoint_SetTime(surviving_point_max,LASPoint_GetTime(p));

        }
  
        p  = LASReader_GetNextPoint(reader);
        i++;
    }

    if (eliminated_first_only) 
        fprintf(stderr, 
                "eliminated based on first returns only: %d\n",
                eliminated_first_only);
    if (eliminated_last_only) 
        fprintf(stderr, 
                "eliminated based on last returns only: %d\n", 
                eliminated_last_only);
    if (eliminated_class) 
        fprintf(stderr, 
                "eliminated classification: %d\n", 
                eliminated_class);
    if (clipped) 
        fprintf(stderr, 
                "clipped: %d\n", 
                clipped);
    if (eliminated_return) 
        fprintf(stderr, 
                "eliminated based on return number: %d\n", 
                eliminated_return);
    if (eliminated_scan_angle) 
        fprintf(stderr, 
                "eliminated based on scan angle: %d\n", 
                eliminated_scan_angle);
    if (eliminated_intensity) 
        fprintf(stderr, 
        "eliminated based on intensity: %d\n", 
        eliminated_intensity);

    if (bThin) 
        fprintf(stderr, 
        "thinned: %d\n", 
        thinned);
    
    if (surviving_number_of_point_records == 0) {
        fprintf(stderr, "All points were eliminated!\n");
        exit(0);
    }
    
    LASReader_Destroy(reader);
    LASHeader_Destroy(header);
  
    if (verbose)
    {
        fprintf(stderr, 
                "x %.3f %.3f %.3f\n",
                LASPoint_GetX(surviving_point_min), 
                LASPoint_GetX(surviving_point_max), 
                LASPoint_GetX(surviving_point_max) - LASPoint_GetX(surviving_point_min)
                );
        fprintf(stderr, 
                "y %.3f %.3f %.3f\n",
                LASPoint_GetY(surviving_point_min), 
                LASPoint_GetY(surviving_point_max), 
                LASPoint_GetY(surviving_point_max) - LASPoint_GetY(surviving_point_min)
                );
        fprintf(stderr, 
                "z %.3f %.3f %.3f\n",
                LASPoint_GetZ(surviving_point_min), 
                LASPoint_GetZ(surviving_point_max), 
                LASPoint_GetZ(surviving_point_max) - LASPoint_GetZ(surviving_point_min)
                );
        fprintf(stderr, 
                "intensity %d %d %d\n",
                LASPoint_GetIntensity(surviving_point_min), 
                LASPoint_GetIntensity(surviving_point_max), 
                LASPoint_GetIntensity(surviving_point_max) - LASPoint_GetIntensity(surviving_point_min)
                );
        fprintf(stderr, 
                "edge_of_flight_line %d %d %d\n",
                LASPoint_GetFlightLineEdge(surviving_point_min), 
                LASPoint_GetFlightLineEdge(surviving_point_max), 
                LASPoint_GetFlightLineEdge(surviving_point_max) - LASPoint_GetFlightLineEdge(surviving_point_min)
                );
        fprintf(stderr, 
                "scan_direction_flag %d %d %d\n",
                LASPoint_GetScanDirection(surviving_point_min), 
                LASPoint_GetScanDirection(surviving_point_max), 
                LASPoint_GetScanDirection(surviving_point_max) - LASPoint_GetScanDirection(surviving_point_min)
                );
        fprintf(stderr, 
                "number_of_returns_of_given_pulse %d %d %d\n",
                LASPoint_GetNumberOfReturns(surviving_point_min), 
                LASPoint_GetNumberOfReturns(surviving_point_max), 
                LASPoint_GetNumberOfReturns(surviving_point_max) - LASPoint_GetNumberOfReturns(surviving_point_min)
                );
        fprintf(stderr, 
                "return_number %d %d %d\n",
                LASPoint_GetReturnNumber(surviving_point_min), 
                LASPoint_GetReturnNumber(surviving_point_max), 
                LASPoint_GetReturnNumber(surviving_point_max) - LASPoint_GetReturnNumber(surviving_point_min)
                );
        fprintf(stderr, 
                "classification %d %d %d\n",
                LASPoint_GetClassification(surviving_point_min), 
                LASPoint_GetClassification(surviving_point_max), 
                LASPoint_GetClassification(surviving_point_max) - LASPoint_GetClassification(surviving_point_min)
                );
        fprintf(stderr, 
                "scan_angle_rank %d %d %d\n",
                LASPoint_GetScanAngleRank(surviving_point_min), 
                LASPoint_GetScanAngleRank(surviving_point_max), 
                LASPoint_GetScanAngleRank(surviving_point_max) - LASPoint_GetScanAngleRank(surviving_point_min)
                );
        fprintf(stderr, 
                "user_data %d %d %d\n",
                LASPoint_GetUserData(surviving_point_min), 
                LASPoint_GetUserData(surviving_point_max), 
                LASPoint_GetUserData(surviving_point_max) - LASPoint_GetUserData(surviving_point_min)
                );
        fprintf(stderr, 
                "gps_time %.8f %.8f %.8f\n",
                LASPoint_GetTime(surviving_point_min), 
                LASPoint_GetTime(surviving_point_max), 
                LASPoint_GetTime(surviving_point_max) - LASPoint_GetTime(surviving_point_min)
                );

    }

    if (file_name_out == NULL && !use_stdout)
    {
        LASError_Print("no output specified. exiting...");
        exit(1);
    }


    fprintf(stderr, "Creating another reader...\n");
    if (file_name_in)
    {
        reader = LASReader_Create(file_name_in);
      if (!reader) { 
          LASError_Print("Could not open file to read");
          exit(1);
      } 
    }
    else
    {
        LASError_Print("no input specified");
        usage();
        exit(1);
    }

    
    header = LASReader_GetHeader(reader);
    if (!header) { 
        LASError_Print("Could not read header");
        exit(1);
    } 


    surviving_header = LASHeader_Copy(header);

    LASHeader_SetPointRecordsCount(surviving_header, surviving_number_of_point_records);
    LASHeader_SetSystemId(surviving_header, "MODIFICATION");

    for (i = 0; i < 5; i++) LASHeader_SetPointRecordsByReturnCount(surviving_header, i, surviving_number_of_points_by_return[i]);

    minx =  LASPoint_GetX(surviving_point_min) * \
            LASHeader_GetScaleX(surviving_header) + \
            LASHeader_GetOffsetX(surviving_header);
            
    maxx = LASPoint_GetX(surviving_point_max) * LASHeader_GetScaleX(surviving_header) + LASHeader_GetOffsetX(surviving_header);

    miny = LASPoint_GetY(surviving_point_min) * LASHeader_GetScaleY(surviving_header) + LASHeader_GetOffsetY(surviving_header);
    maxy = LASPoint_GetY(surviving_point_max) * LASHeader_GetScaleY(surviving_header) + LASHeader_GetOffsetY(surviving_header);

    minz = LASPoint_GetZ(surviving_point_min) * LASHeader_GetScaleZ(surviving_header) + LASHeader_GetOffsetZ(surviving_header);
    maxz = LASPoint_GetZ(surviving_point_max) * LASHeader_GetScaleZ(surviving_header) + LASHeader_GetOffsetZ(surviving_header);
    
    if (format == LAS_FORMAT_10) {
        LASHeader_SetVersionMinor(surviving_header, 0);
    } else if (format == LAS_FORMAT_11){
        LASHeader_SetVersionMinor(surviving_header, 1);
    } else if (format == LAS_FORMAT_12) {
        LASHeader_SetVersionMinor(surviving_header, 2);
    }

    if (do_set_offset) {
        if (use_min_offset) {
            if (verbose) {
                fprintf(stderr, 
                    "Setting xyz offset to minimums...\n");
            }
            LASHeader_SetOffset(surviving_header, 
                                LASPoint_GetX(surviving_point_min), 
                                LASPoint_GetY(surviving_point_min), 
                                LASPoint_GetZ(surviving_point_min));
        } else {
            if (verbose) {
                fprintf(stderr, 
                    "Setting xyz offset to commandline-assigned values...\n");
            }
            LASHeader_SetOffset(surviving_header,
                                xyz_offset[0],
                                xyz_offset[1],
                                xyz_offset[2]);
        }
    }

    if (do_set_scale) {
        if (verbose) {
            fprintf(stderr, 
                "Setting xyz scale...\n");
        }
        LASHeader_SetScale( surviving_header, 
                            xyz_scale[0],
                            xyz_scale[1],
                            xyz_scale[2]);
    }
    
    if (do_pad_header){
        if (verbose) {
            fprintf(stderr, 
                "Padding header by %d bytes. New header will be %d bytes long instead of %d bytes ...\n", 
                header_pad, 
                LASHeader_GetDataOffset(surviving_header)+abs(header_pad), 
                LASHeader_GetDataOffset(surviving_header));
        }
        
        LASHeader_SetDataOffset(surviving_header, LASHeader_GetDataOffset(surviving_header)+abs(header_pad));
    }
    
    /* Do we have vertical cs info to set? */
    if( verticalCSType > 0 )
    {
        if( out_srs == NULL )
            out_srs = LASHeader_GetSRS(surviving_header);

        if( out_srs == NULL )
            out_srs = LASSRS_Create();

        LASSRS_SetVerticalCS( out_srs,
                              verticalCSType, 
                              verticalCitation, 
                              verticalDatum, 
                              verticalUnits );
    }

    if (do_reprojection) {
        if (verbose) {
            proj4_text =  LASSRS_GetProj4(out_srs);
            fprintf(stderr,
                "Setting new coordinate system to %s", proj4_text);
            LASString_Free(proj4_text);
        }
        
        /* keep around the header's SRS if we don't have one set by the user */
        if (in_srs == NULL) {
            in_srs = LASHeader_GetSRS(surviving_header);
        }
        
        LASHeader_SetSRS(surviving_header, out_srs);
    }

    /* Are we just assigning an override SRS? (-a_srs) */
    else if( out_srs != NULL )
    {
        LASHeader_SetSRS(surviving_header, out_srs);
    }

    if (verbose) {
        fprintf(stderr, 
                "second pass reading %d and writing %d points ...\n", 
                LASHeader_GetPointRecordsCount(surviving_header), 
                surviving_number_of_point_records);
    }
    
    
    if (use_stdout) file_name_out = "stdout";
    
    writer = LASWriter_Create(  file_name_out, 
                                surviving_header, 
                                LAS_MODE_WRITE);
    if (!writer) { 
        LASError_Print("Could not open file to write");
        exit(1);
    }  

    if (do_reprojection) {
        if (in_srs != NULL) {
            char* in_wkt = LASSRS_GetWKT(in_srs);
            if (strlen(in_wkt) == 0) {
                LASString_Free(in_wkt);
                LASError_Print("Input SRS is empty, please specify with -s_srs");
                exit(1);
            }
            LASString_Free(in_wkt);
            LASWriter_SetInputSRS(writer, in_srs);
        } else {
            
        }
    
        if (out_srs != NULL) {
            if (verbose) {
                fprintf(stderr,
                    "Setting LASWriter_SetOutputSRS to %s", LASSRS_GetProj4(out_srs));
            }
            LASWriter_SetOutputSRS(writer, out_srs);
        }
    }
/*

  if (!remove_extra_header)
  {
    for (unsigned int u = lasreader->header.header_size; u < lasreader->header.offset_to_point_data; u++)
    {
      fputc(fgetc(file_in),file_out);
    }
  }
*/

    
    LASReader_Destroy(reader);
    reader = NULL;
    if (file_name_in)
    {
        reader = LASReader_Create(file_name_in);
        if (!reader) { 
            LASError_Print("Could not open file to read");
            exit(1);
        } 
    }
    else
    {
        LASError_Print("no input specified");
        usage();
        exit(1);
    }

    
    p = LASReader_GetNextPoint(reader);
    if (!p) {
        if (LASError_GetLastErrorNum()) 
            LASError_Print("Not able to fetch a point.  LASReader is invalid");
        else
            LASError_Print("File does not contain any points to read.");
        exit(1);
    }
    
    i = 0;
    while (p) {

        if (bThin && ((i % nThin) != 0)) {
            i++;
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        
        if (skip_invalid && !LASPoint_IsValid(p)) {
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (last_only && LASPoint_GetReturnNumber(p) != LASPoint_GetNumberOfReturns(p))
        {
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (first_only && LASPoint_GetReturnNumber(p) != 1)
        {
            p = LASReader_GetNextPoint(reader);
            continue;

        }
        if (clip_xy_min && (LASPoint_GetX(p) < clip_xy_min[0] || LASPoint_GetY(p) < clip_xy_min[1]))
        {
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (clip_xy_max && (LASPoint_GetX(p) > clip_xy_max[0] || LASPoint_GetY(p) > clip_xy_max[1]))
        {
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (elim_return && (elim_return & (1 << LASPoint_GetReturnNumber(p))))
        {
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (elim_scan_angle_above && (LASPoint_GetScanAngleRank(p) > elim_scan_angle_above || LASPoint_GetScanAngleRank(p) < -elim_scan_angle_above))
        {
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        if (elim_intensity_below && LASPoint_GetIntensity(p) < elim_intensity_below)
        {
            p = LASReader_GetNextPoint(reader);
            continue;
        }

        clsidx = LASPoint_GetClassification(p);
        clsidx = (clsidx & 31); /* 31 is max index in classification lookup table */
        assert(clsidx <= 31);
        if (elim_class && (elim_class == clsidx))
        {
            p = LASReader_GetNextPoint(reader);
            continue;
        }
        ret = LASWriter_WritePoint(writer,p);
        
        if (ret == LE_Warning) {
            LASError_Print("Unable to write invalid point.  Use --skip_invalid to avoid this problem (you will lose data in the translation)");
            exit(1);
        }
        p  = LASReader_GetNextPoint(reader);
        i++;
    }

    LASWriter_Destroy(writer);
    LASReader_Destroy(reader);
    LASHeader_Destroy(header);
    LASHeader_Destroy(surviving_header);

    if (surviving_point_max != NULL)
        LASPoint_Destroy(surviving_point_max);
    if (surviving_point_min != NULL)
        LASPoint_Destroy(surviving_point_min);

    reader = LASReader_Create(file_name_out);
    if (!reader) { 
        LASError_Print("Could not open file to read");
        exit(1);
    }

    header = LASReader_GetHeader(reader);
    if (!header) { 
        LASError_Print("Could not read header");
        exit(1);
    }

    
    summary = SummarizePoints(reader);
    if (verbose) {
        print_point_summary(stderr, summary, header);
    }
    repair_header(stderr, header, summary) ;

    if (summary != NULL) {
        if (summary->pmin != NULL)
            LASPoint_Destroy(summary->pmin);
        if (summary->pmax != NULL)
            LASPoint_Destroy(summary->pmax);
        free(summary);
    }

    if (reader != NULL) {
        LASReader_Destroy(reader);
        reader = NULL;
    }
    
    writer = LASWriter_Create(file_name_out, header, LAS_MODE_APPEND);
    if (!writer) {
        LASError_Print("Problem creating LASWriterH object for append");
        LASHeader_Destroy(header);
        header = NULL;
        exit(1);
    }
    LASWriter_Destroy(writer);
    writer = NULL;
    LASHeader_Destroy(header);
    header = NULL;
    
    if (out_srs != NULL)
        LASSRS_Destroy(out_srs);
    
    if (in_srs != NULL)
        LASSRS_Destroy(in_srs);

    if (verbose) ptime("done.");

    return 0;
}
