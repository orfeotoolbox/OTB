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

#include "otbMacro.h"

#include "otbSpot6ImageMetadataInterfaceFactory.h"
#include "otbSpot6ImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
Spot6ImageMetadataInterfaceFactory
::Spot6ImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface",
                         "otbSpot6ImageMetadataInterface",
                         "Spot6 Metadata Interface",
                         1,
                         itk::CreateObjectFunction<Spot6ImageMetadataInterface>::New());
}

Spot6ImageMetadataInterfaceFactory
::~Spot6ImageMetadataInterfaceFactory()
{
}

const char*
Spot6ImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
Spot6ImageMetadataInterfaceFactory::GetDescription() const
{
  return "Spot6 Metadata Interface Factory, handle Spot6 metadata in OTB";
}

} // end namespace otb
