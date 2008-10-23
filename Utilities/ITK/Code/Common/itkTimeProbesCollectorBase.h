/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTimeProbesCollectorBase.h,v $
  Language:  C++
  Date:      $Date: 2008-04-03 16:21:58 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTimeProbesCollectorBase_h
#define __itkTimeProbesCollectorBase_h

#include "itkConfigure.h"

#ifdef ITK_USE_REVIEW
#include "../Review/itkTimeProbesCollectorBase.h"
#else

#include "itkMacro.h"
#include "itkTimeProbe.h"
#include <map>
#include <string>

namespace itk
{

  /** \class TimeProbesCollectorBase
   *  \brief Class for aggregating a set of time probes. 
   *
   *  This class defines a set of TimeProbes and assign names to them.
   *  The user can start and stop each one of the probes by addressing them by name.
   *  
   *  \sa TimeProbe
   *  \sa RealTimeClock
   *
   */
class ITKCommon_EXPORT TimeProbesCollectorBase 
{

public:

  typedef std::string                IdType;
  typedef std::map<IdType,TimeProbe> MapType;

  TimeProbesCollectorBase();
  virtual ~TimeProbesCollectorBase();

  /** Start a time probe with a particular name. If the time probe does not
   * exist, it will be created */
  virtual void Start( const char * name );

  /** Stop a time probe identified with a name */
  virtual void Stop( const char * name );
  
  /** Report the summary of results from the time probes */
  virtual void Report( std::ostream & os = std::cout ) const;

  /** Destroy the set of probes. New probes can be created after invoking this method. */
  virtual void Clear(void);

protected:

  MapType   m_Probes;


};

}

#endif // ITK_USE_REVIEW

#endif // __itkTimeProbesCollectorBase_h
