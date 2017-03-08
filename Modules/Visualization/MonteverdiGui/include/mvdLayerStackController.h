/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef mvdLayerStackController_h
#define mvdLayerStackController_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
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
class AbstractLayerModel;
class LayerStackWidget;


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class LayerStackController
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class OTBMonteverdiGUI_EXPORT LayerStackController :
    public AbstractModelController
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  LayerStackController( LayerStackWidget * widget, QObject * p =NULL );

  /** \brief Destructor. */
  ~LayerStackController() ITK_OVERRIDE;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void ApplyAllRequested();
  void ResetEffectsRequested();

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
   */
  void UpdateButtonsState();

  //
  // AbstractModelController overloads.

  /**
   */
  void Connect( AbstractModel * ) ITK_OVERRIDE;

  /**
   */
  void Disconnect( AbstractModel * ) ITK_OVERRIDE;

  /**
   */
  void ClearWidget() ITK_OVERRIDE;

  /**
   */
  void virtual_ResetWidget( bool ) ITK_OVERRIDE;


//
// Private attributes.
private:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /**
   */
  void OnCurrentChanged( int );
  /**
   */
  void OnSelectionChanged( int );
  /**
   */
  void OnStackedLayerCurrentChanged( size_t );
  /**
   */
  void OnProjectionButtonClicked();
  /**
   */
  void OnStackedLayerContentChanged();
  /**
   */
  void OnStackedLayerContentReset();
  /**
   */
  void OnCopyLayerRequested( const AbstractLayerModel * );
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

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

namespace mvd
{
} // end namespace 'mvd'

#endif // mvdLayerStackController_h
