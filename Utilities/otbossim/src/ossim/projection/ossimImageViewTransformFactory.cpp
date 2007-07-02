//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimImageViewTransformFactory.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/projection/ossimImageViewTransformFactory.h>
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <ossim/projection/ossimImageViewAffineTransform.h>
#include <ossim/base/ossimKeywordlist.h>

RTTI_DEF1(ossimImageViewTransformFactory, "ossimImageViewTransformFactory", ossimObject)

ossimImageViewTransformFactory* ossimImageViewTransformFactory::theInstance = 0;

ossimImageViewTransformFactory* ossimImageViewTransformFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageViewTransformFactory;
   }

   return theInstance;
}

ossimImageViewTransform* ossimImageViewTransformFactory::createTransform(const ossimKeywordlist& kwl,
                                                                         const char* prefix)
{
   ossimImageViewTransform* result=NULL;
   
   const char *lookup = kwl.find(prefix, "type");

   if(lookup)
   {
      if(ossimString(lookup) == STATIC_TYPE_NAME(ossimImageViewProjectionTransform))
      {
         result = new ossimImageViewProjectionTransform;
      }
      if(ossimString(lookup) == STATIC_TYPE_NAME(ossimImageViewAffineTransform))
      {
         result = new ossimImageViewAffineTransform;
      }
   }
   if(result)
   {
      result->loadState(kwl, prefix);
   }
   
   return result;
}
   
