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
#include <string>


#include "otbMacro.h"

int otbCompareAsciiTestsEpsilon3_WhiteSpace(int argc, char * argv[])
{
  std::cout << "ASCII COMPARISON TEST: it will use directly the baseline and input files in OTB-Data "
      "to avoid problem with otb-bot." << std::endl;

  // Open input file
  std::ifstream fileTestInput;
  fileTestInput.open(argv[1]);

  // Transfer to temporary object
  std::string strFileTest;
  std::vector<std::string> listLineFileTest;
  while (std::getline(fileTestInput, strFileTest))
    {
    listLineFileTest.push_back(strFileTest);
    }
  fileTestInput.close();

  // Write temporary object without modification
  std::ofstream fileTestOutput;
  fileTestOutput.open(argv[2]);
  for (unsigned int i=0; i < listLineFileTest.size(); ++i)
    {
    fileTestOutput << listLineFileTest[i];
    }
  fileTestOutput.close();

  return EXIT_SUCCESS;
}
