//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Interface class for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderBase.cpp 9942 2006-11-22 21:49:44Z dburken $

#include <ossim/imaging/ossimOverviewBuilderBase.h>

RTTI_DEF3(ossimOverviewBuilderBase,
          "ossimOverviewBuilderBase",
          ossimSource,
          ossimProcessInterface,
          ossimConnectableObjectListener)
   
ossimOverviewBuilderBase::ossimOverviewBuilderBase()
{
}

ossimOverviewBuilderBase::~ossimOverviewBuilderBase()
{
}

bool ossimOverviewBuilderBase::hasOverviewType(const ossimString& type) const
{
   std::vector<ossimString> typeList;
   getTypeNameList(typeList);
   std::vector<ossimString>::const_iterator i = typeList.begin();
   while ( i != typeList.end() )
   {
      if ( (*i) == type )
      {
         return true;
      }
      ++i;
   }
   return false;
}

