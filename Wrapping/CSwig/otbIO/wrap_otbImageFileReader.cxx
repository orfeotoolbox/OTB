/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkImageFileReader_2D.cxx,v $
  Language:  C++
  Date:      $Date: 2005/06/03 08:39:17 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbVectorImage.h"

#ifdef CABLE_CONFIGURATION
#include "otbCSwigMacros.h"
#include "otbCSwigImages.h"

namespace _cable_
{
  const char* const group = OTB_WRAP_GROUP(otbImageFileReader);
  namespace wrappers
  {
    OTB_WRAP_OBJECT1(ImageFileReader, image::F2, otbImageFileReaderF2);
    OTB_WRAP_OBJECT1(ImageFileReader, image::VF2, otbImageFileReaderVF2);
    OTB_WRAP_OBJECT1(ImageFileReader, image::D2, otbImageFileReaderD2);
    OTB_WRAP_OBJECT1(ImageFileReader, image::UC2, otbImageFileReaderUC2);
    OTB_WRAP_OBJECT1(ImageFileReader, image::US2, otbImageFileReaderUS2);
    OTB_WRAP_OBJECT1(ImageFileReader, image::UL2, otbImageFileReaderUL2);
    OTB_WRAP_OBJECT1(ImageFileReader, image::UI2, otbImageFileReaderUI2);
    OTB_WRAP_OBJECT1(ImageFileReader, image::SS2, otbImageFileReaderSS2);
    OTB_WRAP_OBJECT1(ImageFileReader, image::SI2, otbImageFileReaderSI2);
    // Vector images reader
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileReader, image::VIF2, otbImageFileReaderVIF2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileReader, image::VID2, otbImageFileReaderVID2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileReader, image::VIUC2, otbImageFileReaderVIUC2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileReader, image::VIUS2, otbImageFileReaderVIUS2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileReader, image::VIUL2, otbImageFileReaderVIUL2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileReader, image::VIUI2, otbImageFileReaderVIUI2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileReader, image::VISS2, otbImageFileReaderVISS2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileReader, image::VISI2, otbImageFileReaderVISI2);
  }
}

#endif
