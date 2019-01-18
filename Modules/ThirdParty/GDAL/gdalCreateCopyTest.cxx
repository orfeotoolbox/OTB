/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
