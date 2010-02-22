//----------------------------------------------------------------------------
//
// License:  See top levle LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Class declaration of projection factory to return projection from
// a Projection Coordinate System (PCS) code.
//
// For current documentation on pcs codes see:
// 
// http://www.remotesensing.org/geotiff/spec/geotiff6.html#6.3.3.1
// 
//----------------------------------------------------------------------------
//  $Id: ossimPcsCodeProjectionFactory.h 16308 2010-01-09 02:45:54Z eshirschorn $

#ifndef ossimPcsCodeProjectionFactory_HEADER
#define ossimPcsCodeProjectionFactory_HEADER

#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossim/base/ossimFilename.h>
#include <OpenThreads/ReadWriteMutex>

class ossimProjection;
class ossimString;
class ossimMapProjection;

class OSSIMDLLEXPORT ossimPcsCodeProjectionFactory : public ossimProjectionFactoryBase
{
public:

   // anonymous enums...
   //param1 value: origin of latitude
   //param2 value: central meridian
   //param3 value: std parameter 2
   //param4 value: std parameter 1
   //param5 value: false easting 
   //param6 value: false northing 
   enum
   {
      COORD_REF_SYS_CODE    = 0,
      COORD_REF_SYS_NAME    = 1,
      FALSE_EASTING	    = 2,
      FALSE_NORTHING        = 3,	
      PARAMETER_VALUE_1     = 4,
      PARAMETER_VALUE_2	    = 5,
      PARAMETER_VALUE_3	    = 6,
      PARAMETER_VALUE_4	    = 7, 
      UNIT                  = 8,
      KEYS_SIZE             = 9
   };
   
   /*!
    * METHOD: instance()
    * Instantiates singleton instance of this class:
    */
   static ossimPcsCodeProjectionFactory* instance();
   
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   /*!
    * METHOD: create()
    * Attempts to create an instance of the projection specified by name.
    * Returns successfully constructed projection or NULL.
    */
   
   /**
    * Creates projection from a pcs code string.
    *
    * @return ossimProjection* or null if code is not handled.
    */
   virtual ossimProjection* createProjection(const ossimString& name)const;
   
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix = 0)const;
   
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
    * Finds line in theCsvFiles array that matches name.
    *
    * @param name Name to search for.
    *
    * @param result Initialized by method to be the line split into an
    * array of strings.  Empty array if not found.
    */   
   bool findLine(const double param1, const double param2,
                 const double param3, const double param4,
                 const double param5, const double param6,
                 ossimString ossimProj, ossim_uint16& pcsCode ) const;


   bool findLine(const ossimString& name, 
                 ossim_uint16& pcsCode) const;
   
   /**
    * Attempts to determine PCS code from projection.
    *
    * @return a valid code or 0 if no match is found.
    */
   ossim_uint16 getPcsCodeFromProjection(const ossimMapProjection* proj) const;


   /**
   * Attempts to determine PCS code from the projection name.
   *
   * @return a valid code or 0 if no match is found.
   */
   ossim_uint16 getPcsCodeFromProjectionName(const ossimString projName) const;

protected:
   
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
   
   ossimPcsCodeProjectionFactory();
   
   static ossimPcsCodeProjectionFactory*  theInstance;
   
   mutable OpenThreads::ReadWriteMutex theMutex;
   std::vector<ossimFilename> theCsvFiles;
};

#endif
