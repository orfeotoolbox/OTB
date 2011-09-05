//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Oscar Kramer
//
//*******************************************************************
//  $Id: ossimWktProjectionFactory.h 19879 2011-07-30 16:21:50Z dburken $

#ifndef ossimWktProjectionFactory_HEADER
#define ossimWktProjectionFactory_HEADER

#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>

class ossimProjection;
class ossimString;
class ossimMapProjection;

//*************************************************************************************************
//! Projection Factory for coded projections defined in database. These are projections
//! that are fully defined in database files and specified via a projection coordinate system (PCS)
//! code, such as EPSG codes.
//*************************************************************************************************
class OSSIMDLLEXPORT ossimWktProjectionFactory : public ossimProjectionFactoryBase
{
public:
   /*!
    * METHOD: instance()
    * Instantiates singleton instance of this class:
    */
   static ossimWktProjectionFactory* instance();

   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx) const;

   //! This is the principal factory method. It accepts a WKT string, for example 
   //! "Anguilla_1957_British_West_Indies_Grid"
   virtual ossimProjection* createProjection(const ossimString& name) const;

   //! Looks for presence of "projection" keyword with the value in WKT format,
   //! for example "Anguilla_1957_British_West_Indies_Grid" 
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix = 0)const;

   virtual ossimObject* createObject(const ossimString& typeName)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl, const char* prefix=0)const;
   
    //! This returns the type name of all objects in all factories. This is the name used to 
    //! construct the objects dynamically and this name must be unique.
   virtual void getTypeNameList(std::vector<ossimString>& typeList) const;

protected:
   //! Constructor loads the WKT CSV file specified in the ossim prefs with "wkt_database_file"
   //! keyword.
   ossimWktProjectionFactory();

   struct WktProjRecord : public ossimReferenced
   {
      WktProjRecord() : epsgCode(0), wktString(""), proj(0) {}
      ossim_uint32     epsgCode;
      ossimString      wktString;
      ossimRefPtr<ossimMapProjection> proj;
   };

   static ossimWktProjectionFactory*  m_instance; //!< Singleton implementation
   mutable std::vector<ossimRefPtr<WktProjRecord> > m_wktProjRecords;
};

#endif
