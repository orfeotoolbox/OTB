#ifndef AutoencoderModel_txx
#define AutoencoderModel_txx

#include <fstream>
#include <shark/Data/Dataset.h>
#include "itkMacro.h"
#include "otbSharkUtils.h"

//include train function
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/ObjectiveFunctions/SparseAutoencoderError.h>//the error function performing the regularisation of the hidden neurons

#include <shark/Algorithms/GradientDescent/Rprop.h>// the RProp optimization algorithm
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h> // squared loss used for regression
#include <shark/ObjectiveFunctions/Regularizer.h> //L2 regulariziation
#include <shark/Models/ImpulseNoiseModel.h> //noise source to corrupt the inputs
#include <shark/Models/ConcatenatedModel.h>//to concatenate the noise with the model

#include <shark/Algorithms/StoppingCriteria/MaxIterations.h> //A simple stopping criterion that stops after a fixed number of iterations
#include <shark/Algorithms/StoppingCriteria/TrainingProgress.h> //Stops when the algorithm seems to converge, Tracks the progress of the training error over a period of time
namespace otb
{

template <class TInputValue, class AutoencoderType>
AutoencoderModel<TInputValue,AutoencoderType>::AutoencoderModel()
{
	this->m_IsDoPredictBatchMultiThreaded = true;
	this->m_WriteLearningCurve = false;
}

   
template <class TInputValue, class AutoencoderType>
AutoencoderModel<TInputValue,AutoencoderType>::~AutoencoderModel()
{
}

template <class TInputValue, class AutoencoderType>
void AutoencoderModel<TInputValue,AutoencoderType>::Train()
{
	std::cout << this->m_WriteLearningCurve << std::endl;
	std::vector<shark::RealVector> features;
	std::cout << "converting the input ListSample to Shark vector" << std::endl;
	Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
	std::cout << "creating the data vector" << std::endl;
	shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );
	
	
	std::ofstream ofs;
	if (this->m_WriteLearningCurve =true) 
	{
		ofs.open(m_LearningCurveFileName);
		ofs << "learning curve" << std::endl; 
	}
	/*
	std::ofstream ofs("/mnt/data/home/traizetc/computation/learning_curve.txt"); //learning curve
	ofs << "learning curve" << std::endl; 
	*/
	if (m_Epsilon > 0){
		shark::TrainingProgress<> criterion(5,m_Epsilon);
		
		for (unsigned int i = 0 ; i < m_NumberOfHiddenNeurons.Size(); ++i)
		{
			if (m_Noise[i] != 0)   // Shark doesn't allow to train a layer using a sparsity term AND a noisy input. (shark::SparseAutoencoderError takes an autoen
			{
				TrainOneLayer(criterion, m_NumberOfHiddenNeurons[i],m_Noise[i],m_Regularization[i], inputSamples,ofs);
			}
			else
			{
				TrainOneSparseLayer( criterion,m_NumberOfHiddenNeurons[i],m_Rho[i],m_Beta[i],m_Regularization[i],inputSamples, ofs);
			}
			criterion.reset();
		}

	}
	
	else {
		shark::MaxIterations<> criterion(m_NumberOfIterations);
		
		for (unsigned int i = 0 ; i < m_NumberOfHiddenNeurons.Size(); ++i)
		{
			if (m_Noise[i] != 0)   // Shark doesn't allow to train a layer using a sparsity term AND a noisy input. (shark::SparseAutoencoderError takes an autoen
			{
				TrainOneLayer(criterion, m_NumberOfHiddenNeurons[i],m_Noise[i],m_Regularization[i], inputSamples, ofs);
			}
			else
			{
				TrainOneSparseLayer(criterion, m_NumberOfHiddenNeurons[i],m_Rho[i],m_Beta[i],m_Regularization[i], inputSamples, ofs);
			}
			criterion.reset();
		}
		
	}
}

