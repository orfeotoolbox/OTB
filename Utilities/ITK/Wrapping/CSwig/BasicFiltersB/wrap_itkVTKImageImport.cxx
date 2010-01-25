/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkVTKImageImport.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-13 18:37:50 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkVTKImageImport.h"
#include "itkImage.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkVTKImageImport);
  namespace wrappers
  {

    ITK_WRAP_OBJECT1(VTKImageImport, image::F2,
                                     itkVTKImageImportF2);
    ITK_WRAP_OBJECT1(VTKImageImport, image::UC2,
                                     itkVTKImageImportUC2);
    ITK_WRAP_OBJECT1(VTKImageImport, image::US2,
                                     itkVTKImageImportUS2);
    ITK_WRAP_OBJECT1(VTKImageImport, image::F3, 
                                     itkVTKImageImportF3);
    ITK_WRAP_OBJECT1(VTKImageImport, image::UC3, 
                                     itkVTKImageImportUC3);
    ITK_WRAP_OBJECT1(VTKImageImport, image::US3, 
                                     itkVTKImageImportUS3);
  }
}

#endif
