/***************************************************************************
* $Id$
* $Date$
*
* Project: libLAS -- C/C++ read/write library for LAS LIDAR data
* Purpose: LAS information reporting and setting
* Author:  Martin Isenburg isenburg@cs.unc.edu 
***************************************************************************
* Copyright (c) 2007, Martin Isenburg isenburg@cs.unc.edu 
*
* See LICENSE.txt in this source distribution for more information.
**************************************************************************/

/* TODO: Remove before releasing */
#if defined(_MSC_VER) && defined(USE_VLD)
#include <vld.h>
#endif

/* liblas */
#include <liblas.h>
/* std */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_GDAL
#include <cpl_multiproc.h>
#include <cpl_string.h>
#endif

LASPointSummary* SummarizePoints(LASReaderH reader);
void print_point_summary(FILE *file, LASPointSummary* summary, LASHeaderH header);
void print_header(FILE *file, LASHeaderH header, const char* file_name, int bSkipVLR, int bWKT);

void usage()
{
    char* version = LAS_GetFullVersion();
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"    lasinfo-old (version %s) usage:\n", version);
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"\n");
    LASString_Free(version);
    version = NULL;

    fprintf(stderr,"Printing simple header info:\n");
    fprintf(stderr,"  lasinfo-old lidar.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Checking point information:\n");
    fprintf(stderr,"  lasinfo-old --input lidar.las --check\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Print header info and point summary and redirect output to a file:\n");
    fprintf(stderr,"  lasinfo-old --check --input lidar.las 2> output.txt\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Read from stdin, scan the points, and skip printing VLR info:\n");
    fprintf(stderr,"  lasinfo-old --stdin --check --skip_vlr < lidar.las\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Repairing header info to reflect point data:\n");
    fprintf(stderr,"  lasinfo-old -i lidar.las --repair\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Altering header info:\n");
    fprintf(stderr, "  lasinfo-old -i lidar.las --system_identifier \"hello world!\" "
                    "--generating_software \"this is a test (-:\"  --file_creation 8 2007\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Print this output:\n");
    fprintf(stderr, "  lasinfo-old --help\n");
    fprintf(stderr,"\n");

    fprintf(stderr, "\nFor more information, see the full documentation for lasinfo-old at:\n"
                    " http://liblas.org/utilities/lasinfo-old.html\n");
    fprintf(stderr,"----------------------------------------------------------\n");
}

