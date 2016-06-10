#include <gdal.h>
#include <gdal_priv.h>
#include <cpl_string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
  GDALAllRegister();

  GDALDriverH hDriver = GDALGetDriverByName( argv[3] );

  if( hDriver == NULL ) {
  return EXIT_FAILURE;
  }

  char **papszCreateOptions = NULL;

  if( argc == 5 ) {
  papszCreateOptions = CSLAddString( papszCreateOptions, argv[4] );
  }

  GDALDatasetH hSrcDS = GDALOpen( argv[1], GA_ReadOnly );
  if( hSrcDS == NULL ) {
  return EXIT_FAILURE;
  }

  GDALDatasetH hDstDS;
  hDstDS = GDALCreateCopy( hDriver, argv[2], hSrcDS, FALSE,   NULL, NULL, NULL );

  if( hDstDS == NULL )
    return EXIT_FAILURE;
}

/* Once we're done, close properly the dataset */
if( hDstDS != NULL )
  GDALClose( hDstDS );

GDALClose(hSrcDS);

return EXIT_SUCCESS;

}
