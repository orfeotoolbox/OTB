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

#include "otbSpectralResponse.h"
#include "otbProSail.h"
#include "otbProSailParameters.h"

int otbProSailNew(int argc, char * argv[])
{
   typedef otb::SpectralResponse< double,double>  SpectralResponseType;
   typedef otb::ProSail<SpectralResponseType> ProSailType;
   typedef ProSailType::LRTType LRTType;
   typedef otb::ProSailParameters ProSailParametersType;
   
   
   ProSailParametersType::Pointer prosailParams = ProSailParametersType::New();
   ProSailType::Pointer prosail = ProSailType::New();
   std::cout<<prosail<<std::endl;
   
   double Cab=30.0;
   double Car=10.0; 
   double CBrown=0.0; 
   double Cw=0.015; 
   double Cm=0.009;
   double N=1.2;
   double LAI=2.0;
   double Angl=50;
   double PSoil=1;
   double Skyl=70;
   double HSpot=0.2;
   double TTS=30;
   double TTO=0;
   double PSI=0;
   LRTType LRT(3);
   std::vector<double> result(4);

   prosailParams->SetCab(Cab);
   prosailParams->SetCar(Car);
   prosailParams->SetCBrown(CBrown);
   prosailParams->SetCw(Cw);
   prosailParams->SetCm(Cm);
   prosailParams->SetN(N);
   prosailParams->SetLAI(LAI);
   prosailParams->SetAngl(Angl);
   prosailParams->SetPSoil(PSoil);
   prosailParams->SetSkyl(Skyl);
   prosailParams->SetHSpot(HSpot);
   prosailParams->SetTTS(TTS);
   prosailParams->SetTTO(TTO);
   prosailParams->SetPSI(PSI);

   
   
   prosail->SetProSailParameters(prosailParams);
   prosail->Compute();
//    prosail->Volscatt(TTS,TTO,PSI,2.5,result);
//    prosail->Prospect5B(LRT);

   
   std::cout<<"size  : "<<prosail->GetResh()->GetResponse().size()<<std::endl;
   for(unsigned int i=0;i<prosail->GetResh()->GetResponse().size();i++)
   {
      std::cout<<"lambda : "<<prosail->GetResv()->GetResponse()[i].first<<" Resh : "<<prosail->GetResh()->GetResponse()[i].second<<" Resv : "<<prosail->GetResv()->GetResponse()[i].second<<std::endl;
   }

   
   return EXIT_SUCCESS;
}
