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

#include "otbOGRVectorDataIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"
#include "otbOGRVectorDataIO.h"

namespace otb
{

OGRVectorDataIOFactory::OGRVectorDataIOFactory()
{
  static std::string classOverride = std::string("otbVectorDataIOBase");
  static std::string subclass = std::string("otbOGRVectorDataIO");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "OGR Vectordata IO",
                         1,
                         itk::CreateObjectFunction<OGRVectorDataIO>::New());
}

OGRVectorDataIOFactory::~OGRVectorDataIOFactory()
{}

const char*
OGRVectorDataIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
OGRVectorDataIOFactory::GetDescription() const
{
  return "OGR VectorDataIO Factory, allows the loading of ogr supported vector data into OTB";
}

} // end namespace otb
