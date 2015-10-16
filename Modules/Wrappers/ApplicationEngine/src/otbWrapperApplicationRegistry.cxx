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
#include "itkDirectory.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

#include <iterator>

namespace otb
{
namespace Wrapper
{

class ApplicationPrivateRegistry
{
public:
  typedef std::pair<Application*, void* > AppHandlePairType;
  typedef std::list<AppHandlePairType>    AppHandleContainerType;

  ~ApplicationPrivateRegistry()
  {
  AppHandleContainerType::iterator it;
  for (it = m_Container.begin() ; it != m_Container.end() ; ++it)
    {
    itk::DynamicLoader::CloseLibrary( (*it).second);
    }
  m_Container.clear();
  }

  AppHandleContainerType m_Container;
};
// static finalizer to close opened libraries
static ApplicationPrivateRegistry m_ApplicationPrivateRegistryGlobal;


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
ApplicationRegistry::CreateApplication(const std::string& name, bool useFactory)
{
  ApplicationPointer appli;

  // Fast search : uses OTB_APPLICATION_PATH
  appli = ApplicationRegistry::CreateApplicationFaster(name);
  if (appli.IsNotNull())
    {
    return appli;
    }

  // Classic search : uses factories registered by ITK ( see ITK_AUTOLOAD_PATH )
  if (useFactory)
    {
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
    }
  
  return appli;
}

typedef itk::ObjectFactoryBase * ( *ITK_LOAD_FUNCTION )();

Application::Pointer
ApplicationRegistry::CreateApplicationFaster(const std::string& name)
{
  ApplicationPointer appli = NULL;

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
  std::vector<itksys::String> pathList;
  if (otbAppPath)
    {
    pathList = itksys::SystemTools::SplitString(otbAppPath,pathSeparator,false);
    }
  for (unsigned int i=0 ; i<pathList.size() ; ++i)
    {
    std::string possiblePath = pathList[i];
    if ( !possiblePath.empty() && possiblePath[possiblePath.size() - 1] != sep )
      {
      possiblePath += sep;
      }
    possiblePath += appLibName.str();

    appli = LoadApplicationFromPath(possiblePath,name);
    if (appli.IsNotNull())
      {
      break;
      }
    }

  return appli;
}

std::vector<std::string>
ApplicationRegistry::GetAvailableApplications(bool useFactory)
{
  ApplicationPointer appli;
  std::set<std::string> appSet;

  std::string appPrefix("otbapp_");
  std::string appExtension = itksys::DynamicLoader::LibExtension();
#ifdef __APPLE__
  appExtension = ".dylib";
#endif

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
  std::vector<itksys::String> pathList;
  if (otbAppPath)
    {
    pathList = itksys::SystemTools::SplitString(otbAppPath,pathSeparator,false);
    }
  for (unsigned int k=0 ; k<pathList.size() ; ++k)
    {
    itk::Directory::Pointer dir = itk::Directory::New();
    if (!dir->Load(pathList[k].c_str()))
      {
      continue;
      }
    for (unsigned int i = 0; i < dir->GetNumberOfFiles(); i++)
      {
      const char *filename = dir->GetFile(i);
      std::string sfilename(filename);
      std::string::size_type extPos = sfilename.rfind(appExtension);
      std::string::size_type prefixPos = sfilename.find(appPrefix);

      // Check if current file is a shared lib with the right pattern
      if (extPos + appExtension.size() == sfilename.size() &&
          prefixPos == 0)
        {
        std::string name = sfilename.substr(appPrefix.size(),extPos-appPrefix.size());
        std::string fullpath = pathList[k];
        if (!fullpath.empty() && fullpath[fullpath.size() - 1] != sep)
          {
          fullpath.push_back(sep);
          }
        fullpath.append(sfilename);
        appli = LoadApplicationFromPath(fullpath,name);
        if (appli.IsNotNull())
          {
          appSet.insert(name);
          }
        appli = NULL;
        }
      }
    }

  if (useFactory)
    {
    std::list<LightObject::Pointer> allobjects = itk::ObjectFactoryBase::CreateAllInstance("otbWrapperApplication");
    // Downcast and Sanity check
    for (std::list<LightObject::Pointer>::iterator i = allobjects.begin(); i != allobjects.end(); ++i)
      {
      Application* app = dynamic_cast<Application*> (i->GetPointer());
      if (app)
        {
        app->Init();
        std::string curName(app->GetName());
        appSet.insert(curName);
        }
      else
        {
        otbMsgDevMacro( << "Error ApplicationRegistry factory did not return an Application: " << (*i)->GetNameOfClass() << std::endl );
        }
      }
    }

  std::vector<std::string> appVec;
  std::copy(appSet.begin(), appSet.end(), std::back_inserter(appVec));
  return appVec;
}


Application::Pointer
ApplicationRegistry::LoadApplicationFromPath(std::string path,std::string name)
{
  Application::Pointer appli;

  static itk::SimpleMutexLock mutex;

  if (itksys::SystemTools::FileExists(path.c_str(),true))
    {
    itk::LibHandle lib = itk::DynamicLoader::OpenLibrary(path.c_str());
    if (lib)
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
        // Downcast
        ApplicationFactoryBase* appFactory = dynamic_cast<ApplicationFactoryBase*>(newfactory);

        if (appFactory)
          {
          appli = appFactory->CreateApplication(name.c_str());
          if (appli.IsNotNull())
            {
            appli->Init();
            // mutex lock to ensure thread safety
            itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder(mutex);

            ApplicationPrivateRegistry::AppHandlePairType curPair;
            curPair.first = appli.GetPointer();
            curPair.second = (void*) lib;
            m_ApplicationPrivateRegistryGlobal.m_Container.push_back(curPair);
            return appli;
            }
          }
        }
      itk::DynamicLoader::CloseLibrary(lib);
      }
    }
  return appli;
}


} // end namespace Wrapper
} //end namespace otb
