/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef AutoencoderModel_txx
#define AutoencoderModel_txx

#include "otbAutoencoderModel.h"


#include <fstream>
#include "itkMacro.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
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

#include <shark/Algorithms/GradientDescent/SteepestDescent.h>
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

namespace otb
{

template <class TInputValue, class NeuronType>
AutoencoderModel<TInputValue,NeuronType>::AutoencoderModel()
{
	this->m_IsDoPredictBatchMultiThreaded = true;
	this->m_WriteLearningCurve = false;
}

   
template <class TInputValue, class NeuronType>
AutoencoderModel<TInputValue,NeuronType>::~AutoencoderModel()
{
}

template <class TInputValue, class NeuronType>
void AutoencoderModel<TInputValue,NeuronType>::Train()
{
	std::vector<shark::RealVector> features;
	Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
	shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );
	shark::Data<shark::RealVector> inputSamples_copy = inputSamples;
	
	std::ofstream ofs;
	if (this->m_WriteLearningCurve == true)
	{
		ofs.open(m_LearningCurveFileName);
		ofs << "learning curve" << std::endl; 
	}
	
	/// Initialization of the feed forward neural network
	std::vector<size_t> layers;
	layers.push_back(shark::dataDimension(inputSamples));
	for (unsigned int i = 0 ; i < m_NumberOfHiddenNeurons.Size(); ++i)
	{
		layers.push_back(m_NumberOfHiddenNeurons[i]);
	}
	
	for (unsigned int i = std::max(0,static_cast<int>(m_NumberOfHiddenNeurons.Size()-1)) ; i > 0; --i)
	{
		std::cout << i << std::endl;
		layers.push_back(m_NumberOfHiddenNeurons[i-1]);
	}
	
	layers.push_back(shark::dataDimension(inputSamples));
	m_net.setStructure(layers);
	shark::initRandomNormal(m_net,0.1);


	/// Training of the first Autoencoder (first and last layer of the FF network)
	if (m_Epsilon > 0){
        shark::TrainingProgress<> criterion(5,m_Epsilon);
		
        OutAutoencoderType net;
        if (m_Noise[0] != 0)   // Shark doesn't allow to train a layer using a sparsity term AND a noisy input. (shark::SparseAutoencoderError takes an autoen
          {
          TrainOneLayer(criterion,net,0 , m_NumberOfHiddenNeurons[0],m_Noise[0],m_Regularization[0], inputSamples,ofs);
          }
        else
          {
          TrainOneSparseLayer( criterion, net , 0 , m_NumberOfHiddenNeurons[0],m_Rho[0],m_Beta[0],m_Regularization[0],inputSamples, ofs);
          }
        criterion.reset();
	}
	
	else {
		shark::MaxIterations<> criterion(m_NumberOfIterations);

		OutAutoencoderType net;
		if (m_Noise[0] != 0)   // Shark doesn't allow to train a layer using a sparsity term AND a noisy input. (shark::SparseAutoencoderError takes an autoen
		{
			TrainOneLayer(criterion,net,0, m_NumberOfHiddenNeurons[0],m_Noise[0],m_Regularization[0], inputSamples, ofs);
			std::cout << "mnoise " << m_Noise[0] << std::endl;
		}
		else
		{
			TrainOneSparseLayer(criterion, net, 0, m_NumberOfHiddenNeurons[0],m_Rho[0],m_Beta[0],m_Regularization[0], inputSamples, ofs);
		}
		criterion.reset();
	}


	/// Training of the other autoencoders 
	if (m_Epsilon > 0){
		shark::TrainingProgress<> criterion(5,m_Epsilon);
		
		for (unsigned int i = 1 ; i < m_NumberOfHiddenNeurons.Size(); ++i)
		{
			AutoencoderType net;
			if (m_Noise[i] != 0)   // Shark doesn't allow to train a layer using a sparsity term AND a noisy input. (shark::SparseAutoencoderError takes an autoen
			{
				TrainOneLayer(criterion,net,i , m_NumberOfHiddenNeurons[i],m_Noise[i],m_Regularization[i], inputSamples,ofs);
			}
			else
			{
				TrainOneSparseLayer( criterion, net , i , m_NumberOfHiddenNeurons[i],m_Rho[i],m_Beta[i],m_Regularization[i],inputSamples, ofs);
			}
			criterion.reset();
		}

	}
	
