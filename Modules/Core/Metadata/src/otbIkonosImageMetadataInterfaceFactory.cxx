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

#include "otbIkonosImageMetadataInterfaceFactory.h"
#include "otbIkonosImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
/*************************************************
******* IkonosImageMetadataInterfaceFactory ******
**************************************************/
IkonosImageMetadataInterfaceFactory
::IkonosImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface",
                         "otbIkonosImageMetadataInterface",
                         "Ikonos Metadata Interface",
                         1,
                         itk::CreateObjectFunction<IkonosImageMetadataInterface>::New());
}

IkonosImageMetadataInterfaceFactory
::~IkonosImageMetadataInterfaceFactory()
{
}

const char*
IkonosImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
IkonosImageMetadataInterfaceFactory::GetDescription() const
{
  return "Ikonos Metadata Interface Factory, handle Ikonos metadata in OTB";
}

} // end namespace otb
