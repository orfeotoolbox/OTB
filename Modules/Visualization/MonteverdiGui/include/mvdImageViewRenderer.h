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

#ifndef __mvdImageViewRenderer_h
#define __mvdImageViewRenderer_h

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
class ITK_EXPORT ImageViewRenderer :
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
    virtual ~RenderingContext();

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
  virtual ~ImageViewRenderer();

  /**
   */
  virtual bool CheckGLCapabilities( int * );

  /**
   */
  inline const otb::ViewSettings::Pointer GetViewSettings() const;
  /**
   */
  inline otb::ViewSettings::Pointer GetViewSettings();

  //
  // AbstractImageViewRenderer overloads.

  virtual bool GetLayerDynamics( const StackedLayerModel::KeyType & key,
				 ParametersType & params,
				 bool isGlobal ) const;

  virtual const AbstractLayerModel* GetReferenceModel() const;

  virtual AbstractLayerModel * GetReferenceModel();

  virtual void GetLayerExtent( const StackedLayerModel::KeyType & key,
                               PointType& origin,
                               PointType& extent ) const;

  virtual void GetReferenceExtent( PointType& origin,
                                   PointType& extent ) const;

  virtual void GetViewExtent( PointType& origin,
                              PointType& extent ) const;

  virtual
  AbstractImageViewRenderer::RenderingContext* NewRenderingContext() const;

  virtual void InitializeGL();

  virtual void ResizeGL( int width, int height );

  virtual
  void PaintGL( const AbstractImageViewRenderer::RenderingContext* context );

  virtual void Pick( const PointType & view,
                     PixelInfo::Vector & pixels ) const;

  virtual void GetResolutions( PixelInfo::Vector & pixels ) const;

  virtual bool TransformToView( PointType & point,
				const StackedLayerModel::KeyType &,
				const IndexType &,
				bool isPhysical ) const;

  virtual void SaveScreenshot( const QString & ) const;

  bool
    Reproject( PointType & center,
               SpacingType & spacing,
               const PointType & vcenter,
               const SpacingType & vspacing ) const;

  virtual bool IsEffectsEnabled() const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

// public slots
public slots:
  virtual void UpdatePixelInfo( const QPoint & screen,
				const PointType & view,
				const PixelInfo::Vector & pixels );

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

  virtual void virtual_UpdateScene();
  virtual void virtual_RefreshScene();

  virtual bool virtual_ZoomToRegion( const PointType & origin,
				     const PointType & extent,
				     PointType & center,
				     SpacingType & spacing ) const;

  virtual bool virtual_ZoomToExtent( PointType & center, SpacingType & spacing ) const;

  virtual bool virtual_ZoomToLayer( const StackedLayerModel::KeyType & key,
				    PointType & center,
				    SpacingType & spacing ) const;

  virtual bool virtual_ZoomToFull( const StackedLayerModel::KeyType & key,
				   PointType & center,
				   SpacingType & spacing ) const;

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

#endif // __mvdImageViewRenderer_h
