/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSampleAugmentationFilter_h
#define otbSampleAugmentationFilter_h

#include "itkProcessObject.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbSampleAugmentation.h"
#include "OTBSamplingExport.h"
#include <string>

namespace otb
{


/**
 * \class SampleAugmentationFilter
 *
 * \brief Filter to generate synthetic samples from existing ones
 *
 * This class generates synthetic samples from existing ones either by
 * replication, jitter (adding gaussian noise to the features of
 * existing samples) or SMOTE (linear combination of pairs
 * neighbouring samples of the same class.
 *
 * \ingroup OTBSampling
 */

class OTBSampling_EXPORT SampleAugmentationFilter : public itk::ProcessObject
{
public:
  /** typedef for the classes standards. */
  typedef SampleAugmentationFilter      Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(SampleAugmentationFilter, ProcessObject);

  typedef ogr::DataSource                     OGRDataSourceType;
  typedef typename OGRDataSourceType::Pointer OGRDataSourcePointerType;
  typedef ogr::Layer                          OGRLayerType;

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  using SampleType       = sampleAugmentation::SampleType;
  using SampleVectorType = sampleAugmentation::SampleVectorType;

  enum class Strategy
  {
    Replicate,
    Jitter,
    Smote
  };

  /** Set/Get the input OGRDataSource of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const OGRDataSourceType* ds);
  const OGRDataSourceType* GetInput(unsigned int idx);

  virtual void SetOutputSamples(ogr::DataSource* data);

  /** Set the Field Name in which labels will be written. (default is "class")
   * A field "ClassFieldName" of type integer is created in the output memory layer.
   */
  itkSetMacro(ClassFieldName, std::string);
  /**
   * Return the Field name in which labels have been written.
   */
  itkGetMacro(ClassFieldName, std::string);


  itkSetMacro(Layer, size_t);
  itkGetMacro(Layer, size_t);
  itkSetMacro(Label, int);
  itkGetMacro(Label, int);
  void SetStrategy(Strategy s)
  {
    m_Strategy = s;
  }
  Strategy GetStrategy() const
  {
    return m_Strategy;
  }
  itkSetMacro(NumberOfSamples, int);
  itkGetMacro(NumberOfSamples, int);
  void SetExcludedFields(const std::vector<std::string>& ef)
  {
    m_ExcludedFields = ef;
  }
  std::vector<std::string> GetExcludedFields() const
  {
    return m_ExcludedFields;
  }
  itkSetMacro(StdFactor, double);
  itkGetMacro(StdFactor, double);
  itkSetMacro(SmoteNeighbors, size_t);
  itkGetMacro(SmoteNeighbors, size_t);
  itkSetMacro(Seed, int);
  itkGetMacro(Seed, int);
  /**
     * Get the output \c ogr::DataSource which is a "memory" datasource.
     */
  const OGRDataSourceType* GetOutput();

protected:
  SampleAugmentationFilter();
  ~SampleAugmentationFilter() override
  {
  }

  /** Generate Data method*/
  void GenerateData() override;

  /** DataObject pointer */
  typedef itk::DataObject::Pointer DataObjectPointer;

  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;


  SampleVectorType ExtractSamples(const ogr::DataSource::Pointer vectors, size_t layerName, const std::string& classField, const int label,
                                  const std::vector<std::string>& excludedFields = {});

  void SampleToOGRFeatures(const ogr::DataSource::Pointer& vectors, ogr::DataSource* output, const SampleVectorType& samples, const size_t layerName,
                           const std::string& classField, int label, const std::vector<std::string>& excludedFields = {});

  std::set<size_t> GetExcludedFieldsIds(const std::vector<std::string>& excludedFields, const ogr::Layer& inputLayer);
  bool IsNumericField(const ogr::Feature& feature, const int idx);

  ogr::Feature SelectTemplateFeature(const ogr::Layer& inputLayer, const std::string& classField, int label);

private:
  SampleAugmentationFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::string              m_ClassFieldName;
  size_t                   m_Layer;
  int                      m_Label;
  std::vector<std::string> m_ExcludedFields;
  Strategy                 m_Strategy;
  int                      m_NumberOfSamples;
  double                   m_StdFactor;
  size_t                   m_SmoteNeighbors;
  int                      m_Seed;
};


} // end namespace otb

#endif
