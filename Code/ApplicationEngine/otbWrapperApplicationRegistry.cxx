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
#include "itksys/SystemTools.hxx"

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

  const char* currentEnv = itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH");
  if (currentEnv)
    {
    putEnvPath << currentEnv << ":";
    }
  putEnvPath << newpath;

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
    Application* io = dynamic_cast<Application*> (i->GetPointer());
    if (io)
      {
      possibleApp.push_back(io);
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
    if ( (*k)->GetName() == name )
      {
      return *k;
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

  // Return the app list
  std::vector<std::string> availableApp;
  for(std::list<ApplicationPointer>::iterator k = possibleApp.begin();
      k != possibleApp.end(); ++k)
    {
    availableApp.push_back((*k)->GetName());
    }
  // If the ITK_AUTOLOAD_PATH contains
  // several times the same path, application will appears several
  // times in the list.To be cleaner, we erase multiple
  std::vector<std::string>::iterator it;
  it = std::unique(availableApp.begin(), availableApp.end());
  availableApp.resize( it - availableApp.begin() );

  return availableApp;
}


} // end namespace Wrapper
} //end namespace otb
