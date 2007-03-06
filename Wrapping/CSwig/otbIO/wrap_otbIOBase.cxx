/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_IOBase.cxx,v $
  Language:  C++
  Date:      $Date: 2005/08/12 23:02:58 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbBSQImageIOFactory.h"
#include "otbBSQImageIO.h"
#include "otbGDALImageIOFactory.h"
#include "otbGDALImageIO.h"
#include "otbImageIOFactory.h"
#include "otbLUMImageIOFactory.h"
#include "otbLUMImageIO.h"
#include "otbMSTARImageIOFactory.h"
#include "otbMSTARImageIO.h"
#include "otbONERAImageIOFactory.h"
#include "otbONERAImageIO.h"

#ifdef CABLE_CONFIGURATION
#include "otbCSwigMacros.h"
#include "itkCSwigMacros.h"
namespace _cable_
{
  const char* const group = OTB_WRAP_GROUP(otbIOBase);
  namespace wrappers
  {
    ITK_WRAP_OBJECT(ImageIOFactory);
    OTB_WRAP_OBJECT(BSQImageIOFactory);
    OTB_WRAP_OBJECT(BSQImageIO);
    OTB_WRAP_OBJECT(GDALImageIOFactory);
    OTB_WRAP_OBJECT(GDALImageIO);
    OTB_WRAP_OBJECT(ImageIOFactory);
    OTB_WRAP_OBJECT(LUMImageIOFactory);
    OTB_WRAP_OBJECT(LUMImageIO);
    OTB_WRAP_OBJECT(MSTARImageIOFactory);
    OTB_WRAP_OBJECT(MSTARImageIO);
    OTB_WRAP_OBJECT(ONERAImageIOFactory);
    OTB_WRAP_OBJECT(ONERAImageIO);
  }
}

#endif
