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
#ifndef __mvdColorBandDynamicsWidget_h
#define __mvdColorBandDynamicsWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


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
#include "mvdGui.h"
#include "ui_mvdColorBandDynamicsWidget.h"


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
class ColorBandDynamicsWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ColorBandDynamicsWidget
 */
class Monteverdi2_EXPORT ColorBandDynamicsWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  Q_PROPERTY( RgbaChannel channelLabel
	      READ GetChannelLabel
	      WRITE SetChannelLabel );

  Q_PROPERTY( double minIntensity
	      READ GetMinIntensity
	      WRITE SetMinIntensity );

  Q_PROPERTY( double maxIntensity
	      READ GetMaxIntensity
	      WRITE SetMaxIntensity );

  /** */
  Q_PROPERTY( double lowIntensity
	      READ GetLowIntensity
	      WRITE SetLowIntensity );

  /** */
  Q_PROPERTY( double highIntensity
	      READ GetHighIntensity
	      WRITE SetHighIntensity );

  /** */
  Q_PROPERTY( double lowQuantile
	      READ GetLowQuantile
	      WRITE SetLowQuantile );

  /** */
  Q_PROPERTY( double highQuantile
	      READ GetHighQuantile
	      WRITE SetHighQuantile );

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Constructor */
  ColorBandDynamicsWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~ColorBandDynamicsWidget();

  /** */
  inline RgbaChannel GetChannelLabel() const;
  /** */
  inline void SetChannelLabel( RgbaChannel );

  /** */
  inline double GetMinIntensity() const;
  /** */
  inline void SetMinIntensity( double );

  /** */
  inline double GetMaxIntensity() const;
  /** */
  inline void SetMaxIntensity( double );

  /** */
  inline double GetLowIntensity() const;
  /** */
  inline void SetLowIntensity( double );

  /** */
  inline double GetHighIntensity() const;
  /** */
  inline void SetHighIntensity( double );

  /** */
  inline double GetLowQuantile() const;
  /** */
  inline void SetLowQuantile( double );

  /** */
  inline double GetHighQuantile() const;
  /** */
  inline void SetHighQuantile( double );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /** */
  void LowQuantileChanged( RgbaChannel, double );
  /** */
  void HighQuantileChanged( RgbaChannel, double );
  /** */
  void LowIntensityChanged( RgbaChannel, double );
  /** */
  void HighIntensityChanged( RgbaChannel, double );

  /** */
  void ResetIntensityClicked( RgbaChannel );
  /** */
  void ResetQuantileClicked( RgbaChannel );

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

  /** */
  RgbaChannel m_Channel;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  inline void on_lowIntensitySpinBox_valueChanged( double );
  inline void on_highIntensitySpinBox_valueChanged( double );

  inline void on_lowQuantileSpinBox_valueChanged( double );
  inline void on_highQuantileSpinBox_valueChanged( double );

  inline void on_minMaxButton_clicked();
  inline void on_defaultsButton_clicked();
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
inline
double
ColorBandDynamicsWidget
::GetMinIntensity() const
{
  return m_UI->lowIntensitySpinBox->minimum();
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::SetMinIntensity( double value )
{
  m_UI->lowIntensitySpinBox->setMinimum( value );
  m_UI->highIntensitySpinBox->setMinimum( value );
}

/*****************************************************************************/
inline
double
ColorBandDynamicsWidget
::GetMaxIntensity() const
{
  return m_UI->highIntensitySpinBox->maximum();
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::SetMaxIntensity( double value )
{
  m_UI->lowIntensitySpinBox->setMaximum( value );
  m_UI->highIntensitySpinBox->setMaximum( value );
}

/*****************************************************************************/
inline
double
ColorBandDynamicsWidget
::GetLowIntensity() const
{
  return m_UI->lowIntensitySpinBox->value();
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::SetLowIntensity( double value )
{
  m_UI->lowIntensitySpinBox->setValue( value );
}

/*****************************************************************************/
inline
double
ColorBandDynamicsWidget
::GetHighIntensity() const
{
  return m_UI->highIntensitySpinBox->value();
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::SetHighIntensity( double value )
{
  m_UI->highIntensitySpinBox->setValue( value );
}

/*****************************************************************************/
inline
double
ColorBandDynamicsWidget
::GetLowQuantile() const
{
  return m_UI->lowQuantileSpinBox->value();
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::SetLowQuantile( double value )
{
  m_UI->lowQuantileSpinBox->setValue( value );
}

/*****************************************************************************/
inline
double
ColorBandDynamicsWidget
::GetHighQuantile() const
{
  return m_UI->highQuantileSpinBox->value();
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::SetHighQuantile( double value )
{
  m_UI->highQuantileSpinBox->setValue( value );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetChannelLabel( RgbaChannel channel )
{
  m_Channel = channel;

  m_UI->rLabel->setVisible(
    channel==RGBA_CHANNEL_RED || channel==RGBA_CHANNEL_ALL );

  m_UI->gLabel->setVisible(
    channel==RGBA_CHANNEL_GREEN || channel==RGBA_CHANNEL_ALL );

  m_UI->bLabel->setVisible(
    channel==RGBA_CHANNEL_BLUE || channel==RGBA_CHANNEL_ALL );
}

/*****************************************************************************/
RgbaChannel
ColorBandDynamicsWidget
::GetChannelLabel() const
{
  return m_Channel;
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::on_lowIntensitySpinBox_valueChanged( double value )
{
  emit LowIntensityChanged( m_Channel, value );
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::on_highIntensitySpinBox_valueChanged( double value )
{
  emit HighIntensityChanged( m_Channel, value );
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::on_lowQuantileSpinBox_valueChanged( double value )
{
  emit LowQuantileChanged( m_Channel, value );
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::on_highQuantileSpinBox_valueChanged( double value )
{
  emit HighQuantileChanged( m_Channel, value );
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::on_minMaxButton_clicked()
{
  emit ResetIntensityClicked( m_Channel );
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::on_defaultsButton_clicked()
{
  emit ResetQuantileClicked( m_Channel );
}

} // end namespace 'mvd'

#endif // __mvdColorBandDynamicsWidget_h
