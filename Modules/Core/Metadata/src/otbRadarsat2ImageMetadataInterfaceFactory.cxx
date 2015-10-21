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

#include "otbRadarsat2ImageMetadataInterfaceFactory.h"
#include "otbRadarsat2ImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
Radarsat2ImageMetadataInterfaceFactory
::Radarsat2ImageMetadataInterfaceFactory()
{
  this->RegisterOverride("SarImageMetadataInterface",
                         "otbRadarsat2ImageMetadataInterface",
                         "Radarsat2 Metadata Interface",
                         1,
                         itk::CreateObjectFunction<Radarsat2ImageMetadataInterface>::New());
}

Radarsat2ImageMetadataInterfaceFactory
::~Radarsat2ImageMetadataInterfaceFactory()
{
}

const char*
Radarsat2ImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
Radarsat2ImageMetadataInterfaceFactory::GetDescription() const
{
  return "Radarsat2 Metadata Interface Factory, handle Radarsat2 metadata in OTB";
}

} // end namespace otb
