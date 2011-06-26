#ifndef ossimElevationDatabase_HEADER
#define ossimElevationDatabase_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimGeoid.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/elevation/ossimElevSource.h>
#include <ossim/elevation/ossimElevCellHandler.h>
#include <ossim/base/ossimTimer.h>
#include <vector>
#include <map>
class OSSIM_DLL ossimElevationDatabase : public ossimElevSource
{
public:
   
   ossimElevationDatabase()
   :ossimElevSource(),
   m_meanSpacing(0.0)
   {
   }
      
   ossimElevationDatabase(const ossimElevationDatabase& src)
   :ossimElevSource(src),
   m_connectionString(src.m_connectionString),
   m_geoid(src.m_geoid),
   m_meanSpacing(src.m_meanSpacing)
   {
   }

   const ossimElevationDatabase& operator = (const ossimElevationDatabase& rhs);
      
   virtual double getMeanSpacingMeters() const
   {
      return m_meanSpacing;
   }
   virtual void setGeoid(ossimGeoid* geoid)
   {
      m_geoid = geoid;
   }
   ossimGeoid* getGeoid()
   {
      return m_geoid.get();
   }
   const ossimGeoid* getGeoid()const
   {
      return m_geoid.get();
   }
   
   /**
    * Open a connection to a database.  In most cases this will be a pointer
    * to a directory like in a Dted directory reader.  
    *
    */
   virtual bool open(const ossimString& connectionString)=0;
   const ossimString& getConnectionString()const
   {
      return m_connectionString;
   }
   virtual void getOpenCellList(std::vector<ossimFilename>& /* list */) const
   {
   }

   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix)const;
   
protected:
   virtual ~ossimElevationDatabase()
   {
      m_geoid = 0;
   }
   virtual double getOffsetFromEllipsoid(const ossimGpt& gpt)const;

   ossimString m_connectionString;
   ossimRefPtr<ossimGeoid>    m_geoid;
   ossim_float64              m_meanSpacing;
   
TYPE_DATA
};

class OSSIM_DLL ossimElevationCellDatabase : public ossimElevationDatabase
{
public:
   struct CellInfo : ossimReferenced
   {
      CellInfo(ossim_uint64 id,
               ossimElevCellHandler* handler = 0)
         :ossimReferenced(),
          m_id(id),
          m_handler(handler),
          m_timestamp(0)
      {
            m_timestamp = ossimTimer::instance()->tick();
      }
      CellInfo(const CellInfo& src)
         :ossimReferenced(src),
          m_id(src.m_id),
          m_handler(src.m_handler),
          m_timestamp(src.m_timestamp)
      {
      }
      CellInfo()
         :ossimReferenced(),
          m_id(0),
          m_handler(0),
          m_timestamp(0)
      {
      }
      const CellInfo& operator =(const CellInfo& src)
      {
         if (this != &src)
         {
            m_id = src.m_id;
            m_handler = src.m_handler;
            m_timestamp = src.m_timestamp;
         }
         return *this;
      }
      void updateTimestamp()
      {
         m_timestamp = ossimTimer::instance()->tick();
      }
      ossim_uint64 id()const
      {
         return m_id;
      }
      ossim_uint64                      m_id;
      ossimRefPtr<ossimElevCellHandler> m_handler;
      ossimTimer::Timer_t               m_timestamp;
   };
   typedef std::map<ossim_uint64, ossimRefPtr<CellInfo> > CellMap;
   
   ossimElevationCellDatabase()
      :ossimElevationDatabase(),
      m_minOpenCells(5),
      m_maxOpenCells(10),
      m_memoryMapCellsFlag(false)
   {
   }
   ossimElevationCellDatabase(const ossimElevationCellDatabase& src)
      :ossimElevationDatabase(src),
      m_minOpenCells(src.m_minOpenCells),
      m_maxOpenCells(src.m_maxOpenCells),
      m_cacheMap(src.m_cacheMap),
      m_memoryMapCellsFlag(src.m_memoryMapCellsFlag)
   {
   }
   const ossimElevationCellDatabase& operator=(const ossimElevationCellDatabase& rhs);

   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix)const;
   virtual ossim_uint32 getMinOpenCells()const
   {
      return m_minOpenCells;
   }
   virtual ossim_uint32 getMaxOpenCells()const
   {
      return m_maxOpenCells;
   }
   virtual void setMinMaxOpenCells(ossim_uint64 minCellCount,
                                   ossim_uint64 maxCellCount)
   {
      m_minOpenCells = minCellCount;
      m_maxOpenCells = maxCellCount;
   }
   virtual bool getMemoryMapCellsFlag()const
   {
      return m_memoryMapCellsFlag;
   }
   virtual void setMemoryMapCellsFlag(bool flag)
   {
      m_memoryMapCellsFlag = flag;
   }
   virtual void getOpenCellList(std::vector<ossimFilename>& list) const;
   virtual ossim_uint64 createId(const ossimGpt& /* pt */)const
   {
      return 0;
   }
   virtual ossimRefPtr<ossimElevCellHandler> getOrCreateCellHandler(const ossimGpt& gpt);
protected:
   virtual ossimRefPtr<ossimElevCellHandler> createCell(const ossimGpt& /* gpt */)
   {
      return 0;
   }
   virtual void remove(ossim_uint64 id)
   {
      CellMap::iterator iter = m_cacheMap.find(id);
      if(iter != m_cacheMap.end())
      {
         m_cacheMap.erase(iter);
      }
   }
   void flushCacheToMinOpenCells()
   {
      // lets flush the cache from least recently used to recent.
      //
      CellMap sortedMap;
      CellMap::iterator iter = m_cacheMap.begin();
      
      while(iter != m_cacheMap.end())
      {
         sortedMap.insert(std::make_pair(iter->second->m_timestamp, iter->second));
         ++iter;
      }
      
      iter = sortedMap.begin();
      while((iter!=sortedMap.end())&&(m_cacheMap.size() > m_minOpenCells))
      {
         remove(iter->second->id());
         ++iter;
      }
   }
   
   ossim_uint32               m_minOpenCells;
   ossim_uint32               m_maxOpenCells;
   mutable OpenThreads::Mutex m_cacheMapMutex;
   CellMap                    m_cacheMap;
   ossim_uint32               m_memoryMapCellsFlag;
   
   TYPE_DATA;
};

#endif
