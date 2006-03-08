/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkTimeProbesCollector.h,v $
  Language:  C++
  Date:      $Date: 2002/10/02 15:28:31 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __fltkTimeProbesCollector_h
#define __fltkTimeProbesCollector_h


#include "fltkTimeProbesCollectorGUI.h"

namespace fltk
{


class TimeProbesCollector : public TimeProbesCollectorGUI
{
  typedef ::itk::TimeProbesCollectorBase BaseClassType;
public:

  TimeProbesCollector();
  ~TimeProbesCollector();

  void Start( const char * );
  void Stop( const char * );

  void Report(void) const;
  void Clear(void);

};

} // end namespace fltk

#endif
