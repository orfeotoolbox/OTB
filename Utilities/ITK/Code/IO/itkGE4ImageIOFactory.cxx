/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGE4ImageIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2007-12-03 13:13:08 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkGE4ImageIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkGE4ImageIO.h"
#include "itkVersion.h"


namespace itk
{
void GE4ImageIOFactory::PrintSelf(std::ostream&, Indent) const
{

}


GE4ImageIOFactory::GE4ImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "itkGE4ImageIO",
                         "GE4 Image IO",
                         1,
                         CreateObjectFunction<GE4ImageIO>::New());
}

GE4ImageIOFactory::~GE4ImageIOFactory()
{
}

const char*
GE4ImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
GE4ImageIOFactory::GetDescription() const
{
  return "GE4 ImageIO Factory, allows the loading of GE4 images into insight";
}

} // end namespace itk
