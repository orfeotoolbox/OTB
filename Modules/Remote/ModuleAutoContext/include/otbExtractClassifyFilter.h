/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbExtractClassifyFilter_h
#define otbExtractClassifyFilter_h

#include "otbPersistentSamplingFilterBase.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbImage.h"
#include "otbMachineLearningModel.h"
#include "otbMachineLearningModelFactory.h"
#include <string>

namespace otb
{

/**
 * \class PersistentExtractClassifyFilter
 * 
 * \brief Persistent filter to extract sample values from an image, and immediately classify them in memory
 * 
 * \ingroup OTBSampling
 */
  template<class TInputImage, class TOutputLabel>
class ITK_EXPORT PersistentExtractClassifyFilter :
  public PersistentSamplingFilterBase<TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentExtractClassifyFilter                 Self;
  typedef PersistentSamplingFilterBase<TInputImage>       Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef TInputImage                                     InputImageType;
  typedef typename InputImageType::Pointer                InputImagePointer;
  typedef typename InputImageType::RegionType             RegionType;
  typedef typename InputImageType::PointType              PointType;
  typedef typename InputImageType::IndexType              IndexType;
  typedef typename InputImageType::PixelType              PixelType;
  typedef typename InputImageType::InternalPixelType      InternalPixelType;
  typedef itk::VariableLengthVector<double>               SampleType;

  typedef ogr::DataSource                                 OGRDataType;
  typedef ogr::DataSource::Pointer                        OGRDataPointer;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  typedef MachineLearningModel<SampleType, TOutputLabel>         MachineLearningModelType;
  typedef MachineLearningModelFactory<SampleType, TOutputLabel>  MachineLearningModelFactoryType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentExtractClassifyFilter, PersistentSamplingFilterBase);

  /** Set the output samples OGR container
   * (shall be equal to the input container for an 'update' mode) */
  void SetOutputSamples(ogr::DataSource* data);

  /** Get the output samples OGR container */
  ogr::DataSource* GetOutputSamples();

  void Synthetize(void) override{}

  /** Reset method called before starting the streaming*/
  void Reset(void) override;
  
  itkSetMacro(LabelFieldName, std::string);
  itkGetMacro(LabelFieldName, std::string);

  itkSetMacro(ModelString, std::string);
  itkGetMacro(ModelString, std::string);

  void SetExtraFields(std::vector<std::string> const& name);
  std::vector<std::string> GetExtraFields() const;
  
protected:
  /** Constructor */
  PersistentExtractClassifyFilter();
  /** Destructor */
  ~PersistentExtractClassifyFilter() override {}

  void GenerateOutputInformation() override;

  void GenerateInputRequestedRegion() override;

  /** process only points */
  void ThreadedGenerateVectorData(const ogr::Layer& layerForThread, itk::ThreadIdType threadid) override;
  
private:
  PersistentExtractClassifyFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Initialize fields to store extracted values (Label type) */
  void InitializeFields();

  /** Initialize output data source **/

  void InitializeOutputDataSource(ogr::DataSource* inputDS, ogr::DataSource* outputDS) override;

  /** Name of output field for label **/ 
  std::string m_LabelFieldName;

  /** Location of model used for the classification **/
  std::string m_ModelString;

  /** Classifier implemented as a Machine Learning Model */
  typename MachineLearningModelType::Pointer m_Model;

  /** Extra Fields in input shapefile that should be used for classification **/
  std::vector<std::string> m_ExtraFields;
};

/**
 * \class ExtractClassifyFilter
 * 
 * \brief Extract sample values from an image into an OGRDataSource using a persistent filter, and immediately classify them in memory
 * 
 * \sa PersistentExtractClassifyFilter
 *
 * \ingroup OTBSampling
 */
  template<class TInputImage, class TOutputLabel>
class ITK_EXPORT ExtractClassifyFilter :
    public PersistentFilterStreamingDecorator<PersistentExtractClassifyFilter<TInputImage, TOutputLabel> >
{
public:
  /** Standard Self typedef */
  typedef ExtractClassifyFilter      Self;
  typedef PersistentFilterStreamingDecorator
    <PersistentExtractClassifyFilter
     <TInputImage, TOutputLabel> >                     Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  typedef TInputImage                     InputImageType;
  typedef otb::ogr::DataSource            OGRDataType;
  
  typedef typename Superclass::FilterType             FilterType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ExtractClassifyFilter, PersistentFilterStreamingDecorator);

  using Superclass::SetInput;
  virtual void SetInput(const TInputImage* image);

  const TInputImage* GetInput();

  void SetSamplePositions(const otb::ogr::DataSource* data);
  const otb::ogr::DataSource* GetSamplePositions();
  
  void SetOutputSamples(OGRDataType::Pointer data);
  const otb::ogr::DataSource* GetOutputSamples();
  
  void SetLayerIndex(int index);
  int GetLayerIndex();

  void SetLabelFieldName(const std::string &name);
  const std::string GetLabelFieldName();

  void SetModelString(const std::string &name);
  const std::string GetModelString();

  /** Remove this when field name issue is resolved **/
  
  void SetFieldName(const std::string &name);
  const std::string GetFieldName();

  void SetExtraFields(const std::vector<std::string> &name);
  const std::vector<std::string> GetExtraFields();

protected:
  /** Constructor */
  ExtractClassifyFilter() {}
  /** Destructor */
  ~ExtractClassifyFilter() override {}

private:
  ExtractClassifyFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbExtractClassifyFilter.txx"
#endif

#endif
