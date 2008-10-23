/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkCurvatureAnisotropicDiffusionImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004-04-19 18:50:54 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkCurvatureAnisotropicDiffusionImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(CurvatureAnisotropicDiffusionImageFilter, image::F2, image::F2,
                     itkCurvatureAnisotropicDiffusionImageFilterF2F2);
    ITK_WRAP_OBJECT2(CurvatureAnisotropicDiffusionImageFilter, image::F3, image::F3,
                     itkCurvatureAnisotropicDiffusionImageFilterF3F3);
  }
}

#endif
