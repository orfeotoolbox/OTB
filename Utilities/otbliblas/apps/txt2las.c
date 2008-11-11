/***************************************************************************
 * $Id: txt2las.c 864 2008-08-22 16:04:18Z mloskot $
 * $Date: 2008-08-22 11:04:18 -0500 (Fri, 22 Aug 2008) $
 *
 * Project: libLAS -- C/C++ read/write library for LAS LIDAR data
 * Purpose: ASCII text to LAS translation
 * Author:  Martin Isenburg isenburg@cs.unc.edu 
 ***************************************************************************
 * Copyright (c) 2007, Martin Isenburg isenburg@cs.unc.edu 
 *
 * See LICENSE.txt in this source distribution for more information.
 **************************************************************************/

#include <liblas.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void usage()
{

    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"    txt2las (version %s) usage:\n", LAS_GetVersion());
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Parse a text file with a given format:\n");
    fprintf(stderr,"  txt2las -parse tsxyz lidar.txt\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Set the scale:\n");
    fprintf(stderr,"  txt2las --parse xyz --scale 0.02 -i lidar.txt -o lidar.laz\n");
    fprintf(stderr,"\n");
    
    fprintf(stderr,"Set the xyz scale:\n");
    fprintf(stderr,"  txt2las --parse xsysz --verbose --xyz_scale 0.02 0.02 0.01 lidar.txt\n");
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

    fprintf(stderr,"\n----------------------------------------------------------\n");
    fprintf(stderr," The '-scale 0.02' flag specifies the quantization. The\n");
    fprintf(stderr," default value of 0.01 means that the smallest increment\n");
    fprintf(stderr," two between coordinates is 0.01. If measurements are in\n");
    fprintf(stderr," meters this corresponds to centimeter accuracy, which is\n");
    fprintf(stderr," commonly considered sufficient for LIDAR data.\n");

    fprintf(stderr,"\n----------------------------------------------------------\n");
    fprintf(stderr," Other parameters such as '--xyz_offset 500000 2000000 0'\n");
    fprintf(stderr," or '-xyz_scale 0.02 0.02 0.01' or '-file_creation 67 2003'\n");
    fprintf(stderr," or '-system_identifier \"Airborne One Leica 50,000 Hz\"'\n");
    fprintf(stderr," or '-generating_software \"TerraScan\"' are available too.\n");

    fprintf(stderr, "For more information, see the full documentation for txt2las at:\n"
                    " http://liblas.org/browser/trunk/doc/txt2las.txt\n");
    fprintf(stderr,"----------------------------------------------------------\n");

}

static void VecUpdateMinMax3dv(double min[3], double max[3], const double v[3])
{
  if (v[0]<min[0]) min[0]=v[0]; else if (v[0]>max[0]) max[0]=v[0];
  if (v[1]<min[1]) min[1]=v[1]; else if (v[1]>max[1]) max[1]=v[1];
  if (v[2]<min[2]) min[2]=v[2]; else if (v[2]>max[2]) max[2]=v[2];
}

static void VecCopy3dv(double v[3], const double a[3])
{
  v[0] = a[0];
  v[1] = a[1];
  v[2] = a[2];
}

static int parse(const char* parse_string, const char* line, double* xyz, LASPointH point, double* gps_time)
{
    int temp_i;
    float temp_f;
    const char* p = parse_string;
    const char* l = line;


    while (p[0])
    {
        /* // we expect the x coordinate */
        if (p[0] == 'x') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%lf", &(xyz[0])) != 1) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the y coordinate */
        else if (p[0] == 'y') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%lf", &(xyz[1])) != 1) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the x coordinate */
        else if (p[0] == 'z') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%lf", &(xyz[2])) != 1) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect a string or a number that we don't care about */
        else if (p[0] == 's') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the intensity */
        else if (p[0] == 'i') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%f", &temp_f) != 1) return FALSE;
            if (temp_f < 0.0f || temp_f > 65535.0f) 
                fprintf(stderr, 
                        "WARNING: intensity %g is out of range of unsigned short\n", 
                        temp_f);
            LASPoint_SetIntensity(point, (int)temp_f);
/*          point->intensity = (unsigned short)temp_f; */
            /* // then advance to next white space*/
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the scan angle */
        else if (p[0] == 'a') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%f", &temp_f) != 1) return FALSE;
            if (temp_f < -128.0f || temp_f > 127.0f) 
                fprintf(stderr, 
                        "WARNING: scan angle %g is out of range of char\n", 
                        temp_f);
            LASPoint_SetScanAngleRank(point, (int)temp_f);
/*          point->scan_angle_rank = (char)temp_f; */
            /* // then advance to next white space*/
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the number of returns of given pulse */
        else if (p[0] == 'n') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 7) 
                fprintf(stderr, 
                        "WARNING: return number %d is out of range of three bits\n", 
                        temp_i);
            LASPoint_SetNumberOfReturns(point, temp_i);
/*          point->number_of_returns_of_given_pulse = temp_i & 7; */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /*  // we expect the number of the return */
        else if (p[0] == 'r')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 7) 
                fprintf(stderr, 
                        "WARNING: return number %d is out of range of three bits\n", 
                        temp_i);
