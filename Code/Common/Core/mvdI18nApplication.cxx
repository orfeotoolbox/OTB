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
#include "mvdI18nApplication.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbDEMHandler.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"
#include "mvdDatasetModel.h"
#include "mvdSystemError.h"
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::I18nApplication

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */

const char* I18nApplication::DEFAULT_CACHE_DIR_NAME = "mvd2";

const char* I18nApplication::DEFAULT_CACHE_RESULT_DIR_NAME = "result";

const char* I18nApplication::DATASET_EXT = ".ds";

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*******************************************************************************/
bool
I18nApplication
::IsCacheDirValid( const QString& path )
{
  QDir dir( path );
  QFileInfo fileInfo( path );

  return
    fileInfo.exists() &&
    fileInfo.isDir() &&
    fileInfo.isReadable() &&
    fileInfo.isWritable() &&
    dir.dirName()==I18nApplication::DEFAULT_CACHE_DIR_NAME;
}

/*******************************************************************************/
bool
I18nApplication
::IsResultsDirValid( const QString& path )
{
  QDir dir( path );
  QFileInfo fileInfo( path );

  return
    fileInfo.exists() &&
    fileInfo.isDir() &&
    fileInfo.isReadable() &&
    fileInfo.isWritable() &&
    dir.dirName()==I18nApplication::DEFAULT_CACHE_RESULT_DIR_NAME;
}

/*******************************************************************************/
bool
I18nApplication
::MakeDirTree( const QString& path, const QString& tree, QDir* dir )
{
  QDir pathDir( path );

  /*
  qDebug() << path;
  qDebug() << pathDir.currentPath();
  qDebug() << pathDir.path();
  */

  if( !pathDir.exists() )
    throw SystemError( ToStdString( QString( "('%1')" ).arg( path ) ) );

  QDir treeDir( pathDir.filePath( tree ) );
  if( treeDir.exists() )
    return false;

  if( !pathDir.mkpath( tree ) )
    throw SystemError(
      ToStdString(
	QString( "('%1')" ).arg( pathDir.filePath( tree ) )
      )
    );

  if( !pathDir.cd( tree ) )
    throw SystemError(
      ToStdString(
	QString( "('%1')" ).arg(
	  pathDir.filePath( tree )
	)
      )
    );

  if( dir!=NULL )
    {
    *dir = pathDir;
    }

  return true;
}

/*****************************************************************************/
void
I18nApplication
::DatasetPathName( QString& path,
		   QString& name,
		   const QString& imageFilename )
{
  // convenient QFileInfo
  QFileInfo fileInfo( imageFilename );

  // Dataset is stored into application cache-directory.
  // E.g. '$HOME/<CACHE_DIR>'
  path = I18nApplication::Instance()->GetCacheDir().path();
  
  // get the md5 of the filename
  QByteArray result = QCryptographicHash::hash(fileInfo.absoluteFilePath().toAscii(), 
                                               QCryptographicHash::Md5);

  // store the md5 + the dataset extension at the end
  name = result.toHex() + I18nApplication::DATASET_EXT;
}

