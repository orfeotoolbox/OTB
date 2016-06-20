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
#ifndef __otbSharkLearningApplicationBase_h
#define __otbSharkLearningApplicationBase_h

#include "otbConfigure.h"

#include "otbWrapperApplication.h"

#include <iostream>

// ListSample
#include "itkListSample.h"
#include "itkVariableLengthVector.h"

//Estimator
#include "otbMachineLearningModelFactory.h"

#ifdef OTB_USE_SHARK
#include "otbSharkRandomForestsMachineLearningModel.h"
#endif

namespace otb
{
namespace Wrapper
{

/** \class SharkLearningApplicationBase
 *  \brief SharkLearningApplicationBase is the base class for application that
 *         use Shark machine learning models.
 *
 */
template <class TInputValue, class TOutputValue>
class SharkLearningApplicationBase: public Application
{
public:
  /** Standard class typedefs. */
  typedef SharkLearningApplicationBase Self;
  typedef Application             Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkTypeMacro(SharkLearningApplicationBase, otb::Application)

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
  
#ifdef OTB_USE_SHARK
  typedef otb::SharkRandomForestsMachineLearningModel<InputValueType, OutputValueType> SharkRandomForestType;
#endif
protected:
  SharkLearningApplicationBase();

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

#ifdef OTB_USE_SHARK
  void InitSharkRandomForestsParams();
  void TrainSharkRandomForests(typename ListSampleType::Pointer trainingListSample,
                           typename TargetListSampleType::Pointer trainingLabeledListSample,
                           std::string modelPath);
#endif
  //@}
};

}
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSharkLearningApplicationBase.txx"
#ifdef OTB_USE_SHARK
#include "otbTrainSharkRandomForests.txx"
#endif

#endif

#endif
