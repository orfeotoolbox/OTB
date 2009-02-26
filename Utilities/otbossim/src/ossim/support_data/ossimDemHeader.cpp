//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
//         Orginally written by Jamie Moyers (jmoyers@geeks.com)
//         Adapted from the package KDEM.
// Description: This class parses a DEM header.
//
//********************************************************************
// $Id: ossimDemHeader.cpp 13694 2008-10-08 20:16:02Z dburken $

#include <iostream>
#include <iomanip>
#include <ossim/support_data/ossimDemHeader.h>
#include <ossim/support_data/ossimDemUtil.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordNames.h>

static const char* PROCESS_CODE[]
= { "Autocorrelation resample simple bilinear",
    "Manual profile GRIDEM simple bilinear",
    "DLG/hypsography CTOG 8-direction bilinear",
    "Interpolation from photogrammetic system contours DCASS",
    "direction bilinear",
    "DLG/hypsography LINETRACE, LT4X complex linear",
    "DLG/hypsography CPS-3, ANUDEM, GRASS complex polynomial",
    "Electronic imaging (non-photogrammetric), active or passive",
    "sensor systems" };

static const int MAX_PROCESS_CODE_INDEX = 8;

static const char* GROUND_REF_SYSTEM[]
= { "Geographic",
    "UTM",
    "State Plane" };

static const int MAX_GROUND_REF_SYSTEM_INDEX = 2;

static const char* GROUND_REF_SYSTEM_UNITS[]
= { "Radians",
    "Feet",
    "Meters",
    "Arc Seconds" };

static const int MAX_GROUND_REF_SYSTEM_UNITS_INDEX = 3;


static const char* VERTICAL_DATUM[]
= { "local mean sea level",
    "National Geodetic Vertical Datum 1929",
    "North American Vertical Datum 1988" };

static const int MAX_VERTICAL_DATUM_INDEX = 2;

static const char* HORIZONTAL_DATUM[]
= { "North Amercian Datum 1927 (NAD 27)",
    "World Geoditic System 1972 (WGS 72)",
    "WGS 84",
    "NAD 83",
    "Old Hawaii Datum",
    "Puerto Rico Datum" };

static const int MAX_HORIZONTAL_DATUM_INDEX = 5;


ossimDemHeader::ossimDemHeader()
   : _seGeoCornerX(0.0),
     _seGeoCornerY(0.0),
     _processCode(0),
     _levelCode(0),
     _elevPattern(0),
     _groundRefSysCode(0),
     _groundRefSysZone(0),
     _groundRefSysUnits(0),
     _elevUnits(0),
     _numPolySides(0),
     _counterclockAngle(0.0),
     _elevAccuracyCode(0),
     _minElevation(0),
     _maxElevation(0),
     _spatialResX(0.0),
     _spatialResY(0.0),
     _spatialResZ(0.0),
     _profileRows(0),
     _profileColumns(0),
     _largeContInt(0),
     _maxSourceUnits(0),
     _smallContInt(0),
     _minSourceUnits(0),
     _sourceDate(0),
     _inspRevDate(0),
     _valFlag(0),
     _suspectVoidFlg(0),
     _vertDatum(0),
     _horizDatum(0),
     _dataEdition(0),
     _perctVoid(0),
     _westEdgeFlag(0),
     _northEdgeFlag(0),
     _eastEdgeFlag(0),
     _southEdgeFlag(0),
     _vertDatumShift(0.0)
{
}
   
std::string const&
ossimDemHeader::getQuadName() const
{
   return _quadName;
}

std::string const&
ossimDemHeader::getProcessInfo() const
{
   return _processInfo;
}

double
ossimDemHeader::getSEGeoCornerX() const
{
   return _seGeoCornerX;
}

double
ossimDemHeader::getSEGeoCornerY() const
{
   return _seGeoCornerY;
}

ossim_int32
ossimDemHeader::getProcessCode() const
{
   return _processCode;
}

