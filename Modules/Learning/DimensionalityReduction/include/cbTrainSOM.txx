
#ifndef cbTrainSOM_txx
#define cbTrainSOM_txx
#include "cbLearningApplicationBaseDR.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::InitSOMParams()
{

  AddChoice("model.som", "OTB SOM");
  SetParameterDescription("model.som",
                          "This group of parameters allows setting SOM parameters. "
                          );

	AddParameter(ParameterType_Int, "model.som.dim","Dimension of the map");
	SetParameterDescription("model.som.dim","Dimension of the SOM map.");
  
	AddParameter(ParameterType_StringList ,  "model.som.s",   "Size");
    SetParameterDescription("model.som.s", "Size of the SOM map");
    MandatoryOff("model.som.s");
    
	AddParameter(ParameterType_StringList ,  "model.som.n",   "Size Neighborhood");
    SetParameterDescription("model.som.n", "Size of the initial neighborhood in the SOM map");
    MandatoryOff("model.som.n");
    
    AddParameter(ParameterType_Int,  "model.som.sx",   "SizeX");
    SetParameterDescription("model.som.sx", "X size of the SOM map");
    MandatoryOff("model.som.sx");

    AddParameter(ParameterType_Int,  "model.som.sy",   "SizeY");
    SetParameterDescription("model.som.sy", "Y size of the SOM map");
    MandatoryOff("model.som.sy");

    AddParameter(ParameterType_Int,  "model.som.nx",   "NeighborhoodX");
    SetParameterDescription("model.som.nx", "X size of the initial neighborhood in the SOM map");
    MandatoryOff("model.som.nx");

    AddParameter(ParameterType_Int,  "model.som.ny",   "NeighborhoodY");
    SetParameterDescription("model.som.ny", "Y size of the initial neighborhood in the SOM map");
    MandatoryOff("model.som.nx");

    AddParameter(ParameterType_Int,  "model.som.ni",   "NumberIteration");
    SetParameterDescription("model.som.ni", "Number of iterations for SOM learning");
    MandatoryOff("model.som.ni");

    AddParameter(ParameterType_Float,  "model.som.bi",   "BetaInit");
    SetParameterDescription("model.som.bi", "Initial learning coefficient");
    MandatoryOff("model.som.bi");

    AddParameter(ParameterType_Float,  "model.som.bf",   "BetaFinal");
    SetParameterDescription("model.som.bf", "Final learning coefficient");
    MandatoryOff("model.som.bf");

    AddParameter(ParameterType_Float,  "model.som.iv",   "InitialValue");
    SetParameterDescription("model.som.iv", "Maximum initial neuron weight");
    MandatoryOff("model.som.iv");
    
    SetDefaultParameterInt("model.som.sx", 32);
    SetDefaultParameterInt("model.som.sy", 32);
    SetDefaultParameterInt("model.som.nx", 10);
    SetDefaultParameterInt("model.som.ny", 10);
    SetDefaultParameterInt("model.som.ni", 5);
    SetDefaultParameterFloat("model.som.bi", 1.0);
    SetDefaultParameterFloat("model.som.bf", 0.1);
    SetDefaultParameterFloat("model.som.iv", 10.0);

 
}

template <class TInputValue, class TOutputValue>
void
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::BeforeTrainSOM(typename ListSampleType::Pointer trainingListSample,
        std::string modelPath)
{		
	int SomDim = GetParameterInt("model.som.dim");
	std::cout << SomDim << std::endl;
		
	if(SomDim == 2)
		{
		TrainSOM<SOM2DModelType >(trainingListSample,modelPath);
		}
		
	if(SomDim == 3)
		{
		TrainSOM<SOM3DModelType >(trainingListSample,modelPath);
		}
	 
	if(SomDim == 4)
		{
		TrainSOM<SOM4DModelType >(trainingListSample,modelPath);
		}
	 
	if(SomDim == 5)
		{
		TrainSOM<SOM5DModelType >(trainingListSample,modelPath);
		}   
	if(SomDim > 5 || SomDim < 2)
		{
			std::cerr << "k : invalid dimension" << std::endl;
		}
}


template <class TInputValue, class TOutputValue>
template <typename somchoice>
void cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::TrainSOM(typename ListSampleType::Pointer trainingListSample,std::string modelPath)
{
		using TemplateEstimatorType = typename somchoice::EstimatorType;
		typename somchoice::Pointer dimredTrainer = somchoice::New();
		unsigned int dim = dimredTrainer->GetDimension();
		std::cout << dim << std::endl;
		dimredTrainer->SetNumberOfIterations(GetParameterInt("model.som.ni"));
		dimredTrainer->SetBetaInit(GetParameterFloat("model.som.bi"));
		dimredTrainer->SetWriteMap(true);
		dimredTrainer->SetBetaEnd(GetParameterFloat("model.som.bf"));
		dimredTrainer->SetMaxWeight(GetParameterFloat("model.som.iv"));
		typename TemplateEstimatorType::SizeType size;
		std::vector<std::basic_string<char>> s= GetParameterStringList("model.som.s");
		for (int i=0; i<dim; i++){ 
			size[i]=std::stoi(s[i]);
		}
		
        dimredTrainer->SetMapSize(size);
        typename TemplateEstimatorType::SizeType radius;
		std::vector<std::basic_string<char>> n= GetParameterStringList("model.som.n");
		for (int i=0; i<dim; i++){ 
			radius[i]=std::stoi(n[i]);
		}
        dimredTrainer->SetNeighborhoodSizeInit(radius);
        dimredTrainer->SetListSample(trainingListSample);
		dimredTrainer->Train();
		dimredTrainer->Save(modelPath);
}

} //end namespace wrapper
} //end namespace otb

#endif
