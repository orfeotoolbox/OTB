/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mvdVectorImageModel.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itksys/SystemTools.hxx"
#include "vnl/vnl_random.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbConfigure.h"
#include "otbGDALDriverManagerWrapper.h"
#include "otbStandardOneLineFilterWatcher.h"
#include "otbGeoInformationConversion.h"
#include "otbCoordinateToName.h"
#include "otbDEMHandler.h"
#include "otbGroundSpacingImageFunction.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"
#include "Core/mvdQuicklookModel.h"
#include "Core/mvdSystemError.h"



namespace mvd
{
/*
  TRANSLATOR mvd::VectorImageModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
VectorImageModel
::VectorImageModel( QObject* parent ) :
  AbstractImageModel( parent ),
  m_Image(),
  m_ImageFileReader(),
  m_Settings(),
  m_Filename()
{
}

/*****************************************************************************/
VectorImageModel
::~VectorImageModel()
{
}

/*****************************************************************************/
const DatasetModel*
VectorImageModel
::GetDatasetModel() const
{
  // assert( GetParentModel()==GetParentModel< DatasetModel >() );
  return GetParentModel< DatasetModel >();
}

/*****************************************************************************/
DatasetModel*
VectorImageModel
::GetDatasetModel()
{
  // assert( GetParentModel()==GetParentModel< DatasetModel >() );
  return GetParentModel< DatasetModel >();
}

/*****************************************************************************/
void
VectorImageModel
::SetFilename( const QString& filename , int w, int h)
{
  setObjectName( filename );

  // 1. store the input filename
  m_Filename = filename;

  // Get the largest possible region of the image
  m_ImageFileReader = DefaultImageFileReaderType::New();

  m_ImageFileReader->SetFileName( ToStdString( m_Filename ) );
  m_ImageFileReader->UpdateOutputInformation();

  // Retrieve the list of Lod from file
  m_AvailableLod = m_ImageFileReader->GetAvailableResolutions();

  // Remember native largest region.
  m_NativeLargestRegion =
      m_ImageFileReader->GetOutput()->GetLargestPossibleRegion();

  // Remember native spacing
  m_NativeSpacing = m_ImageFileReader->GetOutput()->GetSpacing();

  
  // Setup GenericRSTransform
  m_GenericRSTransform = otb::GenericRSTransform<>::New();
  m_GenericRSTransform->SetInputDictionary(m_ImageFileReader->GetOutput()->GetMetaDataDictionary());
  m_GenericRSTransform->SetOutputProjectionRef(otb::GeoInformationConversion::ToWKT(4326));
  m_GenericRSTransform->InstanciateTransform();

  //Compute estimated spacing here
  //m_EstimatedGroundSpacing
  m_EstimatedGroundSpacing = m_NativeSpacing;
 
  typedef otb::GroundSpacingImageFunction<VectorImageType> GroundSpacingImageType;
  GroundSpacingImageType::Pointer GroundSpacing = GroundSpacingImageType::New();
  GroundSpacing->SetInputImage(m_ImageFileReader->GetOutput());
  
  if (m_GenericRSTransform->IsUpToDate())
    {
    if (m_GenericRSTransform->GetTransformAccuracy() != otb::Projection::UNKNOWN)
      {
      IndexType  index;
      vnl_random rand;

      index[0] = static_cast<IndexType::IndexValueType>(rand.lrand32(0, m_ImageFileReader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]));
      index[1] = static_cast<IndexType::IndexValueType>(rand.lrand32(0, m_ImageFileReader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]));

      m_EstimatedGroundSpacing = GroundSpacing->EvaluateAtIndex(index);
      }
    }

  //
  // 2. Setup file-reader.
  SetupCurrentLodImage(w, h);
}

