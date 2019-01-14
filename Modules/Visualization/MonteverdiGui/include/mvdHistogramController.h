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

#ifndef mvdHistogramController_h
#define mvdHistogramController_h

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

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdCore.h"
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
class HistogramWidget;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class HistogramController
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Color-setup widget controller for VectorImageModel objects.
 */
class OTBMonteverdiGUI_EXPORT HistogramController :
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
  HistogramController( HistogramWidget* widget, QObject* p =NULL );

  /**
   * \brief Destructor.
   */
  ~HistogramController() override;


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

  /**
   * \brief
   */
  void ResetWidget( RgbwChannel channel );

  //
  // AbstractModelController methods.

  void Connect( AbstractModel* ) override;

  void ClearWidget() override;

  void virtual_ResetWidget( bool = false ) override;

  void Disconnect( AbstractModel* ) override;

//
// Private attributes.
private:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
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
   * \brief
   */
  void OnLowIntensityChanged( RgbwChannel channel, double value, bool refresh );

  /**
   * \brief
   */
  void OnHighIntensityChanged( RgbwChannel channel, double value, bool refresh );

  /**
   */
  void OnHistogramRefreshed();
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // mvdHistogramController_h
