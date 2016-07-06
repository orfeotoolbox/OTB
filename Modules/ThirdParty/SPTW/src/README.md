SPTW - Simple Parallel TIFF Writer
==================================

The Simple Parallel TIFF Writer (SPTW) provides a useful interface for writing 
GeoTIFF files using MPI-IO.

In traditional parallel processing applications, individual output rasters are
created per processor because GDAL does not support parallel operation. This
adds unnecessary complexity since these rasters must be recombined at the end.  
    
SPTW provides sequential consistency only when all write
operations are nonconflicting, that is, each write operation accesses a distinct
section of the raster file.

In the
[prasterblater-pio](https://github.com/dmm/prasterblaster/blob/master/src/demos/prasterblaster-pio.cc)
demo program, each pixel of
the output raster is contained in only one partition and each partition is only
assigned to one process. This ensures all write accesses are nonconflicting and
that sequential consistency is maintained.

### API

* open_raster
* populate_tile_offsets
* write_area
* close_raster

Example usage can be found in examples/test.cpp

### Possible improvements

SPTW achieves sequential consistency but it is almost certainly not optimal.
Possble improvements include:

#### Collective Operations

Currently all file writes are done with non-collective operations. MPI I/O
supports collective calls in which use a shared file pointer and file accesses
are coordinated among processes.

#### Using MPI I/O for read operations

SPTW is only used to write the output file in the prasterblasterpio demo
program. The file reads are done with the standard I/O functions. The use
of collective reads may be more efficient.
