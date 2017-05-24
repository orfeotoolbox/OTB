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

  // main choice parameter that will contain all dimensionality reduction options
  AddParameter(ParameterType_Choice, "model", "moddel to use for the training");
  SetParameterDescription("model", "Choice of the dimensionality reduction model to use for the training.");
  

  InitSOMParams();
#ifdef OTB_USE_SHARK
  InitAutoencoderParams();
  InitPCAParams();
#endif
  
}

template <class TInputValue, class TOutputValue>
void
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::Reduce(typename ListSampleType::Pointer validationListSample,std::string modelPath)
{
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
 
 if(modelName == "som")
	{
		BeforeTrainSOM(trainingListSample,modelPath);
	}
 
 if(modelName == "autoencoder")
    {
    #ifdef OTB_USE_SHARK
    BeforeTrainAutoencoder(trainingListSample,modelPath);
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
    
  if(modelName == "pca")
    {
    #ifdef OTB_USE_SHARK
    TrainPCA(trainingListSample,modelPath);
    #else
    otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
    #endif
    }
}

}
}

#endif
