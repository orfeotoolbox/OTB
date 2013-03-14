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
#include "ApplicationsWrapper/mvdApplicationsBrowser.h"


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
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperApplication.h"

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{
/*
  TRANSLATOR mvd::ApplicationsBrowser

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
ApplicationsBrowser
::ApplicationsBrowser( QObject* parent ) :
  QObject( parent ),
  m_AutoLoadPath("")
{
}

/*******************************************************************************/
ApplicationsBrowser
::~ApplicationsBrowser()
{
}

/*******************************************************************************/
void
ApplicationsBrowser
::SetAutoLoadPath(const std::string & itk_auto_load_path)
{
  m_AutoLoadPath = itk_auto_load_path;

  //
  // add the path
  otb::Wrapper::ApplicationRegistry::AddApplicationPath(m_AutoLoadPath);
}

/*******************************************************************************/
core::StringVector
ApplicationsBrowser
::GetAvailableApplications()
{
  //
  // check if search path is not empty
  // TODO : throw exception to be catched by the catalog manager later
  if (m_AutoLoadPath.empty())
    {
    std::cerr << "ERROR: Search path is empty "<< std::endl;
    }

  //
  // get available application in search path
  core::StringVector appList = otb::Wrapper::ApplicationRegistry::GetAvailableApplications();

  // 
  // some verbosity
  // TODO : remove this verbosity later
  if ( appList.size() == 0 )
    {
    std::cerr << "ERROR: Available modules : none." << std::endl;
    }
  else
    {
    std::cout << "--- Available modules :" << std::endl;
    for (core::StringVector::const_iterator it = appList.begin(); it != appList.end(); ++it)
      {
      std::cout << "\t" << *it << std::endl;
      }
    }
    
  return appList;
}

/*******************************************************************************/
core::StringVector
ApplicationsBrowser
::GetApplicationTags( const std::string& appName )
{
  //
  // output vector
  core::StringVector  vtags;
  
  //
  // instantiate the application using the factory
  otb::Wrapper::Application::Pointer application 
    = otb::Wrapper::ApplicationRegistry::CreateApplication(appName);

  //
  // get tags
  if ( !application.IsNull() )
    {
    vtags = application->GetDocTags();
    }

  return vtags;
}

/*******************************************************************************/
void
ApplicationsBrowser
::SearchAvailableApplicationsTags()
{
  //
  // get all the applications in the search path
  core::StringVector vapp = GetAvailableApplications();

  //
  // Fill the  map as following
  // - key   -> tag
  // - value -> list of applications having this tag
  ApplicationsTagContainer      outputContainer;
  StringVector::const_iterator  it = vapp.begin();

  while ( it != vapp.end() )
    {
    // get tags of current app
    core::StringVector ctags = GetApplicationTags( *it );

    // key will be the tag (easier for tree origanisation in )
    core::StringVector::const_iterator  itTag = ctags.begin();
    while ( itTag != ctags.end() )
      {

      // search for this tag in the output container
      core::ApplicationsTagContainer::iterator  pos = outputContainer.find( *itTag );
      if ( pos != outputContainer.end() )  // key found
        {
        //
        // append the application name (*it)
        (*pos).second.push_back(*it);
        }
      else
        {
        //
        // if tag not found in the container, add it
        std::pair< std::string, core::StringVector > currentAppTags;
        currentAppTags.first  = *itTag;
        currentAppTags.second.push_back( *it );
        outputContainer.insert( currentAppTags );
        }

      ++itTag;
      }

    ++it;
    }

  //
  // emit a signal with the ApplicationsTagContainer as
  // parameter 
  emit AvailableApplicationsTagsChanged(outputContainer);
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
