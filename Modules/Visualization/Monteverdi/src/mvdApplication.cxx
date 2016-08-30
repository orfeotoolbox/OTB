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
#include "mvdStackedLayerModel.h"

//
#ifdef OTB_USE_QT4
#  include "mvdOTBApplicationsModel.h"
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

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
Application
::Application( QApplication* qtApp ) :
  I18nApplication( qtApp ),
  m_OTBApplicationsModel( NULL )
{
}

/*******************************************************************************/
Application
::~Application()
{
}

/*******************************************************************************/
/*
CountType
Application
::OpenDatabase()
{
  DatabaseModel* databaseModel = new DatabaseModel( this );

  DatabaseModel::BuildContext context;
  databaseModel->BuildModel( &context );

  SetModel( databaseModel );

  return context.m_NbOutdatedDatasetModels;
}
*/

/*******************************************************************************/
/*
void
Application
::CloseDatabase()
{
  // Delete model and release resources.
  SetModel( NULL );
}
*/

/*******************************************************************************/
void
Application
::OpenApplicationsBrowser()
{
#ifdef OTB_USE_QT4
  m_OTBApplicationsModel = new OTBApplicationsModel( this );

  m_OTBApplicationsModel->BuildModel();
#endif
}

/*******************************************************************************/
void
Application
::virtual_InitializeCore()
{
  I18nApplication::virtual_InitializeCore();

  setObjectName( "Application" );

  InitializeCore(
    PROJECT_NAME, Monteverdi_VERSION_STRING,
    "OrfeoToolBox", "orfeo-toolbox.org"
  );

  //
  // Create the OTBApplications model
  OpenApplicationsBrowser();
}

/*******************************************************************************/
/* SLOTS                                                                       */

} // end namespace 'mvd'
