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


#include "otbCoordinateToName.h"


int main( int argc, char* argv[] )
{

  if (argc!=3)
  {
    std::cout << argv[0] <<" <lon> <lat>"
              << std::endl;

    return EXIT_FAILURE;
  }


  otb::CoordinateToName::Pointer conv = otb::CoordinateToName::New();
  conv->SetLon(atof(argv[1]));
  conv->SetLat(atof(argv[2]));
  conv->Evaluate();

  std::string name = conv->GetPlaceName();
  std::string country = conv->GetCountryName();

  std::cout << "Nearby place: " << name << std::endl;
  std::cout << "Country: " << country << std::endl;


  return EXIT_SUCCESS;

}
