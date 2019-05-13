/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbMacro.h"

#include "otbSatelliteRSR.h"
#include "otbReduceSpectralResponseClassifierRAndNIR.h"


int otbReduceSpectralResponseClassifierRAndNIR(int argc, char * argv[])
{
  if ( argc!=6 )
  {
    std::cout << argv[0] << std::endl << "\t" << "<Spectral_response_filename>"<< "\t" << "<RSR_filename>"<< "\t" << "<Nb total satellite band>" << "\t" << "<Red index>"<< "\t" << "<NIR index>"<< std::endl;

    //bin/otbReduceSpectralResponseFunctor ~/manuel/Datas_Geos/Simulateur_Spectres/usgs_datas/JHU/becknic/vegetation/txt/grass.txt  ~/manuel/Datas_Geos/Rsr/SPOT3/HRV1/rep6S.dat 3 1 > a.out
//     bin/otbReduceSpectralResponseClassifierRAndNIR  ~/manuel/Datas_Geos/Simulateur_Spectres/usgs_datas/JHU/becknic/vegetation/txt/decidous.txt ~/manuel/Datas_Geos/Rsr/SPOT5/VEGETATION/rep6S.dat 4 1 2

    return EXIT_FAILURE;
  }


  typedef otb::SpectralResponse< double, double>  ResponseType;

  typedef otb::SatelliteRSR< double, double>  SatRSRType;

  typedef otb::ReduceSpectralResponse < ResponseType, SatRSRType>  ReduceResponseType;
  typedef ReduceResponseType::Pointer  ReduceResponseTypePointerType;

  typedef otb::Functor::NDVI<double, double>                                               TFunctionType;
  typedef otb::ReduceSpectralResponseClassifierRAndNIR <ReduceResponseType, TFunctionType> ReduceSpectralResponseClassifierRAndNIRType;
  typedef ReduceSpectralResponseClassifierRAndNIRType::Pointer  ReduceSpectralResponseClassifierRAndNIRPointerType;

  const std::string spectreFile(argv[1]);
  const std::string RSRfile(argv[2]);
  unsigned int nbBand = atoi(argv[3]);

  unsigned int RBand = atoi(argv[4]);
  unsigned int NIRBand = atoi(argv[5]);

  ReduceResponseTypePointerType  myReduceResponse=ReduceResponseType::New();
  //Instantiation
  //ResponsePointerType  myResponse=ResponseType::New();
  /** Load the spectral response of the object and the satellite response in the simulator */
  myReduceResponse->LoadInputsFromFiles(spectreFile, RSRfile, nbBand, 100.0, 1.0);

  //Instantiation
  ReduceSpectralResponseClassifierRAndNIRPointerType  myIndice=ReduceSpectralResponseClassifierRAndNIRType::New();

  myIndice->SetInputReduceSpectralResponse(myReduceResponse);

  //FunctorType
  //myIndice->SetFunctor(myReduceResponse);
  myIndice->SetRBandNumber(RBand);
  myIndice->SetNIRBandNumber(NIRBand);

  std::cout << "NDVI value " << (*myIndice)() << std::endl;

  return EXIT_SUCCESS;
}
