
#ifndef SOMModel_txx
#define SOMModel_txx

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkMacro.h"

namespace otb
{


template <class TInputValue>
SOMModel<TInputValue>::SOMModel()
{
	this->m_IsRegressionSupported = true;
}


template <class TInputValue>
SOMModel<TInputValue>::~SOMModel()
{
}


template <class TInputValue>
void SOMModel<TInputValue>::Train()
{
	
    typename EstimatorType::Pointer estimator = EstimatorType::New();
	
    estimator->SetListSample(m_ListSample);
    estimator->SetMapSize(m_MapSize);
    estimator->SetNeighborhoodSizeInit(m_NeighborhoodSizeInit);
    estimator->SetNumberOfIterations(m_NumberOfIterations);
    estimator->SetBetaInit(m_BetaInit);
    estimator->SetBetaEnd(m_BetaEnd);
    estimator->SetMaxWeight(m_MaxWeight);
    //AddProcess(estimator,"Learning");
    
    estimator->Update();
	
    m_SOMMap = estimator->GetOutput();
}


template <class TInputValue>
bool SOMModel<TInputValue>::CanReadFile(const std::string & filename)
{
	try
	{
		this->Load(filename);
	}
	catch(...)
	{
	return false;
	}
	return true;
}


template <class TInputValue>
bool SOMModel<TInputValue>::CanWriteFile(const std::string & filename)
{
	return true;
}

template <class TInputValue>
void SOMModel<TInputValue>::Save(const std::string & filename, const std::string & name)
{
  std::cout << m_SOMMap->GetNumberOfComponentsPerPixel() << std::endl;
  
//Ecriture
  auto kwl = m_SOMMap->GetImageKeywordlist();
  //kwl.AddKey("MachineLearningModelType", "SOM");
  //m_SOMMap->SetImageKeywordList(kwl);
  auto writer = otb::ImageFileWriter<MapType>::New();
  writer->SetInput(m_SOMMap);
  writer->SetFileName(filename);
  writer->Update();

}

template <class TInputValue>
void SOMModel<TInputValue>::Load(const std::string & filename, const std::string & name)
{
	auto reader = otb::ImageFileReader<MapType>::New();
	reader->SetFileName(filename);
	reader->Update();
	//std::cout <<  reader->GetOutput()->GetImageKeywordlist().GetMetadataByKey("MachineLearningModelType") << '\n';
	m_SOMMap = reader->GetOutput();
}


template <class TInputValue>
typename SOMModel<TInputValue>::TargetSampleType
SOMModel<TInputValue>::DoPredict(const InputSampleType & value, ConfidenceValueType *quality) const
{ 
	unsigned int dimension =MapType::ImageDimension;
    TargetSampleType target;
    target.SetSize(dimension);
	
    auto winner =m_SOMMap->GetWinner(value);
    
    
    for (int i=0; i< dimension ;i++) {
		target[i] = winner.GetElement(i); 
	}
	
	return target;
}

} // namespace otb
#endif
