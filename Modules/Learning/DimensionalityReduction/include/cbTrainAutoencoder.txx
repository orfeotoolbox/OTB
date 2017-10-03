
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
   
  AddParameter(ParameterType_Int, "model.autoencoder.nbiterfinetuning",
               "Maximum number of iterations during training");
  SetParameterInt("model.autoencoder.nbiterfinetuning",0, false);
  SetParameterDescription(
    "model.autoencoder.nbiterfinetuning",
    "The maximum number of iterations used during fine tuning of the whole network.");
  
  AddParameter(ParameterType_Float, "model.autoencoder.epsilon",
               " ");
  SetParameterFloat("model.autoencoder.epsilon",0, false);
  SetParameterDescription(
    "model.autoencoder.epsilon",
    " ");
  
  
  AddParameter(ParameterType_Float, "model.autoencoder.initfactor",
               " ");
  SetParameterFloat("model.autoencoder.initfactor",1, false);
  SetParameterDescription(
    "model.autoencoder.initfactor", "parameter that control the weight initialization of the autoencoder");
  
   //Number Of Hidden Neurons
  AddParameter(ParameterType_StringList ,  "model.autoencoder.nbneuron",   "Size");
  /*AddParameter(ParameterType_Int, "model.autoencoder.nbneuron",
               "Number of neurons in the hidden layer");
  SetParameterInt("model.autoencoder.nbneuron",10, false);*/
  SetParameterDescription(
    "model.autoencoder.nbneuron",
    "The number of neurons in each hidden layer.");
  
  //Regularization
  AddParameter(ParameterType_StringList, "model.autoencoder.regularization", "Strength of the regularization");
  SetParameterDescription("model.autoencoder.regularization", 
                         "Strength of the L2 regularization used during training");
                         
  //Noise strength
  AddParameter(ParameterType_StringList, "model.autoencoder.noise", "Strength of the noise");
  SetParameterDescription("model.autoencoder.noise", 
                         "Strength of the noise");
  
  // Sparsity parameter
  AddParameter(ParameterType_StringList, "model.autoencoder.rho", "Sparsity parameter");
  SetParameterDescription("model.autoencoder.rho", 
                         "Sparsity parameter");
  
  // Sparsity regularization strength
  AddParameter(ParameterType_StringList, "model.autoencoder.beta", "Sparsity regularization strength");
  SetParameterDescription("model.autoencoder.beta", 
                         "Sparsity regularization strength");
                         
  AddParameter(ParameterType_OutputFilename, "model.autoencoder.learningcurve", "Learning curve");
  SetParameterDescription("model.autoencoder.learningcurve", "Learning error values");
  MandatoryOff("model.autoencoder.learningcurve");
		
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
	/*	
	if(TiedWeigth == "yes")
		{
		TrainAutoencoder<TiedAutoencoderModelType>(trainingListSample,modelPath);
		}
	 */
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
		itk::Array<unsigned int> nb_neuron;
		itk::Array<float> noise;
		itk::Array<float> regularization;
		itk::Array<float> rho;
		itk::Array<float> beta;
		std::vector<std::basic_string<char>> s_nbneuron= GetParameterStringList("model.autoencoder.nbneuron");
		std::vector<std::basic_string<char>> s_noise= GetParameterStringList("model.autoencoder.noise");
		std::vector<std::basic_string<char>> s_regularization= GetParameterStringList("model.autoencoder.regularization");
		std::vector<std::basic_string<char>> s_rho= GetParameterStringList("model.autoencoder.rho");
		std::vector<std::basic_string<char>> s_beta= GetParameterStringList("model.autoencoder.beta");
		nb_neuron.SetSize(s_nbneuron.size());
		noise.SetSize(s_nbneuron.size());
		regularization.SetSize(s_nbneuron.size());
		rho.SetSize(s_nbneuron.size());
		beta.SetSize(s_nbneuron.size());
		for (int i=0; i<s_nbneuron.size(); i++){ 
			nb_neuron[i]=std::stoi(s_nbneuron[i]);
			noise[i]=std::stof(s_noise[i]);
			regularization[i]=std::stof(s_regularization[i]);
			rho[i]=std::stof(s_rho[i]);
			beta[i]=std::stof(s_beta[i]);
		}
		dimredTrainer->SetNumberOfHiddenNeurons(nb_neuron);
		dimredTrainer->SetNumberOfIterations(GetParameterInt("model.autoencoder.nbiter"));
		dimredTrainer->SetNumberOfIterationsFineTuning(GetParameterInt("model.autoencoder.nbiterfinetuning"));
		dimredTrainer->SetEpsilon(GetParameterFloat("model.autoencoder.epsilon"));
		dimredTrainer->SetInitFactor(GetParameterFloat("model.autoencoder.initfactor"));
		dimredTrainer->SetRegularization(regularization);
		dimredTrainer->SetNoise(noise);
		dimredTrainer->SetRho(rho);
		dimredTrainer->SetBeta(beta);
		
		dimredTrainer->SetWriteWeights(true);
		if (HasValue("model.autoencoder.learningcurve") && IsParameterEnabled("model.autoencoder.learningcurve"))
		{
			std::cout << "yo" << std::endl;
			dimredTrainer->SetWriteLearningCurve(true);
			dimredTrainer->SetLearningCurveFileName(GetParameterString("model.autoencoder.learningcurve"));
		}
		
		dimredTrainer->SetInputListSample(trainingListSample);
		std::cout << "before train" << std::endl;
		dimredTrainer->Train();
		std::cout << "after train" << std::endl;
		dimredTrainer->Save(modelPath);	
		

}

} //end namespace wrapper
} //end namespace otb

#endif
