/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <iostream>

#include <otbConfigure.h>
#include <otbMachineLearningModel.h>

typedef otb::MachineLearningModel<float,short>         MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       InputValueType;
typedef MachineLearningModelType::InputSampleType      InputSampleType;
typedef MachineLearningModelType::InputListSampleType  InputListSampleType;
typedef MachineLearningModelType::TargetValueType      TargetValueType;
typedef MachineLearningModelType::TargetSampleType     TargetSampleType;
typedef MachineLearningModelType::TargetListSampleType TargetListSampleType;


#ifdef OTB_USE_LIBSVM
#include "otbLibSVMMachineLearningModel.h"

int otbLibSVMMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
  
  typedef otb::LibSVMMachineLearningModel<InputValueType, TargetValueType> SVMType;
  SVMType::Pointer classifier = SVMType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::LibSVMMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}
#endif


#ifdef OTB_USE_OPENCV
#include "otbSVMMachineLearningModel.h"
#include "otbRandomForestsMachineLearningModel.h"
#include "otbBoostMachineLearningModel.h"
#include "otbNeuralNetworkMachineLearningModel.h"
#include "otbNormalBayesMachineLearningModel.h"
#include "otbDecisionTreeMachineLearningModel.h"
#include "otbKNearestNeighborsMachineLearningModel.h"

int otbSVMMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
  
  typedef otb::SVMMachineLearningModel<InputValueType, TargetValueType> SVMType;
  SVMType::Pointer classifier = SVMType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::SVMMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbRandomForestsMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
  
  typedef otb::RandomForestsMachineLearningModel<InputValueType, TargetValueType> RFType;
  RFType::Pointer classifier = RFType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::RandomForestsMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbBoostMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
  
  typedef otb::BoostMachineLearningModel<InputValueType, TargetValueType> BoostType;
  BoostType::Pointer classifier = BoostType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::BoostMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbNeuralNetworkMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
  
  typedef otb::NeuralNetworkMachineLearningModel<InputValueType, TargetValueType> ANNType;
  ANNType::Pointer classifier = ANNType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::NeuralNetworkMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbNormalBayesMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
  
  typedef otb::NormalBayesMachineLearningModel<InputValueType, TargetValueType> NormalBayesType;
  NormalBayesType::Pointer classifier = NormalBayesType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::NormalBayesMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbDecisionTreeMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
  
  typedef otb::DecisionTreeMachineLearningModel<InputValueType, TargetValueType> DecisionTreeType;
  DecisionTreeType::Pointer classifier = DecisionTreeType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::DecisionTreeMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbKNNMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
  
  typedef otb::KNearestNeighborsMachineLearningModel<InputValueType, TargetValueType> KNNType;
  KNNType::Pointer classifier = KNNType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::KNearestNeighborsMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
#endif


#ifdef OTB_USE_SHARK
#include "otbSharkRandomForestsMachineLearningModel.h"

int otbSharkRFMachineLearningModelCanRead(int argc, char* argv[])
{
  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0]
              << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for (int i = 1; i < argc; ++i)
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename(argv[1]);
  typedef otb::SharkRandomForestsMachineLearningModel<InputValueType, TargetValueType> RFType;
  RFType::Pointer classifier = RFType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Error otb::SharkRandomForestsMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


#endif
