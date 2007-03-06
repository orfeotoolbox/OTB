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
#include "itkVector.h"
#include "itkCovariantVector.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigImages.h"
#include "itkCSwigMacros.h"
#include "otbCSwigMacros.h"

namespace _cable_
{
  const char* const group = OTB_WRAP_GROUP(otbImage);
  namespace wrappers
  {
    typedef otb::ImageBase otbImageBase;
    typedef itk::MetaDataDictionary itkMetaDataDictionary;
    typedef otb::MetaDataKey otbMetaDataKey;
    OTB_WRAP_OBJECT2(Image, float, 2, otbImageF2);
    OTB_WRAP_OBJECT2(Image, double, 2, otbImageD2);
    OTB_WRAP_OBJECT2(Image, unsigned char, 2, otbImageUC2);
    OTB_WRAP_OBJECT2(Image, unsigned short, 2, otbImageUS2);
    OTB_WRAP_OBJECT2(Image, unsigned int, 2, otbImageUI2);
    OTB_WRAP_OBJECT2(Image, unsigned long, 2, otbImageUL2);
    OTB_WRAP_OBJECT2(Image, signed char, 2, otbImageSC2);
    OTB_WRAP_OBJECT2(Image, signed short, 2, otbImageSS2);
    OTB_WRAP_OBJECT2(Image, signed int, 2, otbImageSI2);
    OTB_WRAP_OBJECT2(Image, itkvector::F2, 2, otbImageVF2);
    OTB_WRAP_OBJECT2(Image, itkvector::D2, 2, otbImageVD2);
    OTB_WRAP_OBJECT2(Image, covariantvector::F2, 2, otbImageCVF2);
    OTB_WRAP_OBJECT2(Image, covariantvector::D2, 2, otbImageCVD2);
   }
}

#endif
