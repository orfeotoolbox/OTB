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
#ifndef __mvdHistogramController_h
#define __mvdHistogramController_h

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
class HistogramWidget;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class HistogramController
 *
 * \brief Color-setup widget controller for VectorImageModel objects.
 */
class Monteverdi2_EXPORT HistogramController :
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
  HistogramController( HistogramWidget* widget, QObject* parent =NULL );

  /**
   * \brief Destructor.
   */
  virtual ~HistogramController();


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
  // AbstractModelController overrides.

  virtual void Connect( AbstractModel* );

  virtual void ResetWidget();

  virtual void Disconnect( AbstractModel* );

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

#endif // __mvdHistogramController_h
