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
#include "otbAtmosphericEffects.h"
#include "otbReduceSpectralResponse.h"

#include "otbAtmosphericCorrectionParameters.h"
#include "otbImageMetadataCorrectionParameters.h"

int otbAtmosphericEffects(int argc, char* argv[])
{

  typedef otb::SpectralResponse<double, double> SpectralResponseType;

  typedef otb::SatelliteRSR<double, double> SatRSRType;

  typedef otb::ReduceSpectralResponse<SpectralResponseType, SatRSRType> ReduceSpectralResponseType;

  typedef otb::AtmosphericEffects<SpectralResponseType, SatRSRType> AtmosphericEffectsType;


  typedef otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms CorrectionParametersToRadiativeTermsType;

  typedef otb::AtmosphericCorrectionParameters  AtmoCorrectionParametersType;
  typedef AtmoCorrectionParametersType::Pointer AtmoCorrectionParametersPointerType;

  typedef AtmoCorrectionParametersType::AerosolModelType AerosolModelType;

  typedef otb::ImageMetadataCorrectionParameters AcquiCorrectionParametersType;
  typedef AcquiCorrectionParametersType::Pointer AcquiCorrectionParametersPointerType;

  typedef otb::AtmosphericRadiativeTerms         AtmosphericRadiativeTermsType;
  typedef AtmosphericRadiativeTermsType::Pointer AtmosphericRadiativeTermsPointerType;


  if (argc != 15)
  {
    std::cout << argv[0] << " <spectrum_filename>"
              << "\t"
              << "<Gabarit_SAT_fileSRname>"
              << "\t"
              << "<nbBands>"
              << "\t"
              << "<day>"
              << "\t"
              << "<month>"
              << "\t"
              << "<zenithSolarAngle>"
              << "\t"
              << "<azimutSolarAngle>"
              << "\t"
              << "<viewingZenitalAngle>"
              << "\t"
              << "<viewingAzimutalAngle>"
              << "\t"
              << "<atmoPressure>"
              << "\t"
              << "<waterVaporAmount>"
              << "\t"
              << "<ozoneAmount>"
              << "\t"
              << "<aerosolModelValue>"
              << "\t"
              << "<aerosolOptical>"
              << "\t" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string file(argv[1]);
  const std::string fileSatG(argv[2]);
  unsigned int      nbBand = atoi(argv[3]);
  const int         day(atoi(argv[4]));
  const int         month(atoi(argv[5]));
  const double      zenithSolarAngle(static_cast<double>(atof(argv[6])));
  const double      azimutSolarAngle(static_cast<double>(atof(argv[7])));
  const double      viewingZenitalAngle(static_cast<double>(atof(argv[8])));
  const double      viewingAzimutalAngle(static_cast<double>(atof(argv[9])));
  const double      atmoPressure(static_cast<double>(atof(argv[10])));
  const double      waterVaporAmount(static_cast<double>(atof(argv[11])));
  const double      ozoneAmount(static_cast<double>(atof(argv[12])));
  const int         aerosolModelValue(::atoi(argv[13]));
  const double      aerosolOptical(static_cast<double>(atof(argv[14])));


  // Instantiation
  SpectralResponseType::Pointer spectrum = SpectralResponseType::New();
  SatRSRType::Pointer           satRSR   = SatRSRType::New();
  // AtmosphericCorrectionParametersType::Pointer dataAtmosphericCorrectionParameters = AtmosphericCorrectionParametersType::New();

  AcquiCorrectionParametersPointerType paramAcqui = AcquiCorrectionParametersType::New();
  AtmoCorrectionParametersPointerType  paramAtmo  = AtmoCorrectionParametersType::New();

  ReduceSpectralResponseType::Pointer reduceSpectralResponse   = ReduceSpectralResponseType::New();
  AtmosphericEffectsType::Pointer     atmosphericEffectsFilter = AtmosphericEffectsType::New();


  spectrum->Load(file, 100.0);

  satRSR->SetNbBands(nbBand);
  satRSR->Load(fileSatG);

  // Set parameters
  paramAcqui->SetSolarZenithalAngle(zenithSolarAngle);
  paramAcqui->SetSolarAzimutalAngle(azimutSolarAngle);
  paramAcqui->SetViewingZenithalAngle(viewingZenitalAngle);
  paramAcqui->SetViewingAzimutalAngle(viewingAzimutalAngle);
  paramAcqui->SetMonth(month);
  paramAcqui->SetDay(day);
  paramAtmo->SetAtmosphericPressure(atmoPressure);
  paramAtmo->SetWaterVaporAmount(waterVaporAmount);
  paramAtmo->SetOzoneAmount(ozoneAmount);
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(aerosolModelValue);
  paramAtmo->SetAerosolModel(aerosolModel);
  paramAtmo->SetAerosolOptical(aerosolOptical);

  AtmosphericRadiativeTermsPointerType radiative = CorrectionParametersToRadiativeTermsType::Compute(paramAtmo, paramAcqui);


  // Compute Reduce Spectral Response
  reduceSpectralResponse->SetInputSatRSR(satRSR);
  reduceSpectralResponse->SetInputSpectralResponse(spectrum);
  reduceSpectralResponse->CalculateResponse();

  atmosphericEffectsFilter->SetAtmosphericRadiativeTerms(radiative);
  atmosphericEffectsFilter->SetInputSatRSR(satRSR);
  atmosphericEffectsFilter->SetInputSpectralResponse(reduceSpectralResponse->GetReduceResponse());
  atmosphericEffectsFilter->Process();

  atmosphericEffectsFilter->GetCorrectedSpectralResponse();


  return EXIT_SUCCESS;
}
