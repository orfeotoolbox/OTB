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
#include <sstream>

#include "gdal_priv.h"
#include "cpl_string.h"
#include "ogr_spatialref.h"
#include "itkMacro.h"

// Class to store ProjRef, GEOTransform and GCP info
class DatasetInfoGDAL
{
public:
  std::string m_Name;
  std::string m_FileType;

  std::string m_ProjRef;

  CPLErr m_GeoTransfo;
  double m_GeoTransfoParam[6];

  std::string m_GCPProjRef;
  int m_GCPCount;
  std::vector<GDAL_GCP> m_GCPs;

  DatasetInfoGDAL(){
    this->Clear();
  };

  void Clear()
  {
    m_Name.clear();
    m_FileType.clear();
    m_ProjRef.clear();
    m_GeoTransfo = CE_None;
    m_GeoTransfoParam[0] = 0; m_GeoTransfoParam[1] = 0; m_GeoTransfoParam[2] = 0;
    m_GeoTransfoParam[3] = 0; m_GeoTransfoParam[4] = 0; m_GeoTransfoParam[5] = 0;
    m_GCPProjRef.clear();
    m_GCPCount = 0;
    m_GCPs.clear();
  };

  void addGCP(const GDAL_GCP* psGCP)
  {
    GDAL_GCP GCP;

    std::string strID = std::string(psGCP->pszId);
    GCP.pszId = new char [strID.size()+1];
    strcpy (GCP.pszId, strID.c_str());

    std::string strInfo = std::string(psGCP->pszInfo);
    GCP.pszInfo = new char [strInfo.size()+1];
    strcpy (GCP.pszInfo, strInfo.c_str());

    GCP.dfGCPLine = psGCP->dfGCPLine;
    GCP.dfGCPPixel = psGCP->dfGCPPixel;
    GCP.dfGCPX = psGCP->dfGCPX;
    GCP.dfGCPY = psGCP->dfGCPY;
    GCP.dfGCPZ = psGCP->dfGCPZ;

    m_GCPs.push_back(GCP);
  };

  ~DatasetInfoGDAL(){  };

  void Print(std::ostream& os) const {
    os << "------" << std::endl;
    os << "Name: " << m_Name << std::endl;
    os << "File type: " << m_FileType << std::endl;
    os << "Projection Ref: " << this->m_ProjRef <<std::endl;

    if (this->m_GeoTransfo == CE_None)
      {
      os << "GeoTransform: " << this->m_GeoTransfoParam[0] << ", "
                             << this->m_GeoTransfoParam[1] << ", "
                             << this->m_GeoTransfoParam[2] << ", "
                             << this->m_GeoTransfoParam[3] << ", "
                             << this->m_GeoTransfoParam[4] << ", "
                             << this->m_GeoTransfoParam[5] << std::endl;
      }
    else
      {
      os << "No Geo Transform" << std::endl;
      }

    os << "GCP Projection Ref: " << this->m_GCPProjRef <<std::endl;
    os << "GCP count: " << this->m_GCPCount << std::endl;
   /* for (int it = 0; it<this->m_GCPCount; ++it)
      {
      os << "   GCP Id = " << this->m_GCPs[it].pszId << std::endl;
      os << "   GCP Info =  " << this->m_GCPs[it].pszInfo << std::endl;
      os << "   GCP (Row, Col) = (" << this->m_GCPs[it].dfGCPPixel << ","
                                    << this->m_GCPs[it].dfGCPLine << ")" << std::endl;
      os << "   GCP (X, Y, Z) = (" << this->m_GCPs[it].dfGCPX << ","
                                 << this->m_GCPs[it].dfGCPY << ","
                                 << this->m_GCPs[it].dfGCPZ << ")" << std::endl;
      }*/

  };
};

bool writeReadDatasetMetadata(std::string filename, std::vector<std::string> options,
                              DatasetInfoGDAL *infoDatasetCreate,
                              DatasetInfoGDAL *infoDatasetWR);

int compareDatasetInfoGDAL(DatasetInfoGDAL *create, DatasetInfoGDAL *WR, std::ostream& os);

