//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: David Burken <dburken@imagelinks.com>
//
//*************************************************************************
// $Id: ossimImageSourceFactoryBase.cpp 15766 2009-10-20 12:37:09Z gpotts $

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
   ossimRefPtr<ossimObject> result =createObject(name);
   
   if(PTR_CAST(ossimImageSource, result.get()))
   {
      return (ossimImageSource*)result.release();
   }
   result = 0;

   return 0;
}

ossimImageSource* ossimImageSourceFactoryBase::createImageSource(const ossimKeywordlist& kwl,
                                                                 const char* prefix)const
{
   ossimRefPtr<ossimObject> result =createObject(kwl, prefix);
   
   if(PTR_CAST(ossimImageSource, result.get()))
   {
      return (ossimImageSource*)result.release();
   }
   result = 0;
   
   return (ossimImageSource*)0;
}
