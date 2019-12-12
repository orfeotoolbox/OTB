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

#ifndef mvdColorDynamicsWidget_h
#define mvdColorDynamicsWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdColorBandDynamicsWidget.h"


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
namespace Ui
{
class ColorDynamicsWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ColorDynamicsWidget
 *
 * \ingroup OTBMonteverdiGUI
 */
class OTBMonteverdiGUI_EXPORT ColorDynamicsWidget : public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  Q_PROPERTY(bool isNoDataChecked READ IsNoDataChecked WRITE SetNoDataChecked);

  Q_PROPERTY(double NoDataValue READ GetNoDataValue WRITE SetNoDataValue);

  Q_PROPERTY(int GammaCursorPosition READ GetGammaCursorPosition WRITE SetGammaCursorPosition);

  Q_PROPERTY(int MinGamma READ GetMinGamma WRITE SetMinGamma);

  Q_PROPERTY(int MaxGamma READ GetMaxGamma WRITE SetMaxGamma);

  Q_PROPERTY(int GammaStep READ GetGammaStep WRITE SetGammaStep);

  Q_PROPERTY(double Gamma READ GetGamma WRITE SetGamma);

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** Constructor. */
  ColorDynamicsWidget(QWidget* p = NULL, Qt::WindowFlags flags = 0);

  /** Destructor. */
  ~ColorDynamicsWidget() override;

  /**
   */
  inline const ColorBandDynamicsWidget* GetChannel(RgbwChannel) const;

  /**
   */
  inline ColorBandDynamicsWidget* GetChannel(RgbwChannel);

  /**
   */
  void SetGrayscaleActivated(bool activated);

  /**
   */
  bool IsGrayscaleActivated() const;

  /**
   */
  void SetNoDataChecked(bool checked);

  /**
   */
  bool IsNoDataChecked() const;

  /**
   */
  void SetNoDataValue(double value);

  /**
   */
  double GetNoDataValue() const;

  /**
   */
  void SetGamma(double value);

  /**
   */
  double GetGamma() const;

  /**
   */
  void SetGammaCursorPosition(int value);

  /**
   */
  int GetGammaCursorPosition() const;

  /**
   */
  void SetMinGamma(int value);

  /**
   */
  int GetMinGamma() const;

  /**
   */
  void SetMaxGamma(int value);

  /**
   */
  int GetMaxGamma() const;

  /**
   */
  void SetGammaStep(int value);

  /**
   */
  int GetGammaStep() const;

  //
  // Public slots.
public slots:

  /**
   */
  void SetNoDataButtonChecked(bool checked);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
signals:
  /**
   */
  void LowQuantileChanged(RgbwChannel, double);

  /**
   */
  void HighQuantileChanged(RgbwChannel, double);

  /**
   */
  void LowIntensityChanged(RgbwChannel, double);

  /**
   */
  void HighIntensityChanged(RgbwChannel, double);

  /**
   */
  void ResetIntensityClicked(RgbwChannel);

  /**
   */
  void ResetQuantileClicked(RgbwChannel);

  /**
   */
  void ApplyAllClicked(RgbwChannel, double, double);

  /**
   */
  void LinkToggled(RgbwChannel, bool);

  /**
   */
  void NoDataFlagToggled(bool enabled);

  /**
   */
  void NoDataValueChanged(double value);

  /**
   */
  void NoDataButtonPressed();

  /**
   */
  void GammaCursorPositionChanged(int value);

  /**
   */
  void GammaValueChanged(double value);

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  /**
   * \brief Connect signals of color-band dynamics child widget to
   * slots of this widget.
   *
   * \param child Color-band dynamics child widget.
   * \param channel RGBAW channel setup of child widget.
   */
  void ConnectChild(ColorBandDynamicsWidget* child, RgbwChannel channel);

  //
  // Private attributes.
private:
  /** */
  static const char* COLOR_BAND_DYNAMICS_WIDGET_NAMES[];

  /**
   * uic generated.
   */
  Ui::ColorDynamicsWidget* m_UI;

  /**
   */
  QDoubleValidator* m_NoDataValidator;

  /**
   */
  bool m_IsGrayscaleActivated;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private slots:

  /**
   */
  void on_noDataLineEdit_textChanged(const QString& text);

  /**
   */
  void on_noDataCheckBox_toggled(bool enabled);

  /**
   */
  void on_noDataButton_toggled(bool checked);

  /**
   */
  void on_gammaSlider_valueChanged(int gamma);

  void on_gammaResetButton_clicked();
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
inline const ColorBandDynamicsWidget* ColorDynamicsWidget::GetChannel(RgbwChannel channel) const
{
  return findChild<const ColorBandDynamicsWidget*>(ColorDynamicsWidget::COLOR_BAND_DYNAMICS_WIDGET_NAMES[channel]);
}

/*****************************************************************************/
inline ColorBandDynamicsWidget* ColorDynamicsWidget::GetChannel(RgbwChannel channel)
{
  return findChild<ColorBandDynamicsWidget*>(ColorDynamicsWidget::COLOR_BAND_DYNAMICS_WIDGET_NAMES[channel]);
}

/*****************************************************************************/
inline bool ColorDynamicsWidget::IsGrayscaleActivated() const
{
  return m_IsGrayscaleActivated;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'

#endif // mvdColorDynamicsWidget_h
