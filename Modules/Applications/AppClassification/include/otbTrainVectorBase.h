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
#ifndef otbTrainVectorBase_h
#define otbTrainVectorBase_h

#include "otbLearningApplicationBase.h"
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"
#include "otbStatisticsXMLFileWriter.h"

#include "itkVariableLengthVector.h"
#include "otbStatisticsXMLFileReader.h"

#include "itkListSample.h"
#include "otbShiftScaleSampleListFilter.h"

#include <algorithm>
#include <locale>
#include <string>

namespace otb
{
namespace Wrapper
{

/** Utility function to negate std::isalnum */
bool IsNotAlphaNum(char c)
{
  return !std::isalnum( c );
}

class TrainVectorBase : public LearningApplicationBase<float, int>
{
public:
  /** Standard class typedefs. */
  typedef TrainVectorBase Self;
  typedef LearningApplicationBase<float, int> Superclass;
  typedef itk::SmartPointer <Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkTypeMacro(Self, Superclass);

  typedef Superclass::SampleType SampleType;
  typedef Superclass::ListSampleType ListSampleType;
  typedef Superclass::TargetListSampleType TargetListSampleType;

  typedef double ValueType;
  typedef itk::VariableLengthVector <ValueType> MeasurementType;

  typedef otb::StatisticsXMLFileReader<SampleType> StatisticsReader;

  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

protected:

  /** Class used to store statistics Measurment (mean/stddev) */
  class ShiftScaleParameters
  {
  public:
    MeasurementType meanMeasurementVector;
    MeasurementType stddevMeasurementVector;
  };

  /** Class used to store a list of sample and the corresponding label */
  class SamplesWithLabel
  {
  public:
    ListSampleType::Pointer listSample;
    TargetListSampleType::Pointer labeledListSample;
    SamplesWithLabel()
    {
      listSample = ListSampleType::New();
      labeledListSample = TargetListSampleType::New();
    }
  };

  /**
   * Features information class used to store information
   * about the field and class name/id of an input vector
   */
  class FeaturesInfo
  {
  public:
    /** Selected Index */
    std::vector<int> m_SelectedIdx;
    /** Index for class field */
    std::vector<int> m_SelectedCFieldIdx;
    /** Selected class field name */
    std::string m_SelectedCFieldName;
    /** Selected names */
    std::vector <std::string> m_SelectedNames;
    unsigned int m_NbFeatures;

    void SetFieldNames(std::vector <std::string> fieldNames, std::vector<int> selectedIdx)
    {
      m_SelectedIdx = selectedIdx;
      m_NbFeatures = static_cast<unsigned int>(selectedIdx.size());
      m_SelectedNames = std::vector<std::string>( m_NbFeatures );
      for( unsigned int i = 0; i < m_NbFeatures; ++i )
        {
        m_SelectedNames[i] = fieldNames[selectedIdx[i]];
        }
    }
    void SetClassFieldNames(std::vector<std::string> cFieldNames, std::vector<int> selectedCFieldIdx)
    {
      m_SelectedCFieldIdx = selectedCFieldIdx;
      // Handle only one class field name, if several are provided only the first one is used.
      m_SelectedCFieldName = selectedCFieldIdx.empty() ? cFieldNames.front() : cFieldNames[selectedCFieldIdx.front()];
    }
  };


protected:

  /**
   * Function which extract and store all samples for Training and Classification.
   * \param measurement statics measurement (mean/stddev)
   * \param featuresInfo information about the features
   */
  virtual void ExtractAllSamples(const ShiftScaleParameters &measurement);

  /**
  * Extract the training sample list
  * \param measurement statics measurement (mean/stddev)
  * \param featuresInfo information about the features
  * \return sample list used for training
  */
  virtual SamplesWithLabel ExtractTrainingSamplesWithLabel(const ShiftScaleParameters &measurement);

  /**
   * Extract classification the sample list
   * \param measurement statics measurement (mean/stddev)
   * \param featuresInfo information about the features
   * \return sample list used for classification
   */
  virtual SamplesWithLabel ExtractClassificationSamplesWithLabel(const ShiftScaleParameters &measurement);


  /** Extract samples from input file for corresponding field name
   *
   * \param parameterName the name of the input file option in the input application parameters
   * \param parameterLayer the name of the layer option in the input application parameters
   * \param measurement statics measurement (mean/stddev)
   * \param nbFeatures the number of features.
   * \return the list of samples and their corresponding labels.
   */
  SamplesWithLabel
  ExtractSamplesWithLabel(std::string parameterName, std::string parameterLayer, const ShiftScaleParameters &measurement);


  /**
   * Retrieve statistics mean and standard deviation if input statistics are provided.
   * Otherwise mean is set to 0 and standard deviation to 1 for each Features.
   * \param nbFeatures
   */
  ShiftScaleParameters GetStatistics(unsigned int nbFeatures);

  SamplesWithLabel m_TrainingSamplesWithLabel;
  SamplesWithLabel m_ClassificationSamplesWithLabel;
  TargetListSampleType::Pointer m_PredictedList;
  FeaturesInfo m_FeaturesInfo;

  void DoInit() override;
  void DoUpdateParameters() override;
  void DoExecute() override;

};

}
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTrainVectorBase.hxx"
#endif

#endif
