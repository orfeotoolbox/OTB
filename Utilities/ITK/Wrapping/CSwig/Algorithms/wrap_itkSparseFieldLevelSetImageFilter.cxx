/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkSparseFieldLevelSetImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-25 16:40:03 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkSparseFieldLevelSetImageFilter.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkSparseFieldLevelSetImageFilter);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(SparseFieldLevelSetImageFilter,image::F2,image::F2,
                     itkSparseFieldLevelSetImageFilterF2F2);
    ITK_WRAP_OBJECT2(SparseFieldLevelSetImageFilter,image::F3,image::F3,
                     itkSparseFieldLevelSetImageFilterF3F3);
  }
}

#endif
