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

#include "itkExceptionObject.h"
#include "otbMacro.h"

int otbCompareAsciiTestsEpsilon3_WhiteSpace(int argc, char * argv[])
{
  std::cout << "ASCII COMPARISON TEST: it will use directly the baseline and input files in OTB-Data "
      "to avoid problem with otb-bot." << std::endl;

  return EXIT_SUCCESS;
}
