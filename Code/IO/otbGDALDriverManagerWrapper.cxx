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
    /*
    driver = GetGDALDriverManager()->GetDriverByName( "JPEG2000" );
    if (driver)
      GetGDALDriverManager()->DeregisterDriver( driver );
    */
    
#ifndef CHECK_HDF4OPEN_SYMBOL
    // Get rid of the HDF4 driver when it is buggy
    driver = GetGDALDriverManager()->GetDriverByName( "hdf4" );
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
  
  // first : test if a driver can identify the dataset
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

GDALOverviewsBuilder::GDALOverviewsBuilder()
{
  m_NbOfResolutions = 1;
  m_ResolutionFactor = 2;
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

// Progress reporting functions compatible with GDAL C API
extern "C"
{
  static int CPL_STDCALL otb_UpdateGDALProgress(double dfComplete,
                                                const char *pszMessage,
                                                void * pProgressArg)
  {
    otb::GDALOverviewsBuilder* _this = (otb::GDALOverviewsBuilder*)pProgressArg;
    _this->UpdateProgress(dfComplete);
    return 1;
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


    if( m_NbOfResolutions==0 )
      {
      itkExceptionMacro(
       << "Wrong number of resolutions: " << m_NbOfResolutions
      );
      }

    // Build the overviews list from nb of resolution desired
    std::vector<int> ovwlist;
    unsigned int factor = 1;
    for (unsigned int i = 1; i < m_NbOfResolutions; i++)
      {
      factor*=m_ResolutionFactor;
      ovwlist.push_back(factor);
      }

    /*std::cout << "list of overviews level= ";
    for (unsigned int i = 0; i < ovwlist.size(); i++)
        {
        std::cout << ovwlist[i] << ",";
        }
    std::cout << std::endl; */

    std::string resampMethod;
    this->GetGDALResamplingMethod(resampMethod);

    CPLErr lCrGdal = wrappedDataset->GetDataSet()->
          BuildOverviews( resampMethod.c_str(),
                          static_cast<int>(m_NbOfResolutions-1),
                          &ovwlist.front(),
                          0, // All bands
                          NULL, // All bands
                          (GDALProgressFunc)otb_UpdateGDALProgress,
                          this);
    if (lCrGdal == CE_Failure)
      {
      itkExceptionMacro(<< "Error while building the GDAL overviews from " << m_InputFileName.c_str() << ".");
      }
}

} // end namespace otb

