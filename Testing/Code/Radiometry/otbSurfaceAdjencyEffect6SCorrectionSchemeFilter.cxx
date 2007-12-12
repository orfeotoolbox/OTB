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
#include "itkExceptionObject.h"

#include "otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbAtmosphericRadiativeTerms.h"
#include<fstream>
#include<iostream>

int otbSurfaceAdjencyEffect6SCorrectionSchemeFilter(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];
  const char * paramFile = argv[5];

  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage<PixelType,Dimension> InputImageType;
  typedef otb::VectorImage<PixelType,Dimension> OutputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
     
  typedef otb::SurfaceAdjencyEffect6SCorrectionSchemeFilter<InputImageType, OutputImageType> SurfaceAdjencyEffect6SCorrectionSchemeFilterType;
  typedef SurfaceAdjencyEffect6SCorrectionSchemeFilterType::WeightingMatrixType            WeightingMatrixType;
  typedef SurfaceAdjencyEffect6SCorrectionSchemeFilterType::WeightingValuesContainerType   WeightingValuesContainerType;

  typedef otb::AtmosphericRadiativeTerms::DataVectorType                                   DataVectorType;
  typedef otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms                CorrectionParametersTo6SRadiativeTermsType;
  typedef otb::AtmosphericCorrectionParameters                                             CorrectionParametersType;
  typedef otb::AtmosphericRadiativeTerms                                                   RadiativeTermsType;
  typedef otb::FilterFunctionValues                                                        FilterFunctionValuesType;
  typedef CorrectionParametersType::AerosolModelType                                       AerosolModelType;
  typedef FilterFunctionValuesType::WavelenghtSpectralBandType                             ValueType;
  typedef FilterFunctionValuesType::ValuesVectorType                                       ValuesVectorType;
      



  RadiativeTermsType::Pointer                         radiative     = RadiativeTermsType::New();
  CorrectionParametersType::Pointer                   param         = CorrectionParametersType::New();
  CorrectionParametersTo6SRadiativeTermsType::Pointer corrToRadia   = CorrectionParametersTo6SRadiativeTermsType::New();
  FilterFunctionValuesType::Pointer                   functionValues;
  // Instantiating object
  SurfaceAdjencyEffect6SCorrectionSchemeFilterType::Pointer filter = SurfaceAdjencyEffect6SCorrectionSchemeFilterType::New();
     
  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->GenerateOutputInformation();
  std::vector<const char *> wavelenghFiles;
  unsigned int nbChannel = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  for(unsigned int i=0; i<nbChannel; i++)
    {
      wavelenghFiles.push_back( argv[i+6] );
    }
      
  ValueType val = 0.0025;
 
  // Correction parameters initialization
  double solarZenithalAngle(0.);
  double solarAzimutalAngle(0.);
  double viewingZenithalAngle(0.);
  double viewingAzimutalAngle(0.);
  unsigned int month(0);
  unsigned int day(0);
  double atmosphericPressure(0.);
  double waterVaporAmount(0.);
  double ozoneAmount(0.);
  double aerosolOptical(0.);
  

  std::ifstream fin;
  fin.open(paramFile);
  //Read input file parameters
  fin >> solarZenithalAngle;//asol;
  fin >> solarAzimutalAngle;//phi0;
  fin >> viewingZenithalAngle;//avis;
  fin >> viewingAzimutalAngle;//phiv;
  fin >> month;//month;
  fin >> day;//jday;
  fin >> atmosphericPressure;//pressure;
  fin >> waterVaporAmount;//uw;
  fin >> ozoneAmount;//uo3;
  unsigned int aer(0);
  fin >> aer;//iaer;
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(aer);
  fin >> aerosolOptical;//taer55;
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

  // Output verifiacation
  std::ofstream fout;
  fout.open(argv[10]);

  ValuesVectorType vect;
  for(unsigned int j=0; j<nbChannel; j++)
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
      fin >> minSpectralValue;//wlinf;
      fin >> maxSpectralValue;//wlsup; 
      fout << "CHANNEL "<<j<<"inf: "<<minSpectralValue<<"    max: "<<maxSpectralValue<<std::endl;
      while (!fin.eof() && fin.good())
	{
	  fin >> value;
	  fout <<value<<std::endl;
	  vect.push_back(value);
	}

      fin.close();
      functionValues->SetFilterFunctionValues(vect);
      functionValues->SetMinSpectralValue(minSpectralValue);
      functionValues->SetMaxSpectralValue(maxSpectralValue);
      functionValues->SetUserStep( val );

      param->SetWavelenghtSpectralBandWithIndex(j, functionValues);
    }
 
  corrToRadia->SetInput( param );

  filter->SetAtmosphericRadiativeTerms(corrToRadia->GetOutput());
  filter->SetWindowRadius(atoi(argv[3]));
  filter->SetPixelSpacingInKilometers(static_cast<double>(atof(argv[4])));
  filter->SetZenithalViewingAngle(param->GetViewingZenithalAngle());

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();


 
 fout <<" ---------------------------------------------------------"<<std::endl;
        fout << "Inputs values:"<<std::setprecision(10)<<std::endl;
        fout << "   ----->  SolarZenithalAngle :                        "<<solarZenithalAngle<<std::endl;
        fout << "   ----->  SolarAzimutalAngle :                        "<<solarAzimutalAngle<<std::endl;
        fout << "   ----->  ViewingZenithalAngle :                      "<<viewingZenithalAngle<<std::endl;
        fout << "   ----->  ViewingAzimutalAngle :                      "<<viewingAzimutalAngle<<std::endl;
        fout << "   ----->  Month :                                     "<<month<<std::endl;
        fout << "   ----->  Day :                                       "<<day<<std::endl;
        fout << "   ----->  AtmosphericPressure :                       "<<atmosphericPressure<<std::endl;
        fout << "   ----->  WaterVaporAmount :                          "<<waterVaporAmount<<std::endl;
        fout << "   ----->  OzoneAmount :                               "<<ozoneAmount<<std::endl;
        fout << "   ----->  AerosolModel :                              "<<aer<<std::endl;
        fout << "   ----->  AerosolOptical :                            "<<aerosolOptical<<std::endl;
        fout << "   ----->  UserStep :                                  "<<functionValues->GetUserStep()<<std::endl;
        fout <<" ---------------------------------------------------------"<<std::endl;
        fout << "Outputs values:"<<std::endl;
	for(unsigned  int k=0; k<nbChannel;k++)
	  {  fout << "CHANNEL "<<k<<std::endl;
        fout << "   ----->  atmospheric reflectance :                   "<<corrToRadia->GetOutput()->GetIntrinsicAtmosphericReflectance(k)<<std::endl;
        fout << "   ----->  atmospheric spherical albedo :              "<<corrToRadia->GetOutput()->GetSphericalAlbedo(k)<<std::endl;
        fout << "   ----->  total gaseous transmission :                "<<corrToRadia->GetOutput()->GetTotalGaseousTransmission(k)<<std::endl;
        fout << "   ----->  downward transmittance :                    "<<corrToRadia->GetOutput()->GetDownwardTransmittance(k)<<std::endl;
        fout << "   ----->  upward transmittance :                      "<<corrToRadia->GetOutput()->GetUpwardTransmittance(k)<<std::endl;
        fout << "   ----->  upward diffuse transmittance :              "<<corrToRadia->GetOutput()->GetUpwardDiffuseTransmittance(k)<<std::endl;
        fout << "   ----->  upward direct transmittance :               "<<corrToRadia->GetOutput()->GetUpwardDirectTransmittance(k)<<std::endl;
        fout << "   ----->  upward diffuse transmittance for rayleigh : "<<corrToRadia->GetOutput()->GetUpwardDiffuseTransmittanceForRayleigh(k)<<std::endl;
        fout << "   ----->  upward diffuse transmittance for aerosols : "<<corrToRadia->GetOutput()->GetUpwardDiffuseTransmittanceForAerosol(k)<<std::endl;
        fout <<" ---------------------------------------------------------"<<std::endl;
        fout<<"Input wavelenght band values ["<<functionValues->GetFilterFunctionValues().size()<<"]:"<<std::endl;
  	 
	fout << "Function Values:"<<std::endl;
        for (unsigned int i=0; i<functionValues->GetFilterFunctionValues().size(); i++)
	{  
	  fout << "    "<<functionValues->GetFilterFunctionValues()[i] <<std::endl;
	}

        fout <<" ---------------------------------------------------------"<<std::endl;
        fout<<"Output wavelenght band values 6S ["<<functionValues->GetFilterFunctionValues6S().size()<<"]:"<<std::endl;
        for (unsigned int i=0; i<functionValues->GetFilterFunctionValues6S().size(); i++)
	{
	  fout<< "    "<<functionValues->GetFilterFunctionValues6S()[i] <<std::endl;
	}
        fout<<std::endl;
	  }
        fout.close();


  return EXIT_SUCCESS;
}
