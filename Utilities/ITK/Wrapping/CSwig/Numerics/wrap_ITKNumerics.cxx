/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_ITKNumerics.cxx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:30:12 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
namespace _cable_
{
  const char* const package = ITK_WRAP_PACKAGE_NAME(ITK_WRAP_PACKAGE);
  const char* const groups[] =
  {
    ITK_WRAP_GROUP(ITKCostFunctions),
    ITK_WRAP_GROUP(ITKOptimizers)
  };
}
#endif
