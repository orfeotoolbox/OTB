#ifndef cbLearningApplicationBaseDR_h
#define cbLearningApplicationBaseDR_h

#include "otbConfigure.h"

#include "otbWrapperApplication.h"

#include <iostream>

// ListSample
#include "itkListSample.h"
#include "itkVariableLengthVector.h"

//Estimator
#include "otbMachineLearningModelFactory.h"

#ifdef OTB_USE_SHARK
#include "AutoencoderModel.h"
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

	  // Machine Learning models
	typedef otb::MachineLearningModelFactory<
				InputValueType, OutputValueType>             ModelFactoryType;
	typedef typename ModelFactoryType::MachineLearningModelTypePointer ModelPointerType;
	typedef typename ModelFactoryType::MachineLearningModelType        ModelType;
	  
	typedef typename ModelType::InputSampleType     SampleType;
	typedef typename ModelType::InputListSampleType ListSampleType;
	  

#ifdef OTB_USE_SHARK
	typedef shark::Autoencoder< shark::TanhNeuron, shark::LinearNeuron> AutoencoderType;
	typedef otb::AutoencoderModel<InputValueType, AutoencoderType> AutoencoderModelType;
	
	typedef shark::TiedAutoencoder< shark::TanhNeuron, shark::LinearNeuron> TiedAutoencoderType;
	typedef otb::AutoencoderModel<InputValueType, TiedAutoencoderType> TiedAutoencoderModelType;
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

#ifdef OTB_USE_SHARK
  void InitAutoencoderParams();
  template <class autoencoderchoice>
  void TrainAutoencoder(typename ListSampleType::Pointer trainingListSample, std::string modelPath);/*{
 // typename AutoencoderModelType::Pointer dimredTrainer = AutoencoderModelType::New();
  typename autoencoderchoice::Pointer dimredTrainer = autoencoderchoice::New();
		dimredTrainer->SetNumberOfHiddenNeurons(GetParameterInt("model.autoencoder.nbneuron"));
		dimredTrainer->SetNumberOfIterations(GetParameterInt("model.autoencoder.nbiter"));
		dimredTrainer->SetRegularization(GetParameterFloat("model.autoencoder.normalizer"));
		dimredTrainer->SetInputListSample(trainingListSample);
		dimredTrainer->Train();
		dimredTrainer->Save(modelPath);
}; // !!!!!!!!!!!!!!!!! How to declare this method body in the .txx ? (double template...) */
#endif
  //@}
};

}
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "cbLearningApplicationBaseDR.txx"
#ifdef OTB_USE_SHARK
#include "cbTrainAutoencoder.txx"
#endif
#endif

#endif
