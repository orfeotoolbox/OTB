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
#include "Core/mvdVectorImageModel.h"
#include "Gui/mvdAbstractImageViewManipulator.h"

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
::SetImageList( const AbstractImageViewRenderer::VectorImageModelList& images )
{
  //
  // Setup image-view settings to reference image-model data.
  if( !images.isEmpty() )
    {
    VectorImageModel* imageModel = images.front();
    assert( imageModel!=NULL );

    DefaultImageType::Pointer image( imageModel->ToImage() );
    assert( !image.IsNull() );

    assert( m_Manipulator!=NULL );
    m_Manipulator->SetViewportSize( width(), height() );

    m_Manipulator->SetOrigin( image->GetOrigin() );
    m_Manipulator->SetSpacing( image->GetSpacing() );
    m_Manipulator->SetWkt( image->GetProjectionRef() );
    m_Manipulator->SetKeywordList( image->GetImageKeywordlist() );
    }

  //
  // Insert image-models into image-view renderer.
  assert( m_Renderer!=NULL );
  m_Renderer->SetImageList( images );

  if( images.isEmpty() )
    return;

  //
  // Center view on center of reference image-model.
  Center();
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
  setFocusPolicy( Qt::WheelFocus );


  assert( manipulator!=NULL );
  assert( renderer!=NULL );

  m_Manipulator = manipulator;
  m_Manipulator->setParent( this );

  m_Renderer = renderer;
  m_Renderer->setParent( this );

  QObject::connect(
    m_Manipulator, SIGNAL( RefreshView() ),
    // to:
    this, SLOT( updateGL() )
  );
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
  QGLWidget::resizeGL( width, height );

  assert( m_Renderer!=NULL );

  m_Renderer->ResizeGL( width, height );
}

/*******************************************************************************/
void
ImageViewWidget
::paintGL()
{
  QGLWidget::paintGL();

  // qDebug() << this << "::paintGL()";

  //
  // Get new rendering-context.
  assert( m_Renderer!=NULL );

  AbstractImageViewRenderer::RenderingContext* context =
    m_Renderer->NewRenderingContext();

  assert( context!=NULL );

  //
  // Setup new rendering-context.
  assert( m_Manipulator!=NULL );

  m_Manipulator->SetupRenderingContext( context );

  //
  // OpenGL paint using new rendering-context.
  m_Renderer->PaintGL( context );

  //
  // Relase rendering-context.
  delete context;
  context = NULL;
}

/*******************************************************************************/
void
ImageViewWidget
::mousePressEvent( QMouseEvent* event )
{
  assert( event!=NULL );

  QGLWidget::mousePressEvent( event );

  m_Manipulator->MousePressEvent( event );
}

/*******************************************************************************/
void
ImageViewWidget
::mouseMoveEvent( QMouseEvent* event )
{
  assert( event!=NULL );

  QGLWidget::mouseMoveEvent( event );

  m_Manipulator->MouseMoveEvent( event );
}

/*******************************************************************************/
void
ImageViewWidget
::mouseReleaseEvent( QMouseEvent* event )
{
  assert( event!=NULL );

  QGLWidget::mouseReleaseEvent( event );

  m_Manipulator->MouseReleaseEvent(event);
}

/*******************************************************************************/
void
ImageViewWidget
::wheelEvent( QWheelEvent* event )
{
  assert( event!=NULL );

  QGLWidget::wheelEvent( event );

  m_Manipulator->WheelEvent(event);
}

/*******************************************************************************/
void
ImageViewWidget
::resizeEvent( QResizeEvent* event )
{
  assert( event!=NULL );

  // qDebug() << this << "::resizeEvent(" << event << ")";

  // First, call superclass implementation
  QGLWidget::resizeEvent( event );

  m_Manipulator->ResizeEvent( event );
}

/*******************************************************************************/
void
ImageViewWidget
::keyPressEvent( QKeyEvent* event )
{
  assert( event!=NULL );

  QGLWidget::keyPressEvent( event );

  m_Manipulator->KeyPressEvent( event );
}

/*******************************************************************************/
void
ImageViewWidget
::keyReleaseEvent( QKeyEvent* event )
{
  assert( event!=NULL );

  QGLWidget::keyReleaseEvent( event );

  m_Manipulator->KeyReleaseEvent( event );
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

/*****************************************************************************/
void
ImageViewWidget
::Center( ZoomType zoom )
{
  assert( m_Renderer!=NULL );
  assert( m_Manipulator!=NULL );

  //
  // Get reference image-model.
  AbstractImageModel* imageModel = m_Renderer->GetReferenceImageModel();
  assert( imageModel!=NULL );

  //
  // Get reference extent.
  PointType origin;
  PointType extent;

  m_Renderer->GetReferenceExtent( origin, extent );

  //
  // Calculate appropriate spacing regarding zoom-type.
  switch( zoom )
    {
    case ZOOM_TYPE_NONE:
      break;

    case ZOOM_TYPE_FULL:
      m_Manipulator->SetSpacing( imageModel->GetSpacing() );
      break;

    case ZOOM_TYPE_EXTENT:
    {
      SizeType size( m_Manipulator->GetViewportSize() );

      SpacingType spacing( m_Manipulator->GetSpacing() );

      double scale = 
        std::max(
          fabs( extent[ 0 ] - origin[ 0 ] ) / static_cast< double >( size[ 0 ] ),
          fabs( extent[ 1 ] - origin[ 1 ] ) / static_cast< double >( size[ 1 ] )
        );

      spacing[ 0 ] *= scale;
      spacing[ 1 ] *= scale;

      m_Manipulator->SetSpacing( spacing );
    }
    break;

    default:
      break;
    };

  //
  // Calculate middle point.
  PointType middle;
  middle.SetToMidPoint( origin, extent );

  //
  // Center on middle point.
  m_Manipulator->CenterOn( middle );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/******************************************************************************/
void
ImageViewWidget
::ZoomToExtent()
{
  assert( m_Renderer!=NULL );

  if( m_Renderer->GetReferenceImageModel()==NULL )
    return;

  // Scale and center.
  Center( ImageViewWidget::ZOOM_TYPE_EXTENT );

  // Refresh view.
  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::ZoomToFullResolution()
{
  assert( m_Renderer!=NULL );

  if( m_Renderer->GetReferenceImageModel()==NULL )
    return;

  // Scale and center.
  Center( ImageViewWidget::ZOOM_TYPE_FULL );

  // Refresh view.
  updateGL();
}

}
