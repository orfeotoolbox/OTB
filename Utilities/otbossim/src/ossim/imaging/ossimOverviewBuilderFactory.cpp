//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: .
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactory.cpp 15766 2009-10-20 12:37:09Z gpotts $

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
   ossimRefPtr<ossimOverviewBuilderBase> result = new  ossimTiffOverviewBuilder();
   if ( result->hasOverviewType(typeName) == true )
   {
      // Capture the type.  (This builder has more than one.)
      result->setOverviewType(typeName);
   }
   else
   {
      result = 0;
   }
   
   return result.release();
}

void ossimOverviewBuilderFactory::getTypeNameList(
   std::vector<ossimString>& typeList) const
{
   ossimRefPtr<ossimOverviewBuilderBase> builder = new  ossimTiffOverviewBuilder();
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
