
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

namespace otb
{


template <class TInputValue, class AutoencoderType>
AutoencoderModel<TInputValue,AutoencoderType>::AutoencoderModel()
{
	this->m_IsDoPredictBatchMultiThreaded = true;
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
	
	for (unsigned int i = 0 ; i < m_NumberOfHiddenNeurons.Size(); ++i)
	{
		TrainOneLayer( m_NumberOfHiddenNeurons[i], inputSamples);
	}
}

template <class TInputValue, class AutoencoderType>
void AutoencoderModel<TInputValue,AutoencoderType>::TrainOneLayer(unsigned int nbneuron, shark::Data<shark::RealVector> &samples)
{
	AutoencoderType net;
	/*std::vector<shark::RealVector> features;
	
	Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
	
	shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );
		*/ //in Train()  now
	std::size_t inputs = dataDimension(samples);
	net.setStructure(inputs, nbneuron);
	initRandomUniform(net,-0.1*std::sqrt(1.0/inputs),0.1*std::sqrt(1.0/inputs));
	shark::ImpulseNoiseModel noise(m_Noise,0.0); //set an input pixel with probability m_Noise to 0
	shark::ConcatenatedModel<shark::RealVector,shark::RealVector> model = noise>> net;
	shark::LabeledData<shark::RealVector,shark::RealVector> trainSet(samples,samples);//labels identical to inputs
	shark::SquaredLoss<shark::RealVector> loss;
	shark::ErrorFunction error(trainSet, &model, &loss);
	//shark::SparseAutoencoderError error(data,&model, &loss, m_rho, m_beta);
	shark::TwoNormRegularizer regularizer(error.numberOfVariables());
	error.setRegularizer(m_Regularization,&regularizer);

	shark::IRpropPlusFull optimizer;
	error.init();
	optimizer.init(error);
	std::cout<<"Optimizing model: "+net.name()<<std::endl;
	for(std::size_t i = 0; i != m_NumberOfIterations; ++i){
		optimizer.step(error);
		std::cout<<i<<" "<<optimizer.solution().value<<std::endl;
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
	
}


template <class TInputValue, class AutoencoderType>
typename AutoencoderModel<TInputValue,AutoencoderType>::TargetSampleType
AutoencoderModel<TInputValue,AutoencoderType>::DoPredict(const InputSampleType & value) const
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
::DoPredictBatch(const InputListSampleType *input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * targets) const
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
