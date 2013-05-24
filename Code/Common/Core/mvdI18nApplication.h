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
#include "mvdAlgorithm.h"
#include "mvdSystemError.h"
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
class AbstractModel;
class DatasetModel;


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
   */
  void Initialize();

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

  //
  // APPLICATION MODEL.
  //

 /**
  * \brief Access the AbstractModel of the MVC which is managed by the
  * application.
  *
  * \return The managed AbstractModel.
  */
  inline const AbstractModel* GetModel() const;

  /**
  * \brief Access the AbstractModel of the MVC which is managed by the
  * application.
  *
  * \return The managed AbstractModel.
   */
  inline AbstractModel* GetModel();

 /**
  * \brief Access the AbstractModel of the MVC which is managed by the
  * application and cast it into the given dynamic type.
  *
  * \return The managed TModel typename instance.
  */
  template< typename TModel >
  inline const TModel* GetModel() const;

  /**
  * \brief Access the AbstractModel of the MVC which is managed by the
  * application and cast it into the given dynamic type.
  *
  * \return The managed TModel typename instance.
   */
  template< typename TModel >
  inline TModel* GetModel();

  //
  // APPLICATION CACHE-DIRECTORY.
  //

  /**
   * \brief Create cache directory if not already existing.
   *
   * \param cacheDirStr path where the function will create the cache directory
   * \return true if cache directory has just been created or false if
   * it was already existing.
   */
  bool MakeCacheDir( const QString& cacheDirStr );


  /**
   * \brief setup elevation management
   * 
   */
  void ElevationSetup();

  //
  // APPLICATION SETTINGS.
  //

  /**
   */
  // TODO: Move method into ApplicationSettings class.
  inline bool HasSettingsKey( const QString& key );

  /**
   */
  // TODO: Move method into ApplicationSettings class.
  inline void StoreSettingsKey( const QString& key, const QVariant& value );

  /**
   */
  // TODO: Move method into Application class.
  inline QVariant RetrieveSettingsKey( const QString& key );

  /**
   */
  inline bool IsCacheDirValid() const;

  //
  // STATIC methods.
  //

  /**
   */
  static bool IsCacheDirValid( const QString& path );

  /**
   */
  static bool IsDirValid( const QString& path );

  /**
   * \brief Make directory tree in given path.
   *
   * Throws mvd::SystemError exception if sub-directory tree could not
   * have been created.
   *
   * \param path Location where to create sub-directory tree.
   * \param tree Sub-directory tree path. E.g.: 'level_1/level_2'.
   * \param dir resulting directory, if non-NULL.
   *
   * \return true if sub-directory tree is newly created or false if
   * it is already existing.
   */
  static bool MakeDirTree( const QString& path,
			   const QString& tree,
			   QDir* dir =NULL );

  /**
   * \brief Construct a consistent Dataset path-name.
   *
   * \param path Resulting path where the Dataset will be stored.
   * \param name Resulting name of the given Dataset.
   * \param imageFilename The image filename used to compute the path and name of the Dataset.
   */
  static
    void DatasetPathName( QString& path,
			  QString& name,
			  const QString& imageFilename );

  /**
   * \brief Load a DatasetModel from disk.
   *
   * \param imageFilename The input image filename.
   * \param with The width (in pixels) of the screen best fit.
   * \param height The height (in pixels) of the screen best fit.
   *
   * \return The loaded DatasetModel instance or NULL if the method has failed.
   */
  static
    DatasetModel* LoadDatasetModel( const QString& imageFilename,
				    int width,
				    int height );

  /**
   * \brief Get the cache directory.
   *
   * \return Return the cache directory.
   */
  inline const QDir& GetCacheDir() const;

  /**
   */
  // TODO: Remove method when Viewer/Application is updated to reuse factorized code of I18nApplication.
  inline QDir& GetCacheDir();
  
  /**
   * \brief Get the results directory (where output from
   * OTBApplicaitons are stored).
   *
   * \return Return the cache directory.
   */
  inline const QDir& GetResultsDir() const;

  /**
   */
  // TODO: Remove method when Viewer/Application is updated to reuse factorized code of I18nApplication.
  inline QDir& GetResultsDir();

//
// Public attributes.
public:
  /**
   * Name of the cache directory
   */
  static const char* DEFAULT_CACHE_DIR_NAME;

  /**
   * Name of the result cache directory
   */
  static const char* DEFAULT_CACHE_RESULT_DIR_NAME;

  /**
   */
  static const char* DATASET_EXT;

  /*-[ PUBLIC SLOTS SECTION ]-----------------------------------------------**/
