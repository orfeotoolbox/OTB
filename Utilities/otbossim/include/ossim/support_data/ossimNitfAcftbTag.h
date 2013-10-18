//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ACFTB - Aircraft Infomation Extension Format tag class
// declaration.
//
// See document STDI-0002 (version 3), Appendix E, Table E-6 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#ifndef ossimNitfAcftbTag_HEADER
#define ossimNitfAcftbTag_HEADER

#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfAcftbTag : public ossimNitfRegisteredTag
{
public:

   enum 
   {
      CEL_SIZE               = 207,
      AC_MSN_ID_SIZE         = 20,
      AC_TAIL_NO_SIZE        = 10,
      AC_TO_SIZE             = 12,
      SENSOR_ID_TYPE_SIZE    = 4,
      SENSOR_ID_SIZE         = 6,
      SCENE_SOURCE_SIZE      = 1,
      SCNUM_SIZE             = 6,
      PDATE_SIZE             = 8,
      IMHOSTNO_SIZE          = 6,
      IMREQID_SIZE           = 5,
      MPLAN_SIZE             = 3,
      ENTLOC_SIZE            = 25,
      LOC_ACCY_SIZE          = 6,
      ENTELV_SIZE            = 6,
      ELV_UNIT_SIZE          = 1,
      EXITLOC_SIZE           = 25,
      EXITELV_SIZE           = 6,
      TMAP_SIZE              = 7,
      ROW_SPACING_SIZE       = 7,
      ROW_SPACING_UNITS_SIZE = 1,
      COL_SPACING_SIZE       = 7,
      COL_SPACING_UINTS_SIZE = 1,
      FOCAL_LENGTH_SIZE      = 6,
      SENSERIAL_SIZE         = 6,
      ABSWVER_SIZE           = 7,
      CAL_DATE_SIZE          = 8,
      PATCH_TOT_SIZE         = 4,
      MTI_TOT_SIZE           = 3
   };
   
   /** @brief default constructor */
   ossimNitfAcftbTag();

   /** @brief destructor */
   virtual ~ossimNitfAcftbTag();

   /**
    * @brief Method to return tag name.
    * @return "ACFTB" as an ossimString.
    */
   virtual std::string getRegisterTagName()const;

   /** @brief Method to parse data from stream. */
   virtual void parseStream(std::istream& in);

   /** @brief Method to write data to stream. */
   virtual void writeStream(std::ostream& out);

   /** @return Byte size of this tag. */
   virtual ossim_uint32 getSizeInBytes()const;

   /** @brief Method to clear all fields including null terminating. */
   virtual void clearFields();

   ossimString getFocalLength()const;
   ossimString getColumnSpacing()const;
   ossimString getColumnSpacingUnits()const;
   ossimString getRowSpacing()const;
   ossimString getRowSpacingUnits()const;
   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
   
protected:

   /** Type R = Required Type <R> = BCS spaces allowed for entire field */
   
   /**
    * FIELD: AC_MSN_ID
    *
    * TYPE: R
    * 
    * 20 byte field
    *
    * Aircraft Mission Identification
    */
   char theAcMsnId[AC_MSN_ID_SIZE+1];

   /**
    * FIELD: AC_TAIL_NO
    *
    * TYPE: <R>
    * 
    * 10 byte field
    *
    * Aircraft Tail Number
    */
   char theAcTailNo[AC_TAIL_NO_SIZE+1];
   
   /**
    * FIELD: AC_TO
    *
    * TYPE: <R>
    * 
    * 12 byte field
    *
    * Aircraft Take-off, CCYYMMDDhhmm
    */
   char theAcTo[AC_TO_SIZE+1];

   /**
    * FIELD: SENSOR_ID_TYPE
    *
    * TYPE: R
    * 
    * 4 byte field
    *
    * Sensor ID Type
    */
   char theSensorIdType[SENSOR_ID_TYPE_SIZE+1];
   
   /**
    * FIELD: SENSOR_ID
    *
    * TYPE: R
    * 
    * 6 byte field
    *
    * Sensor ID 
    */
   char theSensorId[SENSOR_ID_SIZE+1];

   /**
    * FIELD: SCENE_SOURCE
    *
    * TYPE: <R>
    * 
    * 1 byte field
    *
    * Scene Source, 0 to 9
    */
   char theSceneSource[SCENE_SOURCE_SIZE+1];
   
   /**
    * FIELD: SCNUM
    *
    * TYPE: R
    * 
    * 6 byte field
    *
    * Scene Number, 000000 to 999999
    */
   char theScNum[SCNUM_SIZE+1];

   /**
    * FIELD: PDATE
    *
    * TYPE: R
    * 
    * 8 byte field
    *
    * Processing Date, CCYYMMDD
    */
   char thePDate[PDATE_SIZE+1];
   
   /**
    * FIELD: IMHOSTNO
    *
    * TYPE: R
    * 
    * 6 byte field
    *
    * Immediate Scene Host
    */
   char theImHostNo[IMHOSTNO_SIZE+1];

   /**
    * FIELD: IMREQID
    *
    * TYPE: R
    * 
    * 5 byte field
    *
    * Immediate Scene Request ID, 00000, 00001 to 99999
    */
   char theImReqID[IMREQID_SIZE+1];
   
   /**
    * FIELD: MPLAN
    *
    * TYPE: R
    * 
    * 3 byte field
    *
    * Mission Plan Mode, 001 to 999
    */
   char theMPlan[MPLAN_SIZE+1];

   /**
    * FIELD: ENTLOC
    *
    * TYPE: <R>
    * 
    * 25 byte field
    *
    * Entry Location
    */
   char theEntLoc[ENTLOC_SIZE+1];

   /**
    * FIELD: LOC_ACCY
    *
    * TYPE: R
    * 
    * 6 byte field
    *
    * Location Accuracy, feet, 000.01 to 999.99, 000000, 000.00
    */
   char theLocAccy[LOC_ACCY_SIZE+1];
   
   /**
    * FIELD: ENTELV
    *
    * TYPE: <R>
    * 
    * 6 byte field
    *
    * Entry Elevation, feet or meters, -01000 to +30000
    */
   char theEntelv[ENTELV_SIZE+1];

   /**
    * FIELD: ELV_UNIT
    *
    * TYPE: <R>
    * 
    * 1 byte field
    *
    * Unit of Elevation, f or m
    */
   char theElvUnit[ELV_UNIT_SIZE+1];
   
   /**
    * FIELD: EXITLOC
    *
    * TYPE: <R>
    * 
    * 25 byte field
    *
    * Exit Location
    */
   char theExitLoc[EXITLOC_SIZE+1];

   /**
    * FIELD: EXITELV
    *
    * TYPE: <R>
    * 
    * 6 byte field
    *
    * Exit Elevation
    */
   char theExitElev[EXITELV_SIZE+1];
   
   /**
    * FIELD: TMAP
    *
    * TYPE: <R>
    * 
    * 7 byte field
    *
    * True Map Angle, degrees, 000.000 to 180.000
    */
   char theTMap[TMAP_SIZE+1];

   /**
    * FIELD: ROW_SPACING
    *
    * TYPE: R
    * 
    * 7 byte field
    *
    * Row Spacing, feet, meters, rads. 
    */
   char theRowSpacing[ROW_SPACING_SIZE+1];

   /**
    * FIELD: ROW_SPACING_UNITS
    *
    * TYPE: R
    * 
    * 1 byte field
    *
    * Unit of Row Spacing, f, m, r or u
    */
   char theRowSpacingUnits[ROW_SPACING_UNITS_SIZE+1];

   /**
    * FIELD: COL_SPACING
    *
    * TYPE: R
    * 
    * 7 byte field
    *
    * Column Spacing
    */
   char theColSpacing[COL_SPACING_SIZE+1];

   /**
    * FIELD: COL_SPACING_UINTS
    *
    * TYPE: R
    * 
    * 1 byte field
    *
    * Unit of Column Spacing
    */
   char theColSpacingUnits[COL_SPACING_UINTS_SIZE+1];
   
   /**
    * FIELD: FOCAL_LENGTH
    *
    * TYPE: R
    * 
    * 6 byte field
    *
    * Sensor Focal Length, cm, 000.01 to 899.99, 999.99
    */
   char theFocalLength[FOCAL_LENGTH_SIZE+1];

   /**
    * FIELD: SENSERIAL
    *
    * TYPE: <R>
    * 
    * 6 byte field
    *
    * Sensor vendor's serial number, 000001 to 999999
    */
   char theSenserial[SENSERIAL_SIZE+1];
   
   /**
    * FIELD: ABSWVER
    *
    * TYPE: <R>
    * 
    * 7byte field
    *
    * Airborne Software Version
    */
   char theAbSwVer[ABSWVER_SIZE+1];

   /**
    * FIELD: CAL_DATE
    *
    * TYPE: <R>
    * 
    * 8 byte field
    *
    * Calibration Date, CCYYMMDD
    */
   char theCalDate[CAL_DATE_SIZE+1];
   
   /**
    * FIELD: PATCH_TOT
    *
    * TYPE: R
    * 
    * 4 byte field
    *
    * Patch Total
    */
   char thePatchTot[PATCH_TOT_SIZE+1];

   /**
    * FIELD: MTI_TOT
    *
    * TYPE: R
    * 
    * 3 byte field
    *
    * MTI Total, SAR: 000 to 999, EO-IR: 000
    */
   char theMtiTot[MTI_TOT_SIZE+1];
   
TYPE_DATA   
};

#endif /* matches #ifndef ossimNitfAcftbTag_HEADER */
