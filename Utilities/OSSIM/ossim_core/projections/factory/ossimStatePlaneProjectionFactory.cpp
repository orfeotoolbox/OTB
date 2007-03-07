//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimStatePlaneProjectionFactory.cpp,v 1.15 2005/03/30 17:12:24 dburken Exp $
#include "ossimStatePlaneProjectionFactory.h"
#include "projections/ossimStatePlaneProjectionInfo.h"
#include "projections/factory/ossimMapProjectionFactory.h"
#include "base/common/ossimKeywordNames.h"
#include "base/data_types/ossimFilename.h"
#include "projections/ossimProjection.h"
using namespace std;

ossimStatePlaneProjectionFactory* ossimStatePlaneProjectionFactory::theInstance = NULL;

ossimStatePlaneProjectionFactory::~ossimStatePlaneProjectionFactory()
{
   theInstance = NULL;
}

ossimStatePlaneProjectionFactory* ossimStatePlaneProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimStatePlaneProjectionFactory;

      theInstance->initializeMap();
   }

   return theInstance;
}

ossimProjection* ossimStatePlaneProjectionFactory::createProjection(
   const ossimFilename& filename, ossim_uint32 entryIdx)const
{
   // See if there is an external geomtry.
   return createProjectionFromGeometryFile(filename, entryIdx);
}

ossimProjection* ossimStatePlaneProjectionFactory::createProjection(
   const ossimString& name)const
{
   ossimProjection* result = NULL;

   std::map<ossim_int32, ossimStatePlaneProjectionInfo*>::const_iterator current;
   ossim_int32 c = name.toLong();
   current = theStatePlaneInfoMap.find(c);

   if(current != theStatePlaneInfoMap.end())
   {
      ossimKeywordlist kwl2;
      
      (*current).second->populateProjectionKeywords(kwl2);

      result = ossimMapProjectionFactory::instance()->createProjection(kwl2);
   }

   if(!result)
   {
      std::map<ossimString, ossimStatePlaneProjectionInfo*>::const_iterator currentName;
      currentName = theStatePlaneInfoNameMap.find(c);
      if(currentName != theStatePlaneInfoNameMap.end())
      {
         ossimKeywordlist kwl2;
         
         (*currentName).second->populateProjectionKeywords(kwl2);
         
         result = ossimMapProjectionFactory::instance()->createProjection(kwl2);
      }
   }
   
   return result;
}

ossimProjection* ossimStatePlaneProjectionFactory::createProjection(const ossimKeywordlist& kwl,
                                                                    const char* prefix)const
{
   ossimProjection* result = NULL;
   const char* pcs_code = kwl.find(prefix,
                                   ossimKeywordNames::PCS_CODE_KW);
   const char* type_kw  = kwl.find(prefix,
                                   ossimKeywordNames::TYPE_KW);
   if(pcs_code)
   {
      std::map<ossim_int32, ossimStatePlaneProjectionInfo*>::const_iterator current;
      ossim_int32 code = ossimString(pcs_code).toLong();
      current = theStatePlaneInfoMap.find(code);

      if(current != theStatePlaneInfoMap.end())
      {
         ossimKeywordlist kwl2 = kwl;

         (*current).second->populateProjectionKeywords(kwl2, prefix);

         result = ossimMapProjectionFactory::instance()->createProjection(kwl2, prefix);
	 
      }
   }
   else if(type_kw)
   {
      std::map<ossimString, ossimStatePlaneProjectionInfo*>::const_iterator current;
      current = theStatePlaneInfoNameMap.find(ossimString(type_kw));
      if(current != theStatePlaneInfoNameMap.end())
      {
         ossimKeywordlist kwl2 = kwl;

         (*current).second->populateProjectionKeywords(kwl2, prefix);

         result = ossimMapProjectionFactory::instance()->createProjection(kwl2, prefix);
      }
   }
   
   return result;
}

const ossimStatePlaneProjectionInfo* ossimStatePlaneProjectionFactory::getInfo(ossim_int32 pcsCode)const
{
   std::map<ossim_int32, ossimStatePlaneProjectionInfo*>::const_iterator current;
   current = theStatePlaneInfoMap.find(pcsCode);
   
   if(current != theStatePlaneInfoMap.end())
   {
      return (*current).second;
   }

   return (const ossimStatePlaneProjectionInfo*)NULL;
}

const ossimStatePlaneProjectionInfo* ossimStatePlaneProjectionFactory::getInfo(const ossimString& name)const
{
   std::map<ossimString, ossimStatePlaneProjectionInfo*>::const_iterator current;
   current = theStatePlaneInfoNameMap.find(name);
   
   if(current != theStatePlaneInfoNameMap.end())
   {
      return (*current).second;
   }

   return (const ossimStatePlaneProjectionInfo*)NULL;
}



std::list<ossimString> ossimStatePlaneProjectionFactory::getList()const
{
   list<ossimString> result;

   std::map<ossim_int32, ossimStatePlaneProjectionInfo*>::const_iterator current = theStatePlaneInfoMap.begin();

   while(current != theStatePlaneInfoMap.end())
   {
      result.push_back((*current).second->name());
      
      ++current;
   }
   
   return result;
}

ossimObject* ossimStatePlaneProjectionFactory::createObject(const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimStatePlaneProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                                            const char* prefix)const
{
   return createProjection(kwl, prefix);
}
   
void ossimStatePlaneProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   std::map<ossim_int32, ossimStatePlaneProjectionInfo*>::const_iterator current = theStatePlaneInfoMap.begin();

   while(current != theStatePlaneInfoMap.end())
   {
      typeList.push_back((*current).second->name());
      
      ++current;
   }
//   typeList.push_back("ossimStatePlaneProjection");
}

bool ossimStatePlaneProjectionFactory::addStatePlaneInfo(ossimStatePlaneProjectionInfo* info)
{
   if(info)
   {
      theStatePlaneInfoMap.insert(make_pair((ossim_int32)info->code(), 
                                            info));
      theStatePlaneInfoNameMap.insert(make_pair(info->name(), 
                                                info));

      return true;
   }
   
   return false;
   
}

void ossimStatePlaneProjectionFactory::initializeMap()
{
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alabama_East", 26729, "ossimTransMercatorProjection", "30 30 n", "85 50 w",
                                                       25000, 0, 500000, 0.0, "ft"));
   
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alabama_West", 26730, "ossimTransMercatorProjection", "30 n", "87 30 w",
                  15000, 0, 500000, 0.0, "ft"));
   
   // addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_1", 26731 UNSUPPORTED! (Hotine Mercator)

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_2", 26732, "ossimTransMercatorProjection", "54 n", "142 w", 10000, 0,
                  500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_3", 26733, "ossimTransMercatorProjection", "54 n", "146 w", 10000, 0,
                  500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_4", 26734, "ossimTransMercatorProjection", "54 n", "150 w", 10000, 0,
                  500000, 0.0, "ft")); 
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_5", 26735, "ossimTransMercatorProjection", "54 n", "154 w", 10000, 0,
                  500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_6", 26736, "ossimTransMercatorProjection", "54 n", "158 w", 10000, 0,
                  500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_7", 26737, "ossimTransMercatorProjection", "54 n", "162 w", 10000, 0,
                  700000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_8", 26738, "ossimTransMercatorProjection", "54 n", "166 w", 10000, 0,
                  500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_9", 26739, "ossimTransMercatorProjection", "54 n", "170 w", 10000, 0,
                  600000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Alaska_zone_10",  26740, "ossimLambertConformalConicProjection", "51 n", "176 w",
                  "51 50 n", "53 50 n", 3000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_California_I",    26741, "ossimLambertConformalConicProjection", "39 20", "122 w",
                  "40 n", "41 40 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_California_II",   26742, "ossimLambertConformalConicProjection", "37 40", "122 w",
                  "38 20", "39 50", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_California_III",  26743, "ossimLambertConformalConicProjection", "36 30 n", "120 30 w",
                  "37 04 n", "38 26 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_California_IV",   26744, "ossimLambertConformalConicProjection", "35 20 n", "119 w",
                  "36 n", "37 15 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_California_V",    26745, "ossimLambertConformalConicProjection", "33 30 n", "118 w",
                  "34 02 n", "35 28 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_California_VI",   26746, "ossimLambertConformalConicProjection", "32 10", "116 15 w",
                  "32 47 n", "33 53 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_California_VII",  26747, "ossimLambertConformalConicProjection", "34 08 n", "118 20 w",
                  "33 52 n", "34 25 n", 4186692.58, 4160926.74, "ft"));
                              
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Arizona_East",    26748, "ossimTransMercatorProjection", "31 n", "110 10 w",
                  10000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Arizona_Central", 26749, "ossimTransMercatorProjection", "31 n", "111 55 w",
                  10000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Arizona_West", 26750, "ossimTransMercatorProjection", "31 n", "113 45 w",
                  15000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Arkansas_North",  26751, "ossimLambertConformalConicProjection", "34 20 n", "92 w",
                  "34 56 n", "36 14 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Arkansas_South",  26752, "ossimLambertConformalConicProjection", "32 40 n", "92 w",
                  "33 18 n", "34 46 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Colorado_North",  26753, "ossimLambertConformalConicProjection", "39 20 n", "105 30 w",
                  "39 43 n", "40 47 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Colorado_Central",26754, "ossimLambertConformalConicProjection", "37 50 n", "105 30 w",
                  "38 27 n", "39 45 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Colorado_South",  26755, "ossimLambertConformalConicProjection", "36 40 n", "105 30 w",
                  "37 14 n", "38 26 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Connecticut",   26756, "ossimLambertConformalConicProjection", "40 50 n", "72 45 w",
                  "41 12 n", "41 52 n", 600000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Delaware",      26757, "ossimTransMercatorProjection", "38 n", "75 25 w",
                  200000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Florida_East",  26758, "ossimTransMercatorProjection", "24 20 n", "81 w",
                  17000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Florida_West",  26759, "ossimTransMercatorProjection", "24 20 n", "82 w",
                  17000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Florida_North", 26760, "ossimLambertConformalConicProjection", "29 n", "84 30 w",
                  "29 35 n", "30 45 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Hawaii_zone_1", 26761, "ossimTransMercatorProjection", "18 50 n", "155 30 w",
                  30000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Hawaii_zone_2", 26762, "ossimTransMercatorProjection", "20 20 n","156 40 w",
                  30000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Hawaii_zone_3", 26763, "ossimTransMercatorProjection", "21 10 n", "158 w",
                  100000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Hawaii_zone_4", 26764, "ossimTransMercatorProjection", "21 50 n", "159 30 w",
                  100000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Hawaii_zone_5", 26765, "ossimTransMercatorProjection", "21 40 n", "160 10 w",
                  0.0, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Georgia_East",  26766, "ossimTransMercatorProjection", "30 n", "82 10 w",
                  10000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Georgia_West",  26767, "ossimTransMercatorProjection", "30 n", "84 10 w",
                  10000, 0, 500000, 0.0, "ft")); 

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Idaho_East",    26768, "ossimTransMercatorProjection", "41 40 n", "112 10 w",
                  19000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Idaho_Central", 26769, "ossimTransMercatorProjection", "41 40 n", "114 w",
                  19000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Idaho_West",    26770, "ossimTransMercatorProjection", "41 40 n", "115 45 w",
                  15000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Illinois_East", 26771, "ossimTransMercatorProjection", "36 40 n", "88 20 w",
                  40000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Illinois_West", 26772, "ossimTransMercatorProjection", "36 40 n", "90 10 w",
                  17000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Indiana_East",  26773, "ossimTransMercatorProjection", "37 30 n", "85 40 w",
                  30000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Indiana_West",  26774, "ossimTransMercatorProjection", "37 30 n", "87 05 w",
                  30000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Iowa_North",    26775, "ossimLambertConformalConicProjection", "41 30 n", "93 30 w",
                  "42 04 n", "43 16 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Iowa_South",    26776, "ossimLambertConformalConicProjection", "40 n", "93 30 w",
                  "40 37 n", "41 47 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Kansas_North",  26777, "ossimLambertConformalConicProjection", "38 20 n", "98 00 w",
                  "38 43 n", "39 47 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Kansas_South",  26778, "ossimLambertConformalConicProjection", "36 40 n", "98 30 w",
                  "37 16 n", "38 34 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Kentucky_North",  26779, "ossimLambertConformalConicProjection", "37 30 n", "84 15 w",
                  "37 58 n", "38 58 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Kentucky_South",  26780, "ossimLambertConformalConicProjection", "36 20 n", "85 45 w",
                  "36 44 n", "37 56 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Louisiana_North", 26781, "ossimLambertConformalConicProjection", "30 40 n", "92 30 w",
                  "31 10 n", "32 40 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Louisiana_South", 26782, "ossimLambertConformalConicProjection", "28 40 n", "91 20 w",
                  "29 18 n", "30 42 n", 2000000, 0.0, "ft"));

