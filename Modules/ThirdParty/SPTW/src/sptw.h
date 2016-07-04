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
 * Header for the Simple Parallel Tiff Writer
 *
 */


#ifndef SRC_DEMOS_SPTW_H_
#define SRC_DEMOS_SPTW_H_

#include <gdal_priv.h>
#include <mpi.h>

#include <string>
#if HAVE_STDINT_H == 0
#include <cstdint>
#else
#include <stdint.h>
#endif

using std::string;

/*! Simple parallel tiff writer namespace
 *
 * The functions declared here allow certain tiff files to be written to in
 * parallel by multiple processors by MPI-IO.
 *
 * Sequential consistency is guaranteed if the tiff file being written to is
 * stored row-wise, uses pixel interleaving and has its strips store
 * sequentially in the file, and no process writes to the same pixel
 * values. Files created with the create_raster function or with the
 * librasterblaster::ProjectedRaster::CreateRaster function should have the
 * required attributes.
 *
 * Two functions are provided to write to tiff files: write_rows and
 * ::write_subrow . write_rows allows multiple rows to be written at
 * once. ::write_subrows writes a portion of a row.
 *
 */
namespace sptw {
    /*!
     * This enum lists the possible errors used by sptw.
     */
    enum SPTW_ERROR {
        SP_None, /*!< No Error occurred */
        SP_CreateError, /*!< Error creating the file */
        SP_WriteError, /*!< Error Writing to the file */
        SP_BadArg, /*!< A bad argument was provided */
    };

    /**
     * @struct PTIFF sptw.h
     * @brief PTIFF struct represents an open parallel tiff file
     *
     *
     *
     */
    struct PTIFF {
        /*! MPI-IO file handle. This should be changed with open_raster or
         *  close_raster */
        MPI_File fh;
        /*! Size of the opened raster in the x dimension */
        int64_t x_size;
        /*! Size of the opened raster in the y dimension */
        int64_t y_size;
        /*! Number of bands in the raster */
        int band_count;
        /*! Datatype of the band values */
        GDALDataType band_type;
        /*! Size in bytes of the band_type */
        int band_type_size;
        /*! Byte offset to the first strip of the tiff file */
        int64_t first_strip_offset;
        int64_t *tile_offsets;
        /*! Width of each tile */
        int64_t block_x_size;
        /* Height of each tile or strip */
        int64_t block_y_size;
        /* Number of tiles across raster */
        int64_t tiles_across;
        /* Number of tiles down raster */
        int64_t tiles_down;
    };

    SPTW_ERROR populate_tile_offsets(PTIFF *tiff_file,
            int64_t tile_size,
            bool tiled);

    SPTW_ERROR create_generic_raster(string filename,
    		int64_t x_size,
    		int64_t y_size,
    		int band_count,
    		GDALDataType band_type,
    		double *geotransform,
    		string projection_srs,
    		bool tiled_mode,
    		int tiles_size);

    SPTW_ERROR create_raster(string filename,
            int64_t x_size,
            int64_t y_size,
            int band_count,
            GDALDataType band_type,
            double *geotransform,
            string projection_srs);

    SPTW_ERROR create_tiled_raster(string filename,
            int64_t x_size,
            int64_t y_size,
            int band_count,
            GDALDataType band_type,
            double *geotransform,
            string projection_srs,
            int64_t tile_size);
    PTIFF* open_raster(string filename);
    SPTW_ERROR close_raster(PTIFF *ptiff);

    /**
     * @brief
     * This function writes the given buffer to the open PTIFF. The
     * area to be written is specified by the bounding box, in y-down,
     * raster coordinates. The bounding box coordinates are inclusive,
     * the point (lr_x, lr_y) will be written.
     *
     * @param ptiff The open PTIFF file to be written to
     * @param data buffer containing, row-wise, pixel interleaved data to be
     *        written to the file
     * @param ul_x Upper-left, inclusive, y-down, x coordinate of the area to be
     *             written
     * @param ul_y Upper-left, inclusive, y-down, y coordinate of the area to be
     *             written
     * @param lr_x Lower-right, inclusive, y-down, x coordinate of the area to be
     *             written
     * @param _y Lower-right, inclusive, y-down, y coordinate of the area to be
     *             written
     *
     */
    SPTW_ERROR write_area(PTIFF *ptiff,
            void *data,
            int64_t ul_x,
            int64_t ul_y,
            int64_t lr_x,
            int64_t lr_y);
}

#endif  // SRC_DEMOS_SPTW_H_
