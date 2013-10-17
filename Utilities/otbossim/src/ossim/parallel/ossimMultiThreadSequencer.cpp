//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
//! This class manages the sequencing of tile requests across multiple threads. Note that multi-
//! threading can only be achieved through the use of getNextTile() method for sequencing. 
//! Conventional getTiles will not be multi-threaded.
// 
//**************************************************************************************************
//  $Id$

#include <ossim/parallel/ossimMultiThreadSequencer.h>
#include <ossim/parallel/ossimMtDebug.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimTimer.h>

static const ossim_uint32 DEFAULT_MAX_TILE_CACHE_FACTOR = 8; // Must be > 1

ossimMtDebug* ossimMtDebug::m_instance = NULL;

//*************************************************************************************************
// Job's start method performs actual getTile in a thread on cloned chain and saves the result
// in the sequencer's results cache.
//*************************************************************************************************
void ossimMultiThreadSequencer::ossimGetTileJob::start()
{
   running();
   if (m_sequencer.d_debugEnabled)
   {
      ostringstream s1;
      s1<<"THREAD #"<<m_chainID<<" -- Starting tile/job #"<<m_tileID;
      m_sequencer.print(s1);
   }

   // Figure out the rect for this tile. Only process if rect is valid:
   ossimIrect tileRect;
   if (m_sequencer.getTileRect(m_tileID, tileRect))
   {
      // Perform the getTile and save the result:
      ossimRefPtr<ossimImageData> tile = 0;
      ossimImageSource* source = m_sequencer.m_inputChain->getClone(m_chainID);
      double dt = ossimTimer::instance()->time_s(); //###

      if (source != NULL)
         tile = source->getTile(tileRect);
      if (!tile.valid())
      {
         tile = m_sequencer.theBlankTile;
         tile->setImageRectangle(tileRect);
      }
      dt = ossimTimer::instance()->time_s() - dt; //###

      // Give the sequencer the tile. Execution may pause here while waiting for space to free up
      // if the cache is full.
      m_sequencer.setTileInCache(m_tileID, (ossimImageData*)tile->dup(), m_chainID, dt);
   }

   // Unblock the main thread which might be blocked waiting for jobs to finish:
   m_sequencer.m_getTileBlock.release();

   // Queue the next job using this job's freed-up image chain:
   if (t_launchNewJob)
      m_sequencer.nextJob(m_chainID);

   finished();
   
   if (m_sequencer.d_debugEnabled)
   {
      ostringstream s2;
      s2<<"THREAD #"<<m_chainID<<" -- Finished tile/job #"<<m_tileID;
      m_sequencer.print(s2);
   }
}

//*************************************************************************************************
// Constructor
//*************************************************************************************************
ossimMultiThreadSequencer::ossimMultiThreadSequencer(ossimImageSource* input, 
                                                     ossim_uint32 num_threads,
                                                     ossimObject* owner)
   : ossimImageSourceSequencer(input, owner),
   d_maxCacheUsed(0),
   d_cacheEmptyCount(0), 
   d_idleTime1(0.0),     
   d_idleTime2(0.0),     
   d_idleTime3(0.0),     
   d_idleTime4(0.0),     
   d_idleTime5(0.0),     
   d_idleTime6(0.0),
   d_jobGetTileT(0.0),
   m_inputChain(0),
   m_jobMtQueue(0),
   m_numThreads (num_threads),
   m_callback(new ossimGetTileCallback()),
   m_nextTileID (0),
   m_tileCache(),                       
   m_maxCacheSize (DEFAULT_MAX_TILE_CACHE_FACTOR * num_threads),
   m_maxTileCacheFactor (DEFAULT_MAX_TILE_CACHE_FACTOR),
   m_cacheMutex(),
   m_jobMutex(),
   m_totalNumberOfTiles(0),
   m_getTileBlock(),
   m_nextJobBlock(),
   d_printMutex(),
   d_timerMutex(),                                 
   d_debugEnabled(false),
   d_timedBlocksDt(0),
   d_timeMetricsEnabled(false),
   d_t1(0.0)                              
{
   //###### DEBUG ############
   ossimMtDebug* mt_debug = ossimMtDebug::instance();
   if (mt_debug->maxTileCacheSize != 0)
      m_maxCacheSize =  mt_debug->maxTileCacheSize;
   d_debugEnabled = mt_debug->seqDebugEnabled;
   d_timedBlocksDt = mt_debug->seqTimedBlocksDt;
   d_timeMetricsEnabled = mt_debug->seqMetricsEnabled;
   //###### END DEBUG ############

   // The base-class' initialize() method should have been called by the base class constructor
   // unless somebody moved it!
   OpenThreads::Thread::Init();
   m_nextJobBlock.release();
   m_getTileBlock.release();
   ossimTimer::instance()->setStartTick();
}

