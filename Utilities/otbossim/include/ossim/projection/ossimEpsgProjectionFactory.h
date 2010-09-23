//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Oscar Kramer
//
//*******************************************************************
//  $Id: ossimEpsgProjectionFactory.h 12081 2007-11-26 21:44:18Z dburken $

#ifndef ossimEpsgProjectionFactory_HEADER
#define ossimEpsgProjectionFactory_HEADER

#include <ossim/projection/ossimEpsgProjectionDatabase.h>
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
class OSSIMDLLEXPORT ossimEpsgProjectionFactory : public ossimProjectionFactoryBase
{
public:
   /*!
    * METHOD: instance()
    * Instantiates singleton instance of this class:
    */
   static ossimEpsgProjectionFactory* instance();

   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx) const;

   //! This is the principal factory method. It accepts a string in one of three formats:
   //!
   //!   1. <group>:<code>, for example "EPSG:32615"
   //!   2. <code>, for example "32615", this is the CRS code ID for the projerection.
   //!   3. AUTO:<comma-separated spec>, for specifying custom user-defined codes.
   //!
   //! Both projected and geographic (Platte Carre) codes are handled.
   virtual ossimProjection* createProjection(const ossimString& name) const;

   //! Looks for presence of "pcs_code" keyword with the value in the format <group>:<code>,
   //! for example "EPSG:32615" (EPSG and AUTO--see above-- are presently supported).
   //! If only an integer is specified, EPSG code is assumed.
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix = 0)const;

   virtual ossimObject* createObject(const ossimString& typeName)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl, const char* prefix=0)const;
   
    //! This returns the type name of all objects in all factories. This is the name used to 
    //! construct the objects dynamically and this name must be unique.
   virtual void getTypeNameList(std::vector<ossimString>& typeList) const;

protected:
   //! Constructor loads all SRS CSV files specified in the ossim prefs. Protected as part of
   //! singleton implementation.
   ossimEpsgProjectionFactory();

   //! When the CRS is specified with the "AUTO:<spec>" code format, this method is invoked to
   //! parse the <spec> and produce a projection (or NULL if spec invalid):
   //! This really shouldn't be in this class but nobody else is parsing for it, so leave it here 
   //! for now.
   ossimMapProjection* createProjFromAutoCode(const std::vector<ossimString>& spec) const;

   ossimRefPtr<ossimEpsgProjectionDatabase>    m_projDatabase;
   static ossimEpsgProjectionFactory*  m_instance; //!< Singleton implementation
 
};

#endif
