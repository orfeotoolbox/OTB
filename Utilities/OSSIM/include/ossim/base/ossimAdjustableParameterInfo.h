//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimAdjustableParameterInfo.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAdjustableParameterInfo_HEADER
#define ossimAdjustableParameterInfo_HEADER
#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>

class OSSIM_DLL ossimAdjustableParameterInfo
{
public:
   OSSIM_DLL friend std::ostream& operator <<(std::ostream& out, const ossimAdjustableParameterInfo& data);
   
   ossimAdjustableParameterInfo()
      : theParameter(0.0),
        theSigma(0.0),
	theCenter(0.0),
        theUnit(OSSIM_UNIT_UNKNOWN),
      theDescription(""),
      theLockFlag(false)
      {
      }
   ossimAdjustableParameterInfo(const ossimAdjustableParameterInfo& rhs)
      :theParameter(rhs.theParameter),
      theSigma(rhs.theSigma),
      theCenter(rhs.theCenter),
      theUnit(rhs.theUnit),
      theDescription(rhs.theDescription),
      theLockFlag(rhs.theLockFlag)
      {
      }
   double getParameter()const
      {
         return theParameter;
      }
   void setParameter(double parameter)
      {
         if(!theLockFlag)
         {
            theParameter = parameter;
         }
      }
   double getSigma()const
      {
         return theSigma;
      }
   void setSigma(double sigma)
      {
         if(!theLockFlag)
         {
            theSigma = sigma;
         }
      }
   const ossimString& getDescription()const
      {
         return theDescription;
      }
   
   void setDescription(const ossimString& description)
      {
         if(!theLockFlag)
         {
            theDescription = description;
         }
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

   void setOffset(ossim_float64 value);
   
  /*!
   * will return theCenter + theSigma*theParameter
   */
   double computeOffset()const;

   void setLockFlag(bool flag)
   {
      theLockFlag = flag;
   }
   bool getLockFlag()const
   {
      return theLockFlag;
   }
   
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
   bool          theLockFlag;
};

#endif
