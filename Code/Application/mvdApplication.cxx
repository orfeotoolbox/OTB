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
#include "mvdAbstractModel.h"
#include "mvdDatasetModel.h"

// Application icon on Linux platform
#if ! defined(Q_WS_WIN) && ! defined(Q_WS_MAC)
#  include "monteverdi2-48x48.xpm"
#endif

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
DatasetModel*
Application::LoadDatasetModel( const QString& imageFilename,
			       int width,
			       int height )
{
  DatasetModel* model = new DatasetModel();

  model->setObjectName( "mvd::DatasetModel('" + imageFilename + "'" );

  QString path;
  QString name;

  Application::DatasetPathName( path, name, imageFilename );
  qDebug() << "Dataset path: " << path;
  qDebug() << "Dataset name: " << name;

  try
    {
    if( !model->SetContent( path, name ) || true )
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

  QObject::connect(
    this, SIGNAL( aboutToQuit() ),
    this, SLOT( OnAboutToQuit() )
    );

  InitializeModel();
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
    tr( "Centre National d'Etudes Spatiales (CNES)" )
  );
  QCoreApplication::setOrganizationDomain(
    "orfeo-toolbox.org"
  );

#if ! defined(Q_WS_WIN) && ! defined(Q_WS_MAC)
  setWindowIcon( QPixmap( monteverdi2_xpm ) );
#endif

}

/*******************************************************************************/
void
Application
::InitializeModel()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
Application
::OnAboutToQuit()
{
  qDebug() << tr( ">DEBUG> mvd::Application::OnAboutToQuit()." );
}

} // end namespace 'mvd'
