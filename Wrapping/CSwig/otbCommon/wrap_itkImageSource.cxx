/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkImageSource.cxx,v $
  Language:  C++
  Date:      $Date: 2005/04/01 16:30:14 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbVectorImage.h"
#include "itkImageToImageFilter.h"
#include "itkVector.h"
#include "itkCovariantVector.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigImages.h"
#include "itkCSwigMacros.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkImageSource);
  namespace wrappers
  {
    ITK_WRAP_OBJECT1(ImageSource, image::VIF2 , itkImageSourceVIF2 );
    ITK_WRAP_OBJECT1(ImageSource, image::VID2 , itkImageSourceVID2 );
    ITK_WRAP_OBJECT1(ImageSource, image::VIUC2, itkImageSourceVIUC2);
    ITK_WRAP_OBJECT1(ImageSource, image::VIUS2, itkImageSourceVIUS2);
    ITK_WRAP_OBJECT1(ImageSource, image::VIUI2, itkImageSourceVIUI2);
    ITK_WRAP_OBJECT1(ImageSource, image::VIUL2, itkImageSourceVIUL2);
    ITK_WRAP_OBJECT1(ImageSource, image::VISC2, itkImageSourceVISC2);
    ITK_WRAP_OBJECT1(ImageSource, image::VISS2, itkImageSourceVISS2);
    ITK_WRAP_OBJECT1(ImageSource, image::VISI2, itkImageSourceVISI2);

  }
}
#endif