//   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Louisiana_Offshore", 0, "ossimLambertConformalConicProjection", "25 40 n", "91 20 w",
//                                                       "26 10 n", "27 50 n", 2000000, 0.0, "ft"));


   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Maine_East", 26783, "ossimTransMercatorProjection", "43 50 n", "68 30 w",
                  10000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Maine_West", 26784, "ossimTransMercatorProjection", "42 50 n", "70 10 w",
                  30000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Maryland",   26785, "ossimLambertConformalConicProjection", "37 50 n", "77 00 w",
                  "38 18 n", "39 27 n", 800000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Massachusetts",   26786, "ossimLambertConformalConicProjection", "41 00 n", "71 30 w",
                  "41 43 n", "42 41 n", 600000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Massachusetts_Is",26787, "ossimLambertConformalConicProjection", "41 00 n", "70 30 w",
                  "41 17 n", "41 29 n", 200000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Michigan_North",  26788, "ossimLambertConformalConicProjection", "44 47 n", "87 w",
                  "45 29 n", "47 05 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Michigan_Central",26789, "ossimLambertConformalConicProjection", "43 19 n", "84 20 w",
                  "44 11 n", "45 42 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Michigan_South",  26790, "ossimLambertConformalConicProjection", "41 30 n", "84 20 w",
                  "42 06 n", "43 40 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Minnesota_North", 26791, "ossimLambertConformalConicProjection", "46 30 n", "93 06 w",
                  "47 02 n", "48 38 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Minnesota_Cent",  26792, "ossimLambertConformalConicProjection", "45 00 n", "94 15 w",
                  "45 37 n", "47 03 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Minnesota_South", 26793, "ossimLambertConformalConicProjection", "43 00 n", "94 00 w",
                  "43 47 n", "45 13 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Mississippi_East",26794, "ossimTransMercatorProjection", "29 40 n", "88 50 w",
                  25000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Mississippi_West",26795, "ossimTransMercatorProjection", "30 30 n", "90 20 w",
                  17000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Missouri_East",   26796, "ossimTransMercatorProjection", "35 50 n", "90 30 w",
                  15000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Missouri_Central",26797, "ossimTransMercatorProjection", "35 50 n", "92 30 w",
                  15000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Missouri_West",   26798, "ossimTransMercatorProjection", "36 10 n", "94 30 w",
                  17000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD_Michigan_Old_East", 26801, "ossimTransMercatorProjection", "41 30 n", "83 40 w",
                  17500, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD_Michigan_Old_Centr",26802, "ossimTransMercatorProjection", "41 30 n", "85 45 w",
                  11000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD_Michigan_Old_West", 26803, "ossimTransMercatorProjection", "41 30 n", "88 45 w",
                  11000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Montana_North",   32001, "ossimLambertConformalConicProjection", "47 00 n", "109 30 w",
                  "47 51 n", "48 43 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Montana_Central", 32002, "ossimLambertConformalConicProjection", "45 50 n", "109 30 w",
                  "46 27 n", "47 53 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Montana_South",   32003, "ossimLambertConformalConicProjection", "44 00 n", "109 30 w",
                  "44 52 n", "46 24 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Nebraska_North",  32005, "ossimLambertConformalConicProjection", "41 20 n", "100 00 w",
                  "41 51 n", "42 49 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Nebraska_South",  32006, "ossimLambertConformalConicProjection", "39 40 n", "99 30 w",
                  "40 17 n", "41 43 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Nevada_East",     32007, "ossimTransMercatorProjection", "34 45 n", "115 35 w",
                  10000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Nevada_Central",  32008, "ossimTransMercatorProjection", "34 45 n", "116 40 w",
                  10000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Nevada_West",     32009, "ossimTransMercatorProjection", "34 45 n", "118 35 w",
                  10000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_New_Hampshire",   32010, "ossimTransMercatorProjection", "42 30 n", "71 40 w",
                  30000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_New_Jersey",      32011, "ossimTransMercatorProjection", "38 50 n", "74 40 w",
                  40000, 0, 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_New_Mexico_East", 32012, "ossimTransMercatorProjection", "31 00 n", "104 20 w",
                  11000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_New_Mexico_Cent", 32013, "ossimTransMercatorProjection", "31 00 n", "106 15 w",
                  10000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_New_Mexico_West", 32014, "ossimTransMercatorProjection", "31 00 n", "107 50 w",
                  12000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_New_York_East",   32015, "ossimTransMercatorProjection", "40 00 n", "74 20 w",
                  30000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_New_York_Central",32016, "ossimTransMercatorProjection", "40 00 n", "76 35 w",
                  16000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_New_York_West",   32017, "ossimTransMercatorProjection", "40 00 n", "78 35 w",
                  16000, 0, 500000, 0.0, "ft"));
   
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_New_York_Long_Is",32018, "ossimLambertConformalConicProjection", "40 30 n", "74 w",
                  "40 40 n", "41 02 n", 2000000, 100000, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_North_Carolina",  32019, "ossimLambertConformalConicProjection", "33 45 n", "79 00 w",
                  "34 20 n", "36 10 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_North_Dakota_N",  32020, "ossimLambertConformalConicProjection", "47 00 n", "100 30 w",
                  "47 26 n", "48 44 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_North_Dakota_S",  32021, "ossimLambertConformalConicProjection", "45 40 n", "100 30 w",
                  "46 11 n", "47 29 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Ohio_North",      32022, "ossimLambertConformalConicProjection", "39 40 n", "82 30 w",
                  "40 26 n", "41 42 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Ohio_South",      32023, "ossimLambertConformalConicProjection", "38 00 n", "82 30 w",
                  "38 44 n", "40 02 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Oklahoma_North",  32024, "ossimLambertConformalConicProjection", "35 00 n", "98 00 w",
                  "35 34 n", "36 46 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Oklahoma_South",  32025, "ossimLambertConformalConicProjection", "33 20 n", "98 00 w",
                  "33 56 n", "35 14 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Oregon_North",    32026, "ossimLambertConformalConicProjection", "43 40 n", "120 30 w",
                  "44 20 n", "46 00 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Oregon_South",    32027, "ossimLambertConformalConicProjection", "41 40 n", "120 30 w",
                  "42 20 n", "44 00 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Pennsylvania_N",  32028, "ossimLambertConformalConicProjection", "40 10 n", "77 45 w",
                  "40 53 n", "41 57 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Pennsylvania_S",  32029, "ossimLambertConformalConicProjection", "39 20 n", "77 45 w",
                  "39 56 n", "40 58 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Rhode_Island",    32030, "ossimTransMercatorProjection", "41 05 n", "71 30 w",
                  160000, 0, 500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_South_Carolina_N",32031, "ossimLambertConformalConicProjection", "33 00 n", "81 00 w",
                  "33 46 n", "34 58 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_South_Carolina_S",32033, "ossimLambertConformalConicProjection", "31 50 n", "81 00 w",
                  "32 20 n", "33 40 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_South_Dakota_N",  32034, "ossimLambertConformalConicProjection", "43 50 n", "100 00 w",
                  "44 25 n", "45 41 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_South_Dakota_S",  32035, "ossimLambertConformalConicProjection", "42 20 n", "100 20 w",
                  "42 50 n", "44 24 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Tennessee",       32036, "ossimLambertConformalConicProjection", "34 40 n", "86 00 w",
                  "35 15 n", "36 25 n", 2000000, 100000, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Texas_North",     32037, "ossimLambertConformalConicProjection", "34 00 n", "101 30 w",
                  "34 39 n", "36 11 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Texas_North_Cen", 32038, "ossimLambertConformalConicProjection", "31 40 n", "97 30 w",
                  "32 08 n", "33 58 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Texas_Central",   32039, "ossimLambertConformalConicProjection", "29 40 n", "100 20 w",
                  "30 07 n", "31 53 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Texas_South_Cen", 32040, "ossimLambertConformalConicProjection", "27 50 n", "99 00 w",
                  "28 23 n", "30 17 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Texas_South",     32041, "ossimLambertConformalConicProjection", "25 40 n", "98 30 w",
                  "26 10 n", "27 50 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Utah_North",      32042, "ossimLambertConformalConicProjection", "40 20 n", "111 30 w",
                  "40 43 n", "41 47 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Utah_Central",    32043, "ossimLambertConformalConicProjection", "38 20 n", "111 30 w",
                  "39 01 n", "40 39 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Utah_South",      32044, "ossimLambertConformalConicProjection", "36 40 n", "111 30 w",
                  "37 13 n", "38 21 n", 2000000, 0.0, "ft"));


   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Vermont",  32045, "ossimTransMercatorProjection", "42 30 n", "72 30 w", 28000, 0,
                  500000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Virginia_North",  32046, "ossimLambertConformalConicProjection", "37 40 n", "78 30 w",
                  "38 02 n", "39 12 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Virginia_South",  32047, "ossimLambertConformalConicProjection", "36 20 n", "78 30 w",
                  "36 46 n", "37 58 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Washington_North",32048, "ossimLambertConformalConicProjection", "47 00 n", "120 50 w",
                  "47 30 n", "48 44 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Washington_South",32049, "ossimLambertConformalConicProjection", "45 20 n", "120 30 w",
                  "45 50 n", "47 20 n", 2000000, 0.0, "ft"));
   
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_West_Virginia_N", 32050, "ossimLambertConformalConicProjection", "38 30 n", "79 30 w",
                  "39 00 n", "40 15 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_West_Virginia_S", 32051, "ossimLambertConformalConicProjection", "37 00 n", "81 00 w",
                  "37 29 n", "38 53 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Wisconsin_North", 32052, "ossimLambertConformalConicProjection", "45 10 n", "90 00 w",
                  "45 34 n", "46 46 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Wisconsin_Cen",   32053, "ossimLambertConformalConicProjection", "43 50 n", "90 00 w",
                  "44 15 n", "45 30 n", 2000000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Wisconsin_South", 32054, "ossimLambertConformalConicProjection", "42 00 n", "90 00 w",
                  "42 44 n", "44 04 n", 2000000, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Wyoming_East", 32055, "ossimTransMercatorProjection", "40 40 n", "105 10 w",
                  17000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Wyoming_E_Cen", 32056, "ossimTransMercatorProjection", "40 40 n", "107 20 w",
                  17000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Wyoming_W_Cen", 32057, "ossimTransMercatorProjection", "40 40 n", "108 45 w",
                  17000, 0, 500000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Wyoming_West",  32058, "ossimTransMercatorProjection", "40 40 n", "110 05 w",
                  17000, 0, 500000, 0.0, "ft"));
   
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_Puerto_Rico",  32059, "ossimLambertConformalConicProjection", "17 50 n", "66 26 w",
                  "18 02 n", "18 26 n", 500000, 0.0, "ft"));
   
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD27_St_Croix",     32060, "ossimLambertConformalConicProjection", "17 50 n", "66 26 w",
                  "18 02 n", "18 26 n", 500000, 100000, "ft"));


   //***            
   // NAD83 Datum based State Plane Projections
   //***
               
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alabama_East",  26929, "ossimTransMercatorProjection", "30 30 n", "85 50 w",
                  25000, 0, 200000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alabama_West",  26930, "ossimTransMercatorProjection", "30 n", "87 30 w",
                  15000, 0, 600000, 0.0, "m"));

   // addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_1", 26931  UNSUPPORTED!
   // (Hotine Mercator)
   
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_2", 26932, "ossimTransMercatorProjection", "54 n", "142 w", 10000, 0,
                  500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_3", 26933, "ossimTransMercatorProjection", "54 n", "146 w", 10000, 0,
                  500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_4", 26934, "ossimTransMercatorProjection", "54 n", "150 w", 10000, 0,
                  500000, 0.0, "m")); 
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_5", 26935, "ossimTransMercatorProjection", "54 n", "154 w", 10000, 0,
                  500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_6", 26936, "ossimTransMercatorProjection", "54 n", "158 w", 10000, 0,
                  500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_7", 26937, "ossimTransMercatorProjection", "54 n", "162 w", 10000, 0,
                  500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_8", 26938, "ossimTransMercatorProjection", "54 n", "166 w", 10000, 0,
                  500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_9", 26939, "ossimTransMercatorProjection", "54 n", "170 w", 10000, 0,
                  500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Alaska_zone_10",26940, "ossimLambertConformalConicProjection", "51 n", "176 w",
                  "51 50 n", "53 50 n", 1000000, 0.0, "m"));
                              
