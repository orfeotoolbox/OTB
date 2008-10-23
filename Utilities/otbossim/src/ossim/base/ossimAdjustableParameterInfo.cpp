//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAdjustableParameterInfo.cpp 11347 2007-07-23 13:01:59Z gpotts $
#include <sstream>
#include <algorithm>
#include <ossim/base/ossimAdjustableParameterInfo.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimCommon.h>
// static const char* PARAM_NAME_KW       = "name";
// static const char* PARAM_UNITS_KW      = "units";
static const char* PARAM_KW            = "parameter";
static const char* PARAM_SIGMA_KW      = "sigma";
static const char* PARAM_CENTER_KW     = "center";
static const char* PARAM_LOCK_FLAG_KW  = "lock_flag";

std::ostream& operator <<(std::ostream& out, const ossimAdjustableParameterInfo& data)
{
   out << "description: " << data.theDescription << std::endl
       << "center:      " << data.theCenter <<  std::endl
       << "parameter:   " << data.theParameter << std::endl
       << "sigma:       " << data.theSigma << std::endl
       << "units:       " << (ossimUnitTypeLut::instance()->getEntryString(data.theUnit))
       << std::endl
       << "locked:       " << (data.theLockFlag?"true":"false") << std::endl;
   
   return out;
}


ossimString ossimAdjustableParameterInfo::getUnitAsString()const
{
   return ossimUnitTypeLut::instance()->getEntryString((int)theUnit);
}

void ossimAdjustableParameterInfo::setCenter(double center)
{
   if(!theLockFlag)
   {
      theCenter = center;
   }
}

double ossimAdjustableParameterInfo::getCenter()const
{
  return theCenter;
}

double ossimAdjustableParameterInfo::computeOffset()const
{
  return theCenter + theSigma*theParameter;
}

void ossimAdjustableParameterInfo::setOffset(ossim_float64 value)
{
   if(!theLockFlag)
   {
      double minValue = theCenter - theSigma;
      double maxValue = theCenter + theSigma;
      double x = 0.0;
      
      if(std::abs(theSigma) > DBL_EPSILON)
      {
         x = (value - theCenter)/theSigma;
         
         value = theCenter + x*theSigma;
         
         if(value < minValue)
         {
            x = -1;
         }
         else if(value > maxValue)
         {
            x = 1.0;
         }
         theParameter = x;
      }
   }
}

bool ossimAdjustableParameterInfo::loadState(const ossimKeywordlist& kwl,
                                             const ossimString& prefix)
{
   const char* param       = kwl.find(prefix, PARAM_KW);
   const char* sigma       = kwl.find(prefix, PARAM_SIGMA_KW);
   const char* center      = kwl.find(prefix, PARAM_CENTER_KW);
   const char* unit        = kwl.find(prefix, ossimKeywordNames::UNITS_KW);
   const char* locked      = kwl.find(prefix, PARAM_LOCK_FLAG_KW);
   
   theDescription          = kwl.find(prefix, ossimKeywordNames::DESCRIPTION_KW);
   
   if(param)
   {
      theParameter = ossimString(param).toDouble();
   }
   else
   {
      theParameter = 0.0;
   }
   if(unit)
   {
      theUnit = (ossimUnitType)(ossimUnitTypeLut::instance()->getEntryNumber(unit));
   }
   else
   {
      theUnit      = OSSIM_UNIT_UNKNOWN;
   }
   if(sigma)
   {
      theSigma  = ossimString(sigma).toDouble();
   }
   else
   {
      theSigma     = 0.0;
   }
   if(center)
   {
     theCenter = ossimString(center).toDouble();
   }
   else
   {
     theCenter = 0.0;
   }
   if(locked)
   {
      theLockFlag = ossimString(locked).toBool();
   }
   
   return true;
}

bool ossimAdjustableParameterInfo::saveState(ossimKeywordlist& kwl,
                                             const ossimString& prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::DESCRIPTION_KW,
           theDescription,
           true);
   kwl.add(prefix, ossimKeywordNames::UNITS_KW,
           ossimUnitTypeLut::instance()->getEntryString(theUnit), true);
   kwl.add(prefix, PARAM_KW, theParameter, true);
   kwl.add(prefix, PARAM_SIGMA_KW, theSigma, true);
   kwl.add(prefix, PARAM_CENTER_KW, theCenter, true);
   kwl.add(prefix, PARAM_LOCK_FLAG_KW, theLockFlag, true);
   
   return true;
}
