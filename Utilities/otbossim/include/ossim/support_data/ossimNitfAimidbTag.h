//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: AIMIDB - Additional Image ID Extension Format
// tag class declaration.
//
// See document STDI-0002 (version 3), Appendix E, Table E-3 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#ifndef ossimNitfAimidbTag_HEADER
#define ossimNitfAimidbTag_HEADER 1

#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfAimidbTag : public ossimNitfRegisteredTag
{
public:

   enum 
   {
      CEL_SIZE                    = 89,
      ACQUISITION_DATE_SIZE       = 14,
      MISSION_NO_SIZE             = 4,
      MISSION_IDENTIFICATION_SIZE = 10,
      FLIGHT_NO_SIZE              = 2,
      OP_NUM_SIZE                 = 3,
      CURRENT_SEGMENT_SIZE        = 2,
      REPRO_NUM_SIZE              = 2,
      REPLAY_SIZE                 = 3,
      RESERVED_1_SIZE             = 1,
      START_TILE_COLUMN_SIZE      = 3,
      START_TILE_ROW_SIZE         = 5,
      END_SEGMENT_SIZE            = 2,
      END_TILE_COLUMN_SIZE        = 3,
      END_TILE_ROW_SIZE           = 5,
      COUNTRY_SIZE                = 2,
      RESERVED_2_SIZE             = 4,
      LOCATION_SIZE               = 11,
      RESERVED_3_SIZE             = 13
    };
   
   /** @brief default constructor */
   ossimNitfAimidbTag();

   /** @brief destructor */
   virtual ~ossimNitfAimidbTag();

   /**
    * @brief Method to return tag name.
    * @return "AIMIDB" as an std::string.
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
    * FIELD: ACQUISITION_DATE
    *
    * TYPE: R
    * 
    * 14 byte field
    *
    * CCYYMMDDhhmmss
    */
   char theAcquisitionDate[ACQUISITION_DATE_SIZE+1];

   /**
    * FIELD: MISSION_NO
    *
    * TYPE: R
    *
    * 4 byte field
    *
    * Mission Number
    */
   char theMissionNumber[MISSION_NO_SIZE+1];

   /**
    * FIELD: MISSION_IDENTIFICATION
    *
    * TYPE: R
    *
    * 10 byte field
    *
    * Name of the Mission.
    */
   char theMissionIdentification[MISSION_IDENTIFICATION_SIZE+1];
   
   /**
    * FIELD: FLIGHT_NO
    *
    * TYPE: R
    *
    * 2 byte field
    *
    * Flight Number
    */
   char theFlightNo[FLIGHT_NO_SIZE+1];

   /**
    * FIELD: OP_NUM
    *
    * TYPE: R
    *
    * 3 byte field
    *
    * Image Operation Number 000 to 999
    */
   char theOpNum[OP_NUM_SIZE+1];
   
   /**
    * FIELD: CURRENT_SEGMENT
    *
    * TYPE: R
    *
    * 2 byte field
    *
    * Current Segment ID, AA to ZZ
    */
   char theCurrentSegment[CURRENT_SEGMENT_SIZE+1];

   /**
    * FIELD: REPRO_NUM
    *
    * TYPE: R
    *
    * 2 byte field
    *
    * Reprocess Number, 00 to 99
    */
   char theReproNum[REPRO_NUM_SIZE+1];
   
   /**
    * FIELD: REPLAY
    *
    * TYPE: <R>
    *
    * 3 byte field
    *
    * Replay
    */
   char theReplay[REPLAY_SIZE+1];

   /**
    * FIELD: RESERVED_1
    *
    * TYPE: R
    *
    * 1 byte field
    *
    * 1 space
    */
   char theReserved1[RESERVED_1_SIZE+1];
   
   /**
    * FIELD: START_TILE_COLUMN
    *
    * TYPE: R
    *
    * 3 byte field
    *
    * Starting Tile Column Number, 001 to 099
    */
   char theStartTileColumn[START_TILE_COLUMN_SIZE+1];

   /**
    * FIELD: START_TILE_ROW
    *
    * TYPE: R
    *
    * 5 byte field
    *
    * Starting Tile Row Number, 00001 to 99999
    */
   char theStartTileRow[START_TILE_ROW_SIZE+1];
   
   /**
    * FIELD: END_SEGMENT
    *
    * TYPE: R
    *
    * 2 byte field
    *
    * Ending Segment, 00, AA to ZZ
    */
   char theEndSegment[END_SEGMENT_SIZE+1];

   /**
    * FIELD: END_TILE_COLUMN
    *
    * TYPE: R
    *
    * 3 byte field
    *
    * Ending Tile Column Number, 001 to 099
    */
   char theTileColumn[END_TILE_COLUMN_SIZE+1];
   
   /**
    * FIELD: END_TILE_ROW
    *
    * TYPE: R
    *
    * 5 byte field
    *
    * Ending Tile Row Number, 00001 to 99999
    */
   char theTileRow[END_TILE_ROW_SIZE+1];

   /**
    * FIELD: COUNTRY
    *
    * TYPE: <R>
    *
    * 2 byte field
    *
    * Country Code, AA to ZZ
    */
   char theCountry[COUNTRY_SIZE+1];
   
   /**
    * FIELD: RESERVED_2
    *
    * TYPE: R
    *
    * 4 byte field
    *
    * 4 spaces
    */
   char theReserved2[RESERVED_2_SIZE+1];

   /**
    * FIELD: LOCATION
    *
    * TYPE: <R>
    *
    * 11 byte field
    *
    * Location, ddmmXdddmmY, spaces
    */
   char theLocation[LOCATION_SIZE+1];

   /**
    * FIELD: RESERVED_3
    *
    * TYPE: R
    *
    * 13 byte field
    *
    * 13 spaces
    */
   char theReserved3[RESERVED_3_SIZE+1];
   
TYPE_DATA   
};

#endif /* matches #ifndef ossimNitfAimidbTag_HEADER */
