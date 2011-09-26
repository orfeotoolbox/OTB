//*****************************************************************************
// FILE: ossimGeoidManager.cpp
//
// License:  See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class ossimGeoidManager
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimGeoidManager.cpp 20096 2011-09-14 16:44:20Z dburken $

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimEnvironmentUtility.h>

RTTI_DEF1(ossimGeoidManager, "ossimGeoidManager", ossimGeoid);

#include <ossim/base/ossimGeoidNgs.h>
#include <ossim/base/ossimGeoidEgm96.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimNotifyContext.h>
//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimGeoidManager:exec");
static ossimTrace traceDebug ("ossimGeoidManager:debug");

ossimGeoidManager* ossimGeoidManager::theInstance = 0;

//*****************************************************************************
//  CONSTRUCTOR: ossimGeoidManager
//  
//*****************************************************************************
ossimGeoidManager::ossimGeoidManager()
{
   theInstance = this;
   theIdentityGeoid = new ossimIdentityGeoid();
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimGeoidManager
//  
//*****************************************************************************
ossimGeoidManager::~ossimGeoidManager()
{
//    vector<ossimGeoid*>::iterator g = theGeoidList.begin();
//    while (g != theGeoidList.end())
//    {
//       delete *g;
//       ++g;
//    }
   theGeoidList.clear();
}

//*****************************************************************************
//  METHOD: ossimGeoidManager::instance()
//  
//*****************************************************************************
ossimGeoidManager* ossimGeoidManager::instance()
{
   if (!theInstance)
   {
      theInstance = new ossimGeoidManager();
   }
   
   return theInstance;
}

//*****************************************************************************
//  METHOD: ossimElevManager::addGeoidSource
//  
//*****************************************************************************
void ossimGeoidManager::addGeoid(ossimRefPtr<ossimGeoid> geoid, bool toFrontFlag)
{
   if(!toFrontFlag)
   {
      theGeoidList.push_back(geoid);
   }
   else
   {
      theGeoidList.insert(theGeoidList.begin(), geoid);
   }
}

bool ossimGeoidManager::saveState(ossimKeywordlist& /* kwl */,
                                  const char* /* prefix */ ) const
{
   return true;
}

//*****************************************************************************
//  METHOD: ossimGeoidManager::loadState()
//  
//*****************************************************************************
bool ossimGeoidManager::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   static const char MODULE[] = "ossimGeoidManager::loadState()";

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG: " << MODULE << ", entering...\n";

   
   // Look for the ngs geoid directories
   const char* lookup = kwl.find(prefix, "geoid_99_directory");
   ossimByteOrder geoidNgsByteOrder = OSSIM_LITTLE_ENDIAN;
   const char* byteOrder = kwl.find(prefix, "geoid_99_directory.byte_order");
   if (!lookup)
   {
      lookup = kwl.find(prefix, "geoid_ngs_directory");
      byteOrder = kwl.find(prefix, "geoid_ngs_directory.byte_order");
   }
   if(byteOrder)
   {
      if(ossimString(byteOrder).contains("little"))
      {
         geoidNgsByteOrder = OSSIM_LITTLE_ENDIAN;
      }
      else
      {
         geoidNgsByteOrder = OSSIM_BIG_ENDIAN;
      }
   }
   if(lookup)
   {
      ossimFilename f = lookup;
      if (f.isDir())
      {
         ossimRefPtr<ossimGeoid> geoid = new ossimGeoidNgs(f, geoidNgsByteOrder);

         if (geoid->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "DEBUG: " << MODULE
                  << "\nAdded geoid dir:  " << f.c_str() << "\n";
            }

            addGeoid(geoid.get());
         }
         else
         {
            geoid = 0;
         }
      }
   }

   ossimFilename geoidGrid1996 = ossimEnvironmentUtility::instance()->getUserOssimSupportDir();
   geoidGrid1996 = geoidGrid1996.dirCat("geoids");
   geoidGrid1996 = geoidGrid1996.dirCat("geoid1996");
   geoidGrid1996 = geoidGrid1996.dirCat("egm96.grd");

   if(!geoidGrid1996.exists())
   {
      geoidGrid1996 = ossimEnvironmentUtility::instance()->getInstalledOssimSupportDir();
      geoidGrid1996 = geoidGrid1996.dirCat("geoids");
      geoidGrid1996 = geoidGrid1996.dirCat("geoid1996");
      geoidGrid1996 = geoidGrid1996.dirCat("egm96.grd");
   }

   if( geoidGrid1996.exists() )
   {
      ossimRefPtr<ossimGeoid> geoid = new ossimGeoidEgm96(geoidGrid1996);
      if (geoid->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG: " << MODULE
               << "\nAdded geoid egm 96:  " << geoidGrid1996.c_str()
               << "\n";
         }
         addGeoid(geoid.get());
      }
      else
      {
         geoid = 0;
      }
   }
   else
   {
      // Look for the geoid Earth Gravity Model (EGM) 96 directory.
      lookup = kwl.find(prefix, "geoid_egm_96_grid");
      if (lookup)
      {
         ossimFilename f = lookup;
         if (f.isDir() || f.isFile())
         {
            ossimRefPtr<ossimGeoid> geoid = new ossimGeoidEgm96(f);
            
            if (geoid->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
            {
               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "DEBUG: " << MODULE
                     << "\nAdded geoid egm 96:  " << f.c_str()
                     << "\n";
               }
               
               addGeoid(geoid.get());
            }
            else
            {
               geoid = 0;
            }
         }
      }
   }
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG: " << MODULE << ", returning...\n";
   }
   
   return true;
}


//*****************************************************************************
//  METHOD: ossimGeoidManager::open()
//  
//*****************************************************************************
bool ossimGeoidManager::open(const ossimFilename& dir, ossimByteOrder byteOrder)
{
   std::vector<ossimRefPtr<ossimGeoid> >::iterator g = theGeoidList.begin();
   bool status = true;
   while (g != theGeoidList.end())
   {
      status &= (*g)->open(dir, byteOrder);
      ++g;
   }

   return status;
}

//*****************************************************************************
//  METHOD: ossimGeoidManager::open()
//  
//*****************************************************************************
double ossimGeoidManager::offsetFromEllipsoid(const ossimGpt& gpt) const
{
   double offset = ossim::nan();
   std::vector<ossimRefPtr<ossimGeoid> >::const_iterator geoid =
      theGeoidList.begin();
   
   while ( ossim::isnan(offset) && (geoid != theGeoidList.end()))
   {
      offset = ((*geoid))->offsetFromEllipsoid(gpt);
      ++geoid;
   }
   
   return offset;
}

ossimGeoid* ossimGeoidManager::findGeoidByShortName(const ossimString& shortName, bool caseSensitive)
{
   ossim_uint32 idx=0;
   ossimString testString = shortName;
   if(shortName == "identity")
   {
      return theIdentityGeoid.get();
   }
   if(!caseSensitive)
   {
      testString  = testString.downcase();
   }
   for(idx = 0; idx < theGeoidList.size(); ++idx)
   {
      if(!caseSensitive)
      {
         if(theGeoidList[idx]->getShortName().downcase() == testString)
         {
            return theGeoidList[idx].get();
         }
      }
      else
      {
         if(theGeoidList[idx]->getShortName() == testString)
         {
            return theGeoidList[idx].get();
         }
      }
   }
   return 0;
}