public slots:
  /**
   * \brief Accessor used to changed model which is managed by the
   * Application.
   *
   * This method emits the AboutToChangeModel() and ModelChanged()
   * signals respectively, before and after, the model is changed.
   *
   * \param model The newly managed model.
   */
  // Method could be inline but it's better not new/delete in inline
  // methods (heap and memory-alignment contexts).
  void SetModel( AbstractModel* model );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:
  /**
   * \brief Signal emitted when the AbstractModel of the MVC is about
   * to be changed.
   *
   * This signal could be slotted in order to disconnect previously
   * managed model.
   *
   * \param model The newly managed model (previous model can still
   * be accessed using the GetModel() methods).
   */
  void AboutToChangeModel( const AbstractModel* model );

  /**
   * \brief Signal emitted when the AbstractModel of the MVC has been
   * changed.
   *
   * This signal coulb be slotted in order to connect newly managed
   * model.
   *
   * \param model The newly selected model.
   */
  void ModelChanged( AbstractModel* model );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /**
   */
  virtual void virtual_InitializeCore() =0;

  /**
   */
  void InitializeCore( const QString& appName,
		       const QString& appVersion,
		       const QString& orgName,
		       const QString& orgDomain );

  /**
   */
  inline void SynchronizeSettings();

//
// Protected attributes.
protected:



  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   */
  void InitializeLocale();

  /**
   */
  void InitializeSettings();

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
  * \brief Directory where all cached files are stored (repository of datasets).
  */
  // TODO: Move I18nApplication::m_CacheDir to private section.
  QDir m_CacheDir;

  /**
  * \brief Directory where all result files from OTB applications are
  * stored 
  */
  QDir m_ResultsDir;

  /**
   * \brief Application settings 
   */
  QSettings* m_Settings;

  /**
   * \brief AbstractModel of the Model-View-Controller design pattern
   * which is managed by the application.
   */
  AbstractModel* m_Model;

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
inline
const AbstractModel*
I18nApplication
::GetModel() const
{
  return m_Model;
}

/*****************************************************************************/
inline
AbstractModel*
I18nApplication
::GetModel()
{
  return m_Model;
}

/*****************************************************************************/
template< typename TModel >
inline
const TModel*
I18nApplication
::GetModel() const
{
  return qobject_cast< const TModel* >( m_Model );
}

/*****************************************************************************/
template< typename TModel >
inline
TModel*
I18nApplication
::GetModel()
{
  return qobject_cast< TModel* >( m_Model );
}

/*****************************************************************************/
const QDir&
I18nApplication
::GetCacheDir() const
{
  return m_CacheDir;
}

/*****************************************************************************/
const QDir&
I18nApplication
::GetResultsDir() const
{
  return m_ResultsDir;
}

/*****************************************************************************/
inline
bool
I18nApplication
::IsCacheDirValid() const
{
  return I18nApplication::IsCacheDirValid( m_CacheDir.path() );
}

/*****************************************************************************/
QDir&
I18nApplication
::GetCacheDir()
{
  return m_CacheDir;
}

/*****************************************************************************/
QDir&
I18nApplication
::GetResultsDir()
{
  return m_ResultsDir;
}

/*****************************************************************************/
inline
bool
I18nApplication
::HasSettingsKey( const QString& key )
{
  assert( m_Settings!=NULL );

  SynchronizeSettings();

  return m_Settings->contains( key );
}

/*****************************************************************************/
inline
void
I18nApplication
::StoreSettingsKey( const QString& key, const QVariant& value )
{
  assert( m_Settings!=NULL );

  m_Settings->setValue( key, value );

  SynchronizeSettings();
}

/*****************************************************************************/
inline
QVariant
I18nApplication
::RetrieveSettingsKey( const QString& key )
{
  assert( m_Settings!=NULL );

  SynchronizeSettings();

  return m_Settings->value( key );
}

/*****************************************************************************/
inline
void
I18nApplication
::SynchronizeSettings()
{
  assert( m_Settings!=NULL );

  m_Settings->sync();

  switch( m_Settings->status() )
    {
    case QSettings::NoError:
      // Ok.
      break;

    case QSettings::AccessError:
      throw SystemError( ToStdString( tr( "Settings-file access error." ) ) );
      break;

    case QSettings::FormatError:
      throw SystemError( ToStdString( tr( "Settings-file format error." ) ) );
      break;

    default:
      // In case when a new enum value if added in the API.
      assert( false );
      break;
    }
}

} // end namespace 'mvd'

#endif // __I18nApplication_h
