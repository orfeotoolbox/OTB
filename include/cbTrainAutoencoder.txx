
#ifndef cbTrainAutoencoder_txx
#define cbTrainAutoencoder_txx

#include "cbLearningApplicationBaseDR.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::InitAutoencoderParams()
{


  AddChoice("model.tiedautoencoder", "Shark Tied Autoencoder");
  AddChoice("model.autoencoder", "Shark Autoencoder");
  SetParameterDescription("model.autoencoder",
                          "This group of parameters allows setting Shark autoencoder parameters. "
                          );
  //Number Of Iterations
  AddParameter(ParameterType_Int, "model.autoencoder.nbiter",
               "Maximum number of iterations during training");
  SetParameterInt("model.autoencoder.nbiter",100, false);
  SetParameterDescription(
    "model.autoencoder.nbiter",
    "The maximum number of iterations used during training.");
  
  
   //Number Of Hidden Neurons
  AddParameter(ParameterType_Int, "model.autoencoder.nbneuron",
               "Number of neurons in the hidden layer");
  SetParameterInt("model.autoencoder.nbneuron",10, false);
  SetParameterDescription(
    "model.autoencoder.nbneuron",
    "The number of neurons in the hidden layer.");
  
  //normalization
  AddParameter(ParameterType_Float, "model.autoencoder.normalizer", "Strength of the normalization");
  SetParameterFloat("model.autoencoder.normalizer",0, false);
  SetParameterDescription("model.autoencoder.normalizer", 
                         "Strength of the L2 normalization used during training");
}
/*
template <class TInputValue, class TOutputValue>
void cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::template < autoencoderchoice> TrainAutoencoder(typename ListSampleType::Pointer trainingListSample,std::string modelPath)
{
 // typename AutoencoderModelType::Pointer dimredTrainer = AutoencoderModelType::New();
  autoencoderchoice::Pointer dimredTrainer = autoencoderchoice::New();
		dimredTrainer->SetNumberOfHiddenNeurons(GetParameterInt("model.autoencoder.nbneuron"));
		dimredTrainer->SetNumberOfIterations(GetParameterInt("model.autoencoder.nbiter"));
		dimredTrainer->SetRegularization(GetParameterFloat("model.autoencoder.normalizer"));
		dimredTrainer->SetInputListSample(trainingListSample);
		dimredTrainer->Train();
		dimredTrainer->Save(modelPath);
}
*/
} //end namespace wrapper
} //end namespace otb

#endif
