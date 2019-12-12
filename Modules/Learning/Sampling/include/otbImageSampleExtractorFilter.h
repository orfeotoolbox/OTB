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

#ifndef otbImageSampleExtractorFilter_h
#define otbImageSampleExtractorFilter_h

#include "otbPersistentSamplingFilterBase.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbImage.h"
#include <string>

namespace otb
{

/**
 * \class PersistentImageSampleExtractorFilter
 *
 * \brief Persistent filter to extract sample values from an image
 *
 * \ingroup OTBSampling
 */
template <class TInputImage>
class ITK_EXPORT PersistentImageSampleExtractorFilter : public PersistentSamplingFilterBase<TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentImageSampleExtractorFilter      Self;
  typedef PersistentSamplingFilterBase<TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::Pointer           InputImagePointer;
  typedef typename InputImageType::RegionType        RegionType;
  typedef typename InputImageType::PointType         PointType;
  typedef typename InputImageType::IndexType         IndexType;
  typedef typename InputImageType::PixelType         PixelType;
  typedef typename InputImageType::InternalPixelType InternalPixelType;

  typedef ogr::DataSource          OGRDataType;
  typedef ogr::DataSource::Pointer OGRDataPointer;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentImageSampleExtractorFilter, PersistentSamplingFilterBase);

  /** Set the output samples OGR container
   * (shall be equal to the input container for an 'update' mode) */
  void SetOutputSamples(ogr::DataSource* data);

  /** Get the output samples OGR container */
  ogr::DataSource* GetOutputSamples();

  void Synthetize(void) override
  {
  }

  /** Reset method called before starting the streaming*/
  void Reset(void) override;

  itkSetMacro(SampleFieldPrefix, std::string);
  itkGetMacro(SampleFieldPrefix, std::string);

  /** Directly set the output field names (the prefix won't be used) */
  void SetSampleFieldNames(std::vector<std::string>& names);

  /** Get the sample names */
  const std::vector<std::string>& GetSampleFieldNames();

protected:
  /** Constructor */
  PersistentImageSampleExtractorFilter();
  /** Destructor */
  ~PersistentImageSampleExtractorFilter() override
  {
  }

  void GenerateOutputInformation() override;

  void GenerateInputRequestedRegion() override;

  /** process only points */
  void ThreadedGenerateVectorData(const ogr::Layer& layerForThread, itk::ThreadIdType threadid) override;

private:
  PersistentImageSampleExtractorFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Initialize fields to store extracted values (Real type) */
  void InitializeFields();

  /** Prefix to generate field names for each input channel
   *  (ignored if the field names are given directly) */
  std::string m_SampleFieldPrefix;

  /** List of field names for each component */
  std::vector<std::string> m_SampleFieldNames;
};

/**
 * \class ImageSampleExtractorFilter
 *
 * \brief Extract sample values from an image into an OGRDataSource using a persistent filter
 *
 * \sa PersistentImageSampleExtractorFilter
 *
 * \ingroup OTBSampling
 */
template <class TInputImage>
class ITK_EXPORT ImageSampleExtractorFilter : public PersistentFilterStreamingDecorator<PersistentImageSampleExtractorFilter<TInputImage>>
{
public:
  /** Standard Self typedef */
  typedef ImageSampleExtractorFilter                                                            Self;
  typedef PersistentFilterStreamingDecorator<PersistentImageSampleExtractorFilter<TInputImage>> Superclass;
  typedef itk::SmartPointer<Self>                                                               Pointer;
  typedef itk::SmartPointer<const Self>                                                         ConstPointer;

  typedef TInputImage          InputImageType;
  typedef otb::ogr::DataSource OGRDataType;

  typedef typename Superclass::FilterType FilterType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageSampleExtractorFilter, PersistentFilterStreamingDecorator);

  using Superclass::SetInput;
  virtual void SetInput(const TInputImage* image);

  const TInputImage* GetInput();

  void SetSamplePositions(const otb::ogr::DataSource* data);
  const otb::ogr::DataSource* GetSamplePositions();

  void SetOutputSamples(OGRDataType::Pointer data);
  const otb::ogr::DataSource* GetOutputSamples();

  void SetOutputFieldPrefix(const std::string& key);
  std::string GetOutputFieldPrefix();

  /** Set the output field names */
  void SetOutputFieldNames(std::vector<std::string>& names);

  /** Get the output field names */
  const std::vector<std::string>& GetOutputFieldNames();

  void SetLayerIndex(int index);
  int GetLayerIndex();

  void SetClassFieldName(const std::string& name);
  std::string GetClassFieldName(void);

protected:
  /** Constructor */
  ImageSampleExtractorFilter()
  {
  }
  /** Destructor */
  ~ImageSampleExtractorFilter() override
  {
  }

private:
  ImageSampleExtractorFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageSampleExtractorFilter.hxx"
#endif

#endif
