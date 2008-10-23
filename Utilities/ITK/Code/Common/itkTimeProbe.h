/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTimeProbe.h,v $
  Language:  C++
  Date:      $Date: 2008-04-03 16:21:58 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTimeProbe_h
#define __itkTimeProbe_h

#include "itkConfigure.h"

#ifdef ITK_USE_REVIEW
#include "../Review/itkTimeProbe.h"
#else
#include "itkRealTimeClock.h"


namespace itk 
{
 
  /** \class TimeProbe 
   * 
   *  \brief Class for computing the time passed between two points in the code.
   *   
   *   This class allows the user to trace the time passed between the execution of two pieces of code.
   *   It can be started and stopped in order to evaluate the execution over multiple passes.
   *   The values of time are taken from the RealTimeClock.
   *
   *   \sa RealTimeClock
   *
   */
class ITKCommon_EXPORT TimeProbe
{

public:

  /** Type for counting how many times the probe has been started and stopped. */
  typedef unsigned long CountType;

  /** Type for measuring time. See the RealTimeClock class for details on the
   * precision and units of this clock signal */
  typedef RealTimeClock::TimeStampType  TimeStampType;

public:

  /** Constructor */
  TimeProbe();

  /** Destructor */
  ~TimeProbe();

  /** Start counting the time */
  void Start(void);

  /** Stop counting the time */
  void Stop(void);

  /** Returns the number of times that the probe has been started. */
  CountType     GetNumberOfStarts(void) const;

  /** Returns the number of times that the probe has been stopped. */
  CountType     GetNumberOfStops(void) const;

  /** Returns the average times passed between the starts and stops of the
   * probe. See the RealTimeClock for details on the precision and units of
   * this time value. */
  TimeStampType GetMeanTime(void) const;

private:

    TimeStampType   m_Start;
    TimeStampType   m_TotalTime;
    
    CountType       m_NumberOfStarts;
    CountType       m_NumberOfStops;

    RealTimeClock::Pointer   m_RealTimeClock;
};


}

#endif // ITK_USE_REVIEW

#endif
