#include <gdal.h>
#include <gdal_priv.h>
#include <cpl_string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
  GDALAllRegister();

  const char *pszFormat = argv[3];

  GDALDriverH hDriver = GDALGetDriverByName( pszFormat );

  if( hDriver == NULL ) {
  printf ("gdalCreateCopyTest.cxx: hDriver == NULL ");
  return EXIT_FAILURE;
  }

  char **papszCreateOptions = NULL;

  if( argc == 5 ) {
  papszCreateOptions = CSLAddString( papszCreateOptions, argv[4] );
  }

  char **papszMetadata;
  papszMetadata = GDALGetMetadata( hDriver, NULL );
  if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATECOPY, FALSE ) )
    printf( "Driver %s supports CreateCopy() method.\n", pszFormat );

  GDALDatasetH hSrcDS = GDALOpen( argv[1], GA_ReadOnly );
  if( hSrcDS == NULL ) {
  printf ("gdalCreateCopyTest.cxx: hSrcDS == NULL ");
  return EXIT_FAILURE;
  }

  GDALDatasetH hDstDS;
  hDstDS = GDALCreateCopy( hDriver, argv[2], hSrcDS, FALSE,   NULL, NULL, NULL );

  if( hDstDS == NULL )
    {
    printf ("gdalCreateCopyTest.cxx: hDstDS == NULL ");
    return EXIT_FAILURE;
    }


/* Once we're done, close properly the dataset */
  if( hDstDS != NULL )
    GDALClose( hDstDS );

  GDALClose(hSrcDS);

  return EXIT_SUCCESS;

}
