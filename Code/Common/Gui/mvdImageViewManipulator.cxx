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
  m_ViewSettings( viewSettings )
{
}

#else // USE_VIEW_SETTINGS_SIDE_EFFECT

ImageViewManipulator
::ImageViewManipulator( QObject* parent ) :
  AbstractImageViewManipulator( parent ),
  m_ViewSettings( otb::ViewSettings::New() )
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
::virtual_SetViewportSize( int width, int height )
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
::virtual_SetOrigin( const PointType& origin )
{
  assert( !m_ViewSettings.IsNull() );

  m_ViewSettings->SetOrigin( origin );
}

/******************************************************************************/
void
ImageViewManipulator
::virtual_SetSpacing( const SpacingType& spacing )
{
  assert( !m_ViewSettings.IsNull() );

  m_ViewSettings->SetSpacing( spacing );
}

/******************************************************************************/
void
ImageViewManipulator
::virtual_SetWkt( const std::string& wkt )
{
  assert( !m_ViewSettings.IsNull() );

  m_ViewSettings->SetWkt( wkt );
  m_ViewSettings->SetUseProjection( !wkt.empty() );
}

/******************************************************************************/
void
ImageViewManipulator
::virtual_SetupRenderingContext(
  AbstractImageViewRenderer::RenderingContext * const c ) const
{
  assert(
    c==dynamic_cast< ImageViewRenderer::RenderingContext const * >( c )
  );

  ImageViewRenderer::RenderingContext * const context =
    dynamic_cast< ImageViewRenderer::RenderingContext * const >( c );

  context->m_RenderMode =
    AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL;

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
::virtual_MousePressEvent( QMouseEvent* event )
{
}

/******************************************************************************/
void
ImageViewManipulator
::virtual_MouseMoveEvent( QMouseEvent* event)
{
}

/******************************************************************************/
void
ImageViewManipulator
::virtual_MouseReleaseEvent( QMouseEvent* event)
{
}

/******************************************************************************/
void
ImageViewManipulator
::virtual_ResizeEvent( QResizeEvent* event )
{
}

/******************************************************************************/
void
ImageViewManipulator
::virtual_WheelEvent( QWheelEvent* event )
{
}

/******************************************************************************/
void
ImageViewManipulator
::virtual_KeyPressEvent( QKeyEvent* event )
{
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/
} // end namespace 'mvd'
