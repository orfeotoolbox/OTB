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
  fileRef << "1\n";
  fileRef << "1\n";
  fileRef << "2\n";
  fileRef << "2\n";
  fileRef << "2\n";
  fileRef << "3\n";
  fileRef << "3\n";
  fileRef << "3\n";
  fileRef << "4\n";
  fileRef << "4\n";
  fileRef << "4\n";
  fileRef << "A = 10\n";
  fileRef << "A = 11\n";
  fileRef << "A = 12\n";
  fileRef << "B =10\n";
  fileRef << "B =11\n";
  fileRef << "B =12\n";
  fileRef << "C= 10\n";
  fileRef << "C= 11\n";
  fileRef << "C= 12\n";
  fileRef << "D=10\n";
  fileRef << "D=11\n";
  fileRef << "D=12\n";
  fileRef << "E = 10\n";
  fileRef << "E = 11\n";
  fileRef << "E = 12\n";
  fileRef << "F = [20,21,22]\n";
  fileRef << "F = [20,21,22]\n";
  fileRef << "F = [20,21,22]\n";
  fileRef << "G = [20, 21, 22]\n";
  fileRef << "G = [20, 21, 22]\n";
  fileRef << "G = [20, 21, 22]\n";
  fileRef << "H = [ 20 , 21 , 22 ]\n";
  fileRef << "H = [ 20 , 21 , 22 ]\n";
  fileRef << "H = [ 20 , 21 , 22 ]\n";
  fileRef.close();

  std::ofstream fileTest;
  fileTest.open(argv[2]);
  fileTest << "1\n";
  fileTest << "1.001\n";
  fileTest << "1.010\n";
  fileTest << " 2\n";
  fileTest << " 2.001\n";
  fileTest << " 2.010\n";
  fileTest << "3\n ";
  fileTest << "3.001 \n";
  fileTest << "3.010 \n";
  fileTest << "     4     \n ";
  fileTest << "     4.001     \n";
  fileTest << "     4.010     \n";
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
  fileTest << "E =         10\n";
  fileTest << "E =         11.001\n";
  fileTest << "E =         12.010\n";
  fileTest << "F = [20,21,22]\n";
  fileTest << "F = [20.001,21.001,22.001]\n";
  fileTest << "F = [20.010,21.010,22.010]\n";
  fileTest << "G = [20, 21, 22]\n";
  fileTest << "G = [20.001, 21.001, 22.001]\n";
  fileTest << "G = [20.010, 21.010, 22.010]\n";
  fileTest << "H = [ 20 , 21 , 22 ]\n";
  fileTest << "H = [ 20.001 , 21.001 , 22.001 ]\n";
  fileTest << "H = [ 20.010 , 21.010 , 22.010 ]\n";
  fileTest.close();

  return EXIT_SUCCESS;
}
