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

int otbCompareAsciiTests(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " referenceFile testFile" << std::endl;
    return EXIT_FAILURE;
    }

  std::ofstream fileRef;
  fileRef.open(argv[1]);
  fileRef << "1\n";
  fileRef << "2\n";
  fileRef << "3\n";
  fileRef.close();

  std::ofstream fileTest;
  fileTest.open(argv[2]);
  fileTest << "1\n";
  fileTest << "2\n";
  fileTest << "3\n";
  fileTest.close();

  return EXIT_SUCCESS;
}
