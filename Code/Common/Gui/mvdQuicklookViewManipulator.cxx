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
  m_RoiExtent()
{
}

#else // USE_VIEW_SETTINGS_SIDE_EFFECT

QuicklookViewManipulator
::QuicklookViewManipulator( QObject* parent ) :
  ImageViewManipulator( parent ),
  m_RoiOrigin(),
  m_RoiExtent()
{
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
  context->m_RoiExtent = m_RoiExtent;

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

  m_RoiOrigin = origin;

  m_RoiExtent = origin;

  m_RoiExtent[ 0 ] += static_cast< double >( size[ 0 ] ) * spacing[ 0 ];
  m_RoiExtent[ 1 ] += static_cast< double >( size[ 1 ] ) * spacing[ 1 ];

  emit RefreshView();
}

} // end namespace 'mvd'
