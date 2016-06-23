#include <gdal.h>
#include <stdio.h>

/* http://www.gdal.org/formats_list.html */

int main(int argc, char * argv[])
{
  GDALDriverH hDriver = NULL;
  if (argc < 2) {
    printf( "Usage : gdalFormatsTest  driverName.\n");
    return EXIT_FAILURE;;
  }

  GDALAllRegister();

  hDriver = GDALGetDriverByName( argv[1] );
  if( hDriver == NULL ) {
  printf( "No driver for input name '%s' ", argv[1]);
  return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
