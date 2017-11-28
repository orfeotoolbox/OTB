
#ifndef cbTrainAutoencoder_txx
#define cbTrainAutoencoder_txx

#include "otbTrainDimensionalityReductionApplicationBase.h"
#include "otbAutoencoderModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::InitAutoencoderParams()
{


  AddChoice("algorithm.tiedautoencoder", "Shark Tied Autoencoder");
  AddChoice("algorithm.autoencoder", "Shark Autoencoder");
  SetParameterDescription("algorithm.autoencoder",
                          "This group of parameters allows setting Shark autoencoder parameters. "
                          );
                          
  
  //Tied Autoencoder
  AddParameter(ParameterType_Choice, "algorithm.autoencoder.istied",
               "tied weighth <tied/untied>");
  SetParameterDescription(
    "algorithm.autoencoder.istied",
    "Parameter that determine if the weights are tied or not <tied/untied>");
                          
        
  AddChoice("algorithm.autoencoder.istied.yes","Tied weigths");
  AddChoice("algorithm.autoencoder.istied.no","Untied weights");
                  
                          
                          
  //Number Of Iterations
  AddParameter(ParameterType_Int, "algorithm.autoencoder.nbiter",
               "Maximum number of iterations during training");
  SetParameterInt("algorithm.autoencoder.nbiter",100, false);
  SetParameterDescription(
    "algorithm.autoencoder.nbiter",
    "The maximum number of iterations used during training.");
   
  AddParameter(ParameterType_Int, "algorithm.autoencoder.nbiterfinetuning",
               "Maximum number of iterations during training");
  SetParameterInt("algorithm.autoencoder.nbiterfinetuning",0, false);
  SetParameterDescription(
    "algorithm.autoencoder.nbiterfinetuning",
    "The maximum number of iterations used during fine tuning of the whole network.");
  
  AddParameter(ParameterType_Float, "algorithm.autoencoder.epsilon",
               " ");
  SetParameterFloat("algorithm.autoencoder.epsilon",0, false);
  SetParameterDescription(
    "algorithm.autoencoder.epsilon",
    " ");
  
  
  AddParameter(ParameterType_Float, "algorithm.autoencoder.initfactor",
               " ");
  SetParameterFloat("algorithm.autoencoder.initfactor",1, false);
  SetParameterDescription(
    "algorithm.autoencoder.initfactor", "parameter that control the weight initialization of the autoencoder");
  
   //Number Of Hidden Neurons
  AddParameter(ParameterType_StringList ,  "algorithm.autoencoder.nbneuron",   "Size");
  /*AddParameter(ParameterType_Int, "algorithm.autoencoder.nbneuron",
               "Number of neurons in the hidden layer");
  SetParameterInt("algorithm.autoencoder.nbneuron",10, false);*/
  SetParameterDescription(
    "algorithm.autoencoder.nbneuron",
    "The number of neurons in each hidden layer.");
  
  //Regularization
  AddParameter(ParameterType_StringList, "algorithm.autoencoder.regularization", "Strength of the regularization");
  SetParameterDescription("algorithm.autoencoder.regularization", 
                          "Strength of the L2 regularization used during training");
                         
  //Noise strength
  AddParameter(ParameterType_StringList, "algorithm.autoencoder.noise", "Strength of the noise");
  SetParameterDescription("algorithm.autoencoder.noise", 
                          "Strength of the noise");
  
  // Sparsity parameter
  AddParameter(ParameterType_StringList, "algorithm.autoencoder.rho", "Sparsity parameter");
  SetParameterDescription("algorithm.autoencoder.rho", 
                          "Sparsity parameter");
  
  // Sparsity regularization strength
  AddParameter(ParameterType_StringList, "algorithm.autoencoder.beta", "Sparsity regularization strength");
  SetParameterDescription("algorithm.autoencoder.beta", 
                          "Sparsity regularization strength");
                         
  AddParameter(ParameterType_OutputFilename, "algorithm.autoencoder.learningcurve", "Learning curve");
  SetParameterDescription("algorithm.autoencoder.learningcurve", "Learning error values");
  MandatoryOff("algorithm.autoencoder.learningcurve");
		
}


