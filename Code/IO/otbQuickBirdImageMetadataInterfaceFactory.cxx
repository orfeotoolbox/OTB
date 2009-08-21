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


#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <typeinfo>
#include <cassert>

#include "otbMacro.h"

#include "otbQuickBirdImageMetadataInterfaceFactory.h"
#include "otbQuickBirdImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"


namespace otb
{

QuickBirdImageMetadataInterfaceFactory
::QuickBirdImageMetadataInterfaceFactory()
{
  this->RegisterOverride("ImageMetadataInterfaceBase",
                         "otbQuickBirdImageMetadataInterface",
                         "QuickBird Meteada Interface",
                         1,
                         itk::CreateObjectFunction<QuickBirdImageMetadataInterface >::New());
}

QuickBirdImageMetadataInterfaceFactory
::~QuickBirdImageMetadataInterfaceFactory()
{
}

const char*
QuickBirdImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
QuickBirdImageMetadataInterfaceFactory::GetDescription() const
{
  return "QuickBird Metadata Interface Factory, handle QuickBird metadata in OTB";
}

} // end namespace otb
