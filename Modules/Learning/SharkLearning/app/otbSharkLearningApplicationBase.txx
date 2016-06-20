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
#ifndef __otbSharkLearningApplicationBase_txx
#define __otbSharkLearningApplicationBase_txx

#include "otbSharkLearningApplicationBase.h"
// only need this filter as a dummy process object
#include "otbRGBAPixelConverter.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
SharkLearningApplicationBase<TInputValue,TOutputValue>
::SharkLearningApplicationBase() : m_RegressionFlag(false)
{
} 

template <class TInputValue, class TOutputValue>
void
SharkLearningApplicationBase<TInputValue,TOutputValue>
::DoInit()
{
  AddDocTag(Tags::Learning);

  // main choice parameter that will contain all machine learning options
  AddParameter(ParameterType_Choice, "classifier", "Classifier to use for the training");
  SetParameterDescription("classifier", "Choice of the classifier to use for the training.");


#ifdef OTB_USE_SHARK
  InitSharkRandomForestsParams();
#endif

}

template <class TInputValue, class TOutputValue>
void
SharkLearningApplicationBase<TInputValue,TOutputValue>
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
SharkLearningApplicationBase<TInputValue,TOutputValue>
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
if (modelName == "sharkrf")
    {
    #ifdef OTB_USE_SHARK
    TrainSharkRandomForests(trainingListSample, trainingLabeledListSample, modelPath);
#else
    otbAppLogFATAL("Module Shark is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
     #endif
    }
  // update reporter
  dummyFilter->UpdateProgress(1.0f);
  dummyFilter->InvokeEvent(itk::EndEvent());
}

}
}

#endif
