//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimUnitConversionTool.h 10378 2007-01-26 14:27:21Z gpotts $
#ifndef ossimUnitConversionTool_HEADER
#define ossimUnitConversionTool_HEADER
#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimGpt.h>

class OSSIM_DLL ossimUnitConversionTool
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimUnitConversionTool& data);

   ossimUnitConversionTool(double value=1.0,
                           ossimUnitType unitType=OSSIM_METERS);

   ossimUnitConversionTool(const ossimGpt& origin,
                           double value,
                           ossimUnitType unitType=OSSIM_METERS);

   void setOrigin(const ossimGpt& gpt);

   ossimGpt getOrigin()const;

   void getOrigin(ossimGpt& result)const;

   void setValue(double value, ossimUnitType unitType=OSSIM_METERS);

   double getValue(ossimUnitType unitType=OSSIM_METERS) const;
   
   void setMeters(double value);
   void setRadians(double value);
   void setDegrees(double value);
   void setMinutes(double value);
   void setSeconds(double value);
   void setUsSurveyFeet(double value);
   void setFeet(double value);
   void setNauticalMiles(double value);
   void setMiles(double value);
   void setMillimeters(double value);
   void setMicrons(double value);
   void setCentimeters(double value);
   void setYards(double value);
   void setInches(double value);
   void setKilometers(double value);
   double getMeters()const;
   double getRadians()const;
   double getDegrees()const;
   double getMinutes()const;
   double getSeconds()const;
   double getUsSurveyFeet()const;
   double getFeet()const;
   double getNauticalMiles()const;
   double getMiles()const;
   double getMillimeters()const;
   double getMicrons()const;
   double getCentimeters()const;
   double getYards()const;
   double getInches()const;
   double getKilometers()const;
   
protected:
   double         theValue;
   ossimUnitType  theUnitType;
   ossimGpt       theOrigin;

   double computeMeters()const;
};

#endif
