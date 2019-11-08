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

#ifndef otbKMeansImageClassificationFilter_h
#define otbKMeansImageClassificationFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkListSample.h"
#include "itkEuclideanDistanceMetric.h"

namespace otb
{
/** \class KMeansImageClassificationFilter
 *  \brief This filter performs the classification of a VectorImage using a KMeans estimation result.
 *
 *  This filter is streamed and threaded, allowing to classify huge images. Because the
 *  internal sample type has to be an itk::FixedArray, one must specify at compilation time
 *  the maximum sample dimension. It is up to the user to specify a MaxSampleDimension sufficiently
 *  high to integrate all its features. This filter internally use one SVMClassifier per thread.
 *
 * \sa SVMClassifier
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBLearningBase
 */
template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension = 10, class TMaskImage = TOutputImage>
class ITK_EXPORT KMeansImageClassificationFilter : public itk::InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef KMeansImageClassificationFilter Self;
  typedef itk::InPlaceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(KMeansImageClassificationFilter, InPlaceImageFilter);

  /** The max dimension of the sample to classify.
   *  This filter internally uses itk::FixedArray as input for the classifier,
   *  so the max sample size has to be fixed at compilation time.
   */
  itkStaticConstMacro(MaxSampleDimension, unsigned int, VMaxSampleDimension);

  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::ConstPointer      InputImageConstPointerType;
  typedef typename InputImageType::InternalPixelType ValueType;

  typedef TMaskImage                           MaskImageType;
  typedef typename MaskImageType::ConstPointer MaskImageConstPointerType;
  typedef typename MaskImageType::Pointer      MaskImagePointerType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType  LabelType;

  typedef itk::FixedArray<ValueType, MaxSampleDimension> SampleType;
  typedef itk::Array<double> KMeansParametersType;
  typedef std::map<LabelType, SampleType> CentroidsMapType;
  typedef itk::Statistics::EuclideanDistanceMetric<SampleType> DistanceType;

  /** Set/Get the centroids */
  itkSetMacro(Centroids, KMeansParametersType);
  itkGetConstReferenceMacro(Centroids, KMeansParametersType);

  /** Set/Get the default label */
  itkSetMacro(DefaultLabel, LabelType);
  itkGetMacro(DefaultLabel, LabelType);

  /**
   * If set, only pixels within the mask will be classified.
   * \param mask The input mask.
   */
  void SetInputMask(const MaskImageType* mask);

  /**
   * Get the input mask.
   * \return The mask.
   */
  const MaskImageType* GetInputMask(void);

protected:
  /** Constructor */
  KMeansImageClassificationFilter();
  /** Destructor */
  ~KMeansImageClassificationFilter() override
  {
  }

  /** Threaded generate data */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  /** Before threaded generate data */
  void BeforeThreadedGenerateData() override;
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  KMeansImageClassificationFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Centroids used for classification */
  KMeansParametersType m_Centroids;
  /** Default label for invalid pixels (when using a mask) */
  LabelType m_DefaultLabel;
  /** Centroids - labels map */
  CentroidsMapType m_CentroidsMap;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKMeansImageClassificationFilter.hxx"
#endif

#endif
