/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef mvdImageViewManipulator_h
#define mvdImageViewManipulator_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cmath>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbViewSettings.h"
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdCore.h"
#include "mvdAbstractImageViewManipulator.h"
#include "mvdImageViewRenderer.h"


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


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ImageViewManipulator
 *
 * \ingroup OTBMonteverdiGUI
 *
 *  \brief TODO.
 */
class OTBMonteverdiGUI_EXPORT ImageViewManipulator : public AbstractImageViewManipulator
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Constants
public:
  /**
   */
  static const double DEFAULT_DELTA;
  /**
   */
  static const int DEFAULT_GRANULARITY;
  /**
   */
  static const int DEFAULT_ALPHA_GRANULARITY;
  /**
   */
  static const double DEFAULT_DYNAMICS_SHIFT_GRANULARITY;
  /**
   */
  static const int DEFAULT_SCROLL_GRANULARITY;
  /**
   */
  static const int DEFAULT_ZOOM_GRANULARITY;

  //
  // Public methods.
public:
/** \brief Constructor. */
#if USE_VIEW_SETTINGS_SIDE_EFFECT
  ImageViewManipulator(const otb::ViewSettings::Pointer& viewSettings, QObject* p = NULL);
#else  // USE_VIEW_SETTINGS_SIDE_EFFECT
  ImageViewManipulator(QObject* p = NULL);
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT

  /** \brief Destructor. */
  ~ImageViewManipulator() override;

  //
  // AbstractImageViewManipulator overloads.
  //

  //
  // Accessors.

  void SetViewportSize(int width, int height) override;

  SizeType GetViewportSize() const override;

  void SetOrigin(const PointType& origin) override;

  PointType GetOrigin() const override;

  void SetSpacing(const SpacingType& spacing) override;

  SpacingType GetSpacing() const override;

  void SetNativeSpacing(const SpacingType&) override;

  void SetWkt(const std::string& wkt) override;

  void SetKeywordList(const DefaultImageType::ImageKeywordlistType& kwl) override;

  PointType GetCenter() const override;

  ZoomType GetFixedZoomType() const override;

  //
  // Controls.


  void SetupRenderingContext(AbstractImageViewRenderer::RenderingContext* const) const override;

  void ZoomIn() override;

  void ZoomOut() override;

  const PointType& Transform(PointType&, const QPoint&) const override;

  void ResetViewport() override;

  //
  // Events.

  void MouseMoveEvent(QMouseEvent* event) override;

  void MousePressEvent(QMouseEvent* event) override;

  void MouseReleaseEvent(QMouseEvent* event) override;

  void MouseDoubleClickEvent(QMouseEvent*) override;

  void WheelEvent(QWheelEvent* event) override;

  void ResizeEvent(QResizeEvent* event) override;

  void KeyPressEvent(QKeyEvent* event) override;

  void KeyReleaseEvent(QKeyEvent* event) override;

  /*-[ PUBLIC SLOTS SECTION ]-----------------------------------------------**/

  //
  // Public SLOTS.
public slots:

  //
  // AbstractImageViewManipulator overloads.

  void CenterOn(const PointType& point) override;

  void ZoomTo(double scale) override;

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
signals:
  /**
   */
  void RaiseLayerRequested();
  /**
   */
  void LowerLayerRequested();
  /**
   */
  void LayerToTopRequested();
  /**
   */
  void LayerToBottomRequested();
  /**
   */
  void SelectNextLayerRequested();
  /**
   */
  void SelectPreviousLayerRequested();
  /**
   */
  void SelectFirstLayerRequested();
  /**
   */
  void SelectLastLayerRequested();
  /**
   */
  void DeleteAllRequested();
  /**
   */
  // TODO: Rename signal into DeleteLayerRequested().
  void DeleteSelectedRequested();
  /**
   */
  void ToggleLayerVisibilityRequested(bool);
  /**
   */
  void RotateLayersRequested(int);
  /**
   */
  void ZoomToFullResolutionRequested();
  /**
   */
  void ZoomToLayerExtentRequested();
  /**
   */
  void ZoomToFullExtentRequested();

  /**
   */
  void ShiftAlphaRequested(double);
  /**
   */
  void UpdateGammaRequested(double);
  /**
   */
  void ResizeShaderRequested(double);
  /**
   */
  void ReparamShaderRequested(double);
  /**
   */
  void ShiftDynamicsRequested(double);
  /**
   */
  void ScaleDynamicsRequested(double);
  /**
   */
  void ResetQuantilesRequested(bool);
  /**
   */
  void ShaderEffectRequested(Effect);
  /**
   */
  void ApplyAllRequested();
  /**
   */
  void SetReferenceRequested();
  /**
   */
  void TakeScreenshotRequested(bool isQuickMode);

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /**
   */
  static PointType Translate(const QPoint& vector, const PointType& origin, const SpacingType& spacing);

  //
  // Protected attributes.
