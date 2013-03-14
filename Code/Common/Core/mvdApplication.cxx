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
#include "mvdApplication.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractModel.h"
#include "Core/mvdDatasetModel.h"
#include "Core/mvdVectorImageModel.h"

//
// Class implementation.
namespace mvd
{

/*
  TRANSLATOR mvd::Application

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */

const char* Application::DATASET_EXT = ".ds";

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*****************************************************************************/
void
Application
::DatasetPathName( QString& path,
		   QString& name,
		   const QString& imageFilename )
{
  // '/tmp/archive.tar.gz'
  QFileInfo fileInfo( imageFilename );

#if 0
  // Dataset is stored into image-file path.
  // E.g. '/tmp'
  path = fileInfo.path();
#else
  // Dataset is stored into application cache-directory.
  // E.g. '$HOME/<CACHE_DIR>'
  path = Application::Instance()->GetCacheDir().path();
#endif

  // '[_tmp_]archive.tar.gz.<SUFFIX>'
  name =
#if 1
    fileInfo.canonicalPath().replace( QRegExp( "[/\\\\:]+" ), "_") +
    "_" +
#endif
    fileInfo.fileName() +
    Application::DATASET_EXT;
}

/*****************************************************************************/
DatasetModel*
Application
::LoadDatasetModel( const QString& imageFilename,
		    int width,
		    int height )
{
  // New model.
  DatasetModel* model = new DatasetModel();

  // Retrive path and name.
  QString path;
  QString name;

  Application::DatasetPathName( path, name, imageFilename );
  qDebug() << "Dataset path: " << path;
  qDebug() << "Dataset name: " << name;

  // Setup QObject
  model->setObjectName( QDir( path ).filePath( name ) );

  try
    {
    // try if the filename is valid
    VectorImageModel::EnsureValidImage(imageFilename);
    // Build model (relink to cached data).
    DatasetModel::BuildContext context( path, name, width, height );
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
Application
::Application( int& argc, char** argv ) :
  I18nApplication( argc, argv ),
  m_Model( NULL )
{
  InitializeCore();
}

/*******************************************************************************/
Application
::~Application()
{
}

/*******************************************************************************/
void
Application
::SetModel( AbstractModel* model )
{
  emit AboutToChangeSelectedModel( model );

  delete m_Model;

  m_Model = model;

  if( model!=NULL )
    m_Model->setParent( this );

  emit SelectedModelChanged( m_Model );
}

/*******************************************************************************/
void
Application
::InitializeCore()
{
  setObjectName( "Application" );

  //
  // Setup application tags.
  //
  QCoreApplication::setApplicationName(
    PROJECT_NAME
  );
  QCoreApplication::setApplicationVersion(
    Monteverdi2_VERSION_STRING
  );

  //
  // Setup organization tags.
  //
  QCoreApplication::setOrganizationName(
    "OrfeoToolBox"
  );
  QCoreApplication::setOrganizationDomain(
    "orfeo-toolbox.org"
  );

#ifndef Q_WS_MAC
    setWindowIcon(QIcon(QLatin1String(":/images/application_icon")));
#endif
}

/*******************************************************************************/
bool
Application
::HasSettingsFile()
{
  // The settings file should contain the cacheDir key to be valid
  QSettings settings;
  settings.sync();
  return ( (settings.status()==QSettings::NoError) && settings.contains("cacheDir"));
}

/*******************************************************************************/
void
Application
::ReadCacheDirFromSettings()
{
  QSettings settings;
  QString cacheDirSetting = settings.value("cacheDir").toString();
  m_CacheDir.setPath(cacheDirSetting);
}

/*******************************************************************************/
void
Application
::WriteCacheDirIntoSettings()
{
  QSettings settings;
  settings.setValue("cacheDir", m_CacheDir.path());
}


/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
