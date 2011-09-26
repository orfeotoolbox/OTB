#ifndef ossimJobMultiThreadQueue_HEADER
#define ossimJobMultiThreadQueue_HEADER
#include <ossim/parallel/ossimJobThreadQueue.h>

class OSSIM_DLL ossimJobMultiThreadQueue : public ossimReferenced
{
public:
   typedef std::vector<ossimRefPtr<ossimJobThreadQueue> > ThreadQueueList;
   
   ossimJobMultiThreadQueue(ossimJobQueue* q=0, ossim_uint32 nThreads=0);
   ossimJobQueue* getJobQueue();
   const ossimJobQueue* getJobQueue()const;
   void setQueue(ossimJobQueue* q);
   void setNumberOfThreads(ossim_uint32 nThreads);
   ossim_uint32 getNumberOfThreads() const;
   ossim_uint32 numberOfBusyThreads()const;
   bool areAllThreadsBusy()const;
   
   bool hasJobsToProcess()const;

protected:
   mutable OpenThreads::Mutex  m_mutex;
   ossimRefPtr<ossimJobQueue> m_jobQueue;
   ThreadQueueList m_threadQueueList;
};

#endif
