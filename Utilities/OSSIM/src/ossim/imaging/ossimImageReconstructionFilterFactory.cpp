//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimImageReconstructionFilterFactory.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/imaging/ossimImageReconstructionFilterFactory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimMeanMedianFilter.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimImageReconstructionFilterFactory, "ossimImageReconstructionFilterFactory",  ossimImageSourceFactoryBase);

ossimImageReconstructionFilterFactory* ossimImageReconstructionFilterFactory::theInstance = 0;

ossimImageReconstructionFilterFactory::~ossimImageReconstructionFilterFactory()
{
   theInstance = NULL;
}

ossimImageReconstructionFilterFactory* ossimImageReconstructionFilterFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageReconstructionFilterFactory;
   }

   return theInstance;
}

ossimImageReconstructionFilterFactory::ossimImageReconstructionFilterFactory()
{
   theInstance = this;
}

ossimObject* ossimImageReconstructionFilterFactory::createObject(const ossimString& name)const
{
   if(STATIC_TYPE_NAME(ossimMeanMedianFilter) == name)
   {
      return new ossimMeanMedianFilter;
   }

   return 0;
}

ossimObject* ossimImageReconstructionFilterFactory::createObject(const ossimKeywordlist& kwl,
                                                                 const char* prefix)const
{
   const char* typeKw = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(typeKw)
   {
      ossimObject* obj = createObject(ossimString(typeKw).trim());

      if(obj)
      {
         obj->loadState(kwl,
                        prefix);
         
         return obj;
      }
   }
   
   return 0;
}

void ossimImageReconstructionFilterFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimMeanMedianFilter));
}
