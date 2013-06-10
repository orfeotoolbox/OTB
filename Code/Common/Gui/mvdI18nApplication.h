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

#ifndef __mvdI18nApplication_h
#define __mvdI18nApplication_h

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
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdI18nCoreApplication.h"

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
class Monteverdi2_EXPORT I18nApplication
  : public I18nCoreApplication
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
  I18nApplication( QApplication* qtApp );

  /** \brief Destructor. */
  virtual ~I18nApplication();

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

  virtual void virtual_InitializeCore();

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
I18nApplication*
I18nApplication
::Instance()
{
  return I18nCoreApplication::Instance< I18nApplication >();
}

/*****************************************************************************/
const I18nApplication*
I18nApplication
::ConstInstance()
{
  return I18nCoreApplication::ConstInstance< I18nApplication >();
}

} // end namespace 'mvd'

#endif // __I18nApplication_h