std::string const&
ossimDemHeader::getSectionIndicator() const
{
   return _sectionIndicator;
}

std::string const&
ossimDemHeader::getMappingCenterCode() const
{
   return _mapCenterCode;
}

ossim_int32
ossimDemHeader::getLevelCode() const
{
   return _levelCode;
}
         
ossim_int32
ossimDemHeader::getElevPattern() const
{
   return _elevPattern;
}

ossim_int32
ossimDemHeader::getGroundRefSysCode() const
{
   return _groundRefSysCode;
}
    
ossim_int32
ossimDemHeader::getGroundRefSysZone() const
{
   return _groundRefSysZone;
}

ossim_int32
ossimDemHeader::getGroundRefSysUnits() const
{
   return _groundRefSysUnits;
}

ossim_int32
ossimDemHeader::getElevationUnits() const
{
   return _elevUnits;
}

ossim_int32
ossimDemHeader::getNumPolySides() const
{
   return _numPolySides;
}

ossimDemPointVector const&
ossimDemHeader::getDEMCorners() const
{
   return _demCorners;
}

double
ossimDemHeader::getCounterclockAngle() const
{
   return _counterclockAngle;
}

ossim_int32
ossimDemHeader::getElevAccuracyCode() const
{
   return _elevAccuracyCode;
}

double
ossimDemHeader::getMinimumElev() const
{
   return _minElevation;
}

double
ossimDemHeader::getMaximumElev() const
{
   return _maxElevation;
}

double
ossimDemHeader::getSpatialResX() const
{
   return _spatialResX;
}

double
ossimDemHeader::getSpatialResY() const
{
   return _spatialResY;
}

double
ossimDemHeader::getSpatialResZ() const
{
   return _spatialResZ;
}

ossim_int32
ossimDemHeader::getProfileRows() const
{
   return _profileRows;
}

ossim_int32
ossimDemHeader::getProfileColumns() const
{
   return _profileColumns;
}

ossim_int32
ossimDemHeader::getLargeContInt() const
{
   return _largeContInt;
}

ossim_int32
ossimDemHeader::getMaxSourceUnits() const
{
   return _maxSourceUnits;
}

ossim_int32
ossimDemHeader::getSmallContInt() const
{
   return _smallContInt;
}

ossim_int32
ossimDemHeader::getMinSourceUnits() const
{
   return _minSourceUnits;
}

ossim_int32
ossimDemHeader::getSourceDate() const
{
   return _sourceDate;
}

ossim_int32
ossimDemHeader::getInspRevDate() const
{
   return _inspRevDate;
}

std::string const&
ossimDemHeader::getInspFlag() const
{
   return _inspFlag;
}

ossim_int32
ossimDemHeader::getDataValFlag() const
{
   return _valFlag;
}

ossim_int32
ossimDemHeader::getSuspectVoidFlag() const
{
   return _suspectVoidFlg;
}

ossim_int32
ossimDemHeader::getVertDatum () const
{
   return _vertDatum;
}

ossim_int32
ossimDemHeader::getHorizDatum() const
{
   return _horizDatum;
}

ossim_int32
ossimDemHeader::getDataEdition() const
{
   return _dataEdition;
}

ossim_int32
ossimDemHeader::getPerctVoid() const
{
   return _perctVoid;
}

ossim_int32
ossimDemHeader::getWestEdgeFlag() const
{
   return _westEdgeFlag;
}

ossim_int32
ossimDemHeader::getNorthEdgeFlag() const
{
   return _northEdgeFlag;
}

ossim_int32
ossimDemHeader::getEastEdgeFlag() const
{
   return _eastEdgeFlag;
}

ossim_int32
ossimDemHeader::getSouthEdgeFlag() const
{
   return _southEdgeFlag;
}

double
ossimDemHeader::getVertDatumShift() const
{
   return _vertDatumShift;
}

std::ostream&
operator<<(std::ostream& s, const ossimDemHeader& header)
{
   return header.print(s);
}

