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
#include "mvdI18nCoreApplication.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#ifdef _WIN32
#include <windows.h>
#endif
#include <exception>

//
// OTB includes (sorted by alphabetic order)
#include "otbDEMHandler.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"
#include "mvdSystemError.h"
#include "mvdVectorImageModel.h"


#define FORCE_QDEBUG 0
#define FORCE_QWARNING 0
#define FORCE_QCRITICAL 0
#define FORCE_QFATAL 0


namespace mvd
{
/*
  TRANSLATOR mvd::I18nCoreApplication

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */

const char* I18nCoreApplication::DEFAULT_CACHE_DIR_NAME = "mvd2";

const char* I18nCoreApplication::DEFAULT_CACHE_RESULT_DIR_NAME = "result";

const char* I18nCoreApplication::DATASET_EXT = ".ds";

const char* I18nCoreApplication::SETTINGS_KEYS[ SETTINGS_KEY_COUNT ] =
{
  "geoidPath",
  "geoidPathActive",
  "overviewsEnabled",
  "overviewsSize",
  "resolutionAlgorithm",
  "resultsDir",
  "srtmDir",
  "srtmDirActive",
  "tileSize",
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

I18nCoreApplication* I18nCoreApplication::m_Instance = NULL;

/*******************************************************************************/
bool
I18nCoreApplication
::IsResultsDirValid( const QString& path )
{
  QDir dir( path );
  QFileInfo fileInfo( path );

  return
    fileInfo.exists() &&
    fileInfo.isDir() &&
    fileInfo.isReadable() &&
    fileInfo.isWritable();
}

/*******************************************************************************/
bool
I18nCoreApplication
::MakeDirTree( const QString& path, const QString& tree, QDir* dir )
{
  QDir pathDir( path );

  if( !pathDir.exists() )
    throw SystemError( ToStdString( QString( "('%1')" ).arg( path ) ) );

  QDir treeDir( pathDir.filePath( tree ) );
  if( treeDir.exists() )
    {
    if( dir!=NULL )
      {
      *dir = treeDir;
      }

    return false;
    }

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
QString
I18nCoreApplication
::DatasetPathName( QString& name,
		   const QString& imageFilename )
{
  // convenient QFileInfo
  QFileInfo fileInfo( imageFilename );

  // get the md5 of the filename
  QByteArray result =
    QCryptographicHash::hash( fileInfo.absoluteFilePath().toAscii(), 
			      QCryptographicHash::Md5 );

  // MD5 hash-code.
  QString hash( result.toHex() );

  // store the md5 + the dataset extension at the end
  name = hash + I18nCoreApplication::DATASET_EXT;

  // Return hash-code for further use.
  return hash;
}

/*****************************************************************************/
VectorImageModel *
I18nCoreApplication
::LoadImageModel( const QString & filename,
                  int width,
                  int height,
                  QObject * p )
{
  try
    {
    VectorImageModel::EnsureValidImage( filename );
    }
  catch( ... )
    {
    throw;
    }

  VectorImageModel * imageModel = NULL;

  try
    {
    AbstractImageModel::BuildContext context( filename );

    imageModel = new VectorImageModel( p );

    imageModel->SetFilename( filename, width, height );

    imageModel->BuildModel( &context );
    }
  catch( ... )
    {
    delete imageModel;
    imageModel = NULL;

    throw;
    }

  return imageModel;
}

/*****************************************************************************/
void
I18nCoreApplication
::DeleteDatasetModel( const QString & path, const QString & hash )
{
  QFileInfo finfo( path, hash );

  QDir dir( finfo.filePath() );

  QFileInfoList fileInfos(
    dir.entryInfoList( QDir::NoDotAndDotDot | QDir::Files )
  );

  for( QFileInfoList::const_iterator it( fileInfos.begin() );
       it!=fileInfos.end();
       ++it )
    {
    if( !dir.remove( it->fileName() ) )
      throw SystemError(
        ToStdString(
          tr( "Failed to remove file '%1'." ).arg( it->filePath() )
        )
      );
    }

  QDir parentDir( dir );

  if( !parentDir.cdUp() )
    throw SystemError(
      ToStdString(
        tr( "Failed to access parent directory of '%1'." ).arg( dir.path() )
      )
    );

  if( !parentDir.rmdir( hash  ) )
    throw SystemError(
      ToStdString(
        tr( "Failed to remove dataset directory '%1'." ).arg( dir.path() )
      )
    );
}

/*****************************************************************************/
void
I18nCoreApplication
::HandleQtMessage( QtMsgType type, const char* message )
{
  switch( type )
    {
    //
    // DEBUG
    case QtDebugMsg:
#if (ECHO_QDEBUG) || FORCE_QDEBUG
#if _WIN32
      OutputDebugString( message );
      OutputDebugString( "\n" );
#endif
      fprintf( stderr, "%s\n", message );
#endif
#if LOG_QDEBUG
      assert( false && "Not yet implemented!" );
#endif
      break;
    //
    // WARNING
    case QtWarningMsg:
#if (ECHO_QWARNING) || FORCE_QWARNING
#if _WIN32
      OutputDebugString( "WARNG> " );
      OutputDebugString( message );
      OutputDebugString( "\n" );
#endif
      fprintf( stderr, tr( "WARNG> %s\n" ).toLatin1().constData(), message );
#endif
#if LOG_QWARNING
      assert( false && "Not yet implemented!" );
#endif
      break;
    //
    // CRITICAL
    case QtCriticalMsg:
#if (ECHO_QCRITICAL) || FORCE_QCRITICAL
#if _WIN32
      OutputDebugString( "ERROR> " );
      OutputDebugString( message );
      OutputDebugString( "\n" );
#endif
      fprintf( stderr, tr( "ERROR> %s\n" ).toLatin1().constData(), message );
#endif
#if LOG_QCRITICAL
      assert( false && "Not yet implemented!" );
#endif
#if THROW_QCRITICAL
      throw std::runtime_error(
	ToStdString(
	  tr( "Error: " )
	  .arg( message )
	)
      );
#endif
      break;
    //
    // FATAL
    case QtFatalMsg:
#if (ECHO_QFATAL) || FORCE_QFATAL
#if _WIN32
      OutputDebugString( "FATAL> " );
      OutputDebugString( message );
      OutputDebugString( "\n" );
#endif
      fprintf(
	stderr,
	tr( "FATAL> %s\n" ).toLatin1().constData(),
	message
      );
#endif
#if LOG_QFATAL
      assert( false && "Not yet implemented!" );
#endif
#if THROW_QFATAL
      throw std::runtime_error(
	ToStdString(
	  tr( "Fatal error: " )
	  .arg( message )
	)
      );
#endif
      abort();
      break;

    default:
      assert( false && "Unhandled message QtMsgType!" );
      break;
    }
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
I18nCoreApplication
::I18nCoreApplication( QCoreApplication* qtApp ) :
  QObject( qtApp ),
  m_Settings( NULL ),
  m_Model( NULL ),
  m_IsRunningFromBuildDir( false )
{
  if( m_Instance!=NULL )
    {
    throw std::runtime_error(
      ToStdString(
        tr( "I18nCoreApplication is a singleton class!" )
      )
    );
    }

  if( qtApp==NULL )
    {
    throw std::invalid_argument(
      ToStdString(
	tr( "Class 'I18nCoreApplication' instance must be provided a "
	    "QCoreApplication' pointer at construction time!" )
      )
    );
    }

  qInstallMsgHandler( I18nCoreApplication::HandleQtMessage );

  m_Instance = this;
}

/*******************************************************************************/
I18nCoreApplication
::~I18nCoreApplication()
{
}

/*******************************************************************************/
void
I18nCoreApplication
::Initialize()
{
  // Initialize internationlization.
  InitializeLocale();

  //
  // Force numeric options of locale to "C"
  // See issue #635
  //
  // TODO: Check it is still needed here!
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
I18nCoreApplication
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
I18nCoreApplication
::ElevationSetup()
{
  assert( !otb::DEMHandler::Instance().IsNull() );

  otb::DEMHandler::Pointer demHandlerInstance( otb::DEMHandler::Instance() );

  bool geoidUpdated = false;

  if( I18nCoreApplication::HasSettingsKey(
	I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE ) &&
      I18nCoreApplication::RetrieveSettingsKey(
	I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE ).toBool() )
    {
    qDebug() <<
      "Settings/GeoidFile:" <<
      I18nCoreApplication::RetrieveSettingsKey(
	I18nCoreApplication::SETTINGS_KEY_GEOID_PATH
      ).toString();

    try
      {
      QString filename(
	I18nCoreApplication::RetrieveSettingsKey(
	  I18nCoreApplication::SETTINGS_KEY_GEOID_PATH
	).toString()
      );

      geoidUpdated =
	demHandlerInstance->OpenGeoidFile(
	  QFile::encodeName(
	    filename
	  )
	);

      // BUGFIX: When geoid file has not been updated by
      // otb::DEMHandler, the filename may be erronous and unchecked
      // so, add a check, in this case, to report input error to the
      // user.
      if( !geoidUpdated )
	{
	QFileInfo finfo( filename );

	if( !finfo.exists() )
	  throw std::runtime_error(
	    ToStdString(
	      tr( "Geoid file '%1' not found!" )
	      .arg( filename )
	    )
	  );
	}
      }
    catch( const std::exception & err )
      {
      qWarning()
	<< ToStdString( tr( "An error occured while loading the geoid file, "
			    "no geoid file will be used:" ) ).c_str()
	<< err.what();

      throw;
      }
    }
  else
    geoidUpdated = true;

  if( I18nCoreApplication::HasSettingsKey(
	I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE ) &&
      I18nCoreApplication::RetrieveSettingsKey(
	I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE ).toBool() )
    {
    qDebug() <<
      "Settings/DEMDir:" <<
      I18nCoreApplication::RetrieveSettingsKey(
	I18nCoreApplication::SETTINGS_KEY_SRTM_DIR
      ).toString();

    try
      {
      demHandlerInstance->ClearDEMs();

      demHandlerInstance->OpenDEMDirectory(
	QFile::encodeName(
	  I18nCoreApplication::RetrieveSettingsKey(
	    I18nCoreApplication::SETTINGS_KEY_SRTM_DIR
	  )
	  .toString()
	)
      );
      }
    catch( const std::exception & err )
      {
      qWarning()
	<< ToStdString( tr( "An error occured while loading the DEM directory, "
			    "no DEM will be used:" ) ).c_str()
	<< err.what();

      throw;
      }
    }
  else
    {
    otb::DEMHandler::Instance()->ClearDEMs();
    }

  return geoidUpdated;
}

/*******************************************************************************/
void
I18nCoreApplication
::InitializeLocale()
{
  {
  typedef QList< QByteArray > ByteArrayList;

  ByteArrayList codecs( QTextCodec::availableCodecs() );

  qDebug() << "Available codecs:";

  foreach( const QByteArray & codec, codecs )
    qDebug() << "\t" << codec;
  }

  // Literal strings to be translated are UTF-8 encoded because source
  // files are UTF-8 encoded.
  QTextCodec::setCodecForTr( QTextCodec::codecForName( "UTF-8" ) );

  // QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );

  // QTextCodec::setCodecForCStrings( QTextCodec::codecForName("System") );

  qWarning()
    << "Codec for C-strings:"
    << ( QTextCodec::codecForCStrings()!=NULL
	 ? QTextCodec::codecForCStrings()->name()
	 : "none" );

  qWarning()
    << "Codec for Locale:"
    << ( QTextCodec::codecForLocale()!=NULL
	 ? QTextCodec::codecForLocale()->name()
	 : "none" );

  qWarning()
    << "Codec for Tr:"
    << ( QTextCodec::codecForTr()!=NULL
	 ? QTextCodec::codecForTr()->name()
	 : "none" );


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
  
  // Go up in the directory hierarchy until we have a candidate install prefix
  bool prefixFound = false;
  QDir prefix( bin_dir );
  while ( prefix.cdUp() )
    {
    if ( QDir(prefix).cd( Monteverdi_INSTALL_BIN_DIR) )
      {
      prefixFound = true;
      break;
      }
    }
  
  if( !prefixFound )
    throw std::runtime_error(
      ToStdString(
	tr( "Failed to locate translation files directory 'i18n' in '%1'." )
	.arg( prefix.path() )
      )
    );

  QDir i18n_dir( prefix );

  // At this point the candidate install prefix can also be the build dir root
  if ( prefix.exists( Monteverdi_BUILD_DIR_FILE  )
       && i18n_dir.cd( "i18n" ) )
    {
    m_IsRunningFromBuildDir = true;

    // Report found build dir root
    qDebug() <<
      tr( "Running from build directory '%1'." )
      .arg( prefix.path() );

    qDebug()
      << tr( "Loading translation files from directory '%1'." )
      .arg( i18n_dir.path() );
    }
  else
    {
    m_IsRunningFromBuildDir = false;
    
    // Report found install prefix
    qDebug()
      << tr( "Running from install directory '%1'." )
      .arg( prefix.path() );

    if ( i18n_dir.cd( Monteverdi_INSTALL_DATA_I18N_DIR ) )
      {
      qDebug()
	<< tr( "Loading translation files from directory '%1'." )
	.arg( i18n_dir.path() );
      }
    else
      {
      throw std::runtime_error(
	ToStdString(
	  tr( "Failed to access translation-files directory '%1'" )
	  .arg( QDir::cleanPath(
		  prefix.path()
		  + QDir::separator()
		  + Monteverdi_INSTALL_DATA_I18N_DIR
		)
	  )
	)
      );
      }
    }

  try
    {
    //
    // 3.1 Stack Qt translator.
    LoadAndInstallTranslator(
      "qt_" + sys_lc.name(),
      QLibraryInfo::location( QLibraryInfo::TranslationsPath  )
    );
    }
  catch( std::exception& exc )
    {
    qWarning() << exc.what();
    }

  try
    {
    //
    // 3.2 Stack Monteverdi translator as prioritary over Qt translator.
    LoadAndInstallTranslator( sys_lc.name(), i18n_dir.path() );
    }
  catch( std::exception& exc )
    {
    qWarning() << exc.what();
    }

  // TODO: Record locale translation filename(s) used in UI component (e.g.
  // AboutDialog, Settings dialog, Information dialog etc.)
}

/*******************************************************************************/
void
I18nCoreApplication
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

#if 0
#ifndef Q_WS_MAC
    setWindowIcon( QIcon( QLatin1String( ":/images/application_icon" ) ) );
#endif
#endif
}

/*******************************************************************************/
void
I18nCoreApplication
::InitializeSettings()
{
  //
  // Create settings proxy.
  m_Settings = new QSettings(
    QSettings::IniFormat,
    QSettings::UserScope,
    QCoreApplication::organizationName(),
    QCoreApplication::applicationName(),
    this
  );

  //
  // Restore result directory.
  QVariant resultDir(
    RetrieveSettingsKey( I18nCoreApplication::SETTINGS_KEY_RESULTS_DIR )
  );

  if( !resultDir.isNull() )
    {
    QString resultPath( resultDir.toString() );

    qDebug() << "Settings/resultsDir:" << resultPath;

    if( I18nCoreApplication::IsResultsDirValid( resultPath  ) )
      {
      m_ResultsDir.setPath( resultPath );
      }
    }
}

/*******************************************************************************/
void
I18nCoreApplication
::LoadAndInstallTranslator( const QString& filename,
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
    delete lc_translator;
    lc_translator = NULL;

    throw std::runtime_error(
      ToStdString(
	tr( "Failed to load '%1' translation file from '%2'." )
	.arg( filename_ext )
	.arg( directory )
      )
    );
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
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