template <class TInputValue, class TOutputValue>
void
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::BeforeTrainAutoencoder(typename ListSampleType::Pointer trainingListSample,
                         std::string modelPath)
{
  // typedef shark::Autoencoder< shark::TanhNeuron, shark::LinearNeuron> AutoencoderType;
  typedef shark::LogisticNeuron NeuronType;
  typedef otb::AutoencoderModel<InputValueType, NeuronType> AutoencoderModelType;
  /*
  // typedef shark::TiedAutoencoder< shark::TanhNeuron, shark::LinearNeuron> TiedAutoencoderType;
  typedef shark::TiedAutoencoder< shark::TanhNeuron, shark::TanhNeuron> TiedAutoencoderType;
  typedef otb::AutoencoderModel<InputValueType, TiedAutoencoderType> TiedAutoencoderModelType;
  */
  std::string TiedWeigth = GetParameterString("algorithm.autoencoder.istied");
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
void TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>::TrainAutoencoder(typename ListSampleType::Pointer trainingListSample,std::string modelPath)
{
		typename autoencoderchoice::Pointer dimredTrainer = autoencoderchoice::New();
		itk::Array<unsigned int> nb_neuron;
		itk::Array<float> noise;
		itk::Array<float> regularization;
		itk::Array<float> rho;
		itk::Array<float> beta;
		std::vector<std::basic_string<char>> s_nbneuron= GetParameterStringList("algorithm.autoencoder.nbneuron");
		std::vector<std::basic_string<char>> s_noise= GetParameterStringList("algorithm.autoencoder.noise");
		std::vector<std::basic_string<char>> s_regularization= GetParameterStringList("algorithm.autoencoder.regularization");
		std::vector<std::basic_string<char>> s_rho= GetParameterStringList("algorithm.autoencoder.rho");
		std::vector<std::basic_string<char>> s_beta= GetParameterStringList("algorithm.autoencoder.beta");
		nb_neuron.SetSize(s_nbneuron.size());
		noise.SetSize(s_nbneuron.size());
		regularization.SetSize(s_nbneuron.size());
		rho.SetSize(s_nbneuron.size());
		beta.SetSize(s_nbneuron.size());
		for (unsigned int i=0; i<s_nbneuron.size(); i++){
			nb_neuron[i]=std::stoi(s_nbneuron[i]);
			noise[i]=std::stof(s_noise[i]);
			regularization[i]=std::stof(s_regularization[i]);
			rho[i]=std::stof(s_rho[i]);
			beta[i]=std::stof(s_beta[i]);
		}
		dimredTrainer->SetNumberOfHiddenNeurons(nb_neuron);
		dimredTrainer->SetNumberOfIterations(GetParameterInt("algorithm.autoencoder.nbiter"));
		dimredTrainer->SetNumberOfIterationsFineTuning(GetParameterInt("algorithm.autoencoder.nbiterfinetuning"));
		dimredTrainer->SetEpsilon(GetParameterFloat("algorithm.autoencoder.epsilon"));
		dimredTrainer->SetInitFactor(GetParameterFloat("algorithm.autoencoder.initfactor"));
		dimredTrainer->SetRegularization(regularization);
		dimredTrainer->SetNoise(noise);
		dimredTrainer->SetRho(rho);
		dimredTrainer->SetBeta(beta);
		
		dimredTrainer->SetWriteWeights(true);
		if (HasValue("algorithm.autoencoder.learningcurve") && IsParameterEnabled("algorithm.autoencoder.learningcurve"))
                  {
                  std::cout << "yo" << std::endl;
                  dimredTrainer->SetWriteLearningCurve(true);
                  dimredTrainer->SetLearningCurveFileName(GetParameterString("algorithm.autoencoder.learningcurve"));
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
