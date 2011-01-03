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

#include "otbLeafOpticalProperties.h"
#include "otbLeafParameters.h"
#include "otbCanopyParameters.h"
#include "otbCanopyParametersTo4SailCanopyBidirectionalReflectance.h"
#include "otbLeafParametersToProspectLeafOpticalProperties.h"

int otbSailTest(int argc, char * argv[])
{

   typedef otb::LeafParametersToProspectLeafOpticalProperties ProspectType;
   typedef otb::CanopyParametersTo4SailCanopyBidirectionalReflectance SailType;
   typedef otb::LeafParameters LeafParametersType;
   typedef otb::CanopyParameters CanopyParametersType;
   
   
   LeafParametersType::Pointer leafParams = LeafParametersType::New();
   CanopyParametersType::Pointer canopyParams = CanopyParametersType::New();
   ProspectType::Pointer prospect = ProspectType::New();
   SailType::Pointer sail = SailType::New();

   
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


   leafParams->SetCab(Cab);
   leafParams->SetCar(Car);
   leafParams->SetCBrown(CBrown);
   leafParams->SetCw(Cw);
   leafParams->SetCm(Cm);
   leafParams->SetN(N);
   
   canopyParams->SetLAI(LAI);
   canopyParams->SetAngl(Angl);
   canopyParams->SetPSoil(PSoil);
   canopyParams->SetSkyl(Skyl);
   canopyParams->SetHSpot(HSpot);
   canopyParams->SetTTS(TTS);
   canopyParams->SetTTO(TTO);
   canopyParams->SetPSI(PSI);
   
   prospect->SetInput(leafParams);

   sail->SetCanopyParameters(canopyParams);
   sail->SetLeafOpticalProperties(prospect->GetOutput());
   sail->GenerateData();
   sail->Update();
   
//    for(unsigned int i=0;i<prospect->GetOutput()->GetReflectance().size();i++)
//    {
//       std::cout<<"lambda : "<<prospect->GetOutput()->GetReflectance()[i].first;
//       std::cout<<" , refl : "<<prospect->GetOutput()->GetReflectance()[i].second;
//    }

   
   return EXIT_SUCCESS;
}
