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
#ifndef __otbImageWidgetBase_txx
#define __otbImageWidgetBase_txx

#include "otbConfigure.h"
#include "itkImageRegionConstIteratorWithIndex.h"

#include "otbMacro.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TPixel>
ImageWidgetBase<TPixel>
::ImageWidgetBase()
  : Fl_Gl_Window(0, 0, 0, 0, 0)
{
  m_Image = NULL;
  m_OpenGlIsotropicZoom = 1.0;
  SizeType size;
  size.Fill(0);
  IndexType index;
  index.Fill(0);
  RegionType region;
  region.SetSize(size);
  region.SetIndex(index);
  m_BufferedRegion = region;
  m_ViewedRegion = region;
  m_ViewModel = RGB;
  m_RedChannelIndex = 0;
  m_GreenChannelIndex = 1;
  m_BlueChannelIndex = 2;
  m_OpenGlBuffer = NULL;
  m_FormOverlayVisible = false;
  m_ImageOverlayVisible = false;
  m_BlackTransparency = false;
  m_FormList = FormListType::New();
  m_ImageOverlay = NULL;
  m_OpenGlImageOverlayBuffer = NULL;
  m_ImageOverlayOpacity = 128;
  m_TransferFunctionList = TransferFunctionListType::New();
  m_TransferFunctionList->PushBack(AffineTransferFunctionType::New());
  m_SubSamplingRate = 1;
}
/**
 * Destructor.
 */
template <class TPixel>
ImageWidgetBase<TPixel>
::~ImageWidgetBase()
{
  if (m_OpenGlBuffer != NULL)
    {
    delete[] m_OpenGlBuffer;
    }
  if (m_OpenGlImageOverlayBuffer != NULL)
    {
    delete[] m_OpenGlImageOverlayBuffer;
    }
}
/**
 * Reset the widget.
 */
template <class TPixel>
void
ImageWidgetBase<TPixel>
::Reset(void)
{
  int          x = this->x();
  int          y = this->y();
  int          w = this->w();
  int          h = this->h();
  const char * label = this->label();
  ClearBufferedRegion();
  Init(x, y, w, h, label);
}

template <class TPixel>
void
ImageWidgetBase<TPixel>
::ClearBufferedRegion(void)
{
  SizeType size;
  size.Fill(0);
  m_BufferedRegion.SetSize(size);
}

template <class TPixel>
void
ImageWidgetBase<TPixel>
::Init(int /*x*/, int /*y*/, int /*w*/, int /*h*/, const char * /*l*/)
{
  for (unsigned int i = 0; i < m_Image->GetNumberOfComponentsPerPixel(); ++i)
    {
    if (i >= m_TransferFunctionList->Size())
      {
      m_TransferFunctionList->PushBack(AffineTransferFunctionType::New());
      }
    }

}

/**
 * Convert window to image coordinate.
 * \param index The window coordinate.
 * \return The image coordinate.
 */
template <class TPixel>
typename ImageWidgetBase<TPixel>
::IndexType
ImageWidgetBase<TPixel>
::WindowToImageCoordinates(IndexType index)
{
  double    x, y;
  IndexType resp;
  IndexType origin = this->GetViewedRegion().GetIndex();
  x = static_cast<double>(index[0]) / m_OpenGlIsotropicZoom
      + static_cast<double>(origin[0]);
  y = static_cast<double>(origin[1])
      + static_cast<double>(index[1]) / m_OpenGlIsotropicZoom;

  SizeType  maxSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
  IndexType maxIndex = this->GetInput()->GetLargestPossibleRegion().GetIndex();

  if (x < maxIndex[0])
    {
    resp[0] = maxIndex[0];
    }
  else if (x > maxIndex[0] + maxSize[0])
    {
    resp[0] = maxIndex[0] + maxSize[0];
    }
  else
    {
    resp[0] = static_cast<int>(x);
    }

  if (y < maxIndex[1])
    {
    resp[1] = maxIndex[1];

    }
  else if (y > maxIndex[1] + maxSize[1])
    {
    resp[1] = maxIndex[1] + maxSize[1];
    }
  else
    {
    resp[1] = static_cast<int>(y);
    }
  return resp;
}

/** Set the input image.
 * \param image The image to view.
 */
template <class TPixel>
void
ImageWidgetBase<TPixel>
::SetInput(ImageType * image)
{
  m_Image = image;
  m_Image->UpdateOutputInformation();
  SizeType size;
  size.Fill(0);
  m_BufferedRegion.SetSize(size);
  SizeType displaySize;
  displaySize[0] = this->w();
  displaySize[1] = this->h();
  m_ViewedRegion.SetSize(displaySize);
  m_ViewedRegion.Crop(m_Image->GetLargestPossibleRegion());
}
/** Get the input image.
 * \return The image to view.
 */
