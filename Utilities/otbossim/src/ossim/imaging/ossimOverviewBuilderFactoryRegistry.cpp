//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: The factory registry for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactoryRegistry.cpp 9935 2006-11-22 19:30:28Z dburken $

#include <cstddef>   /* for NULL */
#include <algorithm> /* for std::find */

#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryBase.h>

ossimOverviewBuilderFactoryRegistry*
ossimOverviewBuilderFactoryRegistry::theInstance = NULL;

ossimOverviewBuilderFactoryRegistry*
ossimOverviewBuilderFactoryRegistry::instance()
{
   if ( theInstance == NULL )
   {
      theInstance = new ossimOverviewBuilderFactoryRegistry();
   }
   return theInstance;
}

bool ossimOverviewBuilderFactoryRegistry::registerFactory(
   ossimOverviewBuilderFactoryBase* factory, bool pushToFrontFlag)
{
   if ( !factory )
   {
      return false;
   }

   if ( std::find(theFactoryList.begin(), theFactoryList.end(), factory) !=
        theFactoryList.end() )
   {
      // Already in list...
      return false;
   }

   if ( pushToFrontFlag )
   {
      theFactoryList.insert(theFactoryList.begin(), factory);
   }
   else
   {
      theFactoryList.push_back(factory);
   }
   
   return true;
}

void ossimOverviewBuilderFactoryRegistry::unregisterFactory(
   ossimOverviewBuilderFactoryBase* factory)
{
   std::vector<ossimOverviewBuilderFactoryBase*>::iterator iter =
      std::find(theFactoryList.begin(), theFactoryList.end(), factory);
   if(iter != theFactoryList.end())
   {
      theFactoryList.erase(iter);
   }
}

ossimOverviewBuilderBase*
ossimOverviewBuilderFactoryRegistry::createBuilder(
   const ossimString& typeName) const
{
   ossimOverviewBuilderBase* result = NULL;

   std::vector<ossimOverviewBuilderFactoryBase*>::const_iterator i =
      theFactoryList.begin();

   while ( i != theFactoryList.end() )
   {
      result = (*i)->createBuilder(typeName);
      if (result)
      {
         break;
      }
      ++i;
   }
   
   return result;
}

void ossimOverviewBuilderFactoryRegistry::getTypeNameList(
   std::vector<ossimString>& typeList)const
{
   std::vector<ossimOverviewBuilderFactoryBase*>::const_iterator i =
      theFactoryList.begin();
   
   while ( i != theFactoryList.end() )
   {
      (*i)->getTypeNameList(typeList);
      ++i;
   } 
}

ossimOverviewBuilderFactoryRegistry::ossimOverviewBuilderFactoryRegistry()
   : theFactoryList()
{
}

ossimOverviewBuilderFactoryRegistry::ossimOverviewBuilderFactoryRegistry(
   const ossimOverviewBuilderFactoryRegistry& /* obj */ )
   : theFactoryList()
{
}

void ossimOverviewBuilderFactoryRegistry::operator=(
   const ossimOverviewBuilderFactoryRegistry& /* rhs */ )
{
   
}
