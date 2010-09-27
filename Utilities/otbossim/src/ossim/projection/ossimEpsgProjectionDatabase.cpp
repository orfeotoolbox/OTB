//*************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE:  See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Projection database for EPSG coded projections provided in database files
//
//*************************************************************************************************
//  $Id$
#include <ossim/projection/ossimEpsgProjectionDatabase.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimMercatorProjection.h>
#include <ossim/projection/ossimOrthoGraphicProjection.h>
#include <ossim/projection/ossimNewZealandMapGridProjection.h>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/projection/ossimCassiniProjection.h>
#include <ossim/projection/ossimAlbersProjection.h>
#include <ossim/base/ossimEpsgDatumFactory.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimDatumFactoryRegistry.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/base/ossimException.h>
#include <math.h>

ossimEpsgProjectionDatabase* ossimEpsgProjectionDatabase::m_instance = 0;

// Indexes into the DB file fields using "SPADAC EPSG" format (format "A"). 
// IF THE LINE FIELDS CHANGE ORDER, THIS LIST WILL NEED TO BE CHANGED TO REFLECT THE NEW ORDER!
enum
{
   A_CODE = 0,  // This is the EPSG code
   A_NAME,
   A_PROJ_TYPE,
   A_UNITS,
   A_DATUM_CODE,
   A_DATUM_NAME,
   A_ELLIPSOID_NAME,
   A_FALSE_EASTING,
   A_NAT_ORG_LAT,
   A_FALSE_NORTHING,
   A_NAT_ORG_LON,
   A_NAT_ORG_SCALE,
   A_INIT_LINE_AZ,
   A_PROJ_CTR_LAT,
   A_PROJ_CTR_NORTHING,
   A_RECT_TO_SKEW_ANGLE,
   A_INIT_LINE_SCALE,
   A_PROJ_CTR_EASTING,
   A_PROJ_CTR_LON,
   A_PSEUDO_STD_PARL_1,
   A_PSEUDO_STD_PARL_1_SCALE,
   A_ORG_LON,
   A_STD_PARL_2_LAT,
   A_FALSE_ORG_LAT,
   A_FALSE_ORG_LON,
   A_STD_PARL_1_LAT,
   A_FALSE_ORG_NORTHING,
   A_FALSE_ORG_EASTING,
   A_STD_PARL_LAT,
   A_TOPOCTR_HGT,
   A_TOPOCTR_LAT,
   A_TOPOCTR_Z,
   A_TOPOCTR_Y,
   A_TOPOCTR_X,
   A_VIEWPOINT_H,
   A_INIT_LON,
   A_ZONE_WIDTH,
   A_NUM_FIELDS   // Not an index, but a count
};
static const ossimString EPSG_DB_FORMAT_A ("EPSG_DB_FORMAT_A");

// State Plane database CSV file field format (format "B")
// NOTE: Eventually, all EPSG-coded projections, including State Plane/HARN should be specified
// in the EPSG database format. Presently there is a disconnect between Geotrans params and EPSG-Db
// params, particularly regarding datums, that makes a purely EPSG-Db solution too awkward to
// implement, so provision is made here to continue reading the state plane CSV files.
enum 
{
   B_NAME = 0,
   B_CODE,   // This is the EPSG code
   B_PROJ_TYPE,
   B_PARAM1,
   B_PARAM2,
   B_PARAM3,
   B_PARAM4,
   B_FALSE_EASTING,
   B_FALSE_NORTHING,
   B_UNITS,
   B_DATUM_CODE,
   B_NUM_FIELDS   // Not an index, but a count
};
static const ossimString STATE_PLANE_FORMAT_B ("STATE_PLANE");

// "Ming Special" database CSV file format (format "C")
// State Plane Coordinate System (SPCS) coding is an alternate coding scheme that maps to EPSG.
// http://www.pcigeomatics.com/cgi-bin/pcihlp/PROJ%7CSPCS+ZONES
enum 
{
   C_NAME = 0,
   C_CODE,
   C_NUM_FIELDS   // Not an index, but a count
};
static const ossimString SPCS_EPSG_MAP_FORMAT_C ("SPCS_EPSG_MAP");

