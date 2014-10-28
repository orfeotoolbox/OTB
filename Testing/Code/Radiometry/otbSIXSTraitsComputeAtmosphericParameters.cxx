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


#include "otbSIXSTraits.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

int otbSIXSTraitsComputeAtmosphericParametersTest(int itkNotUsed(argc), char * argv[])
{
  const char * inname   = argv[1];
  const char * outname  = argv[2];

  typedef otb::AtmosphericCorrectionParameters AtmosphericCorrectionParametersType;
  // Instantiating object
  typedef otb::FilterFunctionValues FilterFunctionValuesType;
  FilterFunctionValuesType::Pointer          functionValues = FilterFunctionValuesType::New();
  FilterFunctionValuesType::ValuesVectorType vect;

  double       SolarZenithalAngle(0.);
  double       SolarAzimutalAngle(0.);
  double       ViewingZenithalAngle(0.);
  double       ViewingAzimutalAngle(0.);
  unsigned int Month(0);
  unsigned int Day(0);
  double       AtmosphericPressure(0.);
  double       WaterVaporAmount(0.);
  double       OzoneAmount(0.);
  typedef AtmosphericCorrectionParametersType::AerosolModelType AerosolModelType;

  AerosolModelType AerosolModel;
  double           AerosolOptical(0.);
  double           AtmosphericReflectance(0.);
  double           AtmosphericSphericalAlbedo(0.);
  double           TotalGaseousTransmission(0.);
  double           DownwardTransmittance(0.);
  double           UpwardTransmittance(0.);
  double           UpwardDiffuseTransmittance(0.);
  double           UpwardDirectTransmittance(0.);
  double           UpwardDiffuseTransmittanceForRayleigh(0.);
  double           UpwardDiffuseTransmittanceForAerosol(0.);

  std::ifstream fin;
  std::ofstream fout;

  //Read input file parameters
  fin.open(inname);
  fin >> SolarZenithalAngle; //asol;
  fin >> SolarAzimutalAngle; //phi0;
  fin >> ViewingZenithalAngle; //avis;
  fin >> ViewingAzimutalAngle; //phiv;
  fin >> Month; //month;
  fin >> Day; //jday;

  fin >> AtmosphericPressure; //pressure;
  fin >> WaterVaporAmount; //uw;
  fin >> OzoneAmount; //uo3;
  unsigned int aer(0);
  fin >> aer; //iaer;
  AerosolModel = static_cast<AerosolModelType>(aer);
  fin >> AerosolOptical; //taer55;

  float MinSpectralValue(0.);
  float MaxSpectralValue(0.);
  float value(0.);

  fin >> MinSpectralValue; //wlinf;
  fin >> MaxSpectralValue; //wlsup;

  std::string line;
  std::getline(fin, line);

  while (std::getline(fin, line))
    {
    value = atof(line.c_str());
    vect.push_back(value);
    }
  fin.close();

  functionValues->SetFilterFunctionValues(vect);
  functionValues->SetMinSpectralValue(MinSpectralValue);
  functionValues->SetMaxSpectralValue(MaxSpectralValue);
  functionValues->SetUserStep(.0025);

  otb::SIXSTraits::ComputeAtmosphericParameters(
    SolarZenithalAngle,
    SolarAzimutalAngle,
    ViewingZenithalAngle,
    ViewingAzimutalAngle,
    Month,
    Day,
    AtmosphericPressure,
    WaterVaporAmount,
    OzoneAmount,
    AerosolModel,
    AerosolOptical,
    functionValues,
    AtmosphericReflectance,
    AtmosphericSphericalAlbedo,
    TotalGaseousTransmission,
    DownwardTransmittance,
    UpwardTransmittance,
    UpwardDiffuseTransmittance,
    UpwardDirectTransmittance,
    UpwardDiffuseTransmittanceForRayleigh,
    UpwardDiffuseTransmittanceForAerosol);

  fout.open(outname);
  fout << " ---------------------------------------------------------" << std::endl;
  fout << "Inputs values:" << std::setprecision(10) << std::endl;
  fout << "   ----->  SolarZenithalAngle :                        " << SolarZenithalAngle << std::endl;
  fout << "   ----->  SolarAzimutalAngle :                        " << SolarAzimutalAngle << std::endl;
  fout << "   ----->  ViewingZenithalAngle :                      " << ViewingZenithalAngle << std::endl;
  fout << "   ----->  ViewingAzimutalAngle :                      " << ViewingAzimutalAngle << std::endl;
  fout << "   ----->  Month :                                     " << Month << std::endl;
  fout << "   ----->  Day :                                       " << Day << std::endl;
  fout << "   ----->  AtmosphericPressure :                       " << AtmosphericPressure << std::endl;
  fout << "   ----->  WaterVaporAmount :                          " << WaterVaporAmount << std::endl;
  fout << "   ----->  OzoneAmount :                               " << OzoneAmount << std::endl;
  fout << "   ----->  AerosolModel :                              " << aer << std::endl;
  fout << "   ----->  AerosolOptical :                            " << AerosolOptical << std::endl;
  fout << "   ----->  MinSpectralValue :                          " << MinSpectralValue << std::endl;
  fout << "   ----->  MaxSpectralValue :                          " << MaxSpectralValue << std::endl;
  fout << "   ----->  UserStep :                                  " << functionValues->GetUserStep() << std::endl;
  fout << " ---------------------------------------------------------" << std::endl;
  fout << "Outputs values:" << std::endl;
  fout << "   ----->  atmospheric reflectance :                   " << AtmosphericReflectance << std::endl;
  fout << "   ----->  atmospheric spherical albedo :              " << AtmosphericSphericalAlbedo << std::endl;
  fout << "   ----->  total gaseous transmission :                " << TotalGaseousTransmission << std::endl;
  fout << "   ----->  downward transmittance :                    " << DownwardTransmittance << std::endl;
  fout << "   ----->  upward transmittance :                      " << UpwardTransmittance << std::endl;
  fout << "   ----->  upward diffuse transmittance :              " << UpwardDiffuseTransmittance << std::endl;
  fout << "   ----->  upward direct transmittance :               " << UpwardDirectTransmittance << std::endl;
  fout << "   ----->  upward diffuse transmittance for rayleigh : " << UpwardDiffuseTransmittanceForRayleigh <<
  std::endl;
  fout << "   ----->  upward diffuse transmittance for aerosols : " << UpwardDiffuseTransmittanceForAerosol <<
  std::endl;
  fout << "   ----->  MinSpectralValue update:                    " << functionValues->GetMinSpectralValue() <<
  std::endl;
  fout << "   ----->  MaxSpectralValue update :                   " << functionValues->GetMaxSpectralValue() <<
  std::endl;
  fout << " ---------------------------------------------------------" << std::endl;
  fout << "Input wavelength band values [" << functionValues->GetFilterFunctionValues().size() << "]:" << std::endl;
  for (unsigned int i = 0; i < functionValues->GetFilterFunctionValues().size(); ++i)
    {
    fout << "    " << functionValues->GetFilterFunctionValues()[i] << std::endl;
    }
  fout << " ---------------------------------------------------------" << std::endl;
  fout << "Output wavelength band values 6S [" << functionValues->GetFilterFunctionValues6S().size() << "]:" <<
  std::endl;
  for (unsigned int i = 0; i < functionValues->GetFilterFunctionValues6S().size(); ++i)
    {
    fout << "    " << functionValues->GetFilterFunctionValues6S()[i] << std::endl;
    }
  fout << std::endl;
  fout.close();
  return EXIT_SUCCESS;
}