/*****************************************************************************/
DatasetModel*
I18nApplication
::LoadDatasetModel( const QString& imageFilename,
		    int width,
		    int height )
{
  // New model.
  DatasetModel* model = new DatasetModel();

  // Retrive path and name.
  QString path;
  QString name;

  I18nApplication::DatasetPathName( path, name, imageFilename );
  qDebug() << "Dataset path: " << path;
  qDebug() << "Dataset name: " << name;

  // Setup QObject
  model->setObjectName( QDir( path ).filePath( name ) );

  try
    {
    // try if the filename is valid
    VectorImageModel::EnsureValidImage(imageFilename);

    // get the basename from the filename to be used as an Alias
    QFileInfo finfo( imageFilename );

    // Build model (relink to cached data).
    DatasetModel::BuildContext context( path, name, finfo.baseName(), width, height );
    model->BuildModel( &context );

    // Load image if DatasetModel is empty.
    if( !model->HasSelectedImageModel() )
      {
      // Import image from filename given (w; h) size to choose
      // best-fit resolution.
      model->ImportImage( imageFilename, width, height );
      }
    }

  catch( std::exception& exc )
    {
    delete model;
    model = NULL;

    throw;
    }
 
  return model;
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
I18nApplication
::I18nApplication( int& argc, char** argv ) :
  QApplication( argc, argv ),
  m_CacheDir(),
  m_Settings( NULL ),
  m_Model( NULL ),
  m_IsRunningFromBuildDir( false )
{
}

/*******************************************************************************/
I18nApplication
::~I18nApplication()
{
}

/*******************************************************************************/
void
I18nApplication
::Initialize()
{
  // Initialize internationlization.
  InitializeLocale();

  //
  // Force numeric options of locale to "C"
  // See issue #635
  //
  // TODO: Move into I18nApplication.
  setlocale( LC_NUMERIC, "C" );

  // Initialize QCoreApplication.
  virtual_InitializeCore();

  // Initialize settings.
  InitializeSettings();

  // Elevation setup
  ElevationSetup();
}

/*******************************************************************************/
void
I18nApplication
::SetModel( AbstractModel* model )
{
  emit AboutToChangeModel( model );

  delete m_Model;

  m_Model = model;

  if( model!=NULL )
    m_Model->setParent( this );

  emit ModelChanged( m_Model );
}

/*******************************************************************************/
bool
I18nApplication
::MakeCacheDir( const QString& path )
{
  qDebug() << this << "::MakeCacheDir(" << path << ")";

  //
  // Check path.
  QDir homeDir( path );

  if (!homeDir.exists())
    SystemError( ToStdString( QString( "('%1')" ).arg( homeDir.path() ) ) );

  //
  // Create cache-dir.
  bool isNew = I18nApplication::MakeDirTree(
    homeDir.path(),
    I18nApplication::DEFAULT_CACHE_DIR_NAME,
    &m_CacheDir
  );

  //
  // Remember cache-dir.
  StoreSettingsKey( "cacheDir", QDir::cleanPath( m_CacheDir.path() ) );
  
  //
  // Construct result-dir path.
  I18nApplication::MakeDirTree(
    m_CacheDir.path(),
    I18nApplication::DEFAULT_CACHE_RESULT_DIR_NAME,
    &m_ResultsDir
  );

  //
  // Remember result-dir
  StoreSettingsKey( "resultsDir", QDir::cleanPath( m_ResultsDir.path() ) );

  //
  // Result.
  return isNew;
}

/*******************************************************************************/
void
I18nApplication
::ElevationSetup()
{
  QSettings settings;

  otb::DEMHandler::Pointer demHandlerInstance = otb::DEMHandler::Instance();

  if(settings.contains("geoidPathActive") && settings.value("geoidPathActive").toBool())
    {
    qDebug() << "Settings/GeoidFile:" <<settings.value("geoidPath").toString() ;
    try
      {
      demHandlerInstance->OpenGeoidFile(settings.value("geoidPath").toString().toStdString());
      }
    catch(itk::ExceptionObject & err)
      {
      qDebug() <<tr("An error occured while loading the geoid file, no geoid file will be used.");
      qDebug()<<tr("Error: ")<<err.what();
      }
    }
  if(settings.contains("srtmDirActive") && settings.value("srtmDirActive").toBool())
    {
    qDebug() << "Settings/DEMDir:" <<settings.value("srtmDir").toString();
    try
      {
      demHandlerInstance->OpenDEMDirectory(settings.value("srtmDir").toString().toStdString());
      }
    catch(itk::ExceptionObject & err)
      {
      qDebug() <<tr("An error occured while loading the DEM directory, no DEM will be used.");
      qDebug()<<tr("Error: ") << err.what();
      }
    }
}

/*******************************************************************************/
void
I18nApplication
::InitializeLocale()
{
  QTextCodec::setCodecForTr( QTextCodec::codecForName( "utf8" ) );
  QTextCodec::setCodecForLocale( QTextCodec::codecForName("utf8") );
  QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );

  //
  // 1. default UI language is english (no translation).
  QLocale sys_lc( QLocale::system() );
  if( sys_lc.language() == QLocale::C ||
      ( sys_lc.language() == QLocale::English &&
        sys_lc.country() == QLocale::UnitedStates ) )
    {
    return;
    }

  //
  // 2. Choose i18n path between build dir and install dir.

  // Begin from the executable path
  QDir bin_dir( QDir::cleanPath(QCoreApplication::applicationDirPath()) );
  qDebug() << tr( "Executable dir : %1" ).arg( bin_dir.path() );
  
  // Go up in the directory hierarchy until we have a candidate install prefix
  bool prefixFound = false;
  QDir prefix( bin_dir );
  while ( prefix.cdUp() )
    {
    if ( QDir(prefix).cd(Monteverdi2_INSTALL_BIN_DIR) )
      {
      prefixFound = true;
      break;
      }
    }
  
  if (prefixFound)
    {
    qDebug() << tr( "Candidate install prefix found : %1" ).arg( prefix.path() );
    }
  else
    {
    QString message( tr( "Unable to locate translation files" ) );
    qDebug() << message;
    QMessageBox::critical( NULL, tr( "Critical error" ), message );
    return;
    }
  
  QDir i18n_dir;

  // At this point the candidate install prefix can also be the build dir root
  if ( prefix.exists( Monteverdi2_CONFIGURE_FILE )
       && prefix.exists("i18n") )
    {
    m_IsRunningFromBuildDir = true;

    // Report found build dir root
    qDebug() << tr( "Running from build directory '%1'." ).arg( prefix.path() );
    
    // Go into the i18n dir (it exists we just checked for it)
    i18n_dir = prefix;
    i18n_dir.cd("i18n");
    
    qDebug() << tr( "Using translation dir '%1'." ).arg( i18n_dir.path() );
    }
  else
    {
    m_IsRunningFromBuildDir = false;
    
    // Report found install prefix
    qDebug() << tr( "Running from install directory '%1'." ).arg( prefix.path() );

    // Go into the i18n dir configured at cmake-time
    i18n_dir = prefix;
    
    if (i18n_dir.cd(Monteverdi2_INSTALL_DATA_I18N_DIR))
      {
      qDebug() << tr( "Using translation dir '%1'." ).arg( i18n_dir.path() );
      }
    else
      {
      QString message( tr( "Failed to access translation-files directory '%1'" )
                       .arg(QDir::cleanPath(prefix.path()
                             + QDir::separator()
                             + Monteverdi2_INSTALL_DATA_I18N_DIR)) );
      qDebug() << message;
      QMessageBox::critical( NULL, tr( "Critical error" ), message );
      return;
      }
    }
  
  //
  // 3.1 Stack Qt translator.
  LoadAndInstallTranslator(
    "qt_" + sys_lc.name(),
    QLibraryInfo::location( QLibraryInfo::TranslationsPath  )
  );

  //
  // 3.2 Stack Monteverdi2 translator as prioritary over Qt translator.
  LoadAndInstallTranslator( sys_lc.name(), i18n_dir.path() );

  // TODO: Record locale translation filename(s) used in UI component (e.g.
  // AboutDialog, Settings dialog, Information dialog etc.)
}

