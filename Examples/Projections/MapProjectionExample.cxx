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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbMapProjections.h"

int main( int argc, char* argv[] )
{
  if(argc < 3  )
  {
    std::cout << argv[0] <<" <lon> <lat> <outputfile>"  << std::endl;

    return EXIT_FAILURE;
  }

  double lon = atof(argv[1]);
  double lat = atof(argv[2]);
  const char * outFileName = argv[3];

  std::ofstream file;
  file.open(outFileName);
  file << std::setprecision(15);

  otb::UtmForwardProjection::Pointer lUtmProjection = otb::UtmForwardProjection::New();




  return EXIT_SUCCESS;
}
