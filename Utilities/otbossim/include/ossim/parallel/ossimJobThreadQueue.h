#ifndef ossimJobThreadQueue_HEADER
#define ossimJobThreadQueue_HEADER
#include <ossim/parallel/ossimJobQueue.h>
#include <OpenThreads/Mutex>
#include <OpenThreads/Thread>

class OSSIM_DLL ossimJobThreadQueue : public ossimReferenced, 
                                      public OpenThreads::Thread
{
public:
   ossimJobThreadQueue(ossimJobQueue* jqueue=0);
   void setJobQueue(ossimJobQueue* jqueue);
   
   ossimJobQueue* getJobQueue();
   
   const ossimJobQueue* getJobQueue() const; 
   
   ossimRefPtr<ossimJob> currentJob();
   
   void cancelCurrentJob();
   bool isValidQueue()const;
   
   virtual void run();
   
   void setDone(bool done);
   
   bool isDone()const;
   virtual int cancel();
   bool isEmpty()const;
   
   bool isProcessingJob()const;
   
   bool hasJobsToProcess()const;
   
protected:
   virtual ~ossimJobThreadQueue();
   
   void startThreadForQueue();
   virtual ossimRefPtr<ossimJob> nextJob();
   
   bool                       m_doneFlag;
   mutable OpenThreads::Mutex m_threadMutex;
   ossimRefPtr<ossimJobQueue> m_jobQueue;
   ossimRefPtr<ossimJob>      m_currentJob;
   
};

#endif
