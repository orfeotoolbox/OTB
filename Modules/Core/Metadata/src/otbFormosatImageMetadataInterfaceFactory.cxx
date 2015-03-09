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


#include "otbFormosatImageMetadataInterfaceFactory.h"
#include "otbFormosatImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
FormosatImageMetadataInterfaceFactory
::FormosatImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface",
                         "otbFormosatImageMetadataInterface",
                         "Formosat Metadata Interface",
                         1,
                         itk::CreateObjectFunction<FormosatImageMetadataInterface>::New());
}

FormosatImageMetadataInterfaceFactory
::~FormosatImageMetadataInterfaceFactory()
{
}

const char*
FormosatImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
FormosatImageMetadataInterfaceFactory::GetDescription() const
{
  return "Formosat Metadata Interface Factory, handle Formosat metadata in OTB";
}

} // end namespace otb
