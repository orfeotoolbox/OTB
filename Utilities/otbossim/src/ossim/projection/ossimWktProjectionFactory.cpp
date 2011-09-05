//*************************************************************************************************
// License:  See top level LICENSE.txt file.
//
// Author:  Oscar Kramer
//
// DESCRIPTION:
// Projection Factory for EPSG coded projections. These are projections
// that are fully defined in database files and specified via a projection coordinate system (PCS)
// code.
//
//*************************************************************************************************
//  $Id: ossimWktProjectionFactory.cpp 19879 2011-07-30 16:21:50Z dburken $

#include <ossim/projection/ossimWktProjectionFactory.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimOrthoGraphicProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/projection/ossimEpsgProjectionDatabase.h>
#include <sstream>

ossimWktProjectionFactory* ossimWktProjectionFactory::m_instance = 0;
static const ossimString WKT_MAGIC ("WKT_PCS");

//*************************************************************************************************
//! Implements singleton pattern
//*************************************************************************************************
ossimWktProjectionFactory* ossimWktProjectionFactory::instance()
{
   if(!m_instance)
      m_instance = new ossimWktProjectionFactory;
   return m_instance;
}

//*************************************************************************************************
//! Constructor loads all SRS CSV files specified in the ossim prefs
//*************************************************************************************************
ossimWktProjectionFactory::ossimWktProjectionFactory()
{
   // Fetch filename of WKT projection DB file specified in ossim_preferences:
   ossimFilename db_name = 
      ossimPreferences::instance()->preferencesKWL().find("wkt_database_file");
   if (!db_name.isReadable())
      return;

   // Create only once outside the loop:
   ossimString format_id;
   ossimString line;

   // Open the DB file:
   std::ifstream db_stream (db_name.chars());
   bool good_file = false;
   if (db_stream.good())
   {
      // Format specification implied in file's magic number:
      std::getline(db_stream, format_id.string());
      format_id.trim();
      if (format_id == WKT_MAGIC)
         good_file = true;
   }
   if (!good_file)
   {
      ossimNotify(ossimNotifyLevel_WARN)<<"ossimWktProjectionDatabase Constructor -- "
         "Encountered bad WKT database file <"<<db_name<<">. Skipping this file."<<endl;
      db_stream.close();
      return;
   }

   // The file is good. Skip over the column descriptor line:
   std::getline(db_stream, line.string());

   // Loop to read all data records:
   while (!db_stream.eof())
   {
      ossimRefPtr<WktProjRecord> db_record = new WktProjRecord;
      std::getline(db_stream, line.string());
      std::vector<ossimString> csvRecord = line.explode(","); // ONLY CSV FILES CONSIDERED HERE
      if (csvRecord.size())
      {
         db_record->epsgCode  = csvRecord[0].toUInt32();
         db_record->wktString = csvRecord[1];
         m_wktProjRecords.push_back(db_record);
      }
   }

   db_stream.close();

}

//*************************************************************************************************
//! From keywordlist
//*************************************************************************************************
ossimProjection* ossimWktProjectionFactory::createProjection(const ossimKeywordlist &keywordList,
                                                             const char *prefix) const
{
    ossimProjection* proj = 0;

   // Look for a PCS (Projected Coordinate System) code entry and construct the basic projection.
   // This is the bootstrap for creating the fully-initialized proj:
   ossimString proj_spec = keywordList.find(prefix, ossimKeywordNames::PROJECTION_KW);
   if(!proj_spec.empty())
      proj = createProjection(proj_spec);
  
   if (!proj)
      return NULL;

   // The tie points and perhaps other params might still be in the KWL, so pass the KWL on to the
   // new projection after it has been amended with the default proj info extracted from the 
   // EPSG code (i.e., sans tiepoints and gsd):
   ossimKeywordlist proj_kwl;
   proj->saveState(proj_kwl, prefix);
   proj_kwl.remove(prefix, ossimKeywordNames::PIXEL_SCALE_XY_KW);
   proj_kwl.remove(prefix, ossimKeywordNames::PIXEL_SCALE_UNITS_KW);
   proj_kwl.remove(prefix, ossimKeywordNames::TIE_POINT_XY_KW);
   proj_kwl.remove(prefix, ossimKeywordNames::TIE_POINT_UNITS_KW);
   proj_kwl.addList(keywordList, false); // false: do not override existing items
   proj->loadState(proj_kwl, prefix);
   
   return proj;
}

//*************************************************************************************************
// This is the principal factory method. It accepts a WKT string, for example 
// "Anguilla_1957_British_West_Indies_Grid"
//
// IMPORTANT NOTE: Image tie-points cannot be conveyed by a WKT projection string. The projection
// created here will not be fully initialized for use in rendering imagery.
//*************************************************************************************************
ossimProjection* ossimWktProjectionFactory::createProjection(const ossimString &spec) const
{
   ossimProjection* proj = 0;
   
   std::vector<ossimRefPtr<WktProjRecord> >::iterator db_iter = m_wktProjRecords.begin();
   while ((db_iter != m_wktProjRecords.end()) && !proj)
   {
      if ((*db_iter)->wktString == spec)
      {
         // We may already have instantiated this projection, in which case just return its copy.
         // Otherwise, create the projection from the EPSG code that corresponds to the name:
         if ((*db_iter)->proj.valid())
            proj = (ossimProjection*) (*db_iter)->proj->dup();
         else
            proj = ossimEpsgProjectionDatabase::instance()->findProjection((*db_iter)->epsgCode);
      }
      db_iter++;
   }
   return proj;
}

//*************************************************************************************************
ossimObject* ossimWktProjectionFactory::createObject(const ossimString& typeName)const
{
   return createProjection(typeName);
}

//*************************************************************************************************
ossimObject* ossimWktProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
{
   return createProjection(kwl, prefix);
}

//*************************************************************************************************
//! STUB. Not implemented
//*************************************************************************************************
ossimProjection* ossimWktProjectionFactory::createProjection(const ossimFilename& /* filename */,
                                                              ossim_uint32 /* entryIdx */)const
{
   return 0;
}

//*************************************************************************************************
//! This returns the type name of all objects in all factories. This is the name used to 
//! construct the objects dynamically and this name must be unique.
//*************************************************************************************************
void ossimWktProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList) const
{
   std::vector<ossimRefPtr<WktProjRecord> >::iterator db_iter = m_wktProjRecords.begin();
   while (db_iter != m_wktProjRecords.end())
   {
      typeList.push_back((*db_iter)->wktString);
      db_iter++;
   }
   return;
}

