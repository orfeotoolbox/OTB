/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef mvdImageViewRenderer_h
#define mvdImageViewRenderer_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

//
#define USE_VIEW_SETTINGS_SIDE_EFFECT 1
//
#define USE_REMOTE_DESKTOP_DISABLED_RENDERING ((defined( OTB_DEBUG ) && 0) || 0)
//
#define DISABLE_QUICKLOOK_VIEW ( ( defined( OTB_DEBUG ) && 0 ) || 0 )

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbGlActor.h"
#include "otbGlView.h"
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractImageViewRenderer.h"
#include "mvdGui.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace otb
{
}

namespace mvd
{
//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ImageViewRenderer
 *
 * \ingroup OTBMonteverdiGUI
 */
class OTBMonteverdiGUI_EXPORT ImageViewRenderer :
    public AbstractImageViewRenderer
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:

  /**
   */
  struct RenderingContext :
    public AbstractImageViewRenderer::RenderingContext
  {
    /**
     */
    RenderingContext();

    /**
     */
    ~RenderingContext() ITK_OVERRIDE;

    /**
     */
    Resolution m_Resolution;

    /**
     */
    int m_TileSize;

#if USE_VIEW_SETTINGS_SIDE_EFFECT
#else // USE_VIEW_SETTINGS_SIDE_EFFECT
    /**
     */
    otb::ViewSettings::Pointer m_ViewSettings;
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT
  };

//
// Public methods.
public:
  /** Constructor */
  ImageViewRenderer( QObject* p = NULL );

  /** Destructor */
  ~ImageViewRenderer() ITK_OVERRIDE;

  /**
   */
  bool CheckGLCapabilities( int * ) ITK_OVERRIDE;

  /**
   */
  inline const otb::ViewSettings::Pointer GetViewSettings() const;
  /**
   */
  inline otb::ViewSettings::Pointer GetViewSettings();

  //
  // AbstractImageViewRenderer overloads.

  bool GetLayerDynamics( const StackedLayerModel::KeyType & key,
				 ParametersType & params,
				 bool isGlobal ) const ITK_OVERRIDE;

  const AbstractLayerModel* GetReferenceModel() const ITK_OVERRIDE;

  AbstractLayerModel * GetReferenceModel() ITK_OVERRIDE;

  void GetLayerExtent( const StackedLayerModel::KeyType & key,
                               PointType& origin,
                               PointType& extent ) const ITK_OVERRIDE;

  void GetReferenceExtent( PointType& origin,
                                   PointType& extent ) const ITK_OVERRIDE;

  void GetViewExtent( PointType& origin,
                              PointType& extent ) const ITK_OVERRIDE;

  
  AbstractImageViewRenderer::RenderingContext* NewRenderingContext() const ITK_OVERRIDE;

  void InitializeGL() ITK_OVERRIDE;

  void ResizeGL( int width, int height ) ITK_OVERRIDE;

  
  void PaintGL( const AbstractImageViewRenderer::RenderingContext* context ) ITK_OVERRIDE;

  void Pick( const PointType & view,
                     PixelInfo::Vector & pixels ) const ITK_OVERRIDE;

  void GetResolutions( PixelInfo::Vector & pixels ) const ITK_OVERRIDE;

  bool TransformToView( PointType & point,
				const StackedLayerModel::KeyType &,
				const IndexType &,
				bool isPhysical ) const ITK_OVERRIDE;

  void SaveScreenshot( const QString & ) const ITK_OVERRIDE;

  bool
    Reproject( PointType & center,
               SpacingType & spacing,
               const PointType & vcenter,
               const SpacingType & vspacing ) const;

  bool IsEffectsEnabled() const ITK_OVERRIDE;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

// public slots
public slots:
  void UpdatePixelInfo( const QPoint & screen,
				const PointType & view,
				const PixelInfo::Vector & pixels ) ITK_OVERRIDE;

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:
  void ClearProjectionRequired();
  void SetProjectionRequired();
  void UpdateProjectionRequired();
  void ResetViewport();

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
  /**
   */
  inline otb::GlActor::Pointer GetReferenceActor();
  /**
   */
  template< typename T >
    inline typename T::Pointer GetReferenceActor();
  /**
   */
  template< typename T >
    inline typename T::ConstPointer GetReferenceActor() const;
  /**
   */
  virtual
  void UpdateActors( const AbstractImageViewRenderer::RenderingContext * );

//
// Protected attributes.
protected:
  /**
   */
  otb::GlView::Pointer m_GlView;
  /**
   */
  bool m_EffectsEnabled: 1;

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types
private:
  /**
   */
  typedef
    std::pair< AbstractLayerModel *, otb::GlActor::Pointer >
    ModelActorPair;

//
// Private methods.
private:

  virtual void virtual_ClearProjection() {};
  virtual void virtual_SetProjection() {};
  virtual void virtual_UpdateProjection() {};

  //
  // AbstractImageViewRenderer overloads.

  void virtual_ClearScene( bool ) ITK_OVERRIDE;
  void virtual_UpdateScene() ITK_OVERRIDE;
  void virtual_RefreshScene() ITK_OVERRIDE;

  bool virtual_ZoomToRegion( const PointType & origin,
				     const PointType & extent,
				     PointType & center,
				     SpacingType & spacing ) const ITK_OVERRIDE;

  bool virtual_ZoomToExtent( PointType & center, SpacingType & spacing ) const ITK_OVERRIDE;

  bool virtual_ZoomToLayer( const StackedLayerModel::KeyType & key,
				    PointType & center,
				    SpacingType & spacing ) const ITK_OVERRIDE;

  bool virtual_ZoomToFull( const StackedLayerModel::KeyType & key,
				   PointType & center,
				   SpacingType & spacing ) const ITK_OVERRIDE;

//
// Private attributes.
private:
  /**
   */
  ModelActorPair m_ReferencePair;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
inline
const otb::ViewSettings::Pointer
ImageViewRenderer
::GetViewSettings() const
{
  assert( !m_GlView.IsNull() );

  return m_GlView->GetSettings();
}

/*****************************************************************************/
inline
otb::ViewSettings::Pointer
ImageViewRenderer
::GetViewSettings()
{
  assert( !m_GlView.IsNull() );

  return m_GlView->GetSettings();
}

/*****************************************************************************/
inline
otb::GlActor::Pointer
ImageViewRenderer
::GetReferenceActor()
{
  return m_ReferencePair.second;
}

/*****************************************************************************/
template< typename T >
inline
typename T::Pointer
ImageViewRenderer
::GetReferenceActor()
{
  return otb::DynamicCast< T >( m_ReferencePair.second );
}

/*****************************************************************************/
template< typename T >
inline
typename T::ConstPointer
ImageViewRenderer
::GetReferenceActor() const
{
  return otb::DynamicCast< const T >( m_ReferencePair.second );
}

} // end namespace 'mvd'

#endif // mvdImageViewRenderer_h
