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

// GDALDriverManagerWrapper method implementation

GDALDriverManagerWrapper::GDALDriverManagerWrapper()
{
    GDALAllRegister();
    
#ifndef CHECK_HDF4OPEN_SYMBOL
    // Get rid of the HDF4 driver when it is buggy
    GDALDriver* driver = GetGDALDriverManager()->GetDriverByName( "hdf4" );
    if (driver)
      GetGDALDriverManager()->DeregisterDriver( driver );
#endif
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
}
