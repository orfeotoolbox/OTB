/* 
 * This code was taken directly from the OpenSceneGraph
 */
//#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTimer.h>

ossimTimer* ossimTimer::m_instance = 0;

// follows are the constructors of the Timer class, once version
// for each OS combination.  The order is WIN32, FreeBSD, Linux, IRIX,
// and the rest of the world.
//
// all the rest of the timer methods are implemented within the header.


ossimTimer* ossimTimer::instance()
{
   if(!m_instance)
   {
      m_instance = new ossimTimer;
   }
   return m_instance;
}

// ---
// From:  http://msdn.microsoft.com/en-us/library/b0084kay.aspx
// Defined for applications for Win32 and Win64. Always defined.
// ---
#if defined(_WIN32)

#include <sys/types.h>
#include <fcntl.h>
#include <windows.h>
#include <winbase.h>
ossimTimer::ossimTimer()
{
   LARGE_INTEGER frequency;
   if(QueryPerformanceFrequency(&frequency))
   {
      m_secsPerTick = 1.0/(double)frequency.QuadPart;
   }
   else
   {
      m_secsPerTick = 1.0;
      ossimNotify(ossimNotifyLevel_NOTICE)<<"Error: ossimTimer::ossimTimer() unable to use QueryPerformanceFrequency, "<<std::endl;
      ossimNotify(ossimNotifyLevel_NOTICE)<<"timing code will be wrong, Windows error code: "<<GetLastError()<<std::endl;
   }
   
   setStartTick();        
}

ossimTimer::Timer_t ossimTimer::tick() const
{
   LARGE_INTEGER qpc;
   if (QueryPerformanceCounter(&qpc))
   {
      return qpc.QuadPart;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_NOTICE)<<"Error: ossimTimer::ossimTimer() unable to use QueryPerformanceCounter, "<<std::endl;
      ossimNotify(ossimNotifyLevel_NOTICE)<<"timing code will be wrong, Windows error code: "<<GetLastError()<<std::endl;
      return 0;
   }
}

#else

#include <sys/time.h>

ossimTimer::ossimTimer(  )
{
   m_secsPerTick = (1.0 / (double) 1000000);
   
   setStartTick();        
}

ossimTimer::Timer_t ossimTimer::tick() const
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return ((ossimTimer::Timer_t)tv.tv_sec)*1000000+(ossimTimer::Timer_t)tv.tv_usec;
}

#endif