int main(int argc, char *argv[])
{
    int i = 0;
    char* file_name = NULL;

    LASReaderH reader = NULL;
    LASHeaderH header = NULL;
    LASWriterH writer = NULL;

    int check_points = FALSE;
    int repair_header = FALSE;
    int change_header = FALSE;
    int repair_bounding_box = FALSE;
    int use_stdin = FALSE;
    int update_return_counts = FALSE;
    int skip_vlr = FALSE;
    int wkt = FALSE;

    char *system_identifier = NULL;
    char *generating_software = NULL;
    unsigned char file_creation_day = 0;
    unsigned char file_creation_year = 0;

    int err = 0;

    LASPointSummary* summary = NULL;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"-v") == 0 || strcmp(argv[i],"--version") == 0)
        {
            char* ver = LAS_GetFullVersion();
            fprintf(stderr,"%s", ver);
            LASString_Free(ver);
            exit(0);
        }
        else if (strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0)
        {
            usage();
            exit(0);
        }
        else if (strcmp(argv[i],"--input") == 0
              || strcmp(argv[i],"-input") == 0
              || strcmp(argv[i],"-i") == 0
              || strcmp(argv[i],"-in") == 0)
        {
            i++;
            file_name = argv[i];
        }
        else if (strcmp(argv[i], "--points") == 0
              || strcmp(argv[i], "--check") == 0
              || strcmp(argv[i], "--check_points") == 0
              || strcmp(argv[i], "-c") == 0
              || strcmp(argv[i], "-points") == 0
              || strcmp(argv[i], "-check") == 0
              || strcmp(argv[i], "-check_points") == 0)
        {
            check_points = TRUE;
        }
        else if (strcmp(argv[i], "--nocheck") == 0
              || strcmp(argv[i], "-nocheck") == 0)
        {
            check_points = FALSE;
        }
        else if (strcmp(argv[i], "--stdin") == 0
              || strcmp(argv[i], "-ilas") == 0) 
        {
            use_stdin = TRUE;
        }
        else if (strcmp(argv[i], "--repair") == 0
              || strcmp(argv[i], "-r") == 0
              || strcmp(argv[i], "-repair_header") == 0
              || strcmp(argv[i], "-repair") == 0) 
        {
            repair_header = TRUE;
            check_points = TRUE;
        }
        else if (strcmp(argv[i], "--repair_bb") == 0
              || strcmp(argv[i], "--repair_bounding_box") == 0
              || strcmp(argv[i], "--repair_boundingbox") == 0
              || strcmp(argv[i], "-repair_bb") == 0
              || strcmp(argv[i], "-repair_bounding_box") == 0
              || strcmp(argv[i], "-repair_boundingbox") == 0
              || strcmp(argv[i], "-repair") == 0
              || strcmp(argv[i], "-rb") == 0) 
        {
            repair_bounding_box = TRUE;
            check_points = TRUE;
        }

        else if (strcmp(argv[i],"--system_identifier") == 0
              || strcmp(argv[i],"-system_identifier") == 0
              || strcmp(argv[i],"-s") == 0
              || strcmp(argv[i],"-sys_id") == 0)
        {
            i++;
            system_identifier = (char*) malloc(31 * sizeof(char));
            strcpy(system_identifier, argv[i]);
            change_header = TRUE;
        }

        else if (strcmp(argv[i],"--generating_software") == 0
              || strcmp(argv[i],"-generating_software") == 0
              || strcmp(argv[i],"-g") == 0
              || strcmp(argv[i],"-gen_soft") == 0)
        {
            i++;
            generating_software = (char*) malloc(31*sizeof(char));
            strcpy(generating_software, argv[i]);
            change_header = TRUE;
        }
        else if (strcmp(argv[i],"--file_creation") == 0
              || strcmp(argv[i],"-file_creation") == 0)
        {
            /* XXX - mloskot: Consider replacing atoi with strtol,
            see http://www.iso-9899.info/wiki/Converting */
            i++;
            file_creation_day = (unsigned char)atoi(argv[i]);
            i++;
            file_creation_year = (unsigned char)atoi(argv[i]);
            change_header = TRUE;
        }
        else if (strcmp(argv[i],"--skip_vlr") == 0 || strcmp(argv[i],"--no_vlr") == 0)
        {
            skip_vlr = TRUE;
        }
        else if (strcmp(argv[i],"--wkt") == 0)
        {
            wkt = TRUE;
        }    
        else if (file_name == NULL)
        {
            file_name = argv[i];
        } 
        else
        {
            usage();
            fprintf(stderr, "ERROR: unknown argument '%s'\n",argv[i]);
            exit(1);
        }
    }

    if (use_stdin) {
        file_name = "stdin";
    }

    if (!file_name) {
        LASError_Print("No filename was provided to be opened");
        usage();
        exit(1);
    }

    reader = LASReader_Create(file_name);
    if (!reader) { 
        LASError_Print("Could not open file ");
        exit(1);
    } 

    header = LASReader_GetHeader(reader);
    if (!header) { 
        LASError_Print("Could not get LASHeader ");
        exit(1);
    } 

    print_header(stdout, header, file_name, skip_vlr, wkt);

    if (change_header) {
        if (system_identifier) {
            err = LASHeader_SetSystemId (header, system_identifier);
            if (err) LASError_Print("Could not set SystemId");
        }
        if (generating_software) {
            err = LASHeader_SetSoftwareId(header, generating_software);
            if (err) LASError_Print("Could not set SoftwareId");
        }
        if ( file_creation_day || file_creation_year) {
            err = LASHeader_SetCreationDOY(header, file_creation_day);
            if (err) LASError_Print("Could not set file creation day");
            err = LASHeader_SetCreationYear(header, file_creation_year);
            if (err) LASError_Print("Could not set file creation year");
        }

        /* We need to wipe out the reader and make a writer. */
        if (reader) {
            LASReader_Destroy(reader);
            reader = NULL;
        }

        writer = LASWriter_Create(file_name, header, LAS_MODE_APPEND);
        if (!writer) {
            LASError_Print("Problem creating LASWriterH object");
            LASHeader_Destroy(header);
            header = NULL;
            exit(1);
        }

        if (writer) LASWriter_Destroy(writer);
        writer = NULL;
        if (header) LASHeader_Destroy(header);
        header = NULL;
    }

    if (check_points)
    {
        if (!reader) {
            reader = LASReader_Create(file_name);
            if (!reader) { 
                LASError_Print("Could not open file ");
                exit(1);
            } 
        }

        if (! header) {
            header = LASReader_GetHeader(reader);
            if (!header) { 
                LASError_Print("Could not get LASHeader ");
                exit(1);
            } 
        } 

        if (!summary)
            summary = SummarizePoints(reader);
        print_point_summary(stdout, summary, header);

        if (repair_header) {
            fprintf(stdout, "\n---------------------------------------------------------\n");
            fprintf(stdout, "  Repair Summary\n");
            fprintf(stdout, "---------------------------------------------------------\n");

            if (use_stdin) {
                LASError_Print("Cannot update header information on piped input!");
                exit(1);
            }

            if (! header) {
                header = LASReader_GetHeader(reader);
                if (!header) { 
                    LASError_Print("Could not get LASHeader ");
                    exit(1);
                }
            } 

            if (! repair_bounding_box) {
                if ( LASHeader_GetMinX(header) != LASPoint_GetX(summary->pmin) )
                    repair_bounding_box = TRUE;
                if ( LASHeader_GetMinY(header) != LASPoint_GetY(summary->pmin) )
                    repair_bounding_box = TRUE;
                if ( LASHeader_GetMinZ(header) != LASPoint_GetZ(summary->pmin) )
                    repair_bounding_box = TRUE;

                if ( LASHeader_GetMaxX(header) != LASPoint_GetX(summary->pmax) )
                    repair_bounding_box = TRUE;
                if ( LASHeader_GetMaxY(header) != LASPoint_GetY(summary->pmax) )
                    repair_bounding_box = TRUE;
                if ( LASHeader_GetMaxZ(header) != LASPoint_GetZ(summary->pmax) )
                    repair_bounding_box = TRUE;
            }

            if (repair_bounding_box) {
                fprintf(stdout, "  Reparing Bounding Box...\n");
                err = LASHeader_SetMin( header, 
                    LASPoint_GetX(summary->pmin), 
                    LASPoint_GetY(summary->pmin), 
                    LASPoint_GetZ(summary->pmin)
                    );
                if (err) {
                    LASError_Print("Could not set minimum for header ");
                    exit(1);
                }
                err = LASHeader_SetMax( header, 
                    LASPoint_GetX(summary->pmax), 
                    LASPoint_GetY(summary->pmax), 
                    LASPoint_GetZ(summary->pmax)
                    );
                if (err) {
                    LASError_Print("Could not set minimum for header ");
                    exit(1);
                }
            }

            for (i = 0; i < 5; i++) {

                if (LASHeader_GetPointRecordsByReturnCount(header, i) != 
                    summary->number_of_points_by_return[i]) 
                {
                    update_return_counts = TRUE;
                    break;
                }
            }

            if (update_return_counts) {
                fprintf(stdout, "  Reparing Point Count by Return...\n");
                for (i = 0; i < 5; i++) {
                    LASHeader_SetPointRecordsByReturnCount( header,  
                        i, 
                        summary->number_of_points_by_return[i]);
                }                
            }

            if (reader) {
                LASReader_Destroy(reader);
                reader = NULL;
            }

            writer = LASWriter_Create(file_name, header, LAS_MODE_APPEND);
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
        }

        if (summary) {
            LASPoint_Destroy(summary->pmin);
            LASPoint_Destroy(summary->pmax);
            free(summary);

        }
    }   

    if (reader) LASReader_Destroy(reader);
    if (header) LASHeader_Destroy(header);

#ifdef HAVE_GDAL
    /* Various GDAL related cleanups */
#ifdef OSRCleanup
    OSRCleanup();
#endif
    CPLFinderClean();
    CPLFreeConfig();
    CPLCleanupTLS();
#endif

    return 0;
}
