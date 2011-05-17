/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>
#include <fstream>

#include "gdal_priv.h"
#include "cpl_string.h"
#include "ogr_spatialref.h"


int otbGDALImageIOTestWriteMetadata(int argc, char * argv[])
{
  // Parameters
  char* pszDstFilename= argv[1];
  bool setGCP = false;
  bool setProjectionRef = false;
  bool setGeoTransform_ID = false;
  bool setGeoTransform = false;
  std::cout << argc <<std::endl;
  for (unsigned int itOption = 0 ; itOption < (unsigned int)argc-4 ; itOption++)
    {
    std::string arg = argv[itOption+4];
    std::cout << "arg: " << arg << std::endl;
    if ( arg.compare("-setGCP") == 0 )
      {
      std::cout << "-setGCP" << std::endl;
      setGCP = true;
      }
    if (arg.compare("-setGeoTransform_ID") == 0)
      {
      std::cout << "-setGeoTransform_ID" << std::endl;
      setGeoTransform_ID = true;
      }
    if (arg.compare("-setGeoTransform") == 0)
      {
      std::cout << "-setGeoTransform" << std::endl;
      setGeoTransform = true;
      }
    if (arg.compare("-setProjectionRef") == 0)
      {
      std::cout << "-setProjectionRef" << std::endl;
      setProjectionRef = true;
      }
    }

  const char *pszFormat;// = "GTiff";
  // Detect format with the extension
  std::string filename = pszDstFilename;
  if ( filename.compare(filename.size()-4, 4,".tif") == 0 )
    {
    pszFormat = "GTiff";
    }
  else if (filename.compare(filename.size()-4,4,".hdr") == 0 )
    {
    pszFormat = "ENVI";
    filename.erase(filename.size()-4,4);
    std::cout << filename <<std::endl;
    }
  else
    {
    return EXIT_FAILURE;
    }

  std::cout << "format: " << pszFormat <<std::endl;

  // Check the driver used
  GDALDriver *poDriver;
  char **papszMetadata;

  GDALAllRegister();
  poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);

  if( poDriver == NULL )
    {
    std::cout << "impossible to get the driver" <<std::endl;
    return EXIT_FAILURE;
    }

  // Format selected support create method ?
  papszMetadata = poDriver->GetMetadata();
  if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE ) )
    {
    std::cout << "Driver " << pszFormat << " supports Create() method." << std::endl;
    }
  else
    {
    std::cout << "Driver " << pszFormat << " does not support Create() method." << std::endl;
    return EXIT_FAILURE;
    }

  GDALDataset *poDstDS;
  char **papszOptions = NULL;

  // Create dataset
  poDstDS = poDriver->Create( filename.c_str(), 128, 128, 1, GDT_Byte,
                              papszOptions );

  // Set ProjectionRef
  OGRSpatialReference oSRS;
  char *pszSRS_WKT = NULL;
  oSRS.SetWellKnownGeogCS( "WGS84" );
  oSRS.exportToWkt( &pszSRS_WKT );
  if (setProjectionRef)
  {
    poDstDS->SetProjection( pszSRS_WKT );
  }

  // Set GeoTransform
  if (setGeoTransform_ID || setGeoTransform)
    {
    double adfGeoTransform[6] = { 0, 1, 0, 0, 0, 1 };
    if (setGeoTransform)
      {
      adfGeoTransform[0] = 64;
      adfGeoTransform[3] = 32;
      }
    poDstDS->SetGeoTransform( adfGeoTransform );
    }

  // Set GCP
  if (setGCP)
    {
    unsigned int gcpCount = 1;
    GDAL_GCP * gdalGcps = new GDAL_GCP[gcpCount];

    for (unsigned int gcpIndex = 0; gcpIndex < gcpCount; ++gcpIndex)
      {
      gdalGcps[gcpIndex].pszId = const_cast<char *> ("1");
      gdalGcps[gcpIndex].pszInfo = const_cast<char *> ("");
      gdalGcps[gcpIndex].dfGCPPixel = 0.5;
      gdalGcps[gcpIndex].dfGCPLine = 0.5;
      gdalGcps[gcpIndex].dfGCPX = 1.35409622400109;
      gdalGcps[gcpIndex].dfGCPY = 43.6544262050905;
      gdalGcps[gcpIndex].dfGCPZ = 0;

      }
    poDstDS->SetGCPs(gcpCount, gdalGcps, pszSRS_WKT);
    }

  // Write data into dataset
  GDALRasterBand *poBand;
  GByte abyRaster[128*128];
  for (unsigned int it=0; it < 128*128; it++)
    {
    abyRaster[it] = 0;
    }

  poBand = poDstDS->GetRasterBand(1);
  poBand->RasterIO( GF_Write, 0, 0, 128, 128,
                    abyRaster, 128, 128, GDT_Byte, 0, 0 );


  // Get some informations from dataset
  std::ofstream fileCreate;
  fileCreate.open(argv[2]);
  fileCreate << "Projection Ref: "<< poDstDS->GetProjectionRef() << "\n" ;
  double GT_Create[6];
  if (poDstDS->GetGeoTransform(GT_Create) == CE_None)
    {
    fileCreate <<"GeoTransform: "<< GT_Create[0] << ", " << GT_Create[1] << ", "
                                 << GT_Create[2] << ", " << GT_Create[3] << ", "
                                 << GT_Create[4] << ", " << GT_Create[5] << "\n" ;
    }
  else
    {
    fileCreate << "No GeoTransform \n" ;
    }

  fileCreate << "GCP Projection Ref: "<< poDstDS->GetGCPProjection()<< "\n" ;
  fileCreate << "GCP Count: " << poDstDS->GetGCPCount()<< "\n" ;
  const GDAL_GCP * gdalGcpCreate  = poDstDS->GetGCPs();
  for (int gcpIndex = 0; gcpIndex < poDstDS->GetGCPCount(); ++gcpIndex)
    {
    fileCreate << "GCP[" << gcpIndex <<"].pszId: "     << gdalGcpCreate->pszId<< "\n" ;
    fileCreate << "GCP[" << gcpIndex <<"].pszInfo: "   << gdalGcpCreate->pszInfo<< "\n" ;
    fileCreate << "GCP[" << gcpIndex <<"].dfGCPPixel: "<< gdalGcpCreate->dfGCPPixel<< "\n" ;
    fileCreate << "GCP[" << gcpIndex <<"].dfGCPLine: " << gdalGcpCreate->dfGCPLine<< "\n" ;
    fileCreate << "GCP[" << gcpIndex <<"].m_GPCX: "    << gdalGcpCreate->dfGCPX<< "\n" ;
    fileCreate << "GCP[" << gcpIndex <<"].m_GPCY: "    << gdalGcpCreate->dfGCPY<< "\n" ;
    fileCreate << "GCP[" << gcpIndex <<"].m_GPCZ: "    << gdalGcpCreate->dfGCPZ<< "\n" ;
    }
  fileCreate.close();


  // Once we're done, close properly the dataset
  GDALClose( (GDALDatasetH) poDstDS );
  CPLFree( pszSRS_WKT );

  //**************
  // Open File
  GDALDataset  *poDataset;

  poDataset = (GDALDataset *) GDALOpen( filename.c_str(), GA_ReadOnly );
  if( poDataset == NULL )
     return EXIT_FAILURE;

  // Get some informations from file
  std::ofstream fileWriteRead;
  fileWriteRead.open(argv[3]);
  fileWriteRead << "Projection Ref: "<< poDataset->GetProjectionRef()<< "\n"  ;
  double GT[6];
  if (poDataset->GetGeoTransform(GT) == CE_None)
    {
    fileWriteRead <<"GeoTransform: "<< GT[0] << ", " << GT[1] << ", "
                                    << GT[2] << ", " << GT[3] << ", "
                                    << GT[4] << ", " << GT[5] << "\n" ;
    }
  else
    {
    fileWriteRead << "No GeoTransform \n" ;
    }

  fileWriteRead << "GCP Projection Ref: "<< poDataset->GetGCPProjection()<< "\n" ;
  fileWriteRead << "GCP Count: " << poDataset->GetGCPCount()<< "\n" ;
  const GDAL_GCP * gdalGcp  = poDataset->GetGCPs();
  for (int gcpIndex = 0; gcpIndex < poDataset->GetGCPCount(); ++gcpIndex)
    {
    fileWriteRead << "GCP[" << gcpIndex <<"].pszId: "     << gdalGcp->pszId<< "\n" ;
    fileWriteRead << "GCP[" << gcpIndex <<"].pszInfo: "   << gdalGcp->pszInfo<< "\n" ;
    fileWriteRead << "GCP[" << gcpIndex <<"].dfGCPPixel: "<< gdalGcp->dfGCPPixel<< "\n" ;
    fileWriteRead << "GCP[" << gcpIndex <<"].dfGCPLine: " << gdalGcp->dfGCPLine<< "\n" ;
    fileWriteRead << "GCP[" << gcpIndex <<"].m_GPCX: "    << gdalGcp->dfGCPX<< "\n" ;
    fileWriteRead << "GCP[" << gcpIndex <<"].m_GPCY: "    << gdalGcp->dfGCPY<< "\n" ;
    fileWriteRead << "GCP[" << gcpIndex <<"].m_GPCZ: "    << gdalGcp->dfGCPZ<< "\n" ;
    }
  fileWriteRead.close();

  GDALClose(poDataset);
  return EXIT_SUCCESS;
}
