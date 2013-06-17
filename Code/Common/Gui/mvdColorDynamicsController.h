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
#include "Core/mvdCore.h"
//
#include "mvdAbstractModelController.h"


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
  void OnRgbChannelIndexChanged( RgbwChannel channel, int band );

  /**
   * \brief Slot called when the band-index of the white (gray)
   * channel has changed.
   *
   * \param band The new band-index of the white (gray) channel.
   */
  void OnGrayChannelIndexChanged( int band );

  /**
   * \brief Slot called when the activation-state of the
   * grayscale-mode has changed.
   *
   * \param activated The new grayscale-mode activation state.
   */
  void OnGrayscaleActivated( bool activated );

  /**
   * \brief Slot called when the NoDataFlagToggled() signal of the
   * controlled widget has been emitted (\see ColorDynamicsWidget).
   *
   * \param enabled true when the no-data flag is enabled.
   */
  void OnNoDataFlagToggled( bool enabled );

  /**
   * \brief Slot called when the NoDataValueChanged() signal of the
   * controlled widget has been emitted (\see ColorDynamicsWidget).
   */
  void OnNoDataValueChanged( double value );

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
  // AbstractModelController overrides.

  virtual void Connect( AbstractModel* );

  virtual void ResetWidget();

  virtual void Disconnect( AbstractModel* );


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
  void ResetIntensityRanges( RgbwChannel );

  /**
   * \brief Reset low and high intensities to default values for given
   * RGB channels.
   *
   * \param channels Given channels for which to reset current-band
   * index. \see RgbBound() for valid values.
   */
  void ResetIntensities( RgbwChannel );

  /**
   * \brief Set low and high intensitied of controlled widget based on
   * the values stored in the model settings.
   *
   * \param channels RGB/W channels for which to set low and hight intensities.
   */
  void SetIntensities( RgbwChannel channels );

  /**
   * \brief Reset low and high quantiles to default values for given
   * RGB channels.
   *
   * \param channels Given channels for which to reset current-band
   * index. \see RgbBound() for valid values.
   */
  void ResetQuantiles( RgbwChannel );

  /**
   * \brief Convenience method used to compute integer indices in order
   * to iterate through RGB channels such as, for example:
   * \code
   * for( i=begin; i<end; ++i ) {}
   * \endcode
   *
   * \see mvd::RgbwBounds().
   *
   * \param begin    The resulting first index where to begin iteration.
   * \param end      The resulting upper-boundary index of the iteration.
   * \param channels The channels to be iterated. Valid values are:
   * _ RGBA_CHANNEL_NONE to select no video-channel at all;
   * - RGBA_CHANNEL_RED to select red video-channel;
   * - RGBA_CHANNEL_GREEN to select green video-channel;
   * - RGBA_CHANNEL_BLUE to select blue video-channel;
   * - RGBA_CHANNEL_WHITE is equivalent to RGBA_CHANNEL_RGB;
   * - RGBA_CHANNEL_RGB to select all RGB video-channels;
   * - RGBA_CHANNEL_ALL to select all RGB (without the alpha) video-channels.
   *
   * \return true if iteration indices have been set and loop can be
   * processed.
   */
  inline
    static bool RgbwBounds( CountType& begin,
			    CountType& end,
			    RgbwChannel channels );

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
  void OnLowQuantileChanged( RgbwChannel channel, double quantile );

  /**
   * \brief Slot called when high quantile value of a RGB channel has
   * been edited.
   *
   * \param channel RGB channel for which the quantile value has
   * changed.
   * \param quantile The new quantile value.
   */
  void OnHighQuantileChanged( RgbwChannel channel, double quantile );

  /**
   * \brief Slot called when low intensity value of a RGB channel has
   * been edited.
   *
   * \param channel RGB channel for which the intensity value has
   * changed.
   * \param intensity The new intensity value.
   */
  void OnLowIntensityChanged( RgbwChannel channel, double intensity );

  /**
   * \brief Slot called when high intensity value of a RGB channel has
   * been edited.
   *
   * \param channel RGB channel for which the intensity value has
   * changed.
   * \param intensity The new intensity value.
   */
  void OnHighIntensityChanged( RgbwChannel channel, double intensity );

  /**
   * \brief Slot called when the reset intensities button has been
   * clicked.
   *
   * \param channel RGB channel for which to reset low and high
   * intensities.
   */
  void OnResetIntensityClicked( RgbwChannel channel );

  /**
   * \brief Slot called when the reset quantiles button has been
   * clicked.
   *
   * \param channel RGB channel for which to reset low and high
   * quantiles.
   */
  void OnResetQuantileClicked( RgbwChannel channel );

  /**
   * \brief Slot called when the apply all button has been clicked.
   *
   * \param channel RGB channel for which to reset low and high
   * quantiles.
   */
  void OnApplyAllClicked( RgbwChannel channel, double low, double high );

};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*******************************************************************************/
inline
bool
ColorDynamicsController
::RgbwBounds( CountType& begin, CountType& end, RgbwChannel channels )
{
#if 0
  return mvd::RgbBounds(
    begin, end,
    channels==RGBW_CHANNEL_WHITE
    ? RGBW_CHANNEL_RGB
    : channels
  );
#else
  return mvd::RgbwBounds( begin, end, channels );
#endif
}

} // end namespace 'mvd'

#endif // __mvdColorDynamicsController_h
