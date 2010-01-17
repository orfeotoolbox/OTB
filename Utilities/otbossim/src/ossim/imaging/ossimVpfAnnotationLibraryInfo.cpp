//*************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
//**************************************************************************
// $Id: ossimVpfAnnotationLibraryInfo.cpp 15836 2009-10-30 12:29:09Z dburken $

#include <algorithm>

#include <ossim/imaging/ossimVpfAnnotationLibraryInfo.h>
#include <ossim/imaging/ossimVpfAnnotationFeatureInfo.h>
#include <ossim/vec/ossimVpfLibrary.h>
#include <ossim/vec/ossimVpfDatabase.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimVpfAnnotationLibraryInfo:debug");


ossimVpfAnnotationLibraryInfo::ossimVpfAnnotationLibraryInfo(const ossimString& libraryName, bool enabledFlag)
   :
      theName(libraryName),
      theEnabledFlag(enabledFlag),
      theDatabase(NULL)
{
}

ossimVpfAnnotationLibraryInfo::~ossimVpfAnnotationLibraryInfo()
{
   deleteAllCoverage();
}

bool ossimVpfAnnotationLibraryInfo::getEnabledFlag()const
{
   return theEnabledFlag;
}

void ossimVpfAnnotationLibraryInfo::setEnabledFlag(bool flag)
{
   theEnabledFlag = flag;
}

const ossimString& ossimVpfAnnotationLibraryInfo::getName()const
{
   return theName;
}

void ossimVpfAnnotationLibraryInfo::setName(const ossimString& libraryName)
{
   theName = libraryName;
}

void ossimVpfAnnotationLibraryInfo::setDatabase(ossimVpfDatabase* database)
{
   theDatabase = database;
}

ossimVpfDatabase* ossimVpfAnnotationLibraryInfo::getDatabase()
{
   return theDatabase;
}

ossimIrect ossimVpfAnnotationLibraryInfo::getBoundingProjectedRect()const
{
   ossimIrect result;
   result.makeNan();
   
   for(ossim_uint32 idx = 0; idx < theCoverageLayerList.size(); ++idx)
   {
      ossimIrect tempRect = theCoverageLayerList[idx]->getBoundingProjectedRect();
      if(!tempRect.hasNans())
      {
         if(result.hasNans())
         {
            result = tempRect;
         }
         else
         {
            result = result.combine(tempRect);
         }
      }
   }
   
   return result;
}

void ossimVpfAnnotationLibraryInfo::getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& features)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationLibraryInfo::getAllFeatures DEBUG: entered..." << std::endl;
   }
   
   for(ossim_uint32 idx = 0; idx < theCoverageLayerList.size(); ++idx)
   {
      theCoverageLayerList[idx]->getAllFeatures(features);
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationLibraryInfo::getAllFeatures DEBUG: leaving..." << std::endl;
   }
}

void ossimVpfAnnotationLibraryInfo::transform(ossimImageGeometry* geom)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationLibraryInfo::transform DEBUG: entered..." << std::endl;
   }
   
   
   for(ossim_uint32 idx = 0; idx < theCoverageLayerList.size(); ++idx)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "transforming coverage = " << theCoverageLayerList[idx]->getName() << std::endl;
      }
      
      theCoverageLayerList[idx]->transform(geom);
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationLibraryInfo::transform DEBUG: leaving..." << std::endl;
   }
}

void ossimVpfAnnotationLibraryInfo::buildLibrary()
{
   buildLibrary( "", "");
}

void ossimVpfAnnotationLibraryInfo::buildLibrary(const ossimString& coverageName, const ossimString& feature)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationLibraryInfo::buildLibrary DEBUG: entered..." << std::endl;
   }
   deleteAllCoverage();
   if(!theDatabase)
   {
      return;
   }
   
   ossimVpfLibrary* library = theDatabase->getLibrary(theName);
   if(library)
   {
      std::vector<ossimString> coverageNames;
      library->getCoverageNames(coverageNames);
      for(ossim_uint32 idx = 0; idx < coverageNames.size(); ++idx)
      {
         ossimString s1 = coverageName;
         ossimString s2 = coverageNames[idx];
         s1.downcase();
         s2.downcase();
         if ( ! coverageName.length() || (s1 == s2) )
         {
            ossimVpfAnnotationCoverageInfo* coverageInfo =
               new ossimVpfAnnotationCoverageInfo;
            coverageInfo->setName(coverageNames[idx]);
            coverageInfo->setLibrary(library);
            theCoverageLayerList.push_back(coverageInfo);
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "coverage name = " << coverageNames[idx] << std::endl;
            }
            coverageInfo->buildCoverage(feature);
         }
      }
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationLibraryInfo::buildLibrary DEBUG: leaving..." << std::endl;
   }
}


void ossimVpfAnnotationLibraryInfo::drawAnnotations(ossimRgbImage* tile)
{
  for(ossim_uint32 idx = 0; idx < theCoverageLayerList.size(); ++idx)
    {
      theCoverageLayerList[idx]->drawAnnotations(tile);
    }
}

void ossimVpfAnnotationLibraryInfo::deleteAllCoverage()
{
  for(ossim_uint32 idx = 0; idx < theCoverageLayerList.size(); ++idx)
    {
      delete theCoverageLayerList[idx];
    }

  theCoverageLayerList.clear();
}

bool ossimVpfAnnotationLibraryInfo::saveState(ossimKeywordlist& kwl,
					      const char* prefix)const
{
   ossim_uint32 idx;
   
   kwl.add(prefix,
           "name",
           theName,
           true);
   
   for(idx = 0; idx < theCoverageLayerList.size(); ++idx)
   {
      theCoverageLayerList[idx]->saveState(kwl,
					   (ossimString(prefix) + "coverage" + ossimString::toString(idx) + ".").c_str());
   }
   
   return true;
}

bool ossimVpfAnnotationLibraryInfo::loadState(const ossimKeywordlist& kwl,
					      const char* prefix)
{
  deleteAllCoverage();
  theName = kwl.find(prefix, "name");
  
  ossimVpfLibrary* library = theDatabase->getLibrary(theName);

  if(!library)
    {
      return false;
    }
  ossimString regExpression =  ossimString("^(") + ossimString(prefix) + "coverage[0-9]+.)";
  vector<ossimString> keys =
    kwl.getSubstringKeyList( regExpression );
  std::vector<int> theNumberList(keys.size());
  int offset = (int)(ossimString(prefix)+"coverage").size();
  int idx = 0;
  for(idx = 0; idx < (int)theNumberList.size();++idx)
    {
      ossimString numberStr(keys[idx].begin() + offset,
			    keys[idx].end());
      theNumberList[idx] = numberStr.toInt();
    }
  std::sort(theNumberList.begin(), theNumberList.end());
  
  for(idx = 0; idx < (int)keys.size(); ++idx)
    {
      ossimString newPrefix = ossimString(prefix);
      newPrefix += ossimString("coverage");
      newPrefix += ossimString::toString(theNumberList[idx]);
      newPrefix += ossimString(".");
      ossimVpfAnnotationCoverageInfo* coverageInfo = new ossimVpfAnnotationCoverageInfo;
      coverageInfo->setLibrary(library);
      theCoverageLayerList.push_back(coverageInfo);
      coverageInfo->loadState(kwl,
			      newPrefix);
    }
  
  return true;
}
