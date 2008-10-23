//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LGPL
// 
// Author:  Garrett Potts
//
// Description:
//
// ossimLensDistortion
//*******************************************************************
//  $Id: ossimLensDistortion.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <sstream>
#include <ossim/projection/ossimLensDistortion.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>

RTTI_DEF1(ossimLensDistortion, "ossimLensDistortion", ossim2dTo2dTransform);

bool ossimLensDistortion::saveState(ossimKeywordlist& kwl,
                                    const char* prefix)const
{
   ossim2dTo2dTransform::saveState(kwl, prefix);
   
   kwl.add(prefix,
           "center",
           ossimString::toString(theCenter.x) + " " + ossimString::toString(theCenter.y),
           true);

   return true;
}

bool ossimLensDistortion::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   const char* center = kwl.find(prefix, "center");
   theCenter.x = 0.0;
   theCenter.y = 0.0;
   
   if(center)
   {
      std::istringstream in(center);
      
      in >> theCenter.x >> theCenter.y;
   }

   return ossim2dTo2dTransform::loadState(kwl, prefix);

}
