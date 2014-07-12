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

#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbAtmosphericRadiativeTerms.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>

int otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms(int itkNotUsed(argc), char * argv[])
{
  const char * wavelenghFile  = argv[1];
  const char * outputFile     = argv[2];

  typedef otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms CorrectionParametersTo6SRadiativeTermsType;
  typedef otb::AtmosphericCorrectionParameters                              CorrectionParametersType;
  typedef otb::AtmosphericRadiativeTerms                                    RadiativeTermsType;
  typedef CorrectionParametersType::AerosolModelType                        AerosolModelType;
  typedef otb::FilterFunctionValues                                         FilterFunctionValuesType;
  typedef FilterFunctionValuesType::WavelengthSpectralBandType              ValueType;
  typedef FilterFunctionValuesType::ValuesVectorType                        ValuesVectorType;

  // Instantiating object
  CorrectionParametersTo6SRadiativeTermsType::Pointer object         = CorrectionParametersTo6SRadiativeTermsType::New();
  CorrectionParametersType::Pointer                   param          = CorrectionParametersType::New();
  AerosolModelType                                    aerosolModel;
  FilterFunctionValuesType::Pointer                   functionValues = FilterFunctionValuesType::New();
  ValuesVectorType                                    vect;
  RadiativeTermsType::Pointer                         radiative      = RadiativeTermsType::New();

  ValueType val = 0.0025;

  //for(unsigned int i=0; i<wavelenghFiles.size(); ++i)
  //{
  //functionValues = FilterFunctionValuesType::New();
  vect.clear();

  // Filter function values initialization
  float minSpectralValue(0.);
  float maxSpectralValue(0.);
  float value(0.);
  // Correction parameters initialization
  double       solarZenithalAngle(0.);
  double       solarAzimutalAngle(0.);
  double       viewingZenithalAngle(0.);
  double       viewingAzimutalAngle(0.);
  unsigned int month(0);
  unsigned int day(0);
  double       atmosphericPressure(0.);
  double       waterVaporAmount(0.);
  double       ozoneAmount(0.);
  double       aerosolOptical(0.);

  std::ifstream fin;
  std::ofstream fout;
  //Read input file parameters
  fin.open(wavelenghFile);
  fin >> solarZenithalAngle; //asol;
  fin >> solarAzimutalAngle; //phi0;
  fin >> viewingZenithalAngle; //avis;
  fin >> viewingAzimutalAngle; //phiv;
  fin >> month; //month;
  fin >> day; //jday;
  fin >> atmosphericPressure; //pressure;
  fin >> waterVaporAmount; //uw;
  fin >> ozoneAmount; //uo3;
  unsigned int aer(0);
  fin >> aer; //iaer;
  aerosolModel = static_cast<AerosolModelType>(aer);
  fin >> aerosolOptical; //taer55;
  fin >> minSpectralValue; //wlinf;
  fin >> maxSpectralValue; //wlsup;

  std::string line;
  std::getline(fin, line);
  while (std::getline(fin, line))
    {
    value = atof(line.c_str());
    vect.push_back(value);
    }

  fin.close();
  functionValues->SetFilterFunctionValues(vect);
  functionValues->SetMinSpectralValue(minSpectralValue);
  functionValues->SetMaxSpectralValue(maxSpectralValue);
  functionValues->SetUserStep(val);
  param->SetWavelengthSpectralBandWithIndex(0, functionValues);
  //}

  //aerosolModel = static_cast<AerosolModelType>(::atoi(argv[16]));

  // Set parameters
  param->SetSolarZenithalAngle(static_cast<double>(solarZenithalAngle));
  param->SetSolarAzimutalAngle(static_cast<double>(solarAzimutalAngle));
  param->SetViewingZenithalAngle(static_cast<double>(viewingZenithalAngle));
  param->SetViewingAzimutalAngle(static_cast<double>(viewingAzimutalAngle));
  param->SetMonth(month);
  param->SetDay(day);
  param->SetAtmosphericPressure(static_cast<double>(atmosphericPressure));
  param->SetWaterVaporAmount(static_cast<double>(waterVaporAmount));
  param->SetOzoneAmount(static_cast<double>(ozoneAmount));
  param->SetAerosolModel(aerosolModel);
  param->SetAerosolOptical(static_cast<double>(aerosolOptical));

  object->SetInput(param);
  object->Update();
  radiative = object->GetOutput();

  fout.open(outputFile);
  fout << " ---------------------------------------------------------" << std::endl;
  fout << "Inputs values:" << std::setprecision(10) << std::endl;
  fout << "   ----->  SolarZenithalAngle :                              " << solarZenithalAngle << std::endl;
  fout << "   ----->  SolarAzimutalAngle :                              " << solarAzimutalAngle << std::endl;
  fout << "   ----->  ViewingZenithalAngle :                            " << viewingZenithalAngle << std::endl;
  fout << "   ----->  ViewingAzimutalAngle :                            " << viewingAzimutalAngle << std::endl;
  fout << "   ----->  Month :                                           " << month << std::endl;
  fout << "   ----->  Day :                                             " << day << std::endl;
  fout << "   ----->  AtmosphericPressure :                             " << atmosphericPressure << std::endl;
  fout << "   ----->  WaterVaporAmount :                                " << waterVaporAmount << std::endl;
  fout << "   ----->  OzoneAmount :                                     " << ozoneAmount << std::endl;
  fout << "   ----->  AerosolModel :                                    " << aer << std::endl;
  fout << "   ----->  AerosolOptical :                                  " << aerosolOptical << std::endl;
  fout << "   ----->  MinSpectralValue :                                " << minSpectralValue << std::endl;
  fout << "   ----->  MaxSpectralValue :                                " << maxSpectralValue << std::endl;
  fout << "   ----->  UserStep :                                        " << functionValues->GetUserStep() << std::endl;
  fout << " ---------------------------------------------------------" << std::endl;
  fout << "Outputs values:" << std::endl;
  fout << "   ----->  atmospheric reflectance :                         " <<
  radiative->GetIntrinsicAtmosphericReflectance(0) << std::endl;
  fout << "   ----->  atmospheric spherical albedo :                    " << radiative->GetSphericalAlbedo(0) <<
  std::endl;
  fout << "   ----->  total gaseous transmission :                      " <<
  radiative->GetTotalGaseousTransmission(0) << std::endl;
  fout << "   ----->  downward transmittance :                          " << radiative->GetDownwardTransmittance(0) <<
  std::endl;
  fout << "   ----->  upward transmittance :                            " << radiative->GetUpwardTransmittance(0) <<
  std::endl;
  fout << "   ----->  upward diffuse transmittance :                    " <<
  radiative->GetUpwardDiffuseTransmittance(0) << std::endl;
  fout << "   ----->  upward direct transmittance :                     " <<
  radiative->GetUpwardDirectTransmittance(0) << std::endl;
  fout << "   ----->  upward diffuse transmittance for rayleigh :       " <<
  radiative->GetUpwardDiffuseTransmittanceForRayleigh(0) << std::endl;
  fout << "   ----->  upward diffuse transmittance for aerosols :       " <<
  radiative->GetUpwardDiffuseTransmittanceForAerosol(0) << std::endl;
  fout << "   ----->  MinSpectralValue update:                          " << functionValues->GetMinSpectralValue() <<
  std::endl;
  fout << "   ----->  MaxSpectralValue update :                         " << functionValues->GetMaxSpectralValue() <<
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
    fout << "            " << functionValues->GetFilterFunctionValues6S()[i] << std::endl;
    }
  fout << std::endl;
  fout.close();
  return EXIT_SUCCESS;
}
