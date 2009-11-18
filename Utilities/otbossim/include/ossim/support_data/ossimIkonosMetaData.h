//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Class declaration for ossimIkonosMetaData.
// 
// This class parses a Space Imaging Ikonos meta data file.
//
//********************************************************************
// $Id: ossimIkonosMetaData.h 15828 2009-10-28 13:11:31Z dburken $

#ifndef ossimIkonosMetaData_HEADER
#define ossimIkonosMetaData_HEADER

#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimString.h>

class ossimFilename;
class ossimKeywordlist;


class OSSIMDLLEXPORT ossimIkonosMetaData : public ossimObject
{
public:

   /** @brief default constructor */
   ossimIkonosMetaData();


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
   
   ossimString   getSensorID() const;

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
    * @brief Method to parse Ikonos metadata file.
    *
    * @param metadata File name usually in the form of
    * "po_149875_metadata.txt."
    *
    * @return true on success, false on error.
    */
   bool parseMetaData(const ossimFilename& metadata);

   /**
    * @brief Method to parse Ikonos header file.
    *
    * @param metadata File name usually in the form of
    * "po_149875_pan_0000000.hdr"
    *
    * @return true on success, false on error.
    */
   bool parseHdrData(const ossimFilename& data_file);

   /**
    * @brief Method to parse Ikonos rpc file.
    *
    * @param metadata File name usually in the form of
    * "po_149875_pan_0000000_rpc.txt"
    *
    * @return true on success, false on error.
    *
    * @note Currently NOT implemented.
    */
   bool parseRpcData(const ossimFilename& data_file);

protected:
   /** virtual destructor */
   virtual ~ossimIkonosMetaData();

private:
   ossim_float64 theNominalCollectionAzimuth;
   ossim_float64 theNominalCollectionElevation;
   ossim_float64 theSunAzimuth;
   ossim_float64 theSunElevation;
   ossim_uint32  theNumBands;
   ossimString   theBandName;
   ossimString   theProductionDate;
   ossimString   theAcquisitionDate;
   ossimString   theAcquisitionTime;
   ossimString   theSensorID;
   
TYPE_DATA   
};

#endif /* #ifndef ossimIkonosMetaData_HEADER */
