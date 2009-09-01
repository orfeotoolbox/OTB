/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaEvent.h,v $
  Language:  C++
  Date:      $Date: 2009-04-07 14:34:04 $
  Version:   $Revision: 1.6 $

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

/** \class MetaEvent
 * \brief Event abstract class
 */
class ITK_EXPORT MetaEvent : public ::MetaEvent
{

public:

  MetaEvent(){};
  virtual ~MetaEvent(){};

};

} // end namespace itk


#endif
