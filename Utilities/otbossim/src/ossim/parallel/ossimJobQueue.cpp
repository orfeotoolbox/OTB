#include <ossim/parallel/ossimJobQueue.h>

#include <algorithm> /* for std::find */


ossimJobQueue::ossimJobQueue()
{
}

void ossimJobQueue::add(ossimJob* job, bool guaranteeUniqueFlag)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobQueueMutex);
   
   if(guaranteeUniqueFlag)
   {
      if(findByPointer(job) != m_jobQueue.end())
      {
         m_block.set(true);
         return;
      }
   }
   m_jobQueue.push_back(job);
   
   m_block.set(true);
}

ossimRefPtr<ossimJob> ossimJobQueue::removeByName(const ossimString& name)
{
   ossimRefPtr<ossimJob> result;
   if(name.empty()) return result;
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobQueueMutex);
   ossimJob::List::iterator iter = findByName(name);
   if(iter!=m_jobQueue.end())
   {
      result = *iter;
      m_jobQueue.erase(iter);
   }
   
   m_block.set(!m_jobQueue.empty());
   
   return result;
}
ossimRefPtr<ossimJob> ossimJobQueue::removeById(const ossimString& id)
{
   ossimRefPtr<ossimJob> result;
   if(id.empty()) return result;
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobQueueMutex);
   ossimJob::List::iterator iter = findById(id);
   if(iter!=m_jobQueue.end())
   {
      result = *iter;
      m_jobQueue.erase(iter);
   }
   
   m_block.set(!m_jobQueue.empty());
   
   return result;
}

void ossimJobQueue::remove(const ossimJob* Job)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobQueueMutex);
   ossimJob::List::iterator iter = std::find(m_jobQueue.begin(), m_jobQueue.end(), Job);
   if(iter!=m_jobQueue.end())
   {
      m_jobQueue.erase(iter);
   }
}

void ossimJobQueue::removeStoppedJobs()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobQueueMutex);
   ossimJob::List::iterator iter = m_jobQueue.begin();
   while(iter!=m_jobQueue.end())
   {
      if((*iter)->isStopped())
      {
         iter = m_jobQueue.erase(iter);
      }
      else 
      {
         ++iter;
      }
   }
}
void ossimJobQueue::clear()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobQueueMutex);
   m_jobQueue.clear();
}

ossimRefPtr<ossimJob> ossimJobQueue::nextJob(bool blockIfEmptyFlag)
{
   m_jobQueueMutex.lock();
   bool emptyFlag = m_jobQueue.empty();
   m_jobQueueMutex.unlock();
   if (blockIfEmptyFlag && emptyFlag)
   {
      m_block.block();
   }
   
   ossimRefPtr<ossimJob> result;
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobQueueMutex);
   
   if (m_jobQueue.empty())
   {
      m_block.set(false);
      return result;
   }
   
   ossimJob::List::iterator iter= m_jobQueue.begin();
   while((iter != m_jobQueue.end())&&
         (((*iter)->isStopped())))
   {
      iter = m_jobQueue.erase(iter);
   }
   if(iter != m_jobQueue.end())
   {
      result = *iter;
      m_jobQueue.erase(iter);
   }
   m_block.set(!m_jobQueue.empty());
   return result;
}
void ossimJobQueue::releaseBlock()
{
   m_block.release();
}
bool ossimJobQueue::isEmpty()const
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobQueueMutex);
   return m_jobQueue.empty();
}

ossim_uint32 ossimJobQueue::size()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobQueueMutex);
   return m_jobQueue.size();
}

ossimJob::List::iterator ossimJobQueue::findById(const ossimString& id)
{
   if(id.empty()) return m_jobQueue.end();
   ossimJob::List::iterator iter = m_jobQueue.begin();
   while(iter != m_jobQueue.end())
   {
      if(id == (*iter)->id())
      {
         return iter;
      }
      ++iter;
   }  
   return m_jobQueue.end();
}

ossimJob::List::iterator ossimJobQueue::findByName(const ossimString& name)
{
   if(name.empty()) return m_jobQueue.end();
   ossimJob::List::iterator iter = m_jobQueue.begin();
   while(iter != m_jobQueue.end())
   {
      if(name == (*iter)->name())
      {
         return iter;
      }
      ++iter;
   }  
   return m_jobQueue.end();
}

ossimJob::List::iterator ossimJobQueue::findByPointer(const ossimJob* job)
{
   return std::find(m_jobQueue.begin(),
                    m_jobQueue.end(),
                    job);
}

ossimJob::List::iterator ossimJobQueue::findByNameOrPointer(const ossimJob* job)
{
   ossimString n = job->name();
   ossimJob::List::iterator iter = m_jobQueue.begin();
   while(iter != m_jobQueue.end())
   {
      if((*iter).get() == job)
      {
         return iter;
      }
      else if((!n.empty())&&
              (job->name() == (*iter)->name()))
      {
         return iter;
      }
      ++iter;
   }  
   
   return m_jobQueue.end();
}

bool ossimJobQueue::hasJob(ossimJob* job)
{
   ossimJob::List::const_iterator iter = m_jobQueue.begin();
   while(iter != m_jobQueue.end())
   {
      if(job == (*iter).get())
      {
         return true;
      }
      ++iter;
   }
   
   return false;
}

