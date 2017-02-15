#include <iostream>
#include <cstring>
#include <cmath>

#include <ogr_spatialref.h>

#include "../sptw.h"

int main(int argc, char *argv[])
{
    const int TILE_SIZE = 16;
    const int RASTER_WIDTH = 1234;
    const int RASTER_HEIGHT = 5566;

    MPI_Init(&argc, &argv);

    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " raster.tiff" << std::endl;
        return -1;
    }

    int rank = 0, process_count = 1;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    string raster_fn = argv[1];

    if (rank == 0) {
        std::cout << "Creating raster " << raster_fn << std::endl;

        OGRSpatialReference srs;
        char *wkt;
        srs.SetFromUserInput("EPSG:4326");
        srs.exportToPrettyWkt(&wkt);

        double geotransform[6] = {1.0, 0.1, 0, 40, 0, 0.1};

        sptw::SPTW_ERROR err = sptw::create_tiled_raster(raster_fn, RASTER_WIDTH, RASTER_HEIGHT, 1,
                GDT_Byte, geotransform, wkt, TILE_SIZE); 

        if (err != sptw::SP_None) {
            std::cout << "Error creating raster" << std::endl;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // We must open the raster in all the ranks
    // since it is a collective operation

    std::cout << "Opening raster" << std::endl;
    sptw::PTIFF* tmp = sptw::open_raster(raster_fn);

    if (rank == 0) {
        std::cout << "Populating tile offsets" << std::endl;

        sptw::SPTW_ERROR err = sptw::populate_tile_offsets(tmp, TILE_SIZE);

        if (err != sptw::SP_None) {
            std::cout << "Error populating tile offsets" << std::endl;
        }
    }

    sptw::close_raster(tmp);

    MPI_Barrier(MPI_COMM_WORLD);

    sptw::PTIFF* raster = sptw::open_raster(raster_fn);

    uint8_t tile_data[TILE_SIZE*TILE_SIZE];
    memset(tile_data, rank * 255 / process_count, TILE_SIZE*TILE_SIZE);

    int horizontal_tiles = std::ceil((double) RASTER_WIDTH / TILE_SIZE);
    int vertical_tiles = std::ceil((double) RASTER_HEIGHT / TILE_SIZE);
    int num_tiles = horizontal_tiles * vertical_tiles;

    int tiles_per_rank = std::ceil((double)num_tiles / process_count); 

    std::cout << "Rank " << rank << " writing raster (" << tiles_per_rank << " tiles)" << std::endl;
   
    for(int i = 0; i < tiles_per_rank; i++)
    {
        int tile = i*process_count + rank;

        int x = tile % horizontal_tiles;
        int y = tile / horizontal_tiles;

        x *= TILE_SIZE;
        y *= TILE_SIZE;

        int w = std::min(TILE_SIZE, RASTER_WIDTH - x) - 1;
        int h = std::min(TILE_SIZE, RASTER_HEIGHT - y) - 1;

        //printf("Writing (%d,%d) -> (%d, %d)\n", x, y, x+w, y+h);
        sptw::write_area(raster, tile_data, x, y, x+w, y+h);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    sptw::close_raster(raster);

    MPI_Finalize();

    return 0;
}
