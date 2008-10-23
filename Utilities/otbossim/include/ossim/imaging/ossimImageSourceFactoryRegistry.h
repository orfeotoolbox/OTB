//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageSourceFactoryRegistry.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageSourceFactoryRegistry_HEADER
#define ossimImageSourceFactoryRegistry_HEADER
#include <ossim/imaging/ossimImageSourceFactoryBase.h>

class OSSIM_DLL ossimImageSourceFactoryRegistry : public ossimImageSourceFactoryBase
{
public:
   virtual ~ossimImageSourceFactoryRegistry();
   static ossimImageSourceFactoryRegistry* instance();
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
   void registerFactory(ossimImageSourceFactoryBase* factory);
   void unregisterFactory(ossimImageSourceFactoryBase* factory);
   bool findFactory(ossimImageSourceFactoryBase* factory)const;
   
protected:
   ossimImageSourceFactoryRegistry(); // hide
   ossimImageSourceFactoryRegistry(const ossimImageSourceFactoryRegistry&);//hide
   void operator = (ossimImageSourceFactoryRegistry&);// hide
   
   static ossimImageSourceFactoryRegistry* theInstance;
   std::vector<ossimImageSourceFactoryBase*> theFactoryList;
TYPE_DATA
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimImageSourceFactoryRegistryGetInstance();
}
#endif
