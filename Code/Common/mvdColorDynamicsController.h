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
#ifndef __mvdColorDynamicsController_h
#define __mvdColorDynamicsController_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractModelController.h"
#include "mvdGui.h"


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
class ColorDynamicsWidget;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ColorDynamicsController
 *
 * \brief Color-dynamics widget controller for VectorImageModel objects.
 */
class Monteverdi2_EXPORT ColorDynamicsController :
    public AbstractModelController
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /**
   * \brief Constructor.
   *
   * \param widget Controlled widget.
   * \param parent Parent QObject of this QObject.
   */
  ColorDynamicsController( ColorDynamicsWidget* widget, QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~ColorDynamicsController();

  /*-[ PUBLIC SLOTS SECTION ]-----------------------------------------------**/

//
// Slots.
public slots:
  /**
   * \brief Slot called when the band-index of a RGB channel has
   * changed.
   *
   * \param channel The RGB channel for which the band-index has changed.
   * \param band The new band-index of the RGB channel.
   */
  void OnRgbChannelIndexChanged( RgbaChannel channel, int band );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // AbstractModelController overrides.

  virtual void Connect( AbstractModel* );

  virtual void ResetWidget();

  virtual void Disconnect( AbstractModel* );

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
   * \brief Reset intensity ranges to default values for given RGB
   * channels.
   *
   * \param channels Given channels for which to reset current-band
   * index. \see RgbBound() for valid values.
   */
  void ResetIntensityRanges( RgbaChannel );

  /**
   * \brief Reset low and high intensities to default values for given
   * RGB channels.
   *
   * \param channels Given channels for which to reset current-band
   * index. \see RgbBound() for valid values.
   */
  void ResetIntensities( RgbaChannel );

  /**
   * \brief Reset low and high quantiles to default values for given
   * RGB channels.
   *
   * \param channels Given channels for which to reset current-band
   * index. \see RgbBound() for valid values.
   */
  void ResetQuantiles( RgbaChannel );

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /**
   * \brief Slot called when low quantile value of a RGB channel has
   * been edited.
   *
   * \param channel RGB channel for which the quantile value has
   * changed.
   * \param quantile The new quantile value.
   */
  void OnLowQuantileChanged( RgbaChannel channel, double quantile );

  /**
   * \brief Slot called when high quantile value of a RGB channel has
   * been edited.
   *
   * \param channel RGB channel for which the quantile value has
   * changed.
   * \param quantile The new quantile value.
   */
  void OnHighQuantileChanged( RgbaChannel channel, double quantile );

  /**
   * \brief Slot called when low intensity value of a RGB channel has
   * been edited.
   *
   * \param channel RGB channel for which the intensity value has
   * changed.
   * \param intensity The new intensity value.
   */
  void OnLowIntensityChanged( RgbaChannel channel, double intensity );

  /**
   * \brief Slot called when high intensity value of a RGB channel has
   * been edited.
   *
   * \param channel RGB channel for which the intensity value has
   * changed.
   * \param intensity The new intensity value.
   */
  void OnHighIntensityChanged( RgbaChannel channel, double intensity );

  /**
   * \brief Slot called when the reset intensities button has been
   * clicked.
   *
   * \param channel RGB channel for which to reset low and high
   * intensities.
   */
  void OnResetIntensityClicked( RgbaChannel channel );

  /**
   * \brief Slot called when the reset quantiles button has been
   * clicked.
   *
   * \param channel RGB channel for which to reset low and high
   * quantiles.
   */
  void OnResetQuantileClicked( RgbaChannel channel );
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdColorDynamicsController_h