void testWriteMetadata(std::string filenameTIFF,
                       std::string filenameHDR,
                       std::vector<std::string> options,
                       std::ostream& os);
///////
// MAIN
int otbGDALImageIOTestWriteMetadata(int itkNotUsed(argc), char * argv[])
{
  // Parameters
  std::string filenameTIFF = argv[1];
  std::string filenameHDR = argv[2];
  std::string filenameReport = argv[3];

  // Object where store options about setProjRef, setGeoTransfo, setGCPs
  std::vector<std::string> options;

  // Open file where save test result
  std::ofstream file;
  file.open(filenameReport.c_str());

  // None
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // NoProjRef, NoGCP, GeoTransfo_ID
  options.push_back("-setGeoTransform_ID");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // NoProjRef, GCP, GeoTransfo_ID
  options.push_back("-setGCP");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // NoProjRef, NoGCP, GeoTransfo
  options.clear();
  options.push_back("-setGeoTransform");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // NoProjRef, GCP, GeoTransfo
  options.push_back("-setGCP");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // NoProjRef, GCP, No GeoTransfo
  options.clear();
  options.push_back("-setGCP");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  file << "==================================================" << std::endl;
  //////////
  // ProjRef, NoGCP, No GeoTransfo_ID
  options.clear();
  options.push_back("-setProjectionRef");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRef, NoGCP, GeoTransfo_ID
  options.push_back("-setGeoTransform_ID");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRef, GCP, GeoTransfo_ID
  options.push_back("-setGCP");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRef, NoGCP, GeoTransfo
  options.clear();
  options.push_back("-setProjectionRef");
  options.push_back("-setGeoTransform");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRef, GCP, GeoTransfo
  options.push_back("-setGCP");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRef, GCP, No GeoTransfo
  options.clear();
  options.push_back("-setProjectionRef");
  options.push_back("-setGCP");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  file << "==================================================" << std::endl;
  //////////
  // ProjRefUTM, NoGCP, No GeoTransfo_ID
  options.clear();
  options.push_back("-setProjectionRef_UTM");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRefUTM, NoGCP, GeoTransfo_ID
  options.push_back("-setGeoTransform_ID");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRefUTM, GCP, GeoTransfo_ID
  options.push_back("-setGCP");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRefUTM, NoGCP, GeoTransfo
  options.clear();
  options.push_back("-setProjectionRef_UTM");
  options.push_back("-setGeoTransform");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRefUTM, GCP, GeoTransfo
  options.push_back("-setGCP");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);

  // ProjRefUTM, GCP, No GeoTransfo
  options.clear();
  options.push_back("-setProjectionRef_UTM");
  options.push_back("-setGCP");
  testWriteMetadata(filenameTIFF, filenameHDR, options, file);


  file.close();
  return EXIT_SUCCESS;
}

