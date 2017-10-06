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

#include "otbTrainDimensionalityReductionApplicationBase.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::TrainDimensionalityReductionApplicationBase() 
{
}

template <class TInputValue, class TOutputValue>
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::~TrainDimensionalityReductionApplicationBase()
{
  ModelFactoryType::CleanFactories();
}

template <class TInputValue, class TOutputValue>
void
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::DoInit()
{
  AddDocTag(Tags::Learning);

  // main choice parameter that will contain all dimensionality reduction options
  AddParameter(ParameterType_Choice, "algorithm", "algorithm to use for the training");
  SetParameterDescription("algorithm", "Choice of the dimensionality reduction algorithm to use for the training.");
  

  InitSOMParams();
  
#ifdef OTB_USE_SHARK
  InitAutoencoderParams();
  InitPCAParams();
#endif
  
}

template <class TInputValue, class TOutputValue>
void
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::Reduce(typename ListSampleType::Pointer validationListSample,std::string modelPath)
{
}

template <class TInputValue, class TOutputValue>
void
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::Train(typename ListSampleType::Pointer trainingListSample,
        std::string modelPath)
{
 
 // get the name of the chosen machine learning model
  const std::string modelName = GetParameterString("algorithm");
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
    /*
  if(modelName == "tiedautoencoder")
    {
		#ifdef OTB_USE_SHARK
		TrainAutoencoder<TiedAutoencoderModelType>(trainingListSample,modelPath);
		#else
		otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
		#endif
    }
    */
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
