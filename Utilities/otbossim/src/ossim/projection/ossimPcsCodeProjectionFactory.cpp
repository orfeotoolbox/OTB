//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Class definition of projection factory to return projection from
// a Projection Coordinate System (PCS) code.
//
// For current documentation on pcs codes see:
// 
// http://www.remotesensing.org/geotiff/spec/geotiff6.html#6.3.3.1
// 
//----------------------------------------------------------------------------
//  $Id: ossimPcsCodeProjectionFactory.cpp 16470 2010-02-01 19:50:13Z gpotts $

#include <fstream>
#include <sstream>

#include <ossim/projection/ossimPcsCodeProjectionFactory.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/projection/ossimCylEquAreaProjection.h>
#include <ossim/projection/ossimMercatorProjection.h>
#include <ossim/projection/ossimSinusoidalProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimAlbersProjection.h>

#include <ossim/base/ossimPreferences.h>

static const ossimTrace
traceDebug(ossimString("ossimPcsCodeProjectionFactory:debug"));

#if OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimPcsCodeProjectionFactory.cpp 16470 2010-02-01 19:50:13Z gpotts $";
#endif

#ifndef ABS
#  define ABS(x)        ((x<0) ? (-1*(x)) : x)
#endif

ossimPcsCodeProjectionFactory* ossimPcsCodeProjectionFactory::theInstance = 0;

static const ossimString KEYS[] = {"COORD_REF_SYS_CODE",	
                                  "COORD_REF_SYS_NAME",	
                                  "FALSE_EASTING",	
                                  "FALSE_NORTHING",	
                                  "PARAMETER_VALUE_1",
                                  "PARAMETER_VALUE_2",	
                                  "PARAMETER_VALUE_3",	
                                  "PARAMETER_VALUE_4",	
                                  "UNIT"};
                                 

ossimPcsCodeProjectionFactory::ossimPcsCodeProjectionFactory()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPcsCodeProjectionFactory::ossimPcsCodeProjectionFactory"
         << " entered..." << std::endl;
#if OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif
   }

   ossimString regExpression =  ossimString("pcs_csv");

   std::vector<ossimString> keys = ossimPreferences::instance()->
     preferencesKWL().getSubstringKeyList( regExpression );

   std::vector<ossimString>::const_iterator i = keys.begin();
   while ( i != keys.end() )
   {
     const char* lookup = ossimPreferences::instance()->
       preferencesKWL().find( (*i).c_str() );

     if (lookup)
     {
       ossimFilename f = lookup;
       if (f.exists())
       {
         if (isFileInList(f) == false) // Check for duplicate.
         {
           if (isValidCsvFile(f))
           {
             theCsvFiles.push_back(f);
           }
         }
       }
     }
     ++i;
   }
}

ossimPcsCodeProjectionFactory* ossimPcsCodeProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimPcsCodeProjectionFactory;
   }

   return (ossimPcsCodeProjectionFactory*) theInstance;
}

ossimProjection*
ossimPcsCodeProjectionFactory::createProjection(const ossimFilename& filename,
                                                ossim_uint32 entryIdx)const
{
   if(!filename.exists())
   {
      return NULL;
   }

   // Check for external geometry file.
   ossimProjection* proj = createProjectionFromGeometryFile(filename,
                                                            entryIdx);
   if (proj)
   {
      return proj;
   }

   ossimFilename geomFile = filename;
   geomFile = geomFile.setExtension("geom");
   if(!geomFile.exists())
   {
      return NULL;
   }
   
   ossimKeywordlist kwl;
   if(kwl.addFile(geomFile))
   {
      return createProjection(kwl);
   }

   return NULL;
}

ossimProjection* ossimPcsCodeProjectionFactory::createProjection(
   const ossimKeywordlist &keywordList, const char *prefix) const
{
   ossimProjection *result=NULL;
	
   const char *lookup = keywordList.find(prefix,
                                         ossimKeywordNames::PCS_CODE_KW);
   if(lookup)
   {
      result = createProjection(ossimString(lookup).trim());
      if(result)
      {
         result->loadState(keywordList, prefix);
      }
   }
   return result;
}

