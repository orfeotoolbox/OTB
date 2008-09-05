/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaEvent.h,v $
  Language:  C++
  Date:      $Date: 2007-01-28 19:24:56 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaEvent_h
#define __itkMetaEvent_h

#include "metaEvent.h"

namespace itk 
{

class MetaEvent : public ::MetaEvent
{

public:

  MetaEvent(){};
  virtual ~MetaEvent(){};

};

} // end namespace itk


#endif
