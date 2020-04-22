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

#ifndef mvdColorBandDynamicsWidget_h
#define mvdColorBandDynamicsWidget_h

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
#include "mvdCore.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
class DoubleValidator;
//
// Internal classes pre-declaration.
namespace Ui
{
class ColorBandDynamicsWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ColorBandDynamicsWidget
 *
 * \ingroup OTBMonteverdiGUI
 */
class OTBMonteverdiGUI_EXPORT ColorBandDynamicsWidget : public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /** */
  Q_PROPERTY(RgbwChannel channelLabel READ GetChannelLabel WRITE SetChannelLabel);

  /**
   */
  Q_PROPERTY(double minIntensity READ GetMinIntensity WRITE SetMinIntensity);

  /**
   */
  Q_PROPERTY(double maxIntensity READ GetMaxIntensity WRITE SetMaxIntensity);

  /**
   */
  Q_PROPERTY(double lowIntensity READ GetLowIntensity WRITE SetLowIntensity);

  /**
   */
  Q_PROPERTY(double highIntensity READ GetHighIntensity WRITE SetHighIntensity);

  /**
   */
  Q_PROPERTY(double lowQuantile READ GetLowQuantile WRITE SetLowQuantile);

  /**
   */
  Q_PROPERTY(double highQuantile READ GetHighQuantile WRITE SetHighQuantile);

  /**
   */
  Q_PROPERTY(bool isBounded READ IsBounded() WRITE SetBounded());

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** Constructor */
  ColorBandDynamicsWidget(QWidget* p = NULL, Qt::WindowFlags flags = 0);

  /** Destructor */
  ~ColorBandDynamicsWidget() override;

  /** */
  RgbwChannel GetChannelLabel() const;

  /** */
  void SetChannelLabel(RgbwChannel);

  /** */
  double GetMinIntensity() const;
  /** */
  void SetMinIntensity(double);

  /** */
  double GetMaxIntensity() const;
  /** */
  void SetMaxIntensity(double);

  /** */
  double GetLowIntensity() const;
  /** */
  void SetLowIntensity(double);

  /** */
  double GetHighIntensity() const;
  /** */
  void SetHighIntensity(double);

  /** */
  double GetLowQuantile() const;
  /** */
  void SetLowQuantile(double);

  /** */
  double GetHighQuantile() const;
  /** */
  void SetHighQuantile(double);

  /**
   */
  bool IsBounded() const;
  /**
   */
  void SetBounded(bool enabled);
  /**
   */
  void SetLinkButtonEnabled(bool);
  /**
   */
  void SetDefaultsButtonEnabled(bool);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:
  /** */
  void LowQuantileChanged(RgbwChannel, double);
  /** */
  void HighQuantileChanged(RgbwChannel, double);
  /** */
  void LowIntensityChanged(RgbwChannel, double);
  /** */
  void HighIntensityChanged(RgbwChannel, double);

  /** */
  void ResetIntensityClicked(RgbwChannel);
  /** */
  void ResetQuantileClicked(RgbwChannel);
  /** */
  void ApplyAllClicked(RgbwChannel, double, double);

  /**
   */
  void LinkToggled(RgbwChannel, bool);


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
  //
  // Private attributes.
private:
  /**
   * uic generated.
   */
  Ui::ColorBandDynamicsWidget* m_UI;

  /**
   */
  DoubleValidator* m_LowIntensityValidator;

  /**
   */
  DoubleValidator* m_HighIntensityValidator;

  /** */
  RgbwChannel m_Channel;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
  void on_lowIntensityLineEdit_editingFinished();
  void on_highIntensityLineEdit_editingFinished();

  void on_lowQuantileSpinBox_valueChanged(double);
  void on_highQuantileSpinBox_valueChanged(double);

  void on_minMaxButton_clicked();
  void on_defaultsButton_clicked();
  void on_applyAllButton_clicked();

  void on_linkButton_toggled(bool);
};

} // end namespace 'mvd'.

#endif // mvdColorBandDynamicsWidget_h
