/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkRGBImage2DViewer.txx,v $
  Language:  C++
  Date:      $Date: 2003/11/03 17:08:43 $
  Version:   $Revision: 1.8 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "fltkRGBImage2DViewer.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkNumericTraits.h"

namespace fltk {
  
template <class ImagePixelType>
RGBImage2DViewer<ImagePixelType>
::RGBImage2DViewer()
{

  imageViewer->SetIntensityWindow( intensityWindow );
  imageViewer->SetParentWindow( externalWindow );
  
  m_RedrawCommand = ObserverCommandType::New();
  m_RedrawCommand->SetCallbackFunction( this, &Self::Update );
  
  m_Tag = 0L;

  m_FlipY = false ;
  m_OverlayOpacity = 0.5;
}


  
template <class ImagePixelType>
RGBImage2DViewer<ImagePixelType>
::~RGBImage2DViewer()
{
}


  
template <class ImagePixelType>
itk::Object *
RGBImage2DViewer<ImagePixelType>
::GetNotifier(void)
{
  return imageViewer->GetNotifier();
}




template <class ImagePixelType>
void
RGBImage2DViewer<ImagePixelType>
::SetImage(ImageType * image)
{
  if (imageViewer->GetBuffer() == 0)
    {
    m_ImageSize = image->GetRequestedRegion().GetSize() ;
    this->externalWindow->size(m_ImageSize[0], m_ImageSize[1]);
    imageViewer->Allocate( m_ImageSize[0], m_ImageSize[1] );

    // Fill the Image
 
    itk::ImageLinearConstIteratorWithIndex< ImageType > 
                                        it( image, image->GetRequestedRegion() );

    const int bytesPerPixel = imageViewer->GetNumberOfBytesPerPixel();

    it.SetDirection( 0 );

    const unsigned int totalSize  =
                m_ImageSize[0] * m_ImageSize[1] * bytesPerPixel;

    const unsigned int totalWidth = m_ImageSize[0] * bytesPerPixel;

    fltk::RGBImage2DViewerWindow::ValueType * buffer = imageViewer->GetBuffer();

    if(!m_FlipY)
      {
      unsigned char * dest = buffer + totalSize + 3 - totalWidth;
      it.GoToBegin();
      while( !it.IsAtEnd() )  // Should only have one slice...but anyway.
        {
        while( !it.IsAtEndOfLine() ) 
          { 
          const double valueR = it.Get().GetRed();
          const unsigned char valuecR = static_cast<unsigned char>( valueR );
          *dest = valuecR;
          dest++;
          const double valueG = it.Get().GetGreen();
          const unsigned char valuecG = static_cast<unsigned char>( valueG );
          *dest = valuecG;
          dest++;
          const double valueB = it.Get().GetBlue();
          const unsigned char valuecB = static_cast<unsigned char>( valueB );
          *dest = valuecB;
          dest++;
          ++it;
          }
        it.NextLine();
        dest -= 2 * totalWidth;
        }
      }
    else
      {
      unsigned char * dest = buffer;
      
      it.GoToBegin();
      while( !it.IsAtEnd() )  // Should only have one slice...but anyway.
        {
        while( !it.IsAtEndOfLine() ) 
          { 
          const double valueR = it.Get().GetRed();
          const unsigned char valuecR = static_cast<unsigned char>( valueR );
          *dest = valuecR;
          dest++;
          const double valueG = it.Get().GetGreen();
          const unsigned char valuecG = static_cast<unsigned char>( valueG );
          *dest = valuecG;
          dest++;
          const double valueB = it.Get().GetBlue();
          const unsigned char valuecB = static_cast<unsigned char>( valueB );
          *dest = valuecB;
          dest++;
          ++it;
          }
        it.NextLine();
        }
      }
    }

  if( m_Image && m_Tag )
    {
    m_Image->GetSource()->RemoveObserver( m_Tag );
    }
  m_Image = image;
   
  if(m_Image->GetSource())
  {
    m_Tag = m_Image->GetSource()->AddObserver( 
      itk::EndEvent(), m_RedrawCommand );
  }
 
  Update();

}

template <class ImagePixelType>
void
RGBImage2DViewer<ImagePixelType>
::SetOverlay(OverlayType * overlay)
{
  m_Overlay = overlay;
  m_ColorTable = ColorTableType::New();
  m_ColorTable->useDiscrete();
  UpdateOverlay();
}


template <class ImagePixelType>
void
RGBImage2DViewer<ImagePixelType>
::FillChannel(int channel, ChannelImageType* image)
{
  if (imageViewer->GetBuffer() == 0)
    {
      m_ImageRegion = image->GetRequestedRegion() ;
      m_ImageSize = m_ImageRegion.GetSize() ;
      this->externalWindow->size(m_ImageSize[0], m_ImageSize[1]);
      imageViewer->Allocate( m_ImageSize[0], m_ImageSize[1] );
    }

  typedef itk::MinimumMaximumImageCalculator<ChannelImageType> 
    MinMaxCalculatorType;
  typename MinMaxCalculatorType::Pointer MinMaxCalculator = 
    MinMaxCalculatorType::New();
  MinMaxCalculator->SetImage(image);
  MinMaxCalculator->Compute();

  ChannelPixelType max = MinMaxCalculator->GetMaximum();
  ChannelPixelType min = MinMaxCalculator->GetMinimum();

  double factor = 
    255.0/(static_cast<double>(max)-static_cast<double>(min));

 
  itk::ImageLinearConstIteratorWithIndex< ChannelImageType > 
                                        it( image, m_ImageRegion );

  const int bytesPerPixel = imageViewer->GetNumberOfBytesPerPixel();

  it.SetDirection( 0 );

  const unsigned int totalSize  =
    m_ImageSize[0] * m_ImageSize[1] * bytesPerPixel;

  const unsigned int totalWidth = m_ImageSize[0] * bytesPerPixel;

  fltk::RGBImage2DViewerWindow::ValueType * buffer = imageViewer->GetBuffer();

//    if (!m_FlipY)
//      {
      unsigned char * dest = buffer + totalSize + channel - totalWidth;
      it.GoToBegin();
      while( !it.IsAtEnd() )  // Should only have one slice...but anyway.
        {
          while( !it.IsAtEndOfLine() ) 
            {
              const double value = ( it.Get() - min ) * factor;
              const unsigned char valuec = static_cast<unsigned char>( value );
              *dest = valuec;
              dest += 3 ;
              ++it;
            }
          it.NextLine();
          dest -= 2 * totalWidth;
        }
//      }
//    else
//      {
//        unsigned char * dest = buffer + channel ;
//        it.GoToBegin();
//        while( !it.IsAtEnd() )  // Should only have one slice...but anyway.
//          {
//            while( !it.IsAtEndOfLine() ) 
//              {
//                const double value = ( it.Get() - min ) * factor;
//                const unsigned char valuec = static_cast<unsigned char>( value );
//                *dest = valuec;
//                dest += 3 ;
//                ++it;
//              }
//            it.NextLine();
//          }
//      }
}

template <class ImagePixelType>
void
RGBImage2DViewer<ImagePixelType>
::FillChannel(int channel, ChannelPixelType value, int sizeX, int sizeY)
{
  if (imageViewer->GetBuffer() == 0)
    {
      this->externalWindow->size(sizeX, sizeY);
      imageViewer->Allocate( sizeX, sizeY );
    }

  ChannelPixelType max = itk::NumericTraits< ChannelPixelType >::max() ;
  ChannelPixelType min = itk::NumericTraits< ChannelPixelType >::min() ;

  double factor = 
    255.0/(static_cast<double>(max)-static_cast<double>(min));
  const int bytesPerPixel = imageViewer->GetNumberOfBytesPerPixel();
  const unsigned int totalSize = sizeX * sizeY * bytesPerPixel;
  const unsigned int totalWidth = sizeX * bytesPerPixel;

  fltk::RGBImage2DViewerWindow::ValueType * buffer = imageViewer->GetBuffer();
  const unsigned char valuec = static_cast<unsigned char>( value * factor );
  for (unsigned int index = totalSize + channel - totalWidth;
       index < totalSize ; index++)
    {
      buffer[index] = valuec;
      index += 3 ;
    }
}

template <class ImagePixelType>
void
RGBImage2DViewer<ImagePixelType>
::SetRedChannel(ChannelImageType* image) 
{
  m_RedImage = image ;
  FillChannel(0, image) ;
}

template <class ImagePixelType>
void
RGBImage2DViewer<ImagePixelType>
::SetGreenChannel(ChannelImageType* image)
{
  m_GreenImage = image ;
  FillChannel(1, image) ;
}

template <class ImagePixelType>
void
RGBImage2DViewer<ImagePixelType>
::SetBlueChannel(ChannelImageType* image)
{
  m_BlueImage = image ;
  FillChannel(2, image) ;
}

template <class ImagePixelType>
void
RGBImage2DViewer<ImagePixelType>
::Update(void) 
{
  imageViewer->redraw();
  Fl::check();
}


template <class ImagePixelType>
void
RGBImage2DViewer<ImagePixelType>
::UpdateOverlay(void)
{ 
  if( !m_Overlay )
  {
    return;
  }

  OverlayType::RegionType region  = m_Overlay->GetRequestedRegion();
  OverlayType::SizeType   size    = region.GetSize();  

  imageViewer->AllocateOverlay();

  unsigned char * overlay = imageViewer->GetOverlay();
      
  itk::ImageLinearConstIteratorWithIndex< OverlayType > 
                                         it( m_Overlay,region );

  it.SetDirection( 0 );
  it.GoToBegin();

  const unsigned int totalSize = size[0]*size[1]*4;
  const unsigned int totalWidth = size[0]*4;


  

  typedef itk::MinimumMaximumImageCalculator<OverlayType> MinMaxCalculatorType;
  MinMaxCalculatorType::Pointer MinMaxCalculator =  MinMaxCalculatorType::New();
  MinMaxCalculator->SetImage(m_Overlay);
  MinMaxCalculator->Compute();

  typename OverlayType::PixelType max = MinMaxCalculator->GetMaximum();
  typename OverlayType::PixelType min = MinMaxCalculator->GetMinimum();

  if(!m_FlipY)
    {
    unsigned char * dest = overlay + totalSize - totalWidth;
    while( !it.IsAtEnd() )
      {
      while( !it.IsAtEndOfLine() ) 
        {
        OverlayType::PixelType value = it.Get();
        if((max-min != 0.0))
          {
            value = (unsigned char) (value - min)/(max-min);
          }
        *dest++ = static_cast<unsigned char>(m_ColorTable->GetColorComponent(value-1, 'r')*255);
        *dest++ = static_cast<unsigned char>(m_ColorTable->GetColorComponent(value-1, 'g')*255);
        *dest++ = static_cast<unsigned char>(m_ColorTable->GetColorComponent(value-1, 'b')*255);
        *dest++ = static_cast<unsigned char>(255*m_OverlayOpacity);
        ++it;
        }
      it.NextLine();
      dest -= 2 * totalWidth;
      }
    }
  else
    {
    unsigned char * dest = overlay;
    while( !it.IsAtEnd() )
      {
      while( !it.IsAtEndOfLine() ) 
        {
        OverlayType::PixelType value = it.Get();
        if((max-min != 0.0))
          {
            value = (unsigned char) (value - min)/(max-min);
          }
        *dest++ = static_cast<unsigned char>(m_ColorTable->color(value-1)->GetRed()*255);
        *dest++ = static_cast<unsigned char>(m_ColorTable->color(value-1)->GetGreen()*255);
        *dest++ = static_cast<unsigned char>(m_ColorTable->color(value-1)->GetBlue()*255);
        *dest++ = static_cast<unsigned char>(255*m_OverlayOpacity);
        ++it;
        }
      it.NextLine();
      }


    }


  imageViewer->redraw();
  Fl::check();
}



} // end namespace itk
