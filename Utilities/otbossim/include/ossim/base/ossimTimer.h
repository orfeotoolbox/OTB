/* 
 * This code was taken from the OpenScene graph.
 */

#ifndef ossimTimer_HEADER
#define ossimTimer_HEADER 1
#include "ossimConstants.h"

/** Timer class is used for measuring elapsed time or time between two points. */
class OSSIM_DLL ossimTimer 
{
public:
#if defined(_MSC_VER)
   typedef __int64 Timer_t;
#else
   typedef unsigned long long Timer_t;
#endif
   ossimTimer();
   ~ossimTimer() {m_instance = 0;}
   
   static ossimTimer* instance();
   
   /** Get the timers tick value.*/
   Timer_t tick() const;
   
   /** Set the start.*/
   void setStartTick() { m_startTick = tick(); }
   void setStartTick(Timer_t t) { m_startTick = t; }
   Timer_t getStartTick() const { return m_startTick; }
   
   
   /** Get elapsed time in seconds.*/
   inline double time_s() const { return delta_s(m_startTick, tick()); }
   
   /** Get elapsed time in milliseconds.*/
   inline double time_m() const { return delta_m(m_startTick, tick()); }
   
   /** Get elapsed time in micoseconds.*/
   inline double time_u() const { return delta_u(m_startTick, tick()); }
   
   /** Get elapsed time in nanoseconds.*/
   inline double time_n() const { return delta_n(m_startTick, tick()); }
   
   /** Get the time in seconds between timer ticks t1 and t2.*/
   inline double delta_s( Timer_t t1, Timer_t t2 ) const { return (double)(t2 - t1)*m_secsPerTick; }
   
   /** Get the time in milliseconds between timer ticks t1 and t2.*/
   inline double delta_m( Timer_t t1, Timer_t t2 ) const { return delta_s(t1,t2)*1e3; }
   
   /** Get the time in microseconds between timer ticks t1 and t2.*/
   inline double delta_u( Timer_t t1, Timer_t t2 ) const { return delta_s(t1,t2)*1e6; }
   
   /** Get the time in nanoseconds between timer ticks t1 and t2.*/
   inline double delta_n( Timer_t t1, Timer_t t2 ) const { return delta_s(t1,t2)*1e9; }
   
   /** Get the the number of seconds per tick. */
   inline double getSecondsPerTick() const { return m_secsPerTick; }
   
protected:
   static ossimTimer* m_instance;
   Timer_t m_startTick;
   double  m_secsPerTick;
   
};
#endif
