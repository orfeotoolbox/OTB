
#ifndef PCAModel_txx
#define PCAModel_txx

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
	this->m_IsDoPredictBatchMultiThreaded = true;
}


template <class TInputValue>
PCAModel<TInputValue>::~PCAModel()
{
}


template <class TInputValue>
void PCAModel<TInputValue>::Train()
{
	
	std::vector<shark::RealVector> features;
	
	Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
	
	shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );
	m_pca.setData(inputSamples);
	m_pca.encoder(m_encoder, m_Dimension);
	m_pca.decoder(m_decoder, m_Dimension);
	
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
	//ofs << m_encoder.name() << std::endl; //first line
	ofs << "pca" << std::endl; //first line
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
	
	//if (encoderstr != m_encoder.name()){
	if (encoderstr != "pca"){
		itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
	boost::archive::polymorphic_text_iarchive ia(ifs);
	m_encoder.read(ia);
	ifs.close();
	m_Dimension = m_encoder.outputSize();
	//this->m_Size = m_NumberOfHiddenNeurons;
}


template <class TInputValue>
typename PCAModel<TInputValue>::TargetSampleType
PCAModel<TInputValue>::DoPredict(const InputSampleType & value) const
{  
	shark::RealVector samples(value.Size());
	for(size_t i = 0; i < value.Size();i++)
    {
		samples[i]=value[i];
    }
    
    std::vector<shark::RealVector> features;
    features.push_back(samples);
   
    shark::Data<shark::RealVector> data = shark::createDataFromRange(features);
     
	data = m_encoder(data);
    TargetSampleType target;
    target.SetSize(m_Dimension);
	
	for(unsigned int a = 0; a < m_Dimension; ++a){
		target[a]=data.element(0)[a];
	}
	return target;
}


template <class TInputValue>
void PCAModel<TInputValue>
::DoPredictBatch(const InputListSampleType *input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * targets) const
{
	
	std::vector<shark::RealVector> features;
	Shark::ListSampleRangeToSharkVector(input, features,startIndex,size);
	shark::Data<shark::RealVector> data = shark::createDataFromRange(features);
	TargetSampleType target;
	data = m_encoder(data);
	unsigned int id = startIndex;
	target.SetSize(m_Dimension);
	for(const auto& p : data.elements()){
		
		for(unsigned int a = 0; a < m_Dimension; ++a){
			target[a]=p[a];
			//target[a]=1;
		
			//target.SetElement(a,p[a]);
			
			
		}
		//std::cout << p << std::endl;
		targets->SetMeasurementVector(id,target);
		++id;
		
    }

}


} // namespace otb
#endif
