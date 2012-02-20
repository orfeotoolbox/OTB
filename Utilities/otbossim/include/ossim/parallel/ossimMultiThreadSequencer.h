//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
//  $Id$
//**************************************************************************************************
#ifndef ossimMultiThreadSequencer_HEADER
#define ossimMultiThreadSequencer_HEADER

#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/parallel/ossimJobMultiThreadQueue.h>
#include <ossim/parallel/ossimImageChainMtAdaptor.h>
#include <OpenThreads/Thread>

//*************************************************************************************************
//! This class manages the sequencing of tile requests across multiple threads. Note that multi-
//! threading can only be achieved through the use of getNextTile() method for sequencing. 
//! Conventional getTiles will not be multi-threaded.
//*************************************************************************************************
class OSSIMDLLEXPORT ossimMultiThreadSequencer : public ossimImageSourceSequencer
{
public:
   ossimMultiThreadSequencer(ossimImageSource* inputSource=NULL, 
                             ossim_uint32 num_threads=0,
                             ossimObject* owner=NULL);

   virtual ~ossimMultiThreadSequencer();

   //! Overrides base class implementation. This call initiates the multi-threaded getTile jobs for
   //! the first N tiles.
   virtual void setToStartOfSequence();

   //! Overrides base class in order to implement multi-threaded tile requests. The output tile 
   //! should be available in the tile cache, otherwise, method waits until it becomes available.
   virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32 resLevel=0);

   //! Specifies number of thread to support. Default behavior (if this method is never called) is
   //! to query the system for number of cores available.
   void setNumberOfThreads(ossim_uint32 num_threads);

   //! Fetches the number of threads being used. Useful when this object decides the quantity.
   ossim_uint32 getNumberOfThreads() const { return m_numThreads; }

   //! Accessed for performance logging.
   ossim_uint32 maxCacheSize() const { return m_maxCacheSize; }

   // FOR DEBUG:
   ossim_uint32 d_maxCacheUsed;
   ossim_uint32 d_cacheEmptyCount;
   double       d_idleTime1;
   double       d_idleTime2;
   double       d_idleTime3;
   double       d_idleTime4;
   double       d_idleTime5;
   double       d_idleTime6;
   double       d_jobGetTileT;
   double       handlerGetTileT();

protected:

   // Associates a job with a tile ID for quick access to its tile data when finished:
   typedef std::map<ossim_uint32 /*tile_ID*/, ossimRefPtr<ossimImageData> >  TileCache;

   //! Private class representing a getTile job.
   class ossimGetTileJob : public ossimJob
   {
   public:
      ossimGetTileJob(ossim_uint32 tile_id,  ossim_uint32 chain_id,  ossimMultiThreadSequencer& sequencer) 
         :  m_tileID(tile_id), 
            m_chainID(chain_id), 
            m_sequencer(sequencer),
            t_launchNewJob(true) {}

      virtual void start();

   private:
      ossim_uint32                 m_tileID;
      ossim_uint32                 m_chainID;
      ossimMultiThreadSequencer&   m_sequencer;

      bool t_launchNewJob;                    //### for experimental purposes
      friend class ossimMultiThreadSequencer; //### for experimental purposes
   };
   friend class ossimGetTileJob;

   //! Private class for getTile job callbacks.
   class ossimGetTileCallback : public ossimJobCallback
   {
   public:
      ossimGetTileCallback() {}
      virtual void finished(ossimJob* job)
      {
         if (job != NULL)
            job->finished();
      }
   };

   //! Queues up the optimum amount of jobs given the availability of thread chains and space in
   //! the output tile cache.
   void initJobs();

   //! Access method to tile cache with scope lock to avoid multiple threads writing to
   //! the cache simultaneously. 
   //! NOTE: chain_id being passed only for debug. To be removed.
   void setTileInCache(ossim_uint32 tile_id, ossimImageData* tile, ossim_uint32 chain_id, double dt /*for debug*/);

   //! Method to job queue with scope lock to avoid multiple threads modifying the queue
   //! simultaneously.
   void nextJob(ossim_uint32 chain_id);

   //! For debug -- thread-safe console output
   void print(ostringstream& msg) const;

   ossimRefPtr<ossimImageChainMtAdaptor> m_inputChain; //!< Same as base class' theInputConnection
   ossimRefPtr<ossimJobMultiThreadQueue> m_jobMtQueue;
   ossim_uint32                          m_numThreads;
   ossimRefPtr<ossimGetTileCallback>     m_callback;
   ossim_uint32                          m_nextTileID; //!< ID of next tile to be threaded, different from base class' theCurrentTileNumber
   TileCache                             m_tileCache;  //!< Saves tiles output by threaded jobs
   ossim_uint32                          m_maxCacheSize;
   ossim_uint32                          m_maxTileCacheFactor;
   mutable OpenThreads::Mutex            m_cacheMutex;   
   mutable OpenThreads::Mutex            m_jobMutex;   
   ossim_uint32                          m_totalNumberOfTiles;
   OpenThreads::Block                    m_getTileBlock; //<! Blocks execution of main thread while waiting for tile to become available
   OpenThreads::Block                    m_nextJobBlock; //<! Blocks execution of worker threads

   // FOR DEBUG:
   mutable OpenThreads::Mutex d_printMutex;
   mutable OpenThreads::Mutex d_timerMutex;
   bool d_debugEnabled;
   ossim_uint32 d_timedBlocksDt;
   bool d_timeMetricsEnabled;
   double d_t1;
};

#endif
