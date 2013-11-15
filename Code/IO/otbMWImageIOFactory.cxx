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
#include "otbMWImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbMWImageIO.h"
#include "itkVersion.h"

namespace otb
{

MWImageIOFactory::MWImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase",
                         "otbMWImageIO",
                         "MW Image IO",
                         1,
                         itk::CreateObjectFunction<MWImageIO>::New());
}

MWImageIOFactory::~MWImageIOFactory()
{
}

const char*
MWImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
MWImageIOFactory::GetDescription() const
{
  return "MegaWave ImageIO Factory, permettant le chargement d'image au format MW dans l'OTB";
}

// Undocumented API used to register during static initialization.
// DO NOT CALL DIRECTLY.

static bool MWImageIOFactoryHasBeenRegistered;

void MWImageIOFactoryRegister__Private(void)
{
  if( ! MWImageIOFactoryHasBeenRegistered )
    {
    MWImageIOFactoryHasBeenRegistered = true;
    MWImageIOFactory::RegisterOneFactory();
    }
}
} // end namespace otb
