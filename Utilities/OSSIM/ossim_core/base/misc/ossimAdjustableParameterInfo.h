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
// $Id: ossimAdjustableParameterInfo.h,v 1.6 2004/01/09 18:32:00 gpotts Exp $
#ifndef ossimAdjustableParameterInfo_HEADER
#define ossimAdjustableParameterInfo_HEADER
#include <iostream>
#include "base/common/ossimConstants.h"
#include "base/data_types/ossimString.h"
#include "base/data_types/ossimKeywordlist.h"

class ossimAdjustableParameterInfo
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimAdjustableParameterInfo& data);
   
   ossimAdjustableParameterInfo()
      : theParameter(0.0),
        theSigma(0.0),
	theCenter(0.0),
        theUnit(OSSIM_UNIT_UNKNOWN),
        theDescription("")
      {
      }
   double getParameter()const
      {
         return theParameter;
      }
   void setParameter(double parameter)
      {
         theParameter = parameter;
      }
   double getSigma()const
      {
         return theSigma;
      }
   void setSigma(double sigma)
      {
         theSigma = sigma;
      }
   const ossimString& getDescription()const
      {
         return theDescription;
      }
   void setDescription(const ossimString& description)
      {
         theDescription = description;
      }
   ossimUnitType getUnit()const
      {
         return theUnit;
      }
   void setUnit(ossimUnitType unit)
      {
         theUnit = unit;
      }
   ossimString getUnitAsString()const;

   void setCenter(double center);
   double getCenter()const;

  /*!
   * will return theCenter + theSigma*theParameter
   */
   double computeOffset()const;

   bool loadState(const ossimKeywordlist& kwl,
                  const ossimString& prefix=ossimString(""));
   bool saveState(ossimKeywordlist& kwl,
                  const ossimString& prefix=ossimString(""))const;
protected:
   double        theParameter;
   double        theSigma;
   double        theCenter;
   ossimUnitType theUnit;     
   ossimString   theDescription;
};

#endif
