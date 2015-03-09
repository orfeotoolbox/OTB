/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbKMLVectorDataIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"
#include "otbKMLVectorDataIO.h"

namespace otb
{

KMLVectorDataIOFactory::KMLVectorDataIOFactory()
{
  static std::string classOverride = std::string("otbVectorDataIOBase");
  static std::string subclass = std::string("otbKMLVectorDataIO");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "KML Vectordata IO",
                         1,
                         itk::CreateObjectFunction<KMLVectorDataIO>::New());
}

KMLVectorDataIOFactory::~KMLVectorDataIOFactory()
{}

const char*
KMLVectorDataIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}


const char*
KMLVectorDataIOFactory::GetDescription() const
{
  return "KML VectorDataIO Factory, allows the loading of KML vector data into OTB";
}

} // end namespace otb
