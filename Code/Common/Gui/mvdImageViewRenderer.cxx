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
#include "mvdImageViewRenderer.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtOpenGL>

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
#include "Core/mvdDatasetModel.h"
#include "Core/mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ImageViewRenderer

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
ImageViewRenderer
::ImageViewRenderer( QObject* parent ) :
  AbstractImageViewRenderer( parent ),
  m_GlView( otb::GlView::New() ),
  m_ReferenceGlImageActor()
{
  assert( !m_GlView.IsNull() );
}

/*****************************************************************************/
ImageViewRenderer
::~ImageViewRenderer()
{
}

/*******************************************************************************/
void
ImageViewRenderer
::virtual_SetImageList( const VectorImageModelList& images )
{
  assert( !m_GlView.IsNull() );

  //
  // Remove all actors.
  m_GlView->ClearActors();

  //
  // Return if there is no vector-image model.
  if( images.isEmpty() )
    return;

  //
  // Insert new actors corresponding to vector-image model.
  for( VectorImageModelList::const_iterator it( images.begin() );
       it!=images.end();
       ++it )
    {
    assert( *it!=NULL );

    otb::GlImageActor::Pointer imageActor( otb::GlImageActor::New() );

    imageActor->Initialize( ToStdString( ( *it )->GetFilename() ));

    DatasetModel* datasetModel = ( *it )->GetDatasetModel();

    std::string actorKey(
      m_GlView->AddActor(
        imageActor,
        datasetModel==NULL
        ? std::string()
        : ToStdString( datasetModel->GetHash() )
      )
    );

    qDebug() << "Added image-actor:" << FromStdString( actorKey );
    }

  //
  // Remember first actor as reference actor.
  otb::GlView::StringVectorType keys( m_GlView->GetRenderingOrder() );
  assert( !keys.empty() );

  otb::GlActor::Pointer glActor( m_GlView->GetActor( keys.front() ) );
  assert( !glActor.IsNull() );

  assert( glActor==otb::DynamicCast< otb::GlImageActor >( glActor ) );

  m_ReferenceGlImageActor = otb::DynamicCast< otb::GlImageActor >( glActor );
  assert( !m_ReferenceGlImageActor.IsNull() );

  //
  // Center view on center of reference actor.
  otb::ViewSettings::PointType origin;
  otb::ViewSettings::PointType extent;

  m_ReferenceGlImageActor->GetExtent(
    origin[ 0 ], origin[ 1 ],
    extent[ 0 ], extent[ 1 ]
  );

  origin.SetToMidPoint( origin, extent );

  m_GlView->GetSettings()->Center( origin );
}

/*****************************************************************************/
AbstractImageViewRenderer::RenderingContext*
ImageViewRenderer
::virtual_NewRenderingContext() const
{
  RenderingContext* context = new ImageViewRenderer::RenderingContext();

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

/*****************************************************************************/
void
ImageViewRenderer
::virtual_InitializeGL()
{
  assert( !m_GlView.IsNull() );

  m_GlView->Initialize( 0, 0 );
}

/*****************************************************************************/
void
ImageViewRenderer
::virtual_ResizeGL( int width, int height )
{
  /*
  // Should be done here and not in OTB-Ice render routine.
  glViewport(
    0,
    0,
    static_cast< GLint >( width ),
    static_cast< GLint >( height )
  );
  */

  assert( !m_GlView.IsNull() );

  m_GlView->Resize( width, height );

  /*
  glViewport(
    0, 0,
    static_cast< GLint >( width ), static_cast< GLint >( height )
  );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho(
    0, static_cast< GLint >( width ),
    0, static_cast< GLint >( height ),
    0, 1
  );
  */
}

/*****************************************************************************/
void
ImageViewRenderer
::virtual_PaintGL( const AbstractImageViewRenderer::RenderingContext* c )
{
  assert( !m_GlView.IsNull() );

  /*
  //
  // Get appropriate rendering-context.
  assert( c==dynamic_cast< RenderingContext* >( c ) );
  RenderingContext* context = dynamic_cast< RenderingContext* >( c );
  assert( context!=NULL );

  //
  // Set view settings.
  m_GlView->SetSettings( context->m_ViewSettings );
  */

  //
  // Pre-render scene.
  m_GlView->BeforeRendering();
  {
  //
  // Render scene.
  switch( c->m_RenderMode )
    {
    case RenderingContext::RENDER_MODE_LIGHT:
      m_GlView->LightRender();
      break;

    case RenderingContext::RENDER_MODE_FULL:
      m_GlView->HeavyRender();
      break;
    }
  }
  //
  // Post-render scene.
  m_GlView->AfterRendering();
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