template <class TInputValue, class AutoencoderType>
template <class T>
void AutoencoderModel<TInputValue,AutoencoderType>::TrainOneLayer(shark::AbstractStoppingCriterion<T> & criterion, unsigned int nbneuron,double noise_strength,double regularization, shark::Data<shark::RealVector> &samples, std::ostream& File)
{
	AutoencoderType net;

	std::size_t inputs = dataDimension(samples);
	net.setStructure(inputs, nbneuron);
	//initRandomUniform(net,-0.1*std::sqrt(1.0/inputs),0.1*std::sqrt(1.0/inputs));
	initRandomUniform(net,-1,1);
	shark::ImpulseNoiseModel noise(noise_strength,0.0); //set an input pixel with probability m_Noise to 0
	shark::ConcatenatedModel<shark::RealVector,shark::RealVector> model = noise>> net;
	shark::LabeledData<shark::RealVector,shark::RealVector> trainSet(samples,samples);//labels identical to inputs
	shark::SquaredLoss<shark::RealVector> loss;
	shark::ErrorFunction error(trainSet, &model, &loss);

	shark::TwoNormRegularizer regularizer(error.numberOfVariables());
	error.setRegularizer(regularization,&regularizer);

	shark::IRpropPlusFull optimizer;
	error.init();
	optimizer.init(error);
	
	std::cout<<"error before training : " << optimizer.solution().value<<std::endl;
	if (this->m_WriteLearningCurve =true) 
	{
		File << "end layer" << std::endl;
	}
	
	unsigned int i=0;
	do{
		i++;
		optimizer.step(error);
		if (this->m_WriteLearningCurve =true) 
		{	
		File << optimizer.solution().value << std::endl;
		}
	} while( !criterion.stop( optimizer.solution() ) );
	std::cout<<"error after " << i << "iterations : " << optimizer.solution().value<<std::endl;
	
	net.setParameterVector(optimizer.solution().point);
	m_net.push_back(net);
	samples = net.encode(samples);
}


template <class TInputValue, class AutoencoderType>
template <class T>
void AutoencoderModel<TInputValue,AutoencoderType>::TrainOneSparseLayer(shark::AbstractStoppingCriterion<T> & criterion, unsigned int nbneuron,double rho,double beta, double regularization, shark::Data<shark::RealVector> &samples, std::ostream& File)
{
	AutoencoderType net;

	std::size_t inputs = dataDimension(samples);
	net.setStructure(inputs, nbneuron);
	//initRandomUniform(net,-0.1*std::sqrt(1.0/inputs),0.1*std::sqrt(1.0/inputs));
	initRandomUniform(net,-1,1);
	shark::LabeledData<shark::RealVector,shark::RealVector> trainSet(samples,samples);//labels identical to inputs
	shark::SquaredLoss<shark::RealVector> loss;
	shark::SparseAutoencoderError error(trainSet,&net, &loss, rho, beta);
	
	shark::TwoNormRegularizer regularizer(error.numberOfVariables());
	error.setRegularizer(regularization,&regularizer);

	shark::IRpropPlusFull optimizer;
	error.init();
	optimizer.init(error);
	std::cout<<"error before training : " << optimizer.solution().value<<std::endl;
	unsigned int i=0;
	do{
		i++;
		optimizer.step(error);
		if (this->m_WriteLearningCurve =true) 
		{	
		File << optimizer.solution().value << std::endl;
		}
	} while( !criterion.stop( optimizer.solution() ) );
	std::cout<<"error after " << i << "iterations : " << optimizer.solution().value<<std::endl;
	if (this->m_WriteLearningCurve =true) 
	{
		File << "end layer" << std::endl;
	}
	net.setParameterVector(optimizer.solution().point);
	m_net.push_back(net);
	samples = net.encode(samples);
}

template <class TInputValue, class AutoencoderType>
bool AutoencoderModel<TInputValue,AutoencoderType>::CanReadFile(const std::string & filename)
{
	try
	{
		this->Load(filename);
		m_net[0].name();
	}
	catch(...)
	{
	return false;
	}
	return true;
}


template <class TInputValue, class AutoencoderType>
bool AutoencoderModel<TInputValue,AutoencoderType>::CanWriteFile(const std::string & filename)
{
	return true;
}