/*****************************************************************************/
void
VectorImageModel
::EnsureValidImage( const QString& filename )
{
  try
    {
    DefaultImageFileReaderType::Pointer imageFileReader(
      DefaultImageFileReaderType::New()
    );

    imageFileReader->SetFileName( QFile::encodeName( filename ).constData() );
    imageFileReader->UpdateOutputInformation();
    }

  catch( std::exception& exc )
    {
    // TODO manage the message returned by OTB
    qWarning()
      << ToStdString(
	tr( "Exception caught when validating file '%1': ")
	.arg( filename ) ).c_str()
      << exc.what();

    throw
      // std::runtime_error(
      SystemError(
	ToStdString(
	  tr( "File '%1' cannot be read by OTB." )
	  .arg( filename )
	)
      );
    }
}

/*****************************************************************************/
void
VectorImageModel
::BuildGdalOverviews()
{
  // Build overviews if necessary
  bool hasOverviewsSupport = m_ImageFileReader->HasOverviewsSupport();
  int nbOfAvailableOvw = m_ImageFileReader->GetNbOfAvailableOverviews();
  // TODO: this choice should be done by the user during the import of the file
  bool forceToCacheOvw = true;

  if (!hasOverviewsSupport)
    {
    // the current file doesn't have overviews available and the ImageIO doesn't support overviews
    throw std::runtime_error(
      ToStdString(
	tr( "The ImageIO used to read this file doesn't support Overviews." )
      )
    );
    }

  // Else:

  qWarning() << tr( "The ImageIO used to read this file supports overviews." );

  if( nbOfAvailableOvw>0 )
    {
    // qDebug() << tr("The file already has overviews!");
    return;
    }

  // TODO MSD: how to manage case of JPEG2000 with no overviews ? : wait GDAL support OpenJPEG ...

  // The current file don't have overviews available
  qWarning() << tr( "The file doesn't have overviews." );

  if( !forceToCacheOvw )
    {
    // the user doesn't want to cache the overviews, GDAL will virtually compute the ovw on demand
    qWarning() << tr("Letting GDAL decimate the file on-the-fly !");

    return;
    }

  // the user want to cache the overviews
  // qDebug() << tr("Caching of overviews.");
  typedef otb::GDALOverviewsBuilder FilterType;
  FilterType::Pointer filter = FilterType::New();

  //m_ImageFileReader->GetAvailableResolutions(m_AvailableLod);

  std::string tempfilename( ToStdString( m_Filename ) );

  filter->SetInputFileName(tempfilename);
  filter->SetResamplingMethod(otb::AVERAGE);
  filter->SetResolutionFactor(4);
  assert( m_ImageFileReader->GetAvailableResolutions().size()==m_AvailableLod.size() );
  filter->SetNbOfResolutions(
    m_AvailableLod.size() > 1
    ? m_AvailableLod.size() / 2
    : 1
  );

  try
    {
    otb::StandardOneLineFilterWatcher watcher(
      filter,
      ToStdString( tr( "Overviews creation: " ) )
    );
    filter->Update();
    std::cout << std::endl;
    }
  catch( std::exception& /*exc*/ )
    {
    // The user can continue to use the file so we return a warning message
    // TODO MSD return the message to the log widget
    qWarning() <<
      ToStdString(
	tr( "The overviews creation failed.\n"
	    "Navigation in resolution will be slower." )
      ).c_str();

    // throw exc;
    }
}

