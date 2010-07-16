//----------------------------------------------------------------------------
// 
// See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Factory for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderFactory.cpp 17709 2010-07-08 20:21:14Z dburken $


#include <ossim/imaging/ossimOverviewBuilderFactory.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>

ossimOverviewBuilderFactory* ossimOverviewBuilderFactory::theInstance = 0;

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
   theInstance = 0;
}

ossimOverviewBuilderBase* ossimOverviewBuilderFactory::createBuilder(
   const ossimString& typeName) const
{
   ossimRefPtr<ossimOverviewBuilderBase> result = new ossimTiffOverviewBuilder();
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
