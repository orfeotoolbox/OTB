#ifndef SOMModel_txx
#define SOMModel_txx

#include "otbSOMModel.h"

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
	this->m_Dimension = MapType::ImageDimension;
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
bool SOMModel<TInputValue, MapDimension>::CanWriteFile(const std::string & /*filename*/)
{
	return true;
}

template<typename T>
std::ostream& binary_write(std::ostream& stream, const T& value){
    return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}


std::ostream& binary_write_string(std::ofstream& stream, const std::string& value){
    return stream.write(value.c_str(), value.length());
}

template<typename T>
std::istream & binary_read(std::istream& stream, T& value){
    return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}



template <class TInputValue, unsigned int MapDimension>
void SOMModel<TInputValue, MapDimension>::Save(const std::string & filename, const std::string & /*name*/)
{
	itk::ImageRegionConstIterator<MapType> inputIterator(m_SOMMap,m_SOMMap->GetLargestPossibleRegion());
	inputIterator.GoToBegin();
	std::ofstream ofs(filename, std::ios::binary);
	binary_write_string(ofs,"som"); 
	binary_write(ofs,static_cast<unsigned int>(MapDimension));
	SizeType size = m_SOMMap->GetLargestPossibleRegion().GetSize() ;
	for (size_t i=0;i<MapDimension;i++){
		binary_write(ofs,size[i]);
	}
	  
	binary_write(ofs,inputIterator.Get().GetNumberOfElements());
	while(!inputIterator.IsAtEnd()){
		InputSampleType vect = inputIterator.Get();
		for (size_t i=0;i<vect.GetNumberOfElements();i++){
			binary_write(ofs,vect[i]);
		}	
	++inputIterator;
	}
	ofs.close();
	
	if (this->m_WriteMap == true)     // output the map vectors in a txt file
	{
		std::ofstream otxt(filename+".txt");
		inputIterator.GoToBegin();
		while(!inputIterator.IsAtEnd())
		{
			InputSampleType vect = inputIterator.Get();
			for (size_t i=0;i<vect.GetNumberOfElements();i++)
			{
				
				otxt << vect[i] << " ";
			}	
			otxt << std::endl;
			++inputIterator;
		}
		otxt.close();
	}
}

template <class TInputValue, unsigned int MapDimension>
void SOMModel<TInputValue, MapDimension>::Load(const std::string & filename, const std::string & /*name*/)
{
	
	std::ifstream ifs(filename, std::ios::binary);
	
	/**  Read the model key (should be som) */
	char s[]="   ";
	for (int i=0; i<3; i++){	
		binary_read(ifs,s[i]);
	}
	std::string modelType(s);
	/** Read the dimension of the map (should be equal to MapDimension) */
	
	unsigned int dimension;
	binary_read(ifs,dimension);
	if (modelType != "som" || dimension != MapDimension){
		itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
    
	SizeType size;
	itk::Index< MapDimension > index;
	for (unsigned int i=0 ; i<MapDimension; i++)
	{
		binary_read(ifs,size[i]);
		index[i]=0;
	}
	unsigned int numberOfElements;
	binary_read(ifs,numberOfElements);
	m_SOMMap = MapType::New();
	typename MapType::RegionType region;
	region.SetSize( size );
	m_SOMMap->SetNumberOfComponentsPerPixel(numberOfElements);
	region.SetIndex( index );
	m_SOMMap->SetRegions( region );
	m_SOMMap->Allocate();

	itk::ImageRegionIterator<MapType> outputIterator(m_SOMMap,region);
    outputIterator.GoToBegin();
	std::string value;
	while(!outputIterator.IsAtEnd()){
		InputSampleType  vect(numberOfElements);
		for (unsigned int i=0 ; i<numberOfElements; i++)
		{
			float v;    // InputValue type is not the same during training anddimredvector.
			binary_read(ifs,v);
			vect[i] = static_cast<double>(v);
		}
		outputIterator.Set(vect);
		++outputIterator;
	}
	ifs.close();
	
	this->m_Dimension = MapType::ImageDimension;
}


template <class TInputValue, unsigned int MapDimension>
typename SOMModel<TInputValue, MapDimension>::TargetSampleType
SOMModel<TInputValue, MapDimension>::DoPredict(const InputSampleType & value, ConfidenceValueType * /*quality*/) const
{ 
    TargetSampleType target;
    target.SetSize(this->m_Dimension);
	
    auto winner =m_SOMMap->GetWinner(value);
    for (unsigned int i=0; i< this->m_Dimension ;i++) {
		target[i] = winner.GetElement(i); 
	}

	return target;
}

} // namespace otb
#endif
