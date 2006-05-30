/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNrrdImageIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2004/07/15 16:26:40 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkNrrdImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkNrrdImageIO.h"
#include "itkVersion.h"

  
namespace itk
{
NrrdImageIOFactory::NrrdImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "itkNrrdImageIO",
                         "Nrrd Image IO",
                         1,
                         CreateObjectFunction<NrrdImageIO>::New());
}
  
NrrdImageIOFactory::~NrrdImageIOFactory()
{
}

const char* 
NrrdImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
NrrdImageIOFactory::GetDescription() const
{
  return "Nrrd ImageIO Factory, allows the loading of Nrrd images into insight";
}

} // end namespace itk

