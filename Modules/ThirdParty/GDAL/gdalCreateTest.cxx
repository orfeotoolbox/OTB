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

#include <iostream>
#include <stdexcept>
#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_spatialref.h>


int main(int argc, char * argv[])
{
  const char *pszFormat = argv[1];
  char **papszMetadata;

	GDALAllRegister();

	// ------------------- step 1 -------------------

  GDALDriverH hDriver = GDALGetDriverByName( pszFormat );

  if( hDriver == NULL ) {
  printf("poDriver == NULL\n" );
  return EXIT_FAILURE;
  }

  papszMetadata = GDALGetMetadata( hDriver, NULL );

  if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE ) )
    printf( "Driver %s supports Create() method.\n", pszFormat );
  else {
  printf( "Driver %s does supports Create() method.\n", pszFormat );
  return EXIT_FAILURE;
  }

  // ------------------- step 2 -------------------

  GDALDatasetH hDstDS;
  char **papszOptions = NULL;
  hDstDS = GDALCreate( hDriver, argv[2], 512, 512, 1, GDT_Byte,
                       papszOptions );

  // ------------------- step 3 -------------------

  double adfGeoTransform[6] = { 444720, 30, 0, 3751320, 0, -30 };
  OGRSpatialReferenceH hSRS;
  char *pszSRS_WKT = NULL;
  GDALRasterBandH hBand;
  GByte abyRaster[512*512];
  GDALSetGeoTransform( hDstDS, adfGeoTransform );
  hSRS = OSRNewSpatialReference( NULL );
  OSRSetUTM( hSRS, 11, TRUE );
  OSRSetWellKnownGeogCS( hSRS, "NAD27" );
  OSRExportToWkt( hSRS, &pszSRS_WKT );
  OSRDestroySpatialReference( hSRS );
  GDALSetProjection( hDstDS, pszSRS_WKT );
  CPLFree( pszSRS_WKT );
  hBand = GDALGetRasterBand( hDstDS, 1 );
  GDALRasterIO( hBand, GF_Write, 0, 0, 512, 512,
                abyRaster, 512, 512, GDT_Byte, 0, 0 );
	// ------------------- step 4 -------------------

/* Once we're done, close properly the dataset */
  GDALClose( hDstDS );

  return EXIT_SUCCESS;// SUCCESS
}
