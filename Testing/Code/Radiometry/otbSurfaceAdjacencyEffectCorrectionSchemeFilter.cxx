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

#include "otbSurfaceAdjacencyEffectCorrectionSchemeFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbAtmosphericRadiativeTerms.h"
#include <fstream>
#include <iostream>

int otbSurfaceAdjacencyEffectCorrectionSchemeFilter(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
  const char * paramFile = argv[5];

  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputImageType;
  typedef otb::VectorImage<PixelType, Dimension> OutputImageType;
  typedef otb::ImageFileReader<InputImageType>   ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>  WriterType;

  typedef otb::SurfaceAdjacencyEffectCorrectionSchemeFilter<InputImageType,
      OutputImageType>
  SurfaceAdjacencyEffectCorrectionSchemeFilterType;


  typedef otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms     CorrectionParametersToRadiativeTermsType;

  typedef otb::AtmosphericCorrectionParameters                              AtmoCorrectionParametersType;
  typedef AtmoCorrectionParametersType::Pointer                    AtmoCorrectionParametersPointerType;
  typedef AtmoCorrectionParametersType::AerosolModelType                    AerosolModelType;

  typedef otb::ImageMetadataCorrectionParameters                             AcquiCorrectionParametersType;
  typedef AcquiCorrectionParametersType::Pointer                   AcquiCorrectionParametersPointerType;

  typedef otb::AtmosphericRadiativeTerms                                    AtmosphericRadiativeTermsType;
  typedef AtmosphericRadiativeTermsType::Pointer                   AtmosphericRadiativeTermsPointerType;
  typedef otb::AtmosphericRadiativeTerms::DataVectorType DataVectorType;

  typedef otb::FilterFunctionValues                                     FilterFunctionValuesType;
  typedef FilterFunctionValuesType::WavelengthSpectralBandType          ValueType;                //float
  typedef FilterFunctionValuesType::ValuesVectorType                    ValuesVectorType;         //std::vector<float>

  typedef AcquiCorrectionParametersType::WavelengthSpectralBandVectorType        WavelengthSpectralBandVectorType;


  AcquiCorrectionParametersPointerType paramAcqui = AcquiCorrectionParametersType::New();
  AtmoCorrectionParametersPointerType  paramAtmo = AtmoCorrectionParametersType::New();
  FilterFunctionValuesType::Pointer                   functionValues;

  // Instantiating object
  SurfaceAdjacencyEffectCorrectionSchemeFilterType::Pointer filter = SurfaceAdjacencyEffectCorrectionSchemeFilterType::New();

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();
  std::vector<const char *> wavelenghFiles;
  unsigned int              nbChannel = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  for (unsigned int i = 0; i < nbChannel; ++i)
    {
    wavelenghFiles.push_back(argv[i + 6]);
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
  paramAcqui->SetSolarZenithalAngle(solarZenithalAngle);
  paramAcqui->SetSolarAzimutalAngle(solarAzimutalAngle);
  paramAcqui->SetViewingZenithalAngle(viewingZenithalAngle);
  paramAcqui->SetViewingAzimutalAngle(viewingAzimutalAngle);
  paramAcqui->SetMonth(month);
  paramAcqui->SetDay(day);
  paramAtmo->SetAtmosphericPressure(atmosphericPressure);
  paramAtmo->SetWaterVaporAmount(waterVaporAmount);
  paramAtmo->SetOzoneAmount(ozoneAmount);
  paramAtmo->SetAerosolModel(aerosolModel);
  paramAtmo->SetAerosolOptical(aerosolOptical);

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

    std::string line;
    std::getline(fin, line);
    while (std::getline(fin, line) )
      {
      value = atof(line.c_str());
      vect.push_back(value);
      std::cout << "value " << value << std::endl;
      }
    std::cout << "vec size " << vect.size()<< std::endl;
    fin.close();
    functionValues->SetFilterFunctionValues(vect);
    functionValues->SetMinSpectralValue(minSpectralValue);
    functionValues->SetMaxSpectralValue(maxSpectralValue);
    functionValues->SetUserStep(val);

    paramAcqui->SetWavelengthSpectralBandWithIndex(j, functionValues);
    }


  AtmosphericRadiativeTermsPointerType  radiative = CorrectionParametersToRadiativeTermsType::Compute(paramAtmo,paramAcqui);

  filter->SetAtmosphericRadiativeTerms(radiative);
  filter->SetWindowRadius(atoi(argv[3]));
  filter->SetPixelSpacingInKilometers(static_cast<double>(atof(argv[4])));
  filter->SetZenithalViewingAngle(paramAcqui->GetViewingZenithalAngle());

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
