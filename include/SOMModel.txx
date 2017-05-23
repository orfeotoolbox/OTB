
#ifndef SOMModel_txx
#define SOMModel_txx

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkMacro.h"


// test text file
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include <fstream> 


#include "itkImage.h"

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
  SizeType size = m_SOMMap->GetLargestPossibleRegion().GetSize() ;
  //ofs << m_SOMMap->GetLargestPossibleRegion().GetSize() << std::endl;
  for (size_t i=0;i<MapDimension;i++){
		ofs << size[i] << " " ;
  }
  ofs << std::endl;	
  ofs << inputIterator.Get().GetNumberOfElements() << std::endl;;
  while(!inputIterator.IsAtEnd()){
	InputSampleType vect = inputIterator.Get();
	for (size_t i=0;i<vect.GetNumberOfElements();i++){
		ofs << vect[i] << " " ;
	}
		
	++inputIterator;
  }
  ofs.close();

}

template <class TInputValue, unsigned int MapDimension>
void SOMModel<TInputValue, MapDimension>::Load(const std::string & filename, const std::string & name)
{
	/*
	auto reader = otb::ImageFileReader<MapType>::New();
	reader->SetFileName(filename);
	reader->Update();
	if (reader->GetOutput()->GetImageKeywordlist().GetMetadataByKey("MachineLearningModelType") != "SOM"+std::to_string(MapDimension)){
		itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
	m_SOMMap = reader->GetOutput();
	*/
	// test text
	
	
	
	
	
	
	std::ifstream ifs(filename+"2");
	std::string model_type_str;
	std::string dimension_str;
	std::string size_str;
	std::string number_of_elements_str;
	
	std::getline(ifs,model_type_str); 
	std::getline(ifs,dimension_str); 
	if (model_type_str+dimension_str != "SOM"+std::to_string(MapDimension)){
		itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
    
	std::cout << "bug-1?" << std::endl;
	SizeType size;
	itk::Point<double, MapDimension> origin;
	SpacingType spacing;
	itk::Index< MapDimension > index;
	for (int i=0 ; i<MapDimension; i++)
	{
		std::getline(ifs,size_str , ' '); 
		size[i] = stof(size_str);
		origin[i] = 0;
		spacing[i]=0;
		index[i]=0;
	}
	
	
	std::getline(ifs,number_of_elements_str); 
	std::getline(ifs,number_of_elements_str); 
	std::cout << "bug0?" <<  number_of_elements_str << std::endl;
	auto number_of_elements = stof(number_of_elements_str);
	
	//typedef itk::Image< unsigned char, 3 > ImageType;
	//typename MapType::Pointer image = MapType::New();
	m_SOMMap = MapType::New();
	typename MapType::RegionType region;
	region.SetSize( size );
	m_SOMMap->SetNumberOfComponentsPerPixel(number_of_elements);
	region.SetIndex( index );
	m_SOMMap->SetRegions( region );
	m_SOMMap->Allocate();

	std::cout << m_SOMMap << std::endl;
/*
	
	std::cout << "bug1?" << number_of_elements << std::endl;
	itk::ImageRegion<MapDimension> outputRegion;
	
	
	std::cout << "bugoriggin?" << origin << std::endl;
	m_SOMMap->SetNumberOfComponentsPerPixel(number_of_elements);
	outputRegion.SetIndex(index);
		std::cout << "setindex?" << index << std::endl;
	outputRegion.SetSize(size);
	std::cout << origin << size << std::endl;
	m_SOMMap->SetLargestPossibleRegion(outputRegion);
	std::cout << "setRegion" << origin << std::endl;
	
	m_SOMMap->Allocate();
	std::cout << "bug2?" << std::endl;
	
	*/
	itk::ImageRegionIterator<MapType> outputIterator(m_SOMMap,region);
    
	std::string value;
	size_t j=0;
	while(!outputIterator.IsAtEnd()){
		std::cout << j << std::endl;
		std::getline(ifs,value, ' ');
		itk::VariableLengthVector<float>  vect(number_of_elements);
		for (int i=0 ; i<number_of_elements; i++)
		{
			std::getline(ifs,value , ' '); 
			//std::cout << value << " ";
			std::cout << stof(value) << " ";
			vect[i]=std::stof(value);
		}
		std::cout << vect << std::endl;
		outputIterator.Set(vect);
		++outputIterator;
		j++;
		std::cout << j << "end" << std::endl;
		//std::cout << value << std::endl;
    //std::string line;
    //std::getline(ifs, line);
	}
	std::cout << j << std::endl;
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