int compareDatasetInfoGDAL(DatasetInfoGDAL *create, DatasetInfoGDAL *WR, std::ostream& os)
{
  os << "*****COMPARE*****" << std::endl;
  int outputCode = 0;

  // Compare file type
  if (create->m_FileType.compare(WR->m_FileType) != 0)
    {
    os << "not the same filetype" <<std::endl;
    return -1;
    }

  // compare proj ref
  if (create->m_ProjRef.compare(WR->m_ProjRef) != 0)
    {
    os << "not the same ProjRef" << std::endl;
    outputCode = 1;
    }
  else if (create->m_ProjRef.empty() && WR->m_ProjRef.empty() )
    {
    os << "Same ProjRef (No ProjRef)" <<std::endl;
    }
  else
    {
    os << "Same ProjRef" <<std::endl;
    }

  // compare Geo Transfo
  if ( (create->m_GeoTransfo == 0 ) && ( WR->m_GeoTransfo == 0) )
    {
    if ( (create->m_GeoTransfoParam[0] == WR->m_GeoTransfoParam[0]) && (create->m_GeoTransfoParam[1] == WR->m_GeoTransfoParam[1]) &&
         (create->m_GeoTransfoParam[2] == WR->m_GeoTransfoParam[2]) && (create->m_GeoTransfoParam[3] == WR->m_GeoTransfoParam[3]) &&
         (create->m_GeoTransfoParam[4] == WR->m_GeoTransfoParam[4]) &&
         ( (create->m_GeoTransfoParam[5] == WR->m_GeoTransfoParam[5]) || (create->m_GeoTransfoParam[5] == -WR->m_GeoTransfoParam[5]) )  )
      {
      os << "Same GeoTranform" <<std::endl;
      }
    else
      {
      os << "not the same GeoTransform (Parameters)" <<std::endl;
      outputCode = 10 + outputCode;
      }

    }
  else if ( (create->m_GeoTransfo > 0 ) && ( WR->m_GeoTransfo > 0) )
   {
   os << "Same GeoTransform (No GeoTransform)" <<std::endl;
   }
  else
    {
    if ( (create->m_GeoTransfoParam[0] == 0) && (create->m_GeoTransfoParam[1] == 1) && (create->m_GeoTransfoParam[2] == 0) &&
         (create->m_GeoTransfoParam[3] == 0) && (create->m_GeoTransfoParam[4] == 0) && (create->m_GeoTransfoParam[5] == 1) &&
         WR->m_GeoTransfo > 0 )
      {
      os << "Try to write a GeoTransfo = ID but the field is empty after write/read operation => \n"
                << "\t OTB consider this case as GeoTransfo = ID so not a problem" <<std::endl;
      }
    else if ( (WR->m_GeoTransfoParam[0] == 0) && (WR->m_GeoTransfoParam[1] == 1) && (WR->m_GeoTransfoParam[2] == 0) &&
              (WR->m_GeoTransfoParam[3] == 0) && (WR->m_GeoTransfoParam[4] == 0) && (WR->m_GeoTransfoParam[5] == -1) &&
              create->m_GeoTransfo > 0 )
      {
      os << "Try to write a No GeoTransfo but the field is set to ID empty after write/read operation => \n"
                      << "\t we  can consider this case as not a problem" <<std::endl;
      }
    else
      {
      os << "not the same GeoTransform" <<std::endl;
      outputCode = 100 + outputCode;
      }
    }

  // Compare GCP info
  if (create->m_GCPProjRef.compare(WR->m_GCPProjRef) == 0)
    {
    os << "Same GCP ProjRef" << std::endl;
    }
  else
    {
    os << "Not the same GCP ProjRef" << std::endl;
    outputCode = 1000 + outputCode;
    }

  if (create->m_GCPCount == WR->m_GCPCount)
    {
    os << "Same GCP count" << std::endl;
    }
  else
    {
    os << "Not the same GCP count" << std::endl;
    outputCode = 10000 + outputCode;
    }

  return outputCode;
}

void testWriteMetadata(std::string filenameTIFF,
                       std::string filenameHDR,
                       std::vector<std::string> options,
                       std::ostream& os)
{
  DatasetInfoGDAL* infoDatasetCreate_TIFF = new DatasetInfoGDAL();
  DatasetInfoGDAL* infoDatasetWR_TIFF = new DatasetInfoGDAL();
  DatasetInfoGDAL* infoDatasetCreate_HDR = new DatasetInfoGDAL();
  DatasetInfoGDAL* infoDatasetWR_HDR = new DatasetInfoGDAL();

  os << "\n+++++++++++++++++++++++++++" << std::endl;
  if (options.size() == 0)
    {
    os << "None" << std::endl;
    filenameTIFF.insert(filenameTIFF.size()-4,"-None");
    filenameHDR.insert(filenameHDR.size()-4,"-None");
    }
  else
    {
    for(size_t it = 0; it < options.size(); it ++)
      {
      os << options[it] << std::endl;
      filenameTIFF.insert(filenameTIFF.size()-4, options[it]);
      filenameHDR.insert(filenameHDR.size()-4, options[it]);
      }
    }

  os << "\nFormat: TIFF"  <<std::endl;
  writeReadDatasetMetadata(filenameTIFF, options, infoDatasetCreate_TIFF, infoDatasetWR_TIFF);
  int msg = compareDatasetInfoGDAL(infoDatasetCreate_TIFF, infoDatasetWR_TIFF, os);
  if ( msg > 0 )
    {
    os << "output code: " << msg << std::endl;
    infoDatasetCreate_TIFF->Print(os);
    infoDatasetWR_TIFF->Print(os);
    }
  else
    {
    os << "=> No Diff" <<std::endl;
    }

  os << "\nFormat: HDR"  <<std::endl;
  writeReadDatasetMetadata(filenameHDR, options, infoDatasetCreate_HDR, infoDatasetWR_HDR);
  msg = compareDatasetInfoGDAL(infoDatasetCreate_HDR, infoDatasetWR_HDR, os);
  if ( msg > 0 )
    {
    os << "output code: " << msg << std::endl;
    infoDatasetCreate_HDR->Print(os);
    infoDatasetWR_HDR->Print(os);
    }
  else
    {
    os << "=> No Diff" <<std::endl;
    }

  delete infoDatasetCreate_TIFF;
  delete infoDatasetWR_TIFF;
  delete infoDatasetCreate_HDR;
  delete infoDatasetWR_HDR;

}



