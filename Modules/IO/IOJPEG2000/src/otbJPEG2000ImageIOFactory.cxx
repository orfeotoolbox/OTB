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
#include "otbJPEG2000ImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbJPEG2000ImageIO.h"
#include "itkVersion.h"

namespace otb
{

JPEG2000ImageIOFactory::JPEG2000ImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase",
                         "otbJPEG2000ImageIO",
                         "JPEG2000 Image IO",
                         1,
                         itk::CreateObjectFunction<JPEG2000ImageIO>::New());
}

JPEG2000ImageIOFactory::~JPEG2000ImageIOFactory()
{
}

const char*
JPEG2000ImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
JPEG2000ImageIOFactory::GetDescription() const
{
  return "JPEG2000 ImageIO Factory, allows the loading of JPEG2000 images into insight";
}

// Undocumented API used to register during static initialization.
// DO NOT CALL DIRECTLY.

static bool JPEG2000ImageIOFactoryHasBeenRegistered;

void JPEG2000ImageIOFactoryRegister__Private(void)
{
  if( ! JPEG2000ImageIOFactoryHasBeenRegistered )
    {
    JPEG2000ImageIOFactoryHasBeenRegistered = true;
    JPEG2000ImageIOFactory::RegisterOneFactory();
    }
}
} // end namespace otb
