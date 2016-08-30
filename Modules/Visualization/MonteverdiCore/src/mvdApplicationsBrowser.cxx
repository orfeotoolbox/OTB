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

#include "mvdApplicationsBrowser.h"

#include "mvdAlgorithm.h"

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
::ApplicationsBrowser( QObject* p ) :
  QObject( p ),
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
StringVector
ApplicationsBrowser
::GetAvailableApplications()
{
  //
  // get available application in search path
  StringVector appList(
    otb::Wrapper::ApplicationRegistry::GetAvailableApplications()
  );

  // 
  // some verbosity
  // TODO : remove this verbosity later
  if ( appList.size() == 0 )
    {
    qWarning() << "Available OTB applications: NONE!";
    }
  else
    {
    qDebug() << "Available OTB applications:";

    for( StringVector::const_iterator it( appList.begin() );
	 it!=appList.end();
	 ++it )
      {
      qDebug() << "- " << it->c_str();
      }
    }

  return appList;
}

/*******************************************************************************/
StringVector
ApplicationsBrowser
::GetApplicationTags( const std::string& appName )
{
  //
  // instantiate the application using the factory
  otb::Wrapper::Application::Pointer application(
    otb::Wrapper::ApplicationRegistry::CreateApplication( appName )
  );

  if( application.IsNull() )
    return StringVector();

  //
  // get tags
  StringVector vtags( application->GetDocTags() );

  //
  // fill the OTBApps 'docName <-> name' map
  m_DocNameToNameMap[ application->GetDocName() ] = appName;

  return vtags;
}

/*******************************************************************************/
void
ApplicationsBrowser
::SearchAvailableApplicationsTags()
{
  //
  // clear previously filled map
  m_DocNameToNameMap.clear();

  //
  // get all the applications in the search path
  StringVector vapp( GetAvailableApplications() );

  //
  // Fill the  map as following
  // - key   -> tag
  // - value -> list of applications having this tag
  ApplicationsTagContainer      outputContainer;
  StringVector::const_iterator  it = vapp.begin();

  while ( it != vapp.end() )
    {
    // get tags of current app
    StringVector ctags = GetApplicationTags( *it );

    // case applications has no tag associated 
    if (ctags.size() == 0 )
      {
      // add a default tag 'unknown'. The processing after remains
      // unchanged
      ctags.push_back( ToStdString(tr("Uncategorized")) );
      }

    // key will be the tag (easier for tree origanisation in )
    StringVector::const_iterator  itTag = ctags.begin();
    
    while ( itTag != ctags.end() )
      {

      // search for this tag in the output container
      ApplicationsTagContainer::iterator  pos = outputContainer.find( *itTag );
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
        std::pair< std::string, StringVector > currentAppTags;
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
  emit AvailableApplicationsTagsChanged(outputContainer, m_DocNameToNameMap);
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
