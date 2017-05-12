
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


template <class TInputValue>
PCAModel<TInputValue>::PCAModel()
{
	this->m_IsRegressionSupported = true;
}


template <class TInputValue>
PCAModel<TInputValue,AutoencoderType>::~PCAModel()
{
}


template <class TInputValue>
void PCAModel<TInputValue>::Train()
{
	
	std::vector<shark::RealVector> features;
	
	Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
	
	shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );
	
	m_pca(inputSamples);
	pca.encoder(m_encoder, m_Dimension);
	pca.decoder(m_decoder, m_Dimension);
	
}


template <class TInputValue>
bool PCAModel<TInputValue>::CanReadFile(const std::string & filename)
{
	try
	{
		this->Load(filename);
		m_encoder.name();
	}
	catch(...)
	{
	return false;
	}
	return true;
}


template <class TInputValue>
bool PCAModel<TInputValue>::CanWriteFile(const std::string & filename)
{
	return true;
}

template <class TInputValue>
void PCAModel<TInputValue>::Save(const std::string & filename, const std::string & name)
{
	std::ofstream ofs(filename);
	ofs << m_encoder.name() << std::endl; //first line
	boost::archive::polymorphic_text_oarchive oa(ofs);
	m_encoder.write(oa);
	ofs.close();
}

template <class TInputValue>
void PCAModel<TInputValue>::Load(const std::string & filename, const std::string & name)
{
	std::ifstream ifs(filename);
	char encoder[256];
	ifs.getline(encoder,256); 
	std::string encoderstr(encoder);
	
	if (autoencoderstr != m_encoder.name()){
		itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
	boost::archive::polymorphic_text_iarchive ia(ifs);
	m_net.read(ia);
	ifs.close();
	m_Dimension = m_encoder.outputSize();
	//this->m_Size = m_NumberOfHiddenNeurons;
}


template <class TInputValue>
typename PCAModel<TInputValue>::TargetSampleType
PCAModel<TInputValue>::DoPredict(const InputSampleType & value, ConfidenceValueType *quality) const
{  
	shark::RealVector samples(value.Size());
	for(size_t i = 0; i < value.Size();i++)
    {
		samples.push_back(value[i]);
    }
    shark::Data<shark::RealVector> data;
    data.element(0)=samples;
    data = m_encoder(data);
    
    TargetSampleType target;
    
    //target.SetSize(m_NumberOfHiddenNeurons);
	
	for(unsigned int a = 0; a < m_NumberOfHiddenNeurons; ++a){
		target[a]=p[a];
		
			//target.SetElement(a,p[a]);
			
			
	}
	return target;
}


template <class TInputValue>
void PCAModel<TInputValue>
::DoPredictBatch(const InputListSampleType *input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * targets, ConfidenceListSampleType * quality) const
{
	
	std::vector<shark::RealVector> features;
	Shark::ListSampleRangeToSharkVector(input, features,startIndex,size);
	shark::Data<shark::RealVector> data = shark::createDataFromRange(features);
	TargetSampleType target;
	data = m_encoder(data);
	unsigned int id = startIndex;
	target.SetSize(m_NumberOfHiddenNeurons);
	for(const auto& p : data.elements()){
		
		for(unsigned int a = 0; a < m_NumberOfHiddenNeurons; ++a){
			target[a]=p[a];
		
			//target.SetElement(a,p[a]);
			
			
		}
		//std::cout << p << std::endl;
		targets->SetMeasurementVector(id,target);
		++id;
		
    }

}


} // namespace otb
#endif
