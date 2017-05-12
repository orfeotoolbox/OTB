
#ifndef AutoencoderModel_txx
#define AutoencoderModel_txx

#include <fstream>
#include <shark/Data/Dataset.h>
#include "itkMacro.h"
#include "otbSharkUtils.h"
//include train function
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/Algorithms/GradientDescent/Rprop.h>// the RProp optimization algorithm
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h> // squared loss used for regression
#include <shark/ObjectiveFunctions/Regularizer.h> //L2 regulariziation

namespace otb
{


template <class TInputValue, class AutoencoderType>
AutoencoderModel<TInputValue,AutoencoderType>::AutoencoderModel()
{
	this->m_IsRegressionSupported = true;
}


template <class TInputValue, class AutoencoderType>
AutoencoderModel<TInputValue,AutoencoderType>::~AutoencoderModel()
{
}


template <class TInputValue, class AutoencoderType>
void AutoencoderModel<TInputValue,AutoencoderType>::Train()
{
	
	std::vector<shark::RealVector> features;
	
	Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
	
	shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );
		
	std::size_t inputs = dataDimension(inputSamples);
	m_net.setStructure(inputs, m_NumberOfHiddenNeurons);
	initRandomUniform(m_net,-0.1*std::sqrt(1.0/inputs),0.1*std::sqrt(1.0/inputs));
	
	shark::LabeledData<shark::RealVector,shark::RealVector> trainSet(inputSamples,inputSamples);//labels identical to inputs
	shark::SquaredLoss<shark::RealVector> loss;
	shark::ErrorFunction error(trainSet, &m_net, &loss);
	shark::TwoNormRegularizer regularizer(error.numberOfVariables());
	error.setRegularizer(m_Regularization,&regularizer);

	shark::IRpropPlusFull optimizer;
	error.init();
	optimizer.init(error);
	std::cout<<"Optimizing model: "+m_net.name()<<std::endl;
	for(std::size_t i = 0; i != m_NumberOfIterations; ++i){
		optimizer.step(error);
		std::cout<<i<<" "<<optimizer.solution().value<<std::endl;
	}
	//std::cout<<optimizer.solution().value<<std::endl;
	m_net.setParameterVector(optimizer.solution().point);
	
	
}


template <class TInputValue, class AutoencoderType>
bool AutoencoderModel<TInputValue,AutoencoderType>::CanReadFile(const std::string & filename)
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


template <class TInputValue, class AutoencoderType>
bool AutoencoderModel<TInputValue,AutoencoderType>::CanWriteFile(const std::string & filename)
{
	return true;
}

template <class TInputValue, class AutoencoderType>
void AutoencoderModel<TInputValue,AutoencoderType>::Save(const std::string & filename, const std::string & name)
{
	std::ofstream ofs(filename);
	ofs << m_net.name() << std::endl; //first line
	boost::archive::polymorphic_text_oarchive oa(ofs);
	m_net.write(oa);
	ofs.close();
}

template <class TInputValue, class AutoencoderType>
void AutoencoderModel<TInputValue,AutoencoderType>::Load(const std::string & filename, const std::string & name)
{
	std::ifstream ifs(filename);
	char autoencoder[256];
	ifs.getline(autoencoder,256); 
	std::string autoencoderstr(autoencoder);
	
	if (autoencoderstr != m_net.name()){
		itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
	boost::archive::polymorphic_text_iarchive ia(ifs);
	m_net.read(ia);
	ifs.close();
	m_NumberOfHiddenNeurons = m_net.numberOfHiddenNeurons();
	//this->m_Size = m_NumberOfHiddenNeurons;
}


template <class TInputValue, class AutoencoderType>
typename AutoencoderModel<TInputValue,AutoencoderType>::TargetSampleType
AutoencoderModel<TInputValue,AutoencoderType>::DoPredict(const InputSampleType & value, ConfidenceValueType *quality) const
{  
	shark::RealVector samples(value.Size());
	for(size_t i = 0; i < value.Size();i++)
    {
		samples.push_back(value[i]);
    }
    shark::Data<shark::RealVector> data;
    data.element(0)=samples;
    data = m_net.encode(data);
    
    TargetSampleType target;
    
    //target.SetSize(m_NumberOfHiddenNeurons);
	for(unsigned int a = 0; a < m_NumberOfHiddenNeurons; ++a){
		//target[a]=data.element(0)[a];
		target=data.element(0)[a];
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
	data = m_net.encode(data);
	unsigned int id = startIndex;
	for(const auto& p : data.elements()){
		
		for(unsigned int a = 0; a < m_NumberOfHiddenNeurons; ++a){
			//target[a]=p[a];
			target=p[a];
		}
		std::cout << p << std::endl;
		targets->SetMeasurementVector(id,target);
		++id;
    }
}


} // namespace otb
#endif
