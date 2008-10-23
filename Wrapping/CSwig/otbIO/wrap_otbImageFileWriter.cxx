/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkImageFileWriter_2D.cxx,v $
  Language:  C++
  Date:      $Date: 2005/06/03 08:39:17 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbVectorImage.h"

#ifdef CABLE_CONFIGURATION
#include "otbCSwigMacros.h"
#include "otbCSwigImages.h"

namespace _cable_
{
  const char* const group = OTB_WRAP_GROUP(otbImageFileWriter);
  namespace wrappers
  {
    OTB_WRAP_OBJECT1(ImageFileWriter, image::F2, otbImageFileWriterF2);
    OTB_WRAP_OBJECT1(ImageFileWriter, image::VF2, otbImageFileWriterVF2);     
    OTB_WRAP_OBJECT1(ImageFileWriter, image::D2, otbImageFileWriterD2);
    OTB_WRAP_OBJECT1(ImageFileWriter, image::UC2, otbImageFileWriterUC2);
    OTB_WRAP_OBJECT1(ImageFileWriter, image::US2, otbImageFileWriterUS2);
    OTB_WRAP_OBJECT1(ImageFileWriter, image::UL2, otbImageFileWriterUL2);
    OTB_WRAP_OBJECT1(ImageFileWriter, image::UI2, otbImageFileWriterUI2);
    OTB_WRAP_OBJECT1(ImageFileWriter, image::SS2, otbImageFileWriterSS2);
    OTB_WRAP_OBJECT1(ImageFileWriter, image::SI2, otbImageFileWriterSI2);

    // Vector images writer
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileWriter, image::VIF2, otbImageFileWriterVIF2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileWriter, image::VID2, otbImageFileWriterVID2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileWriter, image::VIUC2, otbImageFileWriterVIUC2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileWriter, image::VIUS2, otbImageFileWriterVIUS2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileWriter, image::VIUL2, otbImageFileWriterVIUL2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileWriter, image::VIUI2, otbImageFileWriterVIUI2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileWriter, image::VISS2, otbImageFileWriterVISS2);
    OTB_WRAP_OBJECT1_WITH_SUPERCLASS(ImageFileWriter, image::VISI2, otbImageFileWriterVISI2);
  }
}

#endif
