//*************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// License:  See top level LICENSE.txt file.
//
// Author: Oscar Kramer
//
//*************************************************************************************************
//  $Id: ossimEpsgProjectionDatabase.h 21511 2012-08-21 22:00:11Z dburken $

#ifndef ossimEpsgProjectionDatabase_HEADER
#define ossimEpsgProjectionDatabase_HEADER 1

#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/projection/ossimMapProjection.h>
#include <OpenThreads/Mutex>
#include <fstream>


class ossimProjection;
class ossimString;
class ossimEpsgProjectionFactory;
class ossimUtmProjection;

//*************************************************************************************************
//! Projection Database for coded projections defined in database files and specified via some 
//! coordinate reference system group:code, such as EPSG:26715.
//*************************************************************************************************
class OSSIMDLLEXPORT ossimEpsgProjectionDatabase : public ossimReferenced
{
   friend class ossimEpsgProjectionFactory;

public:
   //! Instantiates singleton instance of this class:
   static ossimEpsgProjectionDatabase* instance();

   //! Destructor
   virtual ~ossimEpsgProjectionDatabase();

   //! Returns a projection corresponding to the projection specified, or NULL if no entry found.
   //! Normally <proj_spec> takes the form of <group>:<code> (e.g., "EPSG:26715"). Other forms 
   //! accepted are <code> (assumed EPSG), or <proj_name> to match a name in the database.
   ossimProjection* findProjection(const ossimString& proj_spec) const;

   //! Returns a projection corresponding to the epsg code specified, or NULL if no entry found.
   ossimProjection* findProjection(ossim_uint32 epsg_code) const;

   //! Given a projection name, assigns the group (e.g., "EPSG") and code of the projection. 
   //! Returns integer EPSG code if match was found or 0 if not found.
   ossim_uint32 findProjectionCode(const ossimString& projection_name) const;

   //! Given a projection instance, this method determines the corresponding EPSG code. Obviously
   //! this is only needed if the projection does not have its PCS code assigned (it is NULL). This
   //! happens when the projection was constructed with full parameters instead of the EPSG code.
   //! Returns integer EPSG code if match was found or 0 if not found.
   ossim_uint32 findProjectionCode(const ossimMapProjection& lost_proj) const;

   //! ### NECESSARY HACK ### (originally implemented by ESH)
   //! ArcMap (9.2 and less) doesn't understand the non-meters HARN codes.  
   //! This method provides acess to the projection name given the EPSG code. The name provides 
   //! enough info for recognizing a non-meters HARN-based projection. 
   ossimString findProjectionName(ossim_uint32 epsg_code) const;

   //! Populates caller's list with all projections currently represented in the database.
   //! The format is: 
   //!    EPSG:<code>  "<Name as appears in Db>"
   //! There may be duplicate entries if the projection is duplicated in the various Db files.
   void getProjectionsList(std::vector<ossimString>& typeList) const;

   //! ENGINEERING CODE. Used for testing
   size_t numRecords() const { return m_projDatabase.size(); }

protected:
   enum RecordFormat
   {  
      NOT_ASSIGNED=0,
      FORMAT_A,
      FORMAT_B,
      FORMAT_C,
      CUSTOM
   };

   //! Type for database record consists of EPSG code and serialized form of corresponding OSSIM 
   //! projection (as a keywordlist)
   class ProjDbRecord : public ossimReferenced
   {
   public:
      ProjDbRecord() : 
            code(0), 
            name(""), 
            datumValid(false), 
            csvFormat(NOT_ASSIGNED), 
            proj(0) {}

      ossim_uint32     code;
      ossimString      name;
      bool             datumValid; //!< FALSE if the datum code was not parsed and WGS84 defaulted
      RecordFormat     csvFormat;
      std::vector<ossimString>        csvRecord;
      ossimRefPtr<ossimMapProjection> proj;
   };

   //! Constructor loads all Db files specified in the ossim prefs. Protected as part of
   //! singleton implementation.
   ossimEpsgProjectionDatabase();

   //! Parses the "Spadac EPSG" Db record format and produces a projection (or NULL if invalid)   
   ossimMapProjection* createProjFromFormatARecord(ProjDbRecord* record) const;

   //! Parses the State Plane Db record format and produce a projection (or NULL if invalid)
   ossimMapProjection* createProjFromFormatBRecord(ProjDbRecord* record) const;

   //!  ### HACK ###
   //! UTM projections as specified in the EPSG are indistinguishable from regular TM.
   //! Unfortunately OSSIM does make a distinction. For the time being, parse the code and
   //! programmatically arrives at the UTM projection.
   ossimMapProjection* createProjFromUtmCode(ossim_uint32 code) const;

   //! Given UTM projection, derives the associated EPSG code. This is faster than a Db lookup.
   ossim_uint32 getCodeFromUtmProj(const ossimUtmProjection* proj) const;

   //! Populates the database with contents of DB files as specified in ossim_preferences.
   void initialize();

   mutable std::multimap<ossim_uint32, ossimRefPtr<ProjDbRecord> > m_projDatabase;
   mutable OpenThreads::Mutex m_mutex;
   static ossimEpsgProjectionDatabase*  m_instance; //!< Singleton implementation
   
};

#endif
