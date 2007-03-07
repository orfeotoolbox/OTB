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
// $Id: ossimDemProfile.h,v 1.4 2005/01/11 18:10:38 dburken Exp $

#ifndef ossimDemProfile_HEADER
#define ossimDemProfile_HEADER

#include "ossimDemPoint.h"

#include <vector>

typedef std::vector<long> ossimDemElevationVector;

class ossimDemProfile
{
public:

   ossimDemProfile();
   ossimDemProfile(ossimDemProfile const& right);
   ossimDemProfile const& operator=(ossimDemProfile const& right);

   friend ostream& operator<<(ostream& s, ossimDemProfile& demp);
   friend istream& operator>>(istream& s, ossimDemProfile& demp);

   // Accessors

   /*!
    *  Row identification number. Typically set to 1.
    */
   long getRowNumber() const;

   /*!
    *  Column identification number. This is the profile sequence number.
    */
   long getColumnNumber() const;

   /*!
    *  Number of elevations in this profile.
    */
   long getNumberOfElevations() const;

   /*!
    *  Location (in ground planimetric coordinates) of the first
    */
   ossimDemPoint const& getProfileLocation() const;

   /*!
    *  Elevation of local datum for the profile.
    *  Value is in units of measure given by the Elevation Units field
    *  found in the DEM header.
    */
   double getLocalDatumElevation() const;

   /*!
    *  Minimum elevation for this profile.
    */
   double getMinimumElev() const;

   /*!
    *  Maximum elevation for this profile.
    */
   double getMaximumElev() const;

   /*!
   *  Elevation values.
   *  From the DEM standard:
   *  "A value in this array would be multiplied by the spatial resolution
   *  value and added to the elevation of the local elevation datum for the
   *  element profile to obtain the elevation for the point."
   */
   ossimDemElevationVector const& getElevations() const;

private:

   long _row;
   long _column;
   long _numberElevations;
   ossimDemPoint _profileLocation;
   double _locDatumElev;
   double _minElev;
   double _maxElev;
   ossimDemElevationVector _elevations;
};

#endif  // #ifndef ossimDemProfile_HEADER
