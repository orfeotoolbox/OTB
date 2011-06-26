//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  Walt Bunch
// 
// Description:   NITF support data class for STDIDC - Standard ID extension.
//
// See:  STDI-000_v2.1 Table 7-3 for detailed description.
// 
//********************************************************************
// $Id: ossimNitfStdidcTag.h 19682 2011-05-31 14:21:20Z dburken $
#ifndef ossimNitfStdidcTag_HEADER
#define ossimNitfStdidcTag_HEADER 1
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfStdidcTag : public ossimNitfRegisteredTag
{
public:

   enum
   {
      ACQ_DATE_SIZE         = 14,
      MISSION_SIZE          = 14,
      PASS_SIZE             = 2,
      OP_NUM_SIZE           = 3,
      START_SEGMENT_SIZE    = 2,
      REPRO_NUM_SIZE        = 2,
      REPLAY_REGEN_SIZE     = 3,
      BLANK_FILL_SIZE       = 1,
      START_COLUMN_SIZE     = 3,
      START_ROW_SIZE        = 5,
      END_SEGMENT_SIZE      = 2,
      END_COLUMN_SIZE       = 3,
      END_ROW_SIZE          = 5,
      COUNTRY_SIZE          = 2,
      WAC_SIZE              = 4,
      LOCATION_SIZE         = 11,
      FIELD17_SIZE          = 5,
      FIELD18_SIZE          = 8
      //                   -----
      //                     89 bytes
   };
      
   ossimNitfStdidcTag();
   virtual ~ossimNitfStdidcTag();
   virtual std::string getRegisterTagName()const;
   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);
   virtual ossim_uint32 getSizeInBytes()const;
   virtual void clearFields();

  // The set methods below taking ossimString args will truncate and
  // pad with spaces, as necessary, to match enumed size
   void setAcqDate(ossimString acqDate);
   ossimString getAcqDate()const;
   void setMission(ossimString mission);
   ossimString getMission()const;
   void setPass(ossimString pass);
   ossimString getPass()const;
   void setOpNum(ossimString opNum);
   ossimString getOpNum()const;
   void setStartSegment(ossimString startSegment);
   ossimString getStartSegment()const;
   void setReproNum(ossimString reproNum);
   ossimString getReproNum()const;
   void setReplayRegen(ossimString replayRegen);
   ossimString getReplayRegen()const;
   void setBlankFill(ossimString blankFill);
   ossimString getBlankFill()const;
   void setStartColumn(ossimString startColumn);
   ossimString getStartColumn()const;
   void setStartRow(ossimString startRow);
   ossimString getStartRow()const;
   void setEndSegment(ossimString endSegment);
   ossimString getEndSegment()const;
   void setEndColumn(ossimString endColumn);
   ossimString getEndColumn()const;
   void setEndRow(ossimString endRow);
   ossimString getEndRow()const;
   void setCountry(ossimString country);
   ossimString getCountry()const;
   void setWac(ossimString wac);
   ossimString getWac()const;
   void setLocation(ossimString location);
   ossimString getLocation()const;
   void setField17(ossimString field17);
   ossimString getField17()const;
   void setField18(ossimString field18);
   ossimString getField18()const;

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;


protected:

   /**
    * FIELD: ACQDATE
    */
   char theAcqDate[ACQ_DATE_SIZE+1];
   
   /**
    * FIELD: MISSION
    */
   char theMission[MISSION_SIZE+1];
   
   /**
    * FIELD: PASS
    */
   char thePass[PASS_SIZE+1];
   
   /**
    * FIELD: OPNUM
    */
   char theOpNum[OP_NUM_SIZE+1];
   
   /**
    * FIELD: STARTSEGMENT
    */
   char theStartSegment[START_SEGMENT_SIZE+1];
   
   /**
    * FIELD: REPRONUM
    */
   char theReproNum[REPRO_NUM_SIZE+1];
   
   /**
    * FIELD: REPLAYREGEN
    */
   char theReplayRegen[REPLAY_REGEN_SIZE+1];
   
   /**
    * FIELD: BLANKFILL
    */
   char theBlankFill[BLANK_FILL_SIZE+1];
   
   /**
    * FIELD: STARTCOLUMN
    */
   char theStartColumn[START_COLUMN_SIZE+1];
   
   /**
    * FIELD: STARTROW
    */
   char theStartRow[START_ROW_SIZE+1];
   
   /**
    * FIELD: ENDSEGMENT
    */
   char theEndSegment[END_SEGMENT_SIZE+1];
   
   /**
    * FIELD: ENDCOLUMN
    */
   char theEndColumn[END_COLUMN_SIZE+1];
   
   /**
    * FIELD: ENDROW
    */
   char theEndRow[END_ROW_SIZE+1];
   
   /**
    * FIELD: COUNTRY
    */
   char theCountry[COUNTRY_SIZE+1];
   
   /**
    * FIELD: WAC
    */
   char theWac[WAC_SIZE+1];
   
   /**
    * FIELD: LOCATION
    */
   char theLocation[LOCATION_SIZE+1];
   
   /**
    * FIELD: FIELD17
    */
   char theField17[FIELD17_SIZE+1];
   
   /**
    * FIELD: FIELD18
    */
   char theField18[FIELD18_SIZE+1];

TYPE_DATA   
};

#endif