// Alternate projection Well Known Text naming scheme database CSV file format (format "D")
// WKT_PCS coding is an alternate naming scheme that maps to EPSG.
enum 
{
   D_CODE = 0,
   D_NAME,
   D_NUM_FIELDS   // Not an index, but a count
};
static const ossimString WKT_PCS_FORMAT_D ("WKT_PCS");


//*************************************************************************************************
//! Converts sexagesimal DMS to decimal degrees
//*************************************************************************************************
double decodeSexagesimalDms(const ossimString& sex_str)
{
   if (!sex_str.contains("."))
      return sex_str.toDouble();

   double sign = 1.0;
   if (sex_str.chars()[0] == '-')
      sign = -1.0;

   double dsex, msex, ssex;
   std::vector<ossimString> splitstr = sex_str.split(".");
   dsex = splitstr[0].toDouble();
   
   if (splitstr.size() < 2)
      return dsex;

   const char* MMSSssstr = splitstr[1].chars();
   unsigned int str_size = (unsigned int) splitstr[1].size();
   char minstr[] = "00";
   ossimString secstr ("00.");
   minstr[0] = MMSSssstr[0];
   if (str_size > 1)
   {
      minstr[1] = MMSSssstr[1];
      if (str_size > 2)
      {
         secstr = &(MMSSssstr[2]);
         if (str_size == 3)
            secstr += ossimString("0.");
         else if (str_size > 4)
            secstr.insert(2, ".");
      }
   }
   msex = ossimString(minstr).toDouble();
   ssex = secstr.toDouble();
   dsex += sign*(msex + ssex/60.0)/60.0;
   return dsex;
}

//*************************************************************************************************
//! Implements singleton pattern
//*************************************************************************************************
ossimEpsgProjectionDatabase* ossimEpsgProjectionDatabase::instance()
{
   if(!m_instance)
      m_instance = new ossimEpsgProjectionDatabase;
   return m_instance;
}

//*************************************************************************************************
//! Destructor
//*************************************************************************************************
ossimEpsgProjectionDatabase::~ossimEpsgProjectionDatabase()
{
   m_instance = 0;
}

//*************************************************************************************************
//! Constructor loads all DB CSV files specified in the ossim prefs
//*************************************************************************************************
ossimEpsgProjectionDatabase::ossimEpsgProjectionDatabase()
{
   // Read the ossim preferences for Db CSV files.
   initialize();
}

