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

#include "otbPlaceNameToLonLat.h"

int main( int argc, char* argv[] )
{
  
  if(argc!=2)
  {
    std::cout << argv[0] <<" <place name> "
        << std::endl;

    return EXIT_FAILURE;
  }
  
  
  otb::PlaceNameToLonLat::Pointer pn2LL = otb::PlaceNameToLonLat::New();
  pn2LL->SetPlaceName(std::string(argv[1]));
  pn2LL->Evaluate();
  
  double lon = pn2LL->GetLon();
  double lat = pn2LL->GetLat();
  
  std::cout << "Latitude: " << lat << std::endl;
  std::cout << "Longitude: " << lon << std::endl;
    
  return EXIT_SUCCESS;

}