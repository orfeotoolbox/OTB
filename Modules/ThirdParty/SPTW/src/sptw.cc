/*!
 * Copyright 0000 <Nobody>
 * @file
 * @author David Matthew Mattli <dmattli@usgs.gov>
 *
 * @section LICENSE
 *
 * This software is in the public domain, furnished "as is", without
 * technical support, and with no warranty, express or implied, as to
 * its usefulness for any purpose.
 *
 * @section DESCRIPTION
 *
 * Implementation of the Simple Parallel Tiff Writer
 *
 */

#include <fcntl.h>
#include <gdal_priv.h>
#include <cpl_string.h>
#include <ogr_api.h>
#include <ogr_spatialref.h>
#include <mpi.h>
#include <tiff.h>
#include <tiffio.h>

#include <algorithm>
#include <sstream>
#include <vector>

#include "sptw.h"

#include "utils.h"

using std::string;
using sptw::Area;

namespace sptw {
/*
 * Return size of TIFFDataType in bytes
 */

int get_type_size(TIFFDataType type) {
  switch (type) {
    case 0:
    case TIFF_BYTE:
    case TIFF_ASCII:
    case TIFF_SBYTE:
    case TIFF_UNDEFINED:
      return 1;
    case TIFF_SHORT:
    case TIFF_SSHORT:
      return 2;
    case TIFF_LONG:
    case TIFF_SLONG:
    case TIFF_FLOAT:
    case TIFF_IFD:
      return 4;
    case TIFF_RATIONAL:
    case TIFF_SRATIONAL:
    case TIFF_DOUBLE:
    case TIFF_LONG8:
    case TIFF_SLONG8:
    case TIFF_IFD8:
      return 8;
    default:
      return 0; /* unknown type */
  }
}

int64_t parse_int64(uint8_t *buffer, bool big_endian) {
  int64_t result = 0;
  int64_t temp = 0;

  if (big_endian) {
    temp = buffer[0];
    result |= temp<<56;
    temp = buffer[1];
    result |= temp<<48;
    temp = buffer[2];
    result |= temp<<40;
    temp = buffer[3];
    result |= temp<<32;
    temp = buffer[4];
    result |= temp<<24;
    temp = buffer[5];
    result |= temp<<16;
    temp = buffer[6];
    result |= temp<<8;
    temp = buffer[7];
    result |= temp<<0;
  }  else {
    temp = buffer[7];
    result |= temp<<56;
    temp = buffer[6];
    result |= temp<<48;
    temp = buffer[5];
    result |= temp<<40;
    temp = buffer[4];
    result |= temp<<32;
    temp = buffer[3];
    result |= temp<<24;
    temp = buffer[2];
    result |= temp<<16;
    temp = buffer[1];
    result |= temp<<8;
    temp = buffer[0];
    result |= temp<<0;
  }

  return result;
}

int export_int64(int64_t num, uint8_t *buffer, bool big_endian) {
  if (big_endian) {
    buffer[0] = (num>>56);
    buffer[1] = (num>>48);
    buffer[2] = (num>>40);
    buffer[3] = (num>>32);
    buffer[4] = (num>>24);
    buffer[5] = (num>>16);
    buffer[6] = (num>>8);
    buffer[7] = (num>>0);
  } else {
    buffer[7] = (num>>56);
    buffer[6] = (num>>48);
    buffer[5] = (num>>40);
    buffer[4] = (num>>32);
    buffer[3] = (num>>24);
    buffer[2] = (num>>16);
    buffer[1] = (num>>8);
    buffer[0] = (num>>0);
  }
  return 0;
}


int16_t parse_int16(uint8_t *buffer, bool big_endian) {
  if (big_endian)
    return (buffer[1]<<0 | buffer[0]<<8);
  else
    return (buffer[0]<<0 | buffer[1]<<8);
}

int64_t read_int64(PTIFF *tiff_file, int64_t offset, bool big_endian) {
  uint8_t read_buffer[8];
  MPI_File_read_at(tiff_file->fh,
                   offset,
                   read_buffer,
                   sizeof(int64_t),
                   MPI_BYTE,
                   MPI_STATUS_IGNORE);
  return parse_int64(read_buffer, big_endian);
}

int write_int64(PTIFF *tiff_file,
                int64_t offset,
                int64_t value,
                bool big_endian) {
  uint8_t buffer[8];
  export_int64(value, buffer, big_endian);
  MPI_File_write_at(tiff_file->fh,
                    offset, buffer,
                    sizeof(int64_t),
                    MPI_BYTE,
                    MPI_STATUS_IGNORE);
  return 0;
}

SPTW_ERROR populate_tile_offsets(PTIFF *tiff_file,
                                 int64_t tile_size,
                                 bool tiled = true) {
  MPI_Status status;
  bool big_endian = false;   // Is tiff file big endian?

  // Read endianess of file
  uint8_t endian_flag[2] = { 0x49, 0x49 };
  MPI_File_read_at(tiff_file->fh, 0, endian_flag, 2, MPI_BYTE, &status);
  if (endian_flag[0] == 0x4d) {
    big_endian = true;
  }

  // Check tiff version, should be 0x002b for BigTiff
  uint8_t version_data[2];
  int16_t version = 0;
  MPI_File_read_at(tiff_file->fh, 2, version_data, 2, MPI_BYTE, &status);

  version = parse_int16(version_data, big_endian);

  if (version != 0x002b) {
    // Wrong Tiff version !
    return SP_BadArg;
  }

  // Read offset to first directory
  int64_t doffset = 0;
  doffset = read_int64(tiff_file, 8, big_endian);

  // Read number of directory entries
  int64_t entry_count = read_int64(tiff_file, doffset, big_endian);
  // directory offset + sizeof directory count
  int64_t entry_offset = doffset + sizeof(int64_t);

  int64_t tile_count = 0;

  int64_t tile_size_bytes = tile_size * tiff_file->band_count
      * tiff_file->band_type_size;
  if (tiled)
  {
	  tile_size_bytes *= tile_size;
  }

  int64_t first_tile_offset = 0;

  for (int64_t i = 0; i < entry_count; ++i) {
    // Read identifying tag of directory entry
    uint8_t tag_buffer[2];
    MPI_File_read_at(tiff_file->fh,
                     entry_offset,
                     tag_buffer,
                     2,
                     MPI_BYTE,
                     &status);
    int16_t entry_tag = parse_int16(tag_buffer, big_endian);

    // Read count of elements in entry
    int64_t element_count = read_int64(tiff_file, entry_offset+4, big_endian);

    // Read entry data
    int64_t entry_data = read_int64(tiff_file, entry_offset+12, big_endian);

    // Check if directory type is TIFFTAG_TILEOFFSETS
    if (entry_tag == TIFFTAG_TILEOFFSETS || entry_tag == TIFFTAG_STRIPOFFSETS) {
      // Read location of first_offset
      int64_t first_offset = read_int64(tiff_file, entry_data, big_endian);
      first_tile_offset = first_offset;
      tile_count = element_count;

      for (int64_t j = 1; j < element_count; ++j) {
        write_int64(tiff_file,
            entry_data+(sizeof(int64_t)*j),
            first_offset+(tile_size_bytes*j),
            big_endian);
      }
    } else if (entry_tag == TIFFTAG_TILEBYTECOUNTS || entry_tag == TIFFTAG_STRIPBYTECOUNTS) {
      for (int64_t j = 1; j < element_count; ++j) {
        write_int64(tiff_file,
                    entry_data+(sizeof(int64_t)*j),
                    tile_size_bytes,
                    big_endian);
      }
    }
    entry_offset += 20;
  }

  // Calculate end of file and write to it
  uint8_t buffer[10];
  buffer[0] = 0;
  int64_t file_size = (tile_count * tile_size_bytes) + first_tile_offset;
  MPI_File_write_at(tiff_file->fh, file_size-1, buffer, 1, MPI_BYTE, &status);
  return SP_None;
}

SPTW_ERROR create_generic_raster(string filename,
		int64_t x_size,
		int64_t y_size,
		int band_count,
		GDALDataType band_type,
		double *geotransform,
		string projection_srs,
		int block_size_x,
		bool tiled_mode) {

	GDALAllRegister();

	// Get GeoTiff driver
	GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("GTiff");

	if (driver == NULL) {
		return SP_CreateError;
	}

	// Set driver options
	char **options = NULL;
	options = CSLSetNameValue(options, "INTERLEAVE", "PIXEL");
	options = CSLSetNameValue(options, "BIGTIFF", "YES");
	options = CSLSetNameValue(options, "COMPRESS", "NONE");
	options = CSLSetNameValue(options, "SPARSE_OK", "YES");
	if (tiled_mode)
	{
		std::stringstream ts;
		ts << block_size_x;
		options = CSLSetNameValue(options, "TILED", "YES");
		options = CSLSetNameValue(options, "BLOCKXSIZE", ts.str().c_str());
		options = CSLSetNameValue(options, "BLOCKYSIZE", ts.str().c_str());
	}

	// Create output raster
	GDALDataset *output =
			driver->Create(filename.c_str(),
					x_size,
					y_size,
					band_count,
					band_type,
					options);
	if (output == NULL) {
		fprintf(stderr, "driver->Create call failed.\n");
		return SP_CreateError;
	}

	// Set GeoTransform
	output->SetGeoTransform(geotransform);

	// Set projection
	OGRSpatialReference out_sr;
	char *wkt = NULL;
	out_sr.SetFromUserInput(projection_srs.c_str());
	out_sr.exportToWkt(&wkt);
	output->SetProjection(wkt);

	// Write first and last pixel in the raster
	double *data = new double(sizeof(*data) * 4 * output->GetRasterCount());
	CPLErr rcode = output->RasterIO(GF_Write,
			0,
			0,
			1,
			1,
			data,
			1,
			1,
			output->GetRasterBand(1)->GetRasterDataType(),
			output->GetRasterCount(),
			NULL,
			0,
			0,
			0);

	if(rcode != CE_None)
	{
		delete data;

		// Clean stuff
		OGRFree(wkt);
		CSLDestroy(options);
		GDALClose(output);

		fprintf(stderr, "Error while writing first pixel in the raster with GDAL\n");
		return SP_CreateError;
	}


	rcode = output->RasterIO(GF_Write,
			x_size-1,
			y_size-1,
			1,
			1,
			data,
			1,
			1,
			output->GetRasterBand(1)->GetRasterDataType(),
			output->GetRasterCount(),
			NULL,
			0,
			0,
			0);

	if(rcode != CE_None)
	{
		delete data;

		// Clean stuff
		OGRFree(wkt);
		CSLDestroy(options);
		GDALClose(output);

		fprintf(stderr, "Error while writing last pixel in the raster with GDAL\n");
		return SP_CreateError;

	}

	delete data;

	// Clean stuff
	OGRFree(wkt);
	CSLDestroy(options);
	GDALClose(output);

	return SP_None;
}

SPTW_ERROR create_raster(string filename,
                         int64_t x_size,
                         int64_t y_size,
                         int band_count,
                         GDALDataType band_type,
                         double *geotransform,
                         string projection_srs) {
  GDALDriver *gtiff_driver = NULL;
  GDALDataset *ds = NULL;
  char **options = NULL;
  OGRSpatialReference srs;
  const char *format = "GTiff";

  GDALAllRegister();

  gtiff_driver = GetGDALDriverManager()->GetDriverByName(format);

  if (gtiff_driver == NULL) {
    return SP_CreateError;
  }

  options = CSLSetNameValue(options, "BIGTIFF", "YES");
  options = CSLSetNameValue(options, "INTERLEAVE", "PIXEL");
  options = CSLSetNameValue(options, "COMPRESS", "NONE");

  ds = gtiff_driver->Create(filename.c_str(),
                            x_size,
                            y_size,
                            band_count,
                            band_type,
                            options);
  // Clean up options
  CSLDestroy(options);

  CPLErr err = ds->SetProjection(projection_srs.c_str());

  if (err != CE_None) {
    return SP_BadArg;
  }

  ds->SetGeoTransform(geotransform);

  // Close dataset
  GDALClose((GDALDatasetH) ds);

  return SP_None;
}

SPTW_ERROR create_tiled_raster(string filename,
                               int64_t x_size,
                               int64_t y_size,
                               int band_count,
                               GDALDataType band_type,
                               double *geotransform,
                               string projection_srs,
                               int64_t tile_size) {
  GDALDriver *gtiff_driver = NULL;
  GDALDataset *ds = NULL;
  char **options = NULL;

  GDALAllRegister();

  gtiff_driver = GetGDALDriverManager()->GetDriverByName("GTiff");

  if (gtiff_driver == NULL) {
    return SP_CreateError;
  }

  std::stringstream ts;
  ts << tile_size;

  options = CSLSetNameValue(options, "BIGTIFF", "YES");
  options = CSLSetNameValue(options, "INTERLEAVE", "PIXEL");
  options = CSLSetNameValue(options, "COMPRESS", "NONE");
  options = CSLSetNameValue(options, "TILED", "YES");
  options = CSLSetNameValue(options, "BLOCKXSIZE", ts.str().c_str());
  options = CSLSetNameValue(options, "BLOCKYSIZE", ts.str().c_str());

  ds = gtiff_driver->Create(filename.c_str(),
                            x_size,
                            y_size,
                            band_count,
                            band_type,
                            options);
  // Clean up options
  CSLDestroy(options);

  CPLErr err = ds->SetProjection(projection_srs.c_str());

  if (err != CE_None) {
    return SP_BadArg;
  }

  ds->SetGeoTransform(geotransform);

  // Close dataset
  GDALClose((GDALDatasetH) ds);

  return SP_None;
}

PTIFF* open_raster(string filename) {
  PTIFF *ptiff = new PTIFF();
  char *c_filename = strdup(filename.c_str());

  GDALAllRegister();

  GDALDataset *ds = static_cast<GDALDataset*>(GDALOpen(filename.c_str(),
                                                       GA_Update));

  if (ds == NULL) {
    free(c_filename);
    return NULL;
  }

  ptiff->x_size = ds->GetRasterXSize();
  ptiff->y_size = ds->GetRasterYSize();
  ptiff->band_count = ds->GetRasterCount();
  ptiff->band_type = ds->GetRasterBand(1)->GetRasterDataType();
  ptiff->band_type_size = GDALGetDataTypeSize(ptiff->band_type)/8;
  ptiff->first_strip_offset = -1;
  ptiff->block_x_size = ptiff->x_size;
  ptiff->block_y_size = ptiff->y_size;

  GDALClose(ds);

  TIFF *tiffds = TIFFOpen(c_filename, "r");
  free(c_filename);

  if (tiffds == NULL) {
    fprintf(stderr, "Couldn't open tiff file\n");
    return NULL;
  }

  // Attempt to read TileWidth tag. If not found we assume file to have strips
  // instead.
  int64_t *offset = NULL;
  int64_t tiles_per_image = -1;
  int64_t *tiff_offsets = NULL;

  int ret = 1;
  ret &= TIFFGetField(tiffds, TIFFTAG_TILEWIDTH, &(ptiff->block_x_size));
  ret &= TIFFGetField(tiffds, TIFFTAG_TILELENGTH, &(ptiff->block_y_size));
  ret &= TIFFGetField(tiffds, TIFFTAG_TILEOFFSETS, &offset);

  if (ret == 1) {  /* for tiled tiff */
    ptiff->tiles_across = (ptiff->x_size
                           + ptiff->block_x_size - 1) / ptiff->block_x_size;
    ptiff->tiles_down = (ptiff->y_size
                         + ptiff->block_y_size - 1) / ptiff->block_y_size;
    tiles_per_image = ptiff->tiles_across * ptiff->tiles_down;
    ptiff->tile_offsets = new int64_t[tiles_per_image];
    if (ptiff->tile_offsets == NULL) {
      //  Allocation Fail!
      return NULL;
    }
    ret = TIFFGetField(tiffds, TIFFTAG_TILEOFFSETS, &tiff_offsets);
    memcpy(ptiff->tile_offsets,
           tiff_offsets,
           sizeof(int64_t) * tiles_per_image);
  } else if (ret != 1) {  /* for striped tiff */
    ret = TIFFGetField(tiffds, TIFFTAG_STRIPOFFSETS, &offset);
    if (ret != 1) {
      fprintf(stderr, "Error reading strip offsets!\n");
      return NULL;
    }
    ptiff->first_strip_offset = *offset;
    ptiff->tiles_across = 1;
    ptiff->tiles_down = ptiff->y_size;
    ptiff->block_y_size = 1;

    // add by Remi for striped images
    tiles_per_image = ptiff->tiles_across * ptiff->tiles_down;
    ptiff->tile_offsets = new int64_t[tiles_per_image];
    for (int i = 0 ; i < ptiff->y_size ; i++)
    {
    	ptiff->tile_offsets[i] = offset[0] + i*(ptiff->band_count)*(ptiff->band_type_size)*ptiff->block_x_size;
    }

  }

  ptiff->first_strip_offset = *offset;

  TIFFClose(tiffds);

  c_filename = strdup(filename.c_str());
  int rc = MPI_File_open(MPI_COMM_WORLD,
                         c_filename,
                         MPI_MODE_RDWR,
                         MPI_INFO_NULL,
                         &(ptiff->fh));

  if (rc != MPI_SUCCESS) {
    char *errstr = static_cast<char*>(malloc(5000));
    int errlen = 0;
    MPI_Error_string(rc, errstr, &errlen);
    fprintf(stderr,
            "MPI_File: Error opening file: %s: %s\n",
            c_filename,
            errstr);

    free(c_filename);
    free(errstr);
    return NULL;
  }

  MPI_File_set_atomicity(ptiff->fh, 0);

  if (c_filename != NULL) {
    free(c_filename);
  }

  return ptiff;
}

SPTW_ERROR close_raster(PTIFF *ptiff) {
  MPI_File_close(&(ptiff->fh));
  delete ptiff;
  return SP_None;
}

SPTW_ERROR fill_stack(std::vector<Area> *write_stack,
                      Area old_area,
                      Area written_subset) {
  const double size_below = old_area.lr.y - written_subset.lr.y;
  const double size_right = old_area.lr.x - written_subset.lr.x;

  if (size_right > 0.0) {
    write_stack->push_back(Area(written_subset.lr.x + 1,
                                old_area.ul.y,
                                old_area.lr.x,
                                old_area.lr.y));
  }

  if (size_below > 0.0) {
    write_stack->push_back(Area(old_area.ul.x,
                               written_subset.lr.y + 1,
                               written_subset.lr.x,
                               old_area.lr.y));
  }
  return SP_None;
}

Area calculate_tile_intersection(PTIFF *tiff_file,
                                 Area subset) {
  const double tile_x_beginning = (static_cast<int64_t>(subset.ul.x)
                                   / tiff_file->block_x_size)
      * tiff_file->block_x_size;
  const double tile_y_beginning = (static_cast<int64_t>(subset.ul.y)
                                   / tiff_file->block_y_size)
      * tiff_file->block_y_size;
  const double tile_x_end = tile_x_beginning + tiff_file->block_x_size - 1;
  const double tile_y_end = tile_y_beginning + tiff_file->block_y_size - 1;
  const double subset_lr_x = std::min(tile_x_end, subset.lr.x);
  const double subset_lr_y = std::min(tile_y_end, subset.lr.y);

  return Area(subset.ul.x,
              subset.ul.y,
              subset_lr_x,
              subset_lr_y);
}

int64_t calculate_file_offset(PTIFF *tiff,
                              const int64_t raster_x,
                              const int64_t raster_y) {
  const int64_t tile_x = raster_x % tiff->block_x_size;
  const int64_t tile_y = raster_y % tiff->block_y_size;
  const int64_t offset_into_tile = (tile_x + (tile_y * tiff->block_x_size))
      * tiff->band_type_size
      * tiff->band_count;
  const int64_t tile_index = (raster_x / tiff->block_x_size)
      + (raster_y / tiff->block_y_size) * tiff->tiles_across;

  const int64_t tiles_per_image = tiff->tiles_across * tiff->tiles_down;

  if (tile_index > tiles_per_image) {
      std::cerr << "SPTW: Writing outside of file bounds! Corruption will occur." << std::endl;
  }

  return tiff->tile_offsets[tile_index] + offset_into_tile;
}

SPTW_ERROR write_subset(PTIFF *tiff_file,
                        void *data,
                        int64_t buffer_ul_x,
                        int64_t buffer_ul_y,
                        int64_t buffer_lr_x,
                        int64_t buffer_lr_y,
                        int64_t write_ul_x,
                        int64_t write_ul_y,
                        int64_t write_lr_x,
                        int64_t write_lr_y) {
  const double tile_x_beginning = (write_ul_x / tiff_file->block_x_size)
      * tiff_file->block_x_size;
  const double tile_x_end = tile_x_beginning + tiff_file->block_x_size - 1;
  int count = 0;
  MPI_Status status;

  if (write_ul_x == tile_x_beginning
      && write_lr_x == tile_x_end) {
    // Area to be written is same width as tile, write area with single
    // operation
    count = ((write_lr_x - write_ul_x + 1) * (write_lr_y - write_ul_y + 1)
             * tiff_file->band_type_size * tiff_file->band_count);
    // Size in bytes of a row of the section of buffer being output
    const int sub_row_size = (write_lr_x - write_ul_x + 1)
        * tiff_file->band_type_size * tiff_file->band_count;

    char *buffer = new(std::nothrow) char[count];
    for (int y = write_ul_y; y <= write_lr_y; ++y) {
      int pixel_offset = ((y - buffer_ul_y) * (buffer_lr_x - buffer_ul_x + 1)
                          + (write_ul_x - buffer_ul_x))
          * tiff_file->band_type_size * tiff_file->band_count;

      memcpy(buffer+((y-write_ul_y)*sub_row_size),
             static_cast<char*>(data)+pixel_offset,
             sub_row_size);
    }
    MPI_File_write_at(tiff_file->fh,
                      calculate_file_offset(tiff_file, write_ul_x, write_ul_y),
                      buffer,
                      count,
                      MPI_BYTE,
                      &status);
    delete[] buffer;
  } else {
    // Subset of the buffer must be written row by row
    count = (write_lr_x - write_ul_x + 1)
        * tiff_file->band_type_size
        * tiff_file->band_count;

    for (int y = write_ul_y; y <= write_lr_y; ++y) {
      int pixel_offset = ((y - buffer_ul_y) * (buffer_lr_x - buffer_ul_x + 1)
                          + (write_ul_x - buffer_ul_x))
          * tiff_file->band_type_size * tiff_file->band_count;
      MPI_File_write_at(tiff_file->fh,
                        calculate_file_offset(tiff_file, write_ul_x, y),
                        (static_cast<char*>(data)) + pixel_offset,
                        count,
                        MPI_BYTE,
                        &status);
    }
  }
  return SP_None;
}

SPTW_ERROR write_area(PTIFF *ptiff,
                      void *data,
                      int64_t ul_x,
                      int64_t ul_y,
                      int64_t lr_x,
                      int64_t lr_y) {
  std::vector<Area> write_stack;
  Area write_area;
  write_area.ul = sptw::Coordinate(ul_x, ul_y, sptw::UNDEF);
  write_area.lr = sptw::Coordinate(lr_x, lr_y, sptw::UNDEF);

  write_stack.push_back(write_area);

  while (!write_stack.empty()) {
    // Pop area needing write from top of stack
    Area top = write_stack.back();
    write_stack.pop_back();

    // Calculate subset of write area that is within the UL tile
    Area subset = calculate_tile_intersection(ptiff, top);

    // Fill the stack with any leftover areas
    fill_stack(&write_stack, top, subset);

    // Finally write the tile-bound subset
    write_subset(ptiff,
                 data,
                 ul_x,
                 ul_y,
                 lr_x,
                 lr_y,
                 subset.ul.x,
                 subset.ul.y,
                 subset.lr.x,
                 subset.lr.y);
  }
  return SP_None;
}
}
