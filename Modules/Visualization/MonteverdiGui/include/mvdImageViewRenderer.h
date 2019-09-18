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


#ifndef mvdImageViewRenderer_h
#define mvdImageViewRenderer_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

//
#define USE_VIEW_SETTINGS_SIDE_EFFECT 1
//
#define USE_REMOTE_DESKTOP_DISABLED_RENDERING 0
//
#define DISABLE_QUICKLOOK_VIEW 0

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
class OTBMonteverdiGUI_EXPORT ImageViewRenderer : public AbstractImageViewRenderer
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public types.
public:
  /**
   */
  struct RenderingContext : public AbstractImageViewRenderer::RenderingContext
  {
    /**
     */
    RenderingContext();

    /**
     */
    ~RenderingContext() override;

    /**
     */
    Resolution m_Resolution;

    /**
     */
    int m_TileSize;

#if USE_VIEW_SETTINGS_SIDE_EFFECT
#else  // USE_VIEW_SETTINGS_SIDE_EFFECT
    /**
     */
    otb::ViewSettings::Pointer m_ViewSettings;
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT
  };

  //
  // Public methods.
public:
  /** Constructor */
  ImageViewRenderer(QObject* p = NULL);

  /** Destructor */
  ~ImageViewRenderer() override;

  /**
   */
  bool CheckGLCapabilities(int*) override;

  /**
   */
  inline const otb::ViewSettings::Pointer GetViewSettings() const;
  /**
   */
  inline otb::ViewSettings::Pointer GetViewSettings();

  //
  // AbstractImageViewRenderer overloads.

  bool GetLayerDynamics(const StackedLayerModel::KeyType& key, ParametersType& params, bool isGlobal) const override;

  const AbstractLayerModel* GetReferenceModel() const override;

  AbstractLayerModel* GetReferenceModel() override;

  void GetLayerExtent(const StackedLayerModel::KeyType& key, PointType& origin, PointType& extent) const override;

  void GetReferenceExtent(PointType& origin, PointType& extent) const override;

  void GetViewExtent(PointType& origin, PointType& extent) const override;


  AbstractImageViewRenderer::RenderingContext* NewRenderingContext() const override;

  void InitializeGL() override;

  void ResizeGL(int width, int height) override;


  void PaintGL(const AbstractImageViewRenderer::RenderingContext* context) override;

  void Pick(const PointType& view, PixelInfo::Vector& pixels) const override;

  void GetResolutions(PixelInfo::Vector& pixels) const override;

  bool TransformToView(PointType& point, const StackedLayerModel::KeyType&, const IndexType&, bool isPhysical) const override;

  void SaveScreenshot(const QString&) const override;

  bool Reproject(PointType& center, SpacingType& spacing, const PointType& vcenter, const SpacingType& vspacing) const;

  bool IsEffectsEnabled() const override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  // public slots
public slots:
  void UpdatePixelInfo(const QPoint& screen, const PointType& view, const PixelInfo::Vector& pixels) override;

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
  template <typename T>
  inline typename T::Pointer GetReferenceActor();
  /**
   */
  template <typename T>
  inline typename T::ConstPointer GetReferenceActor() const;
  /**
   */
  virtual void UpdateActors(const AbstractImageViewRenderer::RenderingContext*);

  //
  // Protected attributes.
protected:
  /**
   */
  otb::GlView::Pointer m_GlView;
  /**
   */
  bool m_EffectsEnabled : 1;

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private types
private:
  /**
   */
  typedef std::pair<AbstractLayerModel*, otb::GlActor::Pointer> ModelActorPair;

  //
  // Private methods.
private:
  virtual void virtual_ClearProjection(){};
  virtual void virtual_SetProjection(){};
  virtual void virtual_UpdateProjection(){};

  //
  // AbstractImageViewRenderer overloads.

  void virtual_ClearScene(bool) override;
  void virtual_UpdateScene() override;
  void virtual_RefreshScene() override;

  bool virtual_ZoomToRegion(const PointType& origin, const PointType& extent, PointType& center, SpacingType& spacing) const override;

  bool virtual_ZoomToExtent(PointType& center, SpacingType& spacing) const override;

  bool virtual_ZoomToLayer(const StackedLayerModel::KeyType& key, PointType& center, SpacingType& spacing) const override;

  bool virtual_ZoomToFull(const StackedLayerModel::KeyType& key, PointType& center, SpacingType& spacing) const override;

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
inline const otb::ViewSettings::Pointer ImageViewRenderer::GetViewSettings() const
{
  assert(!m_GlView.IsNull());

  return m_GlView->GetSettings();
}

/*****************************************************************************/
inline otb::ViewSettings::Pointer ImageViewRenderer::GetViewSettings()
{
  assert(!m_GlView.IsNull());

  return m_GlView->GetSettings();
}

/*****************************************************************************/
inline otb::GlActor::Pointer ImageViewRenderer::GetReferenceActor()
{
  return m_ReferencePair.second;
}

/*****************************************************************************/
template <typename T>
inline typename T::Pointer ImageViewRenderer::GetReferenceActor()
{
  return otb::DynamicCast<T>(m_ReferencePair.second);
}

/*****************************************************************************/
template <typename T>
inline typename T::ConstPointer ImageViewRenderer::GetReferenceActor() const
{
  return otb::DynamicCast<const T>(m_ReferencePair.second);
}

} // end namespace 'mvd'

#endif // mvdImageViewRenderer_h
