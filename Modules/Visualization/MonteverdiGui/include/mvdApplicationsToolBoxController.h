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

#ifndef mvdApplicationsToolBoxController_h
#define mvdApplicationsToolBoxController_h

//
// Configuration include.
//// Included at first position before any other ones.
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "ConfigureMonteverdi.h"
#endif //tag=QT4-boost-compatibility


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
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdAbstractModelController.h"
#endif //tag=QT4-boost-compatibility
//#include "mvdGui.h"


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
class ApplicationsToolBox;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ApplicationsToolBoxController
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Color-setup widget controller for VectorImageModel objects.
 */
class OTBMonteverdiGUI_EXPORT ApplicationsToolBoxController :
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
  ApplicationsToolBoxController( ApplicationsToolBox* widget,
			     QObject* p =NULL );

  /**
   * \brief Destructor.
   */
  ~ApplicationsToolBoxController() ITK_OVERRIDE;

  /** Get the seleceted application Gui */
  // QWidget * GetSelectedApplicationWidget(const QString& appName);


  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // AbstractModelController methods.

  void Connect( AbstractModel* ) ITK_OVERRIDE;

  void ClearWidget() ITK_OVERRIDE;

  void virtual_ResetWidget( bool ) ITK_OVERRIDE;

  void Disconnect( AbstractModel* ) ITK_OVERRIDE;

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   */
  void ResetDatasetTree();

//
// Private attributes.
private:

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
} // end namespace 'mvd'

#endif // mvdApplicationsToolBoxController_h
