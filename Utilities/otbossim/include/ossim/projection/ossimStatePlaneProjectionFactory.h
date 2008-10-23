//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimStatePlaneProjectionFactory.h 12623 2008-04-07 14:10:08Z gpotts $
#ifndef ossimStatePlaneProjectionFactory_HEADER
#define ossimStatePlaneProjectionFactory_HEADER
#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <OpenThreads/ReadWriteMutex>
class ossimStatePlaneProjectionInfo;

class OSSIM_DLL ossimStatePlaneProjectionFactory :
   public ossimProjectionFactoryBase
{
public:

   // anonymous enums...
   enum
   {
      NAME_INDEX             = 0,
      PCS_CODE_INDEX         = 1,
      PROJECTION_CODE_INDEX  = 2,
      PARAM1_INDEX           = 3,
      PARAM2_INDEX           = 4,
      PARAM3_INDEX           = 5,
      PARAM4_INDEX           = 6,
      FALSE_EASTING_INDEX    = 7,
      FALSE_NORTHING_INDEX   = 8,
      LINEAR_UNITS_INDEX     = 9,
      DATUM_CODE_INDEX       = 10,
      KEYS_SIZE              = 11
   };
   
   static ossimStatePlaneProjectionFactory* instance();
   virtual ~ossimStatePlaneProjectionFactory();
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   virtual ossimProjection* createProjection(const ossimString& name)const;
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix=0)const;

   virtual const ossimStatePlaneProjectionInfo* getInfo(
      ossim_int32 pcsCode)const;
   
   const ossimStatePlaneProjectionInfo* getInfo(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimString& typeName)const;

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
   bool  addStatePlaneInfo(ossimStatePlaneProjectionInfo* info);
   void addCsvFile(const ossimFilename csvFile);
protected:
   ossimStatePlaneProjectionFactory();
   ossimStatePlaneProjectionFactory(const ossimStatePlaneProjectionFactory&);
   const ossimStatePlaneProjectionFactory& operator=(
      const ossimStatePlaneProjectionFactory& rhs);

   /**
    * Finds info from pcs code.  This searches theInfos list first and then the
    * theCsvFiles.
    *
    * @param pcsCode The pcs code to search for.
    *
    * @return ossimStatePlaneProjectionInfo* or NULL if not found.
    */
   ossimStatePlaneProjectionInfo* findInfo(ossim_uint32 pcsCode) const;

   /**
    * Finds info from string.  This searches theInfos list first and then the
    * theCsvFiles.
    *
    * @param name The name to search for.
    *
    * @return ossimStatePlaneProjectionInfo* or NULL if not found.
    */
   ossimStatePlaneProjectionInfo* findInfo(const ossimString& name) const;

   /**
    * Finds info from pcs code in theInfos array.
    *
    * @param pcsCode The pcs code to search for.
    *
    * @return ossimStatePlaneProjectionInfo* or NULL if not found.
    */
   ossimStatePlaneProjectionInfo* findInInfoList(ossim_uint32 pcsCode)const;

   /**
    * Finds info from string in theInfos array.
    *
    * @param name The name to search for.
    *
    * @return ossimStatePlaneProjectionInfo* or NULL if not found.
    */
   ossimStatePlaneProjectionInfo* findInInfoList(const ossimString& name)const;

   /**
    * Finds info from pcs code in theCsvFiles array.  If found this will push
    * the pointer onto theInfos array for next retrieval.
    *
    * @param pcsCode The pcs code to search for.
    *
    * @return ossimStatePlaneProjectionInfo* or NULL if not found.
    *
    * @note theInfos data member was made mutable so that this method could
    * be const.
    */
   ossimStatePlaneProjectionInfo* findInCsvFile(ossim_uint32 pcsCode) const;

   /**
    * Finds info from string in theCsvFiles array. If found this will push
    * the pointer onto theInfos array for next retrieval. 
    *
    * @param name The name to search for.
    *
    * @return ossimStatePlaneProjectionInfo* or NULL if not found.
    *
    * @note theInfos data member was made mutable so that this method could
    * be const.
    */
   ossimStatePlaneProjectionInfo* findInCsvFile(const ossimString& name) const;

   /**
    * Finds line in theCsvFiles array that matches pcs code.
    *
    * @param pcsCode Code to search for.
    *
    * @param result Initialized by method to be the line split into an
    * array of strings.  Empty array if not found.
    */
   bool findLine(ossim_uint32 pcsCode,
                 std::vector<ossimString> &result) const;

   /**
    * Finds line in theCsvFiles array that matches name.
    *
    * @param name Name to search for.
    *
    * @param result Initialized by method to be the line split into an
    * array of strings.  Empty array if not found.
    */   
   bool findLine(const ossimString& name,
                 std::vector<ossimString> &result) const;

   /**
    * Creates info object from line array.
    *
    * @param lineArray Line to initialize from.
    *
    * @return ossimStatePlaneProjectionInfo* from line.  The return can be null
    * if the datum code is not found in the datum factory.
    */   
   ossimStatePlaneProjectionInfo* createInfo(
      const std::vector<ossimString> &lineArray) const;

   /**
    * @param file File to search list for.
    * @return true if file is in theCsvFiles array, false if not.
    */
   bool isFileInList(const ossimFilename& file) const;

   /**
    * @param file File to check.
    * @return true if first line of file matches known key order.
    */
   bool isValidCsvFile(const ossimFilename& file) const;

   /**
    * Split line method.  This will split the line between commas ',' making
    * an emptry ossimString on double commas ",,".  Stripping leading and
    * trailing quotes '"'.
    */
   void splitLine(const std::string& line,
                  std::vector<ossimString> &result) const;
   
   static ossimStatePlaneProjectionFactory* theInstance;

   mutable OpenThreads::ReadWriteMutex theMutex;
   std::vector<ossimFilename> theCsvFiles;
   mutable std::vector<ossimStatePlaneProjectionInfo*> theInfos;
};
#endif