//*************************************************************************************************
//! Populates the database with contents of DB files as specified in ossim_preferences.
//*************************************************************************************************
void ossimEpsgProjectionDatabase::initialize()
{
   // Fetch filenames of all projection DB files specified in ossim_preferences:
   ossimString regEx =  ossimString("^epsg_database_file[0-9]+");
   std::vector<ossimString> keys = 
      ossimPreferences::instance()->preferencesKWL().getSubstringKeyList(regEx);
   std::vector<ossimString>::const_iterator i = keys.begin();

   // Create only once outside the loop:
   ossimFilename db_name;
   ossimString group_id;
   ossimString format_id;
   ossimString line;
   DbEntry file_record;

   // Loop over each file and read contents into memory:
   while ( i != keys.end() )
   {
      db_name = ossimPreferences::instance()->preferencesKWL().find( (*i).c_str() );
      ++i;
      if (!db_name.isReadable())
         continue;

      // Open the DB file:
      std::ifstream db_stream (db_name.chars());
      bool good_file = false;
      if (db_stream.good())
      {
         // Format specification implied in file's magic number:
         std::getline(db_stream, format_id);
         format_id.trim();
         if ((format_id == EPSG_DB_FORMAT_A) || 
             (format_id == STATE_PLANE_FORMAT_B) ||
             (format_id == SPCS_EPSG_MAP_FORMAT_C) ||
             (format_id == WKT_PCS_FORMAT_D))
            good_file = true;
      }
      if (!good_file)
      {
         ossimNotify(ossimNotifyLevel_WARN)<<"ossimEpsgProjectionDatabase::initialize() -- "
            "Encountered bad database file <"<<db_name<<">. Skipping this file."<<endl;
         db_stream.close();
         continue;
      }

      // The file is good. Skip over the column descriptor line:
      std::getline(db_stream, line);
      file_record.clear();

      // Loop to read all data records:
      while (!db_stream.eof())
      {
         std::getline(db_stream, line);
         file_record = line.explode(","); // ONLY CSV FILES CONSIDERED HERE
         if (file_record.size())
         {
            ossimRefPtr<ossimMapProjection> proj;
            ProjRecord proj_record;

            // Check if primary EPSG database format A:
            if (format_id == EPSG_DB_FORMAT_A)
            {
               proj = createProjFromFormatARecord(file_record, proj_record.datumValid);
               proj_record.code = file_record[A_CODE].toUInt32();
               proj_record.name = file_record[A_NAME];
            }

            // Check if State Plane (subset of EPSG but handled differently until projection 
            // geotrans-EPSG disconnect is resolved. 
            else if (format_id == STATE_PLANE_FORMAT_B)
            {
               proj = createProjFromFormatBRecord(file_record);
               proj_record.code = file_record[B_CODE].toUInt32();
               proj_record.name = file_record[B_NAME];
               proj_record.datumValid = true;
            }

            // This format is for Ming-special State Plane Coordinate System coded format.
            // This format is simply a mapping from SPCS spec name (OSSIM-specific) to EPSG code.
            // Note that no proj is instantiated and no KWL is populated. Only name and EPSG mapped
            // code is saved.
            else if (format_id == SPCS_EPSG_MAP_FORMAT_C)
            {
               proj_record.code = file_record[C_CODE].toUInt32();
               proj_record.name = file_record[C_NAME];
               m_projDatabase.push_back(proj_record);
            }

            // This format is for alternate projection naming scheme database CSV file format.
            // WKT_PCS coding is an alternate naming scheme that maps to EPSG.
            // Note that no proj is instantiated and no KWL is populated. Only name and EPSG mapped
            // code is saved.
            else if (format_id == WKT_PCS_FORMAT_D)
            {
               proj_record.code = file_record[D_CODE].toUInt32();
               proj_record.name = file_record[D_NAME];
               m_projDatabase.push_back(proj_record);
            }

            if (proj.valid() && proj_record.datumValid)
            {
               // Serialize the projection to a KWL and stick it in the in-memory database:
               proj->saveState(proj_record.kwl);
               m_projDatabase.push_back(proj_record);
            }
         }
      }

      db_stream.close();
   } // end of while loop over all DB files
}

//*************************************************************************************************
//! Returns a projection corresponding to the group (e.g., "EPSG") and PCS code provided, 
//! or NULL if no entry found.
//*************************************************************************************************
ossimProjection* ossimEpsgProjectionDatabase::findProjection(ossim_uint32 epsg_code) const
{
   // Quick check for bogus EPSG:
   if ((epsg_code == 0) || (epsg_code == 32767))
      return 0;

   ossimMapProjectionFactory* factory = ossimMapProjectionFactory::instance();
   ossimMapProjection* proj = 0;

   // Search database for entry:
   std::vector<ProjRecord>::const_iterator db_iter = m_projDatabase.begin();
   while (db_iter != m_projDatabase.end())
   {
      if (db_iter->code == epsg_code)
      {
         // We have a match, instantiate the projection from the associated KWL in the DB. Trick
         // the registry into using appropriate factory by setting PCS code temporarily to 0 to
         // avoid infinite recursion:
         checkForUnhandledDatum(*db_iter);
         ossimKeywordlist proj_kwl (db_iter->kwl); // make copy since this is a const method
         proj_kwl.remove(ossimKeywordNames::PCS_CODE_KW);
         proj = PTR_CAST(ossimMapProjection, factory->createProjection(proj_kwl));
         if (proj)
            proj->setPcsCode(epsg_code);
         break;
      }
      db_iter++;
   }
   return proj;
}

