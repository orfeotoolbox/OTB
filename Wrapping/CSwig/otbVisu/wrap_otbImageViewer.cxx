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
#include "otbImageViewer.h"

#ifdef CABLE_CONFIGURATION
#include "otbCSwigMacros.h"
#include "otbCSwigImages.h"

namespace _cable_
{
  const char* const group = OTB_WRAP_GROUP(otbImageViewer);
  namespace wrappers
  {
    OTB_WRAP_OBJECT1(ImageViewer,float, otbImageViewerVIF2);
    OTB_WRAP_OBJECT1(ImageViewer,double, otbImageViewerVID2);
    OTB_WRAP_OBJECT1(ImageViewer,unsigned char, otbImageViewerVIUC2);
    OTB_WRAP_OBJECT1(ImageViewer,unsigned short, otbImageViewerVIUS2);
    OTB_WRAP_OBJECT1(ImageViewer,unsigned long , otbImageViewerVIUL2);
    OTB_WRAP_OBJECT1(ImageViewer,unsigned int, otbImageViewerVIUI2);
    OTB_WRAP_OBJECT1(ImageViewer,signed short, otbImageViewerVISS2);
    OTB_WRAP_OBJECT1(ImageViewer,signed int, otbImageViewerVISI2);
  }
}

#endif
