/***************************************************************************
 * $Id$
 * $Date$
 *
 * Project: libLAS -- C/C++ read/write library for LAS LIDAR data
 * Purpose: LAS difference scanning
 * Author:  Martin Isenburg isenburg@cs.unc.edu 
 ***************************************************************************
 * Copyright (c) 2007, Martin Isenburg isenburg@cs.unc.edu 
 *
 * See LICENSE.txt in this source distribution for more information.
 **************************************************************************/

#if defined(_MSC_VER) && defined(USE_VLD)
#include <vld.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <liblas.h>


void usage()
{
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"    lasdiff (version %s) usage:\n", LAS_GetVersion());
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"lasdiff lidar1.las lidar2.las\n");
    fprintf(stderr,"lasdiff lidar1.las lidar1.las.lz\n");
    fprintf(stderr,"lasdiff -i lidar1.las -i lidar1.laz\n");
    fprintf(stderr,"lasdiff -h\n");
    fprintf(stderr, "\nFor more information, see the full documentation for lasdiff at:\n"
                    " http://liblas.org/browser/trunk/doc/lasdiff.txt\n");
    fprintf(stderr,"----------------------------------------------------------\n");
}

static int lidardouble2string(char* string, double value0, double value1)
{
  int len;
  len = sprintf(string, "%f", value0) - 1;
  while (string[len] == '0') len--;
  if (string[len] != '.') len++;
  len += sprintf(&(string[len]), " %f", value1) - 1;
  while (string[len] == '0') len--;
  if (string[len] != '.') len++;
  string[len] = '\0';
  return len;
}

void print_error(char* message) {

    if (LASError_GetErrorCount()) {
        fprintf(stdout, 
            "%s: %s (%d) from method %s\n",
            message,
            LASError_GetLastErrorMsg(),
            LASError_GetLastErrorNum(),
            LASError_GetLastErrorMethod()
        ); 
    } else {
        fprintf(stdout, 
            "You have encountered an error. '%s'\n",
            message
        );         
    }

}

