//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimStatePlaneProjectionFactory.cpp 12168 2007-12-11 14:30:10Z gpotts $
#include <ossim/projection/ossimStatePlaneProjectionFactory.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/projection/ossimProjection.h>
#include "ossimStatePlaneProjectionInfo.inc"

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
      currentName = theStatePlaneInfoNameMap.find(name);
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
   ossim_uint32 idx;
   for(idx = 0; format1[idx].name;++idx)
   {
      addStatePlaneInfo(new ossimStatePlaneProjectionInfo(format1[idx].name,
                                                          format1[idx].pcsCode,
                                                          format1[idx].projCode,
                                                          format1[idx].param1,
                                                          format1[idx].param2,
                                                          format1[idx].param3,
                                                          format1[idx].param4,
                                                          format1[idx].falseEasting,
                                                          format1[idx].falseNorthing,
                                                          format1[idx].units));
      
   }

   for(idx = 0; format2[idx].name;++idx)
   {
      addStatePlaneInfo(new ossimStatePlaneProjectionInfo(format2[idx].name,
                                                          format2[idx].pcsCode,
                                                          format2[idx].projCode,
                                                          format2[idx].param1,
                                                          format2[idx].param2,
                                                          format2[idx].param3,
                                                          format2[idx].param4,
                                                          format2[idx].falseEasting,
                                                          format2[idx].falseNorthing,
                                                          format2[idx].units));
      
   }
   
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
