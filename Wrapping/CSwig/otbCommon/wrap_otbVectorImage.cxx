/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkImage_2D.cxx,v $
  Language:  C++
  Date:      $Date: 2005/04/01 16:30:14 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkVector.h"
#include "itkCovariantVector.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigImages.h"
#include "itkCSwigMacros.h"
#include "otbCSwigMacros.h"

namespace _cable_
{
  const char* const group = OTB_WRAP_GROUP(otbVectorImage);
  namespace wrappers
  {
    OTB_WRAP_OBJECT2(VectorImage, float, 2, otbVectorImageF2);
    OTB_WRAP_OBJECT2(VectorImage, double, 2, otbVectorImageD2);
    OTB_WRAP_OBJECT2(VectorImage, unsigned char, 2, otbVectorImageUC2);
    OTB_WRAP_OBJECT2(VectorImage, unsigned short, 2, otbVectorImageUS2);
    OTB_WRAP_OBJECT2(VectorImage, unsigned int, 2, otbVectorImageUI2);
    OTB_WRAP_OBJECT2(VectorImage, unsigned long, 2, otbVectorImageUL2);
    OTB_WRAP_OBJECT2(VectorImage, signed char, 2, otbVectorImageSC2);
    OTB_WRAP_OBJECT2(VectorImage, signed short, 2, otbVectorImageSS2);
    OTB_WRAP_OBJECT2(VectorImage, signed int, 2, otbVectorImageSI2);
    OTB_WRAP_OBJECT2(VectorImage, itkvector::F2, 2, otbVectorImageVF2);
    OTB_WRAP_OBJECT2(VectorImage, itkvector::D2, 2, otbVectorImageVD2);
    OTB_WRAP_OBJECT2(VectorImage, covariantvector::F2, 2, otbVectorImageCVF2);
    OTB_WRAP_OBJECT2(VectorImage, covariantvector::D2, 2, otbVectorImageCVD2);
   }
}

#endif