std::ostream& ossimDemHeader::print(std::ostream& s) const
{
   // Note:  This is only a partial print for now...
   
   s << setiosflags(std::ios::fixed) << std::setprecision(10)
     << "USGS DEM Header Info:"
     << "\nQuadrangle name:                " << getQuadName().c_str()
     << "\nProcess info:                   " << getProcessInfo().c_str()
     << "\nSE Geo Corner X(SDDMMSS.SSSS):  " << getSEGeoCornerX()
     << "\nSE Geo Corner Y(SDDMMSS.SSSS):  " << getSEGeoCornerY()
     << "\nProcess code:                   ";

   ossim_int32 tmpl = getProcessCode() - 1;
   if ( tmpl >= 0 && tmpl <= MAX_PROCESS_CODE_INDEX)
   {
      s << PROCESS_CODE[tmpl];
   }
   else
   {
      s << "Unknown";
   }

   s << "\nSection Indicator:       " << getSectionIndicator().c_str()
     << "\nMapping Center Code:     " << getMappingCenterCode().c_str()
     << "\nLevel Code:              " << getLevelCode()
     << "\nElev Pattern:            " << getElevPattern()
     << "\nGround Ref Sys:          ";

   tmpl = getGroundRefSysCode();
   if ( tmpl >= 0 && tmpl <= MAX_GROUND_REF_SYSTEM_INDEX)
   {
      s << GROUND_REF_SYSTEM[tmpl];
   }
   else
   {
      s << "Unknown";
   }

   s << "\nGround Ref Sys Zone:     " << getGroundRefSysZone()

     << "\nGround Ref Sys Units:    ";
   tmpl = getGroundRefSysUnits();
   if ( tmpl >= 0 && tmpl <= MAX_GROUND_REF_SYSTEM_UNITS_INDEX)
   {
      s << GROUND_REF_SYSTEM_UNITS[tmpl];
   }
   else
   {
      s << "Unknown";
   }
         
   s << "\nElevation Units:         ";
   tmpl = getElevationUnits();
   if ( tmpl >= 0 && tmpl <= MAX_GROUND_REF_SYSTEM_UNITS_INDEX)
   {
      s << GROUND_REF_SYSTEM_UNITS[tmpl];
   }
   else
   {
      s << "Unknown";
   }

   s << "\nNumber Poly Sides:       " << getNumPolySides()
     << "\nCounterclock Angle:      " << getCounterclockAngle()
     << "\nElev Accuracy Code:      " << getElevAccuracyCode()
     << "\nMinimum Elevation:       " << getMinimumElev()
     << "\nMaximum Elevation:       " << getMaximumElev()
     << "\nSpatial Res X:           " << getSpatialResX()
     << "\nSpatial Res Y:           " << getSpatialResY()
     << "\nSpatial Res Z:           " << getSpatialResZ()
     << "\nProfile Rows:            " << getProfileRows()
     << "\nProfile Columns:         " << getProfileColumns()
     << "\nVertical datum:          ";

   tmpl = getVertDatum() - 1;
   if ( tmpl >= 0 && tmpl <= MAX_VERTICAL_DATUM_INDEX)
   {
      s << VERTICAL_DATUM[tmpl];
   }
   else
   {
      s << "Unknown";
   }

   s << "\nVertical datum shift:    " << getVertDatumShift()
     << "\nHorizontal datum:        ";
   tmpl = getHorizDatum() - 1;
   if ( tmpl >= 0 && tmpl <= MAX_HORIZONTAL_DATUM_INDEX)
   {
      s << HORIZONTAL_DATUM[tmpl];
   }
   else
   {
      s << "Unknown";
   }

   const ossimDemPointVector CORNERS = getDEMCorners();
   for (unsigned int i=0; i < CORNERS.size(); ++i)
   {
      s << "Corner[" << i << "].x:  " << CORNERS[i].getX()
        << "\nCorner[" << i << "].y:  " << CORNERS[i].getY()
        << std::endl;
         
   }

   s << std::endl;
   
   return s;
}

