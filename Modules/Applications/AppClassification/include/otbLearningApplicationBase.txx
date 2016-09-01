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
#ifndef otbLearningApplicationBase_txx
#define otbLearningApplicationBase_txx

#include "otbLearningApplicationBase.h"
// only need this filter as a dummy process object
#include "otbRGBAPixelConverter.h"

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
LearningApplicationBase<TInputValue,TOutputValue>
::~LearningApplicationBase()
{
  ModelFactoryType::CleanFactories();
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::DoInit()
{
  AddDocTag(Tags::Learning);

  // main choice parameter that will contain all machine learning options
  AddParameter(ParameterType_Choice, "classifier", "Classifier to use for the training");
  SetParameterDescription("classifier", "Choice of the classifier to use for the training.");

  //Group LibSVM
#ifdef OTB_USE_LIBSVM 
  InitLibSVMParams();
#endif

#ifdef OTB_USE_OPENCV
  // OpenCV SVM implementation is buggy with linear kernel
  // Users should use the libSVM implementation instead.
  // InitSVMParams();
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
  // Setup fake reporter
  RGBAPixelConverter<int,int>::Pointer dummyFilter =
    RGBAPixelConverter<int,int>::New();
  dummyFilter->SetProgress(0.0f);
  this->AddProcess(dummyFilter,"Classify...");
  dummyFilter->InvokeEvent(itk::StartEvent());

  // load a machine learning model from file and predict the input sample list
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

  // update reporter
  dummyFilter->UpdateProgress(1.0f);
  dummyFilter->InvokeEvent(itk::EndEvent());
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::Train(typename ListSampleType::Pointer trainingListSample,
        typename TargetListSampleType::Pointer trainingLabeledListSample,
        std::string modelPath)
{
  // Setup fake reporter
  RGBAPixelConverter<int,int>::Pointer dummyFilter =
    RGBAPixelConverter<int,int>::New();
  dummyFilter->SetProgress(0.0f);
  this->AddProcess(dummyFilter,"Training model...");
  dummyFilter->InvokeEvent(itk::StartEvent());

  // get the name of the chosen machine learning model
  const std::string modelName = GetParameterString("classifier");
  // call specific train function
  if (modelName == "libsvm")
    {
	#ifdef OTB_USE_LIBSVM
    TrainLibSVM(trainingListSample, trainingLabeledListSample, modelPath);
    #else
    otbAppLogFATAL("Module LIBSVM is not installed. You should consider turning OTB_USE_LIBSVM on during cmake configuration.");
    #endif
    }
  // OpenCV SVM implementation is buggy with linear kernel
  // Users should use the libSVM implementation instead.
  // else if (modelName == "svm")
  //  {
	//  #ifdef OTB_USE_OPENCV
  //   TrainSVM(trainingListSample, trainingLabeledListSample, modelPath);
  //  #else
  //   otbAppLogFATAL("Module OPENCV is not installed. You should consider turning OTB_USE_OPENCV on during cmake configuration.");
  //  #endif
  //  }
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
  // update reporter
  dummyFilter->UpdateProgress(1.0f);
  dummyFilter->InvokeEvent(itk::EndEvent());
}

}
}

#endif