int main(int argc, char *argv[])
{
    int i;
    char* file_name_in1 = 0;
    char* file_name_in2 = 0;
    
    LASReaderH reader1 = NULL;
    LASReaderH reader2 = NULL;
    
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"-i") == 0)
        {
            i++;
            if (file_name_in1) file_name_in2 = argv[i];
            else file_name_in1 = argv[i];
        }
        else if (i == argc - 2 && file_name_in1 == 0 && file_name_in2 == 0)
        {
            if (file_name_in1) file_name_in2 = argv[i];
            else file_name_in1 = argv[i];
        }
        else if (i == argc - 1 && file_name_in1 && file_name_in2 == 0)
        {
            if (file_name_in1) file_name_in2 = argv[i];
            else file_name_in1 = argv[i];
        }
        else
        {
            usage();
        }
  }


    if (file_name_in1)
    {
        reader1 = LASReader_Create(file_name_in1);
        if (!reader1) { 
            print_error("Could not open file 1");
            exit(1);
        } 
    }
    else
    {
        print_error("No input specified for first file");
    }

    if (file_name_in2)
    {
        reader2 = LASReader_Create(file_name_in2);
        if (!reader2) { 
            print_error("Could not open file 2");
            exit(1);
        }         
    }
    else 
    {
        print_error("No input specified for second file");
        usage();
    }



  // check header

  if (memcmp((const void*)&(lasreader1->header), (const void*)&(lasreader2->header), sizeof(LASheader)))
  {
    char printstring[128];

    fprintf(stderr, "header is different\n");

    LASheader* lasheader1 = &(lasreader1->header);
    LASheader* lasheader2 = &(lasreader2->header);

    bool fatal_difference = false;

    if (strncmp(lasheader1->file_signature, lasheader2->file_signature, 4))
    {
      fprintf(stderr, "  file_signature: %s %s\n", lasheader1->file_signature, lasheader2->file_signature);
    }
    if (lasheader2->file_source_id != lasheader2->file_source_id)
    {
      fprintf(stderr, "  file_source_id: %d %d\n", lasheader1->file_source_id, lasheader2->file_source_id);
    }
    if (lasheader1->reserved != lasheader2->reserved)
    {
      fprintf(stderr, "  reserved: %d %d\n", lasheader1->reserved, lasheader2->reserved);
    }
    if (lasheader1->project_ID_GUID_data_1 != lasheader2->project_ID_GUID_data_1)
    {
      fprintf(stderr, "  project_ID_GUID_data_1: %d %d\n", lasheader1->project_ID_GUID_data_1, lasheader2->project_ID_GUID_data_1);
    }
    if (lasheader1->project_ID_GUID_data_2 != lasheader2->project_ID_GUID_data_2)
    {
      fprintf(stderr, "  project_ID_GUID_data_2: %d %d\n", lasheader1->project_ID_GUID_data_2, lasheader2->project_ID_GUID_data_2);
    }
    if (lasheader1->project_ID_GUID_data_3 != lasheader2->project_ID_GUID_data_3)
    {
      fprintf(stderr, "  project_ID_GUID_data_3: %d %d\n", lasheader1->project_ID_GUID_data_3, lasheader2->project_ID_GUID_data_3);
    }
    if (strncmp(lasheader1->project_ID_GUID_data_4, lasheader2->project_ID_GUID_data_4, 8))
    {
      fprintf(stderr, "  project_ID_GUID_data_4: %s %s\n", lasheader1->project_ID_GUID_data_4, lasheader2->project_ID_GUID_data_4);
    }
    if (lasheader1->version_major != lasheader2->version_major || lasheader1->version_minor != lasheader2->version_minor)
    {
      fprintf(stderr, "  version: %d.%d %d.%d\n", lasheader1->version_major, lasheader1->version_minor, lasheader2->version_major, lasheader2->version_minor);
    }
    if (strncmp(lasheader1->system_identifier, lasheader2->system_identifier, 32))
    {
      fprintf(stderr, "  system_identifier: %s %s\n", lasheader1->system_identifier, lasheader2->system_identifier);
    }
    if (strncmp(lasheader1->generating_software, lasheader2->generating_software, 32))
    {
      fprintf(stderr, "  generating_software: %s %s\n", lasheader1->generating_software, lasheader2->generating_software);
    }
    if (lasheader1->file_creation_day != lasheader2->file_creation_day || lasheader1->file_creation_year != lasheader2->file_creation_year)
    {
      fprintf(stderr, "  file_creation day.year: %d.%d %d.%d\n", lasheader1->file_creation_day, lasheader1->file_creation_year, lasheader2->file_creation_day, lasheader2->file_creation_year);
    }
    if (lasheader1->header_size != lasheader2->header_size)
    {
      fprintf(stderr, "  header_size: %d %d\n", lasheader1->header_size, lasheader2->header_size);
      fatal_difference = true;
    }
    if (lasheader1->offset_to_point_data != lasheader2->offset_to_point_data)
    {
      fprintf(stderr, "  offset_to_point_data: %d %d\n", lasheader1->offset_to_point_data, lasheader2->offset_to_point_data);
    }
    if (lasheader1->point_data_format != lasheader2->point_data_format)
    {
      fprintf(stderr, "  point_data_format: %d %d\n", lasheader1->point_data_format, lasheader2->point_data_format);
      fatal_difference = true;
    }
    if (lasheader1->point_data_record_length != lasheader2->point_data_record_length)
    {
      fprintf(stderr, "  point_data_record_length: %d %d\n", lasheader1->point_data_record_length, lasheader2->point_data_record_length);
      fatal_difference = true;
    }
    if (lasheader1->number_of_point_records != lasheader2->number_of_point_records)
    {
      fprintf(stderr, "  number_of_point_records: %d %d\n", lasheader1->number_of_point_records, lasheader2->number_of_point_records);
      fatal_difference = true;
    }
    if (lasheader1->number_of_points_by_return[0] != lasheader2->number_of_points_by_return[0] || lasheader1->number_of_points_by_return[1] != lasheader2->number_of_points_by_return[1] || lasheader1->number_of_points_by_return[2] != lasheader2->number_of_points_by_return[2] || lasheader1->number_of_points_by_return[3] != lasheader2->number_of_points_by_return[3] || lasheader1->number_of_points_by_return[4] != lasheader2->number_of_points_by_return[4])
    {
      fprintf(stderr, "  number_of_points_by_return: (%d,%d,%d,%d,%d) (%d,%d,%d,%d,%d)\n", lasheader1->number_of_points_by_return[0], lasheader1->number_of_points_by_return[1], lasheader1->number_of_points_by_return[2], lasheader1->number_of_points_by_return[3], lasheader1->number_of_points_by_return[4], lasheader2->number_of_points_by_return[0], lasheader2->number_of_points_by_return[1], lasheader2->number_of_points_by_return[2], lasheader2->number_of_points_by_return[3], lasheader2->number_of_points_by_return[4]);
    }
    if (lasheader1->x_scale_factor != lasheader2->x_scale_factor)
    {
      lidardouble2string(printstring, lasheader1->x_scale_factor, lasheader2->x_scale_factor); fprintf(stderr, "  x_scale_factor: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->y_scale_factor != lasheader2->y_scale_factor)
    {
      lidardouble2string(printstring, lasheader1->y_scale_factor, lasheader2->y_scale_factor); fprintf(stderr, "  y_scale_factor: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->z_scale_factor != lasheader2->z_scale_factor)
    {
      lidardouble2string(printstring, lasheader1->z_scale_factor, lasheader2->z_scale_factor); fprintf(stderr, "  z_scale_factor: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->x_offset != lasheader2->x_offset)
    {
      lidardouble2string(printstring, lasheader1->x_offset, lasheader2->x_offset); fprintf(stderr, "  x_offset: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->y_offset != lasheader2->y_offset)
    {
      lidardouble2string(printstring, lasheader1->y_offset, lasheader2->y_offset); fprintf(stderr, "  y_offset: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->z_offset != lasheader2->z_offset)
    {
      lidardouble2string(printstring, lasheader1->z_offset, lasheader2->z_offset); fprintf(stderr, "  z_offset: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->max_x != lasheader2->max_x)
    {
      lidardouble2string(printstring, lasheader1->max_x, lasheader2->max_x); fprintf(stderr, "  max_x: %s\n", printstring);
    }
    if (lasheader1->min_x != lasheader2->min_x)
    {
      lidardouble2string(printstring, lasheader1->min_x, lasheader2->min_x); fprintf(stderr, "  min_x: %s\n", printstring);
    }
    if (lasheader1->max_y != lasheader2->max_y)
    {
      lidardouble2string(printstring, lasheader1->max_y, lasheader2->max_y); fprintf(stderr, "  max_y: %s\n", printstring);
    }
    if (lasheader1->min_y != lasheader2->min_y)
    {
      lidardouble2string(printstring, lasheader1->min_y, lasheader2->min_y); fprintf(stderr, "  min_y: %s\n", printstring);
    }
    if (lasheader1->max_z != lasheader2->max_z)
    {
      lidardouble2string(printstring, lasheader1->max_z, lasheader2->max_z); fprintf(stderr, "  max_z: %s\n", printstring);
    }
    if (lasheader1->min_z != lasheader2->min_z)
    {
      lidardouble2string(printstring, lasheader1->min_z, lasheader2->min_z); fprintf(stderr, "  min_z: %s\n", printstring);
    }
    if (fatal_difference)
    {
      fprintf(stderr, "difference was fatal ... no need to check points\n");
      exit(1);
    }
  }

  // check variable header (and user data)

  if (lasreader1->header.offset_to_point_data == lasreader2->header.offset_to_point_data)
  {
    bool already_output = false;
    for (unsigned int u = lasreader1->header.header_size; u < lasreader1->header.offset_to_point_data; u++)
    {
      if (fgetc(file_in1) != fgetc(file_in2))
      {
        if (!already_output)
        {
          fprintf(stderr, "variable header (or user data) is different at byte %d of %d\n", lasreader1->header.header_size - u, lasreader1->header.offset_to_point_data - u);
          already_output = true;
        }
      }
    }
  }
  else
  {
    unsigned int u;
    fprintf(stderr, "skipping check of variable header (or user data) due to length difference\n");
    for (u = lasreader1->header.header_size; u < lasreader1->header.offset_to_point_data; u++) fgetc(file_in1);
    for (u = lasreader2->header.header_size; u < lasreader2->header.offset_to_point_data; u++) fgetc(file_in2);
  }

  // check points

  while (true)
  {
    if (lasreader1->read_point())
    {
      if (lasreader2->read_point())
      {
        if (memcmp((const void*)&(lasreader1->point), (const void*)&(lasreader2->point), sizeof(LASpoint)))
        {
          fprintf(stderr, "point %d of %d is different\n", lasreader1->p_count, lasreader1->npoints);
          if (lasreader1->point.x != lasreader2->point.x)
          {
            fprintf(stderr, "  x: %d %d\n", lasreader1->point.x, lasreader2->point.x);
          }
          if (lasreader1->point.y != lasreader2->point.y)
          {
            fprintf(stderr, "  y: %d %d\n", lasreader1->point.y, lasreader2->point.y);
          }
          if (lasreader1->point.z != lasreader2->point.z)
          {
            fprintf(stderr, "  z: %d %d\n", lasreader1->point.z, lasreader2->point.z);
          }
          if (lasreader1->point.intensity != lasreader2->point.intensity)
          {
            fprintf(stderr, "  intensity: %d %d\n", lasreader1->point.intensity, lasreader2->point.intensity);
          }
          if (lasreader1->point.return_number != lasreader2->point.return_number)
          {
            fprintf(stderr, "  return_number: %d %d\n", lasreader1->point.return_number, lasreader2->point.return_number);
          }
          if (lasreader1->point.number_of_returns_of_given_pulse != lasreader2->point.number_of_returns_of_given_pulse)
          {
            fprintf(stderr, "  number_of_returns_of_given_pulse: %d %d\n", lasreader1->point.number_of_returns_of_given_pulse, lasreader2->point.number_of_returns_of_given_pulse);
          }
          if (lasreader1->point.scan_direction_flag != lasreader2->point.scan_direction_flag)
          {
            fprintf(stderr, "  scan_direction_flag: %d %d\n", lasreader1->point.scan_direction_flag, lasreader2->point.scan_direction_flag);
          }
          if (lasreader1->point.edge_of_flight_line != lasreader2->point.edge_of_flight_line)
          {
            fprintf(stderr, "  edge_of_flight_line: %d %d\n", lasreader1->point.edge_of_flight_line, lasreader2->point.edge_of_flight_line);
          }
          if (lasreader1->point.classification != lasreader2->point.classification)
          {
            fprintf(stderr, "  classification: %d %d\n", lasreader1->point.classification, lasreader2->point.classification);
          }
          if (lasreader1->point.scan_angle_rank != lasreader2->point.scan_angle_rank)
          {
            fprintf(stderr, "  scan_angle_rank: %d %d\n", lasreader1->point.scan_angle_rank, lasreader2->point.scan_angle_rank);
          }
          if (lasreader1->point.user_data != lasreader2->point.user_data)
          {
            fprintf(stderr, "  user_data: %d %d\n", lasreader1->point.user_data, lasreader2->point.user_data);
          }
          if (lasreader1->point.point_source_ID != lasreader2->point.point_source_ID)
          {
            fprintf(stderr, "  point_source_ID: %d %d\n", lasreader1->point.point_source_ID, lasreader2->point.point_source_ID);
          }
          exit(1);
        }
      }
      else
      {
        fprintf(stderr, "%s has fewer points (%d) than %s\n", lasreader2->p_count, file_name_in2, file_name_in1);
        exit(1);
      }
    }
    else
    {
      if (lasreader2->read_point())
      {
        fprintf(stderr, "%s has more points (%d) than %s\n", file_name_in2, lasreader2->p_count, file_name_in1);
        exit(1);
      }
      else
      {
        break;
      }
    }
  }

  fprintf(stderr, "files are identical. both have %d points\n", lasreader1->p_count);

  lasreader1->close();
  lasreader2->close();

  fclose(file_in1);
  fclose(file_in2);

  return 0;
}