template <class TInputValue, class AutoencoderType>
void AutoencoderModel<TInputValue,AutoencoderType>::Save(const std::string & filename, const std::string & name)
{
	std::ofstream ofs(filename);
	ofs << m_net[0].name() << std::endl; // the first line of the model file contains a key
	boost::archive::polymorphic_text_oarchive oa(ofs);
	//m_net.write(oa);
	oa << m_net;
	ofs.close();
	
	
	if (this->m_WriteWeights == true)     // output the map vectors in a txt file
	{
		std::ofstream otxt(filename+".txt");
		
		for (unsigned int i = 0 ; i < m_NumberOfHiddenNeurons.Size(); ++i)
		{
			otxt << m_net[i].encoderMatrix() << std::endl;
			otxt << m_net[i].hiddenBias() << std::endl;
		}
		
		
		std::vector<shark::RealVector> features;
	
		shark::SquaredLoss<shark::RealVector> loss;
		Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
		shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );
		shark::Data<shark::RealVector> outputSamples = inputSamples;
		
		for (unsigned int i = 0 ; i < m_NumberOfHiddenNeurons.Size(); ++i)
		{
			outputSamples = m_net[i].encode(outputSamples);
		}
		
		for (unsigned int i = 0 ; i < m_NumberOfHiddenNeurons.Size(); ++i)
		{
			outputSamples = m_net[m_NumberOfHiddenNeurons.Size()-i-1].decode(outputSamples);   // We decode the data starting from the smallest layer 
		}
		otxt << "Reconstruction error : " << loss.eval(inputSamples,outputSamples) << std::endl; // the mean squared error is returned
		std::cout << "Reconstruction error : " << loss.eval(inputSamples,outputSamples) << std::endl; // the mean squared error is returned
		
		std::cout << "in" << inputSamples.element(0) << std::endl;
		std::cout << "out" << outputSamples.element(0) << std::endl;
		
		otxt.close();
		
		
	}
	
	
	
}

template <class TInputValue, class AutoencoderType>
void AutoencoderModel<TInputValue,AutoencoderType>::Load(const std::string & filename, const std::string & name)
{
	AutoencoderType net;
	std::ifstream ifs(filename);
	char autoencoder[256];
	ifs.getline(autoencoder,256); 
	std::string autoencoderstr(autoencoder);
	if (autoencoderstr != net.name()){
		itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
	boost::archive::polymorphic_text_iarchive ia(ifs);
	//m_net.read(ia);
	ia >> m_net;
	ifs.close();

	m_NumberOfHiddenNeurons.SetSize(m_net.size());
	for (int i=0; i<m_net.size(); i++){ 
		m_NumberOfHiddenNeurons[i] = m_net[i].numberOfHiddenNeurons();
	}
	this->m_Dimension = m_NumberOfHiddenNeurons[m_net.size()-1];
}


template <class TInputValue, class AutoencoderType>
typename AutoencoderModel<TInputValue,AutoencoderType>::TargetSampleType
AutoencoderModel<TInputValue,AutoencoderType>::DoPredict(const InputSampleType & value, ConfidenceValueType * quality) const
{  
	shark::RealVector samples(value.Size());
	for(size_t i = 0; i < value.Size();i++)
    {
		samples[i]=value[i];
    }
    
    std::vector<shark::RealVector> features;
    features.push_back(samples);
   
    shark::Data<shark::RealVector> data = shark::createDataFromRange(features);
     	
	for (int i=0; i<m_net.size(); i++){ // loop over all autoencoders in m_net
		data = m_net[i].encode(data);    
	}
    TargetSampleType target;
    target.SetSize(m_NumberOfHiddenNeurons[m_net.size()-1]);
	
	for(unsigned int a = 0; a < m_NumberOfHiddenNeurons[m_net.size()-1]; ++a){
		target[a]=data.element(0)[a];
	}
	return target;
	
}


template <class TInputValue, class AutoencoderType>
void AutoencoderModel<TInputValue,AutoencoderType>
::DoPredictBatch(const InputListSampleType *input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * targets, ConfidenceListSampleType * quality) const
{
	std::vector<shark::RealVector> features;
	Shark::ListSampleRangeToSharkVector(input, features,startIndex,size);
	shark::Data<shark::RealVector> data = shark::createDataFromRange(features);
	TargetSampleType target;
	
	for (auto net :m_net ){ // loop over all autoencoders in m_net
		data = net.encode(data);    
	}
	
	unsigned int id = startIndex;
	target.SetSize(m_NumberOfHiddenNeurons[m_net.size()-1]);
	for(const auto& p : data.elements())
	{
		for(unsigned int a = 0; a < m_NumberOfHiddenNeurons[m_net.size()-1]; ++a){
			target[a]=p[a];
		}
		targets->SetMeasurementVector(id,target);
		++id;	
    }
}

} // namespace otb
#endif