/*          point->return_number = temp_i & 7; */
            LASPoint_SetReturnNumber(point, temp_i);
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the classification */
        else if (p[0] == 'c') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 255) 
                fprintf(stderr, 
                        "WARNING: classification %d is out of range of unsigned char\n", 
                        temp_i);
            LASPoint_SetClassification(point, temp_i);
/*          point->classification = (unsigned char)temp_i; */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the user data */
        else if (p[0] == 'u') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 255) 
                fprintf(stderr, 
                        "WARNING: user data %d is out of range of unsigned char\n", 
                        temp_i);
            
            LASPoint_SetUserData(point, temp_i);
/*              point->user_data = temp_i & 255; */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the point source ID */
        else if (p[0] == 'p') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 65535) 
                fprintf(stderr, 
                        "WARNING: point source ID %d is out of range of unsigned short\n", 
                        temp_i);
/*          point->point_source_ID = temp_i & 65535; */
            /*// then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the edge of flight line flag */
        else if (p[0] == 'e') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 1) 
                fprintf(stderr, 
                        "WARNING: edge of flight line flag %d is out of range of boolean flag\n", 
                        temp_i);
            LASPoint_SetFlightLineEdge(point, temp_i ? 1: 0);
/*          point->edge_of_flight_line = (temp_i ? 1 : 0); */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the direction of scan flag */
        else if (p[0] == 'd') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 1) 
                fprintf(stderr, 
                        "WARNING: direction of scan flag %d is out of range of boolean flag\n", 
                        temp_i);
            LASPoint_SetScanDirection(point, temp_i ? 1: 0);
/*          point->scan_direction_flag = (temp_i ? 1 : 0); */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        /* // we expect the gps time */
        else if (p[0] == 't') 
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++; 
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%lf", gps_time) != 1) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++; 
        }
        else
        {
        fprintf(stderr, "ERROR: next symbol '%s' unknown in parse control string\n", p);
        }
        p++;
    }
    return TRUE;
}


