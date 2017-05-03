#ifndef AutoencoderModel_h
#define AutoencoderModel_h

#include "DimensionalityReductionModel.h"


namespace otb
{
template <class TInputValue, class AutoencoderType>
class AutoencoderModel: public DimensionalityReductionModel<TInputValue>
{

public:
	
	typedef AutoencoderModel Self;
	typedef DimensionalityReductionModel<TInputValue> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef TInputValue InputValueType;
	typedef itk::VariableLengthVector<InputValueType> InputSampleType;
	typedef itk::Statistics::ListSample<InputSampleType> InputListSampleType;


	
	void Save(const std::string & filename, const std::string & name="")  {};
	void Load(const std::string & filename, const std::string & name="") {};

	void Train();
	void Dimensionality_reduction()  {};

protected:
	AutoencoderModel(){};	
	
};
} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "AutoencoderModel.txx"
#endif


#endif
