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
#include "Core/mvdStackedLayerModel.h"
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
::CenterOn( const PointType & center,
            double scale )
{
  Center( center, scale, scale );
}

/*******************************************************************************/
void
ImageViewWidget
::SetLayerStack( StackedLayerModel * stackedLayerModel )
{
  const VectorImageModel * imageModel =
    stackedLayerModel->GetCurrent< VectorImageModel >();

  //
  // Setup image-view settings to reference image-model data.
  if( imageModel!=NULL )
    {
    DefaultImageType::ConstPointer image( imageModel->ToImage() );
    assert( !image.IsNull() );

    assert( m_Manipulator!=NULL );
    m_Manipulator->SetViewportSize( width(), height() );

    m_Manipulator->SetOrigin( imageModel->GetOrigin() );
    m_Manipulator->SetSpacing( imageModel->GetSpacing() );
    m_Manipulator->SetNativeSpacing( imageModel->GetNativeSpacing() );
    m_Manipulator->SetWkt( image->GetProjectionRef() );
    m_Manipulator->SetKeywordList( image->GetImageKeywordlist() );
    }

  // Insert image-models into image-view renderer.
  assert( m_Renderer!=NULL );
  m_Renderer->SetLayerStack( stackedLayerModel );
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
    m_Manipulator, SIGNAL( RefreshViewRequested() ),
    // to:
    this, SLOT( updateGL() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( CenterRoiRequested( const PointType& ) ),
    // to:
    this,
    SIGNAL( CenterRoiRequested( const PointType& ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL(
      RoiChanged(
        const PointType&, const SizeType&, const SpacingType&, const PointType&
      )
    ),
    // to:
    this,
    SIGNAL(
      RoiChanged(
        const PointType&, const SizeType&, const SpacingType&, const PointType&
      )
    )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL(
      RoiChanged(
        const PointType&, const SizeType&, const SpacingType&, const PointType&
      )
    ),
    // to:
    this,
    SLOT(
      OnRoiChanged(
        const PointType&, const SizeType&, const SpacingType&, const PointType&
      )
    )
  );

  QObject::connect(
    m_Manipulator, SIGNAL( ZoomToExtentRequested() ),
    // to:
    this, SLOT( ZoomToExtent() )
  );

  QObject::connect(
    m_Manipulator, SIGNAL( ZoomToFullResolutionRequested() ),
    // to:
    this, SLOT( ZoomToFullResolution() )
  );


  QObject::connect(
    m_Manipulator,
    SIGNAL( LowerLayerRequested() ),
    // to:
    this,
    SLOT( OnLowerLayerRequested() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( RaiseLayerRequested() ),
    // to:
    this,
    SLOT( OnRaiseLayerRequested() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( SelectPreviousLayerRequested() ),
    // to:
    this,
    SLOT( OnSelectPreviousLayerRequested() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( SelectNextLayerRequested() ),
    // to:
    this,
    SLOT( OnSelectNextLayerRequested() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( DeleteSelectedRequested() ),
    // to:
    this,
    SLOT( OnDeleteSelectedRequested() )
  );


  QObject::connect(
    this, SIGNAL(PhysicalCursorPositionChanged(const PointType&,
                                               const DefaultImageType::PixelType& )),
                 m_Renderer, SLOT(OnPhysicalCursorPositionChanged(const PointType&,
                                                                  const DefaultImageType::PixelType& )));


 QObject::connect(
   this, SIGNAL(ReferenceActorShaderModeChanged(const std::string &)),
   m_Renderer, SLOT(OnReferenceActorShaderModeChanged(const std::string &)));
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

  if(event->button() ==  Qt::RightButton)
    {
    emit ReferenceActorShaderModeChanged("LOCAL_CONTRAST");
    updateGL();
    }

}

/*******************************************************************************/
void
ImageViewWidget
::mouseMoveEvent( QMouseEvent* event )
{
  assert( event!=NULL );

  QGLWidget::mouseMoveEvent( event );

  m_Manipulator->MouseMoveEvent( event );
  
  // Qt::MouseButtons buttons = event->buttons();
  // Qt::KeyboardModifiers modifiers = event->modifiers();
  

  // Transform coordinates from widget space to viewport space.
  assert( m_Manipulator!=NULL );
  
  PointType in;
  
  m_Manipulator->Transform( in, event->pos() );
  
  //
  // Pick pixel of point in viewport space and return point in image
  // space.
  assert( m_Renderer!=NULL );
  
  PointType out;
  DefaultImageType::PixelType pixel;
  
#if 0
    if( m_Renderer->Pick( in, out, pixel ) )
      {
      emit PhysicalCursorPositionChanged( out, pixel );
      }
#else
    m_Renderer->Pick( in, out, pixel );

    emit PhysicalCursorPositionChanged( out, pixel );

    updateGL();

#endif  
}

/*******************************************************************************/
void
ImageViewWidget
::mouseReleaseEvent( QMouseEvent* event )
{
  assert( event!=NULL );

  QGLWidget::mouseReleaseEvent( event );

  m_Manipulator->MouseReleaseEvent(event);

  if(event->button() ==  Qt::RightButton)
    {
    emit ReferenceActorShaderModeChanged("STANDARD");
    updateGL();
    }
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
::Center( const PointType& center, double sx, double sy )
{
  /*
  qDebug()
    << this << "::Center("
    << center[ 0 ] << "," << center[ 1 ] << "," << sx << "," << sy
    << ")";
  */

  assert( m_Renderer!=NULL );
  assert( m_Manipulator!=NULL );

  //
  // Get reference image-model.
  AbstractImageModel* imageModel =
    m_Renderer->GetReferenceModel< AbstractImageModel >();
  assert( imageModel!=NULL );

  // Scale spacing.
  SpacingType spacing( imageModel->GetSpacing() );

  spacing[ 0 ] /= sx;
  spacing[ 1 ] /= sy;

  m_Manipulator->SetSpacing( spacing );

  // Center view.
  m_Manipulator->CenterOn( center );
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
  AbstractImageModel* imageModel =
    m_Renderer->GetReferenceModel< AbstractImageModel >();
  assert( imageModel!=NULL );

  //
  // Reset spacing of image-view manipulator to image-model spacing.
  if( zoom==ZOOM_TYPE_FULL ||
      zoom==ZOOM_TYPE_EXTENT )
    {
    m_Manipulator->SetOrigin( imageModel->GetOrigin() );
    m_Manipulator->SetSpacing( imageModel->GetSpacing() );
    }

  //
  // Get reference extent.
  PointType origin;
  PointType extent;

  m_Renderer->GetReferenceExtent( origin, extent );

  //
  // Calculate appropriate spacing regarding zoom-type.
  if( zoom==ZOOM_TYPE_EXTENT )
    {
    SizeType size( m_Manipulator->GetViewportSize() );

#if 1
    //
    // Does work but continues to zoom if called several times
    // because scaling is relative to current viewport.
    //
    // Fix: setting spacing of image-view manipulator before
    // calculations works.
    SpacingType spacing( m_Manipulator->GetSpacing() );

    double scale = 
      std::max(
        fabs( extent[ 0 ] - origin[ 0 ] ) / static_cast< double >( size[ 0 ] ),
        fabs( extent[ 1 ] - origin[ 1 ] ) / static_cast< double >( size[ 1 ] )
      );

    // Scale is already a spacing, we just need to apply
    spacing[ 0 ] = (spacing[0]>0?1:-1) * scale;
    spacing[ 1 ] = (spacing[1]>0?1:-1) * scale;
#else
    //
    // Might not work if image is RS-transformed!?
    SpacingType spacing( imageModel->GetSpacing() );

    ImageRegionType region( imageModel->GetNativeLargestRegion() );
    const SizeType& sz = region.GetSize();

    double scale = 
      std::max(
        static_cast< double >( sz[ 0 ] ) / static_cast< double >( size[ 0 ] ),
        static_cast< double >( sz[ 1 ] ) / static_cast< double >( size[ 1 ] )
      );

    spacing[ 0 ] *= scale;
    spacing[ 1 ] *= scale;
#endif

    // qDebug() << "scale:" << scale;

    m_Manipulator->SetSpacing( spacing );
    }

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
::CenterOn( const IndexType& index )
{
  /*
  assert( m_Renderer!=NULL );

  AbstractImageModel* imageModel = m_Render->GetReferenceImageModel();

  if( imageModel==NULL )
    return;

  ImageBaseType::Pointer image( imageModel->ToImageBase() );
  assert( !image.IsNull() );

  PointType point;

  image->TransformIndexToPhysicalPoint( index, point );

  m_Renderer->GetReferenceActor();

  assert( m_Manipulator!=NULL );

  m_Manipulator->CenterOn( point );
  */

  assert( m_Renderer!=NULL );

  PointType point;

  if( !m_Renderer->Transform( point, index, false ) )
    return;

  assert( m_Manipulator!=NULL );

  m_Manipulator->CenterOn( point );
}

/******************************************************************************/
void
ImageViewWidget
::OnDeleteSelectedRequested()
{
  qDebug() << this << "::OnDeleteSelectedRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->DeleteCurrent();

  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::OnLowerLayerRequested()
{
  qDebug() << this << "::OnLowerLayerRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->RotateDown();

  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::OnRaiseLayerRequested()
{
  qDebug() << this << "::OnRaiseLayerRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->RotateUp();

  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::OnRoiChanged( const PointType& point,
                const SizeType& size,
                const SpacingType& spacing,
                const PointType& center )
{
  assert( m_Renderer!=NULL );

  /*
  qDebug()
    << this << "::OnRoiChanged("
    << "[" << point[ 0 ] << "," << point[ 1 ] << "]"
    << "[" << center[ 0 ] << "," << center[ 1 ] << "]";
  */

  emit CenterChanged( center );

  AbstractImageModel* imageModel =
    m_Renderer->GetReferenceModel< AbstractImageModel >();

  SpacingType nativeSpacing;
  // MANTIS-970: Fixed crash when no dataset is selected.
  // {
  if( imageModel==NULL )
    {
    nativeSpacing[ 0 ] = 1.0;
    nativeSpacing[ 1 ] = 1.0;
    }
  else
    nativeSpacing = imageModel->GetNativeSpacing();
  // }

  double rsx = nativeSpacing[ 0 ] / spacing[ 0 ];
  double rsy = nativeSpacing[ 1 ] / spacing[ 1 ];

#if 0
  double sx = ( spacing[ 0 ]>0.0 ? 1.0 : -1.0 ) / spacing[ 0 ];
  double sy = ( spacing[ 1 ]>0.0 ? 1.0 : -1.0 ) / spacing[ 1 ];
#endif

  /*
  qDebug() << "sx:" << sx << "; sy:" << sy;
  qDebug() << "rsx:" << rsx << "; rsy:" << rsy;
  */

  // Emit absolute scale.
  emit ScaleChanged( rsx, rsy );

  // Emit zooming scale-factor.
  emit RoiChanged( center, rsx, rsy );
}

/******************************************************************************/
void
ImageViewWidget
::OnSelectPreviousLayerRequested()
{
  qDebug() << this << "::OnSelectPreviousLayerRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->SelectPrevious();

  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::OnSelectNextLayerRequested()
{
  qDebug() << this << "::OnSelectNextLayerRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->SelectNext();

  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::UpdateScene()
{
  assert( m_Renderer!=NULL );

  m_Renderer->UpdateScene();
}

/******************************************************************************/
void
ImageViewWidget
::ZoomToExtent()
{
  assert( m_Renderer!=NULL );

  if( m_Renderer->GetReferenceModel< AbstractImageModel >()==NULL )
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

  if( m_Renderer->GetReferenceModel< AbstractImageModel >()==NULL )
    return;

  // Scale and center.
  Center( ImageViewWidget::ZOOM_TYPE_FULL );

  // Refresh view.
  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::ZoomIn()
{
  assert( m_Manipulator!=NULL );

  m_Manipulator->ZoomIn();

  // Refresh view.
  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::ZoomOut()
{
  assert( m_Manipulator!=NULL );

  m_Manipulator->ZoomOut();

  // Refresh view.
  updateGL();
}

}
