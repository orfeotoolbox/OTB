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

#include <fstream>
#include <string>
#include <algorithm>

#include "otbMachineLearningModel.h"
#include "otbLibSVMMachineLearningModel.h"
#include "otbSVMMachineLearningModel.h"
#include "otbKNearestNeighborsMachineLearningModel.h"
#include "otbRandomForestsMachineLearningModel.h"
#include "otbBoostMachineLearningModel.h"
#include "otbNeuralNetworkMachineLearningModel.h"

#include "otbConfusionMatrixCalculator.h"


typedef otb::MachineLearningModel<float,short>         MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       InputValueType;
typedef MachineLearningModelType::InputSampleType      InputSampleType;
typedef MachineLearningModelType::InputListSampleType  InputListSampleType;
typedef MachineLearningModelType::TargetValueType      TargetValueType;
typedef MachineLearningModelType::TargetSampleType     TargetSampleType;
typedef MachineLearningModelType::TargetListSampleType TargetListSampleType;
typedef otb::ConfusionMatrixCalculator<TargetListSampleType, TargetListSampleType> ConfusionMatrixCalculatorType;

bool ReadDataFile(const std::string & infname, InputListSampleType * samples, TargetListSampleType * labels)
{
  std::ifstream ifs;
  ifs.open(infname.c_str());

  if(!ifs)
    {
    std::cout<<"Could not read file "<<infname<<std::endl;
    return false;
    }

  unsigned int nbfeatures = 0;

  while (!ifs.eof())
    {
    std::string line;
    std::getline(ifs, line);

    if(nbfeatures == 0)
      {
      nbfeatures = std::count(line.begin(),line.end(),' ')-1;
      //std::cout<<"Found "<<nbfeatures<<" features per samples"<<std::endl;
      }

    if(line.size()>1)
      {
      InputSampleType sample(nbfeatures);
      sample.Fill(0);

      std::string::size_type pos = line.find_first_of(" ", 0);

      // Parse label
      TargetSampleType label;
      label[0] = atoi(line.substr(0, pos).c_str());
      //std::cout << "label = " << label[0] << std::endl;

      bool endOfLine = false;
      unsigned int id = 0;

      while(!endOfLine)
        {
        std::string::size_type nextpos = line.find_first_of(" ", pos+1);

        if(nextpos == std::string::npos)
          {
          endOfLine = true;
          nextpos = line.size()-1;
          }
        else
          {
          std::string feature = line.substr(pos,nextpos-pos);
          std::string::size_type semicolonpos = feature.find_first_of(":");
          id = atoi(feature.substr(0,semicolonpos).c_str());
          //std::cout << "id = " << id << std::endl;
          sample[id - 1] = atof(feature.substr(semicolonpos+1,feature.size()-semicolonpos).c_str());
          //std::cout << "sample[" << id << "] = " << sample[id] << std::endl;
          pos = nextpos;
          }
        }
      samples->PushBack(sample);
      labels->PushBack(label);
      }
    }

  //std::cout<<"Retrieved "<<samples->Size()<<" samples"<<std::endl;
  ifs.close();
  return true;
}

int otbLibSVMMachineLearningModelNew(int argc, char * argv[])
{
  typedef otb::LibSVMMachineLearningModel<InputValueType, TargetValueType> SVMType;
  SVMType::Pointer svmclassifier = SVMType::New();
  return EXIT_SUCCESS;
}

int otbLibSVMMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3)
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }


  typedef otb::LibSVMMachineLearningModel<InputValueType, TargetValueType> SVMType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();
  TargetListSampleType::Pointer predicted = TargetListSampleType::New();

  if (!ReadDataFile(argv[1], samples, labels))
    {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
    }

  SVMType::Pointer svmclassifier = SVMType::New();
  svmclassifier->SetInputListSample(samples);
  svmclassifier->SetTargetListSample(labels);
  svmclassifier->Train();

  svmclassifier->SetTargetListSample(predicted);
  svmclassifier->PredictAll();

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout << "Confusion matrix: " << std::endl;
  std::cout << cmCalculator->GetConfusionMatrix() << std::endl;
  std::cout << "Kappa: " << cmCalculator->GetKappaIndex() << std::endl;
  std::cout << "Overall Accuracy: " << cmCalculator->GetOverallAccuracy() << std::endl;

  svmclassifier->Save(argv[2]);

  return EXIT_SUCCESS;
}

int otbSVMMachineLearningModelNew(int argc, char * argv[])
{
  typedef otb::SVMMachineLearningModel<InputValueType,TargetValueType> SVMType;
  SVMType::Pointer svmclassifier = SVMType::New();
  return EXIT_SUCCESS;
}

int otbSVMMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::SVMMachineLearningModel<InputValueType, TargetValueType> SVMType;

  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();
  TargetListSampleType::Pointer predicted = TargetListSampleType::New();

  if(!ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  SVMType::Pointer svmclassifier = SVMType::New();
  svmclassifier->SetInputListSample(samples);
  svmclassifier->SetTargetListSample(labels);
  svmclassifier->Train();

  svmclassifier->SetTargetListSample(predicted);
  svmclassifier->PredictAll();

  svmclassifier->Save(argv[2]);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  std::cout<<"Kappa: "<<cmCalculator->GetKappaIndex()<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  return EXIT_SUCCESS;
}

int otbKNearestNeighborsMachineLearningModelNew(int argc, char * argv[])
{
  typedef otb::KNearestNeighborsMachineLearningModel<InputValueType,TargetValueType> KNearestNeighborsType;
  KNearestNeighborsType::Pointer knnclassifier = KNearestNeighborsType::New();
  return EXIT_SUCCESS;
}