//*************************************************************************************************
//! Returns a projection corresponding to the group (e.g., "EPSG") and PCS code provided, 
//! or NULL if no entry found.
//*************************************************************************************************
ossimProjection* ossimEpsgProjectionDatabase::findProjection(const ossimString& spec) const
{
   ossimProjection* proj = 0;

   // Use the CRS code to access the database. The spec should be <group>:<code> where <group> is 
   // "EPSG" (the only group handled here):
   ossim_uint32 spec_code;
   ossimString spec_group ("epsg"); // default if only integer code provided
   if (spec.contains(":"))
   {
      spec_group = spec.before(":");
      spec_code = spec.after(":").toUInt32();
      spec_group = spec_group.downcase();
   }
   else
   {
      spec_code = spec.toUInt32();
   }
   if(spec_code == 4326)
   {
      return new ossimEquDistCylProjection();
   }
   // Presently only EPSG database is handled:
   if (spec_code != 0)
   {
      if (spec_group != "epsg")
         return 0;
      proj = findProjection(spec_code);
      if (proj)
         return proj;
   }

   // spec_code = 0 indicates that the spec is probably a projection name. Need to search Db
   // by the projection name. Search database for entry:
   ossimRefPtr<ossimMapProjection> map_proj = 0;
   ossimMapProjectionFactory* factory = ossimMapProjectionFactory::instance();
   std::vector<ProjRecord>::const_iterator db_iter = m_projDatabase.begin();
   while ((db_iter != m_projDatabase.end()) && !proj)
   {
      if (db_iter->name == spec)
      {
         // We have a match, instantiate the projection from the associated KWL in the DB. Trick
         // the registry into using appropriate factory by setting PCS code temporarily to 0 to
         // avoid infinite recursion:
         checkForUnhandledDatum(*db_iter);
         if (db_iter->kwl.getSize() > 0)
         {
            ossimKeywordlist proj_kwl (db_iter->kwl); // make copy since this is a const method
            proj_kwl.remove(ossimKeywordNames::PCS_CODE_KW);
            map_proj = PTR_CAST(ossimMapProjection, factory->createProjection(proj_kwl));
            if (map_proj.valid())
            {
               map_proj->setPcsCode(db_iter->code);
               proj = (ossimProjection*) map_proj->dup();
            }
         }
         else
         {
            // An empty KWL indicates the record is simply a map to another EPSG Db entry:
            proj = findProjection(db_iter->code);
         }
      }
      db_iter++;
   }
   if (proj)
      return proj;
    
   // No hit? Could be that just a datum was identified, in which case we need a simple 
   // Platte Carrée:
   const ossimDatum* datum = ossimDatumFactoryRegistry::instance()->create(spec);
   if (datum)
   {
      ossimEquDistCylProjection* proj = new ossimEquDistCylProjection(*(datum->ellipsoid()));
      proj->setDatum(datum);
      proj->setPcsCode(spec_code);
      return proj;
   }

   return 0;
}


//*************************************************************************************************
//! Given a projection name, assigns the group (e.g., "EPSG") and code of the projection. 
//! Returns "<group>:<code>" if match was found or empty string if not found.
//*************************************************************************************************
ossim_uint32 ossimEpsgProjectionDatabase::findProjectionCode(const ossimString& proj_name) const
{
   std::vector<ProjRecord>::const_iterator db_iter = m_projDatabase.begin();
   while (db_iter != m_projDatabase.end())
   {
      if (db_iter->name == proj_name)
         return (db_iter->code);
      db_iter++;
   }
      
   return 0;
}

