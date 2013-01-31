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
#if defined(OTB_USE_JPEG2000)
# include "otbJPEG2000ImageIO.h"
#endif

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdVectorImageModel.h"
#include "mvdAlgorithm.h"


namespace mvd
{
/*
  TRANSLATOR mvd::VectorImageModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
VectorImageModel
::VectorImageModel( QObject* parent ) :
  AbstractImageModel( parent ),
  m_ImageFileReader(),
  m_Image(),
  m_RasterizedBuffer( NULL ),
  m_ExtractFilter(),
  m_RenderingFilter(),
  m_Settings(),
  m_PreviousRegion(),
  m_AlreadyLoadedRegion(),
  m_Region(),
  m_RegionsToLoadVector(),
  m_PreviousBestLevelOfDetail(0)
{
}

/*******************************************************************************/
VectorImageModel
::~VectorImageModel()
{
  this->ClearBuffer();
}

/*******************************************************************************/
QStringList
VectorImageModel
::GetBandNames() const
{
  assert( m_ImageFileReader->GetNumberOfOutputs()==1 );

  DefaultImageType::Pointer output( m_ImageFileReader->GetOutput( 0 ) );

  /*
  itk::MetaDataDictionary dictionary( output->GetMetaDataDictionary() );

  DefaultImageType::ImageMetadataInterfacePointerType metaData(
    GetMetadataInferface()
  );
  */

  StringVector stdBandNames( GetMetadataInterface()->GetBandName() );

  /*
  qDebug() << "stdBandNames.size(): " <<  stdBandNames.size();
  for( unsigned int i=0; i<stdBandNames.size(); ++i )
    {
    qDebug() << i << ": " << QString::fromStdString( stdBandNames[ i ] );
    }
  */

  assert( stdBandNames.empty() ||
	  stdBandNames.size()==output->GetNumberOfComponentsPerPixel() );

  if( stdBandNames.size()!=output->GetNumberOfComponentsPerPixel() )
    {
    stdBandNames.resize( output->GetNumberOfComponentsPerPixel() );
    }

  return ToQStringList( stdBandNames );
}

/*******************************************************************************/
void
VectorImageModel
::LoadFile( const QString& filename )
{
  // test if valid jpeg2k file
  //if (!this->IsJPEG2000File( filename.toLatin1().data() ))

  //
  // 1. Setup file-reader.
  DefaultImageFileReaderType::Pointer imageFileReader(
    DefaultImageFileReaderType::New()
  );

  imageFileReader->SetFileName( filename.toLatin1().data() );
  imageFileReader->UpdateOutputInformation();

  m_ImageFileReader = imageFileReader;

  // Ensure this only one output.
  assert( m_ImageFileReader->GetNumberOfOutputs()==1 );
  // Wrap output image.
  m_Image = m_ImageFileReader->GetOutput( 0 );

  //
  // 2. Initialize internal settings.
  DefaultImageType::ImageMetadataInterfacePointerType metaData(
    GetMetadataInterface()
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

  m_Settings.SetRgbChannels( rgb );

  // store the input filename
  m_InputFilename = filename.toLatin1().data();
}

/*******************************************************************************/
void
VectorImageModel
::ClearBuffer()
{
  // Delete previous buffer if needed
  delete[] m_RasterizedBuffer;
  m_RasterizedBuffer = NULL;
}

/*******************************************************************************/
unsigned char *
VectorImageModel
::RasterizeRegion( const ImageRegionType& region, const double zoomFactor)
{
  m_Region = region;

  // Get the best level of detail
  // don't reinstanciate the reader if the lod requested has not
  // changed 
  int best_lod = 0;
  if ( this->GetBestLevelOfDetail(zoomFactor, best_lod) && 
       best_lod != m_PreviousBestLevelOfDetail)
    {
    std::ostringstream oss;
    oss<<m_InputFilename<<"?&resol="<<best_lod;
    m_ImageFileReader = DefaultImageFileReaderType::New();
    m_ImageFileReader->SetFileName( oss.str() );
    m_ImageFileReader->UpdateOutputInformation();

    m_PreviousBestLevelOfDetail = best_lod;
    }

  // Don't do anything if the region did not changed
  if ( m_PreviousRegion!=region ||
       m_Settings.IsDirty() )
    {
    // check that the current and the previous region have some pixels in
    // common 
    ImageRegionType  tempPreviousRegionRegion = m_PreviousRegion;
    bool res =  tempPreviousRegionRegion.Crop(region);

    // if the first time or no pixels in common , reload all
    if ( res &&
	 m_PreviousRegion!=ImageRegionType() &&
	 !m_Settings.IsDirty() )
      {
      // Compute loaded region, and the four regions not loaded yet
      // within the new requested region
      this->ComputeRegionsToLoad(m_Region);

      // Copy the previous buffer into a temporary buf to access the
      // previously loaded data
      unsigned char * previousRasterizedBuffer =  
        new unsigned char[3 * m_PreviousRegion.GetNumberOfPixels()];

      std::memcpy(previousRasterizedBuffer, m_RasterizedBuffer, 3 * m_PreviousRegion.GetNumberOfPixels());

      // Clear the previous buffer 
      this->ClearBuffer();

      // Allocate new memory
      unsigned int nbPixels = 3 * region.GetNumberOfPixels();
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
          m_RasterizedBuffer[newBufferindex]     = previousRasterizedBuffer[3*idx];
          m_RasterizedBuffer[newBufferindex + 1] = previousRasterizedBuffer[3*idx + 1];
          m_RasterizedBuffer[newBufferindex + 2] = previousRasterizedBuffer[3*idx + 2];
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
      m_RasterizedBuffer = new unsigned char[3 * region.GetNumberOfPixels()];

      // rasterize the region
      this->DumpImagePixelsWithinRegionIntoBuffer(region);
      }
    }

  // Store the region
  m_PreviousRegion = region;
  
  // if ok return the  buffer
  return m_RasterizedBuffer;
}

/*******************************************************************************/
void
VectorImageModel
::DumpImagePixelsWithinRegionIntoBuffer(const ImageRegionType& region)
{
  // Before doing anything, check if region is inside the buffered
  // region of image
  unsigned int currentIndex = 0;

  // TODO : add some checking
  const DefaultImageType* image =  this->GetOutput(currentIndex);

  // some checking
  if (!image->GetBufferedRegion().IsInside(region))
    {
    //itkExceptionMacro(<< "Region to read is oustside of the buffered region.");
    }
  
  // Extract the region of interest in the image
  m_ExtractFilter = ExtractFilterType::New();
  m_ExtractFilter->SetInput(image);
  m_ExtractFilter->SetExtractionRegion(region);

  // Use the rendering filter to get 
  m_RenderingFilter = RenderingFilterType::New();
  m_RenderingFilter->SetInput(m_ExtractFilter->GetOutput());
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

  // TODO: Remove local variable.
  // Local variable because RenderingFunction::SetChannels() gets a
  // non-const std::vector< unsigned int >& as argument instead of a
  // const one.
  Settings::ChannelVector rgb( m_Settings.GetRgbChannels() );
  m_RenderingFilter->GetRenderingFunction()->SetChannelList(
    rgb
  );
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
    m_RasterizedBuffer[index]  = it.Get()[0];
    m_RasterizedBuffer[index + 1] = it.Get()[1];
    m_RasterizedBuffer[index + 2] = it.Get()[2];
    ++it;
    }
}

/*******************************************************************************/
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

/*******************************************************************************/
bool
VectorImageModel::GetBestLevelOfDetail(const double zoomFactor, int& lod)
{
  // Note : index 0 is the full resolution image
  unsigned int  best_lod = 0;
  unsigned int best_factor = 1;
  int inverseZoomFactor =  static_cast<int>((1/zoomFactor + 0.5));
    
#if defined(OTB_USE_JPEG2000)
  otb::JPEG2000ImageIO::Pointer readerJPEG2000 = otb::JPEG2000ImageIO::New();

  readerJPEG2000->SetFileName(m_InputFilename);
  if (readerJPEG2000->CanReadFile(m_InputFilename.c_str()))
    {
    // fill the resolution vector, to be able to find the closest
    // j2k res to the inverse zoom factor
    std::vector<unsigned int> res;
    if (readerJPEG2000->GetAvailableResolutions(res))
      {
      lod = this->Closest(inverseZoomFactor, res);
      return true;
      }
    }
  else
    {
    return false;
    }
#endif

  return false;
}

/*******************************************************************************/
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

/*******************************************************************************/
CountType
VectorImageModel
::GetNbLod() const
{
  // TODO: Implement method.
  return 0;
}

/*******************************************************************************/
void
VectorImageModel
::virtual_SetCurrentLod( unsigned int lod )
{
  // TODO: Update current m_ImageFileReader and/or m_Image.
}

/*******************************************************************************/
ImageBaseType::ConstPointer
VectorImageModel
::ToImageBase() const
{
  return m_Image.GetPointer();
}

/*******************************************************************************/
ImageBaseType::Pointer
VectorImageModel
::ToImageBase()
{
  return m_Image.GetPointer();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
VectorImageModel
::OnCurrentIndexChanged( RgbaChannel channel, int index )
{
  m_Settings.RgbChannel( channel ) = index;

  // TODO: Remove local variable.
  // Local variable because RenderingFunction::SetChannels() gets a
  // non-const std::vector< unsigned int >& as argument instead of a
  // const one.
  Settings::ChannelVector rgb( m_Settings.GetRgbChannels() );
  m_RenderingFilter->GetRenderingFunction()->SetChannelList( rgb );

  emit SettingsUpdated();
}

/*******************************************************************************/
void
VectorImageModel
::OnLowQuantileChanged( RgbaChannel, double )
{
}

/*******************************************************************************/
void
VectorImageModel
::OnHighQuantileChanged( RgbaChannel, double )
{
}

/*******************************************************************************/
void
VectorImageModel
::OnLowIntensityChanged( RgbaChannel, double )
{
}

/*******************************************************************************/
void
VectorImageModel
::OnHighIntensityChanged( RgbaChannel, double )
{
}

/*******************************************************************************/
void
VectorImageModel
::OnResetIntensityClicked( RgbaChannel )
{
}

/*******************************************************************************/
void
VectorImageModel
::OnResetQuantileClicked( RgbaChannel )
{
}

} // end namespace 'mvd'
