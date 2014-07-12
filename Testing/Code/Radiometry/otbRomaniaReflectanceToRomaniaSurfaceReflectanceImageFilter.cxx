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
#include "itkMacro.h"

#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbAtmosphericRadiativeTerms.h"
#include <fstream>
#include <iostream>

int otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char *              inputFileName  = argv[1];
  const char *              outputFileName = argv[2];
  const char *              paramFile = argv[3];
  std::vector<const char *> wavelenghFiles;

  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputImageType;
  typedef otb::VectorImage<PixelType, Dimension> OutputImageType;
  typedef otb::ImageFileReader<InputImageType>   ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>  WriterType;

  typedef otb::ReflectanceToSurfaceReflectanceImageFilter<InputImageType,
      OutputImageType>
  ReflectanceToSurfaceReflectanceImageFilterType;
  typedef otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
  CorrectionParametersTo6SRadiativeTermsType;
  typedef otb::AtmosphericCorrectionParameters                 CorrectionParametersType;
  typedef otb::AtmosphericRadiativeTerms                       RadiativeTermsType;
  typedef otb::FilterFunctionValues                            FilterFunctionValuesType;
  typedef CorrectionParametersType::AerosolModelType           AerosolModelType;
  typedef FilterFunctionValuesType::WavelengthSpectralBandType ValueType;
  typedef FilterFunctionValuesType::ValuesVectorType           ValuesVectorType;

  RadiativeTermsType::Pointer                         radiative     = RadiativeTermsType::New();
  CorrectionParametersType::Pointer                   param         = CorrectionParametersType::New();
  CorrectionParametersTo6SRadiativeTermsType::Pointer corrToRadia   = CorrectionParametersTo6SRadiativeTermsType::New();
  FilterFunctionValuesType::Pointer                   functionValues;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->UpdateOutputInformation();
  unsigned int nbChannel = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  for (unsigned int i = 0; i < nbChannel; ++i)
    {
    wavelenghFiles.push_back(argv[i + 4]);
    }

  ValueType val = 0.0025;

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
  fin.open(paramFile);
  //Read input file parameters
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
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(aer);
  fin >> aerosolOptical; //taer55;
  fin.close();
  // Set atmospheric parameters
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

  ValuesVectorType vect;
  for (unsigned int j = 0; j < nbChannel; ++j)
    {
    functionValues = FilterFunctionValuesType::New();
    vect.clear();

    // Filter function values initialization
    float minSpectralValue(0.);
    float maxSpectralValue(0.);
    float value(0.);

    std::ifstream fin;
    //Read input file parameters
    fin.open(wavelenghFiles[j]);
    fin >> minSpectralValue; //wlinf;
    fin >> maxSpectralValue; //wlsup;

    while (fin.good())
      {
      std::string line;
      std::getline(fin, line);
      if (!line.empty())
       {
       value = atof(line.c_str());
       vect.push_back(value);
       }
      }

    fin.close();

    functionValues->SetFilterFunctionValues(vect);
    functionValues->SetMinSpectralValue(minSpectralValue);
    functionValues->SetMaxSpectralValue(maxSpectralValue);
    functionValues->SetUserStep(val);

    param->SetWavelengthSpectralBandWithIndex(j, functionValues);
    }

  corrToRadia->SetInput(param);
  corrToRadia->Update();

  // Instantiating object
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer filter = ReflectanceToSurfaceReflectanceImageFilterType::New();
  filter->SetAtmosphericRadiativeTerms(corrToRadia->GetOutput());
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