//*************************************************************************************************
//! Given a projection instance, this method determines the corresponding EPSG code. Obviously
//! this is only needed if the projection does not have its PCS code assigned (it is NULL). This
//! happens when the projection was constructed with full parameters instead of the EPSG code.
//! Returns integer EPSG code if match was found or 0 if not found.
//*************************************************************************************************
ossim_uint32 
ossimEpsgProjectionDatabase::findProjectionCode(const ossimMapProjection& lost_proj) const
{
   ossimString lost_type (lost_proj.getClassName());
   ossimString lookup;
   ossimRefPtr<ossimMapProjection> found_proj = 0;
   ossimMapProjectionFactory* factory = ossimMapProjectionFactory::instance();
   ossim_uint32 found_code = 0;

   std::vector<ProjRecord>::const_iterator db_iter = m_projDatabase.begin();
   while ((db_iter != m_projDatabase.end()) && (found_code == 0))
   {
      // To avoid having to instantiate all projections in the database, let's peek in the record's
      // KWL for the projection type and only instantiate those that match:
      lookup = db_iter->kwl.find(ossimKeywordNames::TYPE_KW);
      if (lost_type == lookup)
      {
         // We have a match, instantiate the projection from the associated KWL in the DB. Trick
         // the registry into using appropriate factory by setting PCS code temporarily to 0 to
         // avoid infinite recursion:
         ossimKeywordlist kwl (db_iter->kwl); // make copy since this is a const method
         kwl.remove(ossimKeywordNames::PCS_CODE_KW);
         found_proj = PTR_CAST(ossimMapProjection, factory->createProjection(kwl));

         // Test for same-ness between target and found, and return if matching:
         if (found_proj.valid() && (*found_proj == lost_proj))
         {
            found_code = db_iter->code;

            // Hack to remap projection code 4087 to 4326 (which is not really a projection 
            // code but other packages like to see 4326 for geographic projections.
            // Hacked under protest (OLK, 08/2010)
            if (found_code == 4087)
               found_code = 4326;
         }
      }
      db_iter++;
   }
   return found_code;
}

//*************************************************************************************************
//! This method provides acess to the projection name given the EPSG code. The name provides 
//! enough info for recognizing a non-meters HARN-based projection. 
//*************************************************************************************************
ossimString ossimEpsgProjectionDatabase::findProjectionName(ossim_uint32 epsg_code) const
{
   ossimString name ("");
   std::vector<ProjRecord>::const_iterator db_iter = m_projDatabase.begin();
   while (db_iter != m_projDatabase.end())
   {
      if (db_iter->code == epsg_code)
      {
         name = db_iter->name;
         break;
      }
      db_iter++;
   }
   return name;
}

//*************************************************************************************************
//! Populates caller's list with all projections currently represented in the database.
//! The format is: 
//!    EPSG:<code>  "<Name as appears in Db>"
//! There may be duplicate entries if the projection is duplicated in the various Db files.
//*************************************************************************************************
void ossimEpsgProjectionDatabase::getProjectionsList(std::vector<ossimString>& list) const
{
   std::vector<ProjRecord>::const_iterator db_iter = m_projDatabase.begin();
   while (db_iter != m_projDatabase.end())
   {
      ossimString record ("EPSG:");
      record += ossimString::toString(db_iter->code);
      record += "  \"";
      record += db_iter->name;
      record += "\"";
      list.push_back(record);
      db_iter++;
   }
   return;
}