int main(int argc, char *argv[])
{
    int i;
    int dry = FALSE;
    int verbose = FALSE;
    char* file_name_in = 0;
    char* file_name_out = 0;
    double xyz_min[3] = {0.0, 0.0, 0.0};
    double xyz_max[3] = {0.0, 0.0, 0.0};
    double xyz_scale[3] = {0.01,0.01,0.01};
    double xyz_offset[3] = {0.0,0.0,0.0};
    unsigned int number_of_point_records = 0;
    unsigned int number_of_points_by_return[8] = {0,0,0,0,0,0,0,0};
    char* parse_string = "xyz";
    int file_creation_day = 0;
    int file_creation_year = 0;
    char* system_identifier = 0;
    char* generating_software = 0;
#define MAX_CHARACTERS_PER_LINE 512
    char line[MAX_CHARACTERS_PER_LINE];
    double xyz[3];
    LASPointH point = NULL;
    double gps_time;
    FILE* file_in = NULL;
    char* parse_less = NULL;
    LASHeaderH header = NULL;
    LASWriterH writer = NULL;
    LASError err;

    int xyz_min_quant[3] = {0, 0, 0};
    int xyz_max_quant[3] = {0, 0, 0};

    double xyz_min_dequant[3] = {0.0, 0.0, 0.0};
    double xyz_max_dequant[3] = {0.0, 0.0, 0.0};

    for (i = 1; i < argc; i++)
    {
        if (    strcmp(argv[i],"-h") == 0       ||
                strcmp(argv[i],"--help") == 0
            )
        {
            usage();
            exit(0);
        }
        else if (   strcmp(argv[i],"-v") == 0   ||
                    strcmp(argv[i],"--verbose") == 0
            )
        {
            verbose = TRUE;
        }    
        else if (   strcmp(argv[i],"-d") == 0       ||
                    strcmp(argv[i],"-dry") == 0     ||
                    strcmp(argv[i],"--dry") == 0
            )
        {
            dry = TRUE;
        }          
        else if (   strcmp(argv[i],"--parse") == 0  ||
                    strcmp(argv[i],"-parse") == 0   ||
                    strcmp(argv[i],"-p") == 0       
                )
        {
            i++;
            parse_string = argv[i];
        }
        else if (   strcmp(argv[i],"--scale") == 0  ||
                    strcmp(argv[i],"-scale") == 0   ||
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

        else if (   strcmp(argv[i],"--system_identifier") == 0   ||
                    strcmp(argv[i],"-system_identifier") == 0   ||
                    strcmp(argv[i],"-s") == 0   ||
                    strcmp(argv[i],"-sys_id") == 0)
        {
            i++;
            system_identifier = (char*) malloc(31 * sizeof(char));
            strcpy(system_identifier, argv[i]);
        }

        else if (   strcmp(argv[i],"--generating_software") == 0   ||
                    strcmp(argv[i],"-generating_software") == 0   ||
                    strcmp(argv[i],"-g") == 0   ||
                    strcmp(argv[i],"-gen_soft") == 0)
        {
            i++;
            generating_software = (char*) malloc(31*sizeof(char));
            strcpy(generating_software, argv[i]);
        }

        else if (   strcmp(argv[i],"--file_creation") == 0   ||
                    strcmp(argv[i],"-file_creation") == 0)
        {
            i++;
            file_creation_day = (unsigned short)atoi(argv[i]);
            i++;
            file_creation_year = (unsigned short)atoi(argv[i]);
        }
        else if (   i == argc - 2 
                    && file_name_in == NULL && 
                    file_name_out == NULL
                )
        {
            file_name_in = argv[i];
        }
        else if (   i == argc - 1 && 
                    file_name_in == NULL && 
                    file_name_out == NULL
                )
        {
            file_name_in = argv[i];
        }
        else if (   i == argc - 1 && 
                    file_name_in && 
                    file_name_out == NULL
                )
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

    /* create output file name if none specified and no piped output requested */
    if (file_name_out == NULL && file_name_in != NULL)
    {
        int len = (int)strlen(file_name_in);
        file_name_out = strdup(file_name_in);

        while (len > 0 && file_name_out[len] != '.')
        {
            len--;
        }
        file_name_out[len] = '.';
        file_name_out[len+1] = 'l';
        file_name_out[len+2] = 'a';
        file_name_out[len+3] = 's';
        file_name_out[len+4] = '\0';
    }


    /* make sure that input and output are not *both* piped */
    if (file_name_in == NULL && file_name_out == NULL)
    {
        LASError_Print("both input and output filenames are null!");
        usage();
        exit(1);
    }

    file_in = fopen(file_name_in, "r");

    if (file_in == NULL)
    {
        LASError_Print("could not open file to read for first pass");
        exit(1);
    }

    /* create a cheaper parse string that only looks for 'x' 'y' 'z' and 'r' */
    parse_less = strdup(parse_string);
    for (i = 0; i < (int)strlen(parse_string); i++)
    {
        if (parse_less[i] != 'x' && 
            parse_less[i] != 'y' && 
            parse_less[i] != 'z' && 
            parse_less[i] != 'r') 
        {
            parse_less[i] = 's';
        }
    }

    do
    {
        parse_less[i] = '\0';
        printf("nuking %d for %c\n", i, parse_less[i]);
        i--;
    } while (parse_less[i] == 's');
    
    
    /* first pass to figure out the bounding box and number of returns */
    if (verbose) {
        fprintf(stderr, 
                "first pass over file '%s' with parse '%s'\n", 
                file_name_in, 
                parse_less);
    }

    /* read the first line */
    while (fgets(line, sizeof(char) * MAX_CHARACTERS_PER_LINE, file_in))
    {
        point = LASPoint_Create();
        if (parse(parse_less, line, xyz, point, &gps_time))
        {
            /* init the bounding box */
            VecCopy3dv(xyz_min, xyz);
            VecCopy3dv(xyz_max, xyz);

            /* mark that we found the first point */
            number_of_point_records = 1;

            /* create return histogram */
            number_of_points_by_return[LASPoint_GetReturnNumber(point)]++;
            
            /* we can stop this loop */
            break;
        }
        else
        {
            fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", 
                    line, 
                    parse_less);

        }
        LASPoint_Destroy(point);
        point = NULL;
    }

    /* did we manage to parse a line? */
    if (number_of_point_records != 1)
    {
        fprintf(stderr, "ERROR: could not parse any lines with '%s'\n", 
                parse_less);
        exit(1);
    }

    /* loop over the remaining lines */
    while (fgets(line, sizeof(char) * MAX_CHARACTERS_PER_LINE, file_in))
    {
        point = LASPoint_Create();
        if (parse(parse_less, line, xyz, point, &gps_time))
        {
            /* update bounding box */
            VecUpdateMinMax3dv(xyz_min, xyz_max, xyz);

            /* count points */
            number_of_point_records++;

            /* create return histogram */
            number_of_points_by_return[LASPoint_GetReturnNumber(point)]++;
        }
        else
        {
            fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", 
            line, 
            parse_less);

        }
        LASPoint_Destroy(point);
        point = NULL;
    }

    /* output some stats */
    if (verbose)
    {
        fprintf(stderr, 
                "npoints %d min %g %g %g max %g %g %g\n", 
                number_of_point_records, 
                xyz_min[0], 
                xyz_min[1], 
                xyz_min[2], 
                xyz_max[0], 
                xyz_max[1], 
                xyz_max[2]
                );
        fprintf(stderr, 
                "return histogram %d %d %d %d %d %d %d %d\n", 
                number_of_points_by_return[0], 
                number_of_points_by_return[1], 
                number_of_points_by_return[2], 
                number_of_points_by_return[3], 
                number_of_points_by_return[4], 
                number_of_points_by_return[5], 
                number_of_points_by_return[6], 
                number_of_points_by_return[7]
                );
    }

    /* close the input file */    
      fclose(file_in);

    /*  compute bounding box after quantization */
    for (i = 0; i < 3; i++)
    {
        xyz_min_quant[i] = (int)(0.5 + (xyz_min[i] - xyz_offset[i]) / xyz_scale[i]);
        xyz_max_quant[i] = (int)(0.5 + (xyz_max[i] - xyz_offset[i]) / xyz_scale[i]);
    }

    for (i = 0; i < 3; i++)
    {
        xyz_min_dequant[i] = xyz_offset[i] + (xyz_min_quant[i] * xyz_scale[i]);
        xyz_max_dequant[i] = xyz_offset[i] + (xyz_max_quant[i] * xyz_scale[i]);
    }


#define log_xor !=0==!

    /* make sure there is not sign flip */
    for (i = 0; i < 3; i++)
    {
        if ((xyz_min[i] > 0) log_xor (xyz_min_dequant[i] > 0))
        {
            fprintf(stderr, 
                    "WARNING: quantization sign flip for %s min coord %g -> %g. use offset or scale up\n", 
                    (i ? (i == 1 ? "y" : "z") : "x"), 
                    xyz_min[i], 
                    xyz_min_dequant[i]
                   );
        }
        if ((xyz_max[i] > 0) log_xor (xyz_max_dequant[i] > 0))
        {
            fprintf(stderr, 
                    "WARNING: quantization sign flip for %s max coord %g -> %g. use offset or scale up\n", 
                    (i ? (i == 1 ? "y" : "z") : "x"), 
                    xyz_max[i], 
                    xyz_max_dequant[i]
                   );
        }
    }

#undef log_xor

    /* populate the header */
      
    header = LASHeader_Create();

    if (system_identifier) LASHeader_SetSystemId(header, system_identifier);
    if (generating_software) LASHeader_SetSoftwareId(header, generating_software);
    LASHeader_SetCreationDOY(header, file_creation_day);
    LASHeader_SetCreationYear(header, file_creation_year);
      

    if (strstr(parse_string,"t"))
    {
        fprintf(stderr, "Setting to LAS version 1.1!!!\n");
        LASHeader_SetDataFormatId(header, 1);
    }
    else
    {
        LASHeader_SetDataFormatId(header, 0);
    }
    LASHeader_SetPointRecordsCount(header, number_of_point_records);
    LASHeader_SetScale(header, xyz_scale[0], xyz_scale[1], xyz_scale[2]);
    LASHeader_SetOffset(header, xyz_offset[0], xyz_offset[1], xyz_offset[2]);
    LASHeader_SetMin(header, xyz_min_dequant[0], xyz_min_dequant[1], xyz_min_dequant[2]);
    LASHeader_SetMax(header, xyz_max_dequant[0], xyz_max_dequant[1], xyz_max_dequant[2]);
    LASHeader_SetPointRecordsByReturnCount(header, 0, number_of_points_by_return[1]);
    LASHeader_SetPointRecordsByReturnCount(header, 1, number_of_points_by_return[2]);
    LASHeader_SetPointRecordsByReturnCount(header, 2, number_of_points_by_return[3]);
    LASHeader_SetPointRecordsByReturnCount(header, 3, number_of_points_by_return[4]);
    LASHeader_SetPointRecordsByReturnCount(header, 4, number_of_points_by_return[5]);



    /* reopen input file for the second pass */
    file_in = fopen(file_name_in, "r");

    if (file_in == 0)
    {
        fprintf(stderr, "ERROR: could not open '%s' for second pass\n",file_name_in);
        exit(1);
    }

    /*
        because the output goes to a file we can do everything in a 
        single pass and compute the header information along the way 
    */
    
    /* open output file */
    printf("Creating file...\n");
    writer = LASWriter_Create(file_name_out, header, LAS_MODE_WRITE);
    if (!writer) { 
        LASError_Print("Could not open file for write mode ");
        exit(1);
    }     

    if (verbose) {
        fprintf(stderr, 
                "scanning %s with parse '%s' writing to %s\n", 
                file_name_in , 
                parse_string, 
                file_name_out
               );
    }
    

    /* read the first line */
    while (fgets(line, sizeof(char) * MAX_CHARACTERS_PER_LINE, file_in))
    {
        point = LASPoint_Create();
        if (parse(parse_string, line, xyz, point, &gps_time))
        {
            /* init the bounding box */
            VecCopy3dv(xyz_min, xyz);
            VecCopy3dv(xyz_max, xyz);

            /* mark that we found the first point */
            number_of_point_records = 1;

            /* create return histogram */
            number_of_points_by_return[LASPoint_GetReturnNumber(point)]++;
            
            /* compute the quantized x, y, and z values */
            LASPoint_SetX(point, xyz[0]);
            LASPoint_SetY(point, xyz[1]);
            LASPoint_SetZ(point, xyz[2]);

            /* write the first point */
            err = LASWriter_WritePoint(writer, point);
            if (err) {
                LASError_Print("could not write point");
                exit(1);
            }
              
            /* we can stop this loop */
            break;
        }
        else
        {
            fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", 
                    line, 
                    parse_string);
        }
        LASPoint_Destroy(point);
        point = NULL;
    }

    /* did we manage to parse a line? */
    if (number_of_point_records != 1)
    {
        fprintf(stderr, "ERROR: could not parse any lines with '%s'\n", 
                parse_less);
        exit(1);
    }

    /* loop over the remaining lines */
    while (fgets(line, sizeof(char) * MAX_CHARACTERS_PER_LINE, file_in))
    {
        point = LASPoint_Create();
        if (parse(parse_string, line, xyz, point, &gps_time))
        {
            /* update bounding box */
            VecUpdateMinMax3dv(xyz_min, xyz_max, xyz);

            /* count points */
            number_of_point_records++;

            /* create return histogram */
            number_of_points_by_return[LASPoint_GetReturnNumber(point)]++;

            /* compute the quantized x, y, and z values */
            LASPoint_SetX(point, xyz[0]);
            LASPoint_SetY(point, xyz[1]);
            LASPoint_SetZ(point, xyz[2]);

            /* write the first point */
            err = LASWriter_WritePoint(writer, point);
            if (err) {
                LASError_Print("could not write point");
                exit(1);
            }

        }
        else
        {
            fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n", 
            line, 
            parse_string);
        }
        LASPoint_Destroy(point);
        point = NULL;
    }


    /* close up stuff */
    fclose(file_in);
    LASWriter_Destroy(writer);
    if (verbose)
    {
        fprintf(stderr, "done.\n");
    }


    return 0;
}