//*************************************************************************************************
// Destructor
//*************************************************************************************************
ossimMultiThreadSequencer::~ossimMultiThreadSequencer()
{
}

//*************************************************************************************************
//! Overrides base class in order to implement multi-threaded tile requests. 
//*************************************************************************************************
void ossimMultiThreadSequencer::setToStartOfSequence()
{
   // Reset important indices:
   theCurrentTileNumber = 0;
   m_nextTileID = 0;
   m_totalNumberOfTiles = theNumberOfTilesHorizontal * theNumberOfTilesVertical;

   //! The base class should have successfully assigned its input:
   if (theInputConnection ==  NULL)
      return;

   // Check if this param was already set externally. Query the system capability if not:
   if (m_numThreads == 0)
   {
      m_numThreads = 2 * ossim::getNumberOfThreads();
      m_maxCacheSize = m_maxTileCacheFactor * m_numThreads;
   }

   // Adapt the input source to be an ossimImageChainMtAdaptor since we can only work
   // with this type:
   m_inputChain = dynamic_cast<ossimImageChainMtAdaptor*>(theInputConnection);
   if (m_inputChain.valid())
   {
      m_inputChain->setNumberOfThreads(m_numThreads);
   }
   else
   {
      // Need to adapt input. First, is it a chain?
      ossimImageChain* chain = dynamic_cast<ossimImageChain*>(theInputConnection);
      if (chain == NULL)
      {
         // The input is just a common image source. Make it a chain:
         chain = new ossimImageChain;
         chain->add(theInputConnection);
      }

      // This instantiation creates a set of cloned image chains, one per thread, that will be
      // accessed in parallel for the getTile() operation:
      m_inputChain = new ossimImageChainMtAdaptor(chain, m_numThreads);
   }

   // Set the output of the chain to be this sequencer:
   m_inputChain->disconnectAllOutputs();
   //connectMyInputTo(m_inputChain.get());
   //setAreaOfInterest(m_inputChain->getBoundingRect());

   //// EXPERIMENTAL -- Fetch the first N tiles sequentially:
   for (ossim_uint32 i=0; i<m_numThreads; ++i)
   {
      ossimGetTileJob* job = new ossimGetTileJob(m_nextTileID++, i, *this);
      job->setCallback(m_callback.get());
      job->t_launchNewJob = false;
      job->start();
   }

   // Set up the job queue and fill it with first N jobs:
   ossim_uint32 num_jobs_to_launch =  min<ossim_uint32>(m_numThreads, m_totalNumberOfTiles);
   ossimRefPtr<ossimJobQueue> jobQueue = new ossimJobQueue();
   for (ossim_uint32 chain_id=0; chain_id<num_jobs_to_launch; ++chain_id)
   {
      if (d_debugEnabled)
      {
         ostringstream s;
         s<<"setToStartOfSequence() -- Creating tile/job #"<<m_nextTileID;
         print(s);
      }

      ossimGetTileJob* job = new ossimGetTileJob(m_nextTileID++, chain_id, *this);
      job->setCallback(m_callback.get());
      jobQueue->add(job, false);
   }

   // Initialize the multi-thread queue. Note the setQueue is done after construction as it was 
   // crashing do to jobs being launched during init:
   m_jobMtQueue = new ossimJobMultiThreadQueue(0, num_jobs_to_launch);
   m_jobMtQueue->setQueue(jobQueue.get());
}


