//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimImageReconstructionFilterFactory.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageReconstructionFilterFactory_HEADER
#define ossimImageReconstructionFilterFactory_HEADER
#include <ossim/imaging/ossimImageSourceFactoryBase.h>

class ossimImageReconstructionFilterFactory : public ossimImageSourceFactoryBase
{
public:
   virtual ~ossimImageReconstructionFilterFactory();
   static ossimImageReconstructionFilterFactory* instance();
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   
protected:
   ossimImageReconstructionFilterFactory(); // hide
   ossimImageReconstructionFilterFactory(const ossimImageReconstructionFilterFactory&){}//hide
   void operator = (ossimImageReconstructionFilterFactory&){}// hide

   static ossimImageReconstructionFilterFactory* theInstance;
   
TYPE_DATA
};
#endif
