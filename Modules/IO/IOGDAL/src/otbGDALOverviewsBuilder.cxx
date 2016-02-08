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


char const * const GDAL_RESAMPLING_NAMES[ GDAL_RESAMPLING_COUNT ] =
{
  "NONE",
  "NEAREST",
  "GAUSS",
  "CUBIC",
  "AVERAGE",
  "MODE",
  "AVERAGE_MAGPHASE",
};


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


/***************************************************************************/
GDALOverviewsBuilder
::GDALOverviewsBuilder() :
  m_GdalDataset(),
  m_InputFileName(),
  m_NbResolutions( 1 ),
  m_ResolutionFactor( 2 ),
  m_ResamplingMethod( GDAL_RESAMPLING_NEAREST ),
  m_CompressionMethod( GDAL_COMPRESSION_NONE ),
  m_Format( GDAL_FORMAT_GEOTIFF )
{
  Superclass::SetNumberOfRequiredInputs(0);
  Superclass::SetNumberOfRequiredOutputs(0);
}

/***************************************************************************/
GDALResampling
GDALOverviewsBuilder
::GetResamplingMethod() const
{
  return m_ResamplingMethod;
}

/***************************************************************************/
void
GDALOverviewsBuilder
::SetResamplingMethod( GDALResamplingType resampling )
{
  m_ResamplingMethod = resampling;
};

/***************************************************************************/
GDALCompression
GDALOverviewsBuilder
::GetCompressionMethod() const
{
  return m_CompressionMethod;
}

/***************************************************************************/
void
GDALOverviewsBuilder
::SetCompressionMethod( GDALCompression compression )
{
  m_CompressionMethod = compression;
}


/***************************************************************************/
GDALFormat
GDALOverviewsBuilder
::GetFormat() const
{
  return m_Format;
}

/***************************************************************************/
void
GDALOverviewsBuilder
::SetFormat( GDALFormat format )
{
  m_Format = format;
}

/***************************************************************************/
unsigned int
GDALOverviewsBuilder
::GetNbResolutions() const
{
  return m_NbResolutions;
}

/***************************************************************************/
void
GDALOverviewsBuilder
::SetNbResolutions( unsigned int n )
{
  m_NbResolutions = n;
};

/***************************************************************************/
unsigned int
GDALOverviewsBuilder
::GetResolutionFactor() const
{
  return m_ResolutionFactor;
}

/***************************************************************************/
void
GDALOverviewsBuilder
::SetResolutionFactor( unsigned int factor )
{
  m_ResolutionFactor = factor;
}

/***************************************************************************/
void
GDALOverviewsBuilder
::SetInputFileName( const std::string & filename )
{
  try
    {
    OpenDataset( filename );
    }
  catch( ... )
    {
    throw;
    }

  m_InputFileName = filename;
};

/***************************************************************************/
void
GDALOverviewsBuilder
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Input Filename: " << m_InputFileName << std::endl;
  os << indent << "Number of Resolution requested: " << m_NbResolutions << std::endl;
  os << indent << "Resampling method: " << m_ResamplingMethod << std::endl;
}

/***************************************************************************/
void
GDALOverviewsBuilder
::GetGDALResamplingMethod( std::string & resamplingMethod )
{
  resamplingMethod.clear();
  switch(m_ResamplingMethod)
  {
    case GDAL_RESAMPLING_NONE:
      resamplingMethod = "NONE";
      break;
    case GDAL_RESAMPLING_NEAREST:
      resamplingMethod = "NEAREST";
      break;
    case GDAL_RESAMPLING_GAUSS:
      resamplingMethod = "GAUSS";
      break;
    case GDAL_RESAMPLING_CUBIC:
      resamplingMethod = "CUBIC";
      break;
    case GDAL_RESAMPLING_AVERAGE:
      resamplingMethod = "AVERAGE";
      break;
    case GDAL_RESAMPLING_MODE:
      resamplingMethod = "MODE";
      break;
    case GDAL_RESAMPLING_AVERAGE_MAGPHASE:
      resamplingMethod = "AVERAGE_MAGPHASE";
      break;
    default:
      resamplingMethod = "NONE";
      break;
  }
}

/***************************************************************************/
void GDALOverviewsBuilder::Update()
{
  // typedef itk::SmartPointer<GDALDatasetWrapper> GDALDatasetWrapperPointer;
  //   GDALDatasetWrapperPointer wrappedDataset =
  //       GDALDriverManagerWrapper::GetInstance().Open(m_InputFileName);
  //   if (wrappedDataset.IsNull())
  //     {
  //     itkExceptionMacro(<< "Error while opening the file "<< m_InputFileName.c_str() << ".");
  //     }


  assert( !m_GdalDataset.IsNull() );

  if( m_NbResolutions==0 )
    {
    itkExceptionMacro(
      << "Wrong number of resolutions: " << m_NbResolutions
    );
    }

  // Build the overviews list from nb of resolution desired
  std::vector<int> ovwlist;
  unsigned int factor = 1;
  for (unsigned int i = 1; i < m_NbResolutions; i++)
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

  CPLErr lCrGdal = m_GdalDataset->GetDataSet()->
    BuildOverviews( resampMethod.c_str(),
		    static_cast<int>(m_NbResolutions-1),
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

/***************************************************************************/
void
GDALOverviewsBuilder
::OpenDataset( const std::string & filename )
{
  GDALDatasetWrapper::Pointer dataset(
    GDALDriverManagerWrapper::GetInstance().Open( filename )
  );

  if( dataset.IsNull() )
    itkExceptionMacro(
      << "Error while opening file '"
      << filename.c_str()
      << "' as GDAL dataset."
    );

  m_GdalDataset = dataset;
}


} // end namespace otb
