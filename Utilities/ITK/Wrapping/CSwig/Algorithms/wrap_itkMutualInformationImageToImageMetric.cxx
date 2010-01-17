/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkMutualInformationImageToImageMetric.cxx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:30:12 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkMutualInformationImageToImageMetric.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkMutualInformationImageToImageMetric);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(MutualInformationImageToImageMetric, image::F2, image::F2,
                     itkMutualInformationImageToImageMetricF2F2);
    ITK_WRAP_OBJECT2(MutualInformationImageToImageMetric, image::F3, image::F3,
                     itkMutualInformationImageToImageMetricF3F3);
    ITK_WRAP_OBJECT2(MutualInformationImageToImageMetric, image::US2, image::US2,
                     itkMutualInformationImageToImageMetricUS2US2);
    ITK_WRAP_OBJECT2(MutualInformationImageToImageMetric, image::US3, image::US3,
                     itkMutualInformationImageToImageMetricUS3US3);
  }
}

#endif
