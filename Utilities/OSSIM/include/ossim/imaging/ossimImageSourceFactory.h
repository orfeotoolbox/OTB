//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageSourceFactory.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageSourceFactory_HEADER
#define ossimImageSourceFactory_HEADER
#include <ossim/imaging/ossimImageSourceFactoryBase.h>

class ossimImageSourceFactory : public ossimImageSourceFactoryBase
{
public:
   virtual ~ossimImageSourceFactory();
   static ossimImageSourceFactory* instance();
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
protected:
   // Hide from use.
   ossimImageSourceFactory();
   ossimImageSourceFactory(const ossimImageSourceFactory&);
   const ossimImageSourceFactory& operator=(ossimImageSourceFactory&);

   static ossimImageSourceFactory* theInstance;
TYPE_DATA
};
#endif
