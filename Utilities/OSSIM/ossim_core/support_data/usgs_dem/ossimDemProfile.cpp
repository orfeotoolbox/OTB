//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
//         Orginally written by Jamie Moyers (jmoyers@geeks.com)
//         Adapted from the package KDEM.
// Description: This class defines a DEM profile.
//
//********************************************************************
// $Id: ossimDemProfile.cpp,v 1.5 2005/01/11 18:10:38 dburken Exp $

#include <stdlib.h>
#include <iostream>
#include <sstream>

using namespace std;

#include "ossimDemProfile.h"
#include "ossimDemUtil.h"

ossimDemProfile::ossimDemProfile()
   : _row(0),
     _column(0),
     _numberElevations(0),
     _locDatumElev(0.0),
     _minElev(0.0),
     _maxElev(0.0)
{
}

ossimDemProfile::ossimDemProfile(ossimDemProfile const& right)
{
   operator=(right);
}

ossimDemProfile const&
ossimDemProfile::operator=(ossimDemProfile const& right)
{
   if (this == &right)
      return *this;

   _row = right._row;
   _column = right._column;
   _numberElevations = right._numberElevations;
   _profileLocation = right._profileLocation;
   _locDatumElev = right._locDatumElev;
   _minElev = right._minElev;
   _maxElev = right._maxElev;
   _elevations = right._elevations;

   return *this;
}

long
ossimDemProfile::getRowNumber() const
{
   return _row;
}

long
ossimDemProfile::getColumnNumber() const
{
   return _column;
}

long
ossimDemProfile::getNumberOfElevations() const
{
   return _numberElevations;
}

ossimDemPoint const&
ossimDemProfile::getProfileLocation() const
{
   return _profileLocation;
}

double
ossimDemProfile::getLocalDatumElevation() const
{
   return _locDatumElev;
}

double
ossimDemProfile::getMinimumElev() const
{
   return _minElev;
}

double
ossimDemProfile::getMaximumElev() const
{
   return _maxElev;
}

ossimDemElevationVector const&
ossimDemProfile::getElevations() const
{
   return _elevations;
}

istream&
operator>>(istream& s, ossimDemProfile& demp)
{
   //   string bufstr;
   char bufstr[1024];

   ossimDemUtil::getRecord(s,bufstr);

   demp._row = ossimDemUtil::getLong(bufstr, 0, 6);
   demp._column = ossimDemUtil::getLong(bufstr, 6, 6);
   demp._numberElevations = ossimDemUtil::getLong(bufstr, 12, 6);

   double x,y;
   ossimDemUtil::getDouble(bufstr, 24, 24, x);
   ossimDemUtil::getDouble(bufstr, 48, 24, y);
   demp._profileLocation.setXY(x,y);

   ossimDemUtil::getDouble(bufstr, 72, 24, demp._locDatumElev);
   ossimDemUtil::getDouble(bufstr, 96, 24, demp._minElev);
   ossimDemUtil::getDouble(bufstr, 120, 24, demp._maxElev);

   demp._elevations.erase(demp._elevations.begin(), demp._elevations.end());   
   demp._elevations.reserve(demp._numberElevations);

   // Extract elevations in this record.
   long count = 0;
   while ((count < 146) && (count < demp._numberElevations))
   {
      demp._elevations.push_back(ossimDemUtil::getLong(bufstr, 144+(count*6), 6));
      count++;
   }
   
   long t;
   while (count < demp._numberElevations)
   {
      t = (count - 146) % 170;
      if (t == 0)
         ossimDemUtil::getRecord(s,bufstr);
      demp._elevations.push_back(ossimDemUtil::getLong(bufstr, t*6, 6));
      count++;
   }

   return s;
}


ostream&
operator<<(ostream& s, ossimDemProfile& demp)
{
   return s;
}


