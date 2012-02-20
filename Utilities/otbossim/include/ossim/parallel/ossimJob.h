//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
//**************************************************************************************************
//  $Id$
#ifndef ossimJob_HEADER
#define ossimJob_HEADER
#include <ossim/base/ossimObject.h>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <list>

class ossimJob;

//*************************************************************************************************
//! Generic callback class needed by ossimJob
//*************************************************************************************************
class OSSIM_DLL ossimJobCallback : public ossimReferenced
{
public:
   ossimJobCallback(ossimJobCallback* nextCallback=0):m_nextCallback(nextCallback){}

   virtual void ready(ossimJob* job)    {if(m_nextCallback.valid()) m_nextCallback->ready(job);   }
   virtual void started(ossimJob* job)  {if(m_nextCallback.valid()) m_nextCallback->started(job); }
   virtual void finished(ossimJob* job) {if(m_nextCallback.valid()) m_nextCallback->finished(job);}
   virtual void canceled(ossimJob* job) {if(m_nextCallback.valid()) m_nextCallback->canceled(job);}

   virtual void nameChanged(const ossimString& name, ossimJob* job)
   {if(m_nextCallback.valid()) m_nextCallback->nameChanged(name, job);}
   
   virtual void descriptionChanged(const ossimString& description, ossimJob* job)
   {if(m_nextCallback.valid()) m_nextCallback->descriptionChanged(description, job);}

   virtual void idChanged(const ossimString& id, ossimJob* job)
   {if(m_nextCallback.valid()) m_nextCallback->idChanged(id, job);}

   virtual void percentCompleteChanged(double percentValue, ossimJob* job)
   {if(m_nextCallback.valid()) m_nextCallback->percentCompleteChanged(percentValue, job);}

   void setCallback(ossimJobCallback* c){m_nextCallback = c;}
   ossimJobCallback* callback(){return m_nextCallback.get();}

protected:
   ossimRefPtr<ossimJobCallback> m_nextCallback;
};


//*************************************************************************************************
//! Pure virtual base class for all job types
//*************************************************************************************************
class OSSIM_DLL ossimJob : public ossimObject
{
public:
   typedef std::list<ossimRefPtr<ossimJob> > List;

   /** 
   * This is a Bit vector.  The only value that can be assigned as both active is FINISHED and CANCEL.
   * CANCELED job may not yet be finished.  Once the job is finished the Cancel is complete
   */ 
   enum State
   {
      ossimJob_NONE     = 0,
      ossimJob_READY    = 1,
      ossimJob_RUNNING  = 2,
      ossimJob_CANCEL   = 4,
      ossimJob_FINISHED = 8,
      ossimJob_ALL = (ossimJob_READY|ossimJob_RUNNING|ossimJob_CANCEL|ossimJob_FINISHED)
   };
   
   ossimJob() : m_state(ossimJob_READY),  m_priority(0.0) {}

   virtual void start()=0;
   
   void setPercentComplete(double value)
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      if(m_callback.valid())
      {
         m_callback->percentCompleteChanged(value, this);
      }
   }

   void setPriority(double value)
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      m_priority = value;
   }

   double priority()const
   {
      return m_priority;
   }

   virtual void release(){}

   State state()const
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      return m_state;
   }

   virtual void resetState(int value)
   {
      m_jobMutex.lock();
      if(value != m_state)
      {
         m_state = ossimJob_NONE;
         m_jobMutex.unlock();
         setState(value);
      }
      else 
      {
         m_jobMutex.unlock();
      }

   }

   virtual void setState(int value, bool on=true);

   bool isCanceled()const
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      return (m_state & ossimJob_CANCEL);
   }

   virtual void cancel()
   {
      // append the cancel flag to current state
      setState(ossimJob_CANCEL);
   }

   virtual void ready()
   {
      resetState(ossimJob_READY);
   }

   virtual void running()
   {
      resetState(ossimJob_RUNNING);
   }

   virtual void finished()
   {
      int newState = 0;
      {
         // maintain the cancel flag so we can indicate the job has now finished
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         newState = ((m_state & ossimJob_CANCEL) | 
            (ossimJob_FINISHED));
      }
      // now reset to the new state
      resetState(newState);
   }

   bool isReady()const
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      return m_state & ossimJob_READY;
   }

   bool isStopped()const
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      return (m_state & ossimJob_FINISHED);
   }

   bool isFinished()const
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      return (m_state & ossimJob_FINISHED);
   }

   bool isRunning()const
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      return (m_state & ossimJob_RUNNING);
   }

   void setCallback(ossimJobCallback* callback)
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      m_callback = callback;
   }

   void setName(const ossimString& value)
   {
      bool changed = false;
      ossimRefPtr<ossimJobCallback> callback;
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         changed = value!=m_name;
         m_name = value;
         callback = m_callback;
      }
      if(changed&&callback.valid())
      {
         callback->nameChanged(value, this);
      }
   }

   const ossimString& name()const
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      return m_name;
   }

   void setId(const ossimString& value)
   {
      bool changed = false;
      ossimRefPtr<ossimJobCallback> callback;
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         changed = value!=m_id;
         m_id = value;
         callback = m_callback;
      }
      if(changed&&callback.valid())
      {
         callback->idChanged(value, this);
      }
   }

   const ossimString& id()const
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      return m_id;
   }

   void setDescription(const ossimString& value)
   {
      bool changed = false;
      ossimRefPtr<ossimJobCallback> callback;
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         changed = value!=m_description;
         m_description = value;
         callback = m_callback;
      }
      if(changed&&callback.valid())
      {
         callback->descriptionChanged(value, this);
      }
   }

   const ossimString& description()const
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      return m_description;
   }
   ossimJobCallback* callback() {return m_callback.get();}

protected:
   mutable OpenThreads::Mutex m_jobMutex;
   ossimString m_name;
   ossimString m_description;
   ossimString m_id;
   State       m_state;
   double      m_priority;
   ossimRefPtr<ossimJobCallback> m_callback;
};

#endif
