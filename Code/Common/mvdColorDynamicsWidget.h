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
#ifndef __mvdColorDynamicsWidget_h
#define __mvdColorDynamicsWidget_h

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
 */
class Monteverdi2_EXPORT ColorDynamicsWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Constructor */
  ColorDynamicsWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~ColorDynamicsWidget();

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

  /** */
  inline const ColorBandDynamicsWidget* GetChannel( RgbaChannel ) const;
  /** */
  inline ColorBandDynamicsWidget* GetChannel( RgbaChannel );

//
// Private attributes.
private:
  /** */
  static const char* COLOR_BAND_DYNAMICS_WIDGET_NAMES[];

  /**
   * uic generated.
   */
  Ui::ColorDynamicsWidget* m_UI;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:

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
::GetChannel( RgbaChannel channel ) const
{
  return findChild< const ColorBandDynamicsWidget* >(
    ColorDynamicsWidget::COLOR_BAND_DYNAMICS_WIDGET_NAMES[ channel ]
  );
}

/*****************************************************************************/
inline
ColorBandDynamicsWidget*
ColorDynamicsWidget
::GetChannel( RgbaChannel channel )
{
  return findChild< ColorBandDynamicsWidget* >(
    ColorDynamicsWidget::COLOR_BAND_DYNAMICS_WIDGET_NAMES[ channel ]
  );
}
/*****************************************************************************/
/* SLOTS                                                                     */

} // end namespace 'mvd'

#endif // __mvdColorDynamicsWidget_h
