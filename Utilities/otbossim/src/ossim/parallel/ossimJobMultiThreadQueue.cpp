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
   if(nThreads != m_threadQueueList.size())
   {
      if(nThreads > queueSize)
      {
         for(idx = queueSize; idx < nThreads;++idx)
         {
            ossimRefPtr<ossimJobThreadQueue> threadQueue = new ossimJobThreadQueue();
            threadQueue->setJobQueue(m_jobQueue.get());
            m_threadQueueList.push_back(threadQueue);
         }
      }
      else 
      {
         ThreadQueueList::iterator iter = m_threadQueueList.begin()+nThreads;
         while(iter != m_threadQueueList.end())
         {
            (*iter)->cancel();
            iter = m_threadQueueList.erase(iter);
         }
      }
   }
}
