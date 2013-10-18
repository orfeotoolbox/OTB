//********************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:      Garrett Potts
// 
// Description: Nitf support class
//
// Documentation reference:
// DIGEST Part 2 Annex D - Appendix 1, Table D1-3
// 
//********************************************************************
// $Id: ossimNitfGeoPositioningTag.h 19682 2011-05-31 14:21:20Z dburken $
//
#ifndef ossimNitfGeoPositioningTag_HEADER
#define ossimNitfGeoPositioningTag_HEADER
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfGeoPositioningTag : public ossimNitfRegisteredTag
{
public:
   ossimNitfGeoPositioningTag();
   virtual ~ossimNitfGeoPositioningTag();
   virtual std::string getRegisterTagName()const;
   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);
   virtual ossim_uint32 getSizeInBytes()const;
   virtual void clearFields();

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
   
protected:
   /**
    * FIELD: TYP
    *
    * required 3 byte field.
    * 
    * Coordinate System Type
    *
    * This field shall contain the type of coordinate system to which the Image
    * Segment refers.  Valid values are GEO for a geographic coordinate
    * system (longitude & latitude), MAP for a cartographic (grid)
    * coordinate system (easting & northing) and DIG for a geographic or
    * cartographic coordinate system registered through location grids or
    * registration points.
    * See clause D1.2.2 for details.
    * The default value is MAP.
    *
    *
    * Values: MAP, GEO or DIG 
    */
   char theType[4];

   /**
    * FIELD: UNI
    *
    * required 3 byte field.
    *
    * Coordinate Units This field shall contain the units of measure to which
    * the Image Segment refers.  Valid values are SEC (Decimal seconds of arc),
    * DEG (Decimal degrees) and M (Metres).  The value must be consistent with
    * the coordinate system type.  SEC and DEG are not allowed when the
    * coordinate system type is MAP.  M is not allowed when the coordinate system
    * type is GEO.  The PRJPS extension is expected when the value is M.
    * The default value is M.
    */
   char theCoordinateUnits[4];


   /**
    * FIELD: DAG
    * 
    * required 80 byte field.
    * 
    * Geodetic Datum Name
    *
    * This field shall contain the name of the geodetic datum to which the
    * Image Segment refers.
    * The default value is World Geodetic System 1984.
    * 
    */
   char theGeodeticDatumName[81];

   /**
    * FIELD: DCD
    *
    * required 4 byte field.
    * 
    * Geodetic Datum Code This field shall contain the code of the geodetic
    * datum to which the Image Segment refers. The default value is WGE.
    * 
    */
   char theGeodeticDatumCode[5];

   /**
    * FIELD: ELL
    *
    * required 80 byte field.
    *
    * Ellipsoid Name
    *
    * This field shall contain the name of the ellipsoid to which the
    * Image Segment refers. The default value is World Geodetic System 1984.
    * 
    */
   char theEllipsoidName[81];

   /**
    * FIELD: ELC
    *
    * required 3 byte field.
    *
    * Ellipsoid Code This field shall contain the code of the ellipsoid to
    * which the Image Segment refers.
    * The default value is WE.
    * 
    */
   char theEllipsoidCode[4];

   /**
    * FIELD: DVR
    *
    * required 80 byte field.
    *
    * Vertical Datum Reference This field shall contain the name of the vertical
    * datum reference to which the Image Segment refers, or BCS Spaces if no elevation value
    * appears in the Image Segment. The default name is Geodetic. 
    */
   char theVerticalDatumReference[81];

   /**
    * FIELD: VDCDVR
    *
    * required 4 byte field.
    * 
    * Code (Category) of Vertical Reference This field shall contain the code
    * (or category) of the vertical reference to which the Image Segment
    * refers, or BCS Spaces if no elevation value appears in the Image Segment.
    * The default code is GEOD.
    * 
    */
   char theVerticalReferenceCode[5];

   /**
    * FIELD: SDA
    *
    * required 80 byte field.
    * 
    * Sounding Datum Name This field shall contain the name of the sounding datum
    * to which the Image Segment refers, or BCS Spaces i
    * f no sounding appears in the Image Segment.
    * The default value is Mean Sea.
    */
   char theSoundingDatumName[81];

   /**
    * FIELD: VDCSDA
    *
    * required 4 byte field.
    * 
    * Code for  Sounding Datum This field shall contain the code of the
    * sounding datum to which the Image Segment refers, or BCS Spaces
    * if no sounding appears in the Image Segment.
    * The default valid code is MSL. 4BCS-A See Part 3-6 <R> 
    */
   char theSoundingDatumCode[5];

   /**
    * FIELD ZOR
    *
    * required 15 byte field
    *
    * Z values False Origin
    *
    * This field shall contain the elevation and depth false origin for Z values
    * to which the Image Segment refers. The default value is 000000000000000,
    * which implies that there is no projection false Z origin. 15 BCS-N positive integer
    * 
    */
   char theZFalseOrigin[16];

   /**
    * FIELD: GRD
    *
    * required 3 byte field.
    * 
    * Grid Code This field shall contain the identification code of the grid system
    * to which the Image Segment refers, or BCS Spaces. The default value is
    * BCS Spaces. 3BCS-A See Part 3-6 <R> 
    */
   char theGridCode[4];

   /**
    * FIELD: GRN
    *
    * required 80 byte field.
    *
    * Grid Description If the GRD Field value is not BCS Spaces, this field can
    * contain a text description of the grid system.
    * The default value is BCS Spaces. 80 BCS-A <R> 
    */
   char theGridDescription[81];

   /**
    * FIELD: ZNA
    *
    * required 4 byte field.
    *
    * Grid Zone number This field shall contain the zone number when the GRD
    * Field contains a significant grid code and the corresponding grid system
    * comprises more than one zone. Defaulted to 0000 otherwise.
    * 4 BCS-N integer See Part 3-6 R 
    */
   char theGridZoneNumber[5];

   
TYPE_DATA   
};

#endif
