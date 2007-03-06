/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_ITKIO.cxx,v $
  Language:  C++
  Date:      $Date: 2004/04/15 14:42:45 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef CABLE_CONFIGURATION
#include "otbCSwigMacros.h"

namespace _cable_
{
  const char* const package = OTB_WRAP_PACKAGE_NAME(OTB_WRAP_PACKAGE);
  const char* const groups[] =
  {
    OTB_WRAP_GROUP(otbIOBase),
    OTB_WRAP_GROUP(otbImageFileReader),
    OTB_WRAP_GROUP(otbImageFileWriter),
  };
}
#endif
