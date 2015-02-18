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
#include "otbTileMapImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbTileMapImageIO.h"
#include "itkVersion.h"

namespace otb
{

TileMapImageIOFactory::TileMapImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase",
                         "otbTileMapImageIO",
                         "TileMap Image IO",
                         1,
                         itk::CreateObjectFunction<TileMapImageIO>::New());
}

TileMapImageIOFactory::~TileMapImageIOFactory()
{
}

const char*
TileMapImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
TileMapImageIOFactory::GetDescription() const
{
  return "TileMap ImageIO Factory, enabling TileMap image format loading in OTB";
}

// Undocumented API used to register during static initialization.
// DO NOT CALL DIRECTLY.

static bool TileMapImageIOFactoryHasBeenRegistered;

void TileMapImageIOFactoryRegister__Private(void)
{
  if( ! TileMapImageIOFactoryHasBeenRegistered )
    {
    TileMapImageIOFactoryHasBeenRegistered = true;
    TileMapImageIOFactory::RegisterOneFactory();
    }
}

} // end namespace otb
