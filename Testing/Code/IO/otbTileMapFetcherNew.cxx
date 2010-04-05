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
#include "itkExceptionObject.h"

#include "otbTileMapFetcher.h"

int otbTileMapFetcherNew(int argc, char * argv[])
{
  typedef otb::TileMapFetcher         TileMapFetcherType;
  typedef TileMapFetcherType::Pointer TileMapFetcherPointerType;

  TileMapFetcherPointerType tmf = TileMapFetcherType::New();

  return EXIT_SUCCESS;
}
