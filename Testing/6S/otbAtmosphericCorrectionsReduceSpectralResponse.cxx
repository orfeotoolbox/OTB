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
#include "otbMacro.h"

#include "otbAtmosphericCorrectionsReduceSpectralResponse.h"
#include "otbSatelliteRSR.h"
#include "otbSpectralResponse.h"

#include "otbReduceSpectralResponseClassifierRAndNIR.h"

int main(int argc, char * argv[])
{
  
  typedef otb::SpectralResponse< double,double>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;
  
  typedef otb::SatelliteRSR< double,double>  SatRSRType;
  typedef SatRSRType::Pointer  SatRSRPointerType;
  
  typedef otb::AtmosphericCorrectionsReduceSpectralResponse < ResponseType,SatRSRType>  ReduceResponseType;
  typedef ReduceResponseType::Pointer  ReduceResponseTypePointerType;
  
  
  
  typedef ReduceResponseType::AtmosphericCorrectionParametersType  AtmosphericCorrectionParametersType;
  
  if ( argc!= 17 )
  {
    std::cout << argv[0] << std::endl << "\t" << "<RSR_fileSRname>"  << "\t" << "<Gabarit_SAT_fileSRname>" << "\t" << "<nbBands>" << "\t" << "<day>"  << "\t" << "<month>"  << "\t" << "<zenithSolarAngle>"  << "\t" << "<azimutSolarAngle>"  << "\t" << "<viewingZenitalAngle>"  << "\t" << "<viewingAzimutalAngle>"  << "\t" << "<atmoPressure>"  << "\t" << "<waterVaporAmount>"  << "\t" << "<ozoneAmount>"  << "\t" << "<aerosolModelValue>"  << "\t" << "<aerosolOptical>"  << "\t" << "<rband>"<< "\t" << "<nirband>"<<  std::endl ;
    return EXIT_FAILURE;
  }
  
  
  /*
  27.3    #elevation et azimuth solaire
  4       #day
  12      #month
  # AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms parameters
  152.7 #solar azimutal angle
  2.5 #viewing zenithal angle
  -77.0 #viewing azimutal angle
  1013. #atmo pressure
  2.48134 #water vapour amount
  0.34400 #ozone amount
  1 #aerosol model type 
  0.199854 #aerosol optical
  */
  
  //Instantiation
  ResponsePointerType  myResponse=ResponseType::New();
  //Load fileSR into vector
  const std::string fileSR(argv[1]);
  const std::string fileSatG(argv[2]);
  unsigned int nbBand = atoi(argv[3]);
  
  myResponse->Load(fileSR);
  
//   std::cout << "Response before "<< myResponse << std::endl; 
  
  SatRSRPointerType  myRSR=SatRSRType::New();
  /** Set the satelite number of bands */
  myRSR->SetNbBands(nbBand);
  /** Load the satelite response file*/
  myRSR->Load(fileSatG);
  
  //Process 6S direct chain
  AtmosphericCorrectionParametersType::Pointer dataAtmosphericCorrectionParameters = AtmosphericCorrectionParametersType::New();
  
  //Set inputs of the atmo parameters
  const int day(atoi(argv[4]));
  const int month(atoi(argv[5]));
  const double zenithSolarAngle(static_cast<double>(atof(argv[6])));
  const double azimutSolarAngle(static_cast<double>(atof(argv[7])));
  const double viewingZenitalAngle(static_cast<double>(atof(argv[8])));
  const double viewingAzimutalAngle(static_cast<double>(atof(argv[9])));
  const double atmoPressure(static_cast<double>(atof(argv[10])));
  const double waterVaporAmount(static_cast<double>(atof(argv[11])));
  const double ozoneAmount(static_cast<double>(atof(argv[12])));
  const int aerosolModelValue(::atoi(argv[13]));
  const double aerosolOptical( static_cast<double>(atof(argv[14])) );
  // Set parameters
  dataAtmosphericCorrectionParameters->SetSolarZenithalAngle(zenithSolarAngle);
  dataAtmosphericCorrectionParameters->SetSolarAzimutalAngle(azimutSolarAngle);
  dataAtmosphericCorrectionParameters->SetViewingZenithalAngle(viewingZenitalAngle);
  dataAtmosphericCorrectionParameters->SetViewingAzimutalAngle(viewingAzimutalAngle);
  dataAtmosphericCorrectionParameters->SetMonth(month);
  dataAtmosphericCorrectionParameters->SetDay(day);
  dataAtmosphericCorrectionParameters->SetAtmosphericPressure(atmoPressure);
  dataAtmosphericCorrectionParameters->SetWaterVaporAmount(waterVaporAmount);
  dataAtmosphericCorrectionParameters->SetOzoneAmount(ozoneAmount);
  
  typedef AtmosphericCorrectionParametersType::AerosolModelType
      AerosolModelType;
  
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(aerosolModelValue);
  dataAtmosphericCorrectionParameters->SetAerosolModel(aerosolModel);
  dataAtmosphericCorrectionParameters->SetAerosolOptical(aerosolOptical);
  
  ReduceResponseTypePointerType  myReduceResponse=ReduceResponseType::New();
  //Instantiation
  //ResponsePointerType  myResponse=ResponseType::New();
  /** Load the satelite response in the simulator */
  myReduceResponse->SetInputSatRSR(myRSR);
  /** Load the spectral response of the object in the simulator*/
  myReduceResponse->SetInputSpectralResponse(myResponse);
  
  myReduceResponse->SetDataAtmosphericCorrectionParameters(dataAtmosphericCorrectionParameters);
  std::cout << "Begin 6S!!" << std::endl; 
  
  /*
  for (unsigned int i=0;i < myReduceResponse->m_InputSatRSR->GetNbBands();++i)
  {
    myReduceResponse->Process6S(i);
  }
  */
  myReduceResponse->Process6S();
  
  //std::cout << "Response after "<< myReduceResponse << std::endl; 
//   std::cout << "6S response " << myReduceResponse->GetInputSpectralResponse()->GetResponse() << std::endl;
  //
  myReduceResponse->CalculateResponse();
  
  std::cout << "Reduce response " << myReduceResponse << std::endl;
  
  typedef otb::Functor::NDVI<double,double,double >               TFunctionType;
  typedef otb::ReduceSpectralResponseClassifierRAndNIR <ReduceResponseType,TFunctionType> ReduceSpectralResponseClassifierRAndNIRType;
  typedef ReduceSpectralResponseClassifierRAndNIRType::Pointer  ReduceSpectralResponseClassifierRAndNIRPointerType;
  
  ReduceSpectralResponseClassifierRAndNIRPointerType  myIndice=ReduceSpectralResponseClassifierRAndNIRType::New();
  
  myIndice->SetInputReduceSpectralResponse(myReduceResponse);
  
  //FunctorType
  //myIndice->SetFunctor(myReduceResponse);
  const int rband(::atoi(argv[15]));
  const int nirband(::atoi(argv[16]));
  
  myIndice->SetRBandNumber(rband);
  myIndice->SetNIRBandNumber(nirband);
  
  std::cout << "NDVI value " << (*myIndice)() << std::endl;
             
  
  
  return EXIT_SUCCESS;
}
