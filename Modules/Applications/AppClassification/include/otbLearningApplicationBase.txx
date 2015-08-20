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
#ifndef __otbLearningApplicationBase_txx
#define __otbLearningApplicationBase_txx

#include "otbLearningApplicationBase.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
LearningApplicationBase<TInputValue,TOutputValue>
::LearningApplicationBase() : m_RegressionFlag(false)
{
} 

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::DoInit()
{
  AddDocTag(Tags::Learning);

  AddParameter(ParameterType_Choice, "classifier", "Classifier to use for the training");
  SetParameterDescription("classifier", "Choice of the classifier to use for the training.");

  //Group LibSVM
#ifdef OTB_USE_LIBSVM 
  InitLibSVMParams();
#endif

#ifdef OTB_USE_OPENCV
  InitSVMParams();
  if (!m_RegressionFlag)
    {
    InitBoostParams();  // Regression not supported
    }
  InitDecisionTreeParams();
  InitGradientBoostedTreeParams();
  InitNeuralNetworkParams();
  if (!m_RegressionFlag)
    {
    InitNormalBayesParams(); // Regression not supported
    }
  InitRandomForestsParams();
  InitKNNParams();
#endif
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::Classify(typename ListSampleType::Pointer validationListSample,
           typename TargetListSampleType::Pointer predictedList,
           std::string modelPath)
{
  //Classification
  ModelPointerType model = ModelFactoryType::CreateMachineLearningModel(modelPath,
                                                                        ModelFactoryType::ReadMode);

  if (model.IsNull())
    {
    otbAppLogFATAL(<< "Error when loading model " << modelPath);
    }

  model->Load(modelPath);
  model->SetRegressionMode(this->m_RegressionFlag);
  model->SetInputListSample(validationListSample);
  model->SetTargetListSample(predictedList);
  model->PredictAll();
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::Train(typename ListSampleType::Pointer trainingListSample,
        typename TargetListSampleType::Pointer trainingLabeledListSample,
        std::string modelPath)
{
  const std::string modelName = GetParameterString("classifier");
  if (modelName == "libsvm")
    {
	#ifdef OTB_USE_LIBSVM
    TrainLibSVM(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module LIBSVM is not installed. You should consider turning OTB_USE_LIBSVM on during cmake configuration.");
    #endif
    }
  else if (modelName == "svm")
    {
	#ifdef OTB_USE_OPENCV
    TrainSVM(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module OPENCV is not installed. You should consider turning OTB_USE_OPENCV on during cmake configuration.");
    #endif
    }
  else if (modelName == "boost")
    {
	#ifdef OTB_USE_OPENCV
    TrainBoost(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module OPENCV is not installed. You should consider turning OTB_USE_OPENCV on during cmake configuration.");
    #endif
    }
  else if (modelName == "dt")
    {
	#ifdef OTB_USE_OPENCV
    TrainDecisionTree(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module OPENCV is not installed. You should consider turning OTB_USE_OPENCV on during cmake configuration.");
    #endif
    }
  else if (modelName == "gbt")
    {
	#ifdef OTB_USE_OPENCV
    TrainGradientBoostedTree(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module OPENCV is not installed. You should consider turning OTB_USE_OPENCV on during cmake configuration.");
    #endif
    }
  else if (modelName == "ann")
    {
	#ifdef OTB_USE_OPENCV
    TrainNeuralNetwork(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module OPENCV is not installed. You should consider turning OTB_USE_OPENCV on during cmake configuration.");
    #endif
    }
  else if (modelName == "bayes")
    {
	#ifdef OTB_USE_OPENCV
    TrainNormalBayes(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module OPENCV is not installed. You should consider turning OTB_USE_OPENCV on during cmake configuration.");
    #endif
    }
  else if (modelName == "rf")
    {
	#ifdef OTB_USE_OPENCV
    TrainRandomForests(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module OPENCV is not installed. You should consider turning OTB_USE_OPENCV on during cmake configuration.");
    #endif
    }
  else if (modelName == "knn")
    {
	#ifdef OTB_USE_OPENCV
    TrainKNN(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module OPENCV is not installed. You should consider turning OTB_USE_OPENCV on during cmake configuration.");
    #endif
    }
}

}
}

#endif
