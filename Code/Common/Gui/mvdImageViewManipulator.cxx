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

#include "mvdImageViewManipulator.h"

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
#include "Gui/mvdImageViewRenderer.h"

#define MOUSE_WHEEL_STEP_DEGREES 15

namespace mvd
{
/*
  TRANSLATOR mvd::ImageViewManipulator

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

const int ImageViewManipulator::DEFAULT_ZOOM_GRANULARITY = 2;
const int ImageViewManipulator::DEFAULT_SCROLL_GRANULARITY = 4;

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
#if USE_VIEW_SETTINGS_SIDE_EFFECT

ImageViewManipulator
::ImageViewManipulator( const otb::ViewSettings::Pointer& viewSettings,
                        QObject* parent ) :
  AbstractImageViewManipulator( parent ),
  m_ViewSettings( viewSettings ),
  m_MousePressPosition(),
  m_MousePressOrigin(),
  m_RenderMode( AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL ),
  m_ZoomFactor( 1.0 ),
  m_ZoomGranularity( ImageViewManipulator::DEFAULT_ZOOM_GRANULARITY ),
  m_ScrollGranularity( ImageViewManipulator::DEFAULT_SCROLL_GRANULARITY )
{
}

#else // USE_VIEW_SETTINGS_SIDE_EFFECT

ImageViewManipulator
::ImageViewManipulator( QObject* parent ) :
  AbstractImageViewManipulator( parent ),
  m_ViewSettings( otb::ViewSettings::New() ),
  m_MousePressPosition(),
  m_MousePressOrigin(),
  m_RenderMode( AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL ),
  m_ZoomFactor( 1.0 ),
  m_ZoomGranularity( ImageViewManipulator::DEFAULT_ZOOM_GRANULARITY ),
  m_ScrollGranularity( ImageViewManipulator::DEFAULT_SCROLL_GRANULARITY )
{
}

#endif // USE_VIEW_SETTINGS_SIDE_EFFECT

/*****************************************************************************/
ImageViewManipulator
::~ImageViewManipulator()
{
}

/******************************************************************************/
void
ImageViewManipulator
::SetViewportSize( int width, int height )
{
  SizeType size;

  size[ 0 ] = width;
  size[ 1 ] = height;

  assert( !m_ViewSettings.IsNull() );

  m_ViewSettings->SetViewportSize( size );
}

/******************************************************************************/
SizeType
ImageViewManipulator
::GetViewportSize() const
{
  assert( !m_ViewSettings.IsNull() );

  return m_ViewSettings->GetViewportSize();
}

/******************************************************************************/
void
ImageViewManipulator
::SetOrigin( const PointType& origin )
{
  assert( !m_ViewSettings.IsNull() );

  m_ViewSettings->SetOrigin( origin );
}

/******************************************************************************/
void
ImageViewManipulator
::SetSpacing( const SpacingType& spacing )
{
  assert( !m_ViewSettings.IsNull() );

  m_ViewSettings->SetSpacing( spacing );
}

/******************************************************************************/
SpacingType
ImageViewManipulator
::GetSpacing() const
{
  assert( !m_ViewSettings.IsNull() );

  return m_ViewSettings->GetSpacing();
}

/******************************************************************************/
void
ImageViewManipulator
::SetWkt( const std::string& wkt )
{
  assert( !m_ViewSettings.IsNull() );

  m_ViewSettings->SetWkt( wkt );
#if 0
  m_ViewSettings->SetUseProjection( !wkt.empty() );
#else
  m_ViewSettings->SetUseProjection( true );
#endif
}

/******************************************************************************/
void
ImageViewManipulator
::SetKeywordList( const DefaultImageType::ImageKeywordlistType& kwl )
{
  assert( !m_ViewSettings.IsNull() );

  m_ViewSettings->SetKeywordList( kwl );
}

/******************************************************************************/
void
ImageViewManipulator
::SetupRenderingContext(
  AbstractImageViewRenderer::RenderingContext * const c ) const
{
  assert(
    c==dynamic_cast< ImageViewRenderer::RenderingContext const * >( c )
  );

  ImageViewRenderer::RenderingContext * const context =
    dynamic_cast< ImageViewRenderer::RenderingContext * const >( c );

  context->m_RenderMode = m_RenderMode;

#if USE_VIEW_SETTINGS_SIDE_EFFECT
#else // USE_VIEW_SETTINGS_SIDE_EFFECT
  context->m_ViewSettings->SetOrigin( m_ViewSettings->GetOrigin() );
  context->m_ViewSettings->SetSpacing( m_ViewSettings->GetSpacing() );
  context->m_ViewSettings->SetViewportSize( m_ViewSettings->GetViewportSize() );
  context->m_ViewSettings->SetWkt( m_ViewSettings->GetWkt() );
  context->m_ViewSettings->SetKeywordList( m_ViewSettings->GetKeywordList() );
  context->m_ViewSettings->SetUseProjection( m_ViewSettings->GetUseProjection() );
  context->m_ViewSettings->SetGeometryChanged(
    m_ViewSettings->GetGeometryChanged()
  );
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT
}

/******************************************************************************/
void
ImageViewManipulator
::CenterOn( const PointType& point )
{
  assert( !m_ViewSettings.IsNull() );

  m_ViewSettings->Center( point );
}

/******************************************************************************/
void
ImageViewManipulator
::MousePressEvent( QMouseEvent* event )
{
  assert( event!=NULL );

  // qDebug() << this << ":" << event;

  switch( event->button() )
    {
    case Qt::NoButton:
      break;

    case Qt::LeftButton:
      m_MousePressPosition = event->pos();
      m_MousePressOrigin = m_ViewSettings->GetOrigin();
      break;

    case Qt::RightButton:
      break;

    case Qt::MiddleButton:
      break;

    case Qt::XButton1:
      break;

    case Qt::XButton2:
      break;

    default:
      assert( false && "Unhandled Qt::MouseButton." );
      break;
    }

  /*
    Qt::NoModifier	0x00000000	No modifier key is pressed.
    Qt::ShiftModifier	0x02000000	A Shift key on the keyboard is pressed.
    Qt::ControlModifier	0x04000000	A Ctrl key on the keyboard is pressed.
    Qt::AltModifier	0x08000000	An Alt key on the keyboard is pressed.
    Qt::MetaModifier	0x10000000	A Meta key on the keyboard is pressed.
    Qt::KeypadModifier	0x20000000	A keypad button is pressed.
    Qt::GroupSwitchModifier
  */
}

/******************************************************************************/
void
ImageViewManipulator
::MouseMoveEvent( QMouseEvent* event)
{
  assert( event!=NULL );

  /*
  qDebug() << "------------------------------------------------";

  qDebug() << this << ":" << event;
  */

  Qt::MouseButtons buttons = event->buttons();
  Qt::KeyboardModifiers modifiers = event->modifiers();

  if( buttons==Qt::LeftButton &&
      ( modifiers==Qt::NoModifier || modifiers== Qt::ControlModifier ) )
    {
    // Cursor moves from press position to current position;
    // Image moves the same direction, so apply the negative translation.
    Translate( m_MousePressPosition - event->pos() );

    m_MousePressPosition = event->pos();

    emit RefreshView();
    }
}

/******************************************************************************/
void
ImageViewManipulator
::MouseReleaseEvent( QMouseEvent* event)
{
  assert( event!=NULL );

  /*
  qDebug() << this << ":" << event;
  */

  /*
  Qt::MouseButtons buttons = event->buttons();
  Qt::KeyboardModifiers modifiers = event->modifiers();
  */

  switch( event->button() )
    {
    case Qt::NoButton:
      break;

    case Qt::LeftButton:
      m_MousePressPosition = QPoint();
      m_MousePressOrigin = PointType();

      emit RefreshView();
      break;

    case Qt::RightButton:
      break;

    case Qt::MiddleButton:
      break;

    case Qt::XButton1:
      break;

    case Qt::XButton2:
      break;

    default:
      assert( false && "Unhandled Qt::MouseButton." );
      break;
    }
}

/******************************************************************************/
void
ImageViewManipulator
::ResizeEvent( QResizeEvent* event )
{
  assert( event!=NULL );

  // qDebug() << this << "::ResizeEvent(" << event << ")";

  /*
  qDebug() << m_ViewSettings.GetPointer();

  otb::ViewSettings::SizeType size( m_ViewSettings->GetViewportSize() );

  qDebug() << size[ 0 ] << "," << size[ 1 ] << "\t" << event->size();
  */
}

/******************************************************************************/
void
ImageViewManipulator
::WheelEvent( QWheelEvent* event )
{
  assert( event!=NULL );

  Qt::MouseButtons buttons = event->buttons();
  Qt::KeyboardModifiers modifiers = event->modifiers();

  if( buttons!=Qt::NoButton )
    return;

  // Delta is rotation distance in number of 8th of degrees (see
  // http://qt-project.org/doc/qt-4.8/qwheelevent.html#delta).
  assert( event->delta()!=0 );
  int degrees = event->delta() / 8;

  if( modifiers==Qt::NoModifier || modifiers==Qt::ControlModifier )
    {
    Scale( event->pos(), degrees );

    emit RefreshView();
    }
}

/******************************************************************************/
void
ImageViewManipulator
::KeyPressEvent( QKeyEvent* event )
{
  assert( event!=NULL );

  // qDebug() << this << "::KeyPressEvent(" << event << ")";

  QPoint vector( 0, 0 );
  int steps = 0;

  int key = event->key();
  Qt::KeyboardModifiers modifiers = event->modifiers();

  switch( key )
    {
    case Qt::Key_Up:
      vector.setY( -1 );
      break;

    case Qt::Key_Down:
      vector.setY( +1 );
      break;

    case Qt::Key_Left:
      vector.setX( -1 );
      break;

    case Qt::Key_Right:
      vector.setX( +1 );
      break;

    case Qt::Key_Plus:
      steps = m_ZoomGranularity;
      break;

    case Qt::Key_Minus:
      steps = -m_ZoomGranularity;
      break;

    case Qt::Key_Control:
      SetFastRenderMode( true );
      break;

    default:
      break;
    }

  assert( !m_ViewSettings.IsNull() );

  otb::ViewSettings::SizeType size( m_ViewSettings->GetViewportSize() );

  bool needsRefresh = false;

  //
  // Translate

  if( !vector.isNull() )
    {
    if( modifiers==Qt::NoModifier )
      {
      size[ 0 ] /= m_ScrollGranularity;
      size[ 1 ] /= m_ScrollGranularity;
      }
    else if( modifiers==Qt::ControlModifier )
      {
      size[ 0 ] /= m_ScrollGranularity * 2;
      size[ 1 ] /= m_ScrollGranularity * 2;
      }

    vector.rx() *= size[ 0 ];
    vector.ry() *= size[ 1 ];

    Translate( vector );

    needsRefresh = true;
    }

  //
  // Scale

  if( steps!=0 )
    {
    // Qt::ControlModifier doest not work with keypard Qt::Key_Plus/Minus keys.

    Scale(
      QPoint( size[ 0 ] / 2.0, size[ 1 ] / 2.0 ),
      steps * MOUSE_WHEEL_STEP_DEGREES
    );

    needsRefresh = true;
    }

  //
  // Refresh
  if( needsRefresh )
    emit RefreshView();
}

/******************************************************************************/
void
ImageViewManipulator
::KeyReleaseEvent( QKeyEvent* event )
{
  assert( event!=NULL );

  // qDebug() << this << "::KeyPressEvent(" << event << ")";

  switch( event->key() )
    {
    case Qt::Key_Control:
      SetFastRenderMode( false );
      emit RefreshView();
      break;

    default:
      break;
    }
}

/******************************************************************************/
void
ImageViewManipulator
::Translate( const QPoint& vector )
{
  // qDebug() << this << "::Translate(" << vector << ")";

  // otb::ViewSettings::PointType origin( m_MousePressOrigin );
  otb::ViewSettings::PointType origin( m_ViewSettings->GetOrigin() );

  otb::ViewSettings::SpacingType spacing( m_ViewSettings->GetSpacing() );

  origin[ 0 ] += static_cast< double >( vector.x() ) * spacing[ 0 ];
  origin[ 1 ] += static_cast< double >( vector.y() ) * spacing[ 1 ];

  /*
    qDebug()
    << "(" << m_MousePressOrigin[ 0 ] << "," << m_MousePressOrigin[ 1 ] << ")"
    << "(" << spacing[ 0 ] << "," << spacing[ 1 ] << ")"
    << "(" << origin[ 0 ] << "," << origin[ 1 ] << ")";
  */

  m_ViewSettings->SetOrigin( origin );
}

/******************************************************************************/
void
ImageViewManipulator
::Scale( const QPoint& center, int degrees )
{
  assert( degrees!=0 );

  if( degrees==0 )
    return;

  otb::ViewSettings::PointType point;

  m_ViewSettings->ScreenToViewPortTransform(
    center.x(), center.y(),
    point[ 0 ], point[ 1 ]
  );

  // See http://qt-project.org/doc/qt-4.8/qwheelevent.html#delta .
  assert( m_ZoomGranularity!=0 );

  int granularity = m_ZoomGranularity;

  if( granularity==0 )
    {
    granularity = 1;
    }

  double factor = pow(
    2.0,
    -static_cast< double >( degrees ) /
    static_cast< double >( granularity * MOUSE_WHEEL_STEP_DEGREES )
  );

  m_ZoomFactor *= factor;

  /*
  qDebug()
    << "(" << point[ 0 ] << "," << point[ 1 ] << ")"
    << "g:" << granularity
    << "d:" << degrees
    << "s:" << (static_cast< double >( degrees ) / 15.0)
    << "f:" << factor
    << "z:" << m_ZoomFactor;
  */

  m_ViewSettings->Zoom( point, factor );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/
} // end namespace 'mvd'
