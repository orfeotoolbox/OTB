//*****************************************************************************
// Copyright (C) 2005 Garrett Potts, all rights reserved.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
// 
// DESCRIPTION:
//   Contains declaration of class ossimTraceManager.
//
//*****************************************************************************
// $Id: ossimTraceManager.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <algorithm>

#include <ossim/base/ossimTraceManager.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimRegExp.h>

ossimTraceManager* ossimTraceManager::theInstance = 0;

ossimTraceManager::ossimTraceManager()
{
   theInstance = this;
}

ossimTraceManager* ossimTraceManager::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimTraceManager;
   }
   
   return theInstance;
}

void ossimTraceManager::setTracePattern(const ossimString& pattern)
{
   thePattern = pattern;
   setTraceFlags(true);
}

void ossimTraceManager::addTrace(ossimTrace* traceObj)
{
   if(!traceObj)
   {
      return;
   }
   
   std::vector<ossimTrace*>::iterator iter = std::find(theTraceList.begin(),
                                                       theTraceList.end(),
                                                       traceObj);
   if(iter == theTraceList.end())
   {
      theTraceList.push_back(traceObj);

      if(thePattern.size())
      {
         ossimRegExp rxp;
         rxp.compile(thePattern.c_str());
         if(rxp.find(traceObj->getTraceName().c_str()))
         {
            // Found a match so set the flag and increment the counter.
            traceObj->setTraceFlag(true);
         }
         else
         {
            traceObj->setTraceFlag(false);
         }
      }
      else
      {
         traceObj->setTraceFlag(false);
      } 
   }
}

void ossimTraceManager::removeTrace(ossimTrace* traceObj)
{
   std::vector<ossimTrace*>::iterator iter = std::find(theTraceList.begin(),
                                                       theTraceList.end(),
                                                       traceObj);

   if(iter !=  theTraceList.end())
   {
      theTraceList.erase(iter);
   }
}

void ossimTraceManager::setTraceFlags(bool flag)
{
   ossimRegExp rxp;
   rxp.compile(thePattern.c_str());
   
   for(ossim_uint32 idx = 0; idx < theTraceList.size(); ++idx)
   {
      if (rxp.find(theTraceList[idx]->getTraceName().c_str()))
      {
         // Found a match so set the flag and increment the counter.
         theTraceList[idx]->setTraceFlag(flag);
      }
   }
}