protected:
  /**
   */
  QPoint m_MousePressPosition;
  /**
   */
  otb::ViewSettings::Pointer m_ViewSettings;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Private types.
private:
  //
  // Private methods.
private:
  /**
   */
  template <typename T1, typename T2, typename T3, typename T4>
  inline static double Factor(T1 wheel, T2 degrees, T3 delta, T4 granularity);
  /**
   */
  template <typename T1, typename T2>
  inline static double Factor(T1 wheel, T2 degrees);

  /**
   */
  void Translate(const QPoint& vector);
  /**
   */
  void Scale(const QPoint& center, int degrees, PointType* centerPoint = NULL);

  /**
   */
  // inline void SetRenderMode( const QInputEvent* event );
  /**
   */
  inline void SetFastRenderMode(bool isEnabled);

  //
  // Private attributes.
private:
  /**
   */
  QTimer* m_Timer;
  /**
   */
  SpacingType m_NativeSpacing;
  /**
   */
  otb::ViewSettings::PointType m_MousePressOrigin;
  /**
   */
  AbstractImageViewRenderer::RenderingContext::RenderMode m_RenderMode;
  /**
   */
  double m_ZoomFactor;
  /**
   */
  int m_AlphaGranularity;
  /**
   */
  double m_DynamicsShiftGranularity;
  /**
   */
  int m_ScrollGranularity;
  /**
   */
  int m_ZoomGranularity;
  /**
   */
  bool m_IsMouseDragging : 1;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private slots:
  /**
   */
  void OnTimeout();
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
template <typename T1, typename T2, typename T3, typename T4>
inline double ImageViewManipulator::Factor(T1 wheel, T2 degrees, T3 delta, T4 granularity)
{
  // qDebug() << "wheel:" << wheel;
  // qDebug() << "degrees:" << degrees;
  // qDebug() << "delta:" << delta;
  // qDebug() << "granularity:" << granularity;

  if (abs(granularity) <= std::numeric_limits<T1>::min())
    granularity = 1;

  return pow((1.0 + static_cast<double>(delta)), static_cast<double>(wheel) / (static_cast<double>(granularity) * static_cast<double>(degrees)));
}

/*****************************************************************************/
template <typename T1, typename T2>
inline double ImageViewManipulator::Factor(T1 wheel, T2 degrees)
{
  return ImageViewManipulator::Factor(wheel, degrees, ImageViewManipulator::DEFAULT_DELTA, ImageViewManipulator::DEFAULT_GRANULARITY);
}

/*****************************************************************************/
/*
inline
void
ImageViewManipulator
::SetRenderMode( const QInputEvent* event )
{
  m_RenderMode =
    ( event!=NULL &&
      ( event->modifiers() & Qt::ControlModifier )==Qt::ControlModifier
    )
    ? AbstractImageViewRenderer::RenderingContext::RENDER_MODE_LIGHT
    : AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL;

  qDebug() << "Render-mode:" << m_RenderMode;
}
*/

/*****************************************************************************/
inline void ImageViewManipulator::SetFastRenderMode(bool isEnabled)
{
  m_RenderMode = isEnabled ? AbstractImageViewRenderer::RenderingContext::RENDER_MODE_LIGHT : AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL;

  // qDebug() << "Render-mode:" << m_RenderMode;
}

} // end namespace 'mvd'

#endif // mvdImageViewManipulator_h
