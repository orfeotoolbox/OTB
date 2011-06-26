//----------------------------------------------------------------------------
//
// File: ossimImageElevationDatabase.cpp
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  See class desciption in header file.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/elevation/ossimImageElevationDatabase.h>

#include <ossim/base/ossimCallback2wRet.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/elevation/ossimImageElevationHandler.h>
#include <ossim/util/ossimFileWalker.h>

#include <cmath>

static ossimTrace traceDebug(ossimString("ossimImageElevationDatabase:debug"));

RTTI_DEF1(ossimImageElevationDatabase, "ossimImageElevationDatabase", ossimElevationDatabase);

//---
// Call back class to register with ossimFileWalker for call to
// ossimImageElevationDatabase::processFile
//
// Placed here as it is unique to this class.
//---
class ProcessFileCB: public ossimCallback2wRet<const ossimFilename&, bool&, bool>
{
public:
   ProcessFileCB(
      ossimImageElevationDatabase* obj,
      bool (ossimImageElevationDatabase::*func)(const ossimFilename&, bool&))
      :
      m_obj(obj),
      m_func(func)
   {}
      
   virtual bool operator()(const ossimFilename& file, bool& flag) const
   {
      return ( m_obj->*m_func)(file, flag);
   }

private:
   ossimImageElevationDatabase* m_obj;
   bool (ossimImageElevationDatabase::*m_func)(const ossimFilename& file, bool& flag);
};

ossimImageElevationDatabase::ossimImageElevationDatabase()
   :
   ossimElevationCellDatabase(),
   m_entryMap(),
   m_lastMapKey(0),
   m_lastAccessedId(0)
{
}

// Protected destructor as this is derived from ossimRefenced.
ossimImageElevationDatabase::~ossimImageElevationDatabase()
{
}

bool ossimImageElevationDatabase::open(const ossimString& connectionString)
{
   // return false; // tmp drb...
   
   static const char M[] = "ossimImageElevationDatabase::open";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered...\n"
         << "\nConnection string: " << connectionString << "\n";
   }                   
   
   bool result = false;

   close();

   if ( connectionString.size() )
   {
      m_connectionString = connectionString.c_str();

      loadFileMap();

      if ( m_entryMap.size() )
      {
         result = true;
      }
      else
      {
         m_connectionString.clear();
      }
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " result=" << (result?"true\n":"false\n");
   }

   return result;
}

void ossimImageElevationDatabase::close()
{
   m_meanSpacing = 0.0;
   m_geoid = 0;
   m_connectionString.clear();
}

double ossimImageElevationDatabase::getHeightAboveMSL(const ossimGpt& gpt)
{
   if(isSourceEnabled())
   {
      ossimRefPtr<ossimElevCellHandler> handler = getOrCreateCellHandler(gpt);
      if(handler.valid())
      {
         return handler->getHeightAboveMSL(gpt); // still need to shift
      }
   }
   return ossim::nan();
}

double ossimImageElevationDatabase::getHeightAboveEllipsoid(const ossimGpt& gpt)
{
   double h = getHeightAboveMSL(gpt);
   if(h != ossim::nan())
   {
      h += getOffsetFromEllipsoid(gpt);
   }
   return h;
}

ossimRefPtr<ossimElevCellHandler> ossimImageElevationDatabase::createCell(
   const ossimGpt& gpt)
{
   ossimRefPtr<ossimElevCellHandler> result = 0;
   
   std::map<ossim_uint64, ossimImageElevationFileEntry>::iterator i = m_entryMap.begin();
   while ( i != m_entryMap.end() )
   {
      if ( (*i).second.m_loadedFlag == false )
      {
         // not loaded
         ossimRefPtr<ossimImageElevationHandler> h = new ossimImageElevationHandler();

         if ( (*i).second.m_rect.isLonLatNan() )
         {
            if ( h->open( (*i).second.m_file ) )
            {
               // First time opened.  Capture the rectangle. for next time.
               (*i).second.m_rect = h->getBoundingGndRect();
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimImageElevationDatabase::createCell WARN:\nCould not open: "
                  << (*i).second.m_file << "\nRemoving file from map!" << std::endl;
                  m_entryMap.erase(i);
            }
         }

         // Check the North up bounding rectangle for intersect.
         if ( (*i).second.m_rect.pointWithin(gpt) )
         {
            if ( h->isOpen() == false )
            {
               h->open( (*i).second.m_file );
            }

            if ( h->isOpen() )
            {
               //---
               // Check point coverage again as image may not be geographic and pointHasCoverage
               // has a check on worldToLocal point.
               //---
               if (  h->pointHasCoverage(gpt) )
               {
                  m_lastAccessedId = (*i).first;
                  (*i).second.m_loadedFlag = true;
                  result = h.get();
                  break;
               }
               else
               {
                  h = 0;
               }
            }
         }
         else
         {
            h = 0;
         }
      }

      ++i;
   }
   return result;
}