	else {
		shark::MaxIterations<> criterion(m_NumberOfIterations);
		
		for (unsigned int i = 1 ; i < m_NumberOfHiddenNeurons.Size(); ++i)
		{
			AutoencoderType net;
			if (m_Noise[i] != 0)   // Shark doesn't allow to train a layer using a sparsity term AND a noisy input. (shark::SparseAutoencoderError takes an autoen
			{
				TrainOneLayer(criterion,net, i,m_NumberOfHiddenNeurons[i],m_Noise[i],m_Regularization[i], inputSamples, ofs);
				std::cout << "mnoise " << m_Noise[i] << std::endl;
			}
			else
			{
				TrainOneSparseLayer(criterion, net, i, m_NumberOfHiddenNeurons[i],m_Rho[i],m_Beta[i],m_Regularization[i], inputSamples, ofs);
			}
			criterion.reset();
		}
		
	}
	if (m_NumberOfIterationsFineTuning > 0)
	{
		shark::MaxIterations<> criterion(m_NumberOfIterationsFineTuning);
		TrainNetwork(criterion, m_Rho[0],m_Beta[0],m_Regularization[0], inputSamples_copy, ofs);
	}		
}

template <class TInputValue, class NeuronType>
template <class T, class Autoencoder>
void AutoencoderModel<TInputValue,NeuronType>::TrainOneLayer(shark::AbstractStoppingCriterion<T> & criterion, Autoencoder & net,unsigned int layer_index, unsigned int nbneuron,double noise_strength,double regularization, shark::Data<shark::RealVector> &samples, std::ostream& File)
{
	//AutoencoderType net;
	std::cout << "noise " <<  noise_strength << std::endl;
	std::size_t inputs = dataDimension(samples);
	net.setStructure(inputs, nbneuron);
	initRandomUniform(net,-m_InitFactor*std::sqrt(1.0/inputs),m_InitFactor*std::sqrt(1.0/inputs));
	
	//initRandomUniform(net,-1,1);
	shark::ImpulseNoiseModel noise(inputs,noise_strength,1.0); //set an input pixel with probability m_Noise to 0
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
	if (this->m_WriteLearningCurve == true)
	{
		File << "end layer" << std::endl;
	}
	
	unsigned int i=0;
	do{
		i++;
		optimizer.step(error);
		if (this->m_WriteLearningCurve == true)
		{	
		File << optimizer.solution().value << std::endl;
		}
		std::cout<<"error after " << i << "iterations : " << optimizer.solution().value<<  std::endl ;
	
	} while( !criterion.stop( optimizer.solution() ) );
	
	net.setParameterVector(optimizer.solution().point);
	//m_net.push_back(net);
	m_net.setLayer(layer_index,net.encoderMatrix(),net.hiddenBias());  // Copy the encoder in the FF neural network
	m_net.setLayer( m_NumberOfHiddenNeurons.Size()*2 - 1 - layer_index,net.decoderMatrix(),net.outputBias()); // Copy the decoder in the FF neural network
	samples = net.encode(samples);
}


