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

#include "otbProspectModel.h"
#include "otbLeafParameters.h"

#include <iostream>
#include <fstream>

int otbProspectReflTest(int argc, char * argv[])
{

   if(argc != 8)
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
   char * OutputName      = argv[7];

   //typedef
   typedef otb::ProspectModel PropectType;
   typedef otb::LeafParameters LeafParametersType;

   //instantiation
   LeafParametersType::Pointer leafParams = LeafParametersType::New();
   PropectType::Pointer prospect = PropectType::New();

   leafParams->SetCab(Cab);
   leafParams->SetCar(Car);
   leafParams->SetCBrown(CBrown);
   leafParams->SetCw(Cw);
   leafParams->SetCm(Cm);
   leafParams->SetN(N);

   prospect->SetInput(leafParams);
   prospect->Update();

   std::ofstream outputFile(OutputName, std::ios::out);
   for(unsigned int i=0; i<prospect->GetReflectance()->Size(); ++i)
   {
      outputFile<<prospect->GetReflectance()->GetResponse()[i].second<<std::endl;
   }


   return EXIT_SUCCESS;
}
