/*=========================================================================
 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#ifndef otbLearningApplicationBase_h
#define otbLearningApplicationBase_h

#include "otbConfigure.h"

#include "otbWrapperApplication.h"

#include <iostream>

// ListSample
#include "itkListSample.h"
#include "itkVariableLengthVector.h"

//Estimator
#include "otbMachineLearningModelFactory.h"

#ifdef OTB_USE_OPENCV
# include "otbKNearestNeighborsMachineLearningModel.h"
# include "otbRandomForestsMachineLearningModel.h"
// OpenCV SVM implementation is buggy with linear kernel
// Users should use the libSVM implementation instead.
//# include "otbSVMMachineLearningModel.h"
# include "otbBoostMachineLearningModel.h"
# include "otbDecisionTreeMachineLearningModel.h"
# include "otbGradientBoostedTreeMachineLearningModel.h"
# include "otbNormalBayesMachineLearningModel.h"
# include "otbNeuralNetworkMachineLearningModel.h"
#endif

#ifdef OTB_USE_LIBSVM 
#include "otbLibSVMMachineLearningModel.h"
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
 * There are multiple machine learning models in OTB, some imported from OpenCV,
 * and one imported from LibSVM. They all have different parameters. The
 * purpose of this class is to handle the creation of all parameters related to
 * machine learning models (in DoInit() ), and to dispatch the calls to
 * specific train functions in function Train().
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
class LearningApplicationBase: public Application
{
public:
  /** Standard class typedefs. */
  typedef LearningApplicationBase Self;
  typedef Application             Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkTypeMacro(LearningApplicationBase, otb::Application)

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
  
  typedef typename ModelType::TargetSampleType      TargetSampleType;
  typedef typename ModelType::TargetListSampleType  TargetListSampleType;
  typedef typename ModelType::TargetValueType       TargetValueType;
  
#ifdef OTB_USE_OPENCV
  typedef otb::RandomForestsMachineLearningModel<InputValueType, OutputValueType> RandomForestType;
  typedef otb::KNearestNeighborsMachineLearningModel<InputValueType, OutputValueType> KNNType;
  // OpenCV SVM implementation is buggy with linear kernel
  // Users should use the libSVM implementation instead.
  // typedef otb::SVMMachineLearningModel<InputValueType, OutputValueType> SVMType;
  typedef otb::BoostMachineLearningModel<InputValueType, OutputValueType> BoostType;
  typedef otb::DecisionTreeMachineLearningModel<InputValueType, OutputValueType> DecisionTreeType;
  typedef otb::GradientBoostedTreeMachineLearningModel<InputValueType, OutputValueType> GradientBoostedTreeType;
  typedef otb::NeuralNetworkMachineLearningModel<InputValueType, OutputValueType> NeuralNetworkType;
  typedef otb::NormalBayesMachineLearningModel<InputValueType, OutputValueType> NormalBayesType;
#endif

#ifdef OTB_USE_LIBSVM 
  typedef otb::LibSVMMachineLearningModel<InputValueType, OutputValueType> LibSVMType;
#endif
 
protected:
  LearningApplicationBase();

  ~LearningApplicationBase() ITK_OVERRIDE;

  /** Generic method to train and save the machine learning model. This method
   * uses specific train methods depending on the chosen model.*/
  void Train(typename ListSampleType::Pointer trainingListSample,
             typename TargetListSampleType::Pointer trainingLabeledListSample,
             std::string modelPath);

  /** Generic method to load a model file and use it to classify a sample list*/
  void Classify(typename ListSampleType::Pointer validationListSample,
                typename TargetListSampleType::Pointer predictedList,
                std::string modelPath);

  /** Init method that creates all the parameters for machine learning models */
  void DoInit();

  /** Flag to switch between classification and regression mode.
   * False by default, child classes may change it in their constructor */
  bool m_RegressionFlag;

private:

  /** Specific Init and Train methods for each machine learning model */
  //@{
#ifdef OTB_USE_LIBSVM 
  void InitLibSVMParams();

  void TrainLibSVM(typename ListSampleType::Pointer trainingListSample,
                   typename TargetListSampleType::Pointer trainingLabeledListSample,
                   std::string modelPath);
#endif  

#ifdef OTB_USE_OPENCV
  void InitBoostParams();
  // OpenCV SVM implementation is buggy with linear kernel
  // Users should use the libSVM implementation instead.
  // void InitSVMParams();
  void InitDecisionTreeParams();
  void InitGradientBoostedTreeParams();
  void InitNeuralNetworkParams();
  void InitNormalBayesParams();
  void InitRandomForestsParams();
  void InitKNNParams();

  void TrainBoost(typename ListSampleType::Pointer trainingListSample,
                  typename TargetListSampleType::Pointer trainingLabeledListSample,
                  std::string modelPath);
  // OpenCV SVM implementation is buggy with linear kernel
  // Users should use the libSVM implementation instead.
  // void TrainSVM(typename ListSampleType::Pointer trainingListSample,
  //              typename TargetListSampleType::Pointer trainingLabeledListSample,
  //              std::string modelPath);
  void TrainDecisionTree(typename ListSampleType::Pointer trainingListSample,
                         typename TargetListSampleType::Pointer trainingLabeledListSample,
                         std::string modelPath);
  void TrainGradientBoostedTree(typename ListSampleType::Pointer trainingListSample,
                                typename TargetListSampleType::Pointer trainingLabeledListSample,
                                std::string modelPath);
  void TrainNeuralNetwork(typename ListSampleType::Pointer trainingListSample,
                          typename TargetListSampleType::Pointer trainingLabeledListSample,
                          std::string modelPath);
  void TrainNormalBayes(typename ListSampleType::Pointer trainingListSample,
                        typename TargetListSampleType::Pointer trainingLabeledListSample,
                        std::string modelPath);
  void TrainRandomForests(typename ListSampleType::Pointer trainingListSample,
                          typename TargetListSampleType::Pointer trainingLabeledListSample,
                          std::string modelPath);
  void TrainKNN(typename ListSampleType::Pointer trainingListSample,
                typename TargetListSampleType::Pointer trainingLabeledListSample,
                std::string modelPath);
#endif
  //@}
};

}
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLearningApplicationBase.txx"
#ifdef OTB_USE_OPENCV
#include "otbTrainBoost.txx"
#include "otbTrainDecisionTree.txx"
#include "otbTrainGradientBoostedTree.txx"
#include "otbTrainKNN.txx"
#include "otbTrainNeuralNetwork.txx"
#include "otbTrainNormalBayes.txx"
#include "otbTrainRandomForests.txx"
// OpenCV SVM implementation is buggy with linear kernel
// Users should use the libSVM implementation instead.
//#include "otbTrainSVM.txx"
#endif
#ifdef OTB_USE_LIBSVM
#include "otbTrainLibSVM.txx"
#endif
#endif

#endif
