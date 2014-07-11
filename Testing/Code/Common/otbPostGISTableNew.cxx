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

#include "otbMacro.h"

#include "otbPostGISTable.h"
#include "otbPostGISConnectionImplementation.h"

int otbPostGISTableNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::PostGISConnectionImplementation GISConnectionType;

  typedef otb::PostGISTable<GISConnectionType, double, 2> PostGISTableType;

  //Instantiation
  PostGISTableType::Pointer data = PostGISTableType::New();

  std::cout << data << std::endl;

  return EXIT_SUCCESS;
}
