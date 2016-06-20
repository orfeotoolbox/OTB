#include <gdal.h>
#include <stdio.h>

/* http://www.gdal.org/formats_list.html */

int main(int argc, char * argv[])
{

  if (argc < 2) {
    printf( "gdalFormatsTest: incorrect usage.\n");
     exit(1);
  }

  GDALAllRegister();

  const char *pszFormat = argv[1];
  GDALDriverH hDriver = GDALGetDriverByName( pszFormat );
  char **papszMetadata;
  if( hDriver == NULL ) {
  printf( "hDriver == NULL for input format '%s' ", pszFormat);
  exit( 1 );
  }

  return EXIT_SUCCESS;

}
