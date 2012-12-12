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

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "mvdDatasetModel.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "ConfigureMonteverdi2.h"

//
// Class implementation.
namespace mvd
{
/*
  TRANSLATOR mvd::Application

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
Application
::Application( int& argc, char** argv ) :
  I18nApplication( argc, argv ),
  m_Model( NULL )
{
  IMPLEMENT_APP_SINGLETON( mvdApp );

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
::InitializeCore()
{
  //
  // Setup application tags.
  QCoreApplication::setApplicationName(
    PROJECT_NAME
  );
  QCoreApplication::setApplicationVersion(
    Monteverdi2_VERSION_STRING
  );

  //
  // Setup organization tags.
  QCoreApplication::setOrganizationName(
    tr( "Centre National d'Etudes Spatiales (CNES)" )
  );
  QCoreApplication::setOrganizationDomain(
    "orfeo-toolbox.org"
  );
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
