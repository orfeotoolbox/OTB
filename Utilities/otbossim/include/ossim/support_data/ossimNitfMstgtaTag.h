//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: MSTGTA Mission Target Information Extension Format
// tag class declaration.
//
// See document STDI-0002 Table 8-16 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#ifndef ossimNitfMstgtaTag_HEADER
#define ossimNitfMstgtaTag_HEADER 1

#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfMstgtaTag : public ossimNitfRegisteredTag
{
public:

   enum 
   {
      TGT_NUM_SIZE       = 5,
      TGT_ID_SIZE        = 12,
      TGT_BE_SIZE        = 15,
      TGT_PRI_SIZE       = 3,
      TGT_REQ_SIZE       = 12,
      TGT_LTIOV_SIZE     = 12,
      TGT_TYPE_SIZE      = 1,
      TGT_COLL_SIZE      = 1,
      TGT_CAT_SIZE       = 5,
      TGT_UTC_SIZE       = 7,
      TGT_ELEV_SIZE      = 6,
      TGT_ELEV_UNIT_SIZE = 1,
      TGT_LOC_SIZE       = 21
    };
   
   /** @brief default constructor */
   ossimNitfMstgtaTag();

   /** @brief destructor */
   virtual ~ossimNitfMstgtaTag();

   /**
    * @brief Method to return tag name.
    * @return "MSTGTA" as an ossimString.
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
    * FIELD: TGT_NUM
    *
    * TYPE: R
    * 
    * 5 byte field
    *
    * 00001 ti 99999
    */
   char theTgtNum[TGT_NUM_SIZE+1];

   /**
    * FIELD: TGT_ID
    *
    * TYPE: <R>
    *
    * 12 byte field
    *
    * Disignator of Target, alphnumeric.
    */
   char theTgtId[TGT_ID_SIZE+1];

   /**
    * FIELD: TGT_BE
    *
    * TYPE: <R>
    * 
    * 15 byte field
    * 
    * alphnumeric
    */
   char theTgtBe[TGT_BE_SIZE+1];

   /**
    * FIELD: TGT_PRI
    *
    * TYPE: <R>
    *
    * 3 byte field
    * 
    * Target priority, 1 = top, 2 = 2nd...  001 to 999
    */
   char theTgtPri[TGT_PRI_SIZE+1];

   /**
    * FIELD: TGT_REQ
    *
    * TYPE: <R>
    *
    * 12 byte field
    * 
    * Target requester. alphnumeric 
    */
   char theTgtReq[TGT_REQ_SIZE+1];

   /**
    * FIELD: TGT_LTIOV
    *
    * TYPE: <R>
    * 
    * 12 byte field
    * 
    * Latest Time Information of Value CCYYMMDDhhmm
    */
   char theTgtLtiov[TGT_LTIOV_SIZE+1];

   /**
    * FIELD: TGT_TYPE
    *
    * TYPE: <R>
    *
    * 1 byte field
    * 
    * Pre-Planned Target Type: 0 = point, 1 = strip, 2 = area, 0 to 9.
    */
   char theTgtType[TGT_TYPE_SIZE+1];

   /**
    * FIELD: TGT_COLL
    *
    * TYPE: R
    * 
    * 1 byte field
    * 
    * Pre-Planned Collection Technique: 0 to 9
    */
   char theTgtColl[TGT_COLL_SIZE+1];

   /**
    * FIELD: TGT_CAT
    *
    * TYPE: <R>
    *
    * 5 byte field
    * 
    * Target Functional Category Code 10000 to 99999
    */
   char theTgtCat[TGT_CAT_SIZE+1];

   /**
    * FIELD: TGT_UTC
    *
    * TYPE: <R>
    * 
    * 7 byte field
    *
    * Planned Time at Target hhmmssZ
    */
   char theTgtUtc[TGT_UTC_SIZE+1];

   /**
    * FIELD: TGT_ELEV
    *
    * TYPE: <R>
    * 
    * 6 byte field
    * 
    * Target Elevation -01000 to +30000 feet or meters
    */
   char theTgtElev[TGT_ELEV_SIZE+1];

   /**
    * FIELD: TGT_ELEV_UNIT
    *
    * TYPE: <R>
    * 
    * 1 byte field
    * 
    * Unit of Target Elevation, f = feet, m = meters
    */
   char theTgtElevUnit[TGT_ELEV_UNIT_SIZE+1];

   /**
    * FIELD: TGT_LOC
    *
    * TYPE: R
    *
    * 21 byte field
    * 
    * Target Location
    */
   char theTgtLoc[TGT_LOC_SIZE+1];
   
TYPE_DATA   
};

#endif /* matches #ifndef ossimNitfMstgtaTag_HEADER */
