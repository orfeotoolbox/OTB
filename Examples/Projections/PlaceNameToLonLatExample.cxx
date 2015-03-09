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


// Software Guide : BeginLatex
//
// This example will show how to retrieve the longitude and latitude from
// a place using the name of the city or the address. For that, we will
// use the \doxygen{otb}{PlaceNameToLonLat} class.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbPlaceNameToLonLat.h"
// Software Guide : EndCodeSnippet

int main(int argc, char* argv[])
{

  if (argc != 2)
    {
    std::cout << argv[0] << " <place name> "
              << std::endl;

    return EXIT_FAILURE;
    }

  // Software Guide : BeginLatex
  //
  // You instantiate the class and pass the name you want to look for as a
  // std::string to the SetPlaceName method.
  //
  // The call to evaluate will trigger the retrival process.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  otb::PlaceNameToLonLat::Pointer pn2LL = otb::PlaceNameToLonLat::New();
  pn2LL->SetPlaceName(std::string(argv[1]));
  pn2LL->Evaluate();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // To get the data, you can simply call the GetLon and GetLat methods.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  double lon = pn2LL->GetLon();
  double lat = pn2LL->GetLat();

  std::cout << "Latitude: " << lat << std::endl;
  std::cout << "Longitude: " << lon << std::endl;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // If you tried with a string such as "Toulouse" -- a city where the
  // heart of OTB relies -- you should obtain something
  // like:
  //
  // \begin{verbatim}
  // Latitude: 43.6044
  // Longitude: 1.44295
  // \end{verbatim}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}