bool writeReadDatasetMetadata(std::string filename, std::vector<std::string> options,
                              DatasetInfoGDAL *infoDatasetCreate,
                              DatasetInfoGDAL *infoDatasetWR)
{

  infoDatasetCreate->Clear();
  infoDatasetWR->Clear();
  // Detect format with the extension
  const char *pszFormat;
  if ( filename.compare(filename.size()-4, 4,".tif") == 0 )
    {
    pszFormat = "GTiff";
    infoDatasetCreate->m_FileType = "TIFF";
    infoDatasetWR->m_FileType = "TIFF";
    }
  else if (filename.compare(filename.size()-4, 4,".hdr") == 0 )
    {
    pszFormat = "ENVI";
    infoDatasetCreate->m_FileType = "HDR";
    infoDatasetWR->m_FileType = "HDR";
    filename.erase(filename.size()-4, 4);
    }
  else
    {
    std::cout << "Format not supported" <<std::endl;
    return false;
    }

  // Set parameters
  bool setGCP = false;
  bool setProjectionRef = false;
  bool setProjectionRef_UTM = false;
  bool setGeoTransform_ID = false;
  bool setGeoTransform = false;

  for (unsigned int itOption = 0; itOption < (unsigned int)options.size(); itOption++)
    {
    std::string opt = options[itOption];
    if ( opt.compare("-setGCP") == 0 )
      {
      setGCP = true;
      }
    if (opt.compare("-setGeoTransform_ID") == 0)
      {
      setGeoTransform_ID = true;
      }
    if (opt.compare("-setGeoTransform") == 0)
      {
      setGeoTransform = true;
      }
    if (opt.compare("-setProjectionRef") == 0)
      {
      setProjectionRef = true;
      }
    if (opt.compare("-setProjectionRef_UTM") == 0)
      {
      setProjectionRef_UTM = true;
      }
    }


  // Check the driver used
  GDALDriver *poDriver;
  char **papszMetadata;

  GDALAllRegister();
  poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);

  if( poDriver == NULL )
    {
    std::cout << "impossible to get the driver" <<std::endl;
    return false;
    }

  // Format selected support create method ?
  papszMetadata = poDriver->GetMetadata();
  if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE ) )
    {
    //std::cout << "Driver " << pszFormat << " supports Create() method." << std::endl;
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

  OGRSpatialReference oSRS_GCP;
  char *pszSRS_WKT_GCP = NULL;
  oSRS_GCP.SetWellKnownGeogCS( "WGS84" );
  oSRS_GCP.exportToWkt( &pszSRS_WKT_GCP );

  // Set GCP
  if (setGCP)
    {
    unsigned int gcpCount = 1;
    GDAL_GCP * gdalGcps = new GDAL_GCP[gcpCount];

    std::ostringstream strIndexGCP;
    strIndexGCP << 1;
   
    std::string sstrIndexGCP = strIndexGCP.str();
    gdalGcps[0].pszId = new char[sstrIndexGCP.length()+1];
    strcpy(gdalGcps[0].pszId,sstrIndexGCP.c_str());
    gdalGcps[0].pszInfo = const_cast<char *>("GCP test");
    gdalGcps[0].dfGCPPixel = 0.5;
    gdalGcps[0].dfGCPLine = 0.5;
    gdalGcps[0].dfGCPX = 1.35409622400109;
    gdalGcps[0].dfGCPY = 43.6544262050905;
    gdalGcps[0].dfGCPZ = 0;

    poDstDS->SetGCPs(gcpCount, gdalGcps, pszSRS_WKT_GCP);

    delete [] gdalGcps[0].pszId;
    delete[] gdalGcps;

    /*infoDatasetCreate->m_GCPProjRef = static_cast<std::string>( pszSRS_WKT);
    infoDatasetCreate->m_GCPCount = gcpCount; */
    }

  // Set ProjectionRef
  OGRSpatialReference oSRS_ProjRef;
  char *pszSRS_WKT_ProjRef = NULL;
  if (setProjectionRef || setProjectionRef_UTM)
  {
    oSRS_ProjRef.SetWellKnownGeogCS( "WGS84" );
    if (setProjectionRef_UTM)
      {
      oSRS_ProjRef.SetUTM( 31, TRUE );
      }
    oSRS_ProjRef.exportToWkt( &pszSRS_WKT_ProjRef );
    poDstDS->SetProjection( pszSRS_WKT_ProjRef );
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
    /*infoDatasetCreate->m_GeoTransfo = CE_None;
    infoDatasetCreate->m_GeoTransfoParam[0] = adfGeoTransform[0];
    infoDatasetCreate->m_GeoTransfoParam[1] = adfGeoTransform[1];
    infoDatasetCreate->m_GeoTransfoParam[3] = adfGeoTransform[3];
    infoDatasetCreate->m_GeoTransfoParam[5] = adfGeoTransform[5]; */
    }

  // Write data into dataset
  GDALRasterBand *poBand;
  GByte abyRaster[128*128];
  for (unsigned int it=0; it < 128*128; ++it)
    {
    abyRaster[it] = 0;
    }

  poBand = poDstDS->GetRasterBand(1);
  poBand->RasterIO( GF_Write, 0, 0, 128, 128,
                    abyRaster, 128, 128, GDT_Byte, 0, 0 );


  // Get some information from dataset
  infoDatasetCreate->m_Name = "Create";
  infoDatasetCreate->m_ProjRef = static_cast<std::string>(poDstDS->GetProjectionRef());

  infoDatasetCreate->m_GeoTransfo = poDstDS->GetGeoTransform(infoDatasetCreate->m_GeoTransfoParam);

  infoDatasetCreate->m_GCPProjRef = static_cast<std::string>( poDstDS->GetGCPProjection());
  infoDatasetCreate->m_GCPCount = poDstDS->GetGCPCount();

  for (unsigned int gcpIndex = 0; gcpIndex < (unsigned int) poDstDS->GetGCPCount(); gcpIndex++)
    {
    const GDAL_GCP *psGCP;
    psGCP = poDstDS->GetGCPs() + gcpIndex;
    infoDatasetCreate->addGCP(psGCP);
    }

  // Once we're done, close properly the dataset
  GDALClose( (GDALDatasetH) poDstDS );
  CPLFree( pszSRS_WKT_GCP );
  CPLFree( pszSRS_WKT_ProjRef );

  //**************
  // Open File
  GDALDataset  *poDataset;

  poDataset = (GDALDataset *) GDALOpen( filename.c_str(), GA_ReadOnly );
  if( poDataset == NULL )
     return false;

  // Get some information from file
  infoDatasetWR->m_Name = "WR";
  infoDatasetWR->m_ProjRef = static_cast<std::string>(poDataset->GetProjectionRef());

  infoDatasetWR->m_GeoTransfo = poDataset->GetGeoTransform(infoDatasetWR->m_GeoTransfoParam);

  infoDatasetWR->m_GCPProjRef = static_cast<std::string>( poDataset->GetGCPProjection());
  infoDatasetWR->m_GCPCount = poDataset->GetGCPCount();
  for (unsigned int gcpIndex = 0; gcpIndex < (unsigned int)poDataset->GetGCPCount(); gcpIndex++)
    {
    infoDatasetWR->addGCP( poDataset->GetGCPs() + gcpIndex);
    }

  GDALClose(poDataset);

  return true;
}
