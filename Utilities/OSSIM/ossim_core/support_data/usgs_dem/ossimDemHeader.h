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
// Description: This class parses a DEM header.
//
//********************************************************************
// $Id: ossimDemHeader.h,v 1.7 2005/05/16 18:58:15 dburken Exp $

#ifndef ossimDemHeader_HEADER
#define ossimDemHeader_HEADER

#include <string>
#include <iostream>
using namespace std;

#include <support_data/usgs_dem/ossimDemPoint.h>

class ossimString;
class ossimKeywordlist;

class ossimDemHeader
{
public:
   
   ossimDemHeader();
   friend ostream& operator<<(ostream& s, const ossimDemHeader& header);
   friend istream& operator>>(istream& s, ossimDemHeader& header);
   
   ostream& print(ostream& s) const;
   
   
   // Accessors

   /*! Quadrangle name. */
   string const& getQuadName() const;

   /*! Free format desciptor field containing process information. */
   string const& getProcessInfo () const;

   /*!
    *  SE geographic corner orderd as:
    *  x == Longitude == SDDDMMSS.SSSS
    *  (neg sign (s) right justified, no leading zeros,
    *  plus sign (s) implied
    */
   double getSEGeoCornerX () const;

   /*!
    *  SE geographic corner orderd as:
    *  y == Latitude == SDDMMSS.SSSS
    *  (neg sign (s) right justified, no leading zeros,
    *  plus sign (s) implied
    */
   double getSEGeoCornerY () const;

   /*!
    *  1 == Autocorrelation resample simple bilinear
    *  2 == Manual profile GRIDEM simple bilinear
    *  3 == DLG/hypsography CTOG 8-direction bilinear
    *  4 == Interpolation from photogrammetic system contours DCASS 
    *  4-direction bilinear
    *  5 == DLG/hypsography LINETRACE, LT4X complex linear
    *  6 == DLG/hypsography CPS-3, ANUDEM, GRASS complex polynomial
    *  7 == Electronic imaging (non-photogrammetric), active or passive
    *  sensor systems
    */
   long getProcessCode () const;

   /*!
    *  Used for 30 minute DEMs - Identifies 1:100,000 scale sections
    *  Formatted as XNN, wher X is "S" == 7.5 min. & "F" == 15 min.
    *  NN is a two digit sequence number
    */
   string const& getSectionIndicator() const;

   /*!
    *  Mapping Center origin code. "EMC","WMC","MCMC","RMMC","FS" and "GPM2"
    */
   string const& getMappingCenterCode() const;

   /*!
    *  1 == DEM-1
    *  2 == DEM-2
    *  3 == DEM-3
    */
   long getLevelCode() const;

   /*!
    *  1 == Regular
    *  2 == Random (reserved for future use).
    */
   long getElevPattern() const;

   /*!
    *  Code defining ground planimetric reference system.
    *  0 == Geographic
    *  1 == UTM
    *  2 == State Plane
    */
   long getGroundRefSysCode() const;

   /*!
    * @return The projection type as a string.
    */
   ossimString getProjectionType() const;

   /*!
    *  Code defining zone in ground planimetric reference system.
    */
   long getGroundRefSysZone() const;

   /*!
    *  Code defining unit of measure for ground planimetric coordinates
    *  used throughout the file.
    *  0 == Radians
    *  1 == Feet
    *  2 == Meters
    *  3 == Arc Seconds
    */
   long getGroundRefSysUnits() const;

   /*!
    *  Code defining unit of measure for elevation coordinates used
    *  throughout the file.
    *  1 == Feet
    *  2 == Meters
    */
   long getElevationUnits() const;

   /*!
    *  Set n == 4
    */
   long getNumPolySides() const;

   /*!
    *  The coordinates of the quadrangle corners are ordered in a clockwise
    *  direction beginning with the southwest corner.
    */
   const ossimDemPointVector& getDEMCorners() const;

   /*!
    *  In Radians the counterclockwise angle from the primary axis of ground
    *  planimetric reference to the primary axis of the DEM local reference
    *  system. Set to 0 to align with the coordinate system
    */
   double getCounterclockAngle() const;

   /*!
    *  0 == unknown accuracy
    *  1 == accuracy info is given in logical record type C
    */
   long getElevAccuracyCode() const;

   /*!
    *  Minimum elevation in the DEM.
    */
   double getMinimumElev() const;

   /*!
    *  Maximum elevation in the DEM.
    */
   double getMaximumElev() const;

   double getSpatialResX() const;
   double getSpatialResY() const;
   double getSpatialResZ() const;

