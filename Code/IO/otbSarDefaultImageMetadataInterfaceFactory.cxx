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



#include "otbSarDefaultImageMetadataInterfaceFactory.h"
#include "otbSarDefaultImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{

SarDefaultImageMetadataInterfaceFactory
::SarDefaultImageMetadataInterfaceFactory()
{
  this->RegisterOverride("SarImageMetadataInterface",
                         "otbSarDefaultImageMetadataInterface",
                         "Default Sar Metadata Interface",
                         1,
                         itk::CreateObjectFunction<SarDefaultImageMetadataInterface>::New());
}

SarDefaultImageMetadataInterfaceFactory
::~SarDefaultImageMetadataInterfaceFactory()
{
}

const char*
SarDefaultImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
SarDefaultImageMetadataInterfaceFactory::GetDescription() const
{
  return "Default Sar Metadata Interface Factory, handle Default Sar metadata in OTB";
}

} // end namespace otb
