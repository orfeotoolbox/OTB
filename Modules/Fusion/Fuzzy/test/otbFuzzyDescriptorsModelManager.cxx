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


#include "otbFuzzyDescriptorsModelManager.h"
#include <iostream>
#include <stdlib.h>
#include "itkMacro.h"

int otbFuzzyDescriptorsModelManagerNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  otb::FuzzyDescriptorsModelManager mng;
  return EXIT_SUCCESS;
}


int otbFuzzyDescriptorsModelManagerTest(int itkNotUsed(argc), char* argv[])
{
  otb::FuzzyDescriptorsModelManager::DescriptorsModelType model;
  otb::FuzzyDescriptorsModelManager::ParameterType        param;
  otb::FuzzyDescriptorsModelManager::PairType             couple;

  couple.first = "desc1";
  param.push_back(0);
  param.push_back(1);
  couple.second = param;
  otb::FuzzyDescriptorsModelManager::AddDescriptor(couple, model);

  param.clear();
  param.push_back(9);
  param.push_back(8);
  param.push_back(7);
  param.push_back(6);

  otb::FuzzyDescriptorsModelManager::AddDescriptor("desc2", param, model);

  otb::FuzzyDescriptorsModelManager::Save(argv[1], model);

  otb::FuzzyDescriptorsModelManager::DescriptorsModelType modelRead;
  modelRead =  otb::FuzzyDescriptorsModelManager::Read(argv[1]);

  otb::FuzzyDescriptorsModelManager::DescriptorListType list;
  list = otb::FuzzyDescriptorsModelManager::GetDescriptorList(model);

  if(modelRead != model)
    {
      std::cout<<"Written model and read model from the written one differs."<<std::endl;
      std::cout<<"Input is:"<<std::endl;
      otb::FuzzyDescriptorsModelManager::Print(model);
      std::cout<<"Output is:"<<std::endl;
      otb::FuzzyDescriptorsModelManager::Print(modelRead);
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
