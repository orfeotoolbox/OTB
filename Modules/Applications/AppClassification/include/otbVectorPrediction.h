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

#ifndef otbVectorPrediction_h
#define otbVectorPrediction_h

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"

#include "itkVariableLengthVector.h"
#include "otbStatisticsXMLFileReader.h"

#include "itkListSample.h"
#include "otbShiftScaleSampleListFilter.h"

#include "otbMachineLearningModelFactory.h"

#include "otbMachineLearningModel.h"

#include <time.h>

namespace otb
{
namespace Wrapper
{

template <bool RegressionMode>
class VectorPrediction : public Application
{
public:
  /** Standard class typedefs. */
  using Self         = VectorPrediction;
  using Superclass   = Application;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Self, Application)

      /** Filters typedef */
      using ValueType = float;
  // Label type is float for regression and unsigned int for classification
  using LabelType = typename std::conditional<RegressionMode, float, unsigned int>::type;

  using LabelSampleType     = itk::FixedArray<LabelType, 1>;
  using LabelListSampleType = itk::Statistics::ListSample<LabelSampleType>;

  using MachineLearningModelType        = otb::MachineLearningModel<ValueType, LabelType>;
  using MachineLearningModelFactoryType = otb::MachineLearningModelFactory<ValueType, LabelType>;
  using ModelPointerType                = typename MachineLearningModelType::Pointer;
  using ConfidenceListSampleType        = typename MachineLearningModelType::ConfidenceListSampleType;

  /** Statistics Filters typedef */
  using MeasurementType  = itk::VariableLengthVector<ValueType>;
  using StatisticsReader = otb::StatisticsXMLFileReader<MeasurementType>;

  using InputSampleType      = itk::VariableLengthVector<ValueType>;
  using ListSampleType       = itk::Statistics::ListSample<InputSampleType>;
  using ShiftScaleFilterType = otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType>;

  ~VectorPrediction() override
  {
    MachineLearningModelFactoryType::CleanFactories();
  }

private:
  void DoInit() override;

  /** Method defining the parameters used in the application and their documentation, specialized for RegressionMode=1 and RegrssionMode=0 */
  void DoInitSpecialization();

  void DoUpdateParameters() override;

  void DoExecute() override;

  /** Method returning whether the confidence map should be computed, depending on the regression mode and input parameters */
  bool shouldComputeConfidenceMap() const;

  /** Method returning the input list sample from the input layer */
  typename ListSampleType::Pointer ReadInputListSample(otb::ogr::Layer const& layer);

  /** Normalize a list sample using the statistic file given  */
  typename ListSampleType::Pointer NormalizeListSample(ListSampleType::Pointer input);

  /** Update the output DataSource : the input layer is buffered and the input data source is re opened in update mode. */
  otb::ogr::DataSource::Pointer UpdateOutputDataSource(otb::ogr::DataSource::Pointer source, otb::ogr::Layer& layer, otb::ogr::DataSource::Pointer buffer);

  /** Create the output DataSource. */
  otb::ogr::DataSource::Pointer CreateOutputDataSource(otb::ogr::DataSource::Pointer source, otb::ogr::Layer& layer);

  /** Add a prediction field in the output layer if it does not exist.
   * If computeConfidenceMap evaluates to true a confidence field will be
   * added. */
  void AddPredictionField(otb::ogr::Layer& outLayer, otb::ogr::Layer const& layer, bool computeConfidenceMap);

  /** Fill the output layer with the predicted values and optionnaly the confidence */
  void FillOutputLayer(otb::ogr::Layer& outLayer, otb::ogr::Layer const& layer, typename LabelListSampleType::Pointer target,
                       typename ConfidenceListSampleType::Pointer quality, bool updateMode, bool computeConfidenceMap);

  ModelPointerType m_Model;

  /** Name used for the confidence field */
  std::string confFieldName = "confidence";
};
}
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorPrediction.hxx"
#endif

#endif
