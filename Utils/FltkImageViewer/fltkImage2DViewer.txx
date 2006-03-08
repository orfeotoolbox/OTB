/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkImage2DViewer.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/27 14:52:00 $
  Version:   $Revision: 1.18 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "fltkImage2DViewer.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkMinimumMaximumImageCalculator.h"

namespace fltk {
  
template <class ImagePixelType>
Image2DViewer<ImagePixelType>
::Image2DViewer()
{

  imageViewer->SetIntensityWindow( intensityWindow );
  imageViewer->SetParentWindow( externalWindow );
  
  m_RedrawCommand = ObserverCommandType::New();
  m_RedrawCommand->SetCallbackFunction( this, &Self::Update );
  
  m_Tag = 0L;

  m_FlipY = false ;
}


  
template <class ImagePixelType>
Image2DViewer<ImagePixelType>
::~Image2DViewer()
{
}


  
template <class ImagePixelType>
itk::Object *
Image2DViewer<ImagePixelType>
::GetNotifier(void)
{
  return imageViewer->GetNotifier();
}




template <class ImagePixelType>
void
Image2DViewer<ImagePixelType>
::SetImage(const ImageType * image)
{
 
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

  typedef itk::MinimumMaximumImageCalculator<ImageType> MinMaxCalculatorType;
  typename MinMaxCalculatorType::Pointer MinMaxCalculator =  MinMaxCalculatorType::New();
  MinMaxCalculator->SetImage(m_Image);
  MinMaxCalculator->Compute();

  typename ImageType::PixelType max = MinMaxCalculator->GetMaximum();
  typename ImageType::PixelType min = MinMaxCalculator->GetMinimum();

  minimumSlider->range( min, max );
  maximumSlider->range( min, max );

  minimumSlider->value( min );
  maximumSlider->value( max );

  Update();

}





template <class ImagePixelType>
void
Image2DViewer<ImagePixelType>
::Update(void) 
{

  if( !m_Image )
    {
    return;
    }

  typename ImageType::RegionType region  = m_Image->GetRequestedRegion();
  typename ImageType::SizeType   size    = region.GetSize();  

  if(    size[0] != static_cast<unsigned int>(imageViewer->GetWidth())
      || size[1] != static_cast<unsigned int>(imageViewer->GetHeight())  )
  {
    this->externalWindow->size(size[0],size[1]);
    imageViewer->Allocate( size[0], size[1] );
  }

  typedef itk::MinimumMaximumImageCalculator<ImageType> MinMaxCalculatorType;
  typename MinMaxCalculatorType::Pointer MinMaxCalculator =  MinMaxCalculatorType::New();
  MinMaxCalculator->SetImage(m_Image);
  MinMaxCalculator->Compute();

  typename ImageType::PixelType max = MinMaxCalculator->GetMaximum();
  typename ImageType::PixelType min = MinMaxCalculator->GetMinimum();

  minimumSlider->range(min,max);
  maximumSlider->range(min,max);

  const double selectedMin = minimumSlider->value();
  const double selectedMax = maximumSlider->value();


  this->RenderImage( selectedMin, selectedMax );

}




 
template <class ImagePixelType>
void
Image2DViewer<ImagePixelType>
::RenderImage( double selectedMin, double selectedMax )
{

  double factor = 255.0 / (selectedMax - selectedMin);

  itk::ImageLinearConstIteratorWithIndex< ImageType > 
                it( m_Image, m_Image->GetRequestedRegion() );

  typename ImageType::SizeType   size =
              m_Image->GetRequestedRegion().GetSize();  

  const int bytesPerPixel = imageViewer->GetNumberOfBytesPerPixel();

  it.SetDirection( 0 );

  const unsigned int totalSize  = size[0] * size[1] * bytesPerPixel;
  const unsigned int totalWidth = size[0] * bytesPerPixel;

  fltk::Image2DViewerWindow::ValueType * buffer = imageViewer->GetBuffer();

  if(!m_FlipY)
   {
    unsigned char * dest = buffer + totalSize - totalWidth;
    it.GoToBegin();
    while( !it.IsAtEnd() )  
      {
        while( !it.IsAtEndOfLine() ) 
          {
            double ivalue = it.Get();
            if( ivalue > selectedMax )
              {
              ivalue = selectedMax;
              }
            else if( ivalue < selectedMin )
              {
              ivalue = selectedMin;
              }
            const double value = ( ivalue - selectedMin ) * factor;
            const unsigned char valuec = static_cast<unsigned char>( value );
            *dest++ = valuec;
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
    while( !it.IsAtEnd() )  
      {
        while( !it.IsAtEndOfLine() ) 
          {
            double ivalue = it.Get();
            if( ivalue > selectedMax )
              {
              ivalue = selectedMax;
              }
            else if( ivalue < selectedMin )
              {
              ivalue = selectedMin;
              }
            const double value = ( ivalue - selectedMin ) * factor;
            const unsigned char valuec = static_cast<unsigned char>( value );
            *dest++ = valuec;
            ++it;
          }
        it.NextLine();
      }
    }

  imageViewer->redraw();
  Fl::check();

}




template <class ImagePixelType>
void
Image2DViewer<ImagePixelType>
::SetClickCallBack(void* ptrObject,
                   void (*clickCallBack)(void* ptrObject,
                                      int & px, int & py ))
{
  imageViewer->SetClickCallBack( ptrObject, clickCallBack );
}



template <class ImagePixelType>
void
Image2DViewer<ImagePixelType>
::SetSelectionCallBack(void* ptrObject,
                   void (*selectionCallBack)(void* ptrObject,
                   fltk::Image2DViewerWindow::SelectionBox*))
{
  imageViewer->SetSelectionCallBack( ptrObject, selectionCallBack );
}



template <class ImagePixelType>
void
Image2DViewer<ImagePixelType>
::SetInteractionMode( Image2DViewerWindow::InteractionModeType mode )
{
  imageViewer->SetInteractionMode( mode );
}




} // end namespace itk
