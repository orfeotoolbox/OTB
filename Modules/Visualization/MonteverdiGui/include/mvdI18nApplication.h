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


#ifndef mvdI18nApplication_h
#define mvdI18nApplication_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdI18nCoreApplication.h"

//
// Macros.


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */


//
// Classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class I18nApplication
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief The MVD2 application (which is different from
 * QApplication).
 *
 * I18nApplication do not derive from QApplication to prevent a dread
 * diamong multiple inheritance scheme which could not be resolved
 * using virtual inheritance (because Qt is not designed for).
 *
 *
 * \see I18nCoreApplication.
 */
class OTBMonteverdiGUI_EXPORT I18nApplication : public I18nCoreApplication
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
   * \param qtApp The parent Qt application of this MVD2 application.
   */
  I18nApplication(QApplication* qtApp);

  /** \brief Destructor. */
  ~I18nApplication() override;

  //
  // APPLICATION SINGLETON.
  //

  /**
   * \brief Get the singleton instance of application as a
   * I18Application pointer.
   *
   * \return The singleton instance of I18nApplication.
   */
  inline static I18nApplication* Instance();

  /**
   * \brief Get the singleton constant instance of application as a
   * I18Application pointer.
   *
   * \return The singleton constant instance of I18nApplication.
   */
  inline static const I18nApplication* ConstInstance();

  //
  // Public attributes.
public:
  /*-[ PUBLIC SLOTS SECTION ]-----------------------------------------------**/

public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  void virtual_InitializeCore() override;

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
  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // SLOTS
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
I18nApplication* I18nApplication::Instance()
{
  return I18nCoreApplication::Instance<I18nApplication>();
}

/*****************************************************************************/
const I18nApplication* I18nApplication::ConstInstance()
{
  return I18nCoreApplication::ConstInstance<I18nApplication>();
}

} // end namespace 'mvd'

#endif // I18nApplication_h
