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
#ifndef __mvdColorDynamicsWidget_h
#define __mvdColorDynamicsWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

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
 * \ingroup OTBMonteverdiGui
 */
class ITK_EXPORT ColorDynamicsWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  Q_PROPERTY( bool isNoDataChecked
	      READ IsNoDataChecked
	      WRITE SetNoDataChecked );

  Q_PROPERTY( double NoDataValue
	      READ GetNoDataValue
	      WRITE SetNoDataValue );

  Q_PROPERTY( int GammaCursorPosition
	      READ GetGammaCursorPosition
	      WRITE SetGammaCursorPosition );

  Q_PROPERTY( int MinGamma
	      READ GetMinGamma
	      WRITE SetMinGamma );

  Q_PROPERTY( int MaxGamma
	      READ GetMaxGamma
	      WRITE SetMaxGamma );

  Q_PROPERTY( int GammaStep
	      READ GetGammaStep
	      WRITE SetGammaStep );

  Q_PROPERTY( double Gamma
              READ GetGamma
              WRITE SetGamma );

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Constructor. */
  ColorDynamicsWidget( QWidget* p =NULL, Qt::WindowFlags flags =0 );

  /** Destructor. */
  virtual ~ColorDynamicsWidget();

  /**
   */
  inline const ColorBandDynamicsWidget* GetChannel( RgbwChannel ) const;

  /**
   */
  inline ColorBandDynamicsWidget* GetChannel( RgbwChannel );

  /**
   */
  void SetGrayscaleActivated( bool activated );

  /**
   */
  bool IsGrayscaleActivated() const;

  /**
   */
  void SetNoDataChecked( bool checked );

  /**
   */
  bool IsNoDataChecked() const;

  /**
   */
  void SetNoDataValue( double value );

  /**
   */
  double GetNoDataValue() const;

  /**
   */
  void SetGamma( double value );

  /**
   */
  double GetGamma() const;

  /**
   */
  void SetGammaCursorPosition( int value );

  /**
   */
  int GetGammaCursorPosition() const;

  /**
   */
  void SetMinGamma( int value );

  /**
   */
  int GetMinGamma() const;

  /**
   */
  void SetMaxGamma( int value );

  /**
   */
  int GetMaxGamma() const;

  /**
   */
  void SetGammaStep( int value );

  /**
   */
  int GetGammaStep() const;

//
// Public slots.
public slots:

  /**
   */
  void SetNoDataButtonChecked( bool checked );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void LowQuantileChanged( RgbwChannel, double );

  /**
   */
  void HighQuantileChanged( RgbwChannel, double );

  /**
   */
  void LowIntensityChanged( RgbwChannel, double );

  /**
   */
  void HighIntensityChanged( RgbwChannel, double );

  /**
   */
  void ResetIntensityClicked( RgbwChannel );

  /**
   */
  void ResetQuantileClicked( RgbwChannel );

  /**
   */
  void ApplyAllClicked( RgbwChannel, double, double );

  /**
   */
  void LinkToggled( RgbwChannel, bool );

  /**
   */
  void NoDataFlagToggled( bool enabled );

  /**
   */
  void NoDataValueChanged( double value );

  /**
   */
  void NoDataButtonPressed();

  /**
   */
  void GammaCursorPositionChanged( int value );

  /**
   */
  void GammaValueChanged( double value );

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
  void ConnectChild( ColorBandDynamicsWidget* child, RgbwChannel channel );

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
  void on_noDataLineEdit_textChanged( const QString& text );

  /**
   */
  void on_noDataCheckBox_toggled( bool enabled );

  /**
   */
  void on_noDataButton_toggled( bool checked );

  /**
   */
  void on_gammaSlider_valueChanged( int gamma );
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
inline
const ColorBandDynamicsWidget*
ColorDynamicsWidget
::GetChannel( RgbwChannel channel ) const
{
  return findChild< const ColorBandDynamicsWidget* >(
    ColorDynamicsWidget::COLOR_BAND_DYNAMICS_WIDGET_NAMES[ channel ]
  );
}

/*****************************************************************************/
inline
ColorBandDynamicsWidget*
ColorDynamicsWidget
::GetChannel( RgbwChannel channel )
{
  return findChild< ColorBandDynamicsWidget* >(
    ColorDynamicsWidget::COLOR_BAND_DYNAMICS_WIDGET_NAMES[ channel ]
  );
}

/*****************************************************************************/
inline
bool
ColorDynamicsWidget
::IsGrayscaleActivated() const
{
  return m_IsGrayscaleActivated;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'

#endif // __mvdColorDynamicsWidget_h
