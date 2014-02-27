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

#include "mvdQuicklookViewManipulator.h"


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
#include "Gui/mvdQuicklookViewRenderer.h"

namespace mvd
{
/*
  TRANSLATOR mvd::QuicklookViewManipulator

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
#if USE_VIEW_SETTINGS_SIDE_EFFECT

QuicklookViewManipulator
::QuicklookViewManipulator( const otb::ViewSettings::Pointer& viewSettings,
                            QObject* parent ) :
  ImageViewManipulator( viewSettings, parent ),
  m_RoiOrigin(),
  m_RoiSpacing(),
  m_RoiSize()
{
  m_RoiOrigin.Fill( 0 );
  m_RoiSpacing.Fill( 0 );
  m_RoiSize.Fill( 0 );
}

#else // USE_VIEW_SETTINGS_SIDE_EFFECT

QuicklookViewManipulator
::QuicklookViewManipulator( QObject* parent ) :
  ImageViewManipulator( parent ),
  m_RoiOrigin(),
  m_RoiSpacing(),
  m_RoiSize()
{
  m_RoiOrigin.Fill( 0 );
  m_RoiSpacing.Fill( 0 );
  m_RoiSize.Fill( 0 );
}

#endif // USE_VIEW_SETTINGS_SIDE_EFFECT

/*****************************************************************************/
QuicklookViewManipulator
::~QuicklookViewManipulator()
{
}

/******************************************************************************/
void
QuicklookViewManipulator
::SetupRenderingContext(
  AbstractImageViewRenderer::RenderingContext * const c ) const
{
  assert(
    c==dynamic_cast< QuicklookViewRenderer::RenderingContext const * >( c )
  );

  QuicklookViewRenderer::RenderingContext * const context =
    dynamic_cast< QuicklookViewRenderer::RenderingContext * const >( c );

  context->m_RoiOrigin = m_RoiOrigin;
  context->m_RoiExtent = m_RoiOrigin;

  context->m_RoiExtent[ 0 ] +=
    static_cast< double >( m_RoiSize[ 0 ] ) * m_RoiSpacing[ 0 ];
  context->m_RoiExtent[ 1 ] +=
    static_cast< double >( m_RoiSize[ 1 ] ) * m_RoiSpacing[ 1 ];

#if USE_VIEW_SETTINGS_SIDE_EFFECT
#else // USE_VIEW_SETTINGS_SIDE_EFFECT
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT
}

/******************************************************************************/
void
QuicklookViewManipulator
::MouseMoveEvent( QMouseEvent* event)
{
  assert( event!=NULL );

  // qDebug() << this << ":" << event;

  Qt::MouseButtons buttons = event->buttons();
  Qt::KeyboardModifiers modifiers = event->modifiers();

  if( buttons==Qt::LeftButton && modifiers==Qt::NoModifier )
    {
    // Cursor moves from press position to current position;
    // Image moves the same direction, so apply the negative translation.
    m_RoiOrigin =
      ImageViewManipulator::Translate(
        event->pos() - m_MousePressPosition,
        m_RoiOrigin,
        m_RoiSpacing
      );

    m_MousePressPosition = event->pos();

    emit RefreshView();

    emit RoiChanged( m_RoiOrigin, m_RoiSize, m_RoiSpacing );
    }
}

/******************************************************************************/
/*
void
QuicklookViewManipulator
::MouseReleaseEvent( QMouseEvent* event)
{
  assert( event!=NULL );

  // qDebug() << this << ":" << event;
}
*/

/******************************************************************************/
void
QuicklookViewManipulator
::ResizeEvent( QResizeEvent* event )
{
  ImageViewManipulator::ResizeEvent( event );

  emit ZoomToExtent();
}

/******************************************************************************/
void
QuicklookViewManipulator
::WheelEvent( QWheelEvent* event )
{
  assert( event!=NULL );
}

/******************************************************************************/
void
QuicklookViewManipulator
::KeyPressEvent( QKeyEvent* event )
{
  assert( event!=NULL );

  // qDebug() << this << "::KeyPressEvent(" << event << ")";
}

/******************************************************************************/
void
QuicklookViewManipulator
::KeyReleaseEvent( QKeyEvent* event )
{
  assert( event!=NULL );

  // qDebug() << this << "::KeyPressEvent(" << event << ")";
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
QuicklookViewManipulator
::OnRoiChanged( PointType origin, SizeType size, SpacingType spacing )
{
  qDebug() << this << ":OnRoiChanged()";
  qDebug() << "origin :" << origin[ 0 ] << "," << origin[ 1 ];
  qDebug() << "size   :" << size[ 0 ] << "," << size[ 1 ];
  qDebug() << "spacing:" << spacing[ 0 ] << "," << spacing[ 1 ];

  m_RoiOrigin = origin;
  m_RoiSize = size;
  m_RoiSpacing = spacing;
 
  emit RefreshView();
}

} // end namespace 'mvd'