//*************************************************************************************************
//! Overrides base class in order to implement multi-threaded tile requests. The output tile 
//! should be available in the tile cache, otherwise, method waits until it becomes available.
//*************************************************************************************************
ossimRefPtr<ossimImageData> ossimMultiThreadSequencer::getNextTile(ossim_uint32 /*resLevel*/)
{
   if (!m_inputChain.valid())
      return NULL;

   // May need to initiate the threaded sequencing if not already done:
   if (m_nextTileID == 0)
      setToStartOfSequence();

   // Terminate with null return if done:
   ossimRefPtr<ossimImageData> tile = 0;
   if (theCurrentTileNumber >= m_totalNumberOfTiles)
   {
      return tile;
   }

   // May need to wait until the corresponding job is finished if the tile is not in the cache:
   TileCache::iterator tile_iter = m_tileCache.begin();
   while (!tile.valid()) 
   {
      // If the tile is not yet copied into the cache, it means the job is still running. Let's 
      // block this thread and let the getTile jobs unlock as they finish. We'll exit this loop
      // when the job of interest finishes.
      if (d_timeMetricsEnabled)
         d_t1 = ossimTimer::instance()->time_s(); 
      m_cacheMutex.lock();
      if (d_timeMetricsEnabled)
         d_idleTime1 += ossimTimer::instance()->time_s() - d_t1; 

      tile_iter = m_tileCache.find(theCurrentTileNumber);
      m_cacheMutex.unlock();

      if (tile_iter == m_tileCache.end())
      {
         if (d_debugEnabled)
         {
            ostringstream s1;
            s1<<"getNextTile() -- Waiting on tile #"<<theCurrentTileNumber;
            m_cacheMutex.lock();
            s1<<"\n   cache size = "<<m_tileCache.size();
            TileCache::iterator iter = m_tileCache.begin();
            while(iter != m_tileCache.end())
            {
               s1<<"\n   cache.tile_id = "<<iter->first;
               iter++;
            }
            m_cacheMutex.unlock();
            print(s1);
         }

         if (d_timedBlocksDt > 0)
            m_getTileBlock.block(d_timedBlocksDt); 
         else
         {
            m_getTileBlock.reset();
            if (d_timeMetricsEnabled)
               d_t1 = ossimTimer::instance()->time_s(); 
            m_getTileBlock.block();
            if (d_timeMetricsEnabled)
               d_idleTime2 += ossimTimer::instance()->time_s() - d_t1; 
         }
      }
      else
      {
         // A valid tile was found. Need to assign the output tile and free up the reference in the
         // cache:
         if (d_debugEnabled)
         {
            ostringstream s2;
            s2<<"getNextTile() -- Copying tile #"<<theCurrentTileNumber<<".  Cache size: "<<m_tileCache.size();
            print(s2);
         }
         tile = tile_iter->second;
         
         if (d_timeMetricsEnabled)
            d_t1 = ossimTimer::instance()->time_s(); 
         m_cacheMutex.lock();
         m_tileCache.erase(tile_iter);
         m_cacheMutex.unlock();
         if (d_timeMetricsEnabled)
            d_idleTime3 += ossimTimer::instance()->time_s() - d_t1; 

         if (m_tileCache.empty()) 
            ++d_cacheEmptyCount; 
         m_nextJobBlock.release(); // nextJob() may be blocked until cache space is freed
      }
   }

   // Advance the caller-requested tile ID. This is different from the last threaded getTile()'s
   // tile index maintained in m_nextTileID and advanced in initNextJob():
   ++theCurrentTileNumber;
   return tile;
}

//*************************************************************************************************
// Specifies number of thread to support. Default behavior (if this method is never called) is
// query the system for number of cores available.
//*************************************************************************************************
void ossimMultiThreadSequencer::setNumberOfThreads(ossim_uint32 num_threads)
{
   m_numThreads = num_threads;
   m_maxCacheSize = m_maxTileCacheFactor * m_numThreads;

   if (m_inputChain.valid())
      m_inputChain->setNumberOfThreads(num_threads);

   if (m_jobMtQueue.valid() && m_jobMtQueue->hasJobsToProcess())
      m_jobMtQueue->getJobQueue()->clear();

   m_nextTileID = 0; // effectively resets this sequencer
}

