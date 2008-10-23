/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGEAdwImageIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2007-12-03 13:13:09 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkGEAdwImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkGEAdwImageIO.h"
#include "itkVersion.h"


namespace itk
{
void GEAdwImageIOFactory::PrintSelf(std::ostream&, Indent) const
{

}


GEAdwImageIOFactory::GEAdwImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "itkGEAdwImageIO",
                         "GEAdw Image IO",
                         1,
                         CreateObjectFunction<GEAdwImageIO>::New());
}

GEAdwImageIOFactory::~GEAdwImageIOFactory()
{
}

const char*
GEAdwImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
GEAdwImageIOFactory::GetDescription() const
{
  return "GEAdw ImageIO Factory, allows the loading of GEAdw images into insight";
}

} // end namespace itk
