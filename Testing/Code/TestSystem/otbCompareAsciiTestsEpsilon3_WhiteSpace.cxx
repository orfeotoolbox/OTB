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

#include "itkExceptionObject.h"
#include "otbMacro.h"

int otbCompareAsciiTestsEpsilon3_WhiteSpace(int argc, char * argv[])
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
  fileRef << "4\n";
  fileRef << "5\n";
  fileRef << "6\n";
  fileRef << "7\n";
  fileRef << "8\n";
  fileRef << "9\n";
  fileRef << "A = 10\n";
  fileRef << "A = 11\n";
  fileRef << "A = 12\n";
  fileRef << "B = 10\n";
  fileRef << "B = 11\n";
  fileRef << "B = 12\n";
  fileRef << "C = 10\n";
  fileRef << "C = 11\n";
  fileRef << "C = 12\n";
  fileRef << "D = 10\n";
  fileRef << "D = 11\n";
  fileRef << "D = 12\n";
  fileRef << "E = E,20\n";
  fileRef << "E = E,21\n";
  fileRef << "E = E,22\n";
  fileRef << "F = F,20\n";
  fileRef << "F = F,21\n";
  fileRef << "F = F,22\n";
  fileRef << "G = G,20\n";
  fileRef << "G = G,21\n";
  fileRef << "G = G,22\n";
  fileRef.close();

  std::ofstream fileTest;
  fileTest.open(argv[2]);
  fileTest << "1\n";
  fileTest << "2.001\n";
  fileTest << "3.010\n";
  fileTest << " 4\n";
  fileTest << " 5.001\n";
  fileTest << " 6.010\n";
  fileTest << "7\n ";
  fileTest << "8.001 \n";
  fileTest << "9.010 \n";
  fileTest << "A = 10\n";
  fileTest << "A = 11.001\n";
  fileTest << "A = 12.010\n";
  fileTest << "B =10\n";
  fileTest << "B =11.001\n";
  fileTest << "B =12.010\n";
  fileTest << "C= 10\n";
  fileTest << "C= 11.001\n";
  fileTest << "C= 12.010\n";
  fileTest << "D=10\n";
  fileTest << "D=11.001\n";
  fileTest << "D=12.010\n";
  fileTest << "E = E,20\n";
  fileTest << "E = E,21.001\n";
  fileTest << "E = E,22.010\n";
  fileTest << "F = F ,20\n";
  fileTest << "F = F ,21.001\n";
  fileTest << "F = F ,22.010\n";
  fileTest << "G = G, 20\n";
  fileTest << "G = G, 21.001\n";
  fileTest << "G = G, 22.010\n";

  fileTest.close();

  return EXIT_SUCCESS;
}
