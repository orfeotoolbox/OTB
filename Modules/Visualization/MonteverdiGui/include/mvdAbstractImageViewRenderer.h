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


#ifndef mvdAbstractImageViewRenderer_h
#define mvdAbstractImageViewRenderer_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


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
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdStackedLayerModel.h"

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

//
// Internal classes pre-declaration.
class AbstractLayerModel;
class StackedLayerModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class AbstractImageViewRenderer
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief TODO.
 */
class OTBMonteverdiGUI_EXPORT AbstractImageViewRenderer : public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public types.
public:
  /**
   */
  struct RenderingContext
  {
    /**
     */
    inline RenderingContext() : m_RenderMode(RENDER_MODE_FULL)
    {
    }

    /**
     */
    virtual ~RenderingContext()
    {
    }

    /**
     */
    enum RenderMode
    {
      RENDER_MODE_LIGHT = 0,
      RENDER_MODE_FULL
    };

    RenderMode m_RenderMode;
  };

  //
  // Public methods.
public:
  /**
   * Destructor.
   */
  ~AbstractImageViewRenderer() override{};

  /**
   */
  inline void SetLayerStack(StackedLayerModel*);

  /**
   */
  inline const StackedLayerModel* GetLayerStack() const;

  /**
   */
  inline StackedLayerModel* GetLayerStack();

  /**
   */
  virtual bool CheckGLCapabilities( int * ) const =0;

  template <typename T>
  const T* GetReferenceModel() const;

  template <typename T>
  T* GetReferenceModel();

  /**
   */
  virtual const AbstractLayerModel* GetReferenceModel() const = 0;

  /**
   */
  virtual AbstractLayerModel* GetReferenceModel() = 0;

  /**
   */
  virtual void GetLayerExtent(const StackedLayerModel::KeyType& key, PointType& origin, PointType& extent) const = 0;

  /**
   */
  virtual void GetReferenceExtent(PointType& origin, PointType& extent) const = 0;

  /**
   */
  virtual void GetViewExtent(PointType& origin, PointType& extent) const = 0;

  /**
   */
  virtual bool GetLayerDynamics(const StackedLayerModel::KeyType& key, ParametersType& params, bool isGlobal) const = 0;

  /**
   */
  virtual AbstractImageViewRenderer::RenderingContext* NewRenderingContext() const = 0;

  /**
   */
  virtual void InitializeGL() = 0;

  /**
   */
  virtual void ResizeGL(int width, int height) = 0;

  /**
   */
  virtual void PaintGL(RenderingContext const* context) = 0;

  /**
   */
  virtual void Pick(const PointType& view, PixelInfo::Vector& pixels) const = 0;

  /**
   */
  virtual void GetResolutions(PixelInfo::Vector& pixels) const = 0;

  /**
   */
  virtual bool TransformToView(PointType& point, const StackedLayerModel::KeyType&, const IndexType&, bool isPhysical) const = 0;

  /**
   */
  virtual void SaveScreenshot(const QString&) const = 0;

  /**
   */
  inline bool SetBypassRenderingEnabled(bool);
  /**
   */
  inline bool IsBypassRenderingEnabled() const;
  /**
   */
  virtual bool IsGLSLAvailable() const noexcept = 0;
  /**
   */
  virtual bool SetGLSLEnabled( bool ) = 0;
  /**
   */
  virtual bool IsGLSLEnabled() const noexcept = 0;
  /**
   */
  virtual bool IsEffectsEnabled() const = 0;
  /**
   */
  inline bool ZoomToRegion(const PointType& origin, const PointType& extent, PointType& center, SpacingType& spacing) const;
  /**
   */
  inline bool ZoomToExtent(PointType& center, SpacingType& spacing) const;
  /**
   */
  inline bool ZoomToLayer(const StackedLayerModel::KeyType& key, PointType& center, SpacingType& spacing) const;
  /**
   */
  inline bool ZoomToFull(const StackedLayerModel::KeyType& key, PointType& center, SpacingType& spacing) const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  // public slots