   /*!
    *  The number of rows and columns in profiles in the DEM. 
    *  NOTE: When profile rows == 1, profile columns is the number of profiles
    *  in the DEM.
    */
   long getProfileRows() const;
   long getProfileColumns() const;      

   /*!
    *  Present only if two or more primary intervals exist (level 2 DEM's only)
    */
   long getLargeContInt() const;

   /*!
    *  0 == NA
    *  1 == feet
    *  2 == meters
    *  (level two DEM's only)
    */
   long getMaxSourceUnits() const;

   /*!
    *  Smallest or only primary contour interval (level 2 DEM's only)
    */
   long getSmallContInt() const;

   /*!
    *  1 == feet
    *  2 == meters
    */
   long getMinSourceUnits() const;

   /*!
    *  YYY
    */
   long getSourceDate() const;

   /*!
    *  YYY
    */
   long getInspRevDate() const;

   /*!
    *  "I" indicates all processes of part3, Quality Control have been
    *  performed.
    */
   string const& getInspFlag() const;

   /*!
    *  0 == No Validation
    *  1 == RSME computed from test points, no quantative test or interactive
    *       editing/review
    *  2 == Batch process water body edit and RSME computed from test points
    *  3 == Review and edit, including water edit. No RSME computed from test
    *       points
    *  4 == Level 1 DEM's reviewed & edited. RSME computed from test points
    *  5 == Level 2 & 3 DEM's reviewed & edited. RSME computed from test points
    */
   long getDataValFlag() const;

   /*!
    *  0 == none
    *  1 == suspect areas
    *  2 == void areas
    *  3 == suspect & void areas
    */
   long getSuspectVoidFlag() const;

   /*!
    *  1 == local mean sea level
    *  2 == National Geodetic Vertical Datum 1929
    *  3 == North American Vertical Datum 1988
    */
   long getVertDatum() const;

   /*!
    *  1 == North Amercian Datum 1927 (NAD 27)
    *  2 == World Geoditic System 1972 (WGS 72)
    *  3 == WGS 84
    *  4 == NAD 83
    *  5 == Old Hawaii Datum
    *  6 == Puerto Rico Datum
    */
   long getHorizDatum() const;

   /**
    * @return The ossim datum code as a string.
    */
   ossimString getHorizDatumCode() const;

   /*!
    *  01-99 Primarily a DMA specfic field. (For USGS use set to 01)
    */
   long getDataEdition() const;

   /*!
    *  Contains the percentage of nodes in the file set to void
    */
   long getPerctVoid() const;

   /*!
    *  West edge match status flag
    */
   long getWestEdgeFlag() const;

   /*!
    *  North edge match status flag
    */
   long getNorthEdgeFlag() const;

   /*!
    *  East edge match staus flag
    */
   long getEastEdgeFlag() const;

   /*!
    *  South edge match status flag
    */
   long getSouthEdgeFlag() const;

   /*!
    *  Vertical datum shift - value is in the form of SFFF.DD Value is
    *  the average shift value for the four quadrangle corners obtained
    *  from program VERTCOM.
    */
   double getVertDatumShift() const;

   /*!
    * Populates keyword list with geometry info.
    * 
    * @return true on success, false on error.
    */
   bool getImageGeometry(ossimKeywordlist& kwl,
                         const char* prefix=NULL) const;
	 
private:

   string _quadName;
   string _processInfo;
   double _seGeoCornerX;
   double _seGeoCornerY;
   long _processCode;
   string _sectionIndicator;
   string _mapCenterCode;
   long _levelCode;
   long _elevPattern;
   long _groundRefSysCode;
   long _groundRefSysZone;
   long _groundRefSysUnits;
   long _elevUnits;
   long _numPolySides;
   ossimDemPointVector _demCorners;
   double _counterclockAngle;
   long _elevAccuracyCode;
   double _minElevation;
   double _maxElevation;
   double _spatialResX;
   double _spatialResY;
   double _spatialResZ;
   long _profileRows;
   long _profileColumns;
   long _largeContInt;
   long _maxSourceUnits;
   long _smallContInt;
   long _minSourceUnits;
   long _sourceDate;
   long _inspRevDate;
   string _inspFlag;
   long _valFlag;
   long _suspectVoidFlg;
   long _vertDatum;
   long _horizDatum;
   long _dataEdition;
   long _perctVoid;
   long _westEdgeFlag;
   long _northEdgeFlag;
   long _eastEdgeFlag;
   long _southEdgeFlag;
   double _vertDatumShift;
};

#endif  // #ifndef ossimDemHeader_HEADER