// California
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_California_1",  26941, "ossimLambertConformalConicProjection", "39 20 n", "122 w",
                  "40 n", "41 40 n", 2000000, 500000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_California_2",  26942, "ossimLambertConformalConicProjection", "37 40 n", "122 w",
                  "38 20 n", "39 50 n", 2000000, 500000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_California_3",  26943, "ossimLambertConformalConicProjection", "36 30 n", "120 30 w",
                  "37 04 n", "38 26 n", 2000000, 500000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_California_4",  26944, "ossimLambertConformalConicProjection", "35 20 n", "119 w",
                  "36 n", "37 15 n", 2000000, 500000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_California_5",  26945, "ossimLambertConformalConicProjection", "33 30 n", "118 w",
                  "34 02 n", "35 28 n", 2000000, 500000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_California_6",  26946, "ossimLambertConformalConicProjection", "32 10", "116 15 w",
                  "32 47 n", "33 53 n", 2000000, 500000, "m"));

// Arizona
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Arizona_East",  26948, "ossimTransMercatorProjection", "31 n", "110 10 w",
                  10000, 0, 213360, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Arizona_Centr", 26949, "ossimTransMercatorProjection", "31 n", "111 55 w",
                  10000, 0, 213360, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Arizona_West", 26950, "ossimTransMercatorProjection", "31 n", "113 45 w",
                  15000, 0, 213360, 0.0, "ft"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Arkansas_North", 26951, "ossimLambertConformalConicProjection", "34 20 n", "92 w",
                  "34 56 n", "36 14 n", 400000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Arkansas_South", 26952, "ossimLambertConformalConicProjection", "32 40 n", "92 w",
                  "33 18 n", "34 46 n", 400000, 400000, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Colorado_North",  26953, "ossimLambertConformalConicProjection", "39 20 n", "105 30 w",
                  "39 43 n", "40 47 n", 914401.8289, 304800.6096, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Colorado_Centr",  26954, "ossimLambertConformalConicProjection", "37 50 n", "105 30 w",
                  "38 27 n", "39 45 n", 914401.8289, 304800.6096, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Colorado_South",  26955, "ossimLambertConformalConicProjection", "36 40 n", "105 30 w",
                  "37 14 n", "38 26 n", 914401.8289, 304800.6096, "m"));

               
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Connecticut",   26956, "ossimLambertConformalConicProjection",  "40 50 n", "72 45 w",
                  "41 12 n", "41 52 n", 304800.6096, 152400.3048, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Delaware",  26957, "ossimTransMercatorProjection", "38 n", "75 25 w", 200000, 0,
                  200000, 0.0, "m"));

