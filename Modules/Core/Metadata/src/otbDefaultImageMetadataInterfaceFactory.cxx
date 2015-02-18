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


#include <typeinfo>
#include <cassert>

#include "otbDefaultImageMetadataInterfaceFactory.h"
#include "otbDefaultImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{

DefaultImageMetadataInterfaceFactory
::DefaultImageMetadataInterfaceFactory()
{
  this->RegisterOverride("ImageMetadataInterfaceBase",
                         "otbDefaultImageMetadataInterface",
                         "Default Metadata Interface",
                         1,
                         itk::CreateObjectFunction<DefaultImageMetadataInterface>::New());
}

DefaultImageMetadataInterfaceFactory
::~DefaultImageMetadataInterfaceFactory()
{
}

const char*
DefaultImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
DefaultImageMetadataInterfaceFactory::GetDescription() const
{
  return "Default Metadata Interface Factory, handle Default metadata in OTB";
}

} // end namespace otb
