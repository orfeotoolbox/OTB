//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: David Burken <dburken@imagelinks.com>
//
//*************************************************************************
// $Id: ossimImageSourceFactoryBase.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSource.h>
ossimImageSourceFactoryBase::ossimImageSourceFactoryBase()
   : ossimObjectFactory()
{}

ossimImageSourceFactoryBase::ossimImageSourceFactoryBase(const ossimImageSourceFactoryBase&)
   : ossimObjectFactory()
{}

const ossimImageSourceFactoryBase& ossimImageSourceFactoryBase::operator=(const ossimImageSourceFactoryBase&)
{
   return *this;
}

ossimImageSource* ossimImageSourceFactoryBase::createImageSource(const ossimString& name)const
{
   ossimObject* result =createObject(name);
   
   if(PTR_CAST(ossimImageSource, result))
   {
      return (ossimImageSource*)result;
   }

   if(result)
   {
      delete result;
      result = NULL;
   }

   return (ossimImageSource*)NULL;
}

ossimImageSource* ossimImageSourceFactoryBase::createImageSource(const ossimKeywordlist& kwl,
                                                                 const char* prefix)const
{
   ossimObject* result =createObject(kwl, prefix);
   
   if(PTR_CAST(ossimImageSource, result))
   {
      return (ossimImageSource*)result;
   }

   if(result)
   {
      delete result;
      result = NULL;
   }

   return (ossimImageSource*)NULL;
}
