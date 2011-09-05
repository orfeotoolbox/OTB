//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Class declaration for ossimQuickbirdMetaData.
// 
// This class parses a Space Imaging Quickbird meta data file.
//
//********************************************************************
// $Id: ossimQuickbirdMetaData.h 14412 2009-04-27 16:58:46Z dburken $

#ifndef ossimQuickbirdMetaData_HEADER
#define ossimQuickbirdMetaData_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimString.h>

#include <iosfwd>

class ossimFilename;
class ossimKeywordlist;

class OSSIMDLLEXPORT ossimQuickbirdMetaData : public ossimObject
{
public:

   /** @brief default constructor */
   ossimQuickbirdMetaData();

   /** virtual destructor */
   virtual ~ossimQuickbirdMetaData();

   /**
    * @brief Open method that takes the image file, derives the metadata,
    * header and rpc files, then calls parse methods parseMetaData,
    * parseHdrData, and parseRpcData.
    *
    * @param imageFile Usually in the form of "po_2619900_pan_0000000.tif".
    *
    * @return true on success, false on error.
    */
   bool open(const ossimFilename& imageFile);

   void clearFields();

   //---
   // Convenient method to print important image info:
   //---
   virtual std::ostream& print(std::ostream& out) const;

   /**
    * Method to save the state of the object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0) const;
   
   /**
    * Method to the load (recreate) the state of the object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);

   /**
    * @brief Method to parse Quickbird metadata file.
    *
    * @param metadata File name usually in the form of
    * ".IMD."
    *
    * @return true on success, false on error.
    */
   bool parseMetaData(const ossimFilename& metadata);

   /**
    * @brief Method to parse Quickbird rpc file.
    *
    * @param metadata File name usually in the form of
    * ".GEO.txt"
    *
    * @return true on success, false on error.
    *
    * @note Currently NOT implemented.
    */
   bool parseGEOData(const ossimFilename& data_file);

   bool parseATTData(const ossimFilename& data_file);

   bool parseEPHData(const ossimFilename& data_file);

   /** @return theSatID */
   ossimString getSatID() const;

   bool getEndOfLine( char * fileBuf,
                      ossimString lineBeginning,
                      const char * format,
                      ossimString & name );

   const ossimIpt& getImageSize() const;

/*****************************************
*parseATTData EPH GEO IMD RPB TIL
*
****************************************/
   
private:

   ossimString         theGenerationDate;
   ossimString         theBandId;
   int                 theBitsPerPixel;	
   ossimString         theSatID;
   ossimString         theTLCDate;
   ossim_float64       theSunAzimuth;
   ossim_float64       theSunElevation;
   ossim_float64       theSatAzimuth;
   ossim_float64       theSatElevation;
   int                 theTDILevel;
   std::vector<double> theAbsCalFactors;
   ossimString         theBandNameList;
   ossimIpt            theImageSize;

TYPE_DATA
};

#endif /* #ifndef ossimQuickbirdMetaData_HEADER */
