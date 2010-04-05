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

#include "otbPostGISCreateTableTransactor.h"

int otbPostGISCreateTableTransactorAccessors(int argc, char * argv[])
{
  typedef otb::PostGISCreateTableTransactor TransactorType;

  //Instantiation
  TransactorType myTransactor;

  unsigned short dimension = 2;

  myTransactor.SetDimension(dimension);

  if (dimension != myTransactor.GetDimension()) return EXIT_FAILURE;

  std::string name = "mytable";

  myTransactor.SetTableName(name);

  if (name != myTransactor.GetTableName()) return EXIT_FAILURE;

  int srid = -1;

  myTransactor.SetSRID(srid);

  if (srid != myTransactor.GetSRID()) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