ossimRefPtr<ossimElevCellHandler> ossimImageElevationDatabase::getOrCreateCellHandler(
   const ossimGpt& gpt)
{
   ossimRefPtr<ossimElevCellHandler> result = 0;
   
   // Note: Must do mutex lock / unlock around any cach map access.
   m_cacheMapMutex.lock();

   if ( m_cacheMap.size() )
   {
      //---
      // Look in existing map for handler.
      //
      // Note: Cannot key off of id from gpt as cells can be any arbituary dimensions.
      //---

      CellMap::iterator lastAccessedCellIter = m_cacheMap.find(m_lastAccessedId);
      CellMap::iterator iter = lastAccessedCellIter;
        
      // Check from last accessed to end.
      while ( iter != m_cacheMap.end() )
      {
         if ( iter->second->m_handler->pointHasCoverage(gpt) )
         {
            result = iter->second->m_handler.get();
            break;
         }
         ++iter;
      }
     
      if ( result.valid() == false )
      {
         iter = m_cacheMap.begin();
              
         // Beginning to last accessed.
         while ( iter != lastAccessedCellIter)
         {
            if ( iter->second->m_handler->pointHasCoverage(gpt) )
            {
               result = iter->second->m_handler.get();
               break;
            }
            ++iter;
         }
      }

      if ( result.valid() )
      {
         m_lastAccessedId  = iter->second->m_id;
         iter->second->updateTimestamp();
      }
   }
   m_cacheMapMutex.unlock();
  
   if ( result.valid() == false )
   {
      // Not in m_cacheMap.  Create a new cell for point if we have coverage.
      result = createCell(gpt);

      if(result.valid())
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_cacheMapMutex);

         //---
         // Add the cell to map.
         // NOTE: ossimImageElevationDatabase::createCell sets m_lastAccessedId to that of
         // the entries map key.
         //---
         m_cacheMap.insert(std::make_pair(m_lastAccessedId,
                                          new CellInfo(m_lastAccessedId, result.get())));

         ++m_lastMapKey;

         // Check the map size and purge cells if needed.
         if(m_cacheMap.size() > m_maxOpenCells)
         {
            flushCacheToMinOpenCells();
         }
      }
   }

   return result;
}

bool ossimImageElevationDatabase::pointHasCoverage(const ossimGpt& gpt) const
{
   //---
   // NOTE:
   //
   // The bounding rect is the North up rectangle.  So if the underlying image projection is not
   // a geographic projection and there is a rotation this could return false positives.  Inside
   // the ossimImageElevationDatabase::createCell there is a call to
   // ossimImageElevationHandler::pointHasCoverage which does a real check from the
   // ossimImageGeometry of the image.
   //---
   bool result = false;
   std::map<ossim_uint64, ossimImageElevationFileEntry>::const_iterator i = m_entryMap.begin();
   while ( i != m_entryMap.end() )
   {
      if ( (*i).second.m_rect.pointWithin(gpt) )
      {
         result = true;
         break;
      }
      ++i;
   }
      
   return false;
}

double ossimImageElevationDatabase::getAccuracyLE90(const ossimGpt& /* gpt */) const
{
   return 0;
}

double ossimImageElevationDatabase::getAccuracyCE90(const ossimGpt& /* gpt */) const
{
   return 0;
}

bool ossimImageElevationDatabase::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   static const char M[] = "ossimImageElevationDatabase::loadState";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered..." << "\nkwl:\n" << kwl << "\n";
   }     
   bool result = false;
   const char* lookup = kwl.find(prefix, "type");
   if ( lookup )
   {
      std::string type = lookup;
      if ( ( type == "image_directory" ) || ( type == "ossimImageElevationDatabase" ) )
      {
         result = ossimElevationDatabase::loadState(kwl, prefix);

         if ( result )
         {
            loadFileMap();
         }
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " result=" << (result?"true\n":"false\n");
   }

   return result;
}

bool ossimImageElevationDatabase::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimElevationDatabase::saveState(kwl, prefix);
}

bool ossimImageElevationDatabase::processFile(const ossimFilename& file,
                                              bool& recurseFlag)
{
   static const char M[] = "ossimImageElevationDatabase::processFile";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered...\n" << "file: " << file << "\n";
   }

   // Set the flags for the walker.
   bool continueFlag = true;
   recurseFlag = true;

   // Add the file.
   
   m_entryMap.insert( std::make_pair(m_lastMapKey++, ossimImageElevationFileEntry(file)) );
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " recurseNoMoreFlag=" << recurseFlag
         << " continueFlag=" << continueFlag << "\n";
   }
   return continueFlag;
}

void ossimImageElevationDatabase::loadFileMap()
{
   if ( m_connectionString.size() )
   {
      // Create a file walker which will find files we can load from the connection string.
      ossimFileWalker* fw = new ossimFileWalker();
      fw->initializeDefaultFilterList();
      ossimCallback2wRet<const ossimFilename&, bool&, bool>* cb =
         new ProcessFileCB(this, &ossimImageElevationDatabase::processFile);
      fw->registerProcessFileCallback(cb);
      
      ossimFilename f = m_connectionString;

      // ossimFileWalker::walk will in turn call back to processFile method for each file it finds.
      fw->walk(f); 
      
      delete fw;
      fw = 0;
      delete cb;
      cb = 0;
   }
}

// Hidden from use:
ossimImageElevationDatabase::ossimImageElevationDatabase(
   const ossimImageElevationDatabase& /* copy_this */)
{
}

// Hidden from use:
const ossimImageElevationDatabase& ossimImageElevationDatabase::operator=(
   const ossimImageElevationDatabase& /* rhs */)
{
   return *this;
}

// Private container class:
ossimImageElevationDatabase::ossimImageElevationFileEntry::ossimImageElevationFileEntry()
   : m_file(),
     m_rect(),
     m_loadedFlag(false)
{
   m_rect.makeNan();
}

// Private container class:
ossimImageElevationDatabase::ossimImageElevationFileEntry::ossimImageElevationFileEntry(
   const ossimFilename& file)
   : m_file(file),
     m_rect(),
     m_loadedFlag(false)
{
   m_rect.makeNan();
}