std::istream&
operator>>(std::istream& s, ossimDemHeader& header)
{
   char bufstr[1024];
   char temp[1024];
   ossim_int32 i;

   ossimDemUtil::getRecord(s,bufstr);

   strncpy(temp, bufstr, 40);
   temp[40] = '\0';
   header._quadName = temp;

   strncpy(temp,bufstr+40,40);
   temp[40] = '\0';
   header._processInfo = temp;
   
   ossimDemUtil::getDouble(bufstr, 109, 13, header._seGeoCornerX);
   ossimDemUtil::getDouble(bufstr, 122, 13, header._seGeoCornerY);
   header._processCode = ossimDemUtil::getLong(bufstr, 135, 1);

   strncpy(temp,bufstr+137,3);
   temp[3] = '\0';
   header._sectionIndicator = temp;

   strncpy(temp,bufstr+140,4);
   temp[4] = '\0';
   header._mapCenterCode = temp;
    
   header._levelCode = ossimDemUtil::getLong(bufstr, 144, 6);
   header._elevPattern = ossimDemUtil::getLong(bufstr, 150, 6);
   header._groundRefSysCode = ossimDemUtil::getLong(bufstr, 156, 6);
   header._groundRefSysZone = ossimDemUtil::getLong(bufstr, 162, 6);
   header._groundRefSysUnits = ossimDemUtil::getLong(bufstr, 528, 6);
   header._elevUnits = ossimDemUtil::getLong(bufstr, 534, 6);
   header._numPolySides = ossimDemUtil::getLong(bufstr, 540, 6);

   for (i = 0; i < 4; i++)
   {
      double x,y;
      ossim_int32 pos = 546 + (i * 48);
      ossimDemUtil::getDouble(bufstr, pos, 24, x);
      ossimDemUtil::getDouble(bufstr, pos + 24, 24, y);
      header._demCorners.push_back(ossimDemPoint(x,y));
   }

   ossimDemUtil::getDouble(bufstr, 738, 24, header._minElevation);
   ossimDemUtil::getDouble(bufstr, 762, 24, header._maxElevation);
   ossimDemUtil::getDouble(bufstr, 786, 24, header._counterclockAngle );
   header._elevAccuracyCode = ossimDemUtil::getLong(bufstr, 810, 6);
   ossimDemUtil::getDouble(bufstr, 816, 12, header._spatialResX);
   ossimDemUtil::getDouble(bufstr, 828, 12, header._spatialResY);
   ossimDemUtil::getDouble(bufstr, 840, 12, header._spatialResZ);
   header._profileRows = ossimDemUtil::getLong(bufstr, 852, 6);
   header._profileColumns = ossimDemUtil::getLong(bufstr, 858, 6);
   header._largeContInt = ossimDemUtil::getLong(bufstr, 864, 5);
   header._maxSourceUnits = ossimDemUtil::getLong(bufstr, 869, 1);
   header._smallContInt = ossimDemUtil::getLong(bufstr, 870, 5);
   header._minSourceUnits = ossimDemUtil::getLong(bufstr, 875, 1);
   header._sourceDate = ossimDemUtil::getLong(bufstr, 876, 4);
   header._inspRevDate = ossimDemUtil::getLong(bufstr, 880, 4);
   
   strncpy(temp, bufstr+884,1);
   temp[1]='\0';
   header._inspFlag = temp;

   header._valFlag = ossimDemUtil::getLong(bufstr, 885, 1);
   header._suspectVoidFlg = ossimDemUtil::getLong(bufstr, 886, 2);
   header._vertDatum = ossimDemUtil::getLong(bufstr, 888, 2);
   header._horizDatum = ossimDemUtil::getLong(bufstr, 890, 2);
   if (header._horizDatum == 0)
      header._horizDatum = 1;   // Default to NAD27

   header._dataEdition = ossimDemUtil::getLong(bufstr, 892, 4);
   header._perctVoid = ossimDemUtil::getLong(bufstr, 896, 4);
   header._westEdgeFlag = ossimDemUtil::getLong(bufstr, 900, 2);
   header._northEdgeFlag = ossimDemUtil::getLong(bufstr, 902, 2);
   header._eastEdgeFlag = ossimDemUtil::getLong(bufstr, 904, 2);
   header._southEdgeFlag = ossimDemUtil::getLong(bufstr, 906, 2);
   ossimDemUtil::getDouble(bufstr, 908, 7, header._vertDatumShift);

   return s;  
}

