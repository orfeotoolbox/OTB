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
#include "otbGDALImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbGDALImageIO.h"
#include "itkVersion.h"

namespace otb
{

GDALImageIOFactory::GDALImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase",
                         "otbGDALImageIO",
                         "GDAL Image IO",
                         1,
                         itk::CreateObjectFunction<GDALImageIO>::New());
}

GDALImageIOFactory::~GDALImageIOFactory()
{
}

const char*
GDALImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
GDALImageIOFactory::GetDescription() const
{
  return "GDAL ImageIO Factory, enabling loading gdal images in OTB";
}

} // end namespace otb
