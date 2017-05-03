#ifndef DimensionalityReductionModel_h
#define DimensionalityReductionModel_h
#include "itkObject.h"
#include "itkListSample.h"
namespace otb
{
template <class TInputValue>
class DimensionalityReductionModel: public itk::Object
{

public:
	
	typedef DimensionalityReductionModel Self;
	typedef itk::Object Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef TInputValue InputValueType;
	typedef itk::VariableLengthVector<InputValueType> InputSampleType;
	typedef itk::Statistics::ListSample<InputSampleType> InputListSampleType;


		
	itkSetObjectMacro(InputListSample,InputListSampleType);
	itkGetObjectMacro(InputListSample,InputListSampleType);
	itkGetConstObjectMacro(InputListSample,InputListSampleType);

	virtual void Save(const std::string & filename, const std::string & name="") = 0;
	virtual void Load(const std::string & filename, const std::string & name="") = 0;

	virtual void Train() = 0;
	virtual void Dimensionality_reduction() = 0;

protected:
	DimensionalityReductionModel(){};	
	typename InputListSampleType::Pointer m_InputListSample;


};
} // end namespace otb


//#ifndef OTB_MANUAL_INSTANTIATION
//#include "DimensionalityReductionModel.txx"
//#endif


#endif
