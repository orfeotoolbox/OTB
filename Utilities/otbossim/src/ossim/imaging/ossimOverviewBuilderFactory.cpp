//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: .
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactory.cpp 15833 2009-10-29 01:41:53Z eshirschorn $

#include <cstddef> /* for NULL */

#include <ossim/imaging/ossimOverviewBuilderFactory.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/imaging/ossimVirtualOverviewBuilder.h>

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
   ossimRefPtr<ossimOverviewBuilderBase> result = new ossimTiffOverviewBuilder();
   if ( result->hasOverviewType(typeName) == false )
   {
      result = new ossimVirtualOverviewBuilder();
   }
   if ( result->hasOverviewType(typeName) == false )
   {
      result = 0;
   }

   if ( result.get() )
   {
      result->setOverviewType(typeName);
   }
   return result.release();
}

void ossimOverviewBuilderFactory::getTypeNameList(
   std::vector<ossimString>& typeList) const
{
   ossimRefPtr<ossimOverviewBuilderBase> builder = new  ossimTiffOverviewBuilder();
   builder->getTypeNameList(typeList);

   builder = new  ossimVirtualOverviewBuilder();
   builder->getTypeNameList(typeList);
   builder = 0;
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
