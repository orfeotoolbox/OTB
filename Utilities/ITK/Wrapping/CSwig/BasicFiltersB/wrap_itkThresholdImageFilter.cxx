/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkThresholdImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004-04-02 22:43:59 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkThresholdImageFilter.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkThresholdImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT1(ThresholdImageFilter, image::F2,
                                     itkThresholdImageFilterF2);
    ITK_WRAP_OBJECT1(ThresholdImageFilter, image::US2,
                                     itkThresholdImageFilterUS2);
    ITK_WRAP_OBJECT1(ThresholdImageFilter, image::F3, 
                                     itkThresholdImageFilterF3);
    ITK_WRAP_OBJECT1(ThresholdImageFilter, image::US3, 
                                     itkThresholdImageFilterUS3);
  }
}

#endif
