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
// $Id: ossimUnitConversionTool.cpp 17503 2010-06-02 11:18:49Z dburken $

#include <iostream>
#include <iomanip>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimGpt.h>

std::ostream& operator<<(std::ostream& out,
                         const ossimUnitConversionTool& data)
{
   out << std::setprecision(15) << std::setiosflags(std::ios::fixed)
       << "Meters:           " << data.getMeters()
       << "\nFeet:             " << data.getFeet()
       << "\nU.S. Survey Feet: " << data.getUsSurveyFeet()
       << "\nNautical miles:   " << data.getNauticalMiles()
       << "\nDegrees:          " << data.getDegrees()
       << "\nMinutes:          " << data.getMinutes()
       << "\nSeconds:          " << data.getSeconds() << std::endl;
   
   return out;
}

ossimUnitConversionTool::ossimUnitConversionTool(double value,
                                                 ossimUnitType unitType)
   : theValue(value),
     theUnitType(unitType),
     theOrigin()
{
}

ossimUnitConversionTool::ossimUnitConversionTool(const ossimGpt& origin,
                                                 double value,
                                                 ossimUnitType unitType)
   : theValue(value),
     theUnitType(unitType),
     theOrigin(origin)
{
}

void ossimUnitConversionTool::setOrigin(const ossimGpt& gpt)
{
   theOrigin = gpt;
}

ossimGpt ossimUnitConversionTool::getOrigin()const
{
   return theOrigin;
}

void ossimUnitConversionTool::getOrigin(ossimGpt& result)const
{
   result = theOrigin;
}

void ossimUnitConversionTool::setValue(double value,
                                       ossimUnitType unitType)
{
   theValue    = value;
   theUnitType = unitType;
}

double ossimUnitConversionTool::getValue(ossimUnitType unitType) const
{
   switch(unitType)
   {
      case OSSIM_METERS:
      {
         return getMeters();
      }
      case OSSIM_RADIANS:
      {
         return getRadians();
      }
      case OSSIM_DEGREES:
      {
         return getDegrees();
      }
      case OSSIM_US_SURVEY_FEET:
      {
         return getUsSurveyFeet();
      }
      case OSSIM_FEET:
      {
         return getFeet();
      }
      case OSSIM_SECONDS:
      {
         return getSeconds();
      }
      case OSSIM_MINUTES:
      {
         return getMinutes();
      }
      case OSSIM_NAUTICAL_MILES:
      {
         return getNauticalMiles();
      }
      case OSSIM_MILES:
      {
         return getMiles();
      }
      case OSSIM_MICRONS:
      {
         return getMicrons();
      }
      case OSSIM_CENTIMETERS:
      {
         return getCentimeters();
      }
      case OSSIM_MILLIMETERS:
      {
         return getMillimeters();
      }
      case OSSIM_YARDS:
      {
         return getYards();
      }
      case OSSIM_INCHES:
      {
         return getInches();
      }
      case OSSIM_KILOMETERS:
      {
         return getKilometers();
      }
      default:
         break;
   }

   return ossim::nan();
}

void ossimUnitConversionTool::setMeters(double value)
{
   setValue(value, OSSIM_METERS);
}

void ossimUnitConversionTool::setRadians(double value)
{
   setValue(value, OSSIM_RADIANS);
}

void ossimUnitConversionTool::setDegrees(double value)
{
   setValue(value, OSSIM_DEGREES);
}

void ossimUnitConversionTool::setMinutes(double value)
{
   setValue(value, OSSIM_MINUTES);
}

void ossimUnitConversionTool::setSeconds(double value)
{
   setValue(value, OSSIM_SECONDS);
}

void ossimUnitConversionTool::setUsSurveyFeet(double value)
{
   setValue(value, OSSIM_US_SURVEY_FEET);
}

void ossimUnitConversionTool::setFeet(double value)
{
   setValue(value, OSSIM_FEET);
}

void ossimUnitConversionTool::setNauticalMiles(double value)
{
   setValue(value, OSSIM_NAUTICAL_MILES);
}

void ossimUnitConversionTool::setMiles(double value)
{
   setValue(value, OSSIM_MILES);
}

void ossimUnitConversionTool::setMillimeters(double value)
{
   setValue(value, OSSIM_MILLIMETERS);
}

void ossimUnitConversionTool::setMicrons(double value)
{
   setValue(value, OSSIM_MICRONS);
}

void ossimUnitConversionTool::setCentimeters(double value)
{
   setValue(value, OSSIM_CENTIMETERS);
}

void ossimUnitConversionTool::setYards(double value)
{
   setValue(value, OSSIM_YARDS);
}

void ossimUnitConversionTool::setInches(double value)
{
   setValue(value, OSSIM_INCHES);
}

void ossimUnitConversionTool::setKilometers(double value)
{
   setValue(value, OSSIM_KILOMETERS);
}

double ossimUnitConversionTool::getMeters()const
{
   if(theUnitType == OSSIM_METERS)
   {
      return theValue;
   }
   
   return computeMeters();
}

