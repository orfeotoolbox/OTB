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
#include "otbStandardShader.h"
#include "otbGlImageActor.h"
#include "otbGlVersionChecker.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractLayerModel.h"
#include "Core/mvdAlgorithm.h"
// #include "Core/mvdDatasetModel.h"
#include "Core/mvdStackedLayerModel.h"
#include "Core/mvdTypes.h"
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
  m_StackedLayerModel( NULL ),
#ifdef _WIN32
#else // _WIN32
  m_ReferencePair( NULL, otb::GlActor::Pointer() ),
#endif // _WIN32
  // m_ModelActorPairs(),
  m_ReferenceActorShaderMode( "STANDARD" )
{
  assert( !m_GlView.IsNull() );

#ifdef _WIN32
  m_ReferencePair.first = NULL;
  // m_ReferencePair.second is initialized by otb::GlActor::Pointer default constructor.
#else // _WIN32
#endif // _WIN32
}

/*****************************************************************************/
ImageViewRenderer
::~ImageViewRenderer()
{
}

/*****************************************************************************/
bool
ImageViewRenderer
::CheckGLCapabilities() const
{
#if USE_REMOTE_DESKTOP_DISABLED_RENDERING
  return true;

#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING

  //
  // Trace required OpenGL and GLSL versions.
  qWarning() <<
    ToStdString(
      tr( "Required OpenGL version '%1' with GLSL version '%2'." )
      .arg( otb::GlVersionChecker::REQUIRED_GL_VERSION )
      .arg( otb::GlVersionChecker::REQUIRED_GLSL_VERSION )
    ).c_str();

  //
  // Get and check OpenGL and GLSL versions.

  const char * glVersion = NULL;
  const char * glslVersion = NULL;

  bool isOk = false;

  try
    {
    isOk = otb::GlVersionChecker::CheckGLCapabilities( glVersion, glslVersion );

    //
    // Trace runtime OpenGL and GLSL versions.
    qWarning() <<
    ToStdString(
      tr( "Runtime OpenGL version '%1' with GLSL version '%2'." )
      .arg( glVersion )
      .arg( glslVersion )
    ).c_str();
    }
  catch( std::exception& exc )
    {
    QMessageBox::critical(
      qobject_cast< QWidget* >( parent() ),
      tr( "Monteverdi2 - Critical error!"),
      ToQString( exc.what() )
    );
    }

  //
  // Return if check has succeeded.
  if( isOk )
    return true;

  //
  // Construct message.
  QString message(
    // tr( "Current OpenGL version is '%1' supporting GLSL version '%2'.\nRequired OpenGL version is '%3' with Shading-Laguage version '%4'.\nPlease upgrade your graphics driver and hardware for the application to run properly on this platform.\nUsing the application on this platform may lead to unknown behaviour. Would you still like to continue using the application?" )
    tr( "Current OpenGL version is '%1' supporting OpenGL Shading-Language (GLSL) version '%2'.\nRequired OpenGL version is at least '%3' with GLSL version at least '%4'.\nPlease upgrade your graphics driver and/or hardware for the application to run properly on this platform.\nIf you are running the application under some remote-desktop service, runtime OpenGL and GLSL versions may differ from those running directly on remote platform.\nPlease contact your system administrator.\nApplication will now exit!" )
    .arg( glVersion )
    .arg( glslVersion )
    .arg( otb::GlVersionChecker::REQUIRED_GL_VERSION )
    .arg( otb::GlVersionChecker::REQUIRED_GLSL_VERSION )
  );

  //
  // Warn user is check has failed.
  qWarning() << ToStdString( message ).c_str();

  QMessageBox::critical(
    qobject_cast< QWidget* >( parent() ),
    tr( "Monteverdi2 - Critical error!" ),
    message
  );

  //
  // KO.
  return false;

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING
}

