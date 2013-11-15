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

#include "otbMacro.h"

#include "otbLeafParameters.h"
#include "otbSailModel.h"
#include "otbProspectModel.h"

#include <iostream>
#include <fstream>

int otbSailReflVTest(int argc, char * argv[])
{
   if(argc != 16)
   {
      std::cout<<"Wrong number of arguments !"<<std::endl;
      return EXIT_FAILURE;
   }

   double Cab=static_cast<double>(atof(argv[1]));
   double Car=static_cast<double>(atof(argv[2]));
   double CBrown=static_cast<double>(atof(argv[3]));
   double Cw=static_cast<double>(atof(argv[4]));
   double Cm=static_cast<double>(atof(argv[5]));
   double N=static_cast<double>(atof(argv[6]));
   double LAI=static_cast<double>(atof(argv[7]));
   double Angl=static_cast<double>(atof(argv[8]));
   double PSoil=static_cast<double>(atof(argv[9]));
   double Skyl=static_cast<double>(atof(argv[10]));
   double HSpot=static_cast<double>(atof(argv[11]));
   double TTS=static_cast<double>(atof(argv[12]));
   double TTO=static_cast<double>(atof(argv[13]));
   double PSI=static_cast<double>(atof(argv[14]));
   char * OutputName      = argv[15];


   typedef otb::ProspectModel ProspectType;
   typedef otb::SailModel SailType;
   typedef otb::LeafParameters LeafParametersType;


   LeafParametersType::Pointer leafParams = LeafParametersType::New();
   ProspectType::Pointer prospect = ProspectType::New();
   SailType::Pointer sail = SailType::New();

   leafParams->SetCab(Cab);
   leafParams->SetCar(Car);
   leafParams->SetCBrown(CBrown);
   leafParams->SetCw(Cw);
   leafParams->SetCm(Cm);
   leafParams->SetN(N);



   prospect->SetInput(leafParams);

   sail->SetLAI(LAI);
   sail->SetAngl(Angl);
   sail->SetPSoil(PSoil);
   sail->SetSkyl(Skyl);
   sail->SetHSpot(HSpot);
   sail->SetTTS(TTS);
   sail->SetTTO(TTO);
   sail->SetPSI(PSI);
   sail->SetReflectance(prospect->GetReflectance());
   sail->SetTransmittance(prospect->GetTransmittance());
   sail->Update();

   std::ofstream outputFile(OutputName, std::ios::out);
   for(unsigned int i=0; i<sail->GetViewingReflectance()->Size(); ++i)
   {
      outputFile<<sail->GetViewingReflectance()->GetResponse()[i].second<<std::endl;
   }


   return EXIT_SUCCESS;
}
