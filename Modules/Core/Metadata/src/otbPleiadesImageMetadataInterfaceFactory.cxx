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

#include "otbPleiadesImageMetadataInterfaceFactory.h"
#include "otbPleiadesImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
PleiadesImageMetadataInterfaceFactory
::PleiadesImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface",
                         "otbPleiadesImageMetadataInterface",
                         "Pleiades Metadata Interface",
                         1,
                         itk::CreateObjectFunction<PleiadesImageMetadataInterface>::New());
}

PleiadesImageMetadataInterfaceFactory
::~PleiadesImageMetadataInterfaceFactory()
{
}

const char*
PleiadesImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
PleiadesImageMetadataInterfaceFactory::GetDescription() const
{
  return "Pleiades Metadata Interface Factory, handle Pleiades metadata in OTB";
}

} // end namespace otb
