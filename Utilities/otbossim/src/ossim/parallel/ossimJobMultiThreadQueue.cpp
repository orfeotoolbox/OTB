#include <ossim/parallel/ossimJobMultiThreadQueue.h>

ossimJobMultiThreadQueue::ossimJobMultiThreadQueue(ossimJobQueue* q, ossim_uint32 nThreads)
:m_jobQueue(q?q:new ossimJobQueue())
{
   setNumberOfThreads(nThreads);
}
ossimJobQueue* ossimJobMultiThreadQueue::getJobQueue()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   return m_jobQueue.get();
}
const ossimJobQueue* ossimJobMultiThreadQueue::getJobQueue()const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   return m_jobQueue.get();
}
void ossimJobMultiThreadQueue::setQueue(ossimJobQueue* q)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   ossim_uint32 idx = 0;
   m_jobQueue = q;
   for(idx = 0; idx < m_threadQueueList.size(); ++idx)
   {
      m_threadQueueList[idx]->setJobQueue(m_jobQueue.get());
   }
}
void ossimJobMultiThreadQueue::setNumberOfThreads(ossim_uint32 nThreads)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   ossim_uint32 idx = 0;
   ossim_uint32 queueSize = m_threadQueueList.size();
   
   if(nThreads > queueSize)
   {
      for(idx = queueSize; idx < nThreads;++idx)
      {
         ossimRefPtr<ossimJobThreadQueue> threadQueue = new ossimJobThreadQueue();
         threadQueue->setJobQueue(m_jobQueue.get());
         m_threadQueueList.push_back(threadQueue);
      }
   }
   else if(nThreads < queueSize)
   {
      ThreadQueueList::iterator iter = m_threadQueueList.begin()+nThreads;
      while(iter != m_threadQueueList.end())
      {
         (*iter)->cancel();
         iter = m_threadQueueList.erase(iter);
      }
   }
}

ossim_uint32 ossimJobMultiThreadQueue::getNumberOfThreads() const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   return static_cast<ossim_uint32>( m_threadQueueList.size() );
}

ossim_uint32 ossimJobMultiThreadQueue::numberOfBusyThreads()const
{
   ossim_uint32 result = 0;
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   ossim_uint32 idx = 0;
   ossim_uint32 queueSize = m_threadQueueList.size();
   for(idx = 0; idx < queueSize;++idx)
   {
      if(m_threadQueueList[idx]->isProcessingJob()) ++result;
   }
   return result;
}

bool ossimJobMultiThreadQueue::areAllThreadsBusy()const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   ossim_uint32 idx = 0;
   ossim_uint32 queueSize = m_threadQueueList.size();
   for(idx = 0; idx < queueSize;++idx)
   {
      if(!m_threadQueueList[idx]->isProcessingJob()) return false;
   }
   
   return true;
}

bool ossimJobMultiThreadQueue::hasJobsToProcess()const
{
   bool result = false;
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
      ossim_uint32 queueSize = m_threadQueueList.size();
      ossim_uint32 idx = 0;
      for(idx = 0; ((idx<queueSize)&&!result);++idx)
      {
         result = m_threadQueueList[idx]->hasJobsToProcess();
      }
   }
   
   return result;
}

