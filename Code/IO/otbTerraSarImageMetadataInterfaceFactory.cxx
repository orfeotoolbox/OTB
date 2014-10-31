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


#include "otbTerraSarImageMetadataInterfaceFactory.h"
#include "otbTerraSarImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
TerraSarImageMetadataInterfaceFactory
::TerraSarImageMetadataInterfaceFactory()
{

  this->RegisterOverride("SarImageMetadataInterface",
                         "otbTerraSarImageMetadataInterface",
                         "TerraSar Metadata Interface",
                         1,
                         itk::CreateObjectFunction<TerraSarImageMetadataInterface>::New());
}

TerraSarImageMetadataInterfaceFactory
::~TerraSarImageMetadataInterfaceFactory()
{
}

const char*
TerraSarImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
TerraSarImageMetadataInterfaceFactory::GetDescription() const
{
  return "TerraSar Metadata Interface Factory, handle TerraSar metadata in OTB";
}

} // end namespace otb
