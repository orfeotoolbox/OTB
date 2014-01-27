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

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractImageModel.h"
#include "Core/mvdDatasetModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::GLImageWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
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
  ListGlVersions();
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
  // Test OpenGL.
  ListGlVersions();
  // Accept drops
  setAcceptDrops( true );
  // Set focus policy so that the widget gets the focus if it is clicked
  setMouseTracking( true );
  setFocusPolicy( Qt::StrongFocus );


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
    SIGNAL( ModelImageRegionChanged( const ImageRegionType& , 
                                     const SpacingType&,
                                     const PointType&) ),
    // to:
    m_ImageViewManipulator,
    SLOT( OnModelImageRegionChanged( const ImageRegionType&, 
                                     const SpacingType&,
                                     const PointType&) )
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

  // 
  // emit an event with viewport center and zoom level (will be stored
  // in DatasetModel to be written in Descriptor)
  emit RenderingContextChanged(
    m_ImageViewManipulator->GetViewportPhysicalCenter(), 
    isotropicZoom
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

  //
  m_ImageViewManipulator->mousePressEvent(event);
  this->update();
}

/*******************************************************************************/
void
GLImageWidget
::mouseMoveEvent( QMouseEvent* event )
{
  // if a button is clicked == drag
  if ( event->buttons() & Qt::LeftButton  || 
       event->buttons() & Qt::RightButton ||
       event->buttons() & Qt::MidButton   ||
       event->buttons() & Qt::XButton1    ||
       event->buttons() & Qt::XButton2 )
    {
    // emit a signal movingMouse to update the renderer status
    emit movingMouse();

    // drag detected
    m_ImageViewManipulator->mouseMoveEvent(event);

    // repaint the buffer
    this->update();
    
    // emited to update to force the ql widget (if any) to update
    emit CentralWidgetUpdated();
    }
  else
    {
    // TODO: Out of the if/else ?
    m_ImageViewManipulator->mouseMoveEvent(event);
 
   // update the mouse cursor position if not dragging (just moving
    // the mouse )
    m_ImageViewManipulator->PropagatePointUnderCursorCoordinates(event->pos());
    }
}

/*******************************************************************************/
void
GLImageWidget
::mouseReleaseEvent( QMouseEvent* event )
{
  QCursor stdCursor;
  stdCursor.setShape(Qt::ArrowCursor) ;
  this->setCursor(stdCursor);

  // emit a signal releasingMouse to update the renderer status
  emit releasingMouse();

  // call paintGL
  this->update();

  // emited to update to force the ql widget (if any) to update
  emit CentralWidgetUpdated();

  m_ImageViewManipulator->mouseReleaseEvent(event);
}

/*******************************************************************************/
void
GLImageWidget
::wheelEvent( QWheelEvent* event )
{
  // emit a signal releasingMouse to update the renderer status
  emit releasingMouse();

  m_ImageViewManipulator->wheelEvent(event);

  // repaint the buffer
  this->update();

  // emited to update to force the ql widget (if any) to update
  emit CentralWidgetUpdated();
}

/*******************************************************************************/
void
GLImageWidget
::resizeEvent( QResizeEvent* event )
{
  // First, call superclass implementation
  QGLWidget::resizeEvent(event);

  // emit a signal releasingMouse to update the renderer status
  emit releasingMouse();

  m_ImageViewManipulator->resizeEvent(event);

  // emited to update to force the ql widget (if any) to update
  emit CentralWidgetUpdated();
}

/*******************************************************************************/
void
GLImageWidget
::keyPressEvent( QKeyEvent* event )
{
  m_ImageViewManipulator->keyPressEvent(event);
  this->update();
  
  // emited to update to force the ql widget (if any) to update
  emit CentralWidgetUpdated();
}

/*******************************************************************************/
void
GLImageWidget
::ListGlVersions() const
{
#if QT_VERSION < QT_VERSION_CHECK( 4, 7, 0 )
  qWarning() <<
    tr( "Unable to list suppored OpenGL versions using QGLFormat. "
	"Qt version is %1. Please upgrade to Qt 4.7.0 or newer." )
    .arg( qVersion() )
    .toLatin1()
    .constData();

#else
  qWarning() <<
    tr( "Runtime version of OpenGL used by Qt %1: %2.%3." )
    .arg( qVersion() )
    .arg( format().majorVersion() )
    .arg( format().minorVersion() )
    .toLatin1()
    .constData();

  QGLFormat::OpenGLVersionFlags glVersionFlags(
    QGLFormat::openGLVersionFlags()
  );

  qWarning() <<
    tr( "Version(s) of OpenGL supported by Qt %1:" )
    .arg( qVersion() )
    .toLatin1()
    .constData();

  if( glVersionFlags & QGLFormat::OpenGL_Version_4_0 )
    qWarning() << "QGLFormat::OpenGL_Version_4_0";

  if( glVersionFlags & QGLFormat::OpenGL_Version_3_3 )
    qWarning() << "- QGLFormat::OpenGL_Version_3_3";
  if( glVersionFlags & QGLFormat::OpenGL_Version_3_2 )
    qWarning() << "- QGLFormat::OpenGL_Version_3_2";
  if( glVersionFlags & QGLFormat::OpenGL_Version_3_1 )
    qWarning() << "- QGLFormat::OpenGL_Version_3_1";
  if( glVersionFlags & QGLFormat::OpenGL_Version_3_0 )
    qWarning() << "- QGLFormat::OpenGL_Version_3_0";

  if( glVersionFlags & QGLFormat::OpenGL_Version_2_1 )
    qWarning() << "- QGLFormat::OpenGL_Version_2_1";
  if( glVersionFlags & QGLFormat::OpenGL_Version_2_0 )
    qWarning() << "- QGLFormat::OpenGL_Version_2_0";

  if( glVersionFlags & QGLFormat::OpenGL_Version_1_5 )
    qWarning() << "- QGLFormat::OpenGL_Version_1_5";
  if( glVersionFlags & QGLFormat::OpenGL_Version_1_4 )
    qWarning() << "- QGLFormat::OpenGL_Version_1_4";
  if( glVersionFlags & QGLFormat::OpenGL_Version_1_3 )
    qWarning() << "- QGLFormat::OpenGL_Version_1_3";
  if( glVersionFlags & QGLFormat::OpenGL_Version_1_2 )
    qWarning() << "- QGLFormat::OpenGL_Version_1_2";
  if( glVersionFlags & QGLFormat::OpenGL_Version_1_1 )
    qWarning() << "- QGLFormat::OpenGL_Version_1_1";

  if( glVersionFlags & QGLFormat::OpenGL_Version_None )
    qWarning() << "- QGLFormat::OpenGL_Version_None";
#endif
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
