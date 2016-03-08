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
#include "otbGDALImageIO.h"
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

char const * const
GDAL_RESAMPLING_NAMES[ GDAL_RESAMPLING_COUNT ] =
{
  "NONE",
  "NEAREST",
  "GAUSS",
  "CUBIC",
  "AVERAGE",
  "MODE",
  "AVERAGE_MAGPHASE"
};


char const * const
GDAL_COMPRESSION_NAMES[ GDAL_COMPRESSION_COUNT ] =
{
  "",
  "JPEG",
  "LZW",
  "PACKBITS",
  "DEFLATE",
};


/***************************************************************************/
std::string
GetConfigOption( const char * key )
{
  const char * value = CPLGetConfigOption( key, NULL );
  
  return
    value==NULL
    ? std::string()
    : std::string( value );
}

/***************************************************************************/
bool
GDALOverviewsBuilder
::CanGenerateOverviews( const std::string & filename )
{
  GDALImageIO::Pointer io( GDALImageIO::New() );

  typedef std::vector< std::string > StringVector;

  StringVector names;
  StringVector descs;

  if( !io->CanReadFile( filename.c_str() ) )
    return false;

  io->SetFileName( filename );

  return !io->GetSubDatasetInfo( names, descs );
}

/***************************************************************************/
GDALOverviewsBuilder
::GDALOverviewsBuilder() :
  m_GDALDataset(),
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
unsigned int
GDALOverviewsBuilder
::CountResolutions( unsigned int factor, unsigned int minSize ) const
{
  assert( factor>1 );

  if( factor<=1 )
    return 0;

  assert( minSize>0 );

  if( minSize<=0 )
    return 0;

  assert( !m_GDALDataset.IsNull() );

  // unsigned int minSize = static_cast< unsigned int >( pow( factor, n ) );

  unsigned int size =
    std::min(
      m_GDALDataset->GetWidth(),
      m_GDALDataset->GetHeight()
    );

  if( size<minSize )
    return 0;

  unsigned int count = 0;

  while( size >= minSize )
    {
    ++ count;

    size /= factor;
    }

  return count;
}

/***************************************************************************/
unsigned int
GDALOverviewsBuilder
::CountResolutions() const
{
  return CountResolutions( m_ResolutionFactor );
}

/***************************************************************************/
void
GDALOverviewsBuilder
::ListResolutions( SizeVector & sizes )
{
  ListResolutions( sizes, m_ResolutionFactor, m_NbResolutions );
}

/***************************************************************************/
void
GDALOverviewsBuilder
::ListResolutions( SizeVector & sizes, unsigned int factor, unsigned int count )
{
  assert( factor>1 );

  if( factor<=1 )
    return;

  assert( !m_GDALDataset.IsNull() );

  Size s;

  s[ 0 ] = m_GDALDataset->GetWidth();
  s[ 1 ] = m_GDALDataset->GetHeight();

  unsigned int n =
    std::min(
      count,
      CountResolutions(	factor )
    );

  for( unsigned int i=0; i<n; ++i )
    {
    sizes.push_back( s );

    s[ 0 ] /= factor;
    s[ 1 ] /= factor;
    }
}

/***************************************************************************/
unsigned int
GDALOverviewsBuilder
::GetWidth() const
{
  assert( !m_GDALDataset.IsNull() );

  return m_GDALDataset->GetWidth();
}

/***************************************************************************/
unsigned int
GDALOverviewsBuilder
::GetHeight() const
{
  assert( !m_GDALDataset.IsNull() );

  return m_GDALDataset->GetHeight();
}

/***************************************************************************/
unsigned int
GDALOverviewsBuilder
::GetOverviewsCount() const
{
  assert( !m_GDALDataset.IsNull() );

  return m_GDALDataset->GetOverviewsCount();
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
const std::string &
GDALOverviewsBuilder
::GetInputFileName() const
{
  return m_InputFileName;
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
/*
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
*/

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


  assert( !m_GDALDataset.IsNull() );

  assert( m_NbResolutions>0 );

  if( m_NbResolutions==0 )
    {
    itkExceptionMacro(
      << "Wrong number of resolutions: " << m_NbResolutions
    );
    }

  // Build the overviews list from nb of resolution desired
  std::vector< int > ovwlist;
  unsigned int factor = 1;
  for( unsigned int i = 1; i < m_NbResolutions; i++ )
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

  std::string erdas( GetConfigOption( "USE_RRD" ) );

  CPLSetConfigOption(
    "USE_RRD",
    m_Format==GDAL_FORMAT_ERDAS
    ? "YES"
    : "NO"
  );


  assert(
    m_CompressionMethod>=GDAL_COMPRESSION_NONE &&
    m_CompressionMethod<GDAL_COMPRESSION_COUNT
  );

  std::string compression( GetConfigOption( "COMPRESS_OVERVIEW" ) );

  CPLSetConfigOption(
    "COMPRESS_OVERVIEW",
    GDAL_COMPRESSION_NAMES[ m_CompressionMethod ]
  );


  assert(
    m_ResamplingMethod>=GDAL_RESAMPLING_NONE &&
    m_ResamplingMethod<GDAL_RESAMPLING_COUNT
  );

  CPLErr lCrGdal =
    m_GDALDataset->GetDataSet()->BuildOverviews(
      GDAL_RESAMPLING_NAMES[ m_ResamplingMethod ],
      static_cast< int >( m_NbResolutions - 1 ),
      &ovwlist.front(),
      0, // All bands
      NULL, // All bands
      ( GDALProgressFunc )otb_UpdateGDALProgress,
      this );

  CPLSetConfigOption( "USE_RRD", erdas.c_str() );
  CPLSetConfigOption( "COMPRESS_OVERVIEW", compression.c_str() );

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

  m_GDALDataset = dataset;
}


} // end namespace otb
