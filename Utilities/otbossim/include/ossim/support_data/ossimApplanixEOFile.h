//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LGPL
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimApplanixEOFile.h 20483 2012-01-21 15:42:22Z dburken $
#ifndef ossimApplanixEOFile_HEADER
#define ossimApplanixEOFile_HEADER 1

#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <iostream>
#include <map>
#include <vector>

class OSSIM_DLL ossimApplanixEORecord : public ossimReferenced
{
public:

   OSSIM_DLL friend std::ostream& operator <<(std::ostream& out, const ossimApplanixEORecord& src);
   ossimApplanixEORecord(const ossimApplanixEORecord& src)
      :theField(src.theField)
   {
   }
   ossimApplanixEORecord(ossim_uint32 size=0)
   {
      setSize(size);
   }

   void setSize(ossim_uint32 size)
   {
      theField.resize(size);
   }
   const ossimString& operator [](ossim_uint32 idx)const
   {
      return theField[idx];
   }
   ossimString& operator [](ossim_uint32 idx)
   {
      return theField[idx];
   }

   std::vector<ossimString> theField;
};

class OSSIM_DLL ossimApplanixEOFile : public ossimReferenced
{
public:
   OSSIM_DLL friend std::ostream& operator <<(std::ostream& out, const ossimApplanixEOFile& src);
   ossimApplanixEOFile();

   /**
    * Will parse the entire Exterior orientation file and then index any record
    * ID's found.  This calls the parseStream method.
    *
    * @param file  The exterior orientation file
    */ 
   bool parseFile(const ossimFilename& file);

   /**
    * Will parse the stream and set internal queriable parameters.
    * @param inThe input stream containing the Applanix format exterior orientation file
    * 
    */
   bool parseStream(std::istream& in);
   

   /**
    * Utility method that just checks for an APPlanix exterior orientation header.
    * Will call the isEOFile(istream) method.
    *
    * @param file theExterior orientation file.
    */ 
   bool isEOFile(const ossimFilename& file)const;

   /**
    * Utility method that just checks for an APPlanix exterior orientation header.
    */ 
   bool isEOFile(std::istream& in)const;

   /**
    * Parses the header of the exterior orientation file.
    * Calls the parseHeader(istream, header) method.
    *
    * @param file The filename that is an Applanix Exterior orientation file.
    * @param header The resulting header that was parsed out.
    */
   bool parseHeader(const ossimFilename& file,
                    ossimString& header)const;

   /**
    * Parses the header of the exterior orientation file.
    * Calls the parseHeader(istream, header) method.
    *
    * @param in The input stream That contains an exterior orientation 
    * @param header The resulting header that was parsed out.
    */
   bool parseHeader(std::istream& in,
                    ossimString& header)const;

   /**
    * Is a case insensitive parse of the column names for the orientation
    * record.  Note this will check to see if the search key is contained
    * within any column heading.  So if a Heading has something like
    * "ID MORE TEXT"  then you can call this with just "ID" and get a match
    *
    * @param searchKey  the substring to search for
    * @return The id of the column or -1 if can't find the column
    */
   ossim_int32 getFieldIdxLike(const ossimString& searchKey)const;
   
   
   /**
    * Is a case insensitive parse of the column names for the orientation
    * record.  Note this will check to see if the search key exactly matches
    * a column heading.  So if a Heading has something like
    * "ID MORE TEXT"  then you can call this with just "ID MORE TEXT" to have a match
    *
    * @param searchKey  the substring to search for
    *
    * @return The id of the column or -1 if can't find the column
    */
   ossim_int32 getFieldIdx(const ossimString& searchKey)const;
   
   /** 
    * Will index all records based on the "ID #" field for fast searching.
    */ 
   void indexRecordIds();

   /**
    *  Given an ID it will search and find an EO record that matches the ID.
    *
    *  If the records are not indexed it will do a linear search on the record array.
    *
    *  @param id  The id of the record to find.
    *  
    *  @return The exterior orientation for that record.
    */
   const ossimRefPtr<ossimApplanixEORecord> getRecordGivenId(const ossimString& id)const;

   ossim_uint32 getNumberOfRecords()const;
   const ossimRefPtr<ossimApplanixEORecord> getRecord(ossim_uint32 idx)const;
      
   bool isHeightAboveMSL()const;
   bool isEcefModel()const;
   bool isUtmFrame()const;
   ossim_int32 getUtmZone()const;
   ossimString getUtmHemisphere()const;

   ossimString getSbetField()const;
   double getBoreSightTx()const;
   double getBoreSightTy()const;
   double getBoreSightTz()const;
   double getLeverArmLx()const;
   double getLeverArmLy()const;
   double getLeverArmLz()const;
   double getKardinalKappa()const;
   double getShiftValuesX()const;
   double getShiftValuesY()const;
   double getShiftValuesZ()const;

   ossimString getMappingFrameDatum()const;
   ossimString getMappingFrameProjection()const;
   double getMappingFrameCentralMeridian()const;
   double getMappingFrameOriginLatitude()const;
   double getMappingFrameGridScaleFactor()const;
   double getMappingFrameFalseEasting()const;
   double getMappingFrameFalseNorthing()const;

   ossimString convertToOssimDatumCode(const ossimString& datum)const;
   
protected:
   ossimString theHeader;
   ossimString theVersion;
   ossimString theBoreSightTx;
   ossimString theBoreSightTy;
   ossimString theBoreSightTz;
   ossimString theKappaCardinal;
   ossimString theLeverArmLx;
   ossimString theLeverArmLy;
   ossimString theLeverArmLz;
   ossimString theShiftValuesX;
   ossimString theShiftValuesY;
   ossimString theShiftValuesZ;
   ossimString theSbetField;

   ossimString theMappingFrameDatum;
   ossimString theMappingFrameProjection;
   ossimString theCentralMeridian;
   ossimString theOriginLatitude;
   ossimString theGridScaleFactor;
   ossimString theFalseEasting;
   ossimString theFalseNorthing;
   ossimString theUtmZone;
   ossimString theUtmHemisphere;

   ossim_float64 theMinLat;
   ossim_float64 theMinLon;
   ossim_float64 theMaxLat;
   ossim_float64 theMaxLon;
   std::vector<ossimString> theRecordFormat;
   std::vector< ossimRefPtr<ossimApplanixEORecord> > theApplanixRecordList;
   std::map<ossimString, ossimRefPtr<ossimApplanixEORecord>, ossimStringLtstr> theRecordIdMap;
};

#endif
