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
//#include <QKeyEvent>
//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkImageRegionConstIteratorWithIndex.h"

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractImageModel.h"
#include "mvdApplication.h"
#include "mvdDatasetModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::GLImageWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
GLImageWidget
::GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 QWidget* parent,
		 const QGLWidget* shareWidget,
		 Qt::WindowFlags flags ) :
  QGLWidget( parent, shareWidget, flags ),
  m_ImageViewManipulator( NULL ),
  m_ImageModelRenderer( NULL ),
  m_ImageModel( NULL )
{
  Initialize(manipulator, renderer);
}

/*******************************************************************************/
GLImageWidget
::GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 QGLContext* context,
		 QWidget* parent,
		 const QGLWidget* shareWidget,
		 Qt::WindowFlags flags ) :
  QGLWidget( context, parent, shareWidget, flags ),
  m_ImageViewManipulator( NULL ),
  m_ImageModelRenderer( NULL ),
  m_ImageModel( NULL )
{
  Initialize(manipulator, renderer);
}

/*******************************************************************************/
GLImageWidget
::GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 const QGLFormat& format,
                 QWidget* parent,
                 const QGLWidget* shareWidget,
                 Qt::WindowFlags flags ) :
  QGLWidget( format, parent, shareWidget, flags ),
  m_ImageViewManipulator( NULL ),
  m_ImageModelRenderer( NULL ),
  m_ImageModel( NULL )
{
  Initialize(manipulator, renderer);
}

/*******************************************************************************/
GLImageWidget
::~GLImageWidget()
{
  // m_ImageViewManipulator (deleted as a child of a QObjet parent).
  // m_ImageModelRenderer (deleted as a child of a QObjet parent).
}

/*******************************************************************************/
void
GLImageWidget
::Initialize( AbstractViewManipulator* manipulator,
	      AbstractModelRenderer* renderer )
{
  assert( manipulator!=NULL );
  assert( renderer!=NULL );

  m_ImageViewManipulator = manipulator;
  m_ImageViewManipulator->setParent(this);

  m_ImageModelRenderer = renderer;
  m_ImageModelRenderer->setParent(this);

  QObject::connect(
    this, SIGNAL( movingMouse() ),
    m_ImageModelRenderer, SLOT( onMovingEvent() )
  );

  QObject::connect(
    this, SIGNAL( releasingMouse() ),
    m_ImageModelRenderer, SLOT(onReleasedMouse())
  );

  // Connect this model region changed.
  QObject::connect(
    this,
    SIGNAL( ModelImageRegionChanged( const ImageRegionType& , const SpacingType&) ),
    // to:
    m_ImageViewManipulator,
    SLOT( OnModelImageRegionChanged( const ImageRegionType&, const SpacingType& ) )
  );

  // Connect the renderer origin (of extent) changed to the manipulator
  QObject::connect(
    m_ImageModelRenderer,
    SIGNAL( ViewportOriginChanged( const IndexType& ) ),
    // to:
    m_ImageViewManipulator,
    SLOT( OnViewportOriginChanged( const IndexType&) )
  );

}

/*******************************************************************************/
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

/*******************************************************************************/
void
GLImageWidget
::initializeGL()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

/*******************************************************************************/
void
GLImageWidget
::resizeGL(int width, int height)
{
  // TODO: Replace (GLint) casts with safer casts or no cast (if there is no compile-time warning).
  glViewport(0, 0, (GLint)width, (GLint)height);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, (GLint)width, 0, (GLint)height, -1, 1);
}

/*******************************************************************************/
void
GLImageWidget
::paintGL()
{
  // Clear back-buffer(s) before rendering sub-components.
  glClear( GL_COLOR_BUFFER_BIT );

  // Get the region to draw from the ImageViewManipulator navigation
  // context.
  const ImageRegionType region(
    m_ImageViewManipulator->GetViewportImageRegion() );

  // Get the zoom 
  const double isotropicZoom = m_ImageViewManipulator->GetIsotropicZoom();

  // Setup rendering context with image-model and redering information.
  AbstractModelRenderer::RenderingContext context(
    m_ImageModel,
    region, isotropicZoom, 
    width(), height(),
    m_ImageViewManipulator->HasZoomChanged()
  );

    // use the model renderer to paint the requested region of the image.
  m_ImageModelRenderer->paintGL( context );
}

/*******************************************************************************/
// Delegate the event to the ImageViewManipulator
void
GLImageWidget
::mousePressEvent( QMouseEvent* event )
{
  QCursor dragCursor;
  dragCursor.setShape(Qt::ClosedHandCursor) ;
  this->setCursor(dragCursor);

  m_ImageViewManipulator->mousePressEvent(event);
  this->update();
}

/*******************************************************************************/
void
GLImageWidget
::mouseMoveEvent( QMouseEvent* event )
{
  // emit a signal
  emit movingMouse();

  // handle this event
  m_ImageViewManipulator->mouseMoveEvent(event);

  // repaint the buffer
  this->update();
}

/*******************************************************************************/
void
GLImageWidget
::mouseReleaseEvent( QMouseEvent* event )
{
  QCursor stdCursor;
  stdCursor.setShape(Qt::ArrowCursor) ;
  this->setCursor(stdCursor);

  // emit a signal
  emit releasingMouse();

  // call paintGL
  this->update();
}

/*******************************************************************************/
void
GLImageWidget
::wheelEvent( QWheelEvent* event )
{
  // emit a signal
  emit releasingMouse();

  m_ImageViewManipulator->wheelEvent(event);

  // repaint the buffer
  this->update();
}

/*******************************************************************************/
void
GLImageWidget
::resizeEvent( QResizeEvent* event )
{
  m_ImageViewManipulator->resizeEvent(event);

  // emit a signal
  emit releasingMouse();

  this->resizeGL(event->size().width(), event->size().height());
  this->update();
}

/*******************************************************************************/
void
GLImageWidget
::keyPressEvent( QKeyEvent* event )
{
  m_ImageViewManipulator->keyPressEvent(event);
  this->update();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/******************************************************************************/
void
GLImageWidget
::OnSpacingChanged(const SpacingType& spacing)
{
  m_ImageViewManipulator->SetSpacing(spacing);
}
/*******************************************************************************/

}
