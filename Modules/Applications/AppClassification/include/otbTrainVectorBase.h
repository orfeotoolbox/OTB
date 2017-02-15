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
  typedef TrainVectorBase Self;
  typedef LearningApplicationBase<float, int> Superclass;
  typedef itk::SmartPointer <Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkTypeMacro(Self, Superclass)

  typedef Superclass::SampleType SampleType;
  typedef Superclass::ListSampleType ListSampleType;
  typedef Superclass::TargetListSampleType TargetListSampleType;

  typedef double ValueType;
  typedef itk::VariableLengthVector <ValueType> MeasurementType;

  typedef otb::StatisticsXMLFileReader<SampleType> StatisticsReader;

  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

protected:

  /** Class used to store statistics Measurment (mean/stddev) */
  class StatisticsMeasurement
  {
  public:
    MeasurementType meanMeasurementVector;
    MeasurementType stddevMeasurementVector;
  };

  /** Class used to store a list of sample and the corresponding label */
  class ListSamples
  {
  public:
    ListSampleType::Pointer listSample;
    TargetListSampleType::Pointer labeledListSample;
    ListSamples()
    {
      listSample = ListSampleType::New();
      labeledListSample = TargetListSampleType::New();
    }
  };

  /**
   * Features information class used to store informations
   * about the field and class name/id of an input vector
   */
  class FeaturesInfo
  {
  public:
    /** Index for class field */
    std::vector<int> m_SelectedCFieldIdx;
    /** Selected Index */
    std::vector<int> m_SelectedIdx;
    /** Selected class field name */
    std::string m_SelectedCFieldName;
    /** Selected names */
    std::vector <std::string> m_SelectedNames;
    unsigned int m_NbFeatures;

    FeaturesInfo(std::vector <std::string> fieldNames, std::vector <std::string> cFieldNames,
                 std::vector<int> selectedIdx, std::vector<int> selectedCFieldIdx)
            : m_SelectedIdx( selectedIdx ), m_SelectedCFieldIdx( selectedCFieldIdx )
    {
      m_NbFeatures = static_cast<unsigned int>(selectedIdx.size());
      m_SelectedNames = std::vector<std::string>( m_NbFeatures );
      for( unsigned int i = 0; i < m_NbFeatures; ++i )
        {
        m_SelectedNames[i] = fieldNames[selectedIdx[i]];
        }

      m_SelectedCFieldName = cFieldNames[selectedCFieldIdx.front()];

    }
  };


protected:

  /**
   * Function which extract and store all samples for Training, Classification and Validation.
   * \param measurement statics measurement (mean/stddev)
   * \param featuresInfo information about the features
   * \return sample list used for training
   */
  virtual void ExtractSamples(const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo);

  /**
  * Extract the training sample list
  * \param measurement statics measurement (mean/stddev)
  * \param featuresInfo information about the features
  * \return sample list used for training
  */
  virtual ListSamples ExtractTrainingListSamples(const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo);

  /**
  * Extract the validation sample list
  * \param measurement statics measurement (mean/stddev)
  * \param featuresInfo information about the features
  * \return sample list used for validation
  */
  virtual ListSamples ExtractValidationListSamples(const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo);

  /**
   * Extract the sample list classification
   * \param measurement statics measurement (mean/stddev)
   * \param featuresInfo information about the features
   * \return sample list used for classification
   */
  virtual ListSamples ExtractClassificationListSamples(const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo);

  ListSamples trainingListSamples;
  ListSamples validationListSamples;
  ListSamples classificationListSamples;
  TargetListSampleType::Pointer predictedList;

private:
  virtual void DoTrainInit() = 0;
  virtual void DoTrainExecute() = 0;
  virtual void DoTrainUpdateParameters() = 0;

  void DoInit();
  void DoUpdateParameters();
  void DoExecute();

  /** Extract samples from input file for corresponding field name
 *
 * \param parameterName the name of the input file option in the input application parameters
 * \param parameterLayer the name of the layer option in the input application parameters
 * \param measurement statics measurement (mean/stddev)
 * \param nbFeatures the number of features.
 * \return the list of samples and their corresponding labels.
 */
  ListSamples ExtractListSamples(std::string parameterName, std::string parameterLayer,
                                                  const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo);



  ListSamples ExtractClassificationListSamples(ListSamples &validationListSamples, ListSamples &trainingListSamples);


  /**
  * Retrieve statistics mean and standard deviation if input statistics are provided.
  * Otherwise mean is set to 0 and standard deviation to 1 for each Features.
  * \param nbFeatures
  */
  StatisticsMeasurement ComputeStatistics(unsigned int nbFeatures);

};

}
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTrainVectorBase.txx"
#endif

#endif

