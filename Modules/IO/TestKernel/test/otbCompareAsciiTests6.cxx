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

int otbCompareAsciiTests6(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " referenceFile testFile" << std::endl;
    return EXIT_FAILURE;
    }

  std::ofstream fileRef;
  fileRef.open(argv[1]);
  fileRef << "ImageRegion (0x7fff1639a860)\n";
  fileRef << "0x7fff1639a860\n";
  fileRef << "0.142355\t:230\n";
  fileRef.close();

  std::ofstream fileTest;
  fileTest.open(argv[2]);
  fileTest << "ImageRegion (00C3F798)\n";
  fileTest << "000000001AF56D20\n";
  fileTest << "0.142356\t:230\n";
  fileTest.close();

  return EXIT_SUCCESS;
}
