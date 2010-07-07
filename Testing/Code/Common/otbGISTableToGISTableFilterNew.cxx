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
#include "otbGISTableToGISTableFilter.h"

int otbGISTableToGISTableFilterNew(int argc, char * argv[])
{
  typedef otb::PostGISConnectionImplementation                      GISConnectionType;
  typedef otb::GISTable<GISConnectionType, double, 2>               GISTableType;
  typedef otb::GISTableToGISTableFilter<GISTableType, GISTableType> GISFilterType;

  //Instantiation
  GISFilterType::Pointer data = GISFilterType::New();

  return EXIT_SUCCESS;
}