int otbKNearestNeighborsMachineLearningModel(int argc, char * argv[])
{
  if (argc != 2 )
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file"<<std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::KNearestNeighborsMachineLearningModel<InputValueType,TargetValueType> KNearestNeighborsType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();
  TargetListSampleType::Pointer predicted = TargetListSampleType::New();

  if(!ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  KNearestNeighborsType::Pointer knnclassifier = KNearestNeighborsType::New();
  knnclassifier->SetInputListSample(samples);
  knnclassifier->SetTargetListSample(labels);
  knnclassifier->Train();

  knnclassifier->SetTargetListSample(predicted);
  knnclassifier->Superclass::PredictAll();

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  std::cout<<"Kappa: "<<cmCalculator->GetKappaIndex()<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  //write the model
  //knnclassifier->Save(argv[2]);

  return EXIT_SUCCESS;
}

int otbRandomForestsMachineLearningModelNew(int argc, char * argv[])
{
  typedef otb::RandomForestsMachineLearningModel<InputValueType,TargetValueType> RandomForestType;
  RandomForestType::Pointer rfclassifier = RandomForestType::New();
  return EXIT_SUCCESS;
}

int otbRandomForestsMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
  {
    std::cout<<"Wrong number of arguments "<<std::endl;
    std::cout<<"Usage : sample file, output file "<<std::endl;
    return EXIT_FAILURE;
  }

  typedef otb::RandomForestsMachineLearningModel<InputValueType,TargetValueType> RandomForestType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();
  TargetListSampleType::Pointer predicted = TargetListSampleType::New();

  if(!ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  std::vector<float> priors(26,1.);


  RandomForestType::Pointer rfclassifier = RandomForestType::New();
  rfclassifier->SetInputListSample(samples);
  rfclassifier->SetTargetListSample(labels);

  //set parameters
  rfclassifier->SetPriors(priors);
  // rfclassifier->SetMaxNumberOfTrees(30);
  // rfclassifier->SetMaxDepth(30);
  // rfclassifier->SetMaxNumberOfCategories(30);
  // rfclassifier->SetMaxNumberOfVariables(4);

  rfclassifier->Train();

  rfclassifier->SetTargetListSample(predicted);
  rfclassifier->PredictAll();

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  std::cout<<rfclassifier->GetTargetListSample()->Size()<<std::endl;

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  rfclassifier->Save(argv[2]);
  //std::cout<<"GetTrainError: "<<rfclassifier->GetTrainError() << std::endl;


  //Load Model to new RF
  TargetListSampleType::Pointer predictedLoad = TargetListSampleType::New();
  RandomForestType::Pointer rfclassifierLoad = RandomForestType::New();

  rfclassifierLoad->Load(argv[2]);
  rfclassifierLoad->SetInputListSample(samples);
  rfclassifierLoad->SetTargetListSample(predictedLoad);
  rfclassifierLoad->PredictAll();

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( vcl_abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}

int otbBoostMachineLearningModelNew(int argc, char * argv[])
{
  typedef otb::BoostMachineLearningModel<InputValueType,TargetValueType> BoostType;
  BoostType::Pointer classifier = BoostType::New();
  return EXIT_SUCCESS;
}

int otbBoostMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::BoostMachineLearningModel<InputValueType, TargetValueType> BoostType;

  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();
  TargetListSampleType::Pointer predicted = TargetListSampleType::New();

  if(!ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  BoostType::Pointer classifier = BoostType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->Train();

  classifier->SetTargetListSample(predicted);
  classifier->PredictAll();

  classifier->Save(argv[2]);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  std::cout<<"Kappa: "<<cmCalculator->GetKappaIndex()<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  return EXIT_SUCCESS;
}

int otbANNMachineLearningModelNew(int argc, char * argv[])
{
  typedef otb::NeuralNetworkMachineLearningModel<InputValueType, TargetValueType> ANNType;
  ANNType::Pointer classifier = ANNType::New();
  return EXIT_SUCCESS;
}

int otbANNMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3)
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }


  typedef otb::NeuralNetworkMachineLearningModel<InputValueType, TargetValueType> ANNType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();
  TargetListSampleType::Pointer predicted = TargetListSampleType::New();

  if (!ReadDataFile(argv[1], samples, labels))
    {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
    }

  std::cout<<"number of samples = "<<samples->Size()<<std::endl;

  std::vector<unsigned int> layerSizes;
  layerSizes.push_back(16);
  layerSizes.push_back(25);
  layerSizes.push_back(35);
  layerSizes.push_back(45);
  layerSizes.push_back(1);

  ANNType::Pointer classifier = ANNType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->SetLayerSizes(layerSizes);
  classifier->SetTrainMethod(CvANN_MLP_TrainParams::BACKPROP);
  classifier->SetBackPropDWScale(0.005);
  classifier->SetBackPropMomentScale(0.005);
  classifier->Train();

  classifier->SetTargetListSample(predicted);
  classifier->PredictAll();

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout << "Confusion matrix: " << std::endl;
  std::cout << cmCalculator->GetConfusionMatrix() << std::endl;
  std::cout << "Kappa: " << cmCalculator->GetKappaIndex() << std::endl;
  std::cout << "Overall Accuracy: " << cmCalculator->GetOverallAccuracy() << std::endl;

  classifier->Save(argv[2]);

  return EXIT_SUCCESS;
}