double ossimUnitConversionTool::getRadians()const
{
   if(theUnitType == OSSIM_RADIANS)
   {
      return theValue;
   }
   return getDegrees()*RAD_PER_DEG;
}

double ossimUnitConversionTool::getDegrees()const
{
   switch (theUnitType)
   {
      case OSSIM_DEGREES:
      {
         return theValue;
      }
      case OSSIM_MINUTES:
      {
         return (theValue / 60.0);
      }
      case OSSIM_SECONDS:
      {
         return (theValue / 3600.0);
      }
      case OSSIM_RADIANS:
      {
         return (theValue * DEG_PER_RAD);
      }
      default:
         break;
   }
   
   ossimDpt pt = theOrigin.metersPerDegree();
   return (computeMeters() /((pt.x+pt.y)*.5));
}

double ossimUnitConversionTool::getMinutes()const
{
   if(theUnitType == OSSIM_MINUTES)
   {
      return theValue;
   }
   return (getDegrees()*60.0);
}

double ossimUnitConversionTool::getSeconds()const
{
   if(theUnitType == OSSIM_SECONDS)
   {
      return theValue;
   }
   return (getDegrees()*3600.0);
}

double ossimUnitConversionTool::getUsSurveyFeet()const
{
   if(theUnitType == OSSIM_US_SURVEY_FEET)
   {
      return theValue;
   }
   return (computeMeters()/US_METERS_PER_FT);
}

double ossimUnitConversionTool::getFeet()const
{
   if(theUnitType == OSSIM_FEET)
   {
      return theValue;
   }
   return (computeMeters()*FT_PER_MTRS);
}

double ossimUnitConversionTool::getNauticalMiles()const
{
   if(theUnitType == OSSIM_NAUTICAL_MILES)
   {
      return theValue;
   }
   
   return (computeMeters()/(theOrigin.metersPerDegree().y/60.0));
}

double ossimUnitConversionTool::getMiles()const
{
   if(theUnitType == OSSIM_MILES)
   {
      return theValue;
   }
   return ((computeMeters()*FT_PER_MTRS)/FT_PER_MILE);
}

double ossimUnitConversionTool::getMillimeters()const
{
   if(theUnitType == OSSIM_MILLIMETERS)
   {
      return theValue;
   }
   return ((computeMeters()*1e3));
}

double ossimUnitConversionTool::getMicrons()const
{
   if(theUnitType == OSSIM_MICRONS)
   {
      return theValue;
   }
   return ((computeMeters()*1e6));
}

double ossimUnitConversionTool::getCentimeters()const
{
   if(theUnitType == OSSIM_KILOMETERS)
   {
      return theValue;
   }

   return ((computeMeters()*1e2));   
}

double ossimUnitConversionTool::getYards()const
{
   if(theUnitType == OSSIM_INCHES)
   {
      return theValue;
   }

   return ((computeMeters()/0.914));   
}

double ossimUnitConversionTool::getInches()const
{
   if(theUnitType == OSSIM_INCHES)
   {
      return theValue;
   }

   return ((getFeet()*12.0));   
}

double ossimUnitConversionTool::getKilometers()const
{
   if(theUnitType == OSSIM_KILOMETERS)
   {
      return theValue;
   }
   return ((computeMeters()*1e-3));
}

double ossimUnitConversionTool::computeMeters()const
{
   switch(theUnitType)
   {
      case OSSIM_METERS:
      {
         return theValue;
         break;
      }
      case OSSIM_RADIANS:
      {
         ossimDpt pt = theOrigin.metersPerDegree();
         return (theValue*DEG_PER_RAD)*((pt.x+pt.y)*.5);
      }
      case OSSIM_DEGREES:
      {
         ossimDpt pt = theOrigin.metersPerDegree();
         return theValue*((pt.x+pt.y)*.5);
      }
      case OSSIM_US_SURVEY_FEET:
      {
         return  US_METERS_PER_FT*theValue;
      }
      case OSSIM_FEET:
      {
         return MTRS_PER_FT*theValue;
      }
      case OSSIM_SECONDS:
      {
         ossimDpt pt = theOrigin.metersPerDegree();
         return (theValue/3600.0)*((pt.x+pt.y)*.5);
      }
      case OSSIM_MINUTES:
      {
         ossimDpt pt = theOrigin.metersPerDegree();
         return (theValue/60.0)*((pt.x+pt.y)*.5);
      }
      case OSSIM_NAUTICAL_MILES:
      {
         return (theOrigin.metersPerDegree().y/60.0)*theValue;
      }
      case OSSIM_MILES:
      {
         return MTRS_PER_FT*FT_PER_MILE*theValue;
      }
      case OSSIM_MILLIMETERS:
      {
         return (theValue/(1e3));
      }
      case OSSIM_MICRONS:
      {
         return (theValue/(1e6));
      }
      default:
         break;
   }

   return ossim::nan();
}
