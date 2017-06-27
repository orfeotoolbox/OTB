#ifndef AutoencoderModel_h
#define AutoencoderModel_h

#include "otbMachineLearningModelTraits.h"
#include "otbMachineLearningModel.h"
#include <fstream>
#include <shark/Algorithms/StoppingCriteria/AbstractStoppingCriterion.h>

namespace otb
{
template <class TInputValue, class AutoencoderType>
class ITK_EXPORT AutoencoderModel: public  MachineLearningModel<itk::VariableLengthVector< TInputValue> , itk::VariableLengthVector< TInputValue>>   
{

public:
	
	typedef AutoencoderModel Self;
	typedef MachineLearningModel<itk::VariableLengthVector< TInputValue> , itk::VariableLengthVector< TInputValue>> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef typename Superclass::InputValueType 			InputValueType;
	typedef typename Superclass::InputSampleType 			InputSampleType;
	typedef typename Superclass::InputListSampleType 		InputListSampleType;
	typedef typename InputListSampleType::Pointer 			ListSamplePointerType;
	typedef typename Superclass::TargetValueType 			TargetValueType;
	typedef typename Superclass::TargetSampleType 			TargetSampleType;
	typedef typename Superclass::TargetListSampleType 		TargetListSampleType;

	/// Confidence map related typedefs
	
	typedef typename Superclass::ConfidenceValueType  				ConfidenceValueType;
	typedef typename Superclass::ConfidenceSampleType 				ConfidenceSampleType;
	typedef typename Superclass::ConfidenceListSampleType      		ConfidenceListSampleType;


	itkNewMacro(Self);
	itkTypeMacro(AutoencoderModel, DimensionalityReductionModel);

	//unsigned int GetDimension() {return m_NumberOfHiddenNeurons[m_net.size()-1];};  // Override the Dimensionality Reduction model method, it is used in the dimensionality reduction filter to set the output image size
	itkGetMacro(NumberOfHiddenNeurons,itk::Array<unsigned int>);
	itkSetMacro(NumberOfHiddenNeurons,itk::Array<unsigned int>);

	itkGetMacro(NumberOfIterations,unsigned int);
	itkSetMacro(NumberOfIterations,unsigned int);
	
	itkGetMacro(Epsilon,double);
	itkSetMacro(Epsilon,double);

	itkGetMacro(Regularization,itk::Array<double>);
	itkSetMacro(Regularization,itk::Array<double>);

	itkGetMacro(Noise,itk::Array<double>);
	itkSetMacro(Noise,itk::Array<double>);

	itkGetMacro(Rho,itk::Array<double>);
	itkSetMacro(Rho,itk::Array<double>);

	itkGetMacro(Beta,itk::Array<double>);
	itkSetMacro(Beta,itk::Array<double>);

	bool CanReadFile(const std::string & filename);
	bool CanWriteFile(const std::string & filename);

	void Save(const std::string & filename, const std::string & name="")  ITK_OVERRIDE;
	void Load(const std::string & filename, const std::string & name="")  ITK_OVERRIDE;

	void Train() ITK_OVERRIDE;
	
	template <class T>
	void TrainOneLayer(shark::AbstractStoppingCriterion<T> & criterion, unsigned int,double, double, shark::Data<shark::RealVector> &, std::ostream&);
	
	template <class T>
	void TrainOneSparseLayer(shark::AbstractStoppingCriterion<T> & criterion, unsigned int,double, double,double, shark::Data<shark::RealVector> &, std::ostream&);
	
protected:
	AutoencoderModel();	
	~AutoencoderModel() ITK_OVERRIDE;
 
	virtual TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType * quality = ITK_NULLPTR) const;

	virtual void DoPredictBatch(const InputListSampleType *, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType *, ConfidenceListSampleType * quality = ITK_NULLPTR) const;
  
private:
	
	/** Network attributes */
	std::vector<AutoencoderType> m_net;

	typename InputListSampleType::Pointer m_ValidationListSample; // This list can optionnaly be used during training to prevent overfitting.

	
	itk::Array<unsigned int> m_NumberOfHiddenNeurons;
	/** Training parameters */
	unsigned int m_NumberOfIterations; // stop the training after a fixed number of iterations
	double m_Epsilon; // Stops the training when the training error seems to converge
	itk::Array<double> m_Regularization;  // L2 Regularization parameter
	itk::Array<double> m_Noise;  // probability for an input to be set to 0 (denosing autoencoder)
	itk::Array<double> m_Rho; // Sparsity parameter
	itk::Array<double> m_Beta; // Sparsity regularization parameter
};
} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "AutoencoderModel.txx"
#endif


#endif

