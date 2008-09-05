/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkWatershedImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2004-03-26 12:57:10 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkWatershedImageFilter.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkWatershedImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT1(WatershedImageFilter, image::F2, itkWatershedImageFilterF2);
    ITK_WRAP_OBJECT1(WatershedImageFilter, image::F3, itkWatershedImageFilterF3);
     
    ITK_WRAP_OBJECT1(WatershedImageFilter, image::D2, itkWatershedImageFilterD2);
    ITK_WRAP_OBJECT1(WatershedImageFilter, image::D3, itkWatershedImageFilterD3);
  }
}

#endif
