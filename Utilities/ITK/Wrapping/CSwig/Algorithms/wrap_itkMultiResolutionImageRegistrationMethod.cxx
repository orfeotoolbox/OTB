/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkMultiResolutionImageRegistrationMethod.cxx,v $
  Language:  C++
  Date:      $Date: 2003-10-14 17:59:55 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkMultiResolutionImageRegistrationMethod.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkMultiResolutionImageRegistrationMethod);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(MultiResolutionImageRegistrationMethod, image::F2, image::F2,
                     itkMultiResolutionImageRegistrationMethodF2F2);
    ITK_WRAP_OBJECT2(MultiResolutionImageRegistrationMethod, image::F3, image::F3,
                     itkMultiResolutionImageRegistrationMethodF3F3);
    ITK_WRAP_OBJECT2(MultiResolutionImageRegistrationMethod, image::US2, image::US2,
                     itkMultiResolutionImageRegistrationMethodUS2US2);
    ITK_WRAP_OBJECT2(MultiResolutionImageRegistrationMethod, image::US3, image::US3,
                     itkMultiResolutionImageRegistrationMethodUS3US3);
  }
}

#endif
