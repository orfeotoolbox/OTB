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

namespace mvd
{
/*
  TRANSLATOR mvd::ImageViewManipulator

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
#if USE_VIEW_SETTINGS_SIDE_EFFECT

ImageViewManipulator
::ImageViewManipulator( const otb::ViewSettings::Pointer& viewSettings,
                        QObject* parent ) :
  AbstractImageViewManipulator( parent ),
  m_ViewSettings( viewSettings ),
  m_MousePressPosition(),
  m_MousePressOrigin(),
  m_RenderMode( AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL )
{
}

#else // USE_VIEW_SETTINGS_SIDE_EFFECT

ImageViewManipulator
::ImageViewManipulator( QObject* parent ) :
  AbstractImageViewManipulator( parent ),
  m_ViewSettings( otb::ViewSettings::New() ),
  m_MousePressPosition(),
  m_MousePressOrigin(),
  m_RenderMode( AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL )
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

#if 0
  context->m_RenderMode =
    AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL;
#else
  context->m_RenderMode = m_RenderMode;
#endif

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

  if( ( event->buttons() & Qt::LeftButton )==Qt::LeftButton )
    {
    Translate( event->pos() - m_MousePressPosition );

    SetRenderMode( event );

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

  switch( event->button() )
    {
    case Qt::NoButton:
      break;

    case Qt::LeftButton:
      m_MousePressPosition = QPoint();
      m_MousePressOrigin = PointType();

      SetRenderMode( NULL );

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
}

/******************************************************************************/
void
ImageViewManipulator
::WheelEvent( QWheelEvent* event )
{
}

/******************************************************************************/
void
ImageViewManipulator
::KeyPressEvent( QKeyEvent* event )
{
  // qDebug() << this << "::KeyPressEvent(" << event << ")";
}

/******************************************************************************/
void
ImageViewManipulator
::Translate( const QPoint& vector )
{
  // qDebug() << m_MousePressPosition << event->pos() << vector;

  otb::ViewSettings::PointType origin( m_MousePressOrigin );
  otb::ViewSettings::SpacingType spacing( m_ViewSettings->GetSpacing() );

  origin[ 0 ] -= static_cast< double >( vector.x() ) * spacing[ 0 ];
  origin[ 1 ] -= static_cast< double >( vector.y() ) * spacing[ 1 ];

  /*
    qDebug()
    << "(" << m_MousePressOrigin[ 0 ] << "," << m_MousePressOrigin[ 1 ] << ")"
    << "(" << spacing[ 0 ] << "," << spacing[ 1 ] << ")"
    << "(" << origin[ 0 ] << "," << origin[ 1 ] << ")";
  */

  m_ViewSettings->SetOrigin( origin );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/
} // end namespace 'mvd'
