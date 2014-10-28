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


#include "otbOpticalDefaultImageMetadataInterfaceFactory.h"
#include "otbOpticalDefaultImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{

OpticalDefaultImageMetadataInterfaceFactory
::OpticalDefaultImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface",
                         "otbOpticalDefaultImageMetadataInterface",
                         "Default Metadata Interface",
                         1,
                         itk::CreateObjectFunction<OpticalDefaultImageMetadataInterface>::New());
}

OpticalDefaultImageMetadataInterfaceFactory
::~OpticalDefaultImageMetadataInterfaceFactory()
{
}

const char*
OpticalDefaultImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
OpticalDefaultImageMetadataInterfaceFactory::GetDescription() const
{
  return "Optical Default Metadata Interface Factory, handle Default Optical metadata in OTB";
}

} // end namespace otb
