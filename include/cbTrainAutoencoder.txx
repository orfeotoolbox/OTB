
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
                          
  
  //Tied Autoencoder
  AddParameter(ParameterType_Choice, "model.autoencoder.istied",
               "tied weighth <tied/untied>");
  SetParameterDescription(
    "model.autoencoder.istied",
    "Parameter that determine if the weights are tied or not <tied/untied>");
                          
        
  AddChoice("model.autoencoder.istied.yes","Tied weigths");
  AddChoice("model.autoencoder.istied.no","Untied weights");
                  
                          
                          
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
  
  //Regularization
  AddParameter(ParameterType_Float, "model.autoencoder.regularization", "Strength of the regularization");
  SetParameterFloat("model.autoencoder.regularization",0, false);
  SetParameterDescription("model.autoencoder.regularization", 
                         "Strength of the L2 regularization used during training");
                         
  //Noise strength
  AddParameter(ParameterType_Float, "model.autoencoder.noise", "Strength of the noise");
  SetParameterFloat("model.autoencoder.noise",0, false);
  SetParameterDescription("model.autoencoder.noise", 
                         "Strength of the noise");
}


template <class TInputValue, class TOutputValue>
void
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::BeforeTrainAutoencoder(typename ListSampleType::Pointer trainingListSample,
        std::string modelPath)
{		
	std::string TiedWeigth = GetParameterString("model.autoencoder.istied");
	std::cout << TiedWeigth << std::endl;
		
	if(TiedWeigth == "no")
		{
		TrainAutoencoder<AutoencoderModelType>(trainingListSample,modelPath);
		}
		
	if(TiedWeigth == "yes")
		{
		TrainAutoencoder<TiedAutoencoderModelType>(trainingListSample,modelPath);
		}
	 
	if(TiedWeigth != "yes" && TiedWeigth != "no")
		{
			std::cerr << "istied : invalid choice <yes/no>" << std::endl;
		}
}



template <class TInputValue, class TOutputValue>
template <typename autoencoderchoice>
void cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::TrainAutoencoder(typename ListSampleType::Pointer trainingListSample,std::string modelPath)
{
		typename autoencoderchoice::Pointer dimredTrainer = autoencoderchoice::New();
		dimredTrainer->SetNumberOfHiddenNeurons(GetParameterInt("model.autoencoder.nbneuron"));
		dimredTrainer->SetNumberOfIterations(GetParameterInt("model.autoencoder.nbiter"));
		dimredTrainer->SetRegularization(GetParameterFloat("model.autoencoder.regularization"));
		dimredTrainer->SetRegularization(GetParameterFloat("model.autoencoder.noise"));
		dimredTrainer->SetInputListSample(trainingListSample);
		dimredTrainer->Train();
		dimredTrainer->Save(modelPath);
}

} //end namespace wrapper
} //end namespace otb

#endif
