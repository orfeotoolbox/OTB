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
#ifndef __StreamingFeatherMosaicFilter_H
#define __StreamingFeatherMosaicFilter_H

#include "otbStreamingMosaicFilterWithBlendingBase.h"
#include "itkLinearInterpolateImageFunction.h"

namespace otb
{
/** \class StreamingFeatherMosaicFilter
 * \brief Computes the mosaic of an input images set using
 * feathering technique. The output is a nice seamless mosaic.
 *
 * Images are added-weighted in a one-by-one fashion, blended
 * from the images boundaries to a specified transition distance.
 * The smoothness of the transition curve can be set (exponent).
 * Feathering is performed with respect to the input images order.
 *
 * Inputs are images and distance images. Both needs to be added with the PushBackInputs() method.
 *
 * Support streaming
 *
 * The pixels must support the operator ==, +, /, etc.
 * The "no data value", output spacing, interpolator can be chosen.
 *
 * TODO:
 * -change containers from std::vector to itk::something
 * -move container class to private
 * -create accessors
 *
 * \ingroup OTBMosaic
 */
template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType = double>
class ITK_EXPORT StreamingFeatherMosaicFilter : public StreamingMosaicFilterWithBlendingBase<TInputImage, TOutputImage, TDistanceImage, TInternalValueType>
{
public:
  /** Standard Self typedef */
  typedef StreamingFeatherMosaicFilter Self;
  typedef StreamingMosaicFilterWithBlendingBase<TInputImage, TOutputImage, TDistanceImage, TInternalValueType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(StreamingFeatherMosaicFilter, StreamingMosaicFilterWithBlendingBase);

  /** Input image typedefs.  */
  typedef typename Superclass::InputImageType              InputImageType;
  typedef typename Superclass::InputImagePointer           InputImagePointer;
  typedef typename Superclass::InputImagePointType         InputImagePointType;
  typedef typename Superclass::InputImagePixelType         InputImagePixelType;
  typedef typename Superclass::InputImageIndexType         InputImageIndexType;
  typedef typename Superclass::InputImageSizeType          InputImageSizeType;
  typedef typename Superclass::InputImageSpacingType       InputImageSpacingType;
  typedef typename Superclass::InputImageInternalPixelType InputImageInternalPixelType;
  typedef typename Superclass::InputImageRegionType        InputImageRegionType;

  /** Output image typedefs.  */
  typedef typename Superclass::OutputImageType              OutputImageType;
  typedef typename Superclass::OutputImagePointer           OutputImagePointer;
  typedef typename Superclass::OutputImagePointType         OutputImagePointType;
  typedef typename Superclass::OutputImagePixelType         OutputImagePixelType;
  typedef typename Superclass::OutputImageIndexType         OutputImageIndexType;
  typedef typename Superclass::OutputImageSizeType          OutputImageSizeType;
  typedef typename Superclass::OutputImageSpacingType       OutputImageSpacingType;
  typedef typename Superclass::OutputImageInternalPixelType OutputImageInternalPixelType;
  typedef typename Superclass::OutputImageRegionType        OutputImageRegionType;

  /** Internal computing typedef support. */
  typedef typename Superclass::InternalValueType       InternalValueType;
  typedef typename Superclass::ContinuousIndexType     ContinuousIndexType;
  typedef typename Superclass::InterpolatorType        InterpolatorType;
  typedef typename Superclass::InterpolatorPointerType InterpolatorPointerType;
  typedef typename Superclass::DefaultInterpolatorType DefaultInterpolatorType;
  typedef typename Superclass::InternalImageType       InternalImageType;
  typedef typename Superclass::InternalPixelType       InternalPixelType;
  typedef typename Superclass::IteratorType            IteratorType;
  typedef typename Superclass::ConstIteratorType       ConstIteratorType;
  typedef typename Superclass::StreamingTraitsType     StreamingTraitsType;

  /** Distance image typedefs */
  typedef TDistanceImage                                                  DistanceImageType;
  typedef typename DistanceImageType::Pointer                             DistanceImagePointer;
  typedef typename DistanceImageType::ConstPointer                        DistanceImageConstPointer;
  typedef typename DistanceImageType::PointType                           DistanceImagePointType;
  typedef typename DistanceImageType::PixelType                           DistanceImagePixelType;
  typedef typename DistanceImageType::InternalPixelType                   DistanceImageInternalPixelType;
  typedef typename DistanceImageType::RegionType                          DistanceImageRegionType;
  typedef typename itk::InterpolateImageFunction<DistanceImageType>       DistanceImageInterpolatorType;
  typedef typename itk::LinearInterpolateImageFunction<DistanceImageType> DistanceImageDefaultInterpolatorType;
  typedef typename DistanceImageInterpolatorType::Pointer                 DistanceImageInterpolatorPointer;

  /** Feathering transition distance accessors */
  itkSetMacro(FeatheringTransitionDistance, InternalValueType);
  itkGetMacro(FeatheringTransitionDistance, InternalValueType);

  /** Feathering transition smoothness accessors */
  itkSetMacro(FeatheringSmoothness, InternalValueType);
  itkGetMacro(FeatheringSmoothness, InternalValueType);

protected:
  StreamingFeatherMosaicFilter();
  virtual ~StreamingFeatherMosaicFilter()
  {
  }

  /** Overrided methods */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  InternalValueType m_FeatheringTransitionDistance; // feathering transition
                                                    // distance
  InternalValueType m_FeatheringSmoothness;         // feathering exponent

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingFeatherMosaicFilter.hxx"
#endif

#endif
