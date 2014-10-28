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

#include "otbSatelliteRSR.h"
#include "otbReduceSpectralResponse.h"

typedef otb::SpectralResponse<double, double> ResponseType;
typedef ResponseType::Pointer ResponsePointerType;
typedef ResponseType::VectorPairType SpectrumType;
typedef ResponseType::PairType PairType;
typedef otb::SatelliteRSR<double, double> SatRSRType;
typedef SatRSRType::Pointer SatRSRPointerType;
typedef otb::ReduceSpectralResponse<ResponseType, SatRSRType> ReduceResponseType;
typedef ReduceResponseType::Pointer ReduceResponseTypePointerType;

int otbReduceSpectralResponse(int argc, char * argv[])
{
  if ((argc != 5) && (argc != 6))
    {
    std::cout << argv[0] << std::endl << "\t" << "<Spectral_response_filename>";
    std::cout << "\t" << "<RSR_filename>" << "\t" << "<Nb total satellite band>";
    std::cout << "\t" << "<reflectance mode>" << "\t" << "(<Output_filename>)" << std::endl;
    return EXIT_FAILURE;
    }

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
  //Instantiation
  ReduceResponseTypePointerType myReduceResponse = ReduceResponseType::New();
  /** Load the satelite response in the simulator */
  myReduceResponse->SetInputSatRSR(myRSR);
  /** Load the spectral response of the object in the simulator*/
  myReduceResponse->SetInputSpectralResponse(mySpectralResponse);
  /** Set the reflectance or luminance mode */
  myReduceResponse->SetReflectanceMode(reflectanceMode);
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

bool check_spectral_response(ResponsePointerType spectralResponse,
                             SpectrumType::const_iterator first,
                             SpectrumType::const_iterator last,
                             ResponseType::ValuePrecisionType tolerance)
{
  while( first != last )
    {
    ResponseType::ValuePrecisionType expected = (*first).second;
    ResponseType::ValuePrecisionType actual = (*spectralResponse)((*first).first);
    if(fabs(expected - actual) > tolerance) return false;
    ++first;
    }
  return true;
}

int otbReduceSpectralResponseSimpleValues(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cout << argv[0] << "\t <temporary spectral response filename>";
    std::cout << "\t <reflectance mode>";
    std::cout << std::endl;
    return EXIT_FAILURE;
    }

  // let's create a spectral response
  const ResponseType::PrecisionType lambdaMin(0.4);
  const ResponseType::PrecisionType lambdaMax(1.4);
  const ResponseType::PrecisionType lambdaStep(0.001);
  ResponseType::PrecisionType currentLambda(lambdaMin);
  SpectrumType spectrum;
  while(currentLambda < lambdaMax)
    {
    PairType spectralValue;
    spectralValue.first = currentLambda;
    spectralValue.second = ResponseType::ValuePrecisionType(1.0);
    spectrum.push_back(spectralValue);
    currentLambda += lambdaStep;
    }
  ResponsePointerType spectralResponse = ResponseType::New();
  spectralResponse->SetResponse(spectrum);

  // Check that the spectral response is OK
  const ResponseType::ValuePrecisionType tolerance = 10e-6;
  if(!check_spectral_response(spectralResponse, spectrum.begin(),
                              spectrum.end(), tolerance))
    {
    std::cout << "Error detected in stored spectral response (without guess)"
              << std::endl;
    return EXIT_FAILURE;
    }
  spectralResponse->SetUsePosGuess(true);
  if(!check_spectral_response(spectralResponse, spectrum.begin(),
                              spectrum.end(), tolerance))
    {
    std::cout << "Error detected in stored spectral response (with guess)"
              << std::endl;
    return EXIT_FAILURE;
    }
 
  // Let's create a satellite RSR with 2 bands and constant solar irradiance per band
  // Disjoint bands and B0 is shorter wavelength than B1 for simplicity
  const std::string RSRfileName(argv[1]);
  std::ofstream satFile(RSRfileName.c_str());
  const ResponseType::PrecisionType lambdaMinB0(0.45);
  const ResponseType::PrecisionType lambdaMaxB0(0.65);
  const ResponseType::PrecisionType lambdaMinB1(0.75);
  const ResponseType::PrecisionType lambdaMaxB1(0.85);
  const ResponseType::ValuePrecisionType rsrValueB0(0.5);
  const ResponseType::ValuePrecisionType rsrValueB1(1.0);
  const ResponseType::ValuePrecisionType solarIrradianceB0(2.0);
  const ResponseType::ValuePrecisionType solarIrradianceB1(4.0);
  currentLambda = lambdaMin;
  while(currentLambda < lambdaMax)
    {
    ResponseType::ValuePrecisionType solarIrradiance(0);
    ResponseType::ValuePrecisionType rsrB0(0);
    ResponseType::ValuePrecisionType rsrB1(0);
    // B0
    if(currentLambda >= lambdaMinB0 && currentLambda <= lambdaMaxB0)
      {
      solarIrradiance = solarIrradianceB0;
      rsrB0 = rsrValueB0;
      }
    // B1
    if(currentLambda >= lambdaMinB1 && currentLambda <= lambdaMaxB1)
      {
      solarIrradiance = solarIrradianceB1;
      rsrB1 = rsrValueB1;
      }
    satFile.setf( std::ios::fixed, std:: ios::floatfield );
    satFile.precision(4);
    satFile << currentLambda << "\t"
           << solarIrradiance << "\t"
           << rsrB0 << "\t"
           << rsrB1 << "\t" << std::endl;
    currentLambda += lambdaStep;
    }
  satFile.close();

  SatRSRPointerType myRSR = SatRSRType::New();
  myRSR->SetNbBands(2);
  myRSR->Load(RSRfileName);

  ReduceResponseTypePointerType myReduceResponse = ReduceResponseType::New();
  myReduceResponse->SetInputSatRSR(myRSR);
  myReduceResponse->SetInputSpectralResponse(spectralResponse);
  myReduceResponse->SetReflectanceMode(static_cast<bool>(atoi(argv[2])));
  myReduceResponse->CalculateResponse();

  //check B0
  ResponseType::ValuePrecisionType b0Result = (*myReduceResponse)(0);
  ResponseType::PrecisionType centralLambda = 0.5*(lambdaMaxB0+lambdaMinB0);
  ResponseType::ValuePrecisionType b0Expected = ((*spectralResponse)(centralLambda));

  if(fabs(b0Result-b0Expected)>tolerance)
    {
    std::cout << "Wrong value for B0: expected eq. luminance = " << b0Expected
              << "; got " << b0Result
              << std::endl;
    return EXIT_FAILURE;
    }

  //check B1
  ResponseType::ValuePrecisionType b1Result = (*myReduceResponse)(1);
  centralLambda = 0.5*(lambdaMaxB1+lambdaMinB1);
  ResponseType::ValuePrecisionType b1Expected = ((*spectralResponse)(centralLambda));

  if(fabs(b1Result-b1Expected)>tolerance)
    {
    std::cout << "Wrong value for B1: expected eq. luminance = " << b1Expected
              << "; got " << b1Result
              << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