//*************************************************************************************************
//! This method is invoked to parse the Db record and produce a projection (or NULL if spec invalid)
//
// LIMITATION: Currently not parsing the datum info from the EPSG database file due to disconnect
// with current ossimDatumFactory. Setting to default WGS84 with warning message.
//*************************************************************************************************
ossimMapProjection* 
ossimEpsgProjectionDatabase::createProjFromFormatARecord(const DbEntry& record, 
                                                         bool& datum_valid) const
{
   // Establish EPSG code and test for UTM (full projection is implied in the code itself -- no 
   // accessing the database). Until the database is solidified, it is probably better to do 
   // it this way:
   datum_valid = true;
   ossim_uint32 pcs_code = record[A_CODE].toUInt32();
   ossimMapProjection* proj = createProjFromUtmCode(pcs_code);
   if (proj)
      return proj;

   // Establish the units in which the easting/northing is provided:
   double mtrs_per_unit = 1.0;
   if (record[A_UNITS] == "US survey foot")
      mtrs_per_unit = US_METERS_PER_FT;
   else if (record[A_UNITS].contains("foot")) // THIS IS INTERNATIONAL FOOT, NOT EXACT FOR MANY INTERNATIONAL VARIETIES
      mtrs_per_unit = MTRS_PER_FT;
   else if (!record[A_UNITS].contains("metre") || (record[A_UNITS] == "kilometre"))
   {
      // ### SKIP THIS MESSAGE BUT BE AWARE THAT THIS PROJECTION WON'T BE REPRESENTED IN DB ###
      //ossimNotify(ossimNotifyLevel_WARN)<<MODULE<<"EPSG:"<<pcs_code<<" units of <"
      //   <<record[A_UNITS]<<"> not presently supported."<<endl;
      return 0;
   }

   // First create a datum given the datum code in the record:
   ossim_uint32 gcs_code = record[A_DATUM_CODE].toUInt32();
   const ossimDatum* datum = ossimEpsgDatumFactory::instance()->create(gcs_code);
   if (!datum)
   {
      // Default to WGS 84 -- this may throw an exception:
      datum = ossimEpsgDatumFactory::instance()->create(ossimString("EPSG:4326"));
      datum_valid = false;
   }
   const ossimEllipsoid* ellipsoid = datum->ellipsoid();

   ossimGpt origin(0,0,0);
   ossimString proj_type = record[A_PROJ_TYPE];
   if (proj_type.contains("Transverse Mercator"))
   {
      origin.lat = decodeSexagesimalDms(record[A_NAT_ORG_LAT]);
      origin.lon = decodeSexagesimalDms(record[A_NAT_ORG_LON]);
      double fe = mtrs_per_unit*record[A_FALSE_EASTING].toDouble();
      double fn = mtrs_per_unit*record[A_FALSE_NORTHING].toDouble();
      double sf = record[A_NAT_ORG_SCALE].toDouble();
      proj = new ossimTransMercatorProjection(*ellipsoid, origin, fe, fn, sf);
   }
   else if (proj_type.contains("Lambert Conic Conformal (1SP)"))
   {
      origin.lat = decodeSexagesimalDms(record[A_NAT_ORG_LAT]);
      origin.lon = decodeSexagesimalDms(record[A_NAT_ORG_LON]);
      double fe = mtrs_per_unit*record[A_FALSE_EASTING].toDouble();
      double fn = mtrs_per_unit*record[A_FALSE_NORTHING].toDouble();
      proj = new ossimLambertConformalConicProjection(*ellipsoid, origin, origin.lat, 
         origin.lat, fe, fn);
   }
   else if (proj_type.contains("Lambert Conic Conformal (2SP)"))
   {
      origin.lat = decodeSexagesimalDms(record[A_FALSE_ORG_LAT]);
      origin.lon = decodeSexagesimalDms(record[A_FALSE_ORG_LON]);
      double p1 = decodeSexagesimalDms(record[A_STD_PARL_1_LAT]);
      double p2 = decodeSexagesimalDms(record[A_STD_PARL_2_LAT]);
      double fe = mtrs_per_unit*record[A_FALSE_ORG_EASTING].toDouble();
      double fn = mtrs_per_unit*record[A_FALSE_ORG_NORTHING].toDouble();
      proj = new ossimLambertConformalConicProjection(*ellipsoid, origin, p1, p2, fe, fn);
   }
   else if (proj_type.contains("Cassini"))
   {
      origin.lat = decodeSexagesimalDms(record[A_NAT_ORG_LAT]);
      origin.lon = decodeSexagesimalDms(record[A_NAT_ORG_LON]);
      double fe = mtrs_per_unit*record[A_FALSE_EASTING].toDouble();
      double fn = mtrs_per_unit*record[A_FALSE_NORTHING].toDouble();
      proj =  new ossimCassiniProjection(*ellipsoid, origin, fe, fn);
   }
   else if (proj_type.contains("Mercator (1SP)"))
   {
      origin.lat = decodeSexagesimalDms(record[A_NAT_ORG_LAT]);
      origin.lon = decodeSexagesimalDms(record[A_NAT_ORG_LON]);
      double fe = mtrs_per_unit*record[A_FALSE_EASTING].toDouble();
      double fn = mtrs_per_unit*record[A_FALSE_NORTHING].toDouble();
      double sf = record[A_NAT_ORG_SCALE].toDouble();
      proj = new ossimMercatorProjection(*ellipsoid, origin, fe, fn, sf);
   }
   else if (proj_type.contains("Albers"))
   {
      origin.lat = decodeSexagesimalDms(record[A_FALSE_ORG_LAT]);
      origin.lon = decodeSexagesimalDms(record[A_FALSE_ORG_LON]);
      double p1 = decodeSexagesimalDms(record[A_STD_PARL_1_LAT]);
      double p2 = decodeSexagesimalDms(record[A_STD_PARL_2_LAT]);
      double fe = mtrs_per_unit*record[A_FALSE_ORG_EASTING].toDouble();
      double fn = mtrs_per_unit*record[A_FALSE_ORG_NORTHING].toDouble();
      proj = new ossimAlbersProjection(*ellipsoid, origin, p1, p2, fe, fn);
   }
   else if (proj_type.contains("Equidistant Cylindrical"))
   {
      origin.lat = decodeSexagesimalDms(record[A_STD_PARL_1_LAT]);
      origin.lon = decodeSexagesimalDms(record[A_NAT_ORG_LON]);
      double fe = mtrs_per_unit*record[A_FALSE_EASTING].toDouble();
      double fn = mtrs_per_unit*record[A_FALSE_NORTHING].toDouble();
      proj = new ossimEquDistCylProjection(*ellipsoid, origin, fe, fn);
   }
   else if (proj_type.contains("New Zealand Map Grid"))
   {
      origin.lat = decodeSexagesimalDms(record[A_NAT_ORG_LAT]);
      origin.lon = decodeSexagesimalDms(record[A_NAT_ORG_LON]);
      double fe = mtrs_per_unit*record[A_FALSE_EASTING].toDouble();
      double fn = mtrs_per_unit*record[A_FALSE_NORTHING].toDouble();
      ossimNewZealandMapGridProjection* nz_proj = new ossimNewZealandMapGridProjection;
      nz_proj->setOrigin(origin);
      nz_proj->setFalseEastingNorthing(fe, fn);
      proj = nz_proj;
   }
   else 
   {
      // Can't handle it now. 
      //ossimNotify(ossimNotifyLevel_FATAL)<<MODULE<<"EPSG:"<<record[A_CODE]<<" \""<<proj_type<<"\" "
      //   "not presently supported. Returning NULL projection..."<<endl;
      return 0;
   }
  
   proj->setDatum(datum);
   proj->setPcsCode(pcs_code);
   return proj;
}

