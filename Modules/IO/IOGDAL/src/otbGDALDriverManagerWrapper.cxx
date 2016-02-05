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
#include "otbGDALDriverManagerWrapper.h"
#include <vector>
#include "otb_boost_string_header.h"
#include "otbSystem.h"

namespace otb
{

GDALDatasetWrapper::GDALDatasetWrapper(): m_Dataset(NULL)
{
}

GDALDatasetWrapper::~GDALDatasetWrapper()
{
  if (m_Dataset)
    {
    GDALClose(m_Dataset);
    }
}

// GetDataSet
GDALDataset*
GDALDatasetWrapper::GetDataSet() const
{
  return m_Dataset;
}

// IsJPEG2000
bool
GDALDatasetWrapper::IsJPEG2000() const
{
  if (m_Dataset == NULL)
    {
    return false;
    }
  std::string driverName(m_Dataset->GetDriver()->GetDescription());
  if (driverName.compare("JP2OpenJPEG") == 0 ||
      driverName.compare("JP2KAK") == 0 ||
      driverName.compare("JP2ECW") == 0)
    {
    return true;
    }
  return false;
}

// GDALDriverManagerWrapper method implementation

GDALDriverManagerWrapper::GDALDriverManagerWrapper()
{
    GDALAllRegister();

    GDALDriver* driver = 0;

    // Ignore incompatible Jpeg2000 drivers (Jasper)
    driver = GetGDALDriverManager()->GetDriverByName( "JPEG2000" );
    if (driver)
      GetGDALDriverManager()->DeregisterDriver( driver );

// #ifndef CHECK_HDF4OPEN_SYMBOL
//     // Get rid of the HDF4 driver when it is buggy
//     driver = GetGDALDriverManager()->GetDriverByName( "hdf4" );
//     if (driver)
//       GetGDALDriverManager()->DeregisterDriver( driver );
// #endif
}

GDALDriverManagerWrapper::~GDALDriverManagerWrapper()
{
  GDALDestroyDriverManager();
}

// Open the file for reading and returns a smart dataset pointer
GDALDatasetWrapper::Pointer
GDALDriverManagerWrapper::Open( std::string filename ) const
{
  GDALDatasetWrapper::Pointer datasetWrapper;

  if (boost::algorithm::starts_with(filename, "http://")
      || boost::algorithm::starts_with(filename, "https://") )
    {
    // don't try to open it and exit
    return datasetWrapper;
    }

  // test if a driver can identify the dataset
  GDALDriverH identifyDriverH = GDALIdentifyDriver(filename.c_str(), NULL);
  if(identifyDriverH == NULL)
    {
    // don't try to open it and exit
    return datasetWrapper;
    }

  GDALDriver *identifyDriver = static_cast<GDALDriver*>(identifyDriverH);

  // check if Jasper will be used
  if (strcmp(identifyDriver->GetDescription(),"JPEG2000") == 0)
    {
    itkGenericExceptionMacro(<< "Error : tried to open the file "
      << filename.c_str() << " with GDAL driver Jasper "
      "(which fails on OTB). Try setting the environment variable GDAL_SKIP"
      " in order to avoid this driver.");
    }

  GDALDatasetH dataset = GDALOpen(filename.c_str(), GA_ReadOnly);

  if (dataset != NULL)
    {
    datasetWrapper = GDALDatasetWrapper::New();
    datasetWrapper->m_Dataset = static_cast<GDALDataset*>(dataset);
    }
  return datasetWrapper;
}

// Open the new  file for writing and returns a smart dataset pointer
GDALDatasetWrapper::Pointer
GDALDriverManagerWrapper::Create( std::string driverShortName, std::string filename,
                                    int nXSize, int nYSize, int nBands,
                                    GDALDataType eType, char ** papszOptions ) const
{
  GDALDatasetWrapper::Pointer datasetWrapper;

  GDALDriver*  driver = GetDriverByName( driverShortName );
  if(driver != NULL)
    {
    GDALDataset* dataset = driver->Create(filename.c_str(),
                                          nXSize, nYSize,
                                          nBands, eType,
                                          papszOptions );

    if (dataset != NULL)
      {
      datasetWrapper = GDALDatasetWrapper::New();
      datasetWrapper->m_Dataset = dataset;
      }
    }
  return datasetWrapper;
}

GDALDriver*
GDALDriverManagerWrapper::GetDriverByName( std::string driverShortName ) const
{
  return GetGDALDriverManager()->GetDriverByName(driverShortName.c_str());
}

} // end namespace otb
