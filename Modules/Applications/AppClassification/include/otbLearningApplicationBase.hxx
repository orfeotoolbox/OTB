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

#ifndef otbLearningApplicationBase_hxx
#define otbLearningApplicationBase_hxx

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

  InitSupervisedClassifierParams();
  m_SupervisedClassifier = GetChoiceKeys("classifier");

  InitUnsupervisedClassifierParams();
  std::vector<std::string> allClassifier = GetChoiceKeys("classifier");
  // Check for empty unsupervised classifier
  if( allClassifier.size() > m_UnsupervisedClassifier.size() )
    m_UnsupervisedClassifier.assign( allClassifier.begin() + m_SupervisedClassifier.size(), allClassifier.end() );
}

template <class TInputValue, class TOutputValue>
typename LearningApplicationBase<TInputValue,TOutputValue>::ClassifierCategory
LearningApplicationBase<TInputValue,TOutputValue>
::GetClassifierCategory()
{
  if( m_UnsupervisedClassifier.empty() )
    {
    return Supervised;
    }
  else
    {
    bool foundUnsupervised = std::find( m_UnsupervisedClassifier.begin(), m_UnsupervisedClassifier.end(),
                                        GetParameterString( "classifier" ) ) != m_UnsupervisedClassifier.end();
    return foundUnsupervised ? Unsupervised : Supervised;
    }
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::InitSupervisedClassifierParams()
{

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
  InitNeuralNetworkParams();
  if (!m_RegressionFlag)
    {
    InitNormalBayesParams(); // Regression not supported
    }
  InitRandomForestsParams();
  InitKNNParams();
#endif

#ifdef OTB_USE_SHARK
  InitSharkRandomForestsParams();
#endif
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::InitUnsupervisedClassifierParams()
{
#ifdef OTB_USE_SHARK
  if (!m_RegressionFlag)
  {
    InitSharkKMeansParams(); // Regression not supported
  }
#endif
}

template <class TInputValue, class TOutputValue>
typename LearningApplicationBase<TInputValue,TOutputValue>
::TargetListSampleType::Pointer
LearningApplicationBase<TInputValue,TOutputValue>
::Classify(typename ListSampleType::Pointer validationListSample,
           std::string modelPath)
{
  // Setup fake reporter
  RGBAPixelConverter<int,int>::Pointer dummyFilter =
    RGBAPixelConverter<int,int>::New();
  dummyFilter->SetProgress(0.0f);
  this->AddProcess(dummyFilter,"Validation...");
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

  typename TargetListSampleType::Pointer predictedList = model->PredictBatch(validationListSample, NULL);

  // update reporter
  dummyFilter->UpdateProgress(1.0f);
  dummyFilter->InvokeEvent(itk::EndEvent());

  return predictedList;
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::Train(typename ListSampleType::Pointer trainingListSample,
        typename TargetListSampleType::Pointer trainingLabeledListSample,
        std::string modelPath)
{
  otbAppLogINFO("Computing model file : "<<modelPath);
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
  if(modelName == "sharkrf")
    {
    #ifdef OTB_USE_SHARK
    TrainSharkRandomForests(trainingListSample,trainingLabeledListSample,modelPath);
    #else
    otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
    #endif
    }
  else if(modelName == "sharkkm")
    {
    #ifdef OTB_USE_SHARK
    TrainSharkKMeans( trainingListSample, trainingLabeledListSample, modelPath );
    #else
    otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
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
