/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2016-2019 IRSTEA
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
#ifndef __StreamingMosaicFilterWithBlendingBase_H
#define __StreamingMosaicFilterWithBlendingBase_H

#include "otbStreamingMosaicFilterBase.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "otbStreamingTraits.h"

namespace otb
{
/** \class StreamingMosaicFilterWithBlendingBase
 * \brief Base class for mosaic filters with blending
 *
 * Input must be couples of images: one image (e.g. otb::VectorImage) and
 * one distance image (e.g. otb::Image) which represents the distance between
 * a pixel an the edge of the image.
 * This distance image will be then used to compute the blending of the input images.
 * Two parameters can be set:
 * -Distance image interpolator: explicit
 * -Distance image offset: this value is added to the interpolated distance image
 *                         value, which allows creating a margin around the edges
 *                         of the image (i.e. around the 0-value distance image contour)
 *
 * Support streaming
 *
 * \ingroup OTBMosaic
 */
template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType = double>
class ITK_EXPORT StreamingMosaicFilterWithBlendingBase : public otb::StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>
{
public:
  /** Standard class typedef */
  typedef StreamingMosaicFilterWithBlendingBase Self;
  typedef otb::StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(StreamingMosaicFilterWithBlendingBase, StreamingMosaicFilterBase);

  /** Distance image typedefs */
  typedef TDistanceImage                                                  DistanceImageType;
  typedef typename DistanceImageType::Pointer                             DistanceImagePointer;
  typedef typename DistanceImageType::ConstPointer                        DistanceImageConstPointer;
  typedef typename DistanceImageType::PointType                           DistanceImagePointType;
  typedef typename DistanceImageType::PixelType                           DistanceImagePixelType;
  typedef typename DistanceImageType::InternalPixelType                   DistanceImageInternalPixelType;
  typedef typename itk::InterpolateImageFunction<DistanceImageType>       DistanceImageInterpolatorType;
  typedef typename itk::LinearInterpolateImageFunction<DistanceImageType> DistanceImageDefaultInterpolatorType;
  typedef typename DistanceImageInterpolatorType::Pointer                 DistanceImageInterpolatorPointer;
  typedef typename DistanceImageType::RegionType                          DistanceImageRegionType;

  /** Distance offset accessors */
  itkSetMacro(DistanceOffset, DistanceImageInternalPixelType);
  itkGetMacro(DistanceOffset, DistanceImageInternalPixelType);

  /** Set/Get the distance interpolator function. */
  itkSetObjectMacro(DistanceInterpolator, DistanceImageInterpolatorType);
  itkGetObjectMacro(DistanceInterpolator, DistanceImageInterpolatorType);

  /** Method to add one image
   * Two images are required
   * -1 input image (must be TInputImage type)
   * -1 input distance image (must be TDistanceImage: distance to the edges of the image)
   */
  void PushBackInputs(const TInputImage* image, const TDistanceImage* distance)
  {
    this->PushBackInput(const_cast<TInputImage*>(image));
    this->PushBackInput(const_cast<TDistanceImage*>(distance));
  }

  /** Prepare interpolators, valid regions, and input images pointers */
  virtual void PrepareDistanceImageAccessors(typename std::vector<DistanceImageType*>&               currentDistanceImage,
                                             typename std::vector<DistanceImageInterpolatorPointer>& distanceInterpolator);

protected:
  StreamingMosaicFilterWithBlendingBase();
  ~StreamingMosaicFilterWithBlendingBase()
  {
  }

  /* Overrided methods */
  void AddUsedInputImageIndex(unsigned int inputImageIndex) override;

  unsigned int GetNumberOfInputImages() override;

  unsigned int GetUsedInputImageIndice(unsigned int i) override
  {
    return 0.5 * Superclass::GetUsedInputImageIndice(i);
  }

private:
  StreamingMosaicFilterWithBlendingBase(const Self&); // purposely not
  // implemented
  void operator=(const Self&); // purposely not
  // implemented

  DistanceImageInternalPixelType   m_DistanceOffset;       // distance offset
  DistanceImageInterpolatorPointer m_DistanceInterpolator; // distance image
                                                           // interpolator

}; // end of class

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingMosaicFilterWithBlendingBase.hxx"
#endif

#endif