template <class TPixel>
typename ImageWidgetBase<TPixel>
::ImageType *
ImageWidgetBase<TPixel>
::GetInput(void)
{
  return m_Image;
}

/** Set the input overlay image.
 * \param image The image to view.
 */
template <class TPixel>
void
ImageWidgetBase<TPixel>
::SetInputOverlay(OverlayImageType * image)
{
  m_ImageOverlay = image;
}

/** Set the input overlay form list.
 * \param formList The form list to view.
 */
template <class TPixel>
void
ImageWidgetBase<TPixel>
::SetFormListOverlay(FormListType* formList)
{
  m_FormList = formList;
}

/** Set the transfer function list
 *  \param list The transfer function list.
 */
template <class TPixel>
void
ImageWidgetBase<TPixel>
::SetTransferFunctionList(TransferFunctionListType * list)
{
  m_TransferFunctionList = list;
}

/** Get the input overlay image.
 * \return The image to view.
 */
template <class TPixel>
typename ImageWidgetBase<TPixel>
::OverlayImageType *
ImageWidgetBase<TPixel>
::GetInputOverlay(void)
{
  return m_ImageOverlay;
}

/**
 * Show The widget.
 */
template <class TPixel>
void
ImageWidgetBase<TPixel>
::Show(void)
{
  if (!m_Image)
    {
    itkExceptionMacro(<< "No input image !");
    }
  else
    {
    //otbMsgDebugMacro(<<"Zoomable widget Show");
    this->show();
    //otbMsgDebugMacro(<<"Before redraw.");
    this->redraw();
    //otbMsgDebugMacro(<<"After redraw.");
    }
}

template <class TPixel>
unsigned char
ImageWidgetBase<TPixel>
::Normalize(PixelType value, unsigned int channelIndex)
{
  return m_TransferFunctionList->GetNthElement(channelIndex)->Map(value);
}

/**
 * Draw the widget
 */