/*******************************************************************************/
void
I18nApplication
::InitializeCore( const QString& appName,
		  const QString& appVersion,
		  const QString& orgName,
		  const QString& orgDomain )
{
  setObjectName( "Application" );

  //
  // Setup application tags.
  //
  QCoreApplication::setApplicationName(
    appName
  );
  QCoreApplication::setApplicationVersion(
    appVersion
  );

  //
  // Setup organization tags.
  //
  QCoreApplication::setOrganizationName( orgName );
  QCoreApplication::setOrganizationDomain( orgDomain );

#ifndef Q_WS_MAC
    setWindowIcon( QIcon( QLatin1String( ":/images/application_icon" ) ) );
#endif
}

/*******************************************************************************/
void
I18nApplication
::InitializeSettings()
{
  //
  // Create settings proxy.
  m_Settings = new QSettings(
    // TODO: Change QSettings::NativeFormat by QSettings::IniFormat.
    QSettings::NativeFormat,
    QSettings::UserScope,
    QCoreApplication::organizationName(),
    QCoreApplication::applicationName(),
    this
  );

  //
  // Restore cache directory.
  QVariant value( RetrieveSettingsKey( "cacheDir" ) );

  if( !value.isNull() )
    {
    QString path( value.toString() );

    qDebug() << "Settings/cacheDir:" << path;

    if( I18nApplication::IsCacheDirValid( path ) )
      {
      m_CacheDir.setPath( path );
      }
    }

  //
  // Restore result directory.
  QVariant resultDir( RetrieveSettingsKey( "resultsDir" ) );

  if( !resultDir.isNull() )
    {
    QString resultPath( resultDir.toString() );

    qDebug() << "Settings/resultsDir:" << resultPath;

    if( I18nApplication::IsResultsDirValid( resultPath  ) )
      {
      m_ResultsDir.setPath( resultPath );
      }
    }
}

/*******************************************************************************/
bool
I18nApplication
::LoadAndInstallTranslator(const QString& filename,
                           const QString& directory,
                           const QString& searchDelimiters,
                           const QString& suffix )
{
  QString filename_ext(
    filename +
    ( suffix.isNull()
      ? ".qm"
      : suffix )
  );

  // (a) Do need to new QTranslator() here!
  QTranslator* lc_translator = new QTranslator( this );

  if( !lc_translator->load( filename, directory, searchDelimiters, suffix ) )
    {
    QString message(
      tr( "Failed to load '%1' translation file from '%2'." )
      .arg( filename_ext )
      .arg( directory )
    );

    // TODO: Use log system to trace error while loading locale translation file.

    qWarning() << message;

    // TODO: morph into better HMI design.
#if defined( _DEBUG )
    QMessageBox::warning( NULL, tr( "Warning!" ), message );
#endif

    return false;
    }

  // (a) ...because QTranslator needs to be alive during the whole
  // lifespan of the application.
  QCoreApplication::installTranslator( lc_translator );

  QString message(
    tr( "Successfully loaded '%1' translation file from '%2'." )
    .arg( filename_ext )
    .arg( directory )
  );

  // TODO: Log locale translation filename used.

  qDebug() << message;

  return true;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
