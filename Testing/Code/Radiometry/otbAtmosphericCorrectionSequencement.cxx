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


#include "otbImageToLuminanceImageFilter.h"
#include "otbLuminanceToReflectanceImageFilter.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbSurfaceAdjacencyEffectCorrectionSchemeFilter.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include <string>

int otbAtmosphericCorrectionSequencementTest(int argc, char *argv[])
{
  if (argc != 19)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << "    inputImage , outputImage,"  << std::endl;
    std::cerr << "    alphaBetaFileName , solarZenithalAngle , day, month , solarIlluminationFileName,"  << std::endl;
    std::cerr << "    solarAzimutalAngle , viewingZenithalAngle , viewingAzimutalAngle,"  << std::endl;
    std::cerr << "    atmosphericPressure , waterVaporAmount , ozoneAmount , aerosolModel , AerosolOptical,"  <<
    std::endl;
    std::cerr <<
    "    wavelengthSpectralBandFileName , adjacencyEffect6SCorrectionWindowRadius, pixelSpacingInKilometers" <<
    std::endl;
    std::cerr << std::endl;
    return 1;
    }

  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer reader  = ReaderType::New();
  reader->SetFileName(argv[1]);

  reader->UpdateOutputInformation();

  unsigned int nbOfComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();

//-------------------------------

  typedef otb::ImageToLuminanceImageFilter<ImageType, ImageType>
  ImageToLuminanceImageFilterType;
  typedef ImageToLuminanceImageFilterType::VectorType VectorType;

  VectorType alpha(nbOfComponent);
  VectorType beta(nbOfComponent);
  alpha.Fill(0);
  beta.Fill(0);

  std::ifstream fin;
  fin.open(argv[3]);
  double dalpha(0.), dbeta(0.);
  for (unsigned int i = 0; i < nbOfComponent; ++i)
    {
    fin >> dalpha;
    fin >> dbeta;
    alpha[i] = dalpha;
    beta[i] = dbeta;
    }
  fin.close();

  ImageToLuminanceImageFilterType::Pointer filterImageToLuminance = ImageToLuminanceImageFilterType::New();
  filterImageToLuminance->SetAlpha(alpha);
  filterImageToLuminance->SetBeta(beta);
  filterImageToLuminance->SetInput(reader->GetOutput());

//-------------------------------
  typedef otb::LuminanceToReflectanceImageFilter<ImageType, ImageType>
  LuminanceToReflectanceImageFilterType;

  typedef LuminanceToReflectanceImageFilterType::VectorType VectorType;

  VectorType solarIllumination(nbOfComponent);
  solarIllumination.Fill(0);

  fin.open(argv[7]);
  double dsolarIllumination(0.);
  for (unsigned int i = 0; i < nbOfComponent; ++i)
    {
    fin >> dsolarIllumination;
    solarIllumination[i] = dsolarIllumination;
    }
  fin.close();

  LuminanceToReflectanceImageFilterType::Pointer filterLuminanceToReflectance =
    LuminanceToReflectanceImageFilterType::New();
  const int day(atoi(argv[5]));
  const int month(atoi(argv[6]));

  filterLuminanceToReflectance->SetZenithalSolarAngle(static_cast<double>(atof(argv[4])));
  filterLuminanceToReflectance->SetDay(day);
  filterLuminanceToReflectance->SetMonth(month);
  filterLuminanceToReflectance->SetSolarIllumination(solarIllumination);
  filterLuminanceToReflectance->SetInput(filterImageToLuminance->GetOutput());

