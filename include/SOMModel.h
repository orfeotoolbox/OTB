#ifndef SOMModel_h
#define SOMModel_h

//#include "DimensionalityReductionModel.h"
#include "otbSOMMap.h"

#include "otbSOM.h"

#include "itkEuclideanDistanceMetric.h" // the distance function

#include "otbCzihoSOMLearningBehaviorFunctor.h"
#include "otbCzihoSOMNeighborhoodBehaviorFunctor.h"

#include "otbMachineLearningModelTraits.h"
#include "otbMachineLearningModel.h"

namespace otb
{
template <class TInputValue, unsigned int MapDimension>
class ITK_EXPORT SOMModel: public  MachineLearningModel<itk::VariableLengthVector< TInputValue> , itk::VariableLengthVector< TInputValue>>   
{

public:
	
	typedef SOMModel Self;
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



	typedef SOMMap<itk::VariableLengthVector<TInputValue>,itk::Statistics::EuclideanDistanceMetric<itk::VariableLengthVector<TInputValue>>, MapDimension> MapType;
	typedef typename MapType::SizeType       				SizeType;
	typedef typename MapType::SpacingType       			SpacingType;
	//typedef otb::SOM<InputListSampleType, MapType> 			EstimatorType;
	typedef otb::SOM<InputListSampleType, MapType> 			EstimatorType;

	typedef Functor::CzihoSOMLearningBehaviorFunctor    	SOMLearningBehaviorFunctorType;
	typedef Functor::CzihoSOMNeighborhoodBehaviorFunctor 	SOMNeighborhoodBehaviorFunctorType;

	itkNewMacro(Self);
	itkTypeMacro(SOMModel, DimensionalityReductionModel);
	 
	/** Accessors */
	itkSetMacro(NumberOfIterations, unsigned int);
	itkGetMacro(NumberOfIterations, unsigned int);
    itkSetMacro(BetaInit, double);
	itkGetMacro(BetaInit, double);
	itkSetMacro(BetaEnd, double);
	itkGetMacro(BetaEnd, double);
	itkSetMacro(MinWeight, InputValueType);
	itkGetMacro(MinWeight, InputValueType);
	itkSetMacro(MaxWeight, InputValueType);
	itkGetMacro(MaxWeight, InputValueType);
	itkSetMacro(MapSize, SizeType);
	itkGetMacro(MapSize, SizeType);
	itkSetMacro(NeighborhoodSizeInit, SizeType);
	itkGetMacro(NeighborhoodSizeInit, SizeType);
	itkSetMacro(RandomInit, bool);
	itkGetMacro(RandomInit, bool);
	itkSetMacro(Seed, unsigned int);
	itkGetMacro(Seed, unsigned int);
	itkGetObjectMacro(ListSample, InputListSampleType);
	itkSetObjectMacro(ListSample, InputListSampleType);

	bool CanReadFile(const std::string & filename);
	bool CanWriteFile(const std::string & filename);

	void Save(const std::string & filename, const std::string & name="") ;
	void Load(const std::string & filename, const std::string & name="") ;

	void Train() ITK_OVERRIDE;
	//void Dimensionality_reduction()  {}; // Dimensionality reduction is done by DoPredict
	 
	//unsigned int GetDimension() { return MapType::ImageDimension;};
protected:
	SOMModel();	
	~SOMModel() ITK_OVERRIDE;
 
private:
	typename MapType::Pointer m_SOMMap;
	
	virtual TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType * quality = ITK_NULLPTR) const;

	/** Map Parameters used for training */
  
	SizeType m_MapSize;
	/** Number of iterations */
	unsigned int m_NumberOfIterations;
	/** Initial learning coefficient */
	double m_BetaInit;
	/** Final learning coefficient */
	double m_BetaEnd;
	/** Initial neighborhood size */
	SizeType m_NeighborhoodSizeInit;
	/** Minimum initial neuron weights */
	InputValueType m_MinWeight;
	/** Maximum initial neuron weights */
	InputValueType m_MaxWeight;
	/** Random initialization bool */
	bool m_RandomInit;
	/** Seed for random initialization */
	unsigned int m_Seed;
	/** The input list sample */
	ListSamplePointerType m_ListSample;
	/** Behavior of the Learning weightening (link to the beta coefficient) */
	SOMLearningBehaviorFunctorType m_BetaFunctor;
	/** Behavior of the Neighborhood extent */
	SOMNeighborhoodBehaviorFunctorType m_NeighborhoodSizeFunctor;

};


} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "SOMModel.txx"
#endif


#endif

