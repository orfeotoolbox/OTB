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
#include "Core/mvdDatabaseModel.h"

//
#ifdef OTB_WRAP_QT
#include "ApplicationsWrapper/mvdOTBApplicationsModel.h"
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
  I18nApplication( qtApp )
{
}

/*******************************************************************************/
Application
::~Application()
{
}

/*******************************************************************************/
void
Application
::virtual_InitializeCore()
{
  I18nApplication::virtual_InitializeCore();

  setObjectName( "Application" );

  InitializeCore(
    PROJECT_NAME, Monteverdi2_VERSION_STRING,
    "OrfeoToolBox", "orfeo-toolbox.org"
  );

  SetModel( new OTBApplicationsModel( this ) );
}

/*******************************************************************************/
/* SLOTS                                                                       */

} // end namespace 'mvd'
