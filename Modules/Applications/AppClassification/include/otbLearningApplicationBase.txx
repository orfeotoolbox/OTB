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

  AddParameter(ParameterType_Choice, "category", "Type of classifier use for the training (supervised or unsupervised");
  SetParameterDescription("category", "Choice of the classifier type to use for the training, "
          "choice is supervised or unsupervised.");

  InitSupervisedClassifierParams();
  m_SupervisedClassifier = GetChoiceKeys("classifier");

  InitUnsupervisedClassifierParams();
  std::vector<std::string> allClassifier = GetChoiceKeys("classifier");
  m_UnsupervisedClassifier.assign(allClassifier.begin() + m_SupervisedClassifier.size(), allClassifier.end());
}

template <class TInputValue, class TOutputValue>
typename LearningApplicationBase<TInputValue,TOutputValue>::ClassifierCategory
LearningApplicationBase<TInputValue,TOutputValue>
::GetClassifierCategory()
{
  bool foundUnsupervised =
          std::find(m_UnsupervisedClassifier.begin(), m_UnsupervisedClassifier.end(),
                    GetParameterString("classifier")) != m_UnsupervisedClassifier.end();
  return foundUnsupervised ? Unsupervised : Supervised;
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::DoUpdateParameters()
{
};

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
  InitGradientBoostedTreeParams();
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
  InitSharkKMeansParams();
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
  if(modelName == "sharkrf")
    {
    #ifdef OTB_USE_SHARK
    TrainSharkRandomForests(trainingListSample,trainingLabeledListSample,modelPath);
    #else
    otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
    #endif
    }
  if(modelName == "sharkkm")
    {
    #ifdef OTB_USE_SHARK
    TrainSharkKMeans( trainingListSample, trainingLabeledListSample, modelPath );
    #else
    otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
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
