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
#ifndef __otbLearningApplicationBase_h
#define __otbLearningApplicationBase_h

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
# include "otbSVMMachineLearningModel.h"
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
  
#ifdef OTB_USE_OPENCV
  typedef otb::RandomForestsMachineLearningModel<InputValueType, OutputValueType> RandomForestType;
  typedef otb::KNearestNeighborsMachineLearningModel<InputValueType, OutputValueType> KNNType;
  typedef otb::SVMMachineLearningModel<InputValueType, OutputValueType> SVMType;
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
  
//  using Superclass::AddParameter;
//  friend void InitSVMParams(LearningApplicationBase & app);

  void Train(typename ListSampleType::Pointer trainingListSample,
             typename TargetListSampleType::Pointer trainingLabeledListSample,
             std::string modelPath);

  void Classify(typename ListSampleType::Pointer validationListSample,
                typename TargetListSampleType::Pointer predictedList,
                std::string modelPath);

  void DoInit();

private:

#ifdef OTB_USE_LIBSVM 
  void InitLibSVMParams();

  void TrainLibSVM(typename ListSampleType::Pointer trainingListSample,
                   typename TargetListSampleType::Pointer trainingLabeledListSample,
                   std::string modelPath);
#endif  

#ifdef OTB_USE_OPENCV
  void InitBoostParams();
  void InitSVMParams();
  void InitDecisionTreeParams();
  void InitGradientBoostedTreeParams();
  void InitNeuralNetworkParams();
  void InitNormalBayesParams();
  void InitRandomForestsParams();
  void InitKNNParams();

  void TrainBoost(typename ListSampleType::Pointer trainingListSample,
                  typename TargetListSampleType::Pointer trainingLabeledListSample,
                  std::string modelPath);
  void TrainSVM(typename ListSampleType::Pointer trainingListSample,
                typename TargetListSampleType::Pointer trainingLabeledListSample,
                std::string modelPath);
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

  bool m_RegressionFlag;
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
#include "otbTrainSVM.txx"
#endif
#ifdef OTB_USE_LIBSVM
#include "otbTrainLibSVM.txx"
#endif
#endif

#endif