template <class TInputValue, class NeuronType>
template <class T, class Autoencoder>
void AutoencoderModel<TInputValue,NeuronType>::TrainOneSparseLayer(shark::AbstractStoppingCriterion<T> & criterion, Autoencoder & net, unsigned int layer_index, unsigned int nbneuron,double rho,double beta, double regularization, shark::Data<shark::RealVector> &samples, std::ostream& File)
{
	//AutoencoderType net;
	std::size_t inputs = dataDimension(samples);
	net.setStructure(inputs, nbneuron);
	
	shark::initRandomUniform(net,-m_InitFactor*std::sqrt(1.0/inputs),m_InitFactor*std::sqrt(1.0/inputs));
	
	/*   Idea : set the initials value for the output weights higher than the input weights
	auto weights = net.parameterVector();
	
	for(unsigned int i=net.inputSize()*net.numberOfHiddenNeurons(); i< (net.inputSize()+net.outputSize())*net.numberOfHiddenNeurons(); i++ )
	{
		 weights(i) *= 100;
		 std::cout << weights(i) << std::endl;
	}
	net.setParameterVector(weights);
	std::cout << "dec" << net.decoderMatrix()<< std::endl;
	*/
	
	
	
	//std::cout << "initial seed" << net.parameterVector() << std::endl;
	//initRandomUniform(net,-1,1);
	shark::LabeledData<shark::RealVector,shark::RealVector> trainSet(samples,samples);//labels identical to inputs
	shark::SquaredLoss<shark::RealVector> loss;
	shark::SparseAutoencoderError error(trainSet,&net, &loss, rho, beta);
	
	shark::TwoNormRegularizer regularizer(error.numberOfVariables());
	error.setRegularizer(regularization,&regularizer);
	std::cout << samples.element(0) << std::endl;
	/*shark::SteepestDescent optimizer;
	error.init();
	optimizer.init(error);
	optimizer.setLearningRate(0.01);
	std::cout << optimizer.learningRate() << std::endl;
	*/
	shark::IRpropPlusFull optimizer;
	error.init();
	optimizer.init(error);
	
	
	std::cout<<"error before training : " << optimizer.solution().value<<std::endl;
	unsigned int i=0;
	do{
		
		i++;
		optimizer.step(error);
		std::cout<<"error after " << i << "iterations : " << optimizer.solution().value <<std::endl;
		if (this->m_WriteLearningCurve == true) 
		{	
		File << optimizer.solution().value << std::endl;
		}
	} while( !criterion.stop( optimizer.solution() ) );
	if (this->m_WriteLearningCurve == true)
	{
		File << "end layer" << std::endl;
	}
	net.setParameterVector(optimizer.solution().point);
	//m_net.push_back(net);
	m_net.setLayer(layer_index,net.encoderMatrix(),net.hiddenBias());  // Copy the encoder in the FF neural network
	m_net.setLayer( m_NumberOfHiddenNeurons.Size()*2 - 1 - layer_index,net.decoderMatrix(),net.outputBias()); // Copy the decoder in the FF neural network
	samples = net.encode(samples);
	
}


template <class TInputValue, class NeuronType>
template <class T>
void AutoencoderModel<TInputValue,NeuronType>::TrainNetwork(shark::AbstractStoppingCriterion<T> & criterion,double /*rho*/,double /*beta*/, double regularization, shark::Data<shark::RealVector> &samples, std::ostream& File)
{
	
	shark::LabeledData<shark::RealVector,shark::RealVector> trainSet(samples,samples);//labels identical to inputs
	shark::SquaredLoss<shark::RealVector> loss;
	
	shark::ErrorFunction error(trainSet, &m_net, &loss);
	shark::TwoNormRegularizer regularizer(error.numberOfVariables());
	error.setRegularizer(regularization,&regularizer);

	shark::IRpropPlusFull optimizer;
	error.init();
	optimizer.init(error);
	std::cout<<"error before training : " << optimizer.solution().value<<std::endl;
	unsigned int i=0;
	while( !criterion.stop( optimizer.solution() ) )
	{
		i++;
		optimizer.step(error);
		std::cout<<"error after " << i << "iterations : " << optimizer.solution().value<<std::endl;
		if (this->m_WriteLearningCurve == true)
		{	
			File << optimizer.solution().value << std::endl;
		}
	} 
	//std::cout<<"error after " << i << "iterations : " << optimizer.solution().value<<std::endl;
}



template <class TInputValue, class NeuronType>
bool AutoencoderModel<TInputValue,NeuronType>::CanReadFile(const std::string & filename)
{
	try
	{
		this->Load(filename);
		m_net.name();
	}
	catch(...)
	{
	return false;
	}
	return true;
}


template <class TInputValue, class NeuronType>
bool AutoencoderModel<TInputValue,NeuronType>::CanWriteFile(const std::string & /*filename*/)
{
	return true;
}

