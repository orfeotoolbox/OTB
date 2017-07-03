#ifndef cbLearningApplicationBaseDR_h
#define cbLearningApplicationBaseDR_h

#include "otbConfigure.h"

#include "otbWrapperApplication.h"

#include <iostream>

// ListSample
#include "itkListSample.h"
#include "itkVariableLengthVector.h"

//Estimator
#include "DimensionalityReductionModelFactory.h"

#include "SOMModel.h"

#ifdef OTB_USE_SHARK
#include "AutoencoderModel.h"
#include "PCAModel.h"
#endif

namespace otb
{
namespace Wrapper
{

/** \class LearningApplicationBase
 *  \brief LearningApplicationBase is the base class for application that
 *         use machine learning model.
 *
 * This base class offers a DoInit() method to initialize all the parameters
 * related to machine learning models. They will all be in the choice parameter
 * named "classifier". The class also offers generic Train() and Classify()
 * methods. The classes derived from LearningApplicationBase only need these
 * 3 methods to handle the machine learning model.
 *
 * There are multiple machine learning models in OTB, some imported
 * from OpenCV and one imported from LibSVM. They all have
 * different parameters. The purpose of this class is to handle the
 * creation of all parameters related to machine learning models (in
 * DoInit() ), and to dispatch the calls to specific train functions
 * in function Train().
 *
 * This class is templated over scalar types for input and output values.
 * Typically, the input value type will be either float of double. The choice
 * of an output value type depends on the learning mode. This base class
 * supports both classification and regression modes. For classification
 * (enabled by default), the output value type corresponds to a class
 * identifier so integer types suit well. For regression, the output value
 * should not be an integer type, but rather a floating point type. In addition,
 * an application deriving this base class for regression should initialize
 * the m_RegressionFlag to true in their constructor.
 *
 * \sa TrainImagesClassifier
 * \sa TrainRegression
 *
 * \ingroup OTBAppClassification
 */
template <class TInputValue, class TOutputValue>
class cbLearningApplicationBaseDR: public Application
{
public:
	/** Standard class typedefs. */
	typedef cbLearningApplicationBaseDR Self;
	typedef Application             Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkTypeMacro(cbLearningApplicationBaseDR, otb::Application)

	typedef TInputValue                             InputValueType;
	typedef TOutputValue                            OutputValueType;

	typedef otb::VectorImage<InputValueType>        SampleImageType;
	typedef typename SampleImageType::PixelType     PixelType;

	typedef otb::DimensionalityReductionModelFactory<
				InputValueType, OutputValueType>             ModelFactoryType;
	typedef typename ModelFactoryType::DimensionalityReductionModelTypePointer ModelPointerType;
	typedef typename ModelFactoryType::DimensionalityReductionModelType        ModelType;
	  
	typedef typename ModelType::InputSampleType     SampleType;
	typedef typename ModelType::InputListSampleType ListSampleType;
	  
	// Dimensionality reduction models

	//typedef SOMMap<TInputValue,itk::Statistics::EuclideanDistanceMetric<itk::VariableLengthVector<TInputValue>>, 2> Map2DType;
	typedef otb::SOMModel<InputValueType, 2> SOM2DModelType;
	
	//typedef SOMMap<TInputValue,itk::Statistics::EuclideanDistanceMetric<itk::VariableLengthVector<TInputValue>>, 3> Map3DType;
	typedef otb::SOMModel<InputValueType, 3> SOM3DModelType;
	
	//typedef SOMMap<TInputValue,itk::Statistics::EuclideanDistanceMetric<itk::VariableLengthVector<TInputValue>>, 4> Map4DType;
	typedef otb::SOMModel<InputValueType, 4> SOM4DModelType;
	
	//typedef SOMMap<TInputValue,itk::Statistics::EuclideanDistanceMetric<itk::VariableLengthVector<TInputValue>>, 5> Map5DType;
	typedef otb::SOMModel<InputValueType, 5> SOM5DModelType;
	

#ifdef OTB_USE_SHARK
	typedef shark::Autoencoder< shark::LogisticNeuron, shark::LogisticNeuron> AutoencoderType;
	typedef otb::AutoencoderModel<InputValueType, AutoencoderType> AutoencoderModelType;
	
	typedef shark::TiedAutoencoder< shark::LogisticNeuron, shark::LogisticNeuron> TiedAutoencoderType;
	typedef otb::AutoencoderModel<InputValueType, TiedAutoencoderType> TiedAutoencoderModelType;
	
	typedef otb::PCAModel<InputValueType> PCAModelType;
#endif
  
protected:
  cbLearningApplicationBaseDR();

  ~cbLearningApplicationBaseDR() ITK_OVERRIDE;

  /** Generic method to train and save the machine learning model. This method
   * uses specific train methods depending on the chosen model.*/
  void Train(typename ListSampleType::Pointer trainingListSample,
             std::string modelPath);

  /** Generic method to load a model file and use it to classify a sample list*/
  void Reduce(typename ListSampleType::Pointer validationListSample,
                std::string modelPath);

  /** Init method that creates all the parameters for machine learning models */
  void DoInit();

private:

  /** Specific Init and Train methods for each machine learning model */
  //@{
  
  void InitSOMParams();
  template <class somchoice>
  void TrainSOM(typename ListSampleType::Pointer trainingListSample, std::string modelPath);
  void BeforeTrainSOM(typename ListSampleType::Pointer trainingListSample, std::string modelPath);

#ifdef OTB_USE_SHARK
  void InitAutoencoderParams();
  void InitPCAParams();
  
  void BeforeTrainAutoencoder(typename ListSampleType::Pointer trainingListSample, std::string modelPath);
  template <class autoencoderchoice>
  void TrainAutoencoder(typename ListSampleType::Pointer trainingListSample, std::string modelPath);
  
  void TrainPCA(typename ListSampleType::Pointer trainingListSample, std::string modelPath);
  
 
#endif  
  //@}
};

}
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "cbLearningApplicationBaseDR.txx"
#include "cbTrainSOM.txx"

#ifdef OTB_USE_SHARK
#include "cbTrainAutoencoder.txx"
#include "cbTrainPCA.txx"
#endif
#endif

#endif
