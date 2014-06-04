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

#include "otbMacro.h"

#include "otbSpectralResponse.h"
#include "otbSatelliteRSR.h"
#include "otbReduceSpectralResponse.h"

int otbReduceSpectralResponse(int argc, char * argv[])
{
  if ((argc != 5) && (argc != 6))
    {
    std::cout << argv[0] << std::endl << "\t" << "<Spectral_response_filename>";
    std::cout << "\t" << "<RSR_filename>" << "\t" << "<Nb total satellite band>";
    std::cout << "\t" << "<reflectance mode>" << "\t" << "(<Output_filename>)" << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::SpectralResponse<double, double> ResponseType;
  typedef ResponseType::Pointer ResponsePointerType;

  typedef otb::SatelliteRSR<double, double> SatRSRType;
  typedef SatRSRType::Pointer SatRSRPointerType;

  typedef otb::ReduceSpectralResponse<ResponseType, SatRSRType> ReduceResponseType;
  typedef ReduceResponseType::Pointer ReduceResponseTypePointerType;

  const std::string spectreFile(argv[1]);
  const std::string RSRfile(argv[2]);
  unsigned int nbBand = atoi(argv[3]);
  bool reflectanceMode(atoi(argv[4]));

  //Instantiation
  ResponsePointerType mySpectralResponse = ResponseType::New();
  /** Load the spectral response file*/
  mySpectralResponse->Load(spectreFile, 100.0);

  SatRSRPointerType myRSR = SatRSRType::New();
  /** Set the satelite number of bands */
  myRSR->SetNbBands(nbBand);
  /** Load the satelite response file*/
  myRSR->Load(RSRfile);

  ReduceResponseTypePointerType myReduceResponse = ReduceResponseType::New();
  //Instantiation
  //ResponsePointerType  myResponse=ResponseType::New();
  /** Load the satelite response in the simulator */
  myReduceResponse->SetInputSatRSR(myRSR);
  /** Load the spectral response of the object in the simulator*/
  myReduceResponse->SetInputSpectralResponse(mySpectralResponse);
  /** Set the reflectance or luminance mode */
  myReduceResponse->SetReflectanceMode(reflectanceMode);
  //Load file into vector

  /** Print the input spectral response*/
  //std::cout << mySpectralResponse << std::endl;
  //itk::Indent ind;
  /** Print the input Satelite RSR*/
  //std::cout << myRSR << std::endl;
  //myResponse->PrintSelf(std::cout, ind);
  /** Compute the RSR reduce spectral response*/
  myReduceResponse->CalculateResponse();
  /** Print the Reduce SR*/
  std::cout << myReduceResponse << std::endl;
  if (argc == 6)
    {
    char * outputName = argv[5];
    std::ofstream outputFile(outputName, std::ios::out);

    outputFile << myReduceResponse << std::endl;
    }
  else std::cout << myReduceResponse << std::endl;

  return EXIT_SUCCESS;
}
