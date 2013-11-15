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
#include "otbBSQImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbBSQImageIO.h"
#include "itkVersion.h"

namespace otb
{

BSQImageIOFactory::BSQImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase",
                         "otbBSQImageIO",
                         "BSQ Image IO",
                         1,
                         itk::CreateObjectFunction<BSQImageIO>::New());
}

BSQImageIOFactory::~BSQImageIOFactory()
{
}

const char*
BSQImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
BSQImageIOFactory::GetDescription() const
{
  return "BSQ ImageIO Factory, permettant le chargement d'image au format BSQ dans l'OTB";
}

// Undocumented API used to register during static initialization.
// DO NOT CALL DIRECTLY.

static bool BSQImageIOFactoryHasBeenRegistered;

void BSQImageIOFactoryRegister__Private(void)
{
  if( ! BSQImageIOFactoryHasBeenRegistered )
    {
    BSQImageIOFactoryHasBeenRegistered = true;
    BSQImageIOFactory::RegisterOneFactory();
    }
}

} // end namespace otb
