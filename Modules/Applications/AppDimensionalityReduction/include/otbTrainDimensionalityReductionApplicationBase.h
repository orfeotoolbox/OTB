/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#ifndef otbTrainDimensionalityReductionApplicationBase_h
#define otbTrainDimensionalityReductionApplicationBase_h

#include "otbConfigure.h"
#include "otbWrapperApplication.h"
#include "otbDimensionalityReductionModelFactory.h"

// ListSample
#include "itkListSample.h"
#include "itkVariableLengthVector.h"


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
 * \ingroup OTBAppDimensionalityReduction
 */
template <class TInputValue, class TOutputValue>
class TrainDimensionalityReductionApplicationBase : public Application
{
public:
  /** Standard class typedefs. */
  typedef TrainDimensionalityReductionApplicationBase Self;
  typedef Application                                 Superclass;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;

  /** Standard macro */
  itkTypeMacro(TrainDimensionalityReductionApplicationBase, otb::Application);

  typedef TInputValue  InputValueType;
  typedef TOutputValue OutputValueType;

  typedef otb::VectorImage<InputValueType>    SampleImageType;
  typedef typename SampleImageType::PixelType PixelType;

  typedef otb::DimensionalityReductionModelFactory<InputValueType, OutputValueType> ModelFactoryType;
  typedef typename ModelFactoryType::DimensionalityReductionModelTypePointer ModelPointerType;
  typedef typename ModelFactoryType::DimensionalityReductionModelType        ModelType;

  typedef typename ModelType::InputSampleType     SampleType;
  typedef typename ModelType::InputListSampleType ListSampleType;

protected:
  TrainDimensionalityReductionApplicationBase();
  ~TrainDimensionalityReductionApplicationBase() override;

  /** Generic method to train and save the machine learning model. This method
     * uses specific train methods depending on the chosen model.*/
  void Train(typename ListSampleType::Pointer trainingListSample, std::string modelPath);

  /** Generic method to load a model file and use it to classify a sample list*/
  void Reduce(typename ListSampleType::Pointer validationListSample, std::string modelPath);

  /** Init method that creates all the parameters for machine learning models */
  void DoInit() override;

private:
  /** Specific Init and Train methods for each machine learning model */

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
};

} // end of namespace Wrapper
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTrainDimensionalityReductionApplicationBase.hxx"
#include "otbDimensionalityReductionTrainSOM.hxx"
#include <string>

#ifdef OTB_USE_SHARK
#include "otbDimensionalityReductionTrainAutoencoder.hxx"
#include "otbDimensionalityReductionTrainPCA.hxx"
#endif
#endif

#endif
