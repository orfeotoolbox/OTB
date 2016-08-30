/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdI18nCoreApplication_h
#define __mvdI18nCoreApplication_h

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
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdAlgorithm.h"
#include "mvdSystemError.h"
#include "mvdTypes.h"
#endif //tag=QT4-boost-compatibility

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
class VectorImageModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class I18nCoreApplication
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief The MVD2 core-application (which is different from
 * QCoreApplication).
 * 
 * I18nCoreApplication do not derive from QCoreApplication to
 * prevent a dread diamong multiple inheritance scheme which could not
 * be resolved using virtual inheritance (because Qt is not designed
 * for).
 *
 * I18nCoreApplication does not derive from QApplication to
 * prevent package .../Common/Core to depend on QtGui package (which
 * would have broken the Common/Core-Common/Gui package layout).
 *
 * The dread diamond multiple inheritance scheme is solved using
 * aggregation.
 *
 * QCoreApplication is passed as argument of I18nCoreApplication
 * constructor. So, is user application must derive QApplication (to,
 * for example, provide sessio management), the specialized instance
 * can be passed as argument of constructor.
 */
class ITK_EXPORT I18nCoreApplication
  : public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public constants & types.
public:
  enum SettingsKey
  {
    SETTINGS_KEY_GEOID_PATH,
    SETTINGS_KEY_GEOID_PATH_ACTIVE,
    SETTINGS_KEY_OVERVIEWS_ENABLED,
    SETTINGS_KEY_OVERVIEWS_SIZE,
    SETTINGS_KEY_RESOLUTION,
    SETTINGS_KEY_RESULTS_DIR,
    SETTINGS_KEY_SRTM_DIR,
    SETTINGS_KEY_SRTM_DIR_ACTIVE,
    SETTINGS_KEY_TILE_SIZE,
    //
    SETTINGS_KEY_COUNT
  };

//
// Public methods.
public:

  /**
   * \brief Constructor.
   *
   * \param qtApp The parent Qt application of this MVD2 application.
   */
  I18nCoreApplication( QCoreApplication* qtApp );

  /** \brief Destructor. */
  virtual ~I18nCoreApplication();

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
   * \return The singleton instance of I18nCoreApplication.
   */
  inline static I18nCoreApplication* Instance();

  /**
   * \brief Get the singleton constant instance of application as a
   * I18Application pointer.
   *
   * \return The singleton constant instance of I18nCoreApplication.
   */
  inline static const I18nCoreApplication* ConstInstance();

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
   * \brief setup elevation management
   * 
   */
  bool ElevationSetup();

  //
  // APPLICATION SETTINGS.
  //

  /**
   */
  // TODO: Move method into ApplicationSettings class.
  inline bool HasSettingsKey( SettingsKey ) const;

  /**
   */
  // TODO: Move method into ApplicationSettings class.
  inline void StoreSettingsKey( SettingsKey, const QVariant& value );

  /**
   */
  // TODO: Move method into Application class.
  inline QVariant RetrieveSettingsKey( SettingsKey ) const;

  /**
   */
  // TODO: Move method into ApplicationSettings class.
  inline bool HasSettingsKey( const QString & key ) const;

  /**
   */
  // TODO: Move method into ApplicationSettings class.
  inline void StoreSettingsKey( const QString & key, const QVariant& value );

  /**
   */
  // TODO: Move method into Application class.
  inline QVariant RetrieveSettingsKey( const QString & key ) const;

  //
  // STATIC methods.
  //

  /**
   */
  static bool IsResultsDirValid( const QString& path );
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
   * \param imageFilename The image filename used to compute the path
   * and name of the Dataset.
   *
   * \return The MD5 hash-code generated from to full absolute
   * pathname of image file.
   */
  static
    QString DatasetPathName( QString& name,
                             const QString& imageFilename );

  /**
   */
  static
    VectorImageModel *
    LoadImageModel( const QString & filename,
                    int width,
                    int height,
                    QObject * p =NULL );

  /**
   */
  static
    void DeleteDatasetModel( const QString & path, const QString & hash );

  //
  // NON-STATIC methods.
  //

  /**
   * \brief Get the results directory (where output from
   * OTBApplicaitons are stored).
   *
   * \return Return the cache directory.
   */
  inline const QDir& GetResultsDir() const;

  /**
   */
  // TODO: Remove method when Viewer/Application is updated to reuse factorized code of I18nCoreApplication.
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
   */
  void InitializeCore( const QString& appName,
		       const QString& appVersion,
		       const QString& orgName,
		       const QString& orgDomain );

  /**
   */
  inline void SynchronizeSettings() const;

