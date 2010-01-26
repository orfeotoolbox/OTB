//*******************************************************************
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
// $Id: ossimDemHeader.h 15309 2009-09-01 15:47:15Z dburken $

#ifndef ossimDemHeader_HEADER
#define ossimDemHeader_HEADER

#include <string>
#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimDemPoint.h>

class ossimFilename;
class ossimString;
class ossimKeywordlist;

class OSSIM_DLL ossimDemHeader
{
public:
   
   ossimDemHeader();
   
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& s,
                                             const ossimDemHeader& header);
   friend OSSIM_DLL std::istream& operator>>(std::istream& s,
                                             ossimDemHeader& header);

   /**
    * @brief open method that takes a file.
    *
    * @param file File name to open.
    *
    * @return true on success false on error.
    */
   bool open(const ossimFilename& file);

   /**
    * @brief open method that takes a stream.
    *
    * @param is The input stream to read from.
    *
    * @return stream
    */
   std::istream& open(std::istream& is);

   /**
    * Print method.
    *
    * @param out Stream to print to.
    * 
    * @return std::ostream&
    */   
   std::ostream& print(std::ostream& out) const;
   
   // Accessors

   /*! Quadrangle name. */
   std::string const& getQuadName() const;

   /*! Free format desciptor field containing process information. */
   std::string const& getProcessInfo () const;

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
   ossim_int32 getProcessCode () const;

   /*!
    *  Used for 30 minute DEMs - Identifies 1:100,000 scale sections
    *  Formatted as XNN, wher X is "S" == 7.5 min. & "F" == 15 min.
    *  NN is a two digit sequence number
    */
   std::string const& getSectionIndicator() const;

   /*!
    *  Mapping Center origin code. "EMC","WMC","MCMC","RMMC","FS" and "GPM2"
    */
   std::string const& getMappingCenterCode() const;

   /*!
    *  1 == DEM-1
    *  2 == DEM-2
    *  3 == DEM-3
    */
   ossim_int32 getLevelCode() const;

   /*!
    *  1 == Regular
    *  2 == Random (reserved for future use).
    */
   ossim_int32 getElevPattern() const;

   /*!
    *  Code defining ground planimetric reference system.
    *  0 == Geographic
    *  1 == UTM
    *  2 == State Plane
    */
   ossim_int32 getGroundRefSysCode() const;

   /*!
    * @return The projection type as a string.
    */
   ossimString getProjectionType() const;

   /*!
    *  Code defining zone in ground planimetric reference system.
    */
   ossim_int32 getGroundRefSysZone() const;

   /*!
    *  Code defining unit of measure for ground planimetric coordinates
    *  used throughout the file.
    *  0 == Radians
    *  1 == Feet
    *  2 == Meters
    *  3 == Arc Seconds
    */
   ossim_int32 getGroundRefSysUnits() const;

   /*!
    *  Code defining unit of measure for elevation coordinates used
    *  throughout the file.
    *  1 == Feet
    *  2 == Meters
    */
   ossim_int32 getElevationUnits() const;

   /*!
    *  Set n == 4
    */
   ossim_int32 getNumPolySides() const;

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
   ossim_int32 getElevAccuracyCode() const;

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
   ossim_int32 getProfileRows() const;
   ossim_int32 getProfileColumns() const;      

   /*!
    *  Present only if two or more primary intervals exist (level 2 DEM's only)
    */
   ossim_int32 getLargeContInt() const;

   /*!
    *  0 == NA
    *  1 == feet
    *  2 == meters
    *  (level two DEM's only)
    */
   ossim_int32 getMaxSourceUnits() const;

   /*!
    *  Smallest or only primary contour interval (level 2 DEM's only)
    */
   ossim_int32 getSmallContInt() const;

   /*!
    *  1 == feet
    *  2 == meters
    */
   ossim_int32 getMinSourceUnits() const;

   /*!
    *  YYY
    */
   ossim_int32 getSourceDate() const;

   /*!
    *  YYY
    */
   ossim_int32 getInspRevDate() const;

   /*!
    *  "I" indicates all processes of part3, Quality Control have been
    *  performed.
    */
   std::string const& getInspFlag() const;

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
   ossim_int32 getDataValFlag() const;

   /*!
    *  0 == none
    *  1 == suspect areas
    *  2 == void areas
    *  3 == suspect & void areas
    */
   ossim_int32 getSuspectVoidFlag() const;

   /*!
    *  1 == local mean sea level
    *  2 == National Geodetic Vertical Datum 1929
    *  3 == North American Vertical Datum 1988
    */
   ossim_int32 getVertDatum() const;

   /*!
    *  1 == North Amercian Datum 1927 (NAD 27)
    *  2 == World Geoditic System 1972 (WGS 72)
    *  3 == WGS 84
    *  4 == NAD 83
    *  5 == Old Hawaii Datum
    *  6 == Puerto Rico Datum
    */
   ossim_int32 getHorizDatum() const;

   /**
    * @return The ossim datum code as a string.
    */
   ossimString getHorizDatumCode() const;

   /*!
    *  01-99 Primarily a DMA specfic field. (For USGS use set to 01)
    */
   ossim_int32 getDataEdition() const;

   /*!
    *  Contains the percentage of nodes in the file set to void
    */
   ossim_int32 getPerctVoid() const;

   /*!
    *  West edge match status flag
    */
   ossim_int32 getWestEdgeFlag() const;

   /*!
    *  North edge match status flag
    */
   ossim_int32 getNorthEdgeFlag() const;

   /*!
    *  East edge match staus flag
    */
   ossim_int32 getEastEdgeFlag() const;

   /*!
    *  South edge match status flag
    */
   ossim_int32 getSouthEdgeFlag() const;

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

   std::string _quadName;
   std::string _processInfo;
   double _seGeoCornerX;
   double _seGeoCornerY;
   ossim_int32 _processCode;
   std::string _sectionIndicator;
   std::string _mapCenterCode;
   ossim_int32 _levelCode;
   ossim_int32 _elevPattern;
   ossim_int32 _groundRefSysCode;
   ossim_int32 _groundRefSysZone;
   ossim_int32 _groundRefSysUnits;
   ossim_int32 _elevUnits;
   ossim_int32 _numPolySides;
   ossimDemPointVector _demCorners;
   double _counterclockAngle;
   ossim_int32 _elevAccuracyCode;
   double _minElevation;
   double _maxElevation;
   double _spatialResX;
   double _spatialResY;
   double _spatialResZ;
   ossim_int32 _profileRows;
   ossim_int32 _profileColumns;
   ossim_int32 _largeContInt;
   ossim_int32 _maxSourceUnits;
   ossim_int32 _smallContInt;
   ossim_int32 _minSourceUnits;
   ossim_int32 _sourceDate;
   ossim_int32 _inspRevDate;
   std::string _inspFlag;
   ossim_int32 _valFlag;
   ossim_int32 _suspectVoidFlg;
   ossim_int32 _vertDatum;
   ossim_int32 _horizDatum;
   ossim_int32 _dataEdition;
   ossim_int32 _perctVoid;
   ossim_int32 _westEdgeFlag;
   ossim_int32 _northEdgeFlag;
   ossim_int32 _eastEdgeFlag;
   ossim_int32 _southEdgeFlag;
   double _vertDatumShift;
};

#endif  // #ifndef ossimDemHeader_HEADER
