/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbLearningApplicationBase_h
#define otbLearningApplicationBase_h

#include "otbConfigure.h"

#include "otbWrapperApplication.h"


// ListSample
#include "itkListSample.h"
#include "itkVariableLengthVector.h"

// Estimator
#include "otbMachineLearningModelFactory.h"
#include <string>

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
class LearningApplicationBase : public Application
{
public:
  /** Standard class typedefs. */
  typedef LearningApplicationBase       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkTypeMacro(LearningApplicationBase, otb::Application);

  typedef TInputValue  InputValueType;
  typedef TOutputValue OutputValueType;

  typedef otb::VectorImage<InputValueType>    SampleImageType;
  typedef typename SampleImageType::PixelType PixelType;

  // Machine Learning models
  typedef otb::MachineLearningModelFactory<InputValueType, OutputValueType> ModelFactoryType;
  typedef typename ModelFactoryType::MachineLearningModelTypePointer ModelPointerType;
  typedef typename ModelFactoryType::MachineLearningModelType        ModelType;

  typedef typename ModelType::InputSampleType     SampleType;
  typedef typename ModelType::InputListSampleType ListSampleType;

  typedef typename ModelType::TargetSampleType     TargetSampleType;
  typedef typename ModelType::TargetListSampleType TargetListSampleType;
  typedef typename ModelType::TargetValueType      TargetValueType;

  itkGetConstReferenceMacro(SupervisedClassifier, std::vector<std::string>);
  itkGetConstReferenceMacro(UnsupervisedClassifier, std::vector<std::string>);

  enum ClassifierCategory
  {
    Supervised,
    Unsupervised
  };

  /**
   * Retrieve the classifier category (supervisde or unsupervised)
   * based on the select algorithm from the classifier choice.
   * @return ClassifierCategory the classifier category
   */
  ClassifierCategory GetClassifierCategory();

protected:
  LearningApplicationBase();

  ~LearningApplicationBase() override;

  /** Generic method to train and save the machine learning model. This method
   * uses specific train methods depending on the chosen model.*/
  void Train(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample, std::string modelPath);

  /** Generic method to load a model file and use it to classify a sample list*/
  typename TargetListSampleType::Pointer Classify(typename ListSampleType::Pointer validationListSample, std::string modelPath);

  /** Init method that creates all the parameters for machine learning models */
  void DoInit() override;

  /** Flag to switch between classification and regression mode.
   * False by default, child classes may change it in their constructor */
  bool m_RegressionFlag;

private:
  /** Specific Init and Train methods for each machine learning model */

  /** Init Parameters for Supervised Classifier */
  void                     InitSupervisedClassifierParams();
  std::vector<std::string> m_SupervisedClassifier;

  /** Init Parameters for Unsupervised Classifier */
  void                     InitUnsupervisedClassifierParams();
  std::vector<std::string> m_UnsupervisedClassifier;

//@{
#ifdef OTB_USE_LIBSVM
  void InitLibSVMParams();

  void TrainLibSVM(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample,
                   std::string modelPath);
#endif

#ifdef OTB_USE_OPENCV
  void InitBoostParams();
  void InitSVMParams();
  void InitDecisionTreeParams();
  void InitNeuralNetworkParams();
  void InitNormalBayesParams();
  void InitRandomForestsParams();
  void InitKNNParams();

  void TrainBoost(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample, std::string modelPath);
  void TrainSVM(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample, std::string modelPath);
  void TrainDecisionTree(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample,
                         std::string modelPath);
  void TrainNeuralNetwork(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample,
                          std::string modelPath);
  void TrainNormalBayes(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample,
                        std::string modelPath);
  void TrainRandomForests(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample,
                          std::string modelPath);
  void TrainKNN(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample, std::string modelPath);
#endif

#ifdef OTB_USE_SHARK
  void InitSharkRandomForestsParams();
  void TrainSharkRandomForests(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample,
                               std::string modelPath);
  void InitSharkKMeansParams();
  void TrainSharkKMeans(typename ListSampleType::Pointer trainingListSample, typename TargetListSampleType::Pointer trainingLabeledListSample,
                        std::string modelPath);
#endif
  //@}
};
}
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLearningApplicationBase.hxx"
#ifdef OTB_USE_OPENCV
#include "otbTrainBoost.hxx"
#include "otbTrainDecisionTree.hxx"
#include "otbTrainKNN.hxx"
#include "otbTrainNeuralNetwork.hxx"
#include "otbTrainNormalBayes.hxx"
#include "otbTrainRandomForests.hxx"
#include "otbTrainSVM.hxx"
#endif
#ifdef OTB_USE_LIBSVM
#include "otbTrainLibSVM.hxx"
#endif
#ifdef OTB_USE_SHARK
#include "otbTrainSharkRandomForests.hxx"
#include "otbTrainSharkKMeans.hxx"
#endif
#endif

#endif
