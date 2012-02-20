#include<ossim/parallel/ossimJobThreadQueue.h>

ossimJobThreadQueue::ossimJobThreadQueue(ossimJobQueue* jqueue)
:m_doneFlag(false)
{
   setJobQueue(jqueue);    
}
void ossimJobThreadQueue::setJobQueue(ossimJobQueue* jqueue)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
   
   if (m_jobQueue == jqueue) return;
   
   if(isRunning())
   {
      ossimRefPtr<ossimJobQueue> jobQueueTemp = m_jobQueue;
      m_jobQueue = jqueue;
      if(jobQueueTemp.valid())
      {
         jobQueueTemp->releaseBlock();
      }
   }
   else 
   {
      m_jobQueue = jqueue;
   }
   
   startThreadForQueue();
}

ossimJobQueue* ossimJobThreadQueue::getJobQueue() 
{ 
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
   return m_jobQueue.get(); 
}

const ossimJobQueue* ossimJobThreadQueue::getJobQueue() const 
{ 
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
   return m_jobQueue.get(); 
}

ossimRefPtr<ossimJob> ossimJobThreadQueue::currentJob() 
{ 
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
   return m_currentJob; 
}

void ossimJobThreadQueue::cancelCurrentJob()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
   if(m_currentJob.valid())
   {
      m_currentJob->cancel();
   }
}
bool ossimJobThreadQueue::isValidQueue()const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
   return m_jobQueue.valid();
}

void ossimJobThreadQueue::run()
{
   bool firstTime = true;
   bool validQueue = true;
   ossimRefPtr<ossimJob> job;
   do
   {
      // osg::notify(osg::NOTICE)<<"In thread loop "<<this<<std::endl;
      validQueue = isValidQueue();
      job = nextJob();
      if (job.valid()&&!m_doneFlag)
      {
         {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
            m_currentJob = job;
         }
         
         // if the job is ready to execute
         if(job->isReady())
         {
            job->resetState(ossimJob::ossimJob_RUNNING);
            job->start();
         }
         {            
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
            m_currentJob = 0;
         }
         job->setState(ossimJob::ossimJob_FINISHED);
         job = 0;
      }
      
      if (firstTime)
      {
         // do a yield to get round a peculiar thread hang when testCancel() is called 
         // in certain cirumstances - of which there is no particular pattern.
         YieldCurrentThread();
         firstTime = false;
      }
   } while (!m_doneFlag&&validQueue);
   
   if(job.valid()&&m_doneFlag&&job->isReady())
   {
      {            
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
         m_currentJob = 0;
      }
      job->cancel();
   }
   {            
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
      m_currentJob = 0;
   }
   job = 0;
}

void ossimJobThreadQueue::setDone(bool done)
{
   m_threadMutex.lock();
   if (m_doneFlag==done)
   {
      m_threadMutex.unlock();
      return;
   }
   m_doneFlag = done;
   m_threadMutex.unlock();
   if(done)
   {
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
         if (m_currentJob.valid())
            m_currentJob->release();
      }
      
      if (m_jobQueue.valid())
         m_jobQueue->releaseBlock();
   }
}

bool ossimJobThreadQueue::isDone() const 
{ 
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
   return m_doneFlag; 
}

bool ossimJobThreadQueue::isProcessingJob()const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
   return m_currentJob.valid();
}

int ossimJobThreadQueue::cancel()
{
   
   if( isRunning() )
   {
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
         m_doneFlag = true;
         if (m_currentJob.valid())
         {
            m_currentJob->cancel();
         }
         
         if (m_jobQueue.valid()) 
         {
            m_jobQueue->releaseBlock();
         }
      }
      
      // then wait for the the thread to stop running.
      while(isRunning())
      {
#if 1
         {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
            
            if (m_jobQueue.valid()) 
            {
               m_jobQueue->releaseBlock();
            }
         }
#endif
         OpenThreads::Thread::YieldCurrentThread();
      }
   }
   return OpenThreads::Thread::cancel();
}

bool ossimJobThreadQueue::isEmpty()const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
   return m_jobQueue->isEmpty();
}

ossimJobThreadQueue::~ossimJobThreadQueue()
{
   cancel();
}

void ossimJobThreadQueue::startThreadForQueue()
{
   if(m_jobQueue.valid())
   {
      if(!isRunning())
      {
         start();
         while(!isRunning()) // wait for the thread to start running
         {
            OpenThreads::Thread::YieldCurrentThread();
         }
      }
   }
}

bool ossimJobThreadQueue::hasJobsToProcess()const
{
   bool result = false;
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_threadMutex);
      result = !m_jobQueue->isEmpty()||m_currentJob.valid();
   }
   
   return result;
}

ossimRefPtr<ossimJob> ossimJobThreadQueue::nextJob()
{
   ossimRefPtr<ossimJob> job;
   m_threadMutex.lock();
   ossimRefPtr<ossimJobQueue> jobQueue = m_jobQueue;
   bool checkIfValid = !m_doneFlag&&jobQueue.valid();
   m_threadMutex.unlock();
   if(checkIfValid)
   {
      return jobQueue->nextJob(true);
   }
   return 0;
}