ossimProjection* ossimPcsCodeProjectionFactory::createProjection(
   const ossimString &name) const
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPcsCodeProjectionFactory::createProjection entered..."
         << "\nname:  " << name << endl;
   }
   if(name.empty())
   {
      return 0;
   }
   ossimProjection* result = NULL;
   if(isalpha(*name.begin()))
   {
      return 0;
   }
   ossim_int32 code = name.toInt32();

   //---
   // Divide the code by 100. Then check for a known type.  If it is a
   // utm projection the last two digits represent the zone.
   //---
   int type = code/100;
   int zone = code%100;
   
   switch (type)
   {
      case 322:
      {
         //---
         // utm, WGS72 (WGD), northern hemisphere
         // All 60 zones handled.
         //---
         if ( (zone > 0 ) && (zone < 61) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        wgs72()->ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           wgs72());
            proj->setZone(zone);
            proj->setHemisphere('N');
            result = proj;
            return result;
         }
         break;
      }
         
      case 323:
      {
         //---
         // utm, WGS72 (WGD), southern hemisphere
         // All 60 zones handled.
         //---
         if ( (zone > 0) && (zone < 61) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        wgs72()->ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           wgs72());
            proj->setZone(zone);
            proj->setHemisphere('S');
            result = proj;
            return result;
         }
         break;
      }
      
      case 326:
      {
         //---
         // utm, WGS84 (WGE), northern hemisphere
         // All 60 zones hadled.
         //---
         if ( (zone > 0) && (zone < 61) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        wgs84()->ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           wgs84());
            proj->setZone(zone);
            proj->setHemisphere('N');
            result = proj;
            return result;
         }
         break;
      }
      
      case 327:
      {
         //---
         // utm, WGS84 (WGE), southern hemisphere
         // All 60 zones handled.
         //---
         if ( (zone > 0) && (zone < 61) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        wgs84()->ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           wgs84());
            proj->setZone(zone);
            proj->setHemisphere('S');
            result = proj;
            return result;
         }
         break;
      }
      
      case 267:
      {
         //---
         // utm, "NAS-C", northern hemisphere
         // Only UTM NAD27 North zones 3 to 22 are in the 267xx range...
         // 26729 through 26803 handled by state plane factory.
         //---
         if ( (code > 26702) && (code < 26723) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        create(ossimString("NAS-C"))->
                                        ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           create(ossimString("NAS-C")));
            proj->setZone(zone);
            proj->setHemisphere('N');
            result = proj;
            return result;
         }
         break;
      }
         
      case 269:
      {
         //---
         // utm, "NAR-C", northern hemisphere
         // Only UTM NAD83 North zones 3 to 23 are in the 269xx
         // range...
         // 26929 through 26998 handled by state plane factory.
         //---
         if ( (code > 26902) && (code < 26924) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        create(ossimString("NAR-C"))->
                                        ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           create(ossimString("NAR-C")));
            proj->setZone(zone);
            proj->setHemisphere('N');
            result = proj;
            return result;
         }
         break;
      }
      
      case 248:
      {
         //---
         // Provisional S. American 1956
         // 24818 through 24880
         //---
         if ( (code > 24817) && (code < 24881) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        create(ossimString("PRB-M"))->
                                        ellipsoid()));
            
            proj->setDatum(ossimDatumFactory::instance()->
                           create(ossimString("PRB-M")));
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
            result = proj;
            return result;
         }
         break;
      }
      
   } // End of switch on code.

   return result;
}