//-------------------------------
  /*typedef otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms  RadiometryCorrectionParametersToRadiativeTermsType;
  typedef otb::AtmosphericCorrectionParameters                            AtmosphericCorrectionParametersType;
  typedef otb::AtmosphericRadiativeTerms                                  AtmosphericRadiativeTermsType;
  typedef AtmosphericCorrectionParametersType::AerosolModelType           AerosolModelType;

  typedef otb::FilterFunctionValues                                       FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType                      ValuesVectorType;

  AtmosphericCorrectionParametersType::Pointer dataAtmosphericCorrectionParameters =
    AtmosphericCorrectionParametersType::New();
  AtmosphericRadiativeTermsType::Pointer dataAtmosphericRadiativeTerms = AtmosphericRadiativeTermsType::New(); */

  typedef otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms     CorrectionParametersToRadiativeTermsType;

  typedef otb::AtmosphericCorrectionParameters                              AtmoCorrectionParametersType;
  typedef AtmoCorrectionParametersType::Pointer                   AtmoCorrectionParametersPointerType;
  typedef AtmoCorrectionParametersType::AerosolModelType                    AerosolModelType;

  typedef otb::ImageMetadataCorrectionParameters                            AcquiCorrectionParametersType;
  typedef AcquiCorrectionParametersType::Pointer                  AcquiCorrectionParametersPointerType;

  typedef otb::AtmosphericRadiativeTerms                                    AtmosphericRadiativeTermsType;
  typedef AtmosphericRadiativeTermsType::Pointer                  AtmosphericRadiativeTermsPointerType;
  typedef otb::AtmosphericRadiativeTerms::DataVectorType DataVectorType;

  typedef otb::FilterFunctionValues                                         FilterFunctionValuesType;
  typedef FilterFunctionValuesType::WavelengthSpectralBandType              ValueType;                //float
  typedef FilterFunctionValuesType::ValuesVectorType                        ValuesVectorType;         //std::vector<float>

  typedef AcquiCorrectionParametersType::WavelengthSpectralBandVectorType        WavelengthSpectralBandVectorType;

  AcquiCorrectionParametersPointerType paramAcqui = AcquiCorrectionParametersType::New();
  AtmoCorrectionParametersPointerType  paramAtmo = AtmoCorrectionParametersType::New();
  FilterFunctionValuesType::Pointer                   functionValues;



  double minSpectralValue(0.);
  double maxSpectralValue(0.);
  double userStep(0.);
  double value(0.);

  unsigned int     nbBands(0);
  unsigned int     nbValuesPerBand(0);
  std::string      sString;
  ValuesVectorType valuesVector;

  fin.open(argv[16]);
  fin >> nbBands;
  for (unsigned int i = 0; i < nbBands; ++i)
    {
    valuesVector.clear();
    fin >> sString;
    fin >> minSpectralValue;
    fin >> maxSpectralValue;
    fin >> userStep;
    fin >> nbValuesPerBand;
    for (unsigned int j = 0; j < nbValuesPerBand; ++j)
      {
      fin >> value;
      valuesVector.push_back(value);
      }
    FilterFunctionValuesType::Pointer functionValues = FilterFunctionValuesType::New();
    functionValues->SetFilterFunctionValues(valuesVector);
    functionValues->SetMinSpectralValue(minSpectralValue);
    functionValues->SetMaxSpectralValue(maxSpectralValue);
    functionValues->SetUserStep(userStep);
    paramAcqui->SetWavelengthSpectralBandWithIndex(i, functionValues);
    }

  fin.close();

  // Set parameters
  /*dataAtmosphericCorrectionParameters->SetSolarZenithalAngle(filterLuminanceToReflectance->GetZenithalSolarAngle());
  dataAtmosphericCorrectionParameters->SetSolarAzimutalAngle(static_cast<double>(atof(argv[8])));
  dataAtmosphericCorrectionParameters->SetViewingZenithalAngle(static_cast<double>(atof(argv[9])));
  dataAtmosphericCorrectionParameters->SetViewingAzimutalAngle(static_cast<double>(atof(argv[10])));
  dataAtmosphericCorrectionParameters->SetMonth(month);
  dataAtmosphericCorrectionParameters->SetDay(day);
  dataAtmosphericCorrectionParameters->SetAtmosphericPressure(static_cast<double>(atof(argv[11])));
  dataAtmosphericCorrectionParameters->SetWaterVaporAmount(static_cast<double>(atof(argv[12])));
  dataAtmosphericCorrectionParameters->SetOzoneAmount(static_cast<double>(atof(argv[13])));
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(::atoi(argv[14]));
  dataAtmosphericCorrectionParameters->SetAerosolModel(aerosolModel);
  dataAtmosphericCorrectionParameters->SetAerosolOptical(static_cast<double>(atof(argv[15])));

  RadiometryCorrectionParametersToRadiativeTermsType::Pointer
    filterAtmosphericCorrectionParametersTo6SRadiativeTerms =
    RadiometryCorrectionParametersToRadiativeTermsType::New();
  filterAtmosphericCorrectionParametersTo6SRadiativeTerms->SetInput(dataAtmosphericCorrectionParameters);
  filterAtmosphericCorrectionParametersTo6SRadiativeTerms->Update(); */

  paramAcqui->SetSolarZenithalAngle(filterLuminanceToReflectance->GetZenithalSolarAngle());
  paramAcqui->SetSolarAzimutalAngle(static_cast<double>(atof(argv[8])));
  paramAcqui->SetViewingZenithalAngle(static_cast<double>(atof(argv[9])));
  paramAcqui->SetViewingAzimutalAngle(static_cast<double>(atof(argv[10])));
  paramAcqui->SetMonth(month);
  paramAcqui->SetDay(day);
  paramAtmo->SetAtmosphericPressure(static_cast<double>(atof(argv[11])));
  paramAtmo->SetWaterVaporAmount(static_cast<double>(atof(argv[12])));
  paramAtmo->SetOzoneAmount(static_cast<double>(atof(argv[13])));
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(::atoi(argv[14]));
  paramAtmo->SetAerosolModel(aerosolModel);
  paramAtmo->SetAerosolOptical(static_cast<double>(atof(argv[15])));

  AtmosphericRadiativeTermsPointerType  radiative = CorrectionParametersToRadiativeTermsType::Compute(paramAtmo,paramAcqui);

//-------------------------------

  typedef otb::ReflectanceToSurfaceReflectanceImageFilter<ImageType,
      ImageType> ReflectanceToSurfaceReflectanceImageFilterType;
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer filterReflectanceToSurfaceReflectanceImageFilter =
    ReflectanceToSurfaceReflectanceImageFilterType::New();

  filterReflectanceToSurfaceReflectanceImageFilter->SetAtmosphericRadiativeTerms(radiative);
  filterReflectanceToSurfaceReflectanceImageFilter->SetInput(filterLuminanceToReflectance->GetOutput());

//-------------------------------
  typedef otb::SurfaceAdjacencyEffectCorrectionSchemeFilter<ImageType,
      ImageType> SurfaceAdjacencyEffectCorrectionSchemeFilterType;
  SurfaceAdjacencyEffectCorrectionSchemeFilterType::Pointer filterSurfaceAdjacencyEffectCorrectionSchemeFilter =
    SurfaceAdjacencyEffectCorrectionSchemeFilterType::New();

  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->SetAtmosphericRadiativeTerms(radiative);
  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->SetWindowRadius(atoi(argv[17]));
  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->SetPixelSpacingInKilometers(static_cast<double>(atof(argv[18])));
  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->SetZenithalViewingAngle(paramAcqui->GetViewingZenithalAngle());
  filterSurfaceAdjacencyEffectCorrectionSchemeFilter->SetInput(filterReflectanceToSurfaceReflectanceImageFilter->GetOutput());

//-------------------------------
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(filterSurfaceAdjacencyEffectCorrectionSchemeFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
