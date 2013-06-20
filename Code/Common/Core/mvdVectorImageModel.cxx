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

//
// OTB includes (sorted by alphabetic order)
#include "otbConfigure.h"
#include "otbGDALDriverManagerWrapper.h"
#include "otbStandardOneLineFilterWatcher.h"
#include "otbGeoInformationConversion.h"
#include "otbCoordinateToName.h"
#include "otbDEMHandler.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"
#include "mvdQuicklookModel.h"


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
  m_RasterizedBuffer( NULL ),
  m_ExtractFilter(),
  m_RenderingFilter(),
  m_Settings(),
  m_PreviousRegion(),
  m_AlreadyLoadedRegion(),
  m_Region(),
  m_RegionsToLoadVector(),
  m_Filename()
{
  QObject::connect(
    this,
    SIGNAL( ViewportRegionChanged(double, double) ),
    this,
    SLOT( OnPhysicalCursorPositionChanged(double, double))
    );
}

/*****************************************************************************/
VectorImageModel
::~VectorImageModel()
{
  this->ClearBuffer();
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

  //
  // 2. Setup file-reader.
  SetupCurrentLodImage(w, h);
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
    qDebug() << tr("The file already has overviews!");
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
  qDebug() << tr("Caching of overviews.");
  typedef otb::GDALOverviewsBuilder FilterType;
  FilterType::Pointer filter = FilterType::New();

  //m_ImageFileReader->GetAvailableResolutions(m_AvailableLod);

  std::string tempfilename( ToStdString( m_Filename ) );

  filter->SetInputFileName(tempfilename);
  filter->SetResamplingMethod(otb::AVERAGE);
  filter->SetNbOfResolutions(m_ImageFileReader->GetAvailableResolutions().size());

  try
    {
    otb::StandardOneLineFilterWatcher watcher(
      filter,
      ToStdString( tr( "Overviews creation: " ) )
    );
    filter->Update();
    std::cout << std::endl;
    }
  catch( std::exception& exc )
    {
    // The user can continue to use the file so we return a warning message
    // TODO MSD return the message to the log widget
    qWarning() << tr( "The overviews creation failed.\n"
        "Navigation in resolution will be slower." );

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

  // Initialize RgbaImageModel.
  InitializeRgbaPipeline();
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

  mvd::RgbBounds( begin, end, RGBW_CHANNEL_RGB );

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
void
VectorImageModel
::InitializeRgbaPipeline()
{
  m_ExtractFilter = ExtractFilterType::New();
  m_ExtractFilter->SetInput( m_Image );

  m_RenderingFilter = RenderingFilterType::New();
  m_RenderingFilter->SetInput( m_ExtractFilter->GetOutput() );

  ApplySettings();
}

/*****************************************************************************/
void
VectorImageModel
::ClearBuffer()
{
  // Delete previous buffer if needed
  delete[] m_RasterizedBuffer;
  m_RasterizedBuffer = NULL;
}

/*****************************************************************************/
unsigned char *
VectorImageModel
::RasterizeRegion( const ImageRegionType& region, const double zoomFactor, bool refresh)
{
  m_Region = region;

  // Compute the best level of detail
  CountType bestLod = this->ComputeBestLevelOfDetail(zoomFactor);

  // Set the corresponding Level of Detail
  if( GetCurrentLod()!=bestLod )
    {
    this->SetCurrentLod(bestLod);
    }

  // Don't do anything if the region did not changed
  if ( m_PreviousRegion!=region ||
       !GetSettings().IsApplied() ||
       refresh )
    {
    // check that the current and the previous region have some pixels in
    // common 
    ImageRegionType  tempPreviousRegionRegion = m_PreviousRegion;
    bool res =  tempPreviousRegionRegion.Crop(region);

    // if the first time or no pixels in common , reload all
    if ( res &&
   m_PreviousRegion!=ImageRegionType() &&
   GetSettings().IsApplied() &&
         !refresh )
      {
      // Compute loaded region, and the four regions not loaded yet
      // within the new requested region
      this->ComputeRegionsToLoad(m_Region);

      // Copy the previous buffer into a temporary buf to access the
      // previously loaded data
      unsigned char * previousRasterizedBuffer =  
        new unsigned char[4 * m_PreviousRegion.GetNumberOfPixels()];

      std::memcpy(previousRasterizedBuffer, m_RasterizedBuffer, 4 * m_PreviousRegion.GetNumberOfPixels());

      // Clear the previous buffer 
      this->ClearBuffer();

      // Allocate new memory
      unsigned int nbPixels = 4 * region.GetNumberOfPixels();
      m_RasterizedBuffer = new unsigned char[nbPixels];
    
      // Copy the already loaded pixels into the m_RasterizedBuffer
      unsigned int previousNbPixels = m_PreviousRegion.GetNumberOfPixels();
      
      for (unsigned int idx = 0; idx < previousNbPixels; idx++)
        {
        // compose the image index from the buffer index
        IndexType imageIndex;
        imageIndex = ComputeImageIndexFromFlippedBuffer(idx, m_PreviousRegion);

        if (m_AlreadyLoadedRegion.IsInside(imageIndex))
          {
          // Get the buffer index relative to the imageIndex in the
          // new requested region 
          unsigned int newBufferindex = 0;
          newBufferindex = ComputeXAxisFlippedBufferIndex(imageIndex, m_Region);
          
          // Copy the already loaded values into the new buffer
          m_RasterizedBuffer[newBufferindex]     = previousRasterizedBuffer[4*idx];
          m_RasterizedBuffer[newBufferindex + 1] = previousRasterizedBuffer[4*idx + 1];
          m_RasterizedBuffer[newBufferindex + 2] = previousRasterizedBuffer[4*idx + 2];
          m_RasterizedBuffer[newBufferindex + 3] = previousRasterizedBuffer[4*idx + 3];
          }
        }
      
      // Dump pixels not loaded in the m_RasterizedBuffer
      for (unsigned int idx = 0; idx < m_RegionsToLoadVector.size() ; idx++)
        {
        this->DumpImagePixelsWithinRegionIntoBuffer(m_RegionsToLoadVector[idx]);
        }

      // free the previous buffer memory (copied one)
      if (previousRasterizedBuffer != NULL)
        {
        delete[] previousRasterizedBuffer;
        previousRasterizedBuffer = NULL;
        }
      }
    else
      {
      // Delete previous buffer if needed
      this->ClearBuffer();
 
      // Allocate new memory
      m_RasterizedBuffer = new unsigned char[4 * region.GetNumberOfPixels()];

      // rasterize the region
      this->DumpImagePixelsWithinRegionIntoBuffer(region);
      }
    }

  // settings changes have been taken into account, clean the dirty flag
  GetSettings().SetApplied();

  // Store the region
  m_PreviousRegion = region;
  
  // if ok return the  buffer
  return m_RasterizedBuffer;
}

/*****************************************************************************/
void
VectorImageModel
::DumpImagePixelsWithinRegionIntoBuffer(const ImageRegionType& region)
{
  // Before doing anything, check if region is inside the buffered
  // region of image

  // TODO : add some checking

  assert( !m_Image.IsNull() );

  // some checking
  if (!m_Image->GetBufferedRegion().IsInside(region))
    {
    //itkExceptionMacro(<< "Region to read is oustside of the buffered region.");
    }

  assert( !m_ExtractFilter.IsNull() );

  // Extract the region of interest in the m_Image
  m_ExtractFilter->SetInput( m_Image );
  m_ExtractFilter->SetExtractionRegion(region);

  assert( !m_RenderingFilter.IsNull() );

  // Use the rendering filter to get 
  m_RenderingFilter->GetRenderingFunction()->SetAutoMinMax(false);

// ----------------------------------
  // RenderingFilterType::RenderingFunctionType::ParametersType  paramsMinMax;
  // paramsMinMax.SetSize(6);
    
  // // Update the parameters
  // for (unsigned int i = 0; i < paramsMinMax.Size(); i = i + 2)
  //   {
  //   paramsMinMax.SetElement(i, 30);
  //   paramsMinMax.SetElement(i + 1, 2048/*256*/);
  //   }
  //m_RenderingFilter->GetRenderingFunction()->SetParameters(paramsMinMax);
// ---------------------------- 

  m_RenderingFilter->GetOutput()->SetRequestedRegion(region);
  m_RenderingFilter->Update();

  // Declare the iterator
  itk::ImageRegionConstIteratorWithIndex< RenderingFilterType::OutputImageType >
    it(m_RenderingFilter->GetOutput(), region);

  // Go to begin
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    // Fill the buffer
    unsigned int index = 0;
    index = ComputeXAxisFlippedBufferIndex(it.GetIndex(), m_Region);

    // Fill the buffer
    m_RasterizedBuffer[index]  = it.Get()[2];
    m_RasterizedBuffer[index + 1] = it.Get()[1];
    m_RasterizedBuffer[index + 2] = it.Get()[0]; 

    // Could be changed later for real alpha channal
    m_RasterizedBuffer[index + 3] = 255;
    ++it;
    }
}

/*****************************************************************************/
void
VectorImageModel
::ComputeRegionsToLoad(const ImageRegionType& region)
{
  // Initialize the region and clear vector
  m_AlreadyLoadedRegion = m_PreviousRegion;
  m_RegionsToLoadVector.clear();

  // 4 regions to compute
  ImageRegionType upperRegion;
  ImageRegionType lowerRegion;
  ImageRegionType leftRegion;
  ImageRegionType rightRegion;

  // local variables
  IndexType index;
  ImageRegionType::SizeType size;

  // Compute the already loaded region as a simple Crop
  m_AlreadyLoadedRegion.Crop(region);

  // ------ upper region
  upperRegion.SetIndex(region.GetIndex());
  size[0] = region.GetSize()[0];
  size[1] = vcl_abs( region.GetIndex()[1] - m_AlreadyLoadedRegion.GetIndex()[1]);
  upperRegion.SetSize(size);

  // ------ lower region
  index[0] = region.GetIndex()[0];
  index[1] = m_AlreadyLoadedRegion.GetSize()[1] + m_AlreadyLoadedRegion.GetIndex()[1];
  lowerRegion.SetIndex(index);

  size[0] = region.GetSize()[0];
  size[1] = vcl_abs( static_cast<int>(region.GetIndex()[1] + region.GetSize()[1] 
                     - m_AlreadyLoadedRegion.GetIndex()[1] - m_AlreadyLoadedRegion.GetSize()[1] ));
  lowerRegion.SetSize(size);

  // ------- left region
  index[0] = region.GetIndex()[0];
  index[1] = m_AlreadyLoadedRegion.GetIndex()[1];
  leftRegion.SetIndex(index);

  size[0] = vcl_abs(region.GetIndex(0) - m_AlreadyLoadedRegion.GetIndex()[0]);
  size[1] = m_AlreadyLoadedRegion.GetSize()[1];
  leftRegion.SetSize(size);

  // -------- right region
  index[0] = m_AlreadyLoadedRegion.GetIndex()[0] + m_AlreadyLoadedRegion.GetSize()[0];
  index[1] = m_AlreadyLoadedRegion.GetIndex()[1];
  rightRegion.SetIndex(index);

size[0] = vcl_abs(static_cast<int>(region.GetSize()[0] + region.GetIndex()[0] 
                                   - m_AlreadyLoadedRegion.GetIndex()[0] - m_AlreadyLoadedRegion.GetSize()[0]));
  size[1] = m_AlreadyLoadedRegion.GetSize()[1]; 
  rightRegion.SetSize(size);

  // add the upper region if any pixel
  if ( upperRegion .GetNumberOfPixels() > 0 )
    m_RegionsToLoadVector.push_back(upperRegion);

  // add the lower region if any pixel
  if ( lowerRegion.GetNumberOfPixels() > 0 )
    m_RegionsToLoadVector.push_back(lowerRegion);

  // add the left region if any pixel
  if ( leftRegion.GetNumberOfPixels() > 0 )
    m_RegionsToLoadVector.push_back(leftRegion);

  // add the right region if any pixel
  if ( rightRegion.GetNumberOfPixels() > 0 )
    m_RegionsToLoadVector.push_back(rightRegion);
}

/*****************************************************************************/
CountType
VectorImageModel::ComputeBestLevelOfDetail(const double zoomFactor)
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
    bestInitialLod = ComputeBestLevelOfDetail(initialZoomFactor);
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

  try
    {
    fileReader->SetFileName( ToStdString( lodFilename ) );
    fileReader->UpdateOutputInformation();

    m_ImageFileReader = fileReader;
    }
  catch( std::exception& exc )
    {
    throw;
    }

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
  centerPoint[0] = (centerIndex[0] *  vcl_abs( GetNativeSpacing()[0] ) ) + GetOrigin()[0];
  centerPoint[1] = (centerIndex[1] *  vcl_abs( GetNativeSpacing()[1] ) ) + GetOrigin()[1];
  
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
::ReleaseMemory()
{
  // Release Quicklook as well
  QuicklookModel* quicklookModel = GetQuicklookModel();
  if(quicklookModel!=NULL)
    quicklookModel->ReleaseMemory();

  // Initialize the RGBA pipeline
  InitializeRgbaPipeline();

  // TODO: Do we need to clear the current buffer here ?

  // Clear file reader
  SetCurrentLod(GetCurrentLod());
}

void
VectorImageModel
::ApplySettings()
{
  qDebug() << this << "::ApplySettings()";

  RenderingFilterType::RenderingFunctionType* renderingFunc =
    m_RenderingFilter->GetRenderingFunction();

  Settings::ChannelVector rgb;
  GetSettings().GetRgbwChannels( rgb );

  renderingFunc->SetChannelList( rgb );
  renderingFunc->SetParameters( GetSettings().GetDynamicsParams() );

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
  qDebug() << this << "::OnModelUpdated()";

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
::OnPhysicalCursorPositionChanged(double Xpc, double Ypc)
{
  // stream to fill
  std::ostringstream ossIndex;
  std::ostringstream ossPhysical;
  std::ostringstream ossGeographicLong;
  std::ostringstream ossGeographicLat;
  std::ostringstream ossGeographicElevation;
  std::ostringstream ossRadio;

  // emitted current pixel
  VectorImageType::PixelType currentPixel;
  QStringList      bandNames;

  //emitted current geography
  StringVector     geoVector;
  QStringList      geoList;

  // the current physical point
  PointType point;
  point[0] = Xpc;
  point[1] = Ypc;

  // physical coordinates to index (at resol 0)
  IndexType currentIndex;
  currentIndex[0] = static_cast<unsigned int>( ( Xpc - GetOrigin()[0] ) / vcl_abs(GetNativeSpacing()[0]) );
  currentIndex[1] = static_cast<unsigned int>( ( Ypc - GetOrigin()[1] ) / vcl_abs(GetNativeSpacing()[1]) );

  // show the current pixel description only if the mouse cursor is
  // under the image
  if ( GetNativeLargestRegion().IsInside(currentIndex) )
    {
    //
    // get index stream
    ossIndex<< currentIndex[0] <<","<< currentIndex[1];
    
    //
    // get the physical coordinates
    if (!ToImage()->GetProjectionRef().empty())
      {
      ossPhysical.str("");
      ossPhysical<<"[" << Xpc <<","<< Ypc << "]";
      }

    // index in current Lod image
    IndexType currentLodIndex;
    currentLodIndex[0] = (Xpc - ToImage()->GetOrigin()[0]) / vcl_abs(ToImage()->GetSpacing()[0]);
    currentLodIndex[1] = (Ypc - ToImage()->GetOrigin()[1]) / vcl_abs(ToImage()->GetSpacing()[1]);
    
    //
    // get the LatLong
    // TODO : Is there a better method to detect no geoinfo available ?
    if ( ToImage()->GetBufferedRegion().IsInside(currentLodIndex))
      {
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
        }
      else
        {
        geoVector.push_back("");
        geoVector.push_back("");
        geoVector.push_back("");
        }
      }
    else
      {
      //FIXME handle here the case of QL information display. Would be nice to
      //display geographic info when the user is scrolling over the QL
      geoVector.push_back("");
      geoVector.push_back("");
      geoVector.push_back("");
      }
    geoList = ToQStringList( geoVector );

    //
    // Display the radiometry of the displayed channels
    Settings::ChannelVector rgb;
    GetSettings().GetRgbwChannels( rgb );

    if ( ToImage()->GetBufferedRegion().IsInside(currentLodIndex) )
      {
      //
      // get the pixel at current index
      currentPixel = ToImage()->GetPixel(currentLodIndex);

      ossRadio <<ToStdString( tr("Radiometry") )<<" : [ ";
      for (unsigned int idx = 0; idx < rgb.size(); idx++)
        {
        ossRadio <<currentPixel.GetElement(rgb[idx]) << " ";
        }
      ossRadio <<"]";

      }
    else
      {
      //
      // compute the current ql index
      currentLodIndex[0] = (Xpc - GetQuicklookModel()->ToImage()->GetOrigin()[0]) 
        / vcl_abs(GetQuicklookModel()->ToImage()->GetSpacing()[0]);
      currentLodIndex[1] = (Ypc - GetQuicklookModel()->ToImage()->GetOrigin()[1]) 
        / vcl_abs(GetQuicklookModel()->ToImage()->GetSpacing()[1]);
    
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

    // update band name for the current position 
    bandNames = GetBandNames(true);
    }

  // update the status bar
  emit CurrentIndexUpdated( QString(ossIndex.str().c_str()) );
  emit CurrentPhysicalUpdated( QString(ossPhysical.str().c_str()) );
  emit CurrentGeographicUpdated( geoList );
  emit CurrentRadioUpdated( QString(ossRadio.str().c_str()) );
  emit CurrentPixelValueUpdated( currentPixel,  bandNames);
}

/*****************************************************************************/
void 
VectorImageModel
::OnUserCoordinatesEditingFinished(const QString& coord)
{
  //
  // extract the coordinates from the given text
  QStringList parts = coord.split( ',' );
  if ( parts.size() != 2 )
    return;

  bool xOk;
  double x = parts.at( 0 ).toDouble( &xOk );
  if ( !xOk )
    return;

  bool yOk;
  double y = parts.at( 1 ).toDouble( &yOk );
  if ( !yOk )
    return;
  
  //
  // Center the viewport on this index if inside the largest
  // possible region
  IndexType index;
  index[0] = static_cast<unsigned int>(x);
  index[1] = static_cast<unsigned int>(y);
    
  if ( GetNativeLargestRegion().IsInside(index) )
    {
    // propagate the physical center
    double Xpc = index[0] * vcl_abs( GetNativeSpacing()[0] ) + GetOrigin()[0];
    double Ypc = index[1] * vcl_abs( GetNativeSpacing()[1] ) + GetOrigin()[1];

    emit ViewportRegionChanged( Xpc, Ypc );
    }
}

} // end namespace 'mvd'
