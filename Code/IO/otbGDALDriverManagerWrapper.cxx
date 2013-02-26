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
#include "otbMacro.h"
#include "otbSystem.h"
#include "itkMacro.h"

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

GDALOverviewsBuilder::GDALOverviewsBuilder()
{
  m_NbOfResolutions = 1;
  m_ResamplingMethod = NEAREST;
  Superclass::SetNumberOfRequiredInputs(0);
  Superclass::SetNumberOfRequiredOutputs(0);
}

void GDALOverviewsBuilder::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Input Filename: " << m_InputFileName << std::endl;
  os << indent << "Number of Resolution requested: " << m_NbOfResolutions << std::endl;
  os << indent << "Resampling method: " << m_ResamplingMethod << std::endl;
}

void GDALOverviewsBuilder::GetGDALResamplingMethod(std::string &resamplingMethod)
{
  resamplingMethod.clear();
  switch(m_ResamplingMethod)
  {
    case NONE:
      resamplingMethod = "NONE";
      break;
    case NEAREST:
      resamplingMethod = "NEAREST";
      break;
    case GAUSS:
      resamplingMethod = "GAUSS";
      break;
    case CUBIC:
      resamplingMethod = "CUBIC";
      break;
    case AVERAGE:
      resamplingMethod = "AVERAGE";
      break;
    case MODE:
      resamplingMethod = "MODE";
      break;
    case AVERAGE_MAGPHASE:
      resamplingMethod = "AVERAGE_MAGPHASE";
      break;
    default:
      resamplingMethod = "NONE";
      break;
  }
}

void GDALOverviewsBuilder::Update()
{
  typedef itk::SmartPointer<GDALDatasetWrapper> GDALDatasetWrapperPointer;
    GDALDatasetWrapperPointer wrappedDataset =
        GDALDriverManagerWrapper::GetInstance().Open(m_InputFileName);
    if (wrappedDataset.IsNull())
      {
      itkExceptionMacro(<< "Error while opening the file "<< m_InputFileName.c_str() << ".");
      }

    // Build the overviews list from nb of resolution desired
    std::vector<int> ovwlist;
    for (unsigned int i = 1; i < m_NbOfResolutions; i++)
      {
      ovwlist.push_back(1<<i);
      }

    /*std::cout << "list of overviews level= ";
    for (unsigned int i = 0; i < ovwlist.size(); i++)
        {
        std::cout << ovwlist[i] << ",";
        }
    std::cout << std::endl;*/

    std::string resampMethod;
    this->GetGDALResamplingMethod(resampMethod);

    CPLErr lCrGdal = wrappedDataset->GetDataSet()->
          BuildOverviews( resampMethod.c_str(),
                          static_cast<int>(m_NbOfResolutions-1),
                          &ovwlist.front(),
                          0, // All bands
                          NULL, // All bands
                          (GDALProgressFunc)UpdateGDALProgress,
                          this);
    if (lCrGdal == CE_Failure)
      {
      itkExceptionMacro(<< "Error while building the GDAL overviews from " << m_InputFileName.c_str() << ".");
      }
}

int GDALOverviewsBuilder::UpdateGDALProgress(double dfComplete, const char *pszMessage,
                                               void * pProgressArg)
{
  GDALOverviewsBuilder* _this = (GDALOverviewsBuilder*)pProgressArg;
  _this->UpdateProgress(dfComplete) ;
  return 1;
}

} // end namespace otb
