//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Class declaration of ossimFactoryListInterface.
//
//**************************************************************************************************
// $Id$
#ifndef ossimFactoryListInterface_HEADER
#define ossimFactoryListInterface_HEADER
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <vector>
#include <ossim/base/ossimRefPtr.h>

/**
 * The is a factory list interface that allows registries to be accessed in a common way.  
 */
template <class T, class NativeType>
class ossimFactoryListInterface
   {
   public:
      typedef std::vector<T*> FactoryListType;
      typedef T FactoryType;
      typedef NativeType NativeReturnType;
      
      ossimFactoryListInterface(){}
      
      /**
       * This is for backward compatability and calls registerFactory for simple adds.
       */
      void addFactory(T* factory)
      {
         registerFactory(factory);
      }
      
      /**
       * Public access method to determine if a factory is already registered to this
       * list
       */
      bool isFactoryRegistered(T* factory)const
      {
         if(!factory) return false;
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_factoryListMutex);
         
         return findFactory(factory);
      }
      
      /**
       * Will register a factory to the factory list.  Will append the passed in factory if not
       * already registered to the list.
       */
      void registerFactory(T* factory, bool pushToFrontFlag=false)
      {
         if(!factory) return;
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_factoryListMutex);
         if(!findFactory(factory))
         {
            if (pushToFrontFlag)
            {
               m_factoryList.insert(m_factoryList.begin(), factory);
            }
            else
            {
               m_factoryList.push_back(factory);
            }
         }
      }
      /**
       * Will remove the factory from the registry.
       */
      void unregisterFactory(T* factory)
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_factoryListMutex);
         ossim_uint32 idx = 0;
         for(idx = 0; idx < m_factoryList.size(); ++idx)
         {
            if(factory == m_factoryList[idx])
            {
               m_factoryList.erase(m_factoryList.begin() + idx);
               return;
            }
         }
      }
      
      /**
       * Will remove all factories from the registry.
       */
      void unregisterAllFactories()
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_factoryListMutex);
         m_factoryList.clear();
      }
      
      /**
       * Inserts the factory to the front of the list.
       */
      void registerFactoryToFront(T* factory)
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_factoryListMutex);
         if(!findFactory(factory))
         {
            m_factoryList.insert(m_factoryList.begin(), factory);
         }
      }
      
      /**
       * Will insert the factory before the beforeThisFactory.  If not found
       * it will do a simple append.
       */
      void registerFactoryBefore(T* factory, T* beforeThisFactory)
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_factoryListMutex);
         if(!findFactory(factory))
         {
            ossim_uint32 idx = 0;
            for(idx = 0; idx < m_factoryList.size(); ++idx)
            {
               if(beforeThisFactory == m_factoryList[idx])
               {
                  m_factoryList.insert(m_factoryList.begin() + idx, factory);
                  return;
               }
            }
            m_factoryList.push_back(factory);
         }
      }
      
      /**
       *
       * Will add all object types the factories can allocate.  Typically a list of classnames are returned
       *
       */
      void getAllTypeNamesFromRegistry(std::vector<ossimString>& typeList)const;
      
      /**
       * This is the base object return for all objects in the system.  This is used for 
       * backward compatability.
       */
      ossimObject* createObjectFromRegistry(const ossimString& typeName)const;
      
      /**
       * This is the base object return for all objects in the system.  This is used for 
       * backward compatability.
       */
      ossimObject* createObjectFromRegistry(const ossimKeywordlist& kwl,
                                            const char* prefix=0)const;

      /**
       * This is a helper method that calls the createObject and makes sure that the
       * returned object is of the NativeType base type this registry supports.
       */
      NativeType* createNativeObjectFromRegistry(const ossimString& typeName)const;
      
      /**
       * This is a helper method that calls the createObject and makes sure that the
       * returned object is of the NativeType base type this registry supports.
       *
       * @param kwl is a state keywordlist allowing one to instantiate an object 
       *        defined by a set of name value pairs.  It will use the type 
       *        keyword to create an object of the defined type and then load the 
       *        state.
       */
      NativeType* createNativeObjectFromRegistry(const ossimKeywordlist& kwl,
                                                 const char* prefix=0)const;
   protected:
      /**
       * Utility to find a factory in the list
       */
      bool findFactory(T* factory)const
      {
         if(!factory) return false;
         ossim_uint32 idx = 0;
         for(;idx < m_factoryList.size();++idx)
         {
            if(m_factoryList[idx] == factory)
            {
               return true;
            }
         }
         
         return false;
      }
      mutable OpenThreads::Mutex m_factoryListMutex;
      FactoryListType m_factoryList;
   };

template <class T, class NativeType>
void ossimFactoryListInterface<T, NativeType>::getAllTypeNamesFromRegistry(std::vector<ossimString>& typeList)const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_factoryListMutex);
   ossim_uint32 idx = 0;
   for(; idx<m_factoryList.size(); ++idx)
   {
      m_factoryList[idx]->getTypeNameList(typeList);
   }
}
template <class T, class NativeType>
ossimObject* ossimFactoryListInterface<T, NativeType>::createObjectFromRegistry(const ossimString& typeName)const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_factoryListMutex);
   ossimObject* result = 0;
   ossim_uint32 idx = 0;
   for(;((idx<m_factoryList.size())&&!result); ++idx)
   {
      result = m_factoryList[idx]->createObject(typeName);
   }
   return result;
}

template <class T, class NativeType>
ossimObject* ossimFactoryListInterface<T, NativeType>::createObjectFromRegistry(const ossimKeywordlist& kwl,
                                                                                const char* prefix)const
{
   // OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_factoryListMutex);
   ossimObject* result = 0;
   ossim_uint32 idx = 0;
   for(;((idx<m_factoryList.size())&&!result); ++idx)
   {
      result = m_factoryList[idx]->createObject(kwl, prefix);
   }
   return result;
}

template <class T, class NativeType>
NativeType* ossimFactoryListInterface<T, NativeType>::createNativeObjectFromRegistry(const ossimString& typeName)const
{
   NativeType* result = 0;
   ossimRefPtr<ossimObject> tempObject = createObjectFromRegistry(typeName);
   if(tempObject.valid())
   {
      result = dynamic_cast<NativeType*>(tempObject.get());
      if(result)
      {
         tempObject.release();
      }
   }
   
   return result;
}

template <class T, class NativeType>
NativeType* ossimFactoryListInterface<T, NativeType>::createNativeObjectFromRegistry(const ossimKeywordlist& kwl,
                                                                                     const char* prefix)const
{
   NativeType* result = 0;
   ossimRefPtr<ossimObject> tempObject = createObjectFromRegistry(kwl, prefix);
   if(tempObject.valid())
   {
      result = dynamic_cast<NativeType*>(tempObject.get());
      if(result)
      {
         tempObject.release();
      }
   }
   
   return result;
}

#endif
