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


#include <fstream>
#include <cstdlib>

#include "otbCoordinateToName.h"
#include "otbCurlHelperStub.h"
#include "otbThreads.h"

int otbCoordinateToNameTest(int argc, char* argv[])
{

  if (argc < 4)
    {
    std::cout << argv[0] << " <lon> <lat> <outputfile>"
              << std::endl;

    return EXIT_FAILURE;
    }

  const char * outFileName = argv[3];

  otb::CoordinateToName::Pointer conv = otb::CoordinateToName::New();
  conv->SetLon(atof(argv[1]));
  conv->SetLat(atof(argv[2]));

  //only for testing (remove dependency to network conditions)
  otb::CurlHelperStub::Pointer curlStub = otb::CurlHelperStub::New();
  conv->SetCurl(curlStub);

  if ((argc > 4) && atoi(argv[4]) == 1)
    {
    conv->MultithreadOn();
    conv->Evaluate();
    otb::Threads::Sleep(10000000); //Make sure that the web request has the time to complete
    }
  else
    {
    conv->MultithreadOff();
    conv->Evaluate();
    }

  std::string name = conv->GetPlaceName();
  std::string country = conv->GetCountryName();

  std::cout << "Nearby place: " << name << std::endl;
  std::cout << "Country: " << country << std::endl;

  std::ofstream file;
  file.open(outFileName);
  file << "Nearby place: " << name << std::endl;
  file << "Country: " << country << std::endl;
  file.close();

  return EXIT_SUCCESS;

}
