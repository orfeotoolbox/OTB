#include <ossim/parallel/ossimJob.h>

void ossimJob::setState(ossimJob::State value)
{
   bool stateChangedFlag = false;
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      
      if(value != m_state)
      {
         stateChangedFlag = true;
         m_state = value;
      }
   }
   
   if(stateChangedFlag)
   {
      m_jobMutex.lock();
      ossimRefPtr<ossimJobCallback> callback = m_callback.get();
      m_jobMutex.unlock();
      if(callback.valid())
      {
         switch(value)
         {
            case ossimJob_READY:
            {
               callback->ready(this);
               break;
            }
            case ossimJob_RUNNING:
            {
               callback->started(this);
               break;
            }
            case ossimJob_CANCEL:
            {
               callback->canceled(this);
               break;
            }
            case ossimJob_FINISHED:
            {
               callback->finished(this);
               break;
            }
         }
      }
   }
}
