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

#include "otbLeafParametersToProspectLeafOpticalProperties.h"
#include "otbLeafParameters.h"

#include <iostream>
#include <fstream>

int otbProspectTransTest(int argc, char * argv[])
{

   char * OutputName      = argv[1];
   
   typedef otb::LeafParametersToProspectLeafOpticalProperties PropectType;
   typedef otb::LeafParameters LeafParametersType;
   
   
   LeafParametersType::Pointer leafParams = LeafParametersType::New();
   PropectType::Pointer prospect = PropectType::New();

   
   double Cab=30.0;
   double Car=10.0; 
   double CBrown=0.0; 
   double Cw=0.015; 
   double Cm=0.009;
   double N=1.2;


   leafParams->SetCab(Cab);
   leafParams->SetCar(Car);
   leafParams->SetCBrown(CBrown);
   leafParams->SetCw(Cw);
   leafParams->SetCm(Cm);
   leafParams->SetN(N);
   
   prospect->SetInput(leafParams);
   prospect->Update();
   
   std::ofstream outputFile(OutputName,std::ios::out);
   for(unsigned int i=0;i<prospect->GetOutput()->GetTransmittance().size();i++)
   {
      outputFile<<prospect->GetOutput()->GetTransmittance()[i].second<<std::endl;
   }

   
   return EXIT_SUCCESS;
}
