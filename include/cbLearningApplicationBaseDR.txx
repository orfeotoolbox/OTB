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
#ifndef cbLearningApplicationBaseDR_txx
#define cbLearningApplicationBaseDR_txx

#include "cbLearningApplicationBaseDR.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::cbLearningApplicationBaseDR() 
{
}

template <class TInputValue, class TOutputValue>
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::~cbLearningApplicationBaseDR()
{
  ModelFactoryType::CleanFactories();
}

template <class TInputValue, class TOutputValue>
void
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::DoInit()
{
  AddDocTag(Tags::Learning);

  // main choice parameter that will contain all machine learning options
  AddParameter(ParameterType_Choice, "model", "moddel to use for the training");
  SetParameterDescription("model", "Choice of the dimensionality reduction model to use for the training.");


#ifdef OTB_USE_SHARK
  InitAutoencoderParams();
#endif
  
}

template <class TInputValue, class TOutputValue>
void
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::Reduce(typename ListSampleType::Pointer validationListSample,std::string modelPath)
{/*
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
  dummyFilter->InvokeEvent(itk::EndEvent());*/
}

template <class TInputValue, class TOutputValue>
void
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::Train(typename ListSampleType::Pointer trainingListSample,
        std::string modelPath)
{
 
  // get the name of the chosen machine learning model
  const std::string modelName = GetParameterString("model");
  // call specific train function

  if(modelName == "autoencoder")
    {
    #ifdef OTB_USE_SHARK
    TrainAutoencoder<AutoencoderModelType>(trainingListSample,modelPath);
    #else
    otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
    #endif
    }
  if(modelName == "tiedautoencoder")
    {
    #ifdef OTB_USE_SHARK
    TrainAutoencoder<TiedAutoencoderModelType>(trainingListSample,modelPath);
    #else
    otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
    #endif
    }
}

}
}

#endif
