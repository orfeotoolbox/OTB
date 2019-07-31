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
  typedef VectorPrediction              Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Self, Application)

  /** Filters typedef */
  typedef float                                                         ValueType;
  // Label type is float for regression and unsigned int for classification 
  typedef typename std::conditional<RegressionMode, float, unsigned int>::type   LabelType;
  
  typedef itk::FixedArray<LabelType,1>                  LabelSampleType;
  typedef itk::Statistics::ListSample<LabelSampleType>  LabelListSampleType;

  typedef otb::MachineLearningModel<ValueType,LabelType>          MachineLearningModelType;
  typedef otb::MachineLearningModelFactory<ValueType, LabelType>  MachineLearningModelFactoryType;
  typedef typename MachineLearningModelType::Pointer                       ModelPointerType;
  typedef typename MachineLearningModelType::ConfidenceListSampleType      ConfidenceListSampleType;

  /** Statistics Filters typedef */
  typedef itk::VariableLengthVector<ValueType>                    MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>           StatisticsReader;

  typedef itk::VariableLengthVector<ValueType>                    InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType>            ListSampleType;
  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  ~VectorPrediction() override
    {
    MachineLearningModelFactoryType::CleanFactories();
    }

private:
  /** Utility function to negate std::isalnum */
  static bool IsNotAlphaNum(char c)
  {
  return !std::isalnum(c);
  }
  
  void DoInit() override;

  void DoInitSpecialization();

  void DoUpdateParameters() override;

  void DoExecute() override;

  ModelPointerType m_Model;
};

}
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorPrediction.hxx"
#endif

#endif
