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
#include "otbWrapperApplicationFactoryBase.h"
#include "otbMacro.h"
#include "itksys/SystemTools.hxx"
#include "itkDynamicLoader.h"
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
  putEnvPath << "OTB_APPLICATION_PATH=" << newpath;

  // do NOT use putenv() directly, since the string memory must be managed carefully
  itksys::SystemTools::PutEnv(putEnvPath.str().c_str());
}

void
ApplicationRegistry::AddApplicationPath(std::string newpath)
{
  std::ostringstream putEnvPath;
  putEnvPath << "OTB_APPLICATION_PATH=";

  // Can be NULL if the env var is not set
  const char* currentEnv = itksys::SystemTools::GetEnv("OTB_APPLICATION_PATH");

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
}

Application::Pointer
ApplicationRegistry::CreateApplication(const std::string& name)
{
  ApplicationPointer appli;

  // Fast search
  appli = ApplicationRegistry::CreateApplicationFaster(name);
  if (appli.IsNotNull())
    {
    return appli;
    }

  // Classic search
  ApplicationRegistry::RefreshApplicationFactories();

  LightObject::Pointer possibleApp = itk::ObjectFactoryBase::CreateInstance(name.c_str());
  
  if (possibleApp.IsNotNull())
    {
    // Downcast
    Application* app = dynamic_cast<Application*> (possibleApp.GetPointer());
    if (app)
      {
        appli = app;
        appli->Init();
      }
    else
      {
      otbMsgDevMacro( << "Error ApplicationRegistry factory did not return an Application: " << possibleApp->GetNameOfClass() << std::endl );
      }
    }
  
  return appli;
}

typedef itk::ObjectFactoryBase * ( *ITK_LOAD_FUNCTION )();

Application::Pointer
ApplicationRegistry::CreateApplicationFaster(const std::string& name)
{
  ApplicationPointer appli;

  std::string appExtension = itksys::DynamicLoader::LibExtension();
#ifdef __APPLE__
  appExtension = ".dylib";
#endif
  std::ostringstream appLibName;
  appLibName << "otbapp_" << name << appExtension;

#if defined(WIN32)
  const char pathSeparator = ';';
#else
  const char pathSeparator = ':';
#endif

#ifdef _WIN32
  const char sep = '\\';
#else
  const char sep = '/';
#endif

  const char* otbAppPath = itksys::SystemTools::GetEnv("OTB_APPLICATION_PATH");
  const char* itkLoadPath = itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH");

  std::ostringstream currentPath;
  if (otbAppPath)
    {
    currentPath << otbAppPath << pathSeparator;
    }
  if (itkLoadPath)
    {
    currentPath << itkLoadPath;
    }

  std::vector<itksys::String> pathList = itksys::SystemTools::SplitString(currentPath.str().c_str(),pathSeparator,false);
  for (unsigned int i=0 ; i<pathList.size() ; ++i)
    {
    std::string possiblePath = pathList[i];
    if ( !possiblePath.empty() && possiblePath[possiblePath.size() - 1] != sep )
      {
      possiblePath += sep;
      }
    possiblePath += appLibName.str();

    if (itksys::SystemTools::FileExists(possiblePath.c_str(),true))
      {
      itk::LibHandle   lib = itk::DynamicLoader::OpenLibrary( possiblePath.c_str() );
      if ( lib )
        {
        /**
         * Look for the symbol itkLoad in the library
         */
        ITK_LOAD_FUNCTION loadfunction =
          ( ITK_LOAD_FUNCTION ) itk::DynamicLoader::GetSymbolAddress(lib, "itkLoad");
        /**
         * if the symbol is found call it to create the factory
         * from the library
         */
        if ( loadfunction )
          {
          itk::ObjectFactoryBase *newfactory = ( *loadfunction )( );

          if (dynamic_cast<ApplicationFactoryBase*>(newfactory))
            {
            ApplicationFactoryBase* appFactory = dynamic_cast<ApplicationFactoryBase*>(newfactory);
            appli = appFactory->CreateApplication(name.c_str());
            appli->Init();
            break;
            }
          }
        else
          {
          // In the past, some platforms crashed on the call
          // DynamicLoader::CloseLibrary(lib) if the lib has symbols
          // that the current executable is using.
          itk::DynamicLoader::CloseLibrary(lib);
          }
        }
      }
    }

  return appli;
}

std::vector<std::string>
ApplicationRegistry::GetAvailableApplications()
{
  ApplicationPointer appli;

  ApplicationRegistry::RefreshApplicationFactories();

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

void
ApplicationRegistry::RefreshApplicationFactories()
{
  std::ostringstream putEnvPath;
  putEnvPath << "ITK_AUTOLOAD_PATH=";

  // Can be NULL if the env var is not set
  const char* currentEnv = itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH");

  // OTB specific application path
  const char* otbApplicationPath = itksys::SystemTools::GetEnv("OTB_APPLICATION_PATH");

#if defined(WIN32)
  const char pathSeparator = ';';
#else
  const char pathSeparator = ':';
#endif

  if (otbApplicationPath)
    {
    putEnvPath << otbApplicationPath << pathSeparator;
    }

  if (currentEnv)
    {
    putEnvPath << currentEnv;
    }

  // do NOT use putenv() directly, since the string memory must be managed carefully
  itksys::SystemTools::PutEnv(putEnvPath.str().c_str());

  // Reload factories to take into account new path
  itk::ObjectFactoryBase::ReHash();

  std::ostringstream resetEnvPath;
  resetEnvPath << "ITK_AUTOLOAD_PATH=";
  if (currentEnv)
    {
    resetEnvPath << currentEnv;
    }
  itksys::SystemTools::PutEnv(resetEnvPath.str().c_str());
}


} // end namespace Wrapper
} //end namespace otb
