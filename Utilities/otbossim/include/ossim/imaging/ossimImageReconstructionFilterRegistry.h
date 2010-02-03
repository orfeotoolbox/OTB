//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageReconstructionFilterRegistry.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimImageReconstructionFilterRegistry_HEADER
#define ossimImageReconstructionFilterRegistry_HEADER
#include <ossim/imaging/ossimImageSourceFactoryBase.h>

class ossimImageReconstructionFilterRegistry : public ossimImageSourceFactoryBase
{
public:
   static ossimImageReconstructionFilterRegistry* instance();
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
   void registerFactory(ossimImageSourceFactoryBase* factory);
   void unregisterFactory(ossimImageSourceFactoryBase* factory);
   bool findFactory(ossimImageSourceFactoryBase* factory)const;
   
protected:
   virtual ~ossimImageReconstructionFilterRegistry();
   ossimImageReconstructionFilterRegistry(); // hide
   ossimImageReconstructionFilterRegistry(const ossimImageReconstructionFilterRegistry&){}//hide
   void operator = (ossimImageReconstructionFilterRegistry&){}// hide
   
   static ossimImageReconstructionFilterRegistry* theInstance;
   std::vector<ossimImageSourceFactoryBase*> theFactoryList;
TYPE_DATA
};

#endif
