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
//  $Id: ossimEpsgProjectionFactory.cpp 19879 2011-07-30 16:21:50Z dburken $

#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimOrthoGraphicProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <sstream>

ossimEpsgProjectionFactory* ossimEpsgProjectionFactory::m_instance = 0;

//*************************************************************************************************
//! Implements singleton pattern
//*************************************************************************************************
ossimEpsgProjectionFactory* ossimEpsgProjectionFactory::instance()
{
   if(!m_instance)
      m_instance = new ossimEpsgProjectionFactory;
   return m_instance;
}

//*************************************************************************************************
//! Constructor loads all SRS CSV files specified in the ossim prefs
//*************************************************************************************************
ossimEpsgProjectionFactory::ossimEpsgProjectionFactory()
{
   m_projDatabase = ossimEpsgProjectionDatabase::instance();
}

//*************************************************************************************************
//! From keywordlist
//*************************************************************************************************
ossimProjection* ossimEpsgProjectionFactory::createProjection(const ossimKeywordlist &keywordList,
                                                             const char *prefix) const
{
    ossimProjection* proj = 0;

   // Look for a PCS (Projected Coordinate System) code entry and construct the basic projection.
   // This is the bootstrap for creating the fully-initialized proj:
   ossimString proj_spec = keywordList.find(prefix, ossimKeywordNames::PCS_CODE_KW);
   if(!proj_spec.empty())
      proj = createProjection(proj_spec);
  
   if (!proj)
   {
      // An alternate way of specifying an EPSG projection is by its database name:
      proj_spec = keywordList.find(prefix, ossimKeywordNames::PROJECTION_KW);
      proj = createProjection(proj_spec);
   }

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
// This is the principal factory method. It accepts a string in one of three formats:
//
//   1. <group>:<code>, for example "EPSG:32615"
//   2. <code>, for example "32615", this is the CRS code ID for the projerection.
//   3. AUTO:<comma-separated spec>, for specifying custom user-defined codes.
//
// Both projected and geographic (Platte Carre) codes are handled.
//
// IMPORTANT NOTE: Image tie-points cannot be conveyed by an EPSG projection code. The projection
// created here will not be fully initialized for use in rendering imagery.
//*************************************************************************************************
ossimProjection* ossimEpsgProjectionFactory::createProjection(const ossimString &spec) const
{
   std::vector<ossimString> split_line;
   
   // Intercept custom codes here before resorting to the database. This really shouldn't be in this
   // class but nobody else is parsing for it, so leave it here for now.
   if (ossimString(spec).downcase().contains("auto"))
   {
      split_line = spec.after(":").explode(",");
      return createProjFromAutoCode(split_line);
   }

   // Strip commonly found or bar '|' from end if present.
   ossimString s = spec;
   s.trim(ossimString("|"));

   // Otherwise, pass along the request to the database object for record search:
   return m_projDatabase->findProjection(s);
}

//*************************************************************************************************
//! When the CRS is specified with the "AUTO:<spec>" code format, this method is invoked to
//! parse the <spec> and produce a projection (or NULL if spec invalid):
//*************************************************************************************************
ossimMapProjection* 
ossimEpsgProjectionFactory::createProjFromAutoCode(const std::vector<ossimString>& spec) const
{
   if (spec.size() != 4)
      return 0;

   ossim_uint32 code = spec[0].toUInt32();
   ossimGpt origin (spec[3].toDouble(), spec[2].toDouble()) ;

   // Only a few AUTO codes are considered:
   switch(code)
   {
      case 42001:
      {
         ossimUtmProjection* utm = new ossimUtmProjection;
         utm->setZone(origin);
         utm->setHemisphere(origin);
         utm->setOrigin(origin);
         utm->update();
         utm->setPcsCode(42001);
         return utm;
      }
      
      case 42002:
      {
         ossimTransMercatorProjection* transMerc = new ossimTransMercatorProjection;
         transMerc->setFalseNorthing(origin.latd()>=0.0?0.0:10000000.0);
         transMerc->setOrigin(ossimGpt(0.0, origin.lond()));
         transMerc->setScaleFactor(.9996);
         transMerc->update();
         transMerc->setPcsCode(42002);
         return transMerc;
      }
      
      case 42003:
      {
         ossimOrthoGraphicProjection* ortho = new ossimOrthoGraphicProjection;
         ortho->setOrigin(origin);
         ortho->update();
         ortho->setPcsCode(42003);
         return ortho;
      }
      
      case 42004:
      {
         ossimEquDistCylProjection* geographic = new ossimEquDistCylProjection;
         geographic->setOrigin(origin);
         geographic->update();
         geographic->setPcsCode(42004);
         return geographic;
      }
   }
   return 0;
}

//*************************************************************************************************
ossimObject* ossimEpsgProjectionFactory::createObject(const ossimString& typeName)const
{
   return createProjection(typeName);
}

//*************************************************************************************************
ossimObject* ossimEpsgProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
{
   return createProjection(kwl, prefix);
}

//*************************************************************************************************
//! STUB. Not implemented
//*************************************************************************************************
ossimProjection* ossimEpsgProjectionFactory::createProjection(const ossimFilename& /* filename */,
                                                              ossim_uint32 /* entryIdx */)const
{
   return 0;
}

//*************************************************************************************************
//! This returns the type name of all objects in all factories. This is the name used to 
//! construct the objects dynamically and this name must be unique.
//*************************************************************************************************
void ossimEpsgProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList) const
{
   m_projDatabase->getProjectionsList(typeList);
   return;
}