public Q_SLOTS:
  inline void ClearScene(bool keepViewport = false);
  inline void UpdateScene();
  inline void RefreshScene();

  virtual void UpdatePixelInfo(const QPoint& point, const PointType& view, const PixelInfo::Vector& pixels) = 0;

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // SIGNALS.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /**
   * Constructor.
   */
  AbstractImageViewRenderer( QObject* p = NULL ) :
    QObject( p ),
    m_StackedLayerModel( NULL ),
    m_BypassRenderingEnabled( false )
  {
  }

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  /**
   */
  virtual void virtual_ClearScene(bool){};
  /**
   */
  virtual void virtual_PrepareScene(){};
  /**
   */
  virtual void virtual_UpdateScene(){};
  /**
   */
  virtual void virtual_RefreshScene(){};
  /**
   */
  // virtual void virtual_SetLayerStack( const StackedLayerModel & ) =0;
  /**
   */
  virtual void virtual_FinishScene(){};

  /**
   */
  virtual bool virtual_ZoomToRegion( const PointType &,
				     const PointType &,
				     PointType &,
				     SpacingType & ) const
  { return false; }

  /**
   */
  virtual bool virtual_ZoomToExtent( PointType &, SpacingType & ) const
  { return false; }

  /**
   */
  virtual bool virtual_ZoomToLayer( const StackedLayerModel::KeyType &,
				    PointType &,
				    SpacingType & ) const
  { return false; }

  /**
   */
  virtual bool virtual_ZoomToFull(const StackedLayerModel::KeyType&, PointType&, SpacingType&) const
  {
    return false;
  }

  //
  // Private attributes.
private:
  /**
   */
  StackedLayerModel* m_StackedLayerModel;
  /**
   */
  bool m_BypassRenderingEnabled : 1;
  /**
   */

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // SLOTS.
private Q_SLOTS:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
template <typename T>
const T* AbstractImageViewRenderer::GetReferenceModel() const
{
  return dynamic_cast<const T*>(GetReferenceModel());
}

/*****************************************************************************/
template <typename T>
T* AbstractImageViewRenderer::GetReferenceModel()
{
  return dynamic_cast<T*>(GetReferenceModel());
}

/*****************************************************************************/
inline StackedLayerModel* AbstractImageViewRenderer::GetLayerStack()
{
  return m_StackedLayerModel;
}

/*****************************************************************************/
inline const StackedLayerModel* AbstractImageViewRenderer::GetLayerStack() const
{
  return m_StackedLayerModel;
}

/*****************************************************************************/
inline void AbstractImageViewRenderer::SetLayerStack(StackedLayerModel* stackedLayerModel)
{
  virtual_PrepareScene();

  m_StackedLayerModel = stackedLayerModel;

  virtual_UpdateScene();

  virtual_FinishScene();
}

/*****************************************************************************/
inline bool AbstractImageViewRenderer::IsBypassRenderingEnabled() const
{
  return m_BypassRenderingEnabled;
}

/*****************************************************************************/
inline bool AbstractImageViewRenderer::SetBypassRenderingEnabled(bool isEnabled)
{
  bool bypass = m_BypassRenderingEnabled;

  m_BypassRenderingEnabled = isEnabled;

  return bypass;
}

/*****************************************************************************/
inline void AbstractImageViewRenderer::ClearScene(bool keepViewport)
{
  virtual_ClearScene(keepViewport);
}

/*****************************************************************************/
inline void AbstractImageViewRenderer::UpdateScene()
{
  virtual_PrepareScene();

  virtual_UpdateScene();

  virtual_FinishScene();
}

/*****************************************************************************/
inline void AbstractImageViewRenderer::RefreshScene()
{
  virtual_RefreshScene();
}

/*****************************************************************************/
inline bool AbstractImageViewRenderer::ZoomToExtent(PointType& center, SpacingType& spacing) const
{
  return virtual_ZoomToExtent(center, spacing);
}

/*****************************************************************************/
inline bool AbstractImageViewRenderer::ZoomToLayer(const StackedLayerModel::KeyType& key, PointType& center, SpacingType& spacing) const
{
  return virtual_ZoomToLayer(key, center, spacing);
}

/*****************************************************************************/
inline bool AbstractImageViewRenderer::ZoomToRegion(const PointType& origin, const PointType& extent, PointType& center, SpacingType& spacing) const
{
  return virtual_ZoomToRegion(origin, extent, center, spacing);
}

/*****************************************************************************/
inline bool AbstractImageViewRenderer::ZoomToFull(const StackedLayerModel::KeyType& key, PointType& center, SpacingType& spacing) const
{
  return virtual_ZoomToFull(key, center, spacing);
}

} // end namespace 'mvd'

#endif // mvdAbstractImageViewRenderer_h
