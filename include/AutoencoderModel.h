#ifndef AutoencoderModel_h
#define AutoencoderModel_h

#include "DimensionalityReductionModel.h"


namespace otb
{
template <class TInputValue, class AutoencoderType>
class ITK_EXPORT AutoencoderModel: public  DimensionalityReductionModel<TInputValue,TInputValue>   
{

public:
	
	typedef AutoencoderModel Self;
	typedef DimensionalityReductionModel<TInputValue,TInputValue> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef typename Superclass::InputValueType InputValueType;
	typedef typename Superclass::InputSampleType InputSampleType;
	typedef typename Superclass::InputListSampleType InputListSampleType;
	typedef typename Superclass::TargetValueType TargetValueType;
	typedef typename Superclass::TargetSampleType TargetSampleType;
	typedef typename Superclass::TargetListSampleType TargetListSampleType;
	
	itkNewMacro(Self);
	itkTypeMacro(AutoencoderModel, DimensionalityReductionModel);

	unsigned int GetDimension() {return m_NumberOfHiddenNeurons;};
	itkGetMacro(NumberOfHiddenNeurons,unsigned int);
	itkSetMacro(NumberOfHiddenNeurons,unsigned int);

	itkGetMacro(NumberOfIterations,unsigned int);
	itkSetMacro(NumberOfIterations,unsigned int);

	itkGetMacro(Regularization,double);
	itkSetMacro(Regularization,double);

	itkGetMacro(Noise,double);
	itkSetMacro(Noise,double);

	bool CanReadFile(const std::string & filename);
	bool CanWriteFile(const std::string & filename);

	void Save(const std::string & filename, const std::string & name="")  ITK_OVERRIDE;
	void Load(const std::string & filename, const std::string & name="")  ITK_OVERRIDE;

	void Train() ITK_OVERRIDE;
	//void Dimensionality_reduction()  {}; // Dimensionality reduction is done by DoPredict
	 

protected:
	AutoencoderModel();	
	~AutoencoderModel() ITK_OVERRIDE;
 
	virtual TargetSampleType DoPredict(const InputSampleType& input) const ITK_OVERRIDE;
	virtual void DoPredictBatch(const InputListSampleType *, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType *) const ITK_OVERRIDE;
  
private:
	
	AutoencoderType m_net;
	unsigned int m_NumberOfHiddenNeurons;
	unsigned int m_NumberOfIterations;
	double m_Regularization;
	double m_Noise;
};
} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "AutoencoderModel.txx"
#endif


#endif