template <class TPixel>
void
ImageWidgetBase<TPixel>
::draw(void)
{

  //   otbMsgDebugMacro(<<"Draw");

  if (this->UpdateOpenGlBufferedRegionRequested())
    {
    UpdateOpenGlBufferedRegion();
    m_Image->SetRequestedRegion(m_BufferedRegion);
    m_Image->PropagateRequestedRegion();
    m_Image->UpdateOutputData();
    RebuildOpenGlBuffer();
    if (m_ImageOverlayVisible)
      {
      m_ImageOverlay->SetRequestedRegion(m_BufferedRegion);
      m_ImageOverlay->PropagateRequestedRegion();
      m_ImageOverlay->UpdateOutputData();
      }
    }

  if (m_ImageOverlayVisible)
    {
    RebuildOpenGlImageOverlayBuffer();
    }

  if (!this->valid())
    {
    valid(1);
    glLoadIdentity();
    glViewport(0, 0, w(), h());
    glClearColor((float) 0.0, (float) 0.0, (float) 0.0, (float) 0.0);
    glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

  glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
  glMatrixMode(GL_MODELVIEW);    //clear previous 3D draw params
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  this->ortho();
  glDisable(GL_BLEND);

  // To be consistent with old method
  int displayHeight = static_cast<int>(vcl_ceil(m_BufferedRegion.GetSize()[1] * m_OpenGlIsotropicZoom));
  int viewPortOffset = 0;
  viewPortOffset = h() - displayHeight;
  glRasterPos2i(0, viewPortOffset);

  glPixelZoom(m_OpenGlIsotropicZoom, m_OpenGlIsotropicZoom);

  // display the image
  glDrawPixels(m_BufferedRegion.GetSize()[0],
               m_BufferedRegion.GetSize()[1],
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               m_OpenGlBuffer);


#if 0
/* This code use Texture to draw raster (OTB_GL_USE_ACCEL) */

  glEnable(GL_TEXTURE_2D);
  glColor4f(1.0, 1.0, 1.0, 0.0);
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, m_BufferedRegion.GetSize()[0],
               m_BufferedRegion.GetSize()[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, m_OpenGlBuffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // Nearest Filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // Nearest Filtering
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // Linear Filtering
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // Linear Filtering

  glBindTexture (GL_TEXTURE_2D, texture);
  glBegin (GL_QUADS);
  int hOffset = this->h() - this->hDisplayed();
  glTexCoord2f (0.0, 1.0);
  glVertex3f (0.0, 0.0 + hOffset, 0.0);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (this->wDisplayed(), 0.0 + hOffset, 0.0);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (this->wDisplayed(), this->hDisplayed() + hOffset, 0.0);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (0.0, this->hDisplayed() + hOffset, 0.0);
  glEnd ();

  glDisable(GL_TEXTURE_2D);
#endif

  // if image overlay is activated, display image overlay
  if (m_ImageOverlayVisible)
    {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawPixels(m_BufferedRegion.GetSize()[0],
                 m_BufferedRegion.GetSize()[1],
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 m_OpenGlImageOverlayBuffer);

    glDisable(GL_BLEND);
    glEnd();

#if 0
    /* This code use Texture to draw raster (OTB_GL_USE_ACCEL) */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    GLuint textureOverlay;
    glGenTextures(1, &textureOverlay);
    glBindTexture(GL_TEXTURE_2D, textureOverlay);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, m_BufferedRegion.GetSize()[0],
                 m_BufferedRegion.GetSize()[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, m_OpenGlImageOverlayBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // Nearest Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // Nearest Filtering

    glBindTexture (GL_TEXTURE_2D, textureOverlay);
    glBegin (GL_QUADS);
    int hOffset = this->h() - this->hDisplayed();
    glTexCoord2f (0.0, 1.0);
    glVertex3f (0.0, 0.0 + hOffset, 1.0);
    glTexCoord2f (1.0, 1.0);
    glVertex3f (this->wDisplayed(), 0.0 + hOffset, 1.0);
    glTexCoord2f (1.0, 0.0);
    glVertex3f (this->wDisplayed(), this->hDisplayed() + hOffset, 1.0);
    glTexCoord2f (0.0, 0.0);
    glVertex3f (0.0, this->hDisplayed() + hOffset, 1.0);
    glEnd ();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
#endif

    }

  if (m_FormOverlayVisible)
    {
    ReverseIteratorType it =  m_FormList->ReverseBegin();
    //otbMsgDebugMacro(<<"Formlist size: "<<m_FormList->Size());
    for (; it != m_FormList->ReverseEnd(); ++it)
      {
      it.Get()->Draw(m_OpenGlIsotropicZoom,
                     m_ViewedRegion.GetIndex()[0],
                     m_ViewedRegion.GetIndex()[1],
                     this->h(), m_SubSamplingRate);
      }
    }

}

/**
 * Rebuild opengl buffer.
 */
template <class TPixel>
void
ImageWidgetBase<TPixel>
::RebuildOpenGlBuffer(void)
{
  // Channel indices outofbound check
  if (m_RedChannelIndex >= m_Image->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(<< "Red channel index out of bound.");
    }
  if (m_ViewModel != GRAYSCALE && m_GreenChannelIndex >= m_Image->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(<< "Green channel index out of bound.");
    }
  if (m_ViewModel == RGB && m_BlueChannelIndex >= m_Image->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(<< "Blue channel index out of bound.");
    }

  //otbMsgDebugMacro(<<"RebuildOpenGlBuffer");
  if (m_OpenGlBuffer != NULL)
    {
    //otbMsgDebugMacro(<<"Deleting previous buffer ...");
    delete[] m_OpenGlBuffer;
    }
  //otbMsgDebugMacro(<<"Buffered region: "<<m_BufferedRegion);
  unsigned int bufferLength = 4 * m_BufferedRegion.GetNumberOfPixels();
  //otbMsgDebugMacro(<<"New buffer length: "<<bufferLength);
  m_OpenGlBuffer = new unsigned char[bufferLength];

  typedef itk::ImageRegionConstIteratorWithIndex<ImageType> IteratorType;
  IteratorType it(m_Image, m_BufferedRegion);
  unsigned int index = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    index = GetRevertedBufferIndex(it.GetIndex());
#if 0
    /* OTB_GL_USE_ACCEL */
    index = GetBufferIndex(it.GetIndex());
#endif

    switch (m_ViewModel)
      {
      case RGB:
        {
        m_OpenGlBuffer[index] =   Normalize(it.Get()[m_RedChannelIndex], m_RedChannelIndex);
        m_OpenGlBuffer[index + 1] = Normalize(it.Get()[m_GreenChannelIndex], m_GreenChannelIndex);
        m_OpenGlBuffer[index + 2] = Normalize(it.Get()[m_BlueChannelIndex], m_BlueChannelIndex);
        m_OpenGlBuffer[index + 3] = 255;
        break;
        }
      case GRAYSCALE:
        {
        unsigned char gray = Normalize(it.Get()[m_RedChannelIndex], m_RedChannelIndex);
        m_OpenGlBuffer[index] =   gray;
        m_OpenGlBuffer[index + 1] = gray;
        m_OpenGlBuffer[index + 2] = gray;
        m_OpenGlBuffer[index + 3] = 255;
        break;
        }
      case COMPLEX_MODULUS:
        {
        double re = static_cast<double>(it.Get()[m_RedChannelIndex]);
        double im = static_cast<double>(it.Get()[m_GreenChannelIndex]);
        //     std::cout << " *** " << m_RedChannelIndex << " " << m_GreenChannelIndex << std::endl;
        unsigned char modulus = Normalize(static_cast<PixelType>(vcl_sqrt(static_cast<double>(im * im + re * re))), 0);
        //     std::cout << " ** " << im << " " << re << " -> " << static_cast<int>(modulus) << std::endl;
        m_OpenGlBuffer[index] =   modulus;
        m_OpenGlBuffer[index + 1] = modulus;
        m_OpenGlBuffer[index + 2] = modulus;
        m_OpenGlBuffer[index + 3] = 255;
        break;
        }
      case COMPLEX_PHASE:
        {
        unsigned char phase =
          Normalize(static_cast<PixelType>(vcl_atan2(static_cast<double>(it.Get()[m_GreenChannelIndex]),
                                                     static_cast<double>(it.Get()[m_RedChannelIndex]))), 0);
        m_OpenGlBuffer[index]   = phase;
        m_OpenGlBuffer[index + 1] = phase;
        m_OpenGlBuffer[index + 2] = phase;
        m_OpenGlBuffer[index + 3] = 255;
        break;
        }
      }
    }
}
/**
 * Rebuild opengl image overlay buffer.
 */
template <class TPixel>
void
ImageWidgetBase<TPixel>
::RebuildOpenGlImageOverlayBuffer(void)
{
  if (m_ImageOverlay->GetNumberOfComponentsPerPixel() < 3)
    {
    itkExceptionMacro(<< "Image overlay should have at least 3 channels.");
    }

  //otbMsgDebugMacro(<<"RebuildOpenGlImageOverlayBuffer");
  if (m_OpenGlImageOverlayBuffer != NULL)
    {
    //otbMsgDebugMacro(<<"Deleting previous buffer ...");
    delete[] m_OpenGlImageOverlayBuffer;
    }
  //otbMsgDebugMacro(<<"Buffered region: "<<m_BufferedRegion);
  unsigned int bufferLength = 4 * m_BufferedRegion.GetNumberOfPixels();
  //otbMsgDebugMacro(<<"New buffer length: "<<bufferLength);
  m_OpenGlImageOverlayBuffer = new unsigned char[bufferLength];

  typedef itk::ImageRegionConstIteratorWithIndex<OverlayImageType> IteratorType;
  IteratorType it(m_ImageOverlay, m_BufferedRegion);
  unsigned int index = 0;
  if (m_BlackTransparency)
    {
    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
      {
      index = GetRevertedBufferIndex(it.GetIndex());
#if 0
      /* OTB_GL_USE_ACCEL */
      index = GetBufferIndex(it.GetIndex());
#endif
      if ((it.Get()[0] == 0) && (it.Get()[1] == 0) && (it.Get()[2] == 0))
        {
        m_OpenGlImageOverlayBuffer[index] = 0;
        m_OpenGlImageOverlayBuffer[index + 1] = 0;
        m_OpenGlImageOverlayBuffer[index + 2] = 0;
        m_OpenGlImageOverlayBuffer[index + 3] = 0;
        }
      else
        {
        m_OpenGlImageOverlayBuffer[index] =   static_cast<unsigned char>(it.Get()[0]);
        m_OpenGlImageOverlayBuffer[index + 1] = static_cast<unsigned char>(it.Get()[1]);
        m_OpenGlImageOverlayBuffer[index + 2] = static_cast<unsigned char>(it.Get()[2]);
        m_OpenGlImageOverlayBuffer[index + 3] = m_ImageOverlayOpacity;
        }
      }
    }
  else
    {
    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
      {
      index = GetRevertedBufferIndex(it.GetIndex());
#if 0
      /* OTB_GL_USE_ACCEL */
      index = GetBufferIndex(it.GetIndex());
#endif
      m_OpenGlImageOverlayBuffer[index] =  static_cast<unsigned char>(it.Get()[0]);
      m_OpenGlImageOverlayBuffer[index + 1] = static_cast<unsigned char>(it.Get()[1]);
      m_OpenGlImageOverlayBuffer[index + 2] = static_cast<unsigned char>(it.Get()[2]);
      m_OpenGlImageOverlayBuffer[index + 3] = m_ImageOverlayOpacity;
      }
    }
}
} // end namespace otb
#endif