// Florida
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Florida_East",  26958, "ossimTransMercatorProjection", "24 20 n", "81 w",
                  17000, 0, 200000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Florida_West",  26959, "ossimTransMercatorProjection", "24 20 n", "82 w",
                  17000, 0, 200000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Florida_North", 26960, "ossimLambertConformalConicProjection", "29 n", "84 30 w",
                  "29 35 n", "30 45 n", 600000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Hawaii_zone_1", 26961, "ossimTransMercatorProjection", "18 50 n", "155 30 w",
                  30000, 0, 500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Hawaii_zone_2", 26962, "ossimTransMercatorProjection", "20 20 n","156 40 w",
                  30000, 0, 500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Hawaii_zone_3", 26963, "ossimTransMercatorProjection", "21 10 n", "158 w",
                  100000, 0, 500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Hawaii_zone_4", 26964, "ossimTransMercatorProjection", "21 50 n", "159 30 w",
                  100000, 0, 500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Hawaii_zone_5", 26965, "ossimTransMercatorProjection", "21 40 n", "160 10 w",
                  0.0, 0, 500000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Georgia_East",  26966, "ossimTransMercatorProjection", "30 n", "82 10 w",
                  10000, 0, 200000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Georgia_West",  26967, "ossimTransMercatorProjection", "30 n", "84 10 w",
                  10000, 0, 700000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Idaho_East",    26968, "ossimTransMercatorProjection", "41 40 n", "112 10 w",
                  19000, 0, 200000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Idaho_Central", 26969, "ossimTransMercatorProjection", "41 40 n", "114 w",
                  19000, 0, 500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Idaho_West",    26970, "ossimTransMercatorProjection", "41 40 n", "115 45 w",
                  15000, 0, 800000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Illinois_East", 26971, "ossimTransMercatorProjection", "36 40 n", "88 20 w",
                  40000, 0, 300000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Illinois_West", 26972, "ossimTransMercatorProjection", "36 40 n", "90 10 w",
                  17000, 0, 700000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Indiana_East",  26973, "ossimTransMercatorProjection", "37 30 n", "85 40 w",
                  30000, 0, 100000, 250000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Indiana_West",  26974, "ossimTransMercatorProjection", "37 30 n", "87 05 w",
                  30000, 0, 900000, 250000, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Iowa_North",  26975, "ossimLambertConformalConicProjection", "41 30 n", "93 30 w",
                  "42 04 n", "43 16 n", 1500000, 1000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Iowa_South",  26976, "ossimLambertConformalConicProjection", "40 n", "93 30 w",
                  "40 37 n", "41 47 n", 500000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Kansas_North",  26977, "ossimLambertConformalConicProjection", "38 20 n", "98 00 w",
                  "38 43 n", "39 47 n", 400000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Kansas_South",  26978, "ossimLambertConformalConicProjection", "36 40 n", "98 30 w",
                  "37 16 n", "38 34 n", 400000, 400000, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Kentucky_North",  26979, "ossimLambertConformalConicProjection", "37 30 n", "84 15 w",
                  "37 58 n", "38 58 n", 500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Kentucky_South",  26980, "ossimLambertConformalConicProjection", "36 20 n", "85 45 w",
                  "36 44 n", "37 56 n", 500000, 500000, "m"));

   // Louisiana
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Louisiana_North", 26981, "ossimLambertConformalConicProjection", "30 30 n", "92 30 w",
                  "31 10 n", "32 40 n", 1000000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Louisiana_South", 26982, "ossimLambertConformalConicProjection", "28 30 n", "91 20 w",
                  "29 18 n", "30 42 n", 1000000, 0.0, "m"));