/*****************************************************************************/
void
VectorImageModel
::virtual_BuildModel( void* context )
{
  SetProperties( new ImageProperties() );

   // Get build-context.
  assert( context!=NULL );
  BuildContext* buildContext = static_cast< BuildContext* >( context );

  // Build image overview.
  if( buildContext->IsBeingStored() )
    BuildGdalOverviews();

  // Get build-context settings.
  Settings * const  settings =
    static_cast< Settings * const >( buildContext->m_Settings );

  //
  // Step #1: Perform pre-process of AbstractModel::BuildModel()
  // pattern.

  // Store default display settings in the pre-process stage
  // i.e. before histogram is generated by the standard
  // AbstractImageModel::BuildModel().
  if( settings==NULL )
    InitializeColorSetupSettings();

  //
  // Step #2: Perform standard AbstractModel::BuildModel()
  // pattern. Call parent virtual method.

  // The call to the parent BuildModel() method will, for example,
  // generate the histogram.
  AbstractImageModel::virtual_BuildModel( context );

  //
  // Step #3: Post-process of the BuildModel() pattern.

  // Remember min/max pixel for color-dynamics once histogram has been
  // generated.
  if( settings==NULL )
    InitializeColorDynamicsSettings();
  else
    SetSettings( *settings );

  // Remember image properties.
  if( buildContext->m_Properties!=NULL )
    SetProperties( *buildContext->m_Properties );
}

/*****************************************************************************/
void
VectorImageModel
::InitializeColorSetupSettings()
{
  // Remember meta-data interface.
  otb::ImageMetadataInterfaceBase::ConstPointer metaData(
    GetMetaDataInterface()
  );

  // Ensure default display returns valid band indices (see OTB bug).
  assert( metaData->GetDefaultDisplay().size()==3 );
#if 0
  assert( metaData->GetDefaultDisplay()[ 0 ]
    < m_Image->GetNumberOfComponentsPerPixel() );
  assert( metaData->GetDefaultDisplay()[ 1 ]
    < m_Image->GetNumberOfComponentsPerPixel() );
  assert( metaData->GetDefaultDisplay()[ 2 ]
    < m_Image->GetNumberOfComponentsPerPixel() );
#endif

  // Patch invalid band indices of default-display (see OTB bug).
  Settings::ChannelVector rgb( metaData->GetDefaultDisplay() );

  if( rgb[ 0 ]>=m_Image->GetNumberOfComponentsPerPixel() )
    {
    rgb[ 0 ] = 0;
    }

  if( rgb[ 1 ]>=m_Image->GetNumberOfComponentsPerPixel() )
    {
    rgb[ 1 ] = 0;
    }

  if( rgb[ 2 ]>=m_Image->GetNumberOfComponentsPerPixel() )
    {
    rgb[ 2 ] = 0;
    }

  // Store default display settings.
  GetSettings().SetRgbChannels( rgb );

  // Store default grayscale-mode.
  if( m_Image->GetNumberOfComponentsPerPixel()<3 )
    {
    GetSettings().SetGrayscaleActivated( true );
    GetSettings().SetGrayChannel( rgb[ 0 ] );
    }
}

/*****************************************************************************/
void
VectorImageModel
::InitializeColorDynamicsSettings()
{
  // Get the histogram-model.
  HistogramModel* histogramModel = GetHistogramModel();
  assert( histogramModel!=NULL );

  // Remember min/max pixels.
  DefaultImageType::PixelType min( histogramModel->GetMinPixel() );
  DefaultImageType::PixelType max( histogramModel->GetMaxPixel() );

  CountType begin = -1;
  CountType end = -1;

  mvd::RgbwBounds( begin, end, RGBW_CHANNEL_ALL );

  // Store min/max intensities of default-display channels.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    Settings::ChannelVector::value_type band =
      GetSettings().GetRgbwChannel( channel );

    GetSettings().SetLowIntensity(
      channel,
      histogramModel->Quantile( band , 0.02, BOUND_LOWER )
    );

    GetSettings().SetHighIntensity(
      channel,
      histogramModel->Quantile( band , 0.02, BOUND_UPPER )
    );
    }
}

/*****************************************************************************/
CountType
VectorImageModel::ComputeBestLod( int width, int height ) const
{
  if( width<=0 || height<=0 )
    return 0;

  ImageRegionType nativeLargestRegion( GetNativeLargestRegion() );

  double factorX =
    double( width ) / double( nativeLargestRegion.GetSize()[ 0 ] );

  double factorY =
    double( height ) / double( nativeLargestRegion.GetSize()[ 1 ] );

  double initialZoomFactor = std::min(factorX, factorY);

  // Compute the best lod from the initialZoomFactor
  return ComputeBestLod( initialZoomFactor );
}

