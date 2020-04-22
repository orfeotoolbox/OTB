/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


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
#include "mvdQuicklookViewRenderer.h"

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

QuicklookViewManipulator::QuicklookViewManipulator(const otb::ViewSettings::Pointer& viewSettings, QObject* p)
  : ImageViewManipulator(viewSettings, p), m_RoiOrigin(), m_RoiSpacing(), m_RoiSize()
{
  m_RoiOrigin.Fill(0);
  m_RoiSpacing.Fill(0);
  m_RoiSize.Fill(0);
}

#else // USE_VIEW_SETTINGS_SIDE_EFFECT

QuicklookViewManipulator::QuicklookViewManipulator(QObject* p) : ImageViewManipulator(p), m_RoiOrigin(), m_RoiSpacing(), m_RoiSize()
{
  m_RoiOrigin.Fill(0);
  m_RoiSpacing.Fill(0);
  m_RoiSize.Fill(0);
}

#endif // USE_VIEW_SETTINGS_SIDE_EFFECT

/*****************************************************************************/
QuicklookViewManipulator::~QuicklookViewManipulator()
{
}

/******************************************************************************/
void QuicklookViewManipulator::SetupRenderingContext(AbstractImageViewRenderer::RenderingContext* const c) const
{
  assert(c == dynamic_cast<QuicklookViewRenderer::RenderingContext const*>(c));

  QuicklookViewRenderer::RenderingContext* const context = dynamic_cast<QuicklookViewRenderer::RenderingContext* const>(c);

  // Coverity-19843.
  // {
  assert(context != NULL);
  // }

  context->m_RoiOrigin = m_RoiOrigin;
  context->m_RoiExtent = m_RoiOrigin;

  context->m_RoiExtent[0] += static_cast<double>(m_RoiSize[0]) * m_RoiSpacing[0];
  context->m_RoiExtent[1] += static_cast<double>(m_RoiSize[1]) * m_RoiSpacing[1];

#if USE_VIEW_SETTINGS_SIDE_EFFECT
#else  // USE_VIEW_SETTINGS_SIDE_EFFECT
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT
}

/******************************************************************************/
void QuicklookViewManipulator::MousePressEvent(QMouseEvent* e)
{
  ImageViewManipulator::MousePressEvent(e);

  assert(e != NULL);

  // qDebug() << this << ":" << e;


  Qt::MouseButtons      buttons   = e->buttons();
  Qt::KeyboardModifiers modifiers = e->modifiers();

  if (buttons == Qt::LeftButton && modifiers == Qt::NoModifier)
  {
    assert(!m_ViewSettings.IsNull());

    PointType center;

    m_ViewSettings->ScreenToViewPortTransform(static_cast<double>(m_MousePressPosition.x()), static_cast<double>(m_MousePressPosition.y()), center[0],
                                              center[1]);

    Q_EMIT CenterRoiRequested(center);
  }
}

/******************************************************************************/
void QuicklookViewManipulator::MouseMoveEvent(QMouseEvent* e)
{
  assert(e != NULL);

  // qDebug() << this << ":" << e;

  Qt::MouseButtons      buttons   = e->buttons();
  Qt::KeyboardModifiers modifiers = e->modifiers();

  if (buttons == Qt::LeftButton && modifiers == Qt::NoModifier)
  {
    assert(!m_ViewSettings.IsNull());

    PointType center;

    m_ViewSettings->ScreenToViewPortTransform(static_cast<double>(m_MousePressPosition.x()), static_cast<double>(m_MousePressPosition.y()), center[0],
                                              center[1]);

    m_MousePressPosition = e->pos();

    Q_EMIT CenterRoiRequested(center);
  }
}

/******************************************************************************/
/*
void
QuicklookViewManipulator
::MouseReleaseEvent( QMouseEvent* e)
{
  assert( e!=NULL );

  // qDebug() << this << ":" << e;
}
*/

/******************************************************************************/
void QuicklookViewManipulator::ResizeEvent(QResizeEvent* e)
{
  ImageViewManipulator::ResizeEvent(e);

  Q_EMIT ZoomToExtentRequested();
}

/******************************************************************************/
void QuicklookViewManipulator::WheelEvent(QWheelEvent*)
{
}

/******************************************************************************/
void QuicklookViewManipulator::KeyPressEvent(QKeyEvent*)
{
}

/******************************************************************************/
void QuicklookViewManipulator::KeyReleaseEvent(QKeyEvent*)
{
}

/*****************************************************************************/
ZoomType QuicklookViewManipulator::GetFixedZoomType() const
{
  return ZOOM_TYPE_EXTENT;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void QuicklookViewManipulator::OnRoiChanged(const PointType& origin, const SizeType& size, const SpacingType& spacing, const PointType&)
{
  /*
  qDebug() << this << ":OnRoiChanged()";
  qDebug() << "origin :" << origin[ 0 ] << "," << origin[ 1 ];
  qDebug() << "size   :" << size[ 0 ] << "," << size[ 1 ];
  qDebug() << "spacing:" << spacing[ 0 ] << "," << spacing[ 1 ];
  */

  m_RoiOrigin  = origin;
  m_RoiSize    = size;
  m_RoiSpacing = spacing;

  Q_EMIT RefreshViewRequested();
}

} // end namespace 'mvd'
