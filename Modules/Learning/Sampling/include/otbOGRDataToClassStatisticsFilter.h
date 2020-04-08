/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOGRDataToClassStatisticsFilter_h
#define otbOGRDataToClassStatisticsFilter_h

#include "otbPersistentSamplingFilterBase.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"
#include <string>

namespace otb
{

/**
 * \class PersistentOGRDataToClassStatisticsFilter
 *
 * \brief Persistent filter to compute class statistics based on vectors
 *
 * \ingroup OTBSampling
 */
template <class TInputImage, class TMaskImage>
class ITK_EXPORT PersistentOGRDataToClassStatisticsFilter : public PersistentSamplingFilterBase<TInputImage, TMaskImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentOGRDataToClassStatisticsFilter Self;
  typedef PersistentSamplingFilterBase<TInputImage, TMaskImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointer;
  typedef typename InputImageType::RegionType RegionType;
  typedef typename InputImageType::PointType  PointType;

  /** Wrap output type as DataObject */
  typedef std::map<std::string, unsigned long>   ClassCountMapType;
  typedef std::map<unsigned long, unsigned long> PolygonSizeMapType;
  typedef itk::SimpleDataObjectDecorator<ClassCountMapType>  ClassCountObjectType;
  typedef itk::SimpleDataObjectDecorator<PolygonSizeMapType> PolygonSizeObjectType;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentOGRDataToClassStatisticsFilter, PersistentSamplingFilterBase);

  void Synthetize(void) override;

  /** Reset method called before starting the streaming*/
  void Reset(void) override;

  /** the class count map is stored as output #2 */
  const ClassCountObjectType* GetClassCountOutput() const;
  ClassCountObjectType*       GetClassCountOutput();

  /** the polygon size map is stored as output #3 */
  const PolygonSizeObjectType* GetPolygonSizeOutput() const;
  PolygonSizeObjectType*       GetPolygonSizeOutput();

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

protected:
  /** Constructor */
  PersistentOGRDataToClassStatisticsFilter();
  /** Destructor */
  ~PersistentOGRDataToClassStatisticsFilter() override
  {
  }

  /** Implement generic method called at each candidate position */
  void ProcessSample(const ogr::Feature& feature, typename TInputImage::IndexType& imgIndex, typename TInputImage::PointType& imgPoint,
                     itk::ThreadIdType& threadid) override;

  /** Prepare temporary variables for the current feature */
  void PrepareFeature(const ogr::Feature& feature, itk::ThreadIdType& threadid) override;

private:
  PersistentOGRDataToClassStatisticsFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Number of pixels in all the polygons (per thread) */
  std::vector<unsigned long> m_NbPixelsThread;
  /** Number of pixels in each classes (per thread) */
  std::vector<ClassCountMapType> m_ElmtsInClassThread;
  /** Number of pixels in each polygons (per thread) */
  std::vector<PolygonSizeMapType> m_PolygonThread;
  /** Class name of the current feature (per thread) */
  std::vector<std::string> m_CurrentClass;
  /** FID of the current feature (per thread) */
  std::vector<unsigned long> m_CurrentFID;
};

/**
 * \class OGRDataToClassStatisticsFilter
 *
 * \brief Computes class statistics based on vectors using a persistent filter
 *
 * \sa PersistentOGRDataToClassStatisticsFilter
 *
 * \ingroup OTBSampling
 */
template <class TInputImage, class TMaskImage>
class ITK_EXPORT OGRDataToClassStatisticsFilter : public PersistentFilterStreamingDecorator<PersistentOGRDataToClassStatisticsFilter<TInputImage, TMaskImage>>
{
public:
  /** Standard Self typedef */
  typedef OGRDataToClassStatisticsFilter Self;
  typedef PersistentFilterStreamingDecorator<PersistentOGRDataToClassStatisticsFilter<TInputImage, TMaskImage>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInputImage          InputImageType;
  typedef TMaskImage           MaskImageType;
  typedef otb::ogr::DataSource OGRDataType;

  typedef typename Superclass::FilterType            FilterType;
  typedef typename FilterType::ClassCountMapType     ClassCountMapType;
  typedef typename FilterType::PolygonSizeMapType    PolygonSizeMapType;
  typedef typename FilterType::ClassCountObjectType  ClassCountObjectType;
  typedef typename FilterType::PolygonSizeObjectType PolygonSizeObjectType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(OGRDataToClassStatisticsFilter, PersistentFilterStreamingDecorator);

  using Superclass::SetInput;
  virtual void SetInput(const TInputImage* image);

  const TInputImage* GetInput();

  void SetOGRData(const otb::ogr::DataSource* data);
  const otb::ogr::DataSource* GetOGRData();

  void SetMask(const TMaskImage* mask);
  const TMaskImage* GetMask();

  void SetFieldName(std::string& key);
  std::string GetFieldName();

  void SetLayerIndex(int index);
  int GetLayerIndex();

  const ClassCountObjectType* GetClassCountOutput() const;
  ClassCountObjectType*       GetClassCountOutput();

  const PolygonSizeObjectType* GetPolygonSizeOutput() const;
  PolygonSizeObjectType*       GetPolygonSizeOutput();

protected:
  /** Constructor */
  OGRDataToClassStatisticsFilter()
  {
  }
  /** Destructor */
  ~OGRDataToClassStatisticsFilter() override
  {
  }

private:
  OGRDataToClassStatisticsFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRDataToClassStatisticsFilter.hxx"
#endif

#endif
