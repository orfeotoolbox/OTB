/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_ITKCommonA.cxx,v $
  Language:  C++
  Date:      $Date: 2005/05/10 14:37:07 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef CABLE_CONFIGURATION
#include "otbCSwigMacros.h"
#include "itkCSwigMacros.h"
namespace _cable_
{
  const char* const package = OTB_WRAP_PACKAGE_NAME(OTB_WRAP_PACKAGE);
  const char* const groups[] =
  {
    //ITK_WRAP_GROUP(itkImageSource),
    OTB_WRAP_GROUP(otbImage),
    OTB_WRAP_GROUP(otbVectorImage)
  };
}
#endif