/*****************************************************************************/
CountType
VectorImageModel::ComputeBestLod(const double zoomFactor) const
{
  int inverseZoomFactor =  static_cast<int>((1/zoomFactor + 0.5));
  CountType bestLod = this->Closest(inverseZoomFactor, m_AvailableLod);
  return bestLod;
}

/*****************************************************************************/
unsigned int 
VectorImageModel::Closest(double invZoomfactor, const std::vector<unsigned int> & res) 
{
  double minDist       = 50000.;
  unsigned int closest = 0;

  // Compute the diff and keep the index that minimize the distance
  for (unsigned int idx = 0; idx < res.size(); idx++)
    {
    double diff = vcl_abs((double)(1<<idx) - invZoomfactor);

    if (diff < minDist)
      {
      minDist = diff;
      closest = idx;
      }
    }

  return closest;
}

/*****************************************************************************/
void
VectorImageModel
::SetupCurrentLodImage( int width, int height )
{
  CountType bestInitialLod = 0;
  // Compute the initial zoom factor and the best LOD.
  if( width>0 && height>0 )
    {
    ImageRegionType nativeLargestRegion( GetNativeLargestRegion() );

    double factorX =
      double( width ) / double( nativeLargestRegion.GetSize()[ 0 ] );

    double factorY =
      double( height ) / double( nativeLargestRegion.GetSize()[ 1 ] );

    double initialZoomFactor = std::min(factorX, factorY);

    // Compute the best lod from the initialZoomFactor
    bestInitialLod = ComputeBestLod(initialZoomFactor);
    }

  this->SetCurrentLod( bestInitialLod );
}

/*****************************************************************************/
CountType
VectorImageModel
::GetNbLod() const
{
  return m_AvailableLod.size();
}

/*****************************************************************************/
void
VectorImageModel
::virtual_SetCurrentLod( CountType lod )
{
  // new filename if lod is not 0
  QString lodFilename( m_Filename );

  // If model is a multi-resolution image.
  lodFilename.append( QString( "?&resol=%1" ).arg( lod ) );

  // Update m_ImageFileReader
  DefaultImageFileReaderType::Pointer fileReader(
    DefaultImageFileReaderType::New()
  );

  fileReader->SetFileName( QFile::encodeName(lodFilename).constData() );
  fileReader->UpdateOutputInformation();

  m_ImageFileReader = fileReader;

  // (Always) Update m_Image reference.
  m_Image = m_ImageFileReader->GetOutput();
}

/*****************************************************************************/
void
VectorImageModel
::virtual_RefreshHistogram()
{
  DatasetModel* datasetModel = GetDatasetModel();
  assert( datasetModel!=NULL );

  assert( GetProperties()!=NULL );

  HistogramModel::BuildContext context(
    true, // Force writing new histogram-model.
    datasetModel->HistogramFileInfo( GetFilename() ).filePath()
  );

  RefreshHistogram( &context );
}

/*****************************************************************************/
ImageBaseType::ConstPointer
VectorImageModel
::ToImageBase() const
{
  return ImageBaseType::ConstPointer( m_Image );
}

/*****************************************************************************/
ImageBaseType::Pointer
VectorImageModel
::ToImageBase()
{
  return ImageBaseType::Pointer( m_Image );
}

