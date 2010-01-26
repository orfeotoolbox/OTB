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
#include "otbMacro.h"

#include "otbGISTable.h"
#include "otbPostGISConnectionImplementation.h"

int otbGISTableNew(int argc, char * argv[])
{
  typedef otb::PostGISConnectionImplementation GISConnectionType;

  typedef otb::GISTable<GISConnectionType, double, 2> GISTableType;

  //Instantiation
  GISTableType::Pointer data = GISTableType::New();

  return EXIT_SUCCESS;
}
