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

#include "otbCosmoSkymedImageMetadataInterfaceFactory.h"
#include "otbCosmoSkymedImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
CosmoSkymedImageMetadataInterfaceFactory
::CosmoSkymedImageMetadataInterfaceFactory()
{
  this->RegisterOverride("SarImageMetadataInterface",
                         "otbCosmoSkymedImageMetadataInterface",
                         "CosmoSkymed Metadata Interface",
                         1,
                         itk::CreateObjectFunction<CosmoSkymedImageMetadataInterface>::New());
}

CosmoSkymedImageMetadataInterfaceFactory
::~CosmoSkymedImageMetadataInterfaceFactory()
{
}

const char*
CosmoSkymedImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
CosmoSkymedImageMetadataInterfaceFactory::GetDescription() const
{
  return "CosmoSkymed Metadata Interface Factory, handle CosmoSkymed metadata in OTB";
}

} // end namespace otb
