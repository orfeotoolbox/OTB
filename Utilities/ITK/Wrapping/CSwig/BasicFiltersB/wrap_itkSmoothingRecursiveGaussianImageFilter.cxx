/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkSmoothingRecursiveGaussianImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004-04-21 20:24:21 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkSmoothingRecursiveGaussianImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(SmoothingRecursiveGaussianImageFilter, image::F2, image::F2,
                     itkSmoothingRecursiveGaussianImageFilterF2F2);
    ITK_WRAP_OBJECT2(SmoothingRecursiveGaussianImageFilter, image::F3, image::F3,
                     itkSmoothingRecursiveGaussianImageFilterF3F3);
    ITK_WRAP_OBJECT2(SmoothingRecursiveGaussianImageFilter, image::US2, image::US2,
                     itkSmoothingRecursiveGaussianImageFilterUS2US2);
    ITK_WRAP_OBJECT2(SmoothingRecursiveGaussianImageFilter, image::US3, image::US3,
                     itkSmoothingRecursiveGaussianImageFilterUS3US3);
  }
}

#endif
