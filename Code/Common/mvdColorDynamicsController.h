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
#include "mvdTypes.h"


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

  /** Constructor */
  ColorDynamicsController( ColorDynamicsWidget* widget, QObject* parent =NULL );

  /** Destructor */
  virtual ~ColorDynamicsController();


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

  /** */
  virtual void Connect( AbstractModel* );

  /** */
  virtual void ResetWidget();

  /** */
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

  /** */
  void ResetIntensityRanges( RgbaChannel );

  /** */
  void ResetIntensities( RgbaChannel );

  /** */
  void ResetQuantiles( RgbaChannel );

  /** */
  inline static
    bool
    RgbBounds( CountType& begin,
	       CountType& end,
	       RgbaChannel channel );

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /** */
  void OnLowQuantileChanged( RgbaChannel, double );
  /** */
  void OnHighQuantileChanged( RgbaChannel, double );
  /** */
  void OnLowIntensityChanged( RgbaChannel, double );
  /** */
  void OnHighIntensityChanged( RgbaChannel, double );
  /** */
  void OnResetIntensityClicked( RgbaChannel );
  /** */
  void OnResetQuantileClicked( RgbaChannel );
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
/*****************************************************************************/
inline
bool
ColorDynamicsController
::RgbBounds( CountType& begin, CountType& end, RgbaChannel channel )
{
  assert(
    channel==RGBA_CHANNEL_NONE ||
    channel==RGBA_CHANNEL_RED ||
    channel==RGBA_CHANNEL_GREEN ||
    channel==RGBA_CHANNEL_BLUE ||
    channel==RGBA_CHANNEL_RGB
  );

  switch( channel )
    {
    case RGBA_CHANNEL_NONE:
    case RGBA_CHANNEL_ALPHA:
      begin = RGBA_CHANNEL_NONE;
      end = RGBA_CHANNEL_NONE;
      break;

    case RGBA_CHANNEL_RED:
    case RGBA_CHANNEL_GREEN:
    case RGBA_CHANNEL_BLUE:
      begin = channel;
      end = channel + 1;
      break;

    case RGBA_CHANNEL_RGB:
      begin = RGBA_CHANNEL_RED;
      end = RGBA_CHANNEL_ALPHA;
      break;

    default:
      return false;
      break;
    }

  return true;
}

} // end namespace 'mvd'

#endif // __mvdColorDynamicsController_h
