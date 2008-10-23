//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: .
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactory.cpp 9935 2006-11-22 19:30:28Z dburken $

#include <cstddef> /* for NULL */

#include <ossim/imaging/ossimOverviewBuilderFactory.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>

ossimOverviewBuilderFactory*
ossimOverviewBuilderFactory::theInstance = NULL;

ossimOverviewBuilderFactory* ossimOverviewBuilderFactory::instance()
{
   if ( !theInstance )
   {
      theInstance = new ossimOverviewBuilderFactory();
   }
   return theInstance;
}

ossimOverviewBuilderFactory::~ossimOverviewBuilderFactory()
{
   theInstance = NULL;
}

ossimOverviewBuilderBase* ossimOverviewBuilderFactory::createBuilder(
   const ossimString& typeName) const
{
   ossimOverviewBuilderBase* result = new  ossimTiffOverviewBuilder();
   if ( result->hasOverviewType(typeName) == true )
   {
      // Capture the type.  (This builder has more than one.)
      result->setOverviewType(typeName);
   }
   else
   {
      delete result;
      result = NULL;
   }
   
   return result;
}

void ossimOverviewBuilderFactory::getTypeNameList(
   std::vector<ossimString>& typeList) const
{
   ossimOverviewBuilderBase* builder = new  ossimTiffOverviewBuilder();
   builder->getTypeNameList(typeList);
   delete builder;
   builder = NULL;
}

ossimOverviewBuilderFactory::ossimOverviewBuilderFactory()
{
}

ossimOverviewBuilderFactory::ossimOverviewBuilderFactory(
   const ossimOverviewBuilderFactory& /* obj */)
{
}

void ossimOverviewBuilderFactory::operator=(
   const ossimOverviewBuilderFactory& /* rhs */)
{
}