//
// Protected attributes.
protected:



  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  //
  // Static methods.
  //

  /**
   * \brief Handles Qt messages thrown by calls to qDebug(),
   * qWarning(), qCritical(), qFatal().
   *
   * This is a callback method registered into Qt message handling
   * system.
   *
   * \param type Type of caught message.
   * \param message Content of caught message.
   */
  static void HandleQtMessage( QtMsgType type, const char* message );

  /**
   */
  virtual void virtual_InitializeCore() =0;

  /**
   */
  void InitializeLocale();

  /**
   */
  void InitializeSettings();

  /**
   */
  void LoadAndInstallTranslator( const QString& filename,
				 const QString& directory =QString(),
				 const QString& searchDelimiters =QString(),
				 const QString& suffix =QString() );

//
// Private attributes.
private:

  /**
   */
  static const char* SETTINGS_KEYS[ SETTINGS_KEY_COUNT ];

  /**
   * \brief I18nCoreApplication singleton instance.
   */
  static I18nCoreApplication* m_Instance;

  /**
  * \brief Directory where all result files from OTB applications are
  * stored 
  */
  QDir m_ResultsDir;

  /**
   * \brief Application settings 
   */
  QSettings * m_Settings;

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
I18nCoreApplication*
I18nCoreApplication
::Instance()
{
  return I18nCoreApplication::m_Instance;
}

/*****************************************************************************/
const I18nCoreApplication*
I18nCoreApplication
::ConstInstance()
{
  return I18nCoreApplication::m_Instance;
}

/*****************************************************************************/
template< typename T >
inline
T*
I18nCoreApplication
::Instance()
{
  return qobject_cast< T* >( I18nCoreApplication::Instance() );
}

/*****************************************************************************/
template< typename T >
inline
const T*
I18nCoreApplication
::ConstInstance()
{
  return qobject_cast< const T* >( I18nCoreApplication::ConstInstance() );
}

/*****************************************************************************/
inline
const AbstractModel*
I18nCoreApplication
::GetModel() const
{
  return m_Model;
}

/*****************************************************************************/
inline
AbstractModel*
I18nCoreApplication
::GetModel()
{
  return m_Model;
}

/*****************************************************************************/
template< typename TModel >
inline
const TModel*
I18nCoreApplication
::GetModel() const
{
  return qobject_cast< const TModel* >( m_Model );
}

/*****************************************************************************/
template< typename TModel >
inline
TModel*
I18nCoreApplication
::GetModel()
{
  return qobject_cast< TModel* >( m_Model );
}

/*****************************************************************************/
const QDir&
I18nCoreApplication
::GetResultsDir() const
{
  return m_ResultsDir;
}

/*****************************************************************************/
QDir&
I18nCoreApplication
::GetResultsDir()
{
  return m_ResultsDir;
}

/*****************************************************************************/
inline
bool
I18nCoreApplication
::HasSettingsKey( SettingsKey key ) const
{
  return HasSettingsKey( I18nCoreApplication::SETTINGS_KEYS[ key ] );
}

/*****************************************************************************/
inline
void
I18nCoreApplication
::StoreSettingsKey( SettingsKey key, const QVariant & value )
{
  StoreSettingsKey( I18nCoreApplication::SETTINGS_KEYS[ key ], value );
}

/*****************************************************************************/
inline
QVariant
I18nCoreApplication
::RetrieveSettingsKey( SettingsKey key ) const
{
  return RetrieveSettingsKey( I18nCoreApplication::SETTINGS_KEYS[ key ] );
}

/*****************************************************************************/
inline
bool
I18nCoreApplication
::HasSettingsKey( const QString & key ) const
{
  assert( m_Settings!=NULL );

  SynchronizeSettings();

  return m_Settings->contains( key );
}

/*****************************************************************************/
inline
void
I18nCoreApplication
::StoreSettingsKey( const QString & key, const QVariant& value )
{
  assert( m_Settings!=NULL );

  // qDebug() << this << "::StoreSettingsKey(" << key << ", " << value << ")";

  m_Settings->setValue( key, value );

  SynchronizeSettings();
}

/*****************************************************************************/
inline
QVariant
I18nCoreApplication
::RetrieveSettingsKey( const QString & key ) const
{
  assert( m_Settings!=NULL );

  SynchronizeSettings();

  return m_Settings->value( key );
}

/*****************************************************************************/
inline
void
I18nCoreApplication
::SynchronizeSettings() const
{
  assert( m_Settings!=NULL );

  m_Settings->sync();

  switch( m_Settings->status() )
    {
    case QSettings::NoError:
      // Ok.
      break;

    case QSettings::AccessError:
      throw SystemError( ToStdString( tr( "Cannot access settings file." ) ) );
      break;

    case QSettings::FormatError:
      throw SystemError( ToStdString( tr( "Bad settings file format." ) ) );
      break;

    default:
      // In case when a new enum value if added in the API.
      assert( false );
      break;
    }
}

} // end namespace 'mvd'

#endif // __I18nCoreApplication_h
