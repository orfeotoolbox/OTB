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
#include "mvdTypes.h"

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
 * \brief WIP.
 *
 */
class Monteverdi2_EXPORT I18nApplication
  : public QApplication
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
   * \param argc Command-line argument count in the argv array.
   * \param argv Array of command-argument (whitespace-separated) strings.
   */
  I18nApplication( int& argc, char** argv );

  /** \brief Destructor. */
  virtual ~I18nApplication();

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

  /**
   * \brief Get the singleton instance of application (safe) casted
   * into the TApplication type.
   *
   * \return The singleton instance of application (safely) casted
   * into TApplication type.
   */
  template< typename TApplication >
    inline static TApplication* Instance();

  /**
   * \brief Get the constant singleton instance of application (safe)
   * casted  into the TApplication type.
   *
   * \return The constant singleton instance of application (safely)
   * casted into TApplication type.
   */
  template< typename TApplication >
    inline static const TApplication* ConstInstance();

  /**
   * \brief Create cache directory if not already existing.
   *
   * \param cacheDirStr path where the function will create the cache directory
   * \return true if cache directory has just been created or false if
   * it was already existing.
   */
  bool MakeCacheDir(QString cacheDirStr);

  /**
   * \brief Check if the cache directory is valid.
   *
   * \return true if the cache directory is correct
   */
  bool CheckCacheDirIsCorrect();

  /**
   * \brief Test if the application can write in the directory provided.
   *
   * \param dir input directory
   * \return true if the directory can be used
   */
  bool TestDirExistenceAndWriteAcess( QDir dir);

  /**
   * \brief Get the cache directory.
   *
   * \return Return the cache directory.
   */
  inline const QDir& GetCacheDir() const;

  /**
   * \brief Get the cache directory name.
   *
   * \return Return the cache directory name.
   */
  inline const char* GetCacheDirName() const;

  //
  // STATIC methods.

  /**
   * \brief Make directory tree in given path.
   *
   * Throws mvd::SystemError exception if sub-directory tree could not
   * have been created.
   *
   * \param path Location where to create sub-directory tree.
   * \param tree Sub-directory tree path. E.g.: 'level_1/level_2'.
   *
   * \return true if sub-directory tree is newly created or false if
   * it was already existing.
   */
  static bool MakeDirTree( const QString& path, const QString& tree );

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

//
// Protected attributes.
protected:

  /**
  * Repository where all the elements saved by the application will be cached
  */
  QDir m_CacheDir;



  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:
  /**
   */
  void InitializeLocale();

  /**
   */
  bool LoadAndInstallTranslator( const QString& filename,
				 const QString& directory =QString(),
				 const QString& searchDelimiters =QString(),
				 const QString& suffix =QString() );

//
// Private attributes.
private:
  /**
   * Name of the cache directory
   */
  static const char* CACHE_DIR_NAME;



  /**
   */
  bool m_IsRunningFromBuildDir;

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
  return I18nApplication::Instance< I18nApplication >();
}

/*****************************************************************************/
const I18nApplication*
I18nApplication
::ConstInstance()
{
  return I18nApplication::ConstInstance< I18nApplication >();
}

/*****************************************************************************/
template< typename TApplication >
inline
TApplication*
I18nApplication
::Instance()
{
  return qobject_cast< TApplication* >( qApp );
}

/*****************************************************************************/
template< typename TApplication >
inline
const TApplication*
I18nApplication
::ConstInstance()
{
  return qobject_cast< const TApplication* >( qApp );
}

/*****************************************************************************/
const QDir&
I18nApplication
::GetCacheDir() const
{
  return m_CacheDir;
}

/*****************************************************************************/
const char*
I18nApplication
::GetCacheDirName() const
{
  return I18nApplication::CACHE_DIR_NAME;
}



} // end namespace 'mvd'

#endif // __I18nApplication_h
