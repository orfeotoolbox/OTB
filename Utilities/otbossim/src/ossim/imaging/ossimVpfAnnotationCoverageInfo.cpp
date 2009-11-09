//*************************************************************************
// Copyright (C) 2004 Intelligence Data Systems, Inc.  All rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
//**************************************************************************
// $Id: ossimVpfAnnotationCoverageInfo.cpp 15833 2009-10-29 01:41:53Z eshirschorn $

#include <vector>
#include <algorithm>
#include <ossim/imaging/ossimVpfAnnotationCoverageInfo.h>
#include <ossim/vec/ossimVpfLibrary.h>
#include <ossim/vec/ossimVpfFeatureClassSchema.h>

bool ossimVpfAnnotationCoverageInfo::hasRenderableFeature()const
{
  for(ossim_uint32 idx = 0; idx < theFeatureInfoArray.size();++idx)
  {
     if(theFeatureInfoArray[idx]->getEnabledFlag())
     {
        return true;
     }
  }

  return false;
}

void ossimVpfAnnotationCoverageInfo::getEnabledFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& result)
{
  for(ossim_uint32 idx = 0; idx < theFeatureInfoArray.size();++idx)
    {
      if(theFeatureInfoArray[idx]->getEnabledFlag())
	{
	  result.push_back(theFeatureInfoArray[idx]);
	}
    }
}

void ossimVpfAnnotationCoverageInfo::getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& result)
{
  for(ossim_uint32 idx = 0; idx < theFeatureInfoArray.size();++idx)
  {
     result.push_back(theFeatureInfoArray[idx]);
  }
}

ossimIrect ossimVpfAnnotationCoverageInfo::getBoundingProjectedRect()const
{
  ossimIrect result;
  result.makeNan();

  for(ossim_uint32 idx = 0; idx < theFeatureInfoArray.size();++idx)
    {
      ossimIrect tempRect = theFeatureInfoArray[idx]->getBoundingProjectedRect();
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


void ossimVpfAnnotationCoverageInfo::transform(ossimImageGeometry* geom)
{
  for(ossim_uint32 idx = 0; idx < theFeatureInfoArray.size();++idx)
  {
     theFeatureInfoArray[idx]->transform(geom);
  }
}

void ossimVpfAnnotationCoverageInfo::buildCoverage()
{
   buildCoverage( "" );
}

void ossimVpfAnnotationCoverageInfo::buildCoverage(const ossimString& feature)
{
   deleteAllFeatures();
   if(theLibrary)
   {
      ossimVpfCoverage coverage;
      
      if(theLibrary->getCoverage(theName, coverage))
      {
         ossimVpfFeatureClassSchema schema;
         if(coverage.openFeatureClassSchema(schema))
         {
            std::vector<ossimString> featureClassArray;
            schema.getFeatureClasses(featureClassArray);
            schema.closeTable();
            for(ossim_uint32  idx = 0; idx < featureClassArray.size(); ++idx)
            {
               ossimString s1 = feature;
               ossimString s2 = featureClassArray[idx];
               s1.downcase();
               s2.downcase();
               if (! feature.length() || (s1 == s2) )
               {
                  ossimVpfAnnotationFeatureInfo* featureInfo =
                     new ossimVpfAnnotationFeatureInfo;
                  featureInfo->setName(featureClassArray[idx]);
                  featureInfo->setCoverage(coverage);
                  theFeatureInfoArray.push_back(featureInfo);
                  featureInfo->buildFeature();
               }
            }
         }
      }
   }
}

void ossimVpfAnnotationCoverageInfo::drawAnnotations(ossimRgbImage* tile)
{
   for(ossim_uint32 idx = 0; idx < theFeatureInfoArray.size();++idx)
   {
      if(theFeatureInfoArray[idx]->getEnabledFlag())
      {
         theFeatureInfoArray[idx]->drawAnnotations(tile);
      }
   }
}

void ossimVpfAnnotationCoverageInfo::deleteAllFeatures()
{
   for(ossim_uint32 idx = 0; idx < theFeatureInfoArray.size();++idx)
   {
      theFeatureInfoArray[idx];
   }
   
   theFeatureInfoArray.clear();
}



bool ossimVpfAnnotationCoverageInfo::saveState(ossimKeywordlist& kwl,
					       const char* prefix)const
{
   kwl.add(prefix,
           "name",
           theName,
           true);
   
   for(ossim_uint32 idx = 0; idx < theFeatureInfoArray.size(); ++idx)
   {
      theFeatureInfoArray[idx]->saveState(kwl,
					  (ossimString(prefix) + "feature" +
					   ossimString::toString(idx) + ".").c_str());
   }
   
   return true;
}

bool ossimVpfAnnotationCoverageInfo::loadState(const ossimKeywordlist& kwl,
					       const char* prefix)
{
  deleteAllFeatures();

  if(!theLibrary)
    {
      return false;
    }

  theName = kwl.find(prefix, "name");
  ossimVpfCoverage coverage;

  if(theLibrary->getCoverage(theName, coverage))
    {
      ossimString regExpression =  ossimString("^(") + ossimString(prefix) + "feature[0-9]+.)";
      vector<ossimString> keys =
	kwl.getSubstringKeyList( regExpression );
      std::vector<int> theNumberList(keys.size());
      int offset = (int)(ossimString(prefix)+"feature").size();
      int idx = 0;
      for(idx = 0; idx < (int)theNumberList.size();++idx)
	{
	  ossimString numberStr(keys[idx].begin() + offset,
				keys[idx].end());
	  theNumberList[idx] = numberStr.toInt();
	}
      std::sort(theNumberList.begin(), theNumberList.end());
      
      for(idx=0;idx < (int)keys.size();++idx)
	{
	  ossimString newPrefix = ossimString(prefix);
	  newPrefix += ossimString("feature");
	  newPrefix += ossimString::toString(theNumberList[idx]);
	  newPrefix += ossimString(".");
	  ossimVpfAnnotationFeatureInfo* featureInfo = new ossimVpfAnnotationFeatureInfo;
	  featureInfo->setCoverage(coverage);
	  theFeatureInfoArray.push_back(featureInfo);
	  if(!featureInfo->loadState(kwl,
				     newPrefix))
	    {
	      return false;
	    }
	}
    }
  else
    {
      return false;
    }

  return true;
}
