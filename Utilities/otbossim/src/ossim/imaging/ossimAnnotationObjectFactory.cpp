//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationObjectFactory.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/imaging/ossimAnnotationObjectFactory.h>

#include <ossim/imaging/ossimGeoAnnotationEllipseObject.h>
#include <ossim/imaging/ossimGeoAnnotationFontObject.h>
#include <ossim/imaging/ossimGeoAnnotationMultiPolyLineObject.h>
#include <ossim/imaging/ossimGeoAnnotationPolyObject.h>

ossimAnnotationObjectFactory*  ossimAnnotationObjectFactory::theInstance=NULL;

ossimAnnotationObjectFactory::ossimAnnotationObjectFactory()
   :ossimFactoryBase<ossimAnnotationObject>()
{
}

ossimAnnotationObject* ossimAnnotationObjectFactory::create(
   const ossimString& spec) const
{
   if(spec == STATIC_TYPE_NAME(ossimGeoAnnotationEllipseObject))
   {
      return new ossimGeoAnnotationEllipseObject;
   }
   if(spec == STATIC_TYPE_NAME(ossimGeoAnnotationFontObject))
   {
      return new ossimGeoAnnotationFontObject;
   }
   if(spec == STATIC_TYPE_NAME(ossimGeoAnnotationMultiPolyLineObject))
   {
      return new ossimGeoAnnotationMultiPolyLineObject;
   }
   if(spec == STATIC_TYPE_NAME(ossimGeoAnnotationPolyObject))
   {
      return new ossimGeoAnnotationPolyObject;
   }

   return NULL;
}

ossimAnnotationObject* ossimAnnotationObjectFactory::create(
   const ossimKeywordlist& kwl,
   const char* prefix)    const
{
   ossimAnnotationObject* result = NULL;
   
   const char* type = kwl.find(prefix, "type");
   if(type)
   {
      result = create(ossimString(type));
      if(result)
      {
         result->loadState(kwl, prefix);
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimAnnotationObjectFactory::create WARNING"
            << "Unable to create object of type:  "
            << type << std::endl;
      }
   }

   return result;
}

ossimAnnotationObjectFactory* ossimAnnotationObjectFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimAnnotationObjectFactory();
   }

   return theInstance;
}

//*****************************************************************************
//  METHOD: ossimProjectionFactory::getList()
//  
//*****************************************************************************
list<ossimString> ossimAnnotationObjectFactory::getList() const 
{
   list<ossimString> rtn_list;
   list<ossimString> sub_list;
   list<ossimFactoryBase<ossimAnnotationObject>*>::const_iterator factory;

   factory = theRegistry.begin();
   while(factory != theRegistry.end())
   {
      sub_list = (*factory)->getList();
      rtn_list.merge(sub_list);
      ++factory;
   }

   return rtn_list;
}
