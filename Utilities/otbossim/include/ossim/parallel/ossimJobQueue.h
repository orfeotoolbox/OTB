#ifndef ossimJobQueue_HEADER
#define ossimJobQueue_HEADER
#include <ossim/parallel/ossimJob.h>
#include <OpenThreads/Block>
class OSSIM_DLL ossimJobQueue : public ossimReferenced
{
public:
   ossimJobQueue();
   
   virtual void add(ossimJob* job, bool guaranteeUniqueFlag=true);
   virtual ossimRefPtr<ossimJob> removeByName(const ossimString& name);
   virtual ossimRefPtr<ossimJob> removeById(const ossimString& id);
   virtual void remove(const ossimJob* Job);
   virtual void removeStoppedJobs();
   virtual void clear();
   virtual ossimRefPtr<ossimJob> nextJob(bool blockIfEmptyFlag=true);
   virtual void releaseBlock();
   bool isEmpty()const;
   ossim_uint32 size();
protected:
   ossimJob::List::iterator findById(const ossimString& id);
   ossimJob::List::iterator findByName(const ossimString& name);
   ossimJob::List::iterator findByPointer(const ossimJob* job);
   ossimJob::List::iterator findByNameOrPointer(const ossimJob* job);
   bool hasJob(ossimJob* job);
   
   mutable OpenThreads::Mutex m_jobQueueMutex;
   OpenThreads::Block m_block;
   ossimJob::List m_jobQueue;
   
};

#endif
