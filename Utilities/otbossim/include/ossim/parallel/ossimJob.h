#ifndef ossimJob_HEADER
#define ossimJob_HEADER
#include <ossim/base/ossimObject.h>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <list>

   class ossimJob;
   class OSSIM_DLL ossimJobCallback : public ossimReferenced
   {
   public:
      ossimJobCallback(){}
      virtual void ready(ossimJob* /*job*/){}
      virtual void started(ossimJob* /*job*/){}
      virtual void finished(ossimJob* /*job*/){}
      virtual void canceled(ossimJob* /*job*/){}
      virtual void logError(const ossimString& /*value*/){}
      virtual void logWarning(const ossimString& /*value*/){}
      virtual void logInfo(const ossimString& /*value*/){}
      
   };
   class OSSIM_DLL ossimJob : public ossimObject
   {
   public:
      typedef std::list<ossimRefPtr<ossimJob> > List;
      enum State
      {
         ossimJob_READY    = 0,
         ossimJob_RUNNING  = 1,
         ossimJob_CANCEL   = 2,
         ossimJob_FINISHED = 4,
         
      };
      virtual void start()=0;
      ossimJob()
      :m_state(ossimJob_READY),
      m_priority(0.0)
      {
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
      void setState(State value);
      bool isCanceled()const
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         return (m_state == ossimJob_CANCEL);
      }
      void cancel()
      {
         setState(ossimJob_CANCEL);
      }
      bool isReady()const
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         return m_state == ossimJob_READY;
      }
      bool isStopped()const
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         return ((m_state == ossimJob_CANCEL)||
                 (m_state == ossimJob_FINISHED));
      }
      
      void setCallback(ossimJobCallback* callback)
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         m_callback = callback;
      }
      void setName(const ossimString& value)
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         m_name = value;
      }
      const ossimString& name()const
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         return m_name;
      }
      void setId(const ossimString& value)
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         m_id = value;
      }
      const ossimString& id()const
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         return m_id;
      }
      void setDescription(const ossimString& value)
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         m_description = value;
      }
      const ossimString& description()const
      {
         OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
         return m_description;
      }
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