//*************************************************************************************************
//! Access method to tile cache with scope lock to avoid multiple threads writing to
//! the cache simultaneously.
//*************************************************************************************************
void ossimMultiThreadSequencer::setTileInCache(ossim_uint32 tile_id, 
                                               ossimImageData* tile, 
                                               ossim_uint32 chain_id,
                                               double dt)
{
   if (d_timeMetricsEnabled)
      d_t1 = ossimTimer::instance()->time_s(); 
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_cacheMutex);
   if (d_timeMetricsEnabled)
      d_idleTime4 += ossimTimer::instance()->time_s() - d_t1; 

   d_jobGetTileT += dt;

   m_tileCache[tile_id] = tile;
   if (d_debugEnabled)
   {
      ostringstream s2;
      s2<<"THREAD #"<<chain_id<<" -- setTileInCache() Wrote tile #"<<tile_id;
      print(s2);
   }
   if (d_maxCacheUsed < m_tileCache.size())
      d_maxCacheUsed = (ossim_uint32) m_tileCache.size();
}

//*************************************************************************************************
// Queues up the next getTile job if cache is not full. This is called as soon as the job
// handling the corresponding chain ID is finished.
//*************************************************************************************************
void ossimMultiThreadSequencer::nextJob(ossim_uint32 chain_id)
{
   // Check for end of sequence:
   if (m_nextTileID >= m_totalNumberOfTiles)
      return;

   while (((ossim_uint32) m_tileCache.size()) >= m_maxCacheSize)
   {
      if (d_debugEnabled)
      {
         m_cacheMutex.lock();
         TileCache::const_iterator iter = m_tileCache.begin();
         ostringstream s1;
         s1<<"THREAD #"<<chain_id<<" -- nextJob() Waiting on cache before queuing tile/job #"
            <<m_nextTileID<<"using chain #"<<chain_id<<". Cache size: "<<m_tileCache.size();
         while(iter != m_tileCache.end())
         {
            s1<<"\n   cache.tile_id = "<<iter->first;
            iter++;
         }
         m_cacheMutex.unlock();
         print(s1);
      }

      if (d_timedBlocksDt > 0)
         m_nextJobBlock.block(d_timedBlocksDt);
      else
      {
         m_nextJobBlock.reset();
         if (d_timeMetricsEnabled)
            d_t1 = ossimTimer::instance()->time_s(); 
         m_nextJobBlock.block();
         if (d_timeMetricsEnabled)
         d_idleTime5 += ossimTimer::instance()->time_s() - d_t1; 
      }
   }

   if (d_debugEnabled)
   {
      ostringstream s2;
      s2<<"THREAD #"<<chain_id<<" -- nextJob() Queuing tile/job #"<<m_nextTileID;
      print(s2);
   }

   // Job queue will receive pointer into ossimRefPtr so no leak here:
   if (d_timeMetricsEnabled)
      d_t1 = ossimTimer::instance()->time_s(); 
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
   if (d_timeMetricsEnabled)
      d_idleTime6 += ossimTimer::instance()->time_s() - d_t1; 

   ossimGetTileJob* job = new ossimGetTileJob(m_nextTileID++, chain_id, *this);
   job->setCallback(m_callback.get());
   m_jobMtQueue->getJobQueue()->add(job);
}

//*************************************************************************************************
// For Debugging
//*************************************************************************************************
void ossimMultiThreadSequencer::print(ostringstream& msg) const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(d_printMutex);
   cerr << msg.str() << endl;
}
double ossimMultiThreadSequencer::handlerGetTileT() 
{
   if (m_inputChain->m_sharedHandlers.empty())
      return -1.0;
   ossimRefPtr<ossimImageHandlerMtAdaptor> ha = m_inputChain->m_sharedHandlers[0].get();
   if (ha.valid())
      return ha->d_getTileT;
   return -1;
}

