#include <ossim/parallel/ossimJob.h>

void ossimJob::setState(int value, bool on)
{
   // we will need to make sure that the state flags are set properly
   // so if you turn on running then you can't have finished or ready turned onturned on
   // but can stil have cancel turned on
   //
   int newState = m_state;
   if(on)
   {
      newState = ((newState | value)&ossimJob_ALL);
   }
   else 
   {
      newState = ((newState & ~value)&ossimJob_ALL);
   }

   int oldState     = 0;
   int currentState = 0;
   ossimRefPtr<ossimJobCallback> callback;

   bool stateChangedFlag = false;
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_jobMutex);
      
      stateChangedFlag = newState != m_state;
      oldState = m_state;
      m_state = static_cast<State>(newState);
      currentState = m_state;
      callback = m_callback.get();
   }
   
   if(stateChangedFlag&&callback.valid())
   {
      if(callback.valid())
      {
         if(!(oldState&ossimJob_READY)&&
            (currentState&ossimJob_READY))
         {
            callback->ready(this);
         }
         else if(!(oldState&ossimJob_RUNNING)&&
                 (currentState&ossimJob_RUNNING))
         {
            callback->started(this);
         }
         else if(!(oldState&ossimJob_CANCEL)&&
                 (currentState&ossimJob_CANCEL))
         {
            callback->canceled(this);
         }
         else if(!(oldState&ossimJob_FINISHED)&&
                 (currentState&ossimJob_FINISHED))
         {
            callback->finished(this);
         }
      }
   }
}
