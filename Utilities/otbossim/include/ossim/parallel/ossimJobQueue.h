//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
//**************************************************************************************************
//  $Id$
#ifndef ossimJobQueue_HEADER
#define ossimJobQueue_HEADER

#include <ossim/parallel/ossimJob.h>
#include <OpenThreads/Block>

//*************************************************************************************************
//! Class for maintaining an ordered list of jobs to be processed. As the jobs are completed and
//! the product consumed, the jobs are removed from this list
//*************************************************************************************************
class OSSIM_DLL ossimJobQueue : public ossimReferenced
{
public:
   class OSSIM_DLL Callback : public ossimReferenced
   {
   public:
      Callback(){}
      virtual void adding(ossimJobQueue* /*q*/, ossimJob* /*job*/){}
      virtual void added(ossimJobQueue* /*q*/, ossimJob* /*job*/){}
      virtual void removed(ossimJobQueue* /*q*/, ossimJob* /*job*/){}
   protected:
   };
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
   void setCallback(Callback* c);
   Callback* callback();
   
protected:
   ossimJob::List::iterator findById(const ossimString& id);
   ossimJob::List::iterator findByName(const ossimString& name);
   ossimJob::List::iterator findByPointer(const ossimJob* job);
   ossimJob::List::iterator findByNameOrPointer(const ossimJob* job);
   bool hasJob(ossimJob* job);
   
   mutable OpenThreads::Mutex m_jobQueueMutex;
   OpenThreads::Block m_block;
   ossimJob::List m_jobQueue;
   ossimRefPtr<Callback> m_callback;
};

#endif