template <class TInputValue, class NeuronType>
void AutoencoderModel<TInputValue,NeuronType>::Save(const std::string & filename, const std::string & /*name*/)
{
	std::cout << "saving model ..." << std::endl;
	std::ofstream ofs(filename);
	ofs << m_net.name() << std::endl; // the first line of the model file contains a key
	boost::archive::polymorphic_text_oarchive oa(ofs);
	//m_net.write(oa);
	oa << m_net;
	ofs.close();
	
	
	if (this->m_WriteWeights == true)     // output the map vectors in a txt file
	{
		std::ofstream otxt(filename+".txt");
		
		
		for (unsigned int i = 0 ; i < m_net.layerMatrices().size(); ++i)
		{
			otxt << "layer " << i << std::endl;
			otxt << m_net.layerMatrix(i) << std::endl;
			otxt << m_net.bias(i) << std::endl;
			otxt << std::endl;
		}
		
	/*	
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
		
		*/
	}
	
	
	
}

template <class TInputValue, class NeuronType>
void AutoencoderModel<TInputValue,NeuronType>::Load(const std::string & filename, const std::string & /*name*/)
{
	
	NetworkType net;
	std::ifstream ifs(filename);
	char autoencoder[256];
	ifs.getline(autoencoder,256); 
	std::string autoencoderstr(autoencoder);
	
	std::cout << autoencoderstr << std::endl;
	if (autoencoderstr != net.name()){
		itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
	boost::archive::polymorphic_text_iarchive ia(ifs);
	//m_net.read(ia);
	ia >> m_net;
	ifs.close();
	
	// This gives us the dimension if we keep the encoder and decoder
	size_t feature_layer_index = m_net.layerMatrices().size()/2; 
	this->m_Dimension = m_net.layerMatrix(feature_layer_index).size1(); // number of neurons in the feature layer (first dimension of the first decoder weight matrix)
	std::cout << this->m_Dimension << std::endl;
	
	
	
	/* This might not be needed
	int number_of_layers = m_net.layerMatrices().size()/2;
	std::cout << "number of layers" << number_of_layers << std::endl;
	m_NumberOfHiddenNeurons.SetSize(number_of_layers);
	for (int i=0; i<number_of_layers; i++){ 
		m_NumberOfHiddenNeurons[i] = m_net[i].numberOfHiddenNeurons();
	}
	this->m_Dimension = m_NumberOfHiddenNeurons[number_of_layers-1];
	*/
}


template <class TInputValue, class NeuronType>
typename AutoencoderModel<TInputValue,NeuronType>::TargetSampleType
AutoencoderModel<TInputValue,NeuronType>::DoPredict(const InputSampleType & value, ConfidenceValueType * /*quality*/) const
{  
	
	shark::RealVector samples(value.Size());
	for(size_t i = 0; i < value.Size();i++)
    {
		samples[i]=value[i];
    }
    
    std::vector<shark::RealVector> features;
    features.push_back(samples);
   
    shark::Data<shark::RealVector> data = shark::createDataFromRange(features);
    
    data = m_net.evalLayer( m_net.layerMatrices().size()/2-1 ,data); // features layer for a network containing the encoder and decoder part
    /*
	for (int i=0; i<m_net.size(); i++){ // loop over all autoencoders in m_net
		data = m_net[i].encode(data);    
	}*/
    TargetSampleType target;
    target.SetSize(this->m_Dimension);
	
	for(unsigned int a = 0; a < this->m_Dimension; ++a){
		target[a]=data.element(0)[a];
	}
	return target;
	
}


template <class TInputValue, class NeuronType>
void AutoencoderModel<TInputValue,NeuronType>
::DoPredictBatch(const InputListSampleType *input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * targets, ConfidenceListSampleType * /*quality*/) const
{
	
	std::vector<shark::RealVector> features;
	Shark::ListSampleRangeToSharkVector(input, features,startIndex,size);
	shark::Data<shark::RealVector> data = shark::createDataFromRange(features);
	TargetSampleType target;
	/*
	for (auto net :m_net ){ // loop over all autoencoders in m_net
		data = net.encode(data);    
	}
	*/
	data = m_net.evalLayer( m_net.layerMatrices().size()/2-1 ,data);   // features layer for a network containing the encoder and decoder part
	
	unsigned int id = startIndex;
	target.SetSize(this->m_Dimension);
	
	for(const auto& p : data.elements())
	{
		for(unsigned int a = 0; a < this->m_Dimension; ++a){
			target[a]=p[a];
		}
		targets->SetMeasurementVector(id,target);
		++id;	
    }
    
}

} // namespace otb
#endif
