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
#include "otbGDALOverviewsBuilder.h"


#include <vector>

#include "gdal.h"
#include "otb_boost_string_header.h"

#include "otbGDALDriverManagerWrapper.h"
#include "otbSystem.h"


namespace otb
{

// Progress reporting functions compatible with GDAL C API
extern "C"
{
  static int CPL_STDCALL otb_UpdateGDALProgress( double dfComplete,
                                                 const char * itkNotUsed( pszMessage ),
                                                 void * pProgressArg )
  {
    otb::GDALOverviewsBuilder* _this = (otb::GDALOverviewsBuilder*)pProgressArg;
    _this->UpdateProgress(dfComplete);
    return 1;
  }
}


GDALOverviewsBuilder
::GDALOverviewsBuilder()
{
  m_NbOfResolutions = 1;
  m_ResolutionFactor = 2;
  m_ResamplingMethod = NEAREST;
  Superclass::SetNumberOfRequiredInputs(0);
  Superclass::SetNumberOfRequiredOutputs(0);
}


void
GDALOverviewsBuilder
::SetResamplingMethod( GDALResamplingType resampMethod )
{
  m_ResamplingMethod = resampMethod;
};


void
GDALOverviewsBuilder
::SetNbOfResolutions( unsigned int nbResol )
{
  m_NbOfResolutions = nbResol;
};


void
GDALOverviewsBuilder
::SetResolutionFactor( unsigned int factor )
{
  m_ResolutionFactor = factor;
}


void
GDALOverviewsBuilder
::SetInputFileName( const std::string & str )
{
  m_InputFileName = str;
};


void
GDALOverviewsBuilder
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Input Filename: " << m_InputFileName << std::endl;
  os << indent << "Number of Resolution requested: " << m_NbOfResolutions << std::endl;
  os << indent << "Resampling method: " << m_ResamplingMethod << std::endl;
}


void
GDALOverviewsBuilder
::GetGDALResamplingMethod( std::string & resamplingMethod )
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
