#ifndef ossimDtedElevationDatabase_HEADER
#define ossimDtedElevationDatabase_HEADER
#include "ossimElevationDatabase.h"
#include <ossim/base/ossimFilename.h>
#include <ossim/elevation/ossimDtedHandler.h>
#include <OpenThreads/Mutex>

class OSSIM_DLL ossimDtedElevationDatabase : public ossimElevationCellDatabase
{
public:
   typedef std::vector<ossimRefPtr<CellInfo> > DirectMap; // 360x180 cell grid
   ossimDtedElevationDatabase()
   :ossimElevationCellDatabase(),
   m_extension("")
   {
   }
   ossimDtedElevationDatabase(const ossimDtedElevationDatabase& rhs)
   :ossimElevationCellDatabase(rhs),
   m_extension(rhs.m_extension)
   {
   }
   virtual ~ossimDtedElevationDatabase()
   {
   }
   ossimObject* dup()const
   {
      return new ossimDtedElevationDatabase(*this);
   }
   virtual bool open(const ossimString& connectionString);
   virtual bool pointHasCoverage(const ossimGpt& gpt) const
   {
      ossimFilename filename;
      createFullPath(filename, gpt);
      
      return filename.exists();
   }
   
   /**
    * METHODS: accuracyLE90(), accuracyCE90()
    * Returns the vertical and horizontal accuracy (90% confidence) in the
    * region of gpt:
    */
   virtual double getAccuracyLE90(const ossimGpt& /* gpt */) const
   {
      std::cout << "ossimDtedDatabase::getAccuracyLE90 \n";
      return 0.0;
   }
   virtual double getAccuracyCE90(const ossimGpt& /* gpt */) const
   {
      std::cout << "ossimDtedDatabase::getAccuracyCE90 \n";
      return 0.0;
   }
   virtual double getHeightAboveMSL(const ossimGpt&);
   virtual double getHeightAboveEllipsoid(const ossimGpt& gpt);
   virtual ossim_uint64 createId(const ossimGpt& pt)const
   {
      ossim_uint64 y = static_cast<ossim_uint64>(ossim::wrap(pt.latd(), -90.0, 90.0)+90.0);
      ossim_uint64 x = static_cast<ossim_uint64>(ossim::wrap(pt.lond(),-180.0,180.0)+180.0);
      // map the extreme edge to the same ID ax the 179 west cell and the same for the 89
      // degree north cell.
      //
      x = x==360?359:x;
      y = y==180?179:y;
      // dted databases are 1x1 degree cells and we will use a world 
      // grid for id generation.
      //
      return (y*360+x);
   }
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix = 0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix = 0)const;
   
protected:
   ossimString                m_extension;

   bool openDtedDirectory(const ossimFilename& dir);
   void createRelativePath(ossimFilename& file, const ossimGpt& gpt)const;
   void createFullPath(ossimFilename& file, const ossimGpt& gpt)const
   {
      ossimFilename relativeFile;
      createRelativePath(relativeFile, gpt);
      file = ossimFilename(m_connectionString).dirCat(relativeFile);
   }
   virtual ossimRefPtr<ossimElevCellHandler> createCell(const ossimGpt& gpt);
#if 0
   ossimRefPtr<ossimElevCellHandler> getOrCreateHandler(const ossimGpt& gpt)
   {
      ossimRefPtr<ossimElevCellHandler> result = 0;
      ossim_uint64 id = createId(gpt);
      m_cacheMapMutex.lock();
      CellMap::iterator iter = m_cacheMap.find(id);
      
      if(iter != m_cacheMap.end())
      {
         iter->second->updateTimestamp();
         result = iter->second->m_handler.get();
      }
      else
      {
         ossimFilename f;
         createFullPath(f, gpt);
         if(f.exists())
         {
            ossimRefPtr<ossimDtedHandler> h = new ossimDtedHandler(f, m_memoryMapCellsFlag);
            if (!(h->getErrorStatus()))
            {
               m_cacheMap.insert(std::make_pair(id, new CellInfo(createId(gpt), h.get())));
               result = h.get();
            }
         }
         if(m_cacheMap.size() > m_maxOpenCells)
         {
            flushCacheToMinOpenCells();
         }
      }
      m_cacheMapMutex.unlock();
      
      return result;
   }
#endif
TYPE_DATA
};
#endif