//*************************************************************************************************
// ### HACK ###
//! State Plane projections as specified in the EPSG are indistinguishable from regular TM. 
//! Unfortunately OSSIM does make a distinction. For the time being, parse the code and 
//! programmatically arrive at the UTM projection.
//*************************************************************************************************
ossimMapProjection* 
ossimEpsgProjectionDatabase::createProjFromFormatBRecord(const DbEntry& record) const
{
   // Unfortunately, as of this writing, there is a disconnect between the Geotrans projection
   // parameters and those provided by EPSG database. In the meantime, to maintain functionality,
   // we intercept the EPSG code and programmatically arrive at the projection versus pulling
   // all the parameters out of the Db record.
   ossimStatePlaneProjectionInfo  info(record[B_NAME],
                                       record[B_CODE].toInt(),
                                       record[B_PROJ_TYPE],
                                       record[B_PARAM1],
                                       record[B_PARAM2],
                                       record[B_PARAM3],
                                       record[B_PARAM4],
                                       record[B_FALSE_EASTING].toDouble(),
                                       record[B_FALSE_NORTHING].toDouble(),
                                       record[B_UNITS],
                                       record[B_DATUM_CODE]);
   
   // NOTE: In order to avoid infinite recursion with this object, we initialized the PCS code in 
   // info to NULL to insure that the projection is instantiated directly (not via this class):
   ossimKeywordlist kwl;
   info.populateProjectionKeywords(kwl);
   kwl.remove(ossimKeywordNames::PCS_CODE_KW);
   ossimMapProjection* proj = PTR_CAST(ossimMapProjection,
      ossimMapProjectionFactory::instance()->createProjection(kwl));
   if (proj)
      proj->setPcsCode(record[B_CODE].toUInt32());

   return proj;
}

