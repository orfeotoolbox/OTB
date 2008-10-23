/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkCurvesLevelSetImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:30:12 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkCurvesLevelSetImageFilter.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkCurvesLevelSetImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(CurvesLevelSetImageFilter, image::F2, image::F2,
                     itkCurvesLevelSetImageFilterF2F2);
    ITK_WRAP_OBJECT2(CurvesLevelSetImageFilter, image::F3, image::F3,
                     itkCurvesLevelSetImageFilterF3F3);
  }
}

#endif