ossimObject* ossimPcsCodeProjectionFactory::createObject(
   const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimPcsCodeProjectionFactory::createObject(
   const ossimKeywordlist& kwl,
   const char* prefix)const
{
   return createProjection(kwl, prefix);
}

bool ossimPcsCodeProjectionFactory::isFileInList(
  const ossimFilename& file) const
{
   OpenThreads::ScopedReadLock lock(theMutex);
   std::vector<ossimFilename>::const_iterator i = theCsvFiles.begin();
   while (i != theCsvFiles.end())
   {
      if (file == (*i))
      {
         return true;
      }
      ++i;
   }
   return false;
}

bool ossimPcsCodeProjectionFactory::isValidCsvFile(
   const ossimFilename& file) const
{
   // open file
   std::ifstream in( file.c_str() );
   if (!in)
   {
      return false;
   }
   
   // Grab the first line.
   std::string line;
   std::getline(in, line);
   
   if (!in)
   {
      return false;
   }
   
   // Split the line between commas stripping quotes.
   std::vector<ossimString> v;
   splitLine(line, v);
   
   // Check the size
   if (v.size() != KEYS_SIZE)
   {
      return false;
   }
   
   for (ossim_uint32 i = 0; i < KEYS_SIZE; ++i)
   {
      if (v[i] != KEYS[i])
      {
         return false;
      }
   }
   return true;
}

void ossimPcsCodeProjectionFactory::splitLine(
   const std::string& line, std::vector<ossimString> &result) const
{
   //---
   // Splits line between commas, stripping quotes and making empty string
   // on double comma.
   //
   // Sample line:  ",,foo,you,,,\"too\""
   //---
   
   if (result.size())
   {
      result.clear();
   }
   
   if (!line.size())
   {
      return;
   }
   
   // Constants used.
   const char DELIM = ',';
   const std::streamsize SIZE = 64;
   
   // Buffer for reads.
   char s[SIZE];
   s[SIZE-1] = '\0';
   
   // tmp
   ossimString os;
   
   // Open a stream to the line.
   std::istringstream istr(line);
   while (istr.good())
   {
      // We can have lines with empty fields like foo,,,you so peek for it.
      if (istr.peek() != ',')
      {
         istr.get(s, SIZE, DELIM);
         os = s;
         
         if (istr.peek() == ',')
         {
            istr.ignore();  // Gobble the comma.
         }
         
         // Get rid of any trailing quote.
         std::string::size_type pos = os.find_last_of('"');
         if (pos != std::string::npos)
         {
            os.erase(pos, 1);
         }
         
         // Get rid of leading quote if any.
         pos = os.find_first_of('"');
         if (pos != std::string::npos)
         {
            os.erase(pos, 1);
         }
         
         // Trim leading trailing blanks.
         os.trim();
      }
      else
      {
         // Gobble the comma.
         istr.ignore(); 
         
         // Make an empty string on double comma.
         os = "";
      }
      
      result.push_back(os);
   }
}

void ossimPcsCodeProjectionFactory::getTypeNameList(
   std::vector<ossimString>& /* typeList */)const
{
}

ossim_uint16 ossimPcsCodeProjectionFactory::getPcsCodeFromProjection(
   const ossimMapProjection* proj) const
{
   ossim_uint16 pcsCode = 0;
   
   if (!proj)
   {
      return pcsCode;
   }
   
   ossimString ossimProj = proj->getClassName();
   
   if (ossimProj == "ossimUtmProjection")
   {
      ossimUtmProjection* utm = PTR_CAST(ossimUtmProjection, proj);
      if (utm)
      {
         ossim_uint16 mapZone   = static_cast<ossim_uint16>(utm->getZone());
         ossimString hemisphere = utm->getHemisphere();
         ossimString datumCode  = utm->getDatum()?utm->getDatum()->code():"s";

         if (datumCode == "WGE")
         {
            if (hemisphere == "N") // Northern hemisphere.
            {
               pcsCode = 32600 + mapZone;
            }
            else // Southern hemisphere.
            {
               pcsCode = 32700 + mapZone;
            }
         }
         else if (datumCode == "WGD")
         {
            if (hemisphere == "N") // Northern hemisphere.
            {
               pcsCode = 32200 + mapZone;
            }
            else // Southern hemisphere.
            {
               pcsCode = 32300 + mapZone;
            }
         }
         else if (datumCode.contains("NAS"))
         {
            if (hemisphere == "N") // Northern hemisphere.
            {
               pcsCode = 26700 + mapZone;
            }
            else // Southern hemisphere.
            {
               pcsCode = 32000 + mapZone;
            }
         }
         else if (datumCode.contains("NAR"))
         {
            if (hemisphere == "N") // Northern hemisphere.
            {
               pcsCode = 26900 + mapZone;
            }
            else // Southern hemisphere.
            {
               pcsCode = 32100 + mapZone;
            }
         }
      }
      return pcsCode;
   }
   else if(ossimProj == "ossimCylEquAreaProjection")
   {
      ossimCylEquAreaProjection* cyle = PTR_CAST(ossimCylEquAreaProjection, proj);
      if (cyle)
      {
         double originLat = cyle->getOrigin().lat;
         double centerMeridian = cyle->getOrigin().lon;
         double falseEast = cyle->getFalseEasting();
         double falseNorth = cyle->getFalseNorthing();

         double para1 = cyle->getStandardParallel1();
         double para2 = cyle->getStandardParallel2();

         if (findLine(falseEast, falseNorth, centerMeridian, para1, para2, originLat, "ossimCylEquAreaProjection", pcsCode))
         {
            return pcsCode;
         }
      }
   }
   else if( (ossimProj == "ossimLambertConformalConicProjection") ||
            (ossimProj == "ossimAlbersProjection") )
   {
      ossimLambertConformalConicProjection* lamb = PTR_CAST(ossimLambertConformalConicProjection, proj);
      ossimAlbersProjection* alber = PTR_CAST(ossimAlbersProjection, proj);
      if (lamb)
      {
         double originLat = lamb->getOrigin().lat;
         double centerMeridian = lamb->getOrigin().lon;
         double falseEast = lamb->getFalseEasting();
         double falseNorth = lamb->getFalseNorthing();

         double para1 = lamb->getStandardParallel1();
         double para2 = lamb->getStandardParallel2();
         if (findLine(falseEast, falseNorth, centerMeridian, para1, para2, originLat, "ossimLambertConformalConicProjection", pcsCode))
         {
            return pcsCode;
         }
      }
      else if (alber)
      {
         double originLat = alber->getOrigin().lat;
         double centerMeridian = alber->getOrigin().lon;
         double falseEast = alber->getFalseEasting();
         double falseNorth = alber->getFalseNorthing();

         double para1 = alber->getStandardParallel1();
         double para2 = alber->getStandardParallel2();
         if (findLine(falseEast, falseNorth, centerMeridian, para1, para2, originLat, "ossimAlbersProjection", pcsCode))
         {
            return pcsCode;
         }
      }
   }
   else if(ossimProj == "ossimMercatorProjection")
   {
      ossimMercatorProjection* merc = PTR_CAST(ossimMercatorProjection, proj);
      if (merc)
      {
         double originLat = merc->getOrigin().lat;
         double centerMeridian = merc->getOrigin().lon;
         double falseEast = merc->getFalseEasting();
         double falseNorth = merc->getFalseNorthing();

         if (findLine(falseEast, falseNorth, centerMeridian, 0, 0, originLat, "ossimMercatorProjection", pcsCode))
         {
            return pcsCode;
         }
      }
   }
   else if(ossimProj == "ossimSinusoidalProjection")
   {
      ossimSinusoidalProjection* sinu = PTR_CAST(ossimSinusoidalProjection, proj);
      if (sinu)
      {
         double originLat = sinu->getOrigin().lat;
         double centerMeridian = sinu->getOrigin().lon;
         double falseEast = sinu->getFalseEasting();
         double falseNorth = sinu->getFalseNorthing();

         if (findLine(falseEast, falseNorth, centerMeridian, 0, 0, originLat, "ossimSinusoidalProjection", pcsCode))
         {
            return pcsCode;
         }
      }
   }
   else if(ossimProj == "ossimTransMercatorProjection")
   {
      ossimTransMercatorProjection* tran = PTR_CAST(ossimTransMercatorProjection, proj);
      if (tran)
      {
         double scalFactor = tran->getScaleFactor();
         double originLat = tran->getOrigin().lat;
         double centerMeridian = tran->getOrigin().lon;
         double falseEast = tran->getFalseEasting();
         double falseNorth = tran->getFalseNorthing();

         if (findLine(falseEast, falseNorth, centerMeridian, scalFactor, 0, originLat, "ossimTransMercatorProjection", pcsCode))
         {
            return pcsCode;
         }
      }
   }
   else 
   {
      double originLat = proj->getOrigin().lat;
      double centerMeridian = proj->getOrigin().lon;

      double falseEast = proj->getFalseEasting();
      double falseNorth = proj->getFalseNorthing();

      if (findLine(falseEast,falseNorth, centerMeridian, 0, 0, originLat, "", pcsCode))
      {
         return pcsCode;
      }
   }

   return pcsCode;
}

ossim_uint16 ossimPcsCodeProjectionFactory::getPcsCodeFromProjectionName(const ossimString projName) const
{
  ossim_uint16 pcsCode = 0;

  if (findLine(projName, pcsCode))
  {
    return pcsCode;
  }

  return pcsCode;
}

bool ossimPcsCodeProjectionFactory::findLine(const double param1, 
                                             const double param2, 
                                             const double param3, 
                                             const double param4,
                                             const double param5, 
                                             const double param6,
                                             ossimString ossimProj,
                                             ossim_uint16& pcsCode) const
{
   OpenThreads::ScopedReadLock lock(theMutex);
   std::string savedLine;

   // Iterate throught the cvs files to try and find pcs code.
   std::vector<ossimFilename>::const_iterator i = theCsvFiles.begin();
   while (i != theCsvFiles.end())
   {
      // open file
      std::ifstream in( (*i).c_str() );
      if (!in)
      {
         continue; // Go to next iteration(file) if there is one.
      }

      // used throughout
      std::string line;
      std::vector<ossimString> result;

      // Eat the first line.
      std::getline(in, line);

      // Iterate through the lines of file.
      while(in.good())
      {
         // Read in a line.
         std::getline(in, line);

         // Split the line between commas stripping quotes.
         splitLine(line, result);

         // Check the size
         if (result.size() != KEYS_SIZE)
         {
            continue; // next line
         }

         double paramTmp1 = result[FALSE_EASTING].toDouble();
         double paramTmp2 = result[FALSE_NORTHING].toDouble();
         double paramTmp3 = result[PARAMETER_VALUE_1].toDouble();
         double paramTmp4 = result[PARAMETER_VALUE_2].toDouble();
         double paramTmp5 = result[PARAMETER_VALUE_3].toDouble();
         double paramTmp6 = result[PARAMETER_VALUE_4].toDouble();

         if(ossimProj == "ossimCylEquAreaProjection")
         {
            if (ABS(param1-paramTmp1) < 0.01 && 
                ABS(param2-paramTmp2) < 0.01 && 
                ABS(param3-paramTmp3) < 0.01 &&
                ABS(param4-paramTmp4) < 0.01 &&
                ABS(param5-paramTmp5) < 0.01 && 
                ABS(param6-paramTmp6) < 0.01)
            {
               pcsCode = result[COORD_REF_SYS_CODE].toInt16();
               return true;
            }
         }
         else if( (ossimProj == "ossimLambertConformalConicProjection") ||
                  (ossimProj == "ossimAlbersProjection") )
         {
            if (ABS(param1-paramTmp1) < 0.01 && 
                ABS(param2-paramTmp2) < 0.01 && 
                ABS(param3-paramTmp3) < 0.01 &&
                ABS(param4-paramTmp4) < 0.01 &&
                ABS(param5-paramTmp5) < 0.01 && 
                ABS(param6-paramTmp6) < 0.01)
            {
               pcsCode = result[COORD_REF_SYS_CODE].toInt16();
               return true;
            }
         }
         else if(ossimProj == "ossimMercatorProjection")
         {
            if (ABS(param1-paramTmp1) < 0.01 && 
                ABS(param2-paramTmp2) < 0.01 && 
                ABS(param5-paramTmp5) < 0.01 && 
                ABS(param6-paramTmp6) < 0.01)
            {
               pcsCode = result[COORD_REF_SYS_CODE].toInt16();
               return true;
            }
         }
         else if(ossimProj == "ossimSinusoidalProjection")
         {
            if (ABS(param1-paramTmp1) < 0.01 && 
                ABS(param2-paramTmp2) < 0.01 && 
                ABS(param5-paramTmp5) < 0.01 && 
                ABS(param6-paramTmp6) < 0.01)
            {
               pcsCode = result[COORD_REF_SYS_CODE].toInt16();
               return true;
            }
         }
         else if(ossimProj == "ossimTransMercatorProjection")
         {
            if (ABS(param1-paramTmp1) < 0.01 && 
                ABS(param2-paramTmp2) < 0.01 && 
                ABS(param3-paramTmp3) < 0.01 && 
                ABS(param5-paramTmp5) < 0.01 && 
                ABS(param6-paramTmp6) < 0.01)
            {
               pcsCode = result[COORD_REF_SYS_CODE].toInt16();
               return true;
            }
         }
         else 
         {
            if (ABS(param1-paramTmp1) < 0.01 && 
                ABS(param2-paramTmp2) < 0.01 && 
                ABS(param5-paramTmp5) < 0.01 && 
                ABS(param6-paramTmp6) < 0.01)
            {
               pcsCode = result[COORD_REF_SYS_CODE].toInt16();
               return true;
            }
         }
      }
      ++i; // go to next csv file
   }

   return false;
}

bool ossimPcsCodeProjectionFactory::findLine(const ossimString& name, 
                                             ossim_uint16& pcsCode) const
{
  OpenThreads::ScopedReadLock lock(theMutex);
  std::string savedLine;
  // bool bSavedLine = false;
  // Iterate throught the cvs files to try and find pcs code.
  std::vector<ossimFilename>::const_iterator i = theCsvFiles.begin();
  while (i != theCsvFiles.end())
  {
    // open file
    std::ifstream in( (*i).c_str() );
    if (!in)
    {
      continue; // Go to next iteration(file) if there is one.
    }

    // used throughout
    std::string line;
    std::vector<ossimString> result;

    // Eat the first line.
    std::getline(in, line);

    // Iterate through the lines of file.
    while(in.good())
    {
      // Read in a line.
      std::getline(in, line);

      // Split the line between commas stripping quotes.
      splitLine(line, result);

      // Check the size
      if (result.size() != KEYS_SIZE)
      {
        continue; // next line
      }

      if (result[COORD_REF_SYS_NAME] == name)
      {
        pcsCode = result[COORD_REF_SYS_CODE].toInt16();
        return true;
      }
    }
    ++i; // go to next csv file
  }

  return false;
}
