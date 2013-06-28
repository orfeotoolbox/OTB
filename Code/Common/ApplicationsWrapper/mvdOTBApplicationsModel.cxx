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
#include "ApplicationsWrapper/mvdOTBApplicationsModel.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "ApplicationsWrapper/mvdApplicationsBrowser.h"
#include "ApplicationsWrapper/mvdApplicationLauncher.h"

namespace mvd
{
/*
  TRANSLATOR mvd::OTBApplicationsModel

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
OTBApplicationsModel
::OTBApplicationsModel( QObject* parent ) :
  AbstractModel( parent ),
  m_ApplicationsBrowser( NULL ),
  m_ApplicationLauncher( NULL )
{
  //
  // instanciate apps browser
  m_ApplicationsBrowser = new ApplicationsBrowser( this );  

  //
  // instanciate app launcher
  m_ApplicationLauncher = new ApplicationLauncher( this );

  // initialize the app browser with empty string. The default
  // behavior use the env ITK_AUTOLOAD_PATH
  //m_ApplicationsBrowser->SetAutoLoadPath("");
}

/*******************************************************************************/
OTBApplicationsModel
::~OTBApplicationsModel()
{
}

/*******************************************************************************/
void
OTBApplicationsModel
::virtual_BuildModel( void* context )
{
  // nothing to do
}

/*******************************************************************************/
void
OTBApplicationsModel
::FillTree( )
{
  //
  // this method is connected to the widget via a signal. In
  // ApplicationBrowser, when the map is filled with tags/Apps, a
  // signal is emitted and received by the widget to fill its tree.
  m_ApplicationsBrowser->SearchAvailableApplicationsTags();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
