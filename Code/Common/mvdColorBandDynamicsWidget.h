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

#if 0
  /** */
  Q_PROPERTY( double lowIntensity
	      READ GetLowIntensity
	      WRITE SetLowIntensity );

  /** */
  Q_PROPERTY( double highIntensity
	      READ GetHighIntensity
	      WRITE SetHighIntensity );

  /** */
  Q_PROPERTY( double minIntensity
	      READ GetMinIntensity
	      WRITE SetMinIntensity );

  /** */
  Q_PROPERTY( double maxIntensity
	      READ GetMinIntensity
	      WRITE SetMinIntensity );

  /** */
  Q_PROPERTY( double lowQuantile
	      READ GetLowQuantile
	      WRITE SetLowQuantile );

  /** */
  Q_PROPERTY( double highQuantile
	      READ GetHighQuantile
	      WRITE SetHighQuantile );

  /** */
  Q_PROPERTY( double minQuantile
	      READ GetMinQuantile
	      WRITE SetMinQuantile );

  /** */
  Q_PROPERTY( double maxQuantile
	      READ GetMinQuantile
	      WRITE SetMinQuantile );
#endif

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Constructor */
  ColorBandDynamicsWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~ColorBandDynamicsWidget();

  /** */
  inline void SetChannelLabel( RgbaChannel );

  /** */
  inline RgbaChannel GetChannelLabel() const;

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

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

  void on_lowQuantileSpinBox_valueChanged( double );
  void on_highQuantileSpinBox_valueChanged( double );

  void on_minMaxButton_clicked();
  void on_defaultsButton_clicked();
};

/*****************************************************************************/
/* INLINE SECTION                                                            */

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
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::on_highIntensitySpinBox_valueChanged( double value )
{
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::on_minMaxButton_clicked()
{
}

/*****************************************************************************/
inline
void
ColorBandDynamicsWidget
::on_defaultsButton_clicked()
{
}

} // end namespace 'mvd'

#endif // __mvdColorBandDynamicsWidget_h
