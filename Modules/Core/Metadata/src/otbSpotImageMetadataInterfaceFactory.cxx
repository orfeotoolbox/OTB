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

#include "otbSpotImageMetadataInterfaceFactory.h"
#include "otbSpotImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
SpotImageMetadataInterfaceFactory
::SpotImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface",
                         "otbSpotImageMetadataInterface",
                         "Spot Metadata Interface",
                         1,
                         itk::CreateObjectFunction<SpotImageMetadataInterface>::New());
}

SpotImageMetadataInterfaceFactory
::~SpotImageMetadataInterfaceFactory()
{
}

const char*
SpotImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
SpotImageMetadataInterfaceFactory::GetDescription() const
{
  return "Spot Metadata Interface Factory, handle Spot metadata in OTB";
}

} // end namespace otb
