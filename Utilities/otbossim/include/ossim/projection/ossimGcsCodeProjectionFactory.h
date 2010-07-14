//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//*******************************************************************
//  $Id: ossimGcsCodeProjectionFactory.h 509 2010-02-02 20:07:53Z eric.hirschorn $
#ifndef ossimGcsCodeProjectionFactory_HEADER
#define ossimGcsCodeProjectionFactory_HEADER
#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <OpenThreads/ReadWriteMutex>

class ossimMapProjection;

class OSSIM_DLL ossimGcsCodeProjectionFactory :
   public ossimProjectionFactoryBase
{
public:

   // anonymous enums...
   enum
   {
      OSSIM_COORD_REF_SYS_CODE     = 0,
      OSSIM_COORD_REF_SYS_NAME     = 1,
      OSSIM_DATUM_CODE             = 2,
      OSSIM_DATUM_NAME             = 3,
      OSSIM_GREENWICH_DATUM        = 4,
      OSSIM_UOM_CODE               = 5,
      OSSIM_ELLIPSOID_CODE         = 6,
      OSSIM_PRIME_MERIDIAN_CODE    = 7,
      OSSIM_SHOW_CRS               = 8,
      OSSIM_DEPRECATED             = 9,
      OSSIM_COORD_SYS_CODE         = 10,
      OSSIM_COORD_OP_METHOD_CODE   = 11,
      OSSIM_DX                     = 12,
      OSSIM_DY                     = 13,
      OSSIM_DZ                     = 14,
      OSSIM_RX                     = 15,
      OSSIM_RY                     = 16,
      OSSIM_RZ                     = 17,
      OSSIM_DS                     = 18,
      OSSIM_KEYS_SIZE              = 19
      
   };

   static ossimGcsCodeProjectionFactory* instance();
   virtual ~ossimGcsCodeProjectionFactory();
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   virtual ossimProjection* createProjection(const ossimString& name)const;
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix=0)const;

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

   /**
    * Attempts to determine GCS code from projection.
    *
    * @return a valid code or 0 if no match is found.
    */
   ossim_uint16 getGcsCodeFromProjection(const ossimMapProjection* proj) const;

   ossim_uint16 getGcsCodeFromProjectionName(const ossimString projName) const;

protected:
   ossimGcsCodeProjectionFactory();
   ossimGcsCodeProjectionFactory(const ossimGcsCodeProjectionFactory&);
   const ossimGcsCodeProjectionFactory& operator=(
      const ossimGcsCodeProjectionFactory& rhs);

   /**
    * Finds line in theCsvFiles array that matches name.
    *
    * @param name Name to search for.
    *
    * @param result Initialized by method to be the line split into an
    * array of strings.  Empty array if not found.
    */   
   bool findLine(const ossimString& name, ossim_uint16& gcsCode) const;

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

   static ossimGcsCodeProjectionFactory* theInstance;

   mutable OpenThreads::ReadWriteMutex theMutex;
   std::vector<ossimFilename> theCsvFiles;
};
#endif
