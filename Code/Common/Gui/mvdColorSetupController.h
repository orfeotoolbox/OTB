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
#ifndef __mvdColorSetupController_h
#define __mvdColorSetupController_h

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
class ColorSetupWidget;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ColorSetupController
 *
 * \brief Color-setup widget controller for VectorImageModel objects.
 */
class Monteverdi2_EXPORT ColorSetupController :
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
  ColorSetupController( ColorSetupWidget* widget, QObject* parent =NULL );

  /**
   * \brief Destructor.
   */
  virtual ~ColorSetupController();


  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   * \brief Signal emitted when the band-index of a RGB channel has
   * been changed. This signal is used to forward events to other
   * controllers such as ColorDynamicsController.
   *
   * \param channel The RGB channel for which the band-index has changed.
   * \param band The new band-index of the RGB channel.
   */
  void RgbChannelIndexChanged( RgbaChannel channel, int band );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // AbstractModelController overrides.

  virtual void Connect( core::AbstractModel* );

  virtual void ResetWidget();

  virtual void Disconnect( core::AbstractModel* );

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   * \brief Reset current band-index to default values for given RGB
   * channels.
   *
   * \param channels Given channels for which to reset current-band
   * index. \see RgbBound() for valid values.
   */
  void ResetIndices( RgbaChannel channels );

//
// Private attributes.
private:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /**
   * \brief Slot called when the current index of a RGB channel of the controlled widget has changed.
   *
   * \param channel The RGB video-channel for which the current
   * band-index has changed.
   *
   * \param index The new band-index of the RGB video-channel.
   */
  void OnCurrentIndexChanged( RgbaChannel channel, int index );

};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdColorSetupController_h