//   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Louisiana_Offshore", 0, "ossimLambertConformalConicProjection", "25 30 n", "91 20 w",
//                  "26 10 n", "27 50 n", 1000000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Maine_East", 26983, "ossimTransMercatorProjection", "43 40 n", "68 30 w",
                  10000, 0, 300000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Maine_West", 26984, "ossimTransMercatorProjection", "42 50 n", "70 10 w",
                  30000, 0, 900000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Maryland", 26985, "ossimLambertConformalConicProjection","37 40 n", "77 00 w",
                  "38 18 n", "39 27 n", 400000, 0.0, "m"));
                              
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Massachusetts",   26986, "ossimLambertConformalConicProjection",  "41 00 n", "71 30 w",
                  "41 43 n", "42 41 n", 200000, 750000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Massachusetts_Is", 26987, "ossimLambertConformalConicProjection", "41 00 n", "70 30 w",
                  "41 17 n", "41 29 n", 500000, 0.0, "m"));
                              
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Michigan_North",  26988, "ossimLambertConformalConicProjection", "44 47 n", "87 w",
                  "45 29 n", "47 05 n", 8000000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Michigan_Central",26989, "ossimLambertConformalConicProjection", "43 19 n", "84 22 w",
                  "44 11 n", "45 42 n", 6000000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Michigan_South",  26990, "ossimLambertConformalConicProjection", "41 30 n", "84 22 w",
                  "42 06 n", "43 40 n", 4000000, 0.0, "m")); 

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Minnesota_North", 26991, "ossimLambertConformalConicProjection", "46 30 n", "93 06 w",
                  "47 02 n", "48 38 n", 800000, 100000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Minnesota_Cent",  26992, "ossimLambertConformalConicProjection", "45 00 n", "94 15 w",
                  "45 37 n", "47 03 n", 800000, 100000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Minnesota_South", 26993, "ossimLambertConformalConicProjection", "43 00 n", "94 00 w",
                  "43 47 n", "45 13 n", 800000, 100000, "m"));
   
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Mississippi_East",26994, "ossimTransMercatorProjection", "29 30 n", "88 50 w",
                  20000, 0, 300000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Mississippi_West",26995, "ossimTransMercatorProjection", "29 30 n", "90 20 w",
                  20000, 0, 700000, 0.0, "m"));
                              
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Missouri_East",   26996, "ossimTransMercatorProjection", "35 50 n", "90 30 w",
                  15000, 0, 250000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Missouri_Central",26997, "ossimTransMercatorProjection", "35 50 n", "92 30 w",
                  15000, 0, 500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Missouri_West",   26998, "ossimTransMercatorProjection", "36 10 n", "94 30 w",
                  17000, 0, 850000, 0.0, "m"));
                              
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Montana",  32100, "ossimLambertConformalConicProjection", "44 15 n", "109 30 w",
                  "45 00 n", "49 00 n", 600000, 0.0, "m"));
                              
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Nebraska", 32104, "ossimLambertConformalConicProjection", "39 50 n", "100 00 w",
                  "40 00 n", "43 00 n", 500000, 0.0, "m"));
                                                            
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Nevada_East",     32107, "ossimTransMercatorProjection",  "34 45 n", "115 35 w",
                  10000, 0, 200000, 8000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Nevada_Central",  32108, "ossimTransMercatorProjection", "34 45 n", "116 40 w",
                  10000, 0, 500000, 6000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Nevada_West",     32109, "ossimTransMercatorProjection", "34 45 n", "118 35 w",
                  10000, 0, 800000, 4000000, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_New_Hampshire",   32110, "ossimTransMercatorProjection", "42 30 n", "71 40 w",
                  30000, 0, 300000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_New_Jersey",      32111, "ossimTransMercatorProjection", "38 50 n", "74 30 w",
                  10000, 0, 150000, 0.0, "m"));

   // AddStatePlaneMapInfo(New Mexico
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_New_Mexico_East", 32112, "ossimTransMercatorProjection", "31 00 n", "104 20 w",
                  11000, 0, 165000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_New_Mexico_Cent", 32113, "ossimTransMercatorProjection", "31 00 n", "106 15 w",
                  10000, 0, 500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_New_Mexico_West", 32114, "ossimTransMercatorProjection", "31 00 n", "107 50 w",
                  12000, 0, 830000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_New_York_East",   32115, "ossimTransMercatorProjection", "38 50 n", "74 30 w",
                  10000, 0, 150000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_New_York_Central",32116, "ossimTransMercatorProjection", "40 00 n", "76 35 w",
                  16000, 0, 250000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_New_York_West",   32117, "ossimTransMercatorProjection", "40 00 n", "78 35 w",
                  16000, 0, 350000, 0.0, "ft"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_New_York_Long_Is",32118, "ossimLambertConformalConicProjection", "40 10 n", "74 w",
                  "40 40 n", "41 02 n", 300000, 0.0, "m"));
   
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_North_Carolina",  32119, "ossimLambertConformalConicProjection", "33 45 n", "79 00 w",
                  "34 20 n", "36 10 n", 609601.22, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_North_Dakota_N",  32120, "ossimLambertConformalConicProjection", "47 00 n", "100 30 w",
                  "47 26 n", "48 44 n", 600000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_North_Dakota_S",  32121, "ossimLambertConformalConicProjection", "45 40 n", "100 30 w",
                  "46 11 n", "47 29 n", 600000, 0.0, "m"));

   // Ohio
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Ohio_North",      32122, "ossimLambertConformalConicProjection",  "39 40 n", "82 30 w",
                  "40 26 n", "41 42 n", 600000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Ohio_South",      32123, "ossimLambertConformalConicProjection", "38 00 n", "82 30 w",
                  "38 44 n", "40 02 n", 600000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Oklahoma_North",  32124, "ossimLambertConformalConicProjection",  "35 00 n", "98 00 w",
                  "35 34 n", "36 46 n", 600000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Oklahoma_South",  32125, "ossimLambertConformalConicProjection", "33 20 n", "98 00 w",
                  "33 56 n", "35 14 n", 600000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Oregon_North",    32126, "ossimLambertConformalConicProjection",  "43 40 n",
                  "120 30 w", "44 20 n", "46 00 n", 2500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Oregon_South",    32127, "ossimLambertConformalConicProjection", "41 40 n", "120 30 w",
                  "42 20 n", "44 00 n", 1500000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Pennsylvania_N",  32128, "ossimLambertConformalConicProjection", "40 10 n", "77 45 w",
                  "40 53 n", "41 57 n", 600000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Pennsylvania_S",  32129, "ossimLambertConformalConicProjection", "39 20 n", "77 45 w",
                  "39 56 n", "40 58 n", 600000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Rhode_Island", 32130, "ossimTransMercatorProjection", "41 05 n", "71 30 w",
                  160000, 0, 100000, 0.0, "m")); 

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_South_Carolina",  32133, "ossimLambertConformalConicProjection",  "31 50 n", "81 00 w",
                  "32 30 n", "34 50 n", 609600, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_South_Dakota_N",  32134, "ossimLambertConformalConicProjection", "43 50 n", "100 00 w",
                  "44 25 n", "45 41 n", 600000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_South_Dakota_S",  32135, "ossimLambertConformalConicProjection", "42 20 n", "100 20 w",
                  "42 50 n", "44 24 n", 600000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Tennessee",       32136, "ossimLambertConformalConicProjection", "34 20 n", "86 00 w",
                  "35 15 n", "36 25 n", 600000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Texas_North",     32137, "ossimLambertConformalConicProjection", "34 00 n", "101 30 w",
                  "34 39 n", "36 11 n", 200000, 1000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Texas_North_Cen", 32138, "ossimLambertConformalConicProjection", "31 40 n", "98 30 w",
                  "32 08 n", "33 58 n", 600000, 2000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Texas_Central",   32139, "ossimLambertConformalConicProjection", "29 40 n", "100 20 w",
                  "30 07 n", "31 53 n", 700000, 3000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Texas_South_Cen", 32140, "ossimLambertConformalConicProjection", "27 50 n", "99 00 w",
                  "28 23 n", "30 17 n", 600000, 4000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Texas_South",     32141, "ossimLambertConformalConicProjection", "25 40 n", "98 30 w",
                  "26 10 n", "27 50 n", 300000, 5000000, "m"));
   
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Utah_North",      32142, "ossimLambertConformalConicProjection", "40 20 n", "111 30 w",
                  "40 43 n", "41 47 n", 500000, 1000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Utah_Central",    32143, "ossimLambertConformalConicProjection", "38 20 n", "111 30 w",
                  "39 01 n", "40 39 n", 500000, 2000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Utah_South",      32144, "ossimLambertConformalConicProjection", "36 40 n", "111 30 w",
                  "37 13 n", "38 21 n", 500000, 3000000, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Vermont",  32145, "ossimTransMercatorProjection", "42 30 n", "72 30 w", 28000, 0,
                  500000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Virginia_North",  32146, "ossimLambertConformalConicProjection", "37 40 n", "78 30 w",
                  "38 02 n", "39 12 n", 3500000, 2000000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Virginia_South",  32147, "ossimLambertConformalConicProjection", "36 20 n", "78 30 w",
                  "36 46 n", "37 58 n", 3500000, 1000000, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Washington_North",32148, "ossimLambertConformalConicProjection", "47 00 n", "120 50 w",
                  "47 30 n", "48 44 n", 500000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Washington_South",32149, "ossimLambertConformalConicProjection", "45 20 n", "120 30 w",
                  "45 50 n", "47 20 n", 500000, 0.0, "m"));

   // West Virginia
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_West_Virginia_N", 32150, "ossimLambertConformalConicProjection", "38 30 n", "79 30 w",
                  "39 00 n", "40 15 n", 600000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_West_Virginia_S", 32151, "ossimLambertConformalConicProjection", "37 00 n", "81 00 w",
                  "37 29 n", "38 53 n", 600000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Wisconsin_North", 32152, "ossimLambertConformalConicProjection", "45 10 n", "90 00 w",
                  "45 34 n", "46 46 n", 600000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Wisconsin_Cen",   32153, "ossimLambertConformalConicProjection", "43 50 n", "90 00 w",
                  "44 15 n", "45 30 n", 600000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Wisconsin_South", 32154, "ossimLambertConformalConicProjection", "42 00 n", "90 00 w",
                  "42 44 n", "44 04 n", 600000, 0.0, "m"));

   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Wyoming_East",  32155, "ossimTransMercatorProjection", "40 30 n", "105 10 w",
                  16000, 0, 200000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Wyoming_E_Cen", 32156, "ossimTransMercatorProjection", "40 30 n", "107 20 w",
                  16000, 0, 400000, 100000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Wyoming_W_Cen", 32157, "ossimTransMercatorProjection", "40 30 n", "108 45 w",
                  16000, 0, 600000, 0.0, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo("NAD83_Wyoming_West",  32158, "ossimTransMercatorProjection", "40 30 n", "110 05 w",
                  16000, 0, 800000, 100000, "m"));
   addStatePlaneInfo(new ossimStatePlaneProjectionInfo ("NAD83_Puerto_Rico", 32161, "ossimLambertConformalConicProjection", "17 50 n", "66 26 w",
                  "18 02 n", "18 26 n", 200000, 200000, "m"));
}

//---
// Hidden from use stuff.
//---
ossimStatePlaneProjectionFactory::ossimStatePlaneProjectionFactory()
   : ossimProjectionFactoryBase()
{}

ossimStatePlaneProjectionFactory::ossimStatePlaneProjectionFactory(const ossimStatePlaneProjectionFactory&)
   : ossimProjectionFactoryBase()
{}

void ossimStatePlaneProjectionFactory::operator=(const ossimStatePlaneProjectionFactory& /* rhs */)
{}