/*****************************************************************************/
std::string
VectorImageModel
::GetCenterPixelPlaceName()
{
  // center index
  IndexType centerIndex;
  centerIndex[0] = GetNativeLargestRegion().GetIndex()[0] + GetNativeLargestRegion().GetSize(0)/2;
  centerIndex[1] = GetNativeLargestRegion().GetIndex()[1] + GetNativeLargestRegion().GetSize(1)/2;
  
  //
  // Compute the physical coordinates of the center pixel 
  PointType centerPoint;
  centerPoint[0] = (centerIndex[0] *   GetNativeSpacing()[0] )  + GetOrigin()[0];
  centerPoint[1] = (centerIndex[1] *   GetNativeSpacing()[1] )  + GetOrigin()[1];
  
  // lat / long
  PointType wgs84;
  wgs84 = GetGenericRSTransform()->TransformPoint(centerPoint);
  
  // get placename
  otb::CoordinateToName::Pointer coordinateToName = otb::CoordinateToName::New();
  coordinateToName->SetLonLat(wgs84);
  coordinateToName->Evaluate();

  // get the placename - Country (if any)
  std::ostringstream oss;
  
  std::string placeName = coordinateToName->GetPlaceName();
  std::string countryName = coordinateToName->GetCountryName();
  
  if (placeName != "")
    oss << placeName;
  
  if (countryName != "")
    oss << " - "<< countryName;

  return oss.str();
}

/*****************************************************************************/
bool
VectorImageModel
::IsModified() const
{
  return
    GetSettings().IsModified() ||
    ( GetProperties()!=NULL && GetProperties()->IsModified() );
}

/*****************************************************************************/
void
VectorImageModel
::ClearModified()
{
  GetSettings().ClearModified();

  if( GetProperties()!=NULL )
    GetProperties()->ClearModified();

  // TODO: Remove temporary hack (Quicklook modified flag).
  QuicklookModel* quicklookModel = GetQuicklookModel();
  // If image-model is not quicklook-model.
  if( quicklookModel!=NULL )
    quicklookModel->ClearModified();
}

