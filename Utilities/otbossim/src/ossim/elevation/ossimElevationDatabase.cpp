#include <ossim/elevation/ossimElevationDatabase.h>

RTTI_DEF1(ossimElevationDatabase, "ossimElevationDatabase", ossimObject);
RTTI_DEF1(ossimElevationCellDatabase, "ossimElevationCellDatabase", ossimElevationDatabase);

const ossimElevationDatabase& ossimElevationDatabase::operator=(const ossimElevationDatabase& rhs)
{
   if ( this != &rhs )
   {
      ossimElevSource::operator=(rhs);
      m_connectionString = rhs.m_connectionString;
      m_geoid            = rhs.m_geoid;
      m_meanSpacing      = rhs.m_meanSpacing;
   }
   return *this;
}

double ossimElevationDatabase::getOffsetFromEllipsoid(const ossimGpt& gpt)const
{
   double result = 0.0;
   if(m_geoid.valid())
   {
      result = m_geoid->offsetFromEllipsoid(gpt);
   }
   else 
   {
      result = ossimGeoidManager::instance()->offsetFromEllipsoid(gpt);
   }
   
   if(ossim::isnan(result))
   {
      result = 0.0;
   }
   
   return result;
}

bool ossimElevationDatabase::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   m_connectionString = kwl.find(prefix, "connection_string");
   ossimString minOpenCells = kwl.find(prefix, "min_open_cells");
   ossimString maxOpenCells = kwl.find(prefix, "max_open_cells");
   ossimString geoidType    = kwl.find(prefix, "geoid.type");
   if(m_connectionString.empty())
   {
      // try backward compatability to a filename
      //
      m_connectionString = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   }
   if(!geoidType.empty())
   {
      m_geoid = ossimGeoidManager::instance()->findGeoidByShortName(geoidType);
   }
   return ossimObject::loadState(kwl, prefix);
}

bool ossimElevationDatabase::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   kwl.add(prefix, "connection_string", m_connectionString, true);
   
   if(m_geoid.valid())
   {
      kwl.add(prefix, "geoid.type", m_geoid->getShortName(), true);
   }
   
   return ossimObject::saveState(kwl, prefix);
}

const ossimElevationCellDatabase& ossimElevationCellDatabase::operator=(
   const ossimElevationCellDatabase& rhs)
{
   if ( this != &rhs )
   {
      ossimElevationDatabase::operator=(rhs);
      m_minOpenCells       = rhs.m_minOpenCells;
      m_maxOpenCells       = rhs.m_maxOpenCells;
      m_cacheMap           = rhs.m_cacheMap;
      m_memoryMapCellsFlag = rhs.m_memoryMapCellsFlag;
   }
   return *this;
}

void ossimElevationCellDatabase::getOpenCellList(std::vector<ossimFilename>& list) const
{
   CellMap::const_iterator iter = m_cacheMap.begin();

   while(iter!=m_cacheMap.end())
   {
      list.push_back(iter->second->m_handler->getFilename());
      ++iter;
   }

}

ossimRefPtr<ossimElevCellHandler> ossimElevationCellDatabase::getOrCreateCellHandler(const ossimGpt& gpt)
{
  ossimRefPtr<ossimElevCellHandler> result = 0;
  ossim_uint64 id = createId(gpt);
  
  {
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_cacheMapMutex);
    CellMap::iterator iter = m_cacheMap.find(id);
    if(iter != m_cacheMap.end())
    {
      iter->second->updateTimestamp();
      result = iter->second->m_handler.get();
      
      return result.get();
    }
  }
  
  result = createCell(gpt);
  
  {
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_cacheMapMutex);
    if(result.valid())
    {
      m_cacheMap.insert(std::make_pair(id, new CellInfo(id, result.get())));

      // Check the map size and purge cells if needed.
      if(m_cacheMap.size() > m_maxOpenCells)
      {
         flushCacheToMinOpenCells();
      }
    }
  }

  return result;
}

bool ossimElevationCellDatabase::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   ossimString minOpenCells = kwl.find(prefix, "min_open_cells");
   ossimString maxOpenCells = kwl.find(prefix, "max_open_cells");
   ossimString geoidType    = kwl.find(prefix, "geoid.type");
   if(!minOpenCells.empty()&&
      !maxOpenCells.empty())
   {
      m_minOpenCells = minOpenCells.toUInt32();
      m_maxOpenCells = maxOpenCells.toUInt32();
      if(m_maxOpenCells < m_minOpenCells)
      {
         std::swap(m_minOpenCells, m_maxOpenCells);
      }
   }
   ossimString memoryMapCellsFlag = kwl.find(prefix, "memory_map_cells");
   if(!memoryMapCellsFlag.empty())
   {
      m_memoryMapCellsFlag  = memoryMapCellsFlag.toBool();
   }
   return ossimElevationDatabase::loadState(kwl, prefix);
}

bool ossimElevationCellDatabase::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   kwl.add(prefix, "memory_map_cells", m_memoryMapCellsFlag, true);
   kwl.add(prefix, "min_open_cells", m_minOpenCells, true);
   kwl.add(prefix, "max_open_cells", m_maxOpenCells, true);

   if(m_geoid.valid())
   {
      kwl.add(prefix, "geoid.type", m_geoid->getShortName(), true);
   }

   return ossimElevationDatabase::saveState(kwl, prefix);
}



