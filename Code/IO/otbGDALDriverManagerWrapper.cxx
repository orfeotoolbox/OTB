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

namespace gdal {
bool GDALBuildOverviewsFromResolution(  std::string inputFilename,
                                        unsigned int nbResolution,
                                        const char* resamplingMethod,
                                        GDALProgressFunc pfnProgress,
                                        void* pProgressData)
{
  typedef itk::SmartPointer<GDALDatasetWrapper> GDALDatasetWrapperPointer;
  GDALDatasetWrapperPointer wrappedDataset =
      GDALDriverManagerWrapper::GetInstance().Open(inputFilename);
  if (wrappedDataset.IsNull())
    return false;

  // Build the overviews list from nb of resolution desired
  std::vector<int> ovwlist;
  for (unsigned int i = 1; i < nbResolution; i++)
    {
    ovwlist.push_back(1<<i);
    }

  std::cout << "list of overviews level= ";
  for (unsigned int i = 0; i < ovwlist.size(); i++)
      {
      std::cout << ovwlist[i] << ",";
      }
  std::cout << std::endl;

  CPLErr lCrGdal = wrappedDataset->GetDataSet()->
        BuildOverviews( resamplingMethod,
                        static_cast<int>(nbResolution-1),
                        ovwlist.data(),
                        0, // All bands
                        NULL, // All bands
                        pfnProgress,
                        pProgressData);
  if (lCrGdal == CE_Failure)
    return false;

  return true;
}
} // end namespace gdal
} // end namespace otb
