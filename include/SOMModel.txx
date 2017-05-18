
#ifndef SOMModel_txx
#define SOMModel_txx

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkMacro.h"


// test text file
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include <fstream> 

namespace otb
{


template <class TInputValue, unsigned int MapDimension>
SOMModel<TInputValue,  MapDimension>::SOMModel()
{
}


template <class TInputValue, unsigned int MapDimension>
SOMModel<TInputValue, MapDimension>::~SOMModel()
{
}


template <class TInputValue, unsigned int MapDimension>
void SOMModel<TInputValue,  MapDimension>::Train()
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
    std::cout << "dr of the first sample : " << m_SOMMap->GetWinner(m_ListSample->GetMeasurementVector(0)) << std::endl;
}


template <class TInputValue, unsigned int MapDimension>
bool SOMModel<TInputValue, MapDimension>::CanReadFile(const std::string & filename)
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


template <class TInputValue, unsigned int MapDimension>
bool SOMModel<TInputValue, MapDimension>::CanWriteFile(const std::string & filename)
{
	return true;
}

template <class TInputValue, unsigned int MapDimension>
void SOMModel<TInputValue, MapDimension>::Save(const std::string & filename, const std::string & name)
{
  std::cout << m_SOMMap->GetNumberOfComponentsPerPixel() << std::endl;
  
//Ecriture
  auto kwl = m_SOMMap->GetImageKeywordlist();
  kwl.AddKey("MachineLearningModelType", "SOM"+std::to_string(MapDimension));
  m_SOMMap->SetImageKeywordList(kwl);
  auto writer = otb::ImageFileWriter<MapType>::New();
  writer->SetInput(m_SOMMap);
  writer->SetFileName(filename);
  writer->Update();


 // test text
  itk::ImageRegionConstIterator<MapType> inputIterator(m_SOMMap,m_SOMMap->GetLargestPossibleRegion());
  std::ofstream ofs(filename+"2");
  ofs << "SOM" << std::endl; 
  ofs << MapDimension << std::endl;
  ofs << m_SOMMap->GetLargestPossibleRegion().GetSize() << std::endl;
  ofs << inputIterator.Get().GetNumberOfElements() << std::endl;;
  while(!inputIterator.IsAtEnd()){
	InputSampleType vect = inputIterator.Get();
	for (size_t i=0;i<vect.GetNumberOfElements();i++){
		ofs << vect[i] << " " ;
	}
	ofs << std::endl;
		
	++inputIterator;
  }
  ofs.close();

}

template <class TInputValue, unsigned int MapDimension>
void SOMModel<TInputValue, MapDimension>::Load(const std::string & filename, const std::string & name)
{
	auto reader = otb::ImageFileReader<MapType>::New();
	reader->SetFileName(filename);
	reader->Update();
	if (reader->GetOutput()->GetImageKeywordlist().GetMetadataByKey("MachineLearningModelType") != "SOM"+std::to_string(MapDimension)){
		itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
	m_SOMMap = reader->GetOutput();
	
	// test text
	std::ifstream ifs(filename+"2");
	std::string model_type_str;
	std::string dimension_str;
	std::string size_str;
	std::string number_of_elements_str;
	SizeType size;
	
	std::getline(ifs,model_type_str); 
	std::getline(ifs,dimension_str); 
	std::getline(ifs,size_str); 
	std::getline(ifs,number_of_elements_str); 
	
	for (int i=0 ; i<MapDimension; i++)
	{
		
	}
	
	while (!ifs.eof())
    {
    std::string line;
    std::getline(ifs, line);
	}
	ifs.close();
	std::cout << "model type " << model_type_str << std::endl;
	std::cout << "dimension " << dimension_str << std::endl;
	std::cout << "size " << size_str << std::endl;
}


template <class TInputValue, unsigned int MapDimension>
typename SOMModel<TInputValue, MapDimension>::TargetSampleType
SOMModel<TInputValue, MapDimension>::DoPredict(const InputSampleType & value) const
{ 
	unsigned int dimension =MapType::ImageDimension;
    TargetSampleType target;
    target.SetSize(dimension);
	
    auto winner =m_SOMMap->GetWinner(value);
    // std::cout <<  winner << std::endl;
    
    for (int i=0; i< dimension ;i++) {
		target[i] = winner.GetElement(i); 
	}
	
	return target;
}

} // namespace otb
#endif
