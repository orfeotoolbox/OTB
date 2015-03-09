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

int otbCompareAsciiTestScientificNotation(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " referenceFile testFile" << std::endl;
    return EXIT_FAILURE;
    }

  std::ofstream fileRef;
  fileRef.open(argv[1]);
  fileRef << "1e15\n";
  fileRef << "2e15\n";
  fileRef << "3e15\n";
  fileRef << "4e15\n";
  fileRef << "5e-3\n";
  fileRef << "6e-3\n";
  fileRef << "7e-3\n";
  fileRef << "8e-3\n";
  fileRef << "A = 1e15\n";
  fileRef << "A = 2e15\n";
  fileRef << "A = 3e15\n";
  fileRef << "A = 4e15\n";
  fileRef << "A = 5e-3\n";
  fileRef << "A = 6e-3\n";
  fileRef << "A = 7e-3\n";
  fileRef << "A = 8e-3\n";
  fileRef.close();

  std::ofstream fileTest;
  fileTest.open(argv[2]);
  fileTest << "1e15\n";
  fileTest << "2E15\n";
  fileTest << "3e015\n";
  fileTest << "4E015\n";
  fileTest << "5e-3\n";
  fileTest << "6E-3\n";
  fileTest << "7e-03\n";
  fileTest << "8E-03\n";
  fileTest << "A = 1e15\n";
  fileTest << "A = 2E15\n";
  fileTest << "A = 3e015\n";
  fileTest << "A = 4E015\n";
  fileTest << "A = 5e-3\n";
  fileTest << "A = 6E-3\n";
  fileTest << "A = 7e-03\n";
  fileTest << "A = 8E-03\n";
  fileTest.close();

  return EXIT_SUCCESS;
}
