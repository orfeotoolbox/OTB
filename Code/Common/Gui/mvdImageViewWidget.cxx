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
// See ::OnUpdateProjectionRequired()
#include "Gui/mvdImageViewRenderer.h"

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
#if USE_XP_REGION_OPTIM
  ,m_Position()
#endif // USE_XP_REGION_OPTION
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
#if USE_XP_REGION_OPTION
  ,m_Position()
#endif // USE_XP_REGION_OPTION
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
#if USE_XP_REGION_OPTION
  ,m_Position()
#endif // USE_XP_REGION_OPTION
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
  assert( m_Renderer!=NULL );

  {
  StackedLayerModel * model = m_Renderer->GetLayerStack();

  if( model!=NULL )
    {
    for( StackedLayerModel::ConstIterator it( model->Begin() );
         it!=model->End();
         ++it )
    Disconnect( it->second );

    //
    // Disconnect stacked-layer model from this image-view.
    QObject::disconnect(
      model,
      SIGNAL( LayerAdded( size_t ) ),
      // from:
      this,
      SLOT( OnLayerAdded( size_t ) )
    );

    QObject::disconnect(
      model,
      SIGNAL( LayerAboutToBeDeleted( size_t ) ),
      // from:
      this,
      SLOT( LayerAboutToBeDeleted( size_t ) )
    );

    QObject::disconnect(
      model,
      SIGNAL( ReferenceChanged( size_t ) ),
      // from:
      this,
      SLOT( OnReferenceChanged( size_t ) )
    );

#if 0
    QObject::disconnect(
      model,
      SIGNAL( ContentChanged() ),
      // from:
      this,
      SLOT( UpdateScene() )
    );

    QObject::disconnect(
      model,
      SIGNAL( ContentChanged() ),
      // from:
      this,
      SLOT( updateGL() )
    );
#else
    QObject::disconnect(
      model,
      SIGNAL( ContentChanged() ),
      // from:
      this,
      SLOT( OnContentChanged() )
    );
#endif

    QObject::disconnect(
      model,
      SIGNAL( ContentReset() ),
      // from:
      this,
      SLOT( OnContentReset() )
    );

    QObject::disconnect(
      model,
      SIGNAL( OrderChanged() ),
      // from:
      this,
      SLOT( updateGL() )
    );

    //
    // Disconnect layer-stack model from this widget manipulator.
    QObject::disconnect(
      GetManipulator(),
      SIGNAL( RotateLayersRequested( int ) ),
      // from:
      model,
      SLOT( RotateLayers( int ) )
    );
    }
  }

  // Set projection parameters of manipulator from layer data.
  OnSetProjectionRequired();

  // Insert image-models into image-view renderer.
  m_Renderer->SetLayerStack( stackedLayerModel );


  //
  // Connect layer-stack model to this widget manipulator.
  QObject::connect(
    GetManipulator(),
    SIGNAL( RotateLayersRequested( int ) ),
    // to:
    stackedLayerModel,
    SLOT( RotateLayers( int ) )
  );

  //
  // Connect stacked layer-model to image-view manipulator.

  QObject::connect(
    stackedLayerModel,
    SIGNAL( LayerAdded( size_t ) ),
    // to:
    this,
    SLOT( OnLayerAdded( size_t ) )
  );

  //
  // Connect stacked layer-model to this image-view
  QObject::connect(
    stackedLayerModel,
    SIGNAL( LayerAboutToBeDeleted( size_t ) ),
    // to:
    this,
    SLOT( OnLayerAboutToBeDeleted( size_t ) )
  );

  QObject::connect(
    stackedLayerModel,
    SIGNAL( OrderChanged() ),
    // to:
    this,
    SLOT( updateGL() )
  );

#if 0
  QObject::connect(
    stackedLayerModel,
    SIGNAL( ContentChanged() ),
    // to:
    this,
    SLOT( UpdateScene() )
  );

  QObject::connect(
    stackedLayerModel,
    SIGNAL( ContentChanged() ),
    // to:
    this,
    SLOT( updateGL() )
  );
#else
  QObject::connect(
    stackedLayerModel,
    SIGNAL( ContentChanged() ),
    // to:
    this,
    SLOT( OnContentChanged() )
  );
