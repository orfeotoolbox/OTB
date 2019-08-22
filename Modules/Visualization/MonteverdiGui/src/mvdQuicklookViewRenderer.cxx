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

#include "mvdQuicklookViewRenderer.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QGLWidget>
// #include <QtOpenGL>

//
// System includes (sorted by alphabetic order)
// necessary for the opengl variables and methods

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbGlImageActor.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdStackedLayerModel.h"
#include "mvdTypes.h"
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::QuicklookViewRenderer

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
QuicklookViewRenderer
::QuicklookViewRenderer( QObject* p ) :
  ImageViewRenderer( p ),
  m_GlRoiActor( otb::GlROIActor::New() )
{
  assert( !m_GlRoiActor.IsNull() );

  setObjectName( "QuicklookViewRenderer" );

  m_EffectsEnabled = false;
}

/*****************************************************************************/
QuicklookViewRenderer
::~QuicklookViewRenderer()
{
}

/*****************************************************************************/
AbstractImageViewRenderer::RenderingContext*
QuicklookViewRenderer
::NewRenderingContext() const
{
  RenderingContext* context = new QuicklookViewRenderer::RenderingContext();

#if USE_VIEW_SETTINGS_SIDE_EFFECT
#else
  assert( !m_GlView.IsNull() );

  //
  // Share otb::GlViewRendering settings with manipulator using
  // RenderingContext. Manipulator can then setup otb::ViewSettings
  // directly by side-effect.
  context->m_ViewSettings = m_GlView->GetSettings();
#endif

  return context;
}

/*******************************************************************************/
void
QuicklookViewRenderer
::virtual_SetProjection()
{
  SetWktAndKwl();
}

/*******************************************************************************/
void
QuicklookViewRenderer
::virtual_UpdateProjection()
{
  SetWktAndKwl();
}

/*******************************************************************************/
void
QuicklookViewRenderer
::SetWktAndKwl()
{
  // qDebug() << this << "::SetWktAndKwl()";

#if DISABLE_QUICKLOOK_VIEW
  return;
#endif

  assert( GetLayerStack()!=NULL );

  if( GetLayerStack()->IsEmpty() )
    return;

  otb::GlImageActor::Pointer referenceGlImageActor(
    GetReferenceActor< otb::GlImageActor >()
  );

  assert( !referenceGlImageActor.IsNull() );

  m_GlRoiActor->SetKwl( referenceGlImageActor->GetKwl() );
  m_GlRoiActor->SetWkt( referenceGlImageActor->GetWkt() );
}

/*******************************************************************************/
void
QuicklookViewRenderer
::virtual_FinishScene()
{
  // qDebug() << this << "::virtual_FinishScene()";

  assert( !m_GlView.IsNull() );

#if DISABLE_QUICKLOOK_VIEW
  return;
#endif

  m_GlRoiActor->CreateShader();

  std::string key( m_GlView->AddActor( m_GlRoiActor, "ROI" ) );

  m_GlRoiActor->SetVisible( true );
  m_GlRoiActor->SetOverlay( true );

  // qDebug() << "Added roi-actor:" << FromStdString( key );

  /*
  ColorType color;

  color.Fill( 1.0 );

  m_GlRoiActor->SetColor( color ); 
  */
  m_GlRoiActor->SetFill( false );
  m_GlRoiActor->SetAlpha( 0.2 );

  m_GlView->MoveActorToEndOfRenderingOrder( key, true );
}

/*****************************************************************************/
void
QuicklookViewRenderer
::UpdateActors( const AbstractImageViewRenderer::RenderingContext* c )
{
  // qDebug() << this << "::UpdateActors()";

  assert( c!=NULL );

  ImageViewRenderer::UpdateActors( c );

  assert(
    c==dynamic_cast< const QuicklookViewRenderer::RenderingContext * >( c )
  );

  const QuicklookViewRenderer::RenderingContext * context =
    dynamic_cast< const QuicklookViewRenderer::RenderingContext * >( c );

  // Coverity-19842
  // {
  assert( context!=NULL );
  // }

  /*
  qDebug()
    << "ROI-origin:"
    << context->m_RoiOrigin[ 0 ] << "," << context->m_RoiOrigin[ 1 ]
    << "ROI-extent:"
    << context->m_RoiExtent[ 0 ] << "," << context->m_RoiExtent[ 1 ];
  */

  m_GlRoiActor->SetUL( context->m_RoiOrigin );
  m_GlRoiActor->SetLR( context->m_RoiExtent );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
