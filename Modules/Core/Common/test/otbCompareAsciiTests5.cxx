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
#include <iostream>
#include <fstream>


#include "otbMacro.h"

int otbCompareAsciiTests5(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " referenceFile testFile" << std::endl;
    return EXIT_FAILURE;
    }

  std::ofstream fileRef;
  fileRef.open(argv[1]);
  fileRef << "[ 1 1 1 ]\n";
  fileRef << "[ 2 2 2 ]\n";
  fileRef << "[ 3 3 3 ]\n";
  fileRef.close();

  std::ofstream fileTest;
  fileTest.open(argv[2]);
  fileTest << "[ 3.010 2.999 3.000 ]\n";
  fileTest << "[ 1.999 2.000 2.001 ]\n";
  fileTest << "[ 1.001 0.999 1.0005 ]\n";
  fileTest.close();

  return EXIT_SUCCESS;
}
