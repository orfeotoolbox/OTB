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

#include "mvdImageViewWidget.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{
/*
  TRANSLATOR mvd::ImageViewWidget

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
ImageViewWidget
::ImageViewWidget( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer,
                   QWidget* parent,
                   const QGLWidget* shareWidget,
                   Qt::WindowFlags flags ) :
  QGLWidget( parent, shareWidget, flags ),
  m_Manipulator( NULL ),
  m_Renderer( NULL )
{
  Initialize( manipulator, renderer );
}

/*******************************************************************************/
ImageViewWidget
::ImageViewWidget( AbstractImageViewManipulator * manipulator,
                 AbstractImageViewRenderer * renderer,
                 QGLContext* context,
		 QWidget* parent,
		 const QGLWidget* shareWidget,
		 Qt::WindowFlags flags ) :
  QGLWidget( context, parent, shareWidget, flags ),
  m_Manipulator( NULL ),
  m_Renderer( NULL )
{
  Initialize( manipulator, renderer );
}

/*******************************************************************************/
ImageViewWidget
::ImageViewWidget( AbstractImageViewManipulator * manipulator,
                 AbstractImageViewRenderer * renderer,
                 const QGLFormat& format,
                 QWidget* parent,
                 const QGLWidget* shareWidget,
                 Qt::WindowFlags flags ) :
  QGLWidget( format, parent, shareWidget, flags ),
  m_Manipulator( NULL ),
  m_Renderer( NULL )
{
  Initialize( manipulator, renderer );
}

/*******************************************************************************/
ImageViewWidget
::~ImageViewWidget()
{
  // m_Manipulator (deleted as a child of a QObjet parent).
  // m_Renderer (deleted as a child of a QObjet parent).
}

/*******************************************************************************/
void
ImageViewWidget
::Initialize( AbstractImageViewManipulator* manipulator,
	      AbstractImageViewRenderer* renderer )
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

  m_Manipulator = manipulator;
  m_Manipulator->setParent( this );

  m_Renderer = renderer;
  m_Renderer->setParent( this );
}

/*******************************************************************************/
void
ImageViewWidget
::initializeGL()
{
}

/*******************************************************************************/
void
ImageViewWidget
::resizeGL( int width, int height )
{
  glViewport(
    0,
    0,
    static_cast< GLint >( width ),
    static_cast< GLint >( height )
  );
}

/*******************************************************************************/
void
ImageViewWidget
::paintGL()
{
  AbstractImageViewRenderer::RenderingContext context;

  m_Renderer->paintGL( context );
}

/*******************************************************************************/
void
ImageViewWidget
::mousePressEvent( QMouseEvent* event )
{
  QGLWidget::mousePressEvent( event );

  m_Manipulator->mousePressEvent( event );
}

/*******************************************************************************/
void
ImageViewWidget
::mouseMoveEvent( QMouseEvent* event )
{
  QGLWidget::mouseMoveEvent( event );

  m_Manipulator->mouseMoveEvent( event );
}

/*******************************************************************************/
void
ImageViewWidget
::mouseReleaseEvent( QMouseEvent* event )
{
  QGLWidget::mouseReleaseEvent( event );

  m_Manipulator->mouseReleaseEvent(event);
}

/*******************************************************************************/
void
ImageViewWidget
::wheelEvent( QWheelEvent* event )
{
  QGLWidget::wheelEvent( event );

  m_Manipulator->wheelEvent(event);
}

/*******************************************************************************/
void
ImageViewWidget
::resizeEvent( QResizeEvent* event )
{
  // First, call superclass implementation
  QGLWidget::resizeEvent( event );

  m_Manipulator->resizeEvent( event );
}

/*******************************************************************************/
void
ImageViewWidget
::keyPressEvent( QKeyEvent* event )
{
  QGLWidget::keyPressEvent( event );

  m_Manipulator->keyPressEvent( event );
}

/*******************************************************************************/
void
ImageViewWidget
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

}
