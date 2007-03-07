//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimAdjustableParameterInfo.cpp,v 1.8 2004/01/09 18:32:00 gpotts Exp $
#include <sstream>
#include <algorithm>
#include "ossimAdjustableParameterInfo.h"
#include "base/misc/lookup_tables/ossimUnitTypeLut.h"
#include "base/common/ossimKeywordNames.h"

// static const char* PARAM_NAME_KW       = "name";
// static const char* PARAM_UNITS_KW      = "units";
static const char* PARAM_KW            = "parameter";
static const char* PARAM_SIGMA_KW      = "sigma";
static const char* PARAM_CENTER_KW     = "center";

std::ostream& operator <<(std::ostream& out, const ossimAdjustableParameterInfo& data)
{
   out << "description: " << data.theDescription << std::endl
       << "center:      " << data.theCenter <<  std::endl
       << "parameter:   " << data.theParameter << std::endl
       << "sigma:       " << data.theSigma << std::endl
       << "units:       " << ossimUnitTypeLut().getEntryString(data.theUnit) << endl;
   
   return out;
}


ossimString ossimAdjustableParameterInfo::getUnitAsString()const
{
   return ossimUnitTypeLut().getEntryString((int)theUnit);
}

void ossimAdjustableParameterInfo::setCenter(double center)
{
  theCenter = center;
}

double ossimAdjustableParameterInfo::getCenter()const
{
  return theCenter;
}

double ossimAdjustableParameterInfo::computeOffset()const
{
  return theCenter + theSigma*theParameter;
}

bool ossimAdjustableParameterInfo::loadState(const ossimKeywordlist& kwl,
                                             const ossimString& prefix)
{
   const char* param       = kwl.find(prefix, PARAM_KW);
   const char* sigma       = kwl.find(prefix, PARAM_SIGMA_KW);
   const char* center      = kwl.find(prefix, PARAM_CENTER_KW);
   const char* unit        = kwl.find(prefix, ossimKeywordNames::UNITS_KW);
   
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
      theUnit = (ossimUnitType)(ossimUnitTypeLut().getEntryNumber(unit));
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

   return true;
}

bool ossimAdjustableParameterInfo::saveState(ossimKeywordlist& kwl,
                                             const ossimString& prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::DESCRIPTION_KW,
           theDescription,
           true);
   kwl.add(prefix, ossimKeywordNames::UNITS_KW, ossimUnitTypeLut().getEntryString(theUnit), true);
   kwl.add(prefix, PARAM_KW, theParameter, true);
   kwl.add(prefix, PARAM_SIGMA_KW, theSigma, true);
   kwl.add(prefix, PARAM_CENTER_KW, theCenter, true);

   return true;
}