//*************************************************************************************************
// ### HACK ###
//! UTM projections as specified in the EPSG are indistinguishable from regular TM. Unfortunately
//! OSSIM does make a distinction. For the time being, parse the code and programmatically arrive 
//! at the UTM projection.
//*************************************************************************************************
ossimMapProjection* ossimEpsgProjectionDatabase::createProjFromUtmCode(ossim_uint32 code) const
{
   // This code originally found in deprecated ossimPcsCodeProjectionFactory authored by
   // David Burken.
   int type = code/100;
   int zone = code%100;
   ossimUtmProjection* proj = NULL;
   switch (type)
   {
   case 322:
      // utm, WGS72 (WGD), northern hemisphere
      if ( (zone > 0 ) && (zone < 61) )
      {
         proj = new ossimUtmProjection(*(ossimDatumFactory::instance()->wgs72()->ellipsoid()));
         proj->setDatum(ossimDatumFactory::instance()->wgs72());
         proj->setZone(zone);
         proj->setHemisphere('N');
      }
      break;

   case 323:
      // utm, WGS72 (WGD), southern hemisphere
      if ( (zone > 0) && (zone < 61) )
      {
         proj = new ossimUtmProjection(*(ossimDatumFactory::instance()->wgs72()->ellipsoid()));
         proj->setDatum(ossimDatumFactory::instance()->wgs72());
         proj->setZone(zone);
         proj->setHemisphere('S');
      }
      break;

   case 326:
      // utm, WGS84 (WGE), northern hemisphere
      if ( (zone > 0) && (zone < 61) )
      {
         proj = new ossimUtmProjection(*(ossimDatumFactory::instance()->wgs84()->ellipsoid()));
         proj->setDatum(ossimDatumFactory::instance()->wgs84());
         proj->setZone(zone);
         proj->setHemisphere('N');
      }
      break;

   case 327:
      // utm, WGS84 (WGE), southern hemisphere
      if ( (zone > 0) && (zone < 61) )
      {
         proj = new ossimUtmProjection(*(ossimDatumFactory::instance()->wgs84()->ellipsoid()));
         proj->setDatum(ossimDatumFactory::instance()->wgs84());
         proj->setZone(zone);
         proj->setHemisphere('S');
      }
      break;

   case 267:
      // utm, "NAS-C", northern hemisphere
      // Only UTM NAD27 North zones 3 to 22 are in the 267xx range...
      // 26729 through 26803 handled by state plane factory.
      //---
      if ( (code > 26702) && (code < 26723) )
      {
         proj = new ossimUtmProjection(*(ossimDatumFactory::instance()->
            create(ossimString("NAS-C"))->ellipsoid()));
         proj->setDatum(ossimDatumFactory::instance()->create(ossimString("NAS-C")));
         proj->setZone(zone);
         proj->setHemisphere('N');
      }
      break;

   case 269:
      // utm, "NAR-C", northern hemisphere
      // Only UTM NAD83 North zones 3 to 23 are in the 269xx range...
      // 26929 through 26998 handled by state plane factory.
      if ( (code > 26902) && (code < 26924) )
      {
         proj = new ossimUtmProjection(*(ossimDatumFactory::instance()->
            create(ossimString("NAR-C"))->ellipsoid()));
         proj->setDatum(ossimDatumFactory::instance()->create(ossimString("NAR-C")));
         proj->setZone(zone);
         proj->setHemisphere('N');
      }
      break;

   case 248:
      // Provisional S. American 1956, 24818 through 24880
      if ( (code > 24817) && (code < 24881) )
      {
         proj = new ossimUtmProjection(*(ossimDatumFactory::instance()->
            create(ossimString("PRP-M"))->ellipsoid()));
         proj->setDatum(ossimDatumFactory::instance()->create(ossimString("PRP-M")));
         if (zone > 60)
         {
            proj->setZone(zone - 60);
            proj->setHemisphere('S');
         }
         else
         {
            proj->setZone(zone);
            proj->setHemisphere('N');
         }
      }
      break;

   } // End of switch on code.

   if (proj)
      proj->setPcsCode(code);

   return proj;
}

//*************************************************************************************************
//! Throws an exception if datum code is not handled yet.
//*************************************************************************************************
void ossimEpsgProjectionDatabase::checkForUnhandledDatum(const ProjRecord& record) const
{
   if (!record.datumValid)
   {
      ossimString errMsg ("ossimEpsgProjectionDatabase -- "
         "EPSG database parsing of datum code has not been implemented for EPSG code=");
      errMsg += ossimString::toString(record.code);
      throw(ossimException(errMsg));
   }
}



