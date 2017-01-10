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

// Constant : environment variable for application path
static const char OTB_APPLICATION_VAR[] = "OTB_APPLICATION_PATH";

class ApplicationPrivateRegistry
{
public:
  typedef std::pair<Application*, void* > AppHandlePairType;
  typedef std::list<AppHandlePairType>    AppHandleContainerType;

  /** Add a pair (application, library handle) in the private registry */
  bool AddPair(Application *app, void *handle)
    {
    AppHandlePairType pair;
    if (app && handle)
      {
      // mutex lock to ensure thread safety
      itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder(m_Mutex);
      pair.first = app;
      pair.second = handle;
      m_Container.push_back(pair);
      return true;
      }
    return false;
    }

  /** When an application is deleted, unregister its pointer from private registry */
  void UnregisterApp(const Application *app)
    {
    if (app)
      {
      // mutex lock to ensure thread safety
      itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder(m_Mutex);
      AppHandleContainerType::iterator it = m_Container.begin();
      while (it != m_Container.end())
        {
        if ((*it).first == app)
          {
          (*it).first = ITK_NULLPTR;
          }
        ++it;
        }
      }
    }

  /** Release the library handles from applications already deleted */
  void ReleaseUnusedHandle()
    {
    itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder(m_Mutex);
    AppHandleContainerType::iterator it;
    for (it = m_Container.begin() ; it != m_Container.end() ; ++it)
      {
      if ((*it).first == ITK_NULLPTR)
        {
        itk::DynamicLoader::CloseLibrary( static_cast<itk::LibHandle>((*it).second));
        (*it).second = ITK_NULLPTR;
        }
      }
    m_Container.remove(AppHandlePairType((Application*) ITK_NULLPTR, (void*) ITK_NULLPTR));
    }

  /** close all handles at program exit */
  ~ApplicationPrivateRegistry()
  {
  // unregister all ITK factories, because some of them could have been
  // registered by the shared libs we are about to close.
  itk::ObjectFactoryBase::UnRegisterAllFactories();
  // Close all opened shared libs
  AppHandleContainerType::iterator it;
  for (it = m_Container.begin() ; it != m_Container.end() ; ++it)
    {
    itk::DynamicLoader::CloseLibrary( static_cast<itk::LibHandle>((*it).second));
    }
  m_Container.clear();
  }

private:
  AppHandleContainerType m_Container;

  itk::SimpleMutexLock m_Mutex;
};
// static finalizer to close opened libraries
static ApplicationPrivateRegistry m_ApplicationPrivateRegistryGlobal;

// Define callbacks to unregister applications in ApplicationPrivateRegistry
void DeleteAppCallback(itk::Object *obj,const itk::EventObject &, void *)
  {
  Application *appPtr = dynamic_cast<Application*>(obj);
  m_ApplicationPrivateRegistryGlobal.UnregisterApp(appPtr);
  }
void DeleteAppConstCallback(const itk::Object *obj,const itk::EventObject &, void *)
  {
  const Application *appPtr = dynamic_cast<const Application*>(obj);
  m_ApplicationPrivateRegistryGlobal.UnregisterApp(appPtr);
  }

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
  putEnvPath << OTB_APPLICATION_VAR << "=" << newpath;

  // do NOT use putenv() directly, since the string memory must be managed carefully
  itksys::SystemTools::PutEnv(putEnvPath.str().c_str());
}

void
ApplicationRegistry::AddApplicationPath(std::string newpath)
{
  std::ostringstream putEnvPath;
  putEnvPath << OTB_APPLICATION_VAR <<"=";

  // Can be NULL if the env var is not set
  const char* currentEnv = itksys::SystemTools::GetEnv(OTB_APPLICATION_VAR);

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

std::string
ApplicationRegistry::GetApplicationPath()
{
  std::string ret;
  // Can be NULL if the env var is not set
  const char* currentEnv = itksys::SystemTools::GetEnv(OTB_APPLICATION_VAR);
  if (currentEnv)
    {
    ret = std::string(currentEnv);
    }
  return ret;
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
  ApplicationPointer appli = ITK_NULLPTR;

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

  std::string otbAppPath = GetApplicationPath();
  std::vector<itksys::String> pathList;
  if (!otbAppPath.empty())
    {
    pathList = itksys::SystemTools::SplitString(otbAppPath.c_str(),pathSeparator,false);
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

  std::string otbAppPath = GetApplicationPath();
  std::vector<itksys::String> pathList;
  if (!otbAppPath.empty())
    {
    pathList = itksys::SystemTools::SplitString(otbAppPath.c_str(),pathSeparator,false);
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
        appli = ITK_NULLPTR;
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

void
ApplicationRegistry::CleanRegistry()
{
  m_ApplicationPrivateRegistryGlobal.ReleaseUnusedHandle();
}

Application::Pointer
ApplicationRegistry::LoadApplicationFromPath(std::string path,std::string name)
{
  Application::Pointer appli;

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
            // register library handle
            m_ApplicationPrivateRegistryGlobal.AddPair(appli.GetPointer(), (void*) lib);
            // set a callback on DeleteEvent
            itk::CStyleCommand::Pointer command = itk::CStyleCommand::New();
            command->SetCallback(&DeleteAppCallback);
            command->SetConstCallback(&DeleteAppConstCallback);
            appli->AddObserver(itk::DeleteEvent(),command);
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
