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
#include "otbConfigure.h"

#include "otbWrapperApplicationFactory.h"

#include <iostream>

//Image
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbListSampleGenerator.h"

// ListSample
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

// Statistic XML Reader
#include "otbStatisticsXMLFileReader.h"

// Validation
#include "otbConfusionMatrixCalculator.h"

#include "otbStandardFilterWatcher.h"

// Normalize the samples
#include "otbShiftScaleSampleListFilter.h"

// List sample concatenation
#include "otbConcatenateSampleListFilter.h"

// Balancing ListSample
#include "otbListSampleToBalancedListSampleFilter.h"

// VectorData projection filter

// Extract a ROI of the vectordata
#include "otbVectorDataIntoImageProjectionFilter.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class TrainImagesClassifier: public Application
{
public:
  /** Standard class typedefs. */
  typedef TrainImagesClassifier Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self)

  itkTypeMacro(TrainImagesClassifier, otb::Application)

  typedef FloatVectorImageType::PixelType         PixelType;
  typedef FloatVectorImageType::InternalPixelType InternalPixelType;

  // Training vectordata
  typedef itk::VariableLengthVector<InternalPixelType> MeasurementType;

  // SampleList manipulation
  typedef otb::ListSampleGenerator<FloatVectorImageType, VectorDataType> ListSampleGeneratorType;

  typedef ListSampleGeneratorType::ListSampleType ListSampleType;
  typedef ListSampleGeneratorType::LabelType LabelType;
  typedef ListSampleGeneratorType::ListLabelType LabelListSampleType;
  typedef otb::Statistics::ConcatenateSampleListFilter<ListSampleType> ConcatenateListSampleFilterType;
  typedef otb::Statistics::ConcatenateSampleListFilter<LabelListSampleType> ConcatenateLabelListSampleFilterType;

  // Statistic XML file Reader
  typedef otb::StatisticsXMLFileReader<MeasurementType> StatisticsReader;

  // Enhance List Sample  typedef otb::Statistics::ListSampleToBalancedListSampleFilter<ListSampleType, LabelListSampleType>      BalancingListSampleFilterType;
  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  // Machine Learning models
  typedef otb::MachineLearningModelFactory<InternalPixelType, ListSampleGeneratorType::ClassLabelType> MachineLearningModelFactoryType;
  typedef MachineLearningModelFactoryType::MachineLearningModelTypePointer ModelPointerType;

#ifdef OTB_USE_OPENCV
  typedef otb::RandomForestsMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> RandomForestType;
  typedef otb::KNearestNeighborsMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> KNNType;
  typedef otb::SVMMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> SVMType;
  typedef otb::BoostMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> BoostType;
  typedef otb::DecisionTreeMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> DecisionTreeType;
  typedef otb::GradientBoostedTreeMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> GradientBoostedTreeType;
  typedef otb::NeuralNetworkMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> NeuralNetworkType;
  typedef otb::NormalBayesMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> NormalBayesType;
#endif

#ifdef OTB_USE_LIBSVM
  typedef otb::LibSVMMachineLearningModel<InternalPixelType, ListSampleGeneratorType::ClassLabelType> LibSVMType;
#endif

  // Estimate performance on validation sample
  typedef otb::ConfusionMatrixCalculator<LabelListSampleType, LabelListSampleType> ConfusionMatrixCalculatorType;
  typedef ConfusionMatrixCalculatorType::ConfusionMatrixType ConfusionMatrixType;
  typedef ConfusionMatrixCalculatorType::MapOfIndicesType MapOfIndicesType;
  typedef ConfusionMatrixCalculatorType::ClassLabelType ClassLabelType;


  // VectorData projection filter
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> VectorDataProjectionFilterType;

  // Extract ROI
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, FloatVectorImageType> VectorDataReprojectionType;

protected:
  using Superclass::AddParameter;
  friend void InitSVMParams(TrainImagesClassifier & app);

private:
  void DoInit();

  void DoUpdateParameters();

  void LogConfusionMatrix(ConfusionMatrixCalculatorType* confMatCalc);

#ifdef OTB_USE_LIBSVM
  void InitLibSVMParams();
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
#endif

#ifdef OTB_USE_LIBSVM
  void TrainLibSVM(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
#endif

#ifdef OTB_USE_OPENCV
  void TrainBoost(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
  void TrainSVM(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
  void TrainDecisionTree(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
  void TrainGradientBoostedTree(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
  void TrainNeuralNetwork(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
  void TrainNormalBayes(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
  void TrainRandomForests(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
  void TrainKNN(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample);
#endif

  void Classify(ListSampleType::Pointer validationListSample, LabelListSampleType::Pointer predictedList);

  void DoExecute();

  VectorDataReprojectionType::Pointer vdreproj;
};

}
}