#endif

  QObject::connect(
    stackedLayerModel,
    SIGNAL( ContentReset() ),
    // to:
    this,
    SLOT( OnContentReset() )
  );

  QObject::connect(
    stackedLayerModel,
    SIGNAL( ReferenceChanged( size_t ) ),
    // to
    this,
    SLOT( OnReferenceChanged( size_t ) )
  );

  for( StackedLayerModel::ConstIterator it( stackedLayerModel->Begin() );
       it!=stackedLayerModel->End();
       ++it )
    Connect( it->second );
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

  m_Manipulator = manipulator;
  m_Manipulator->setParent( this );
  m_Manipulator->SetViewportSize( width(), height() );


  assert( renderer!=NULL );

  m_Renderer = renderer;
  m_Renderer->setParent( this );


#if USE_XP_REGION_OPTION
  m_Position[ 0 ] = m_Position[ 1 ] = 0;
#endif // USE_XP_REGION_OPTION


  //
  // Manipulator -> this
  //

  QObject::connect(
    m_Manipulator,
    SIGNAL( RefreshViewRequested() ),
    // to:
    this,
    SLOT( updateGL() )
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
    SIGNAL( DeleteAllRequested() ),
    // to:
    this,
    SLOT( OnDeleteAllRequested() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( DeleteSelectedRequested() ),
    // to:
    this,
    SLOT( OnDeleteSelectedRequested() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ToggleLayerVisibilityRequested( bool ) ),
    // to:
    this,
    SLOT( OnToggleLayerVisibilityRequested( bool ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ZoomToFullResolutionRequested() ),
    // to:
    this,
    SLOT( ZoomToFullResolution() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ZoomToLayerExtentRequested() ),
    // to:
    this,
    SLOT( ZoomToLayerExtent() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ZoomToExtentRequested() ),
    // to:
    this,
    SLOT( ZoomToExtent() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ResizeShaderRequested( double ) ),
    // to:
    this,
    SLOT( OnResizeShaderRequested( double ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ReparamShaderRequested( double ) ),
    // to:
    this,
    SLOT( OnReparamShaderRequested( double ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ShiftAlphaRequested( double ) ),
    // to:
    this,
    SLOT( OnShiftAlphaRequested( double ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( UpdateGammaRequested( double ) ),
    // to:
    this,
    SLOT( OnUpdateGammaRequested( double ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ShiftDynamicsRequested( double ) ),
    // to:
    this,
    SLOT( OnShiftDynamicsRequested( double ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ScaleDynamicsRequested( double ) ),
    // to:
    this,
    SLOT( OnScaleDynamicsRequested( double ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ResetQuantilesRequested( bool ) ),
    // to:
    this,
    SLOT( OnResetQuantilesRequested( bool ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ShaderEffectRequested( Effect ) ),
    // to:
    this,
    SLOT( OnShaderEffectRequested( Effect ) )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( ApplyAllRequested() ),
    // to:
    this,
    SLOT( OnApplyAllRequested() )
  );

  QObject::connect(
    m_Manipulator,
    SIGNAL( SetReferenceRequested() ),
    // to:
    this,
    SLOT( OnSetReferenceRequested() )
  );


  //
  // Renderer -> this
  //

  QObject::connect(
    this,
    SIGNAL(
      PhysicalCursorPositionChanged(
        const QPoint &,
        const PointType &,
        const PointType &,
        const DefaultImageType::PixelType &
      )
    ),
    // to:
    m_Renderer,
    SLOT(
      OnPhysicalCursorPositionChanged(
        const QPoint &,
        const PointType &,
        const PointType &,
        const DefaultImageType::PixelType &
      )
    )
  );


  QObject::connect(
    m_Renderer,
    SIGNAL( SetProjectionRequired() ),
    // to:
    this,
    SLOT( OnSetProjectionRequired() )
  );

  QObject::connect(
    m_Renderer,
    SIGNAL( UpdateProjectionRequired() ),
    // to:
    this,
    SLOT( OnUpdateProjectionRequired() )
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

  // Qt::MouseButtons buttons = event->buttons();
  // Qt::KeyboardModifiers modifiers = event->modifiers();


  // Transform coordinates from widget space to viewport space.
  assert( m_Manipulator!=NULL );

  PointType in;

  m_Manipulator->Transform( in, event->pos() );

  // qDebug() << "--------";
  // qDebug() << "mouse:" << in[ 0 ] << "," << in[ 1 ];

  //
  // Pick pixel of point in viewport space and return point in image
  // space.
  assert( m_Renderer!=NULL );

  PointType out;
  DefaultImageType::PixelType pixel;

  m_Renderer->Pick( in, out, pixel );

  // qDebug() << "PhysicalCursorPositionChanged(" << event->pos() << ")";

  emit PhysicalCursorPositionChanged( event->pos(), in, out, pixel );

  {
  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  for( StackedLayerModel::ConstIterator it( stackedLayerModel->Begin() );
       it!=stackedLayerModel->End();
       ++ it )
    {
    assert( it->second!=NULL );

    if( it->second->inherits( AbstractImageModel::staticMetaObject.className() ) )
      {
      VectorImageModel * imageModel = qobject_cast< VectorImageModel * >( it->second );
      assert( imageModel!=NULL );

      if( imageModel->GetSettings().GetEffect()!=EFFECT_NONE &&
	  imageModel->GetSettings().GetEffect()!=EFFECT_NORMAL )
	{
#if USE_XP_REGION_OPTIM
	PointType origin;
	PointType extent;

	m_Renderer->GetLayerExtent( it->first, origin, extent );

	if( ( origin[ 0 ]<=in[ 0 ] && in[ 0 ]<=extent[ 0 ] &&
	      origin[ 1 ]<=in[ 1 ] && in[ 1 ]<=extent[ 1 ] ) |
	    ( origin[ 0 ]<=m_Position[ 0 ] && m_Position[ 0 ]<=extent[ 0 ] &&
	      origin[ 1 ]<=m_Position[ 1 ] && m_Position[ 1 ]<=extent[ 1 ] ) )
	  {
	  qDebug() << FromStdString( it->first );

	  qDebug()
	    << "o:" << origin[ 0 ] << "," << origin[ 1 ] << ";"
	    << "e:" << extent[ 0 ] << "," << extent[ 1 ];
#endif // USE_XP_REGION_OPTIM

	  // qDebug() << "updateGL(" << in[ 0 ] << "," << in[ 1 ] << ")";

	  updateGL();

	  break;

	  }
#if USE_XP_REGION_OPTIM
	}
#endif // USE_XP_REGION_OPTIM
      }
    }
  }

#if USE_XP_REGION_OPTIM
  m_Position = in;
#endif // USE_XP_REGION_OPTION
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
  assert(
    zoom==ZOOM_TYPE_EXTENT || zoom==ZOOM_TYPE_FULL || zoom==ZOOM_TYPE_LAYER
  );

  assert( m_Renderer!=NULL );
  assert( m_Manipulator!=NULL );


  PointType center;
  SpacingType spacing;

  center[ 0 ] = center[ 1 ] = 0.0;
  spacing[ 0 ] = spacing[ 1 ] = 1.0;


  switch( zoom )
    {
    case ZOOM_TYPE_EXTENT:
    {
    // qDebug() << "ZOOM_TYPE_EXTENT";

    //
    // Calculate center
    PointType origin;
    PointType extent;

    m_Renderer->GetViewExtent( origin, extent );

    center.SetToMidPoint( origin, extent );

    //
    // Get spacing of reference layer.
    const AbstractLayerModel * layer = m_Renderer->GetReferenceModel();
    assert( layer!=NULL );

    if( layer->inherits( AbstractImageModel::staticMetaObject.className() ) )
      {
      const AbstractImageModel * imageModel =
        qobject_cast< const AbstractImageModel * >( layer );

      assert( imageModel!=NULL );

      spacing = imageModel->GetSpacing();

      assert( spacing[ 0 ]!=0.0 );
      assert( spacing[ 1 ]!=0.0 );
      }
    else
      assert( false && "Unhandled AbstractLayerModel derived type." );

    //
    // Get Viewport size.
    SizeType size( m_Manipulator->GetViewportSize() );

    //
    // Calculate scale.
    double scale = 
      std::max(
        fabs( extent[ 0 ] - origin[ 0 ] ) / static_cast< double >( size[ 0 ] ),
        fabs( extent[ 1 ] - origin[ 1 ] ) / static_cast< double >( size[ 1 ] )
      );

    //
    // Calculate spacing.
    //
    // Scale of Viewport is a spacing, sign of spacing needs to be
    // reported back.
    spacing[ 0 ] = ( spacing[ 0 ] >=0.0 ? 1.0 : -1.0 ) * scale;
    spacing[ 1 ] = ( spacing[ 1 ] >=0.0 ? 1.0 : -1.0 ) * scale;
    }
    break;

    case ZOOM_TYPE_FULL:
    {
    // qDebug() << "ZOOM_TYPE_FULL";

    const AbstractLayerModel * layer = m_Renderer->GetReferenceModel();
    assert( layer!=NULL );

    if( layer->inherits( AbstractImageModel::staticMetaObject.className() ) )
      {
      const AbstractImageModel * imageModel =
        qobject_cast< const AbstractImageModel * >( layer );
        
      assert( imageModel!=NULL );

      spacing = imageModel->GetSpacing();
      }
    else
      assert( false && "Unhandled AbstractLayerModel derived class." );

    PointType origin;
    PointType extent;

    m_Renderer->GetReferenceExtent( origin, extent );

    center.SetToMidPoint( origin, extent );
    }
    break;

    case ZOOM_TYPE_LAYER:
    {
    // qDebug() << "ZOOM_TYPE_LAYER";

    assert( GetLayerStack()!=NULL );

    //
    // Calculate center
    PointType origin;
    PointType extent;

    m_Renderer->GetLayerExtent(
      m_Renderer->GetLayerStack()->GetCurrentKey(),
      origin,
      extent
    );

    center.SetToMidPoint( origin, extent );

    //
    // Get Viewport size.
    SizeType size( m_Manipulator->GetViewportSize() );

    //
    // Calculate scale.
    double scale = 
      std::max(
        fabs( extent[ 0 ] - origin[ 0 ] ) / static_cast< double >( size[ 0 ] ),
        fabs( extent[ 1 ] - origin[ 1 ] ) / static_cast< double >( size[ 1 ] )
      );

    //
    // Get spacing of layer in viewport system.
    spacing[ 0 ] = ( extent[ 0 ] - origin[ 0 ] ) / size[ 0 ];
    spacing[ 1 ] = ( extent[ 1 ] - origin[ 1 ] ) / size[ 1 ];

    //
    // Calculate spacing.
    //
    // Scale of Viewport is a spacing, sign of spacing needs to be
    // reported back.
    spacing[ 0 ] = ( spacing[ 0 ] >=0.0 ? 1.0 : -1.0 ) * scale;
    spacing[ 1 ] = ( spacing[ 1 ] >=0.0 ? 1.0 : -1.0 ) * scale;
    }
    break;

    default:
      assert( false && "Unhandled ImageViewWidget::ZoomType value!" );
      break;
    }

  //
  // Center on middle point.
  m_Manipulator->SetSpacing( spacing );
  m_Manipulator->CenterOn( center );
}

/*******************************************************************************/
void
ImageViewWidget
::Connect( AbstractLayerModel * layer )
{
  assert( layer!=NULL );

  QObject::connect(
    layer,
    SIGNAL( VisibilityChanged() ),
    // to:
    this,
    SLOT( updateGL() )
  );

  QObject::connect(
    this,
    SIGNAL( ModelUpdated() ),
    // to:
    layer,
    SLOT( OnModelUpdated() )
  );
}

/*******************************************************************************/
void
ImageViewWidget
::Disconnect( AbstractLayerModel * layer )
{
  assert( layer!=NULL );

  QObject::disconnect(
    layer,
    SIGNAL( VisibilityChanged() ),
    // from:
    this,
    SLOT( updateGL() )
  );

  QObject::disconnect(
    this,
    SIGNAL( ModelUpdated() ),
    // from:
    layer,
    SLOT( OnModelUpdated() )
  );
}

/*****************************************************************************/
bool
ImageViewWidget
::IsBypassRenderingEnabled() const
{
  assert( m_Renderer!=NULL );

  return m_Renderer->IsBypassRenderingEnabled();
}

/*****************************************************************************/
bool
ImageViewWidget
::SetBypassRenderingEnabled( bool isEnabled )
{
  assert( m_Renderer!=NULL );

  return m_Renderer->SetBypassRenderingEnabled( isEnabled );
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
::OnApplyAllRequested()
{
  // qDebug() << this << "::OnApplyAllRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * layerStack = m_Renderer->GetLayerStack();
  assert( layerStack!=NULL );

  AbstractLayerModel * layer = layerStack->GetCurrent();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    VectorImageModel * imageModel =
      qobject_cast< VectorImageModel * >( layer );

    assert( imageModel!=NULL );

    const VectorImageSettings & settings = imageModel->GetSettings();

    for( StackedLayerModel::ConstIterator it( layerStack->Begin() );
	 it!=layerStack->End();
	 ++it )
      if( it->second!=layer )
	{
	if( it->second->inherits( VectorImageModel::staticMetaObject.className() ) )
	  {
	  VectorImageModel * vim =
	    qobject_cast< VectorImageModel * >( it->second );

	  vim->SetSettings( settings  );
	  }
	}

    emit ModelUpdated();
    }
}

/******************************************************************************/
void
ImageViewWidget
::OnContentChanged()
{
  qDebug() << this << "::OnContentChanged()";

  UpdateScene();

  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::OnContentReset()
{
  qDebug() << this << "::OnContentReset()";

  UpdateScene();

  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::OnDeleteAllRequested()
{
  qDebug() << this << "::OnDeleteAllRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->Clear();
}

/******************************************************************************/
void
ImageViewWidget
::OnDeleteSelectedRequested()
{
  // qDebug() << this << "::OnDeleteSelectedRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->DeleteCurrent();
}

/******************************************************************************/
void
ImageViewWidget
::OnLayerAboutToBeDeleted( size_t index )
{
  assert( GetLayerStack()!=NULL );

  Disconnect( GetLayerStack()->At( index ) );
}

/******************************************************************************/
void
ImageViewWidget
::OnLayerAdded( size_t index )
{
  assert( GetLayerStack()!=NULL );

  Connect( GetLayerStack()->At( index ) );
}

/******************************************************************************/
void
ImageViewWidget
::OnLowerLayerRequested()
{
  // qDebug() << this << "::OnLowerLayerRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->LowerCurrent();
}

/******************************************************************************/
void
ImageViewWidget
::OnRaiseLayerRequested()
{
  // qDebug() << this << "::OnRaiseLayerRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->RaiseCurrent();
}

/******************************************************************************/
void
ImageViewWidget
::OnReferenceChanged( size_t index )
{
  // qDebug() << this << "::OnReferenceChanged(" << index << ")";

  assert( m_Renderer!=NULL );

  m_Renderer->RefreshScene();
}

/******************************************************************************/
void
ImageViewWidget
::OnResetQuantilesRequested( bool isGlobal )
{
  // qDebug() << this << "::OnResetQuantilesRequested(" << isGlobal << ")";

  //
  // Get layer-stack.
  StackedLayerModel * layerStack = m_Renderer->GetLayerStack();
  assert( layerStack!=NULL );

  if( !layerStack->HasCurrent() )
    return;

  // qDebug() << "current:" << FromStdString( layerStack->GetCurrentKey() );

  //
  // Get dynamics of current layer.
  assert( m_Renderer!=NULL );

  ParametersType params( 6 );

  if( !m_Renderer->GetLayerDynamics( layerStack->GetCurrentKey(), params, isGlobal ) )
    return;

  // qDebug() << "R: [" << params[ 0 ] << "," << params[ 1 ] << "]";
  // qDebug() << "G: [" << params[ 2 ] << "," << params[ 3 ] << "]";
  // qDebug() << "B: [" << params[ 4 ] << "," << params[ 5 ] << "]";

  //
  // Apply dynamics to current layer.
  AbstractLayerModel * layer = layerStack->GetCurrent();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    VectorImageModel * imageModel =
      qobject_cast< VectorImageModel * >( layer );

    VectorImageSettings & settings = imageModel->GetSettings();

    if( settings.IsGrayscaleActivated() )
      settings.SetGrayDynamicsParams( params );

    else
      settings.SetRgbDynamicsParams( params );

    /*
    HistogramModel * histogram = imageModel->GetHistogramModel();
    assert( histogram!=NULL );

    // Get min/max pixels.
    DefaultImageType::PixelType minPx( histogram->GetMinPixel() );
    DefaultImageType::PixelType maxPx( histogram->GetMaxPixel() );

    // Iterator bounds.
    CountType begin = -1;
    CountType end = -1;

    // Shift intensity for each channel.
    if( RgbwBounds( begin,
		    end,
		    settings.IsGrayscaleActivated() ? RGBW_CHANNEL_WHITE : RGBW_CHANNEL_RGB ) )
      for( CountType i=begin; i<end; ++i )
	{
	// Channel.
	RgbwChannel c = static_cast< RgbwChannel >( i );

	// Band.
	VectorImageSettings::ChannelVector::value_type b = settings.GetSmartChannel( c );

	// Compute step.
	DefaultImageType::PixelType::ValueType step =
	  static_cast< DefaultImageType::PixelType::ValueType >(
	    delta * ( maxPx[ b ] - minPx[ b ] )
	);

	// Apply step shift.
	settings.SetLowIntensity( c, settings.GetLowIntensity( c ) + step );
	settings.SetHighIntensity( c, settings.GetHighIntensity( c ) + step );
	}
    */

    emit ModelUpdated();
    }
}

/******************************************************************************/
void
ImageViewWidget
::OnResizeShaderRequested( double factor )
{
  // qDebug() << this << "::OnResizeShaderRequested(" << factor << ")";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  AbstractLayerModel * layer = stackedLayerModel->GetCurrent();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    VectorImageModel * imageModel =
      qobject_cast< VectorImageModel * >( layer );

    assert( imageModel!=NULL );

    imageModel->GetSettings().SetSize(
      static_cast< double >( imageModel->GetSettings().GetSize() ) * factor
    );

    emit ModelUpdated();
    }
}

/******************************************************************************/
void
ImageViewWidget
::OnReparamShaderRequested( double factor )
{
  // qDebug() << this << "::OnReparamShaderRequested(" << factor << ")";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  AbstractLayerModel * layer = stackedLayerModel->GetCurrent();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    VectorImageModel * imageModel =
      qobject_cast< VectorImageModel * >( layer );

    assert( imageModel!=NULL );

    imageModel->GetSettings().SetValue(
      static_cast< double >( imageModel->GetSettings().GetValue() ) * factor
    );

    emit ModelUpdated();
    }
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
::OnScaleDynamicsRequested( double factor )
{
  // qDebug() << this << "::OnScaleDynamicsRequested(" << factor << ")";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  AbstractLayerModel * layer = stackedLayerModel->GetCurrent();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    VectorImageModel * imageModel =
      qobject_cast< VectorImageModel * >( layer );

    VectorImageSettings & settings = imageModel->GetSettings();

    CountType begin = -1;
    CountType end = -1;

    // Shift intensity for each channel.
    if( RgbwBounds( begin,
		    end,
		    settings.IsGrayscaleActivated() ? RGBW_CHANNEL_WHITE : RGBW_CHANNEL_RGB ) )
      for( CountType i=begin; i<end; ++i )
	{
	RgbwChannel c = static_cast< RgbwChannel >( i );

	ParametersType::ValueType lo = settings.GetLowIntensity( c );
	ParametersType::ValueType hi = settings.GetHighIntensity( c );
	ParametersType::ValueType o = ( lo + hi ) / 2;

	settings.SetLowIntensity( c, factor * ( lo - o ) + o );
	settings.SetHighIntensity( c, factor * ( hi - o ) + o );
	}

    /*
    // Grayscale
    if( settings.IsGrayscaleActivated() )
      {
      // Get color-dynamics.
      ParametersType params( settings.GetGrayDynamicsParams() );

      // Compute center value.
      ParametersType::ValueType center = ( params[ 0 ] + params[ 1 ] ) / 2;

      // Scale color-dynamics around center value.
      params[ 0 ] = factor * ( params[ 0 ] - center ) + center;
      params[ 1 ] = factor * ( params[ 1 ] - center ) + center;

      // Set color-dynamics.
      settings.SetLowIntensity( RGBW_CHANNEL_WHITE, params[ 0 ] );
      settings.SetHighIntensity( RGBW_CHANNEL_WHITE, params[ 1 ] );
      }
    // RGB
    else
      {
      // Get color-setup.
      VectorImageSettings::ChannelVector channels( settings.GetRgbChannels() );

      // Get color-dynamics.
      ParametersType params(
	settings.GetRgbDynamicsParams()
      );

      CountType begin = -1;
      CountType end = -1;

      // Shift intensity for each channel.
      if( RgbBounds( begin, end, RGBW_CHANNEL_RGB ) )
	for( CountType i=begin; i<end; ++i )
	  {
	  // Compute center value.
	  ParametersType::ValueType center = ( params[ 2 * i ] + params[ 2 * i + 1 ] ) / 2;

	  // Scale color-dynamics around center value.
	  params[ 2 * i ] = factor * ( params[ 2 * i ] - center ) + center;
	  params[ 2 * i + 1 ] = factor * ( params[ 2 * i + 1 ] - center ) + center;
	  }

      // Set shifted intensities.
      settings.SetRgbDynamicsParams( params );
      }
    */

    emit ModelUpdated();
    }
}

/******************************************************************************/
void
ImageViewWidget
::OnSelectPreviousLayerRequested()
{
  // qDebug() << this << "::OnSelectPreviousLayerRequested()";

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
  // qDebug() << this << "::OnSelectNextLayerRequested()";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  stackedLayerModel->SelectNext();

  updateGL();
}

/******************************************************************************/
void
ImageViewWidget
::OnSetProjectionRequired()
{
  StackedLayerModel * stackedLayerModel = GetLayerStack();

  if( stackedLayerModel==NULL )
    return;


  const AbstractLayerModel * layer = stackedLayerModel->GetReference();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    const VectorImageModel * imageModel =
      qobject_cast< const VectorImageModel * >( layer );

    assert( imageModel!=NULL );

    DefaultImageType::ConstPointer image( imageModel->ToImage() );
    assert( !image.IsNull() );

    assert( m_Manipulator!=NULL );

    m_Manipulator->SetWkt( image->GetProjectionRef() );
    m_Manipulator->SetKeywordList( image->GetImageKeywordlist() );

    m_Manipulator->SetOrigin( imageModel->GetOrigin() );
    m_Manipulator->SetSpacing( imageModel->GetSpacing() );
    m_Manipulator->SetNativeSpacing( imageModel->GetNativeSpacing() );
    }
  else
    {
    assert( false && "Unhandled AbstractLayerModel derived type." );
    }
}

/******************************************************************************/
void
ImageViewWidget
::OnSetReferenceRequested()
{
  // qDebug() << this << "::OnSetReferenceRequested()";

  StackedLayerModel * stackedLayerModel = GetLayerStack();

  if( stackedLayerModel==NULL )
    return;

  if( !stackedLayerModel->HasCurrent() )
    return;

  stackedLayerModel->SetReference( stackedLayerModel->GetCurrentKey() );
}

/******************************************************************************/
void
ImageViewWidget
::OnShaderEffectRequested( Effect effect )
{
  // qDebug()
  //   << this
  //   << "::OnShaderEffectRequested(" << QString( EFFECT_NAME[ effect ] ) << ")";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  AbstractLayerModel * layer = stackedLayerModel->GetCurrent();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    VectorImageModel * imageModel =
      qobject_cast< VectorImageModel * >( layer );

    assert( imageModel!=NULL );

    imageModel->GetSettings().SetEffect( effect );

    emit ModelUpdated();
    }
}

/******************************************************************************/
void
ImageViewWidget
::OnShiftAlphaRequested( double delta )
{
  // qDebug() << this << "::OnShiftAlphaRequested(" << delta << ")";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  AbstractLayerModel * layer = stackedLayerModel->GetCurrent();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    VectorImageModel * imageModel =
      qobject_cast< VectorImageModel * >( layer );

    imageModel->GetSettings().SetAlpha( imageModel->GetSettings().GetAlpha() + delta );

    emit ModelUpdated();
    }
}

/******************************************************************************/
void
ImageViewWidget
::OnShiftDynamicsRequested( double delta )
{
  // qDebug() << this << "::OnShiftDynamicsRequested(" << delta << ")";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  AbstractLayerModel * layer = stackedLayerModel->GetCurrent();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    VectorImageModel * imageModel =
      qobject_cast< VectorImageModel * >( layer );

    VectorImageSettings & settings = imageModel->GetSettings();

    HistogramModel * histogram = imageModel->GetHistogramModel();
    assert( histogram!=NULL );

    // Get min/max pixels.
    DefaultImageType::PixelType minPx( histogram->GetMinPixel() );
    DefaultImageType::PixelType maxPx( histogram->GetMaxPixel() );

    // Iterator bounds.
    CountType begin = -1;
    CountType end = -1;

    // Shift intensity for each channel.
    if( RgbwBounds( begin,
		    end,
		    settings.IsGrayscaleActivated() ? RGBW_CHANNEL_WHITE : RGBW_CHANNEL_RGB ) )
      for( CountType i=begin; i<end; ++i )
	{
	// Channel.
	RgbwChannel c = static_cast< RgbwChannel >( i );

	// Band.
	VectorImageSettings::ChannelVector::value_type b = settings.GetSmartChannel( c );

	// Compute step.
	DefaultImageType::PixelType::ValueType step =
	  static_cast< DefaultImageType::PixelType::ValueType >(
	    delta * ( maxPx[ b ] - minPx[ b ] )
	);

	// Apply step shift.
	settings.SetLowIntensity( c, settings.GetLowIntensity( c ) + step );
	settings.SetHighIntensity( c, settings.GetHighIntensity( c ) + step );
	}

    /*
    // Grayscale
    if( settings.IsGrayscaleActivated() )
      {
      // Get color-setup.
      unsigned int channel = settings.GetGrayChannel();

      // Get color-dynamics.
      ParametersType params( settings.GetGrayDynamicsParams() );

      // Compute step.
      DefaultImageType::PixelType::ValueType step =
	static_cast< DefaultImageType::PixelType::ValueType >(
	  delta * ( maxPx[ channel ] - minPx[ channel ]
	  )
	);

      // Shift min & max intensities of channel.
      params[ 0 ] += step;
      params[ 1 ] += step;

      // Set color-dynamics.
      settings.SetLowIntensity( RGBW_CHANNEL_WHITE, params[ 0 ] );
      settings.SetHighIntensity( RGBW_CHANNEL_WHITE, params[ 1 ] );
      }
    // RGB
    else
      {	
      // Get color-setup.
      VectorImageSettings::ChannelVector channels( settings.GetRgbChannels() );

      // Get color-dynamics.
      ParametersType params(
	settings.GetRgbDynamicsParams()
      );

      // Shift intensity for each channel.
      for( VectorImageSettings::ChannelVector::const_iterator it( channels.begin() );
	   it!=channels.end();
	   ++it )
	params[ *it ] +=
	  static_cast< DefaultImageType::PixelType::ValueType >(
	    delta * ( maxPx[ *it ] - minPx[ *it ]
	    )
	  );

      // Set shifted intensities.
      settings.SetRgbDynamicsParams( params );
      }
    */

    emit ModelUpdated();
    }
}

/******************************************************************************/
void
ImageViewWidget
::OnToggleLayerVisibilityRequested( bool )
{
  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  AbstractLayerModel * layer = stackedLayerModel->GetCurrent();

  if( layer==NULL )
    return;

  layer->SetVisible( !layer->IsVisible() );
}

/******************************************************************************/
void
ImageViewWidget
::OnUpdateGammaRequested( double factor )
{
  // qDebug() << this << "::OnUpdateGammaRequested(" << factor << ")";

  assert( m_Renderer!=NULL );

  StackedLayerModel * stackedLayerModel = m_Renderer->GetLayerStack();
  assert( stackedLayerModel!=NULL );

  AbstractLayerModel * layer = stackedLayerModel->GetCurrent();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    VectorImageModel * imageModel =
      qobject_cast< VectorImageModel * >( layer );

    imageModel->GetSettings().SetGamma( imageModel->GetSettings().GetGamma() * factor );

    emit ModelUpdated();
    }
}

/******************************************************************************/
void
ImageViewWidget
::OnUpdateProjectionRequired()
{
  // Reminder: specific #include "mvdImageViewRenderer.h"
  assert( m_Manipulator!=NULL );

  assert( m_Renderer==qobject_cast< const ImageViewRenderer * >( m_Renderer ) );

  //
  // ImageViewRenderer specific code (as opposing to
  // AbstractImageViewRenderer).
  //
  // Could be moved in derived class if necessary.
  // {
  const ImageViewRenderer * renderer =
    qobject_cast< const ImageViewRenderer * >( m_Renderer );

  assert( m_Renderer!=NULL );


  PointType center;
  SpacingType spacing;

  renderer->Reproject(
    center,
    spacing,
    m_Manipulator->GetCenter(),
    m_Manipulator->GetSpacing()
  );
  // }


  // Projection WKT and KWL must preferably be set before changing center/spacing
  const AbstractLayerModel * layer = GetLayerStack()->GetReference();
  assert( layer!=NULL );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    assert( layer==qobject_cast< const VectorImageModel * >( layer ) );

    const VectorImageModel * imageModel =
      qobject_cast< const VectorImageModel * >( layer );

    assert( imageModel!=NULL );

    DefaultImageType::ConstPointer image( imageModel->ToImage() );
    assert( !image.IsNull() );

    assert( m_Manipulator!=NULL );

    m_Manipulator->SetWkt( image->GetProjectionRef() );
    m_Manipulator->SetKeywordList( image->GetImageKeywordlist() );
    }
  else
    {
    assert( false && "Unhandled AbstractLayerModel derived type." );
    }

  // Spacing must be set before centering.
  m_Manipulator->SetSpacing( spacing );

  // Centering must be done at the last step.
  m_Manipulator->CenterOn( center );
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
::ZoomToLayerExtent()
{
  assert( m_Renderer!=NULL );

  // Scale and center.
  Center( ImageViewWidget::ZOOM_TYPE_LAYER );

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
