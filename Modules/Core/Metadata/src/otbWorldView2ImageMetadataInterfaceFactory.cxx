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

#include "otbWorldView2ImageMetadataInterfaceFactory.h"
#include "otbWorldView2ImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{

WorldView2ImageMetadataInterfaceFactory
::WorldView2ImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface",
                         "otbWorldView2ImageMetadataInterface",
                         "WorldView2 Metadata Interface",
                         1,
                         itk::CreateObjectFunction<WorldView2ImageMetadataInterface>::New());
}

WorldView2ImageMetadataInterfaceFactory
::~WorldView2ImageMetadataInterfaceFactory()
{
}

const char*
WorldView2ImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
WorldView2ImageMetadataInterfaceFactory::GetDescription() const
{
  return "WorldView2 Metadata Interface Factory, handle WorldView2 metadata in OTB";
}

} // end namespace otb