/*****************************************************************************/
AbstractImageViewRenderer::RenderingContext*
ImageViewRenderer
::NewRenderingContext() const
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
const AbstractLayerModel *
ImageViewRenderer
::GetReferenceModel() const
{
  return m_ReferencePair.first;
}

/*****************************************************************************/
AbstractLayerModel*
ImageViewRenderer
::GetReferenceModel()
{
  return m_ReferencePair.first;
}

/*****************************************************************************/
void
ImageViewRenderer
::GetReferenceExtent( PointType& origin, PointType& extent  ) const
{
#if USE_REMOTE_DESKTOP_DISABLED_RENDERING
  extent[ 0 ] = 0.0;
  extent[ 1 ] = 0.0;

#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING
  assert( !m_ReferencePair.second.IsNull() );

  m_ReferencePair.second->GetExtent(
    origin[ 0 ], origin[ 1 ],
    extent[ 0 ], extent[ 1 ]
  );

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING
}

/*****************************************************************************/
void
ImageViewRenderer
::InitializeGL()
{
  assert( !m_GlView.IsNull() );

  //
  // WARNING: This call to otb::GlView::Initialize() allocates a new
  // otb::ViewSettings() instance. So, the shared pointer between
  // renderer and manipulator may be unshated depending on order of
  // calls.
  //
  m_GlView->Initialize( 0, 0 );
}

