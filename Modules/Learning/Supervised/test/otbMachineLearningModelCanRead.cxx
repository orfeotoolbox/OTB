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

#include <iostream>

#include <otbMachineLearningModel.h>

#ifdef OTB_USE_LIBSVM
#include "otbLibSVMMachineLearningModel.h"
#endif
#ifdef OTB_USE_OPENCV
#include "otbSVMMachineLearningModel.h"
#include "otbRandomForestsMachineLearningModel.h"
#include "otbBoostMachineLearningModel.h"
#include "otbNeuralNetworkMachineLearningModel.h"
#include "otbNormalBayesMachineLearningModel.h"
#include "otbDecisionTreeMachineLearningModel.h"
#include "otbGradientBoostedTreeMachineLearningModel.h"
#include "otbKNearestNeighborsMachineLearningModel.h"
#endif
#include "otbSharkRandomForestsMachineLearningModel.h"

typedef otb::MachineLearningModel<float,short>         MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       InputValueType;
typedef MachineLearningModelType::InputSampleType      InputSampleType;
typedef MachineLearningModelType::InputListSampleType  InputListSampleType;
typedef MachineLearningModelType::TargetValueType      TargetValueType;
typedef MachineLearningModelType::TargetSampleType     TargetSampleType;
typedef MachineLearningModelType::TargetListSampleType TargetListSampleType;

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
  
  #ifdef OTB_USE_LIBSVM
  typedef otb::LibSVMMachineLearningModel<InputValueType, TargetValueType> SVMType;
  SVMType::Pointer classifier = SVMType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::LibSVMMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  #endif

  return EXIT_SUCCESS;
}

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
  
  #ifdef OTB_USE_OPENCV
  typedef otb::SVMMachineLearningModel<InputValueType, TargetValueType> SVMType;
  SVMType::Pointer classifier = SVMType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::SVMMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  #endif

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
  
  #ifdef OTB_USE_OPENCV
  typedef otb::RandomForestsMachineLearningModel<InputValueType, TargetValueType> RFType;
  RFType::Pointer classifier = RFType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::RandomForestsMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  #endif

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
  
  #ifdef OTB_USE_OPENCV
  typedef otb::BoostMachineLearningModel<InputValueType, TargetValueType> BoostType;
  BoostType::Pointer classifier = BoostType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::BoostMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  #endif

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
  
  #ifdef OTB_USE_OPENCV
  typedef otb::NeuralNetworkMachineLearningModel<InputValueType, TargetValueType> ANNType;
  ANNType::Pointer classifier = ANNType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::NeuralNetworkMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  #endif

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
  
  #ifdef OTB_USE_OPENCV
  typedef otb::NormalBayesMachineLearningModel<InputValueType, TargetValueType> NormalBayesType;
  NormalBayesType::Pointer classifier = NormalBayesType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::NormalBayesMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  #endif

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
  
  #ifdef OTB_USE_OPENCV
  typedef otb::DecisionTreeMachineLearningModel<InputValueType, TargetValueType> DecisionTreeType;
  DecisionTreeType::Pointer classifier = DecisionTreeType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::DecisionTreeMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  #endif

  return EXIT_SUCCESS;
}

int otbGradientBoostedTreeMachineLearningModelCanRead(int argc, char* argv[])
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
  
  #ifdef OTB_USE_OPENCV
  typedef otb::GradientBoostedTreeMachineLearningModel<InputValueType, TargetValueType> GBTreeType;
  GBTreeType::Pointer classifier = GBTreeType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::GradientBoostedTreeMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  #endif

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
  
  #ifdef OTB_USE_OPENCV
  typedef otb::KNearestNeighborsMachineLearningModel<InputValueType, TargetValueType> KNNType;
  KNNType::Pointer classifier = KNNType::New();
  bool lCanRead = classifier->CanReadFile(filename);
  if (lCanRead == false)
    {
    std::cerr << "Erreur otb::KNearestNeighborsMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }
  #endif

  return EXIT_SUCCESS;
}

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
    std::cerr << "Erreur otb::SharkRandomForestsMachineLearningModel : impossible to open the file " << filename << "." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
