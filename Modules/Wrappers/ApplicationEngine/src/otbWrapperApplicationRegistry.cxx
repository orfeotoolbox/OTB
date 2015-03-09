/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplicationRegistry.h"
#include "otbMacro.h"
#include "itksys/SystemTools.hxx"
#include <iterator>

namespace otb
{
namespace Wrapper
{

ApplicationRegistry::ApplicationRegistry()
{
}

ApplicationRegistry::~ApplicationRegistry()
{
}

void
ApplicationRegistry::SetApplicationPath(std::string newpath)
{
  std::ostringstream putEnvPath;
  putEnvPath << "ITK_AUTOLOAD_PATH=" << newpath;

  // do NOT use putenv() directly, since the string memory must be managed carefully
  itksys::SystemTools::PutEnv(putEnvPath.str().c_str());

  // Reload factories to take into account new path
  itk::ObjectFactoryBase::ReHash();
}

void
ApplicationRegistry::AddApplicationPath(std::string newpath)
{
  std::ostringstream putEnvPath;
  putEnvPath << "ITK_AUTOLOAD_PATH=";

  // Can be NULL if the env var is not set
  const char* currentEnv = itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH");

#if defined(WIN32)
  const char pathSeparator = ';';
#else
  const char pathSeparator = ':';
#endif

  putEnvPath << newpath << pathSeparator;

  if (currentEnv)
    {
    putEnvPath << currentEnv;
    }

  // do NOT use putenv() directly, since the string memory must be managed carefully
  itksys::SystemTools::PutEnv(putEnvPath.str().c_str());

  // Reload factories to take into account new path
  itk::ObjectFactoryBase::ReHash();

}

Application::Pointer
ApplicationRegistry::CreateApplication(const std::string& name)
{
  ApplicationPointer appli;

  std::list<ApplicationPointer> possibleApp;
  std::list<LightObject::Pointer> allobjects = itk::ObjectFactoryBase::CreateAllInstance("otbWrapperApplication");

  // Downcast and Sanity check
  for (std::list<LightObject::Pointer>::iterator i = allobjects.begin(); i != allobjects.end(); ++i)
    {
    Application* app = dynamic_cast<Application*> (i->GetPointer());
    if (app)
      {
      possibleApp.push_back(app);
      }
    else
      {
      otbMsgDevMacro( << "Error ApplicationRegistry factory did not return an Application: " << (*i)->GetNameOfClass() << std::endl );
      }
    }

  // Return the app with the desired name
  for(std::list<ApplicationPointer>::iterator k = possibleApp.begin();
      k != possibleApp.end(); ++k)
    {
    try
      {
      (*k)->Init();
      if ( (*k)->GetName() == name )
        {
        appli = *k;
        break;
        }
      }
    catch(...)
      {
      otbMsgDevMacro( << "Error a faulty Application has been detected: "<<(*k)->GetNameOfClass() << std::endl );
      }
    }

  return appli;
}

std::vector<std::string>
ApplicationRegistry::GetAvailableApplications()
{
  ApplicationPointer appli;

  std::list<ApplicationPointer> possibleApp;
  std::list<LightObject::Pointer> allobjects = itk::ObjectFactoryBase::CreateAllInstance("otbWrapperApplication");

  // Downcast and Sanity check
  for (std::list<LightObject::Pointer>::iterator i = allobjects.begin(); i != allobjects.end(); ++i)
    {
    Application* io = dynamic_cast<Application*> (i->GetPointer());
    if (io)
      {
      possibleApp.push_back(io);
      }
    else
      {
      otbMsgDevMacro( "Error ApplicationRegistry factory did not return an Application: " << (*i)->GetNameOfClass() << std::endl );
      }
    }

  // Get all the app names
  // If ITK_AUTOLOAD_PATH contains several times the same path, then the same app appear several times
  // Use a temporary std::set to fix this
  std::set<std::string> appSet;
  for(std::list<ApplicationPointer>::iterator k = possibleApp.begin();
      k != possibleApp.end(); ++k)
    {
    (*k)->Init();
    appSet.insert((*k)->GetName());
    }

  std::vector<std::string> appVec;
  std::copy(appSet.begin(), appSet.end(), std::back_inserter(appVec));
  return appVec;
}


} // end namespace Wrapper
} //end namespace otb