/*****************************************************************************/
void
ImageViewRenderer
::ResizeGL( int width, int height )
{
  // qDebug() << this << "::ResizeGL(" << width << "," << height << ")";

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

  // qDebug() << m_GlView.GetPointer();

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
::PaintGL( const AbstractImageViewRenderer::RenderingContext* c )
{
  assert( !m_GlView.IsNull() );

  // qDebug() << this << "::PaintGL(" << c << ")";

  // qDebug() << m_GlView.GetPointer();

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

#if USE_REMOTE_DESKTOP_DISABLED_RENDERING && 0

#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING
  //
  // Apply VectorImageModel::Settings to otb::GlImageActor.
  UpdateActors( c );

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

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING
}

/*****************************************************************************/
bool
ImageViewRenderer
::Pick( const PointType& in,
        PointType& out,
        DefaultImageType::PixelType& pixel )
{
  /*
  qDebug()
    << this << "::Pick("
    << in[ 0 ] << "," << in[ 1 ]
    << ")";
  */

  otb::GlImageActor::Pointer glImageActor(
    GetReferenceActor< otb::GlImageActor >()
  );

  if( glImageActor.IsNull() )
    return false;

  //
  // Compute output/physical point.
  out = glImageActor->ViewportToImageTransform( in, true );

  //
  // Read pixel value.
  return glImageActor->GetPixelFromViewport( in, pixel );
}

/*****************************************************************************/
bool
ImageViewRenderer
::Transform( PointType& point, const IndexType& index, bool isPhysical ) const
{
  otb::GlImageActor::ConstPointer glImageActor(
    GetReferenceActor< otb::GlImageActor >()
  );

  if( glImageActor.IsNull() )
    return false;

  point[ 0 ] = static_cast< double >( index[ 0 ] );
  point[ 1 ] = static_cast< double >( index[ 1 ] );

  point = glImageActor->ImageToViewportTransform( point, isPhysical );

  return true;
}

/*****************************************************************************/
void
ImageViewRenderer
::UpdateActors( const AbstractImageViewRenderer::RenderingContext * )
{
  assert( !m_GlView.IsNull() );


  StackedLayerModel * stackedLayerModel = GetLayerStack();
  assert( stackedLayerModel!=NULL );

  otb::GlImageActor::Pointer refImageActor(
    GetReferenceActor< otb::GlImageActor >()
  );


  for( StackedLayerModel::ConstIterator it( stackedLayerModel->Begin() );
       it!=stackedLayerModel->End();
       ++it )
    {
    assert( !it->first.empty() );
    assert( it->second!=NULL );

    if( m_GlView->ContainsActor( it->first ) )
      {
      if( it->second->inherits( VectorImageModel::staticMetaObject.className() ) )
        {
        //
        // Get vector image-model.
        VectorImageModel * vectorImageModel =
          dynamic_cast< VectorImageModel * >( it->second );

        assert( vectorImageModel!=NULL );

        //
        // Get vector image-model data.
        const VectorImageModel::Settings & settings =
          vectorImageModel->GetSettings();

        const ImageProperties * properties = 
          vectorImageModel->GetProperties();

        //
        // Get GL image-actor.
        assert(
          m_GlView->GetActor( it->first )==
          otb::DynamicCast< otb::GlImageActor >(
            m_GlView->GetActor( it->first )
          )
        );

        otb::GlImageActor::Pointer imageActor(
          otb::DynamicCast< otb::GlImageActor >(
            m_GlView->GetActor( it->first )
          )
        );
        assert( !imageActor.IsNull() );


        //
        // Apply color-setup.
        VectorImageModel::Settings::ChannelVector channels;

        settings.GetSmartChannels( channels );

        imageActor->SetRedIdx( channels[ RGBW_CHANNEL_RED ] + 1 );
        imageActor->SetGreenIdx( channels[ RGBW_CHANNEL_GREEN ] + 1 );
        imageActor->SetBlueIdx( channels[ RGBW_CHANNEL_BLUE ] + 1 );

        //
        // Apply color-dynamics.
        //
        // Must use local variable to cast from T* to T::Pointer because
        // of ITK set/get macros...
        otb::FragmentShader::Pointer fragmentShader( imageActor->GetShader() );
        assert(
          fragmentShader==otb::DynamicCast< otb::StandardShader >( fragmentShader )
        );

        otb::StandardShader::Pointer shader(
          otb::DynamicCast< otb::StandardShader >( fragmentShader )
        );

        assert( !shader.IsNull() );

        if( settings.IsGrayscaleActivated() )
          {
          shader->SetMinRed( settings.GetLowIntensity( RGBW_CHANNEL_WHITE ) );
          shader->SetMaxRed( settings.GetHighIntensity(RGBW_CHANNEL_WHITE  ) );
      
          shader->SetMinGreen( settings.GetLowIntensity( RGBW_CHANNEL_WHITE ) );
          shader->SetMaxGreen( settings.GetHighIntensity(RGBW_CHANNEL_WHITE ) );
      
          shader->SetMinBlue( settings.GetLowIntensity( RGBW_CHANNEL_WHITE ) );
          shader->SetMaxBlue( settings.GetHighIntensity(RGBW_CHANNEL_WHITE ) );
          }
        else
          {
          shader->SetMinRed( settings.GetLowIntensity( RGBW_CHANNEL_RED ) );
          shader->SetMaxRed( settings.GetHighIntensity(RGBW_CHANNEL_RED  ) );
      
          shader->SetMinGreen( settings.GetLowIntensity( RGBW_CHANNEL_GREEN ) );
          shader->SetMaxGreen( settings.GetHighIntensity(RGBW_CHANNEL_GREEN ) );
      
          shader->SetMinBlue( settings.GetLowIntensity( RGBW_CHANNEL_BLUE ) );
          shader->SetMaxBlue( settings.GetHighIntensity(RGBW_CHANNEL_BLUE ) );
          }

        shader->SetGamma( settings.GetGamma() );

        if( properties==NULL )
          shader->SetUseNoData( false );

        else
          {
          shader->SetUseNoData( properties->IsNoDataEnabled() );
          shader->SetNoData( properties->GetNoData() );
          }

        // If reference actor
        if( refImageActor.IsNotNull() &&
            refImageActor==imageActor )
          {
          // TODO: Replace std::string by otb::ShaderType and if/else sequence by switch/case statement.
          // TODO: Move following code into private sub-function.
          if( m_ReferenceActorShaderMode=="LOCAL_CONTRAST" )
            {
            shader->SetShaderType( otb::SHADER_LOCAL_CONTRAST );

            shader->SetLocalContrastRange(
              0.1 * std::max(
                std::max(
                  shader->GetMaxRed() - shader->GetMinRed(),
                  shader->GetMaxGreen() - shader->GetMinGreen()
                ),
                shader->GetMaxBlue() - shader->GetMinBlue()
              )
            );
            }
          else
            shader->SetShaderType( otb::SHADER_STANDARD );
          }
        }
      //
      else
        {
        qDebug()
          << __FILE__ << ":" << __LINE__
          << ": Unhandled AbstractLayerModel derived class.";
        }
      }
    }

  m_GlView->SetRenderingOrder( stackedLayerModel->GetKeys() );
}

/*******************************************************************************/
/*
void
ImageViewRenderer
::virtual_ClearScene()
{
  assert( !m_GlView.IsNull() );

  // qDebug() << this << "::virtual_CleraScene()";

  //
  // Remove all actors.
  m_GlView->ClearActors();

  // Forget image-model/actors relationships.
  m_ImageModelActorPairs.clear();

  // Forget reference instances.
  m_ReferenceImageModel = NULL;
  m_ReferenceGlImageActor = otb::GlImageActor::Pointer();
}
*/

/*******************************************************************************/
void
ImageViewRenderer
// ::virtual_SetLayerStack( StackedLayerModel * stackedLayerModel )
::virtual_UpdateScene()
{
  assert( !m_GlView.IsNull() );

  StackedLayerModel * stackedLayerModel = GetLayerStack();


  if( stackedLayerModel==NULL )
    {
    m_GlView->ClearActors();

    m_ReferencePair.first = NULL;
    m_ReferencePair.second = otb::GlActor::Pointer();

    return;
    }


  {
  otb::GlView::StringVectorType keys( m_GlView->GetActorsKeys() );

  for( otb::GlView::StringVectorType::const_iterator it( keys.begin() );
       it!=keys.end();
       ++it )
    if( !stackedLayerModel->Contains( *it ) )
      m_GlView->RemoveActor( *it );
  }


#if USE_REMOTE_DESKTOP_DISABLED_RENDERING
#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING

  for( StackedLayerModel::ConstIterator it( stackedLayerModel->Begin() );
       it!=stackedLayerModel->End();
       ++it )
    if( !m_GlView->ContainsActor( it->first ) )
      {
      assert( it->second!=NULL );

      if( it->second->inherits( VectorImageModel::staticMetaObject.className() ) )
        {
        otb::GlImageActor::Pointer glImageActor( otb::GlImageActor::New() );

        // Should all AbstractLayerModel have a ::GetFilename()
        // method?
        // -> Not sure: AbstractImageModel coud be derived as a
        // in-memory image-model.
        VectorImageModel * vectorImageModel =
          dynamic_cast< VectorImageModel * >( it->second );

        glImageActor->Initialize(
          ToStdString(
            vectorImageModel->GetFilename()
          )
        );

        m_GlView->AddActor( glImageActor, it->first );

        glImageActor->SetVisible( true );

        qDebug()
          << "Added image-actor" << FromStdString( it->first )
          << "from file" << vectorImageModel->GetFilename();
        }
      else
        {
        }
      }

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING

  //
  // Remember first vector image-model as reference image-model.
  m_ReferencePair.first = stackedLayerModel->GetReference();

#if USE_REMOTE_DESKTOP_DISABLED_RENDERING
  m_ReferencePair.second = otb::GlActor::Pointer();

#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING

  //
  // Remember first actor as reference actor.
  otb::GlView::StringVectorType keys( m_GlView->GetRenderingOrder() );

  if( keys.empty() )
    m_ReferencePair.second = otb::GlActor::Pointer();

  else
    {
    otb::GlActor::Pointer glActor( m_GlView->GetActor( keys.front() ) );
    assert( !glActor.IsNull() );

    assert( glActor==otb::DynamicCast< otb::GlImageActor >( glActor ) );
    m_ReferencePair.second = otb::DynamicCast< otb::GlImageActor >( glActor );
    assert( !m_ReferencePair.second.IsNull() );
    }

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING
}

/*******************************************************************************/
/*
void
ImageViewRenderer
::virtual_SetImageList( const VectorImageModelList & images )
{
  assert( !m_GlView.IsNull() );

  // qDebug() << this << "::virtual_SetImageList(" << images << ")";

  //
  // Check that references are clear.
  assert( m_ImageModelActorPairs.empty() );
  assert( m_ReferenceImageModel==NULL );
  assert( m_ReferenceGlImageActor.IsNull() );

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

#if USE_REMOTE_DESKTOP_DISABLED_RENDERING

    ImageModelActorPair pair(
      *it,
      otb::GlImageActor::Pointer()
    );

#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING

    ImageModelActorPair pair(
      *it,
      otb::GlImageActor::New()
    );

    pair.second->Initialize( ToStdString( pair.first->GetFilename() ) );

    DatasetModel* datasetModel = pair.first->GetDatasetModel();

    ActorKey actorKey(
      m_GlView->AddActor(
        pair.second,
        pair.first==NULL
        ? std::string()
        : ToStdString( datasetModel->GetHash() )
      )
    );

    pair.second->SetVisible( true );

    m_ImageModelActorPairs.insert(
      ImageModelActorPairMap::value_type( actorKey, pair )
    );

    qDebug() << "Added image-actor:" << FromStdString( actorKey );

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING
    }

  //
  // Remember first vector image-model as reference image-model.
  m_ReferenceImageModel = images.front();

#if USE_REMOTE_DESKTOP_DISABLED_RENDERING
#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING

  //
  // Remember first actor as reference actor.
  otb::GlView::StringVectorType keys( m_GlView->GetRenderingOrder() );
  assert( !keys.empty() );

  otb::GlActor::Pointer glActor( m_GlView->GetActor( keys.front() ) );
  assert( !glActor.IsNull() );

  assert( glActor==otb::DynamicCast< otb::GlImageActor >( glActor ) );
  m_ReferenceGlImageActor = otb::DynamicCast< otb::GlImageActor >( glActor );
  assert( !m_ReferenceGlImageActor.IsNull() );

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING
}
*/


/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
ImageViewRenderer
::OnPhysicalCursorPositionChanged(const PointType& point,
                                  const DefaultImageType::PixelType& pixel)
{
  otb::GlImageActor::Pointer glImageActor(
    GetReferenceActor< otb::GlImageActor >()
  );

  if( glImageActor.IsNull() )
    return;

  // Get shader of reference actor
  otb::FragmentShader::Pointer fragmentShader( glImageActor->GetShader() );
    
  assert(
    fragmentShader==otb::DynamicCast< otb::StandardShader >( fragmentShader )
  );
    
  otb::StandardShader::Pointer shader(
    otb::DynamicCast< otb::StandardShader >( fragmentShader )
  );
    
  assert( !shader.IsNull() );
    
  shader->SetShaderType(otb::SHADER_LOCAL_CONTRAST);

  if(pixel.Size()>0)
    {
    shader->SetCurrentRed(pixel[0]);
    shader->SetCurrentGreen(pixel[1]);
    shader->SetCurrentBlue(pixel[2]);
    }

  PointType p, pscreen;
  p = glImageActor->ImageToViewportTransform(point,true);

  m_GlView->GetSettings()->ViewportToScreenTransform(p[0],p[1], pscreen[0], pscreen[1]);

  pscreen[1] = m_GlView->GetSettings()->GetViewportSize()[1] - pscreen[1];

  shader->SetCenter(pscreen);
}

/******************************************************************************/
void
ImageViewRenderer
::OnReferenceActorShaderModeChanged(const std::string & mode)
{
  m_ReferenceActorShaderMode = mode;
}

/******************************************************************************/
} // end namespace 'mvd'
