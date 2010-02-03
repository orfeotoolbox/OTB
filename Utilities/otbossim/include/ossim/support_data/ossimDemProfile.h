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
// $Id: ossimDemProfile.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimDemProfile_HEADER
#define ossimDemProfile_HEADER

#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimDemPoint.h>

typedef std::vector<ossim_int32> ossimDemElevationVector;

class ossimDemProfile
{
public:

   ossimDemProfile();
   ossimDemProfile(ossimDemProfile const& right);
   ossimDemProfile const& operator=(ossimDemProfile const& right);

   friend std::ostream& operator<<(std::ostream& s, ossimDemProfile& demp);
   friend std::istream& operator>>(std::istream& s, ossimDemProfile& demp);

   // Accessors

   /*!
    *  Row identification number. Typically set to 1.
    */
   ossim_int32 getRowNumber() const;

   /*!
    *  Column identification number. This is the profile sequence number.
    */
   ossim_int32 getColumnNumber() const;

   /*!
    *  Number of elevations in this profile.
    */
   ossim_int32 getNumberOfElevations() const;

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

   ossim_int32 _row;
   ossim_int32 _column;
   ossim_int32 _numberElevations;
   ossimDemPoint _profileLocation;
   double _locDatumElev;
   double _minElev;
   double _maxElev;
   ossimDemElevationVector _elevations;
};

#endif  // #ifndef ossimDemProfile_HEADER
