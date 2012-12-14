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

#include "mvdGLImageWidget.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "itkImageRegionConstIteratorWithIndex.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"

namespace mvd
{

GLImageWidget::GLImageWidget(QWidget *parent)
: QGLWidget(parent),
  m_IsotropicZoom(1.0),
  m_OpenGlBuffer(NULL),
  m_OpenGlBufferedRegion(),
  m_Extent()
/*,  m_ImageToScreenTransform(AffineTransformType::New()),
  m_ScreenToImageTransform(AffineTransformType::New()) */
{
  m_ImageViewManipulator = new ImageViewEventHandlerType(parent);
}

GLImageWidget::~GLImageWidget()
{
  this->ClearBuffer();
}

void GLImageWidget::ReadBuffer(const ImageType * image, const RegionType& region)
{
  // Before doing anything, check if region is inside the buffered
  // region of image
  if (!image->GetBufferedRegion().IsInside(region))
    {
    //itkExceptionMacro(<< "Region to read is oustside of the buffered region.");
    }
  // Delete previous buffer if needed
  this->ClearBuffer();

  // Allocate new memory
  m_OpenGlBuffer = new unsigned char[3 * region.GetNumberOfPixels()];

  // Declare the iterator
  itk::ImageRegionConstIteratorWithIndex<ImageType> it(image, region);

  // Go to begin
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    // Fill the buffer
    unsigned int index = 0;
    index = ComputeXAxisFlippedBufferIndex(it.GetIndex(), region);

    // Fill the buffer
    m_OpenGlBuffer[index]  = it.Get()[0];
    m_OpenGlBuffer[index + 1] = it.Get()[1];
    m_OpenGlBuffer[index + 2] = it.Get()[2];
    ++it;
    }
  
  // Last, updating buffer size
  m_OpenGlBufferedRegion = region;
}

void GLImageWidget::ClearBuffer()
{
  // Delete previous buffer if needed
  if (m_OpenGlBuffer != NULL)
    {
    delete[] m_OpenGlBuffer;
    m_OpenGlBuffer = NULL;
    }

  // Last, updating buffer size
  m_OpenGlBufferedRegion = RegionType();
}

/*
void GLImageWidget::UpdateTransforms(int w, int h)
{

  if (m_IsotropicZoom <= 0)
    {
    itkExceptionMacro(<< "Internal error: Isotropic zoom should be non null positive.");
    }

  RegionType::IndexType index;
  RegionType::SizeType  size;
  // Update image extent
  std::cout << " size1 " << size[1] << std::endl;
  size[0] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[0]));
  size[1] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[1]));
  index[0] = m_OpenGlBufferedRegion.GetIndex()[0];
  index[1] = m_OpenGlBufferedRegion.GetIndex()[1];
  std::cout <<"GLWidget x "<< this->x() << " y "<< this->y() << std::endl;
  m_Extent.SetIndex(index) ;
  m_Extent.SetSize(size);

  std::cout <<"m_Extent : "<< m_Extent << std::endl;

  // Image to screen matrix
  AffineTransformType::MatrixType s2iMatrix;
  s2iMatrix.Fill(0);
  const double s2iSpacing = (m_SubsamplingRate) / m_IsotropicZoom;
  s2iMatrix(0, 0) = s2iSpacing;
  s2iMatrix(1, 1) = -s2iSpacing;
  m_ScreenToImageTransform->SetMatrix(s2iMatrix);

  // Image to screen translation
  AffineTransformType::OutputVectorType translation;
  translation[0] = m_SubsamplingRate *
    (m_OpenGlBufferedRegion.GetIndex()[0] - 
     m_Extent.GetIndex()[0] / m_IsotropicZoom);
  translation[1] = m_SubsamplingRate *
    (((m_Extent.GetIndex()[1] +
       m_Extent.GetSize()[1]) / m_IsotropicZoom) + m_OpenGlBufferedRegion.GetIndex()[1]);
  m_ScreenToImageTransform->SetTranslation(translation) ;

  // Compute the inverse transform
  bool couldInvert = m_ScreenToImageTransform->GetInverse(m_ImageToScreenTransform);
  if (couldInvert == false)
    {
    itkExceptionMacro(<< "Internal error: Could not invert ScreenToImageTransform.");
    }
}
*/

void GLImageWidget::initializeGL()
 {
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
 }

 void GLImageWidget::resizeGL(int w, int h)
 {
   RegionType::SizeType size;
   size [0] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[0]));
   size [1] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[1]));

   RegionType::IndexType index;
   index[0] = (w - static_cast<int>(size[0])) / 2;
   index[1] = (h - static_cast<int>(size[1])) / 2;

   m_Extent.SetIndex(index);
   m_Extent.SetSize(size);

   m_W = (GLint)w;
   m_H =  (GLint)h;

  glViewport(0, 0, m_W, m_H);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, m_W, 0, m_H, -1, 1);

}

void GLImageWidget::paintGL()
{
  // Get the region to draw from the ImageViewManipulator navigation
  // context 
  const RegionType & region = m_ImageViewManipulator->GetNavigationContext().bufferedRegion;

  ImageModelRenderer::RenderingContext context(
    dynamic_cast< const AbstractImageModel* >(
      dynamic_cast< Application* >( qApp )->GetModel()
    ), region
  );

  std::cout <<"GLImageWidget::paintGL " << std::endl;
  // use the model renderer to paint the requested region of the image
  m_ImageModelRender->paintGL( context );
}

// Delegate the event to the ImageViewManipulator
void GLImageWidget::mousePressEvent(  QMouseEvent * event)
{
  
  m_ImageViewManipulator->mousePressEvent(event);
}

void GLImageWidget::mouseMoveEvent(  QMouseEvent * event)
{
  m_ImageViewManipulator->mouseMoveEvent(event);
}

void GLImageWidget::resizeEvent( QResizeEvent  * event)
{
  m_ImageViewManipulator->resizeEvent(event);
}

}
