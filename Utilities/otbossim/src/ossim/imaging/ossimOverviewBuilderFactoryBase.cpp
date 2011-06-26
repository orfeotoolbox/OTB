//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: The base for overview builder factories.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactoryBase.cpp 19187 2011-03-23 12:21:58Z gpotts $

#include <ossim/imaging/ossimOverviewBuilderFactoryBase.h>
#include <ossim/imaging/ossimOverviewBuilderBase.h>


ossimObject* ossimOverviewBuilderFactoryBase::createObject(const ossimString& typeName) const 
{ 
   return createBuilder(typeName); 
}

ossimObject* ossimOverviewBuilderFactoryBase::createObject(const ossimKeywordlist& kwl, const char* prefix) const
{
   ossimObject* result = 0;
   ossimString typeValue = kwl.find(prefix, "type");
   if(!typeValue.empty())
   {
      result = createObject(typeValue);
      if(result)
      {
         if(!result->loadState(kwl, prefix))
         {
            delete result;
            result = 0;
         }
      }
   }
   
   return result;
}


ossimOverviewBuilderFactoryBase::ossimOverviewBuilderFactoryBase()
{
}

ossimOverviewBuilderFactoryBase::~ossimOverviewBuilderFactoryBase()
{
}