/*****************************************************************************/
void
VectorImageModel
::ApplySettings()
{
  // qDebug() << this << "::ApplySettings()";

  // TODO: Remove temporary hack (Quicklook rendering settings).
  QuicklookModel* quicklookModel = GetQuicklookModel();
  // If image-model is not quicklook-model.
  if( quicklookModel!=NULL )
    {
    // Update quicklook rendering-settings.
    quicklookModel->SetSettings( GetSettings() );
    quicklookModel->ApplySettings();
    }
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
VectorImageModel
::OnModelUpdated()
{
  // qDebug() << this << "::OnModelUpdated()";

  // Apply settings to rendering pipeline.
  ApplySettings();

  // Emit settings update to notify display refresh.
  emit SettingsUpdated( this );

  // Emit properties update.
  emit PropertiesUpdated( this );
}

/*****************************************************************************/
void
VectorImageModel
::OnPhysicalCursorPositionChanged( const PointType& point,
                                   const DefaultImageType::PixelType& pixel )
{
// Pixel is read from otb::GlImageActor (inside ImageViewRender)
// which does only components contain red, green and blue channel components.
// Only read, green and blue channel related component are
// present.
#define USE_RGB_CHANNELS_LIMIT 1

  // stream to fill
  std::ostringstream ossPhysicalX;
  std::ostringstream ossPhysicalY;
  std::ostringstream ossGeographicLong;
  std::ostringstream ossGeographicLat;
  std::ostringstream ossGeographicElevation;
  std::ostringstream ossRadio;

  // emitted current pixel
  QStringList      bandNames;
#if USE_RGB_CHANNELS_LIMIT
  QStringList stringList;
#endif

  //emitted current geography
  StringVector     geoVector;
  QStringList      geoList;

  //emitted current geography
  StringVector     cartoVector;
  QStringList      cartoList;

  SpacingType nativeSpacing( GetNativeSpacing() );

  // physical coordinates to index (at resol 0)
  IndexType currentIndex;

  currentIndex[ 0 ]
    = static_cast< unsigned int >(
      ( point[ 0 ] - GetOrigin()[ 0 ] ) / nativeSpacing[ 0 ]
    );

  currentIndex[ 1 ] =
    static_cast< unsigned int >(
      ( point[ 1 ] - GetOrigin()[ 1 ] ) / nativeSpacing[ 1 ]
    );

  bool isInsideNativeLargestRegion =
    GetNativeLargestRegion().IsInside( currentIndex );

  emit CurrentIndexUpdated( currentIndex, isInsideNativeLargestRegion );

  //
  // Display the radiometry of the displayed channels
  Settings::ChannelVector rgb;
  GetSettings().GetSmartChannels( rgb );

  // show the current pixel description only if the mouse cursor is
  // under the image
  if( isInsideNativeLargestRegion || 1 )
    {   
    //
    // get the physical coordinates
    if (!ToImage()->GetProjectionRef().empty())
      {
      cartoVector.push_back(ToStdString(tr("Cartographic")));
      }
    else
      {
      //No cartographic info available
      cartoVector.push_back(ToStdString(tr("Physical")));
      }

    ossPhysicalX << point[ 0 ];
    ossPhysicalY << point[ 1 ];

    cartoVector.push_back(ossPhysicalX.str());
    cartoVector.push_back(ossPhysicalY.str());

    // index in current Lod image
    IndexType currentLodIndex;
    currentLodIndex[0] = (point[ 0 ] - ToImage()->GetOrigin()[0]) / ToImage()->GetSpacing()[0];
    currentLodIndex[1] = (point[ 1 ] - ToImage()->GetOrigin()[1]) / ToImage()->GetSpacing()[1];
    
    //
    // get the LatLong
    
    if (!ToImage()->GetProjectionRef().empty()) 
      {
      geoVector.push_back(ToStdString(tr("Geographic(exact)")));
      }
    else if (ToImage()->GetImageKeywordlist().GetSize() != 0)
      {
      geoVector.push_back(ToStdString(tr("Geographic(sensor model)")));
      }
    else
      {
      geoVector.push_back(ToStdString(tr("No geoinfo")));
      }

    if( ToImage()->GetLargestPossibleRegion().IsInside(currentLodIndex) || 1 )
      {
      // TODO : Is there a better method to detect no geoinfo available ?
      if (!ToImage()->GetProjectionRef().empty() || ToImage()->GetImageKeywordlist().GetSize() != 0) 
        {
        PointType wgs84;
        wgs84 = GetGenericRSTransform()->TransformPoint(point);
      
        ossGeographicLong.precision(6);
        ossGeographicLat.precision(6);
        
        ossGeographicLong << std::fixed << wgs84[0];
        ossGeographicLat << std::fixed << wgs84[1];

        geoVector.push_back(ossGeographicLong.str());
        geoVector.push_back(ossGeographicLat.str());
      
        double elev = otb::DEMHandler::Instance()->GetHeightAboveEllipsoid(wgs84[0],wgs84[1]);

        if(elev > -32768)
          {
          ossGeographicElevation << elev;
          geoVector.push_back(ossGeographicElevation.str());
          }
        else
          geoVector.push_back( "" );
        }
      else
        {
        //No geoinfo available
        geoVector.push_back("");
        geoVector.push_back("");
        geoVector.push_back("");
        }
      }
    /*
    else
      {
      //handle here the case of QL information display. It 
      //displays geographic info when the user is scrolling over the QL
      //
      // compute the current ql index

      if (!ToImage()->GetProjectionRef().empty() || ToImage()->GetImageKeywordlist().GetSize() != 0) 
        {
      currentLodIndex[0] = (Xpc - GetQuicklookModel()->ToImage()->GetOrigin()[0]) 
        / GetQuicklookModel()->ToImage()->GetSpacing()[0];
      currentLodIndex[1] = (Ypc - GetQuicklookModel()->ToImage()->GetOrigin()[1]) 
        / GetQuicklookModel()->ToImage()->GetSpacing()[1];

       PointType wgs84;
       PointType currentLodPoint;
       GetQuicklookModel()->ToImage()->TransformIndexToPhysicalPoint(currentLodIndex, currentLodPoint);
       wgs84 = GetGenericRSTransform()->TransformPoint(currentLodPoint);
      
       ossGeographicLong.precision(6);
       ossGeographicLat.precision(6);
       
       ossGeographicLong << std::fixed << wgs84[0];
       ossGeographicLat << std::fixed << wgs84[1];

       //Update geovector with location over QL index
       geoVector.push_back(ossGeographicLong.str());
       geoVector.push_back(ossGeographicLat.str());
      
       double elev = otb::DEMHandler::Instance()->GetHeightAboveEllipsoid(wgs84[0],wgs84[1]);

       if(elev > -32768)
         {
         ossGeographicElevation << elev;
         geoVector.push_back(ossGeographicElevation.str());
         }
        }
      else
        {
        //No geoinfo available
        geoVector.push_back("");
        geoVector.push_back("");
        geoVector.push_back("");
        }
      }
    */

    cartoList = ToQStringList( cartoVector );
    geoList = ToQStringList( geoVector );

    if( true /* ToImage()->GetBufferedRegion().IsInside(currentLodIndex) */ )
      {
      /*
      //
      // get the pixel at current index
      currentPixel = ToImage()->GetPixel(currentLodIndex);
      */

      ossRadio << ToStdString( tr( "Radiometry: [ " ) );

#if USE_RGB_CHANNELS_LIMIT
      for( unsigned int i=0; i<pixel.GetSize(); ++i )
        ossRadio << pixel.GetElement( i ) << " ";
#else
      for (unsigned int idx = 0; idx < rgb.size(); idx++)
        {
        ossRadio << pixel.GetElement(rgb[idx]) << " ";
        }
#endif

      ossRadio << "]";

      // qDebug() << ossRadio.str().c_str();
      }
    /*
    else
      {
      //
      // compute the current ql index
      currentLodIndex[0] = (Xpc - GetQuicklookModel()->ToImage()->GetOrigin()[0]) 
        / GetQuicklookModel()->ToImage()->GetSpacing()[0];
      currentLodIndex[1] = (Ypc - GetQuicklookModel()->ToImage()->GetOrigin()[1]) 
        / GetQuicklookModel()->ToImage()->GetSpacing()[1];
    
      //
      // Get the radiometry form the Ql
      if ( GetQuicklookModel()->ToImage()->GetBufferedRegion().IsInside(currentLodIndex) )
        {
        currentPixel = 
          GetQuicklookModel()->ToImage()->GetPixel(currentLodIndex);

        ossRadio <<"Ql [ ";
        for (unsigned int idx = 0; idx < rgb.size(); idx++)
          {
          ossRadio <<currentPixel.GetElement(rgb[idx]) << " ";
          }
        ossRadio <<"]";
        }
      }
    */
    }

  // update band name for the current position 
  bandNames = GetBandNames( true );

    // qDebug() << bandNames;

#if USE_RGB_CHANNELS_LIMIT
  stringList
    << bandNames.at( rgb[ RGBW_CHANNEL_RED ] )
    << bandNames.at( rgb[ RGBW_CHANNEL_GREEN ] )
    << bandNames.at( rgb[ RGBW_CHANNEL_BLUE ] );

  // qDebug() << "Bands:" << stringList;
#endif

  // update the status bar
  emit CurrentPhysicalUpdated( cartoList );
  emit CurrentGeographicUpdated( geoList );
  emit CurrentRadioUpdated( ToQString( ossRadio.str().c_str() ) );
#if USE_RGB_CHANNELS_LIMIT
  emit CurrentPixelValueUpdated( pixel,  stringList );
#else
  emit CurrentPixelValueUpdated( pixel,  bandNames );
#endif
}

} // end namespace 'mvd'
