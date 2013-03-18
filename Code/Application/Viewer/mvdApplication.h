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

#ifndef __mvdApplication_h
#define __mvdApplication_h

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
#include "Core/mvdI18nApplication.h"


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
// Class declaration.
class AbstractModel;
class DatasetModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class Application
 *
 */
class Monteverdi2_EXPORT Application
  : public I18nApplication
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public constants.
public:
  static const char* DATASET_EXT;

//
// Public methods.
public:

  /**
   * \brief Constructor.
   *
   * \param argc Command-line argument count in the argv array.
   * \param argv Array of command-argument (whitespace-separated) strings.
   */
  Application( int& argc, char** argv );

  /** \brief Destructor. */
  virtual ~Application();

  /**
   *
   */
  bool HasSettingsFile();

  /**
   *
   */
  void ReadCacheDirFromSettings();

  /**
    *
    */
  void WriteCacheDirIntoSettings();

  /**
   * \brief Check if the cache directory is valid.
   *
   * \return true if the cache directory is correct
   */
  // TODO: Remove method when class is upgraded to I18nApplication fatorized code.
  bool CheckCacheDirIsCorrect();

  /**
   * \brief Test if the application can write in the directory provided.
   *
   * \param dir input directory
   * \return true if the directory can be used
   */
  // TODO: Remove method when class is upgraded to I18nApplication fatorized code.
  bool TestDirExistenceAndWriteAcess( QDir dir);

  //
  // STATIC METHODS.

  /**
   * \brief Get the singleton instance of application as an
   * Application pointer.
   *
   * \return The singleton instance of Application.
   */
  inline static Application* Instance();

  /**
   * \brief Get the singleton constant instance of application as an
   * Application pointer.
   *
   * \return The singleton constant instance of I18nApplication.
   */
  inline static const Application* ConstInstance();

//
// PUBLIC SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
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
// Slots
private slots:

  /** */
  void OnAboutToQuit();
};

/*****************************************************************************/
/* INLINE SECTION                                                            */

/*****************************************************************************/
inline
Application*
Application
::Instance()
{
  return I18nApplication::Instance< Application >();
}

/*****************************************************************************/
inline
const Application*
Application
::ConstInstance()
{
  return I18nApplication::ConstInstance< Application >();
}

/*****************************************************************************/

} // end namespace 'mvd'

#endif // __Application_h