bool ossimDemHeader::getImageGeometry(ossimKeywordlist& kwl,
                                      const char* prefix) const
{
   ossimString type = getProjectionType();
   ossimString datum = getHorizDatumCode();
   if ( (type == "unknown") || (datum == "unknown") )
   {
      return false;
   }

   if (_demCorners.size() != 4)
   {
      return false;
   }

   double tieX   = _demCorners[1].getX();
   double tieY   = _demCorners[1].getY();
   double scaleX = getSpatialResX();
   double scaleY = getSpatialResX();
   
   // Add these for all projections.
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           type);
   
   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           datum);

   if (getGroundRefSysCode() == 0) // Geographic.
   {
      // ESH 10/2008 -- The Dem ground units can be either radians or 
      // arc seconds, so we have to convert parameters in these units 
      // to degrees which is what OSSIM is assuming.
      bool bIsArcSecs = (getGroundRefSysUnits() == 3) ? true : false;
      bool bIsRadians = (getGroundRefSysUnits() == 0) ? true : false;

      double convertFactor = 1.0;
      if ( bIsArcSecs == true )
      {
         convertFactor = 1.0 / 3600;
      }
      else if ( bIsRadians == true )
      {
         convertFactor = 180.0 / M_PI;
      }

      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LON_KW,
              (tieX * convertFactor) );
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LAT_KW,
              (tieY * convertFactor) );
      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
              (scaleX * convertFactor) );
      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
              (scaleY * convertFactor) );
   }
   else if (getGroundRefSysCode() == 1) // UTM
   {
      // Get the zone.
      kwl.add(prefix,
              ossimKeywordNames::ZONE_KW,
              abs(getGroundRefSysZone()));

      // Set the hemisphere.
      kwl.add(prefix,
              ossimKeywordNames::HEMISPHERE_KW,
              (getGroundRefSysZone()<0?"S":"N"));

      // Set the tie.
      if (getGroundRefSysUnits() == 1) // Feet...
      {
         tieX = ossim::usft2mtrs(tieX);
         tieY = ossim::usft2mtrs(tieY);
         scaleX = ossim::usft2mtrs(scaleX);
         scaleY = ossim::usft2mtrs(scaleY);
         
      }
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_EASTING_KW,
              tieX);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_NORTHING_KW,
              tieY);
      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_X_KW,
              scaleX);
      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
              scaleY);
   }
   else if (getGroundRefSysCode() == 2) // State Plane
   {
      return false; // Need to code...
   }

   
   return true;
}

ossimString ossimDemHeader::getHorizDatumCode() const
{
   ossimString code;
   switch(getHorizDatum())
   {
      case 1:
         code = "NAS-C";
         break;
      case 2:
         code = "WGD";
         break;
      case 3:
         code = "WGE";
        break;         
      case 4:
         code = "NAR-C";
        break;         
      case 5:
         code = "OHA-M";
        break;         
      case 6:
         code = "PUR";
         break;
      default:
         code = "unknown";
        break;         
   }
   return code;
}
         
ossimString ossimDemHeader::getProjectionType() const
{
   ossimString type;
   switch(getGroundRefSysCode())
   {
      case 0:
         type = "ossimEquDistCylProjection";
         break;
      case 1:
         type = "ossimUtmProjection";
         break;
      case 2:
      default:
         type = "unknown";
         break;
   }
   return type;
}
