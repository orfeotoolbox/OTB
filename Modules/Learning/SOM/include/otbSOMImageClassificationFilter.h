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

#ifndef otbSOMImageClassificationFilter_h
#define otbSOMImageClassificationFilter_h

#include "otbSOMClassifier.h"
#include "itkInPlaceImageFilter.h"
#include "itkListSample.h"

namespace otb
{
/** \class SOMImageClassificationFilter
 *  \brief This filter performs the classification of a VectorImage using a given SOM map.
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
 * \ingroup OTBSOM
 */
template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage = TOutputImage>
class ITK_EXPORT SOMImageClassificationFilter
  : public itk::InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef SOMImageClassificationFilter                       Self;
  typedef itk::InPlaceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SOMImageClassificationFilter, InPlaceImageFilter);

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

  typedef TSOMMap                        SOMMapType;
  typedef typename SOMMapType::Pointer   SOMMapPointerType;
  typedef typename SOMMapType::PixelType SampleType;

  typedef itk::Statistics::ListSample<SampleType>                   ListSampleType;
  typedef typename ListSampleType::Pointer                          ListSamplePointerType;
  typedef otb::SOMClassifier<ListSampleType, SOMMapType, LabelType> ClassifierType;
  typedef typename ClassifierType::Pointer                          ClassifierPointerType;

  /** Set/Get the svm model */
  itkSetObjectMacro(Map, SOMMapType);
  itkGetObjectMacro(Map, SOMMapType);

  /** Set/Get the default label */
  itkSetMacro(DefaultLabel, LabelType);
  itkGetMacro(DefaultLabel, LabelType);

  /**
   * If set, only pixels within the mask will be classified.
   * \param mask The input mask.
   */
  void SetInputMask(const MaskImageType * mask);

  /**
   * Get the input mask.
   * \return The mask.
   */
  const MaskImageType * GetInputMask(void);

protected:
  /** Constructor */
  SOMImageClassificationFilter();
  /** Destructor */
  ~SOMImageClassificationFilter() override {}

  /** Threaded generate data */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  /** Before threaded generate data */
  void BeforeThreadedGenerateData() override;
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SOMImageClassificationFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** The SOM model used for classification */
  SOMMapPointerType m_Map;
  /** Default label for invalid pixels (when using a mask) */
  LabelType m_DefaultLabel;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMImageClassificationFilter.hxx"
#endif

#endif
