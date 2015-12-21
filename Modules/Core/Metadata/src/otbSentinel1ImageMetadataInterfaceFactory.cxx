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

#include "otbSentinel1ImageMetadataInterfaceFactory.h"
#include "otbSentinel1ImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
Sentinel1ImageMetadataInterfaceFactory
::Sentinel1ImageMetadataInterfaceFactory()
{
  this->RegisterOverride("SarImageMetadataInterface",
                         "otbSentinel1ImageMetadataInterface",
                         "Sentinel1 Metadata Interface",
                         1,
                         itk::CreateObjectFunction<Sentinel1ImageMetadataInterface>::New());
}

Sentinel1ImageMetadataInterfaceFactory
::~Sentinel1ImageMetadataInterfaceFactory()
{
}

const char*
Sentinel1ImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
Sentinel1ImageMetadataInterfaceFactory::GetDescription() const
{
  return "Sentinel1 Metadata Interface Factory, handle Sentinel1 metadata in OTB";
}

} // end namespace otb
