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

#ifndef otbMorphologicalPyramidResampler_h
#define otbMorphologicalPyramidResampler_h

#include "itkImageToImageFilter.h"

namespace otb
{
namespace MorphologicalPyramid
{
/**
 * \class Resampler
 * \brief This class performs the resampling of the given image to the given size.
 *
 * This class performs the resampling of the given image to the given size.It is used
 * in the MorphologicalPyramidAnalyseFilter and MorphologicalPyramidSynthesisFilter.
 *
 * The resampling is done by the itk::ResampleImageFilter, templated with the interpolator
 * itk::LinearInterpolateImageFunction and the transform itk::ScaleTransform.
 *
 * \sa MorphologicalPyramidAnalyseFilter
 * \sa MorphologicalPyramidSynthesisFilter
 * \sa ResampleImageFilter
 * \sa LinearInterpolateImageFunction
 * \sa ScaleTransform
 *
 * \ingroup OTBMorphologicalPyramid
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT Resampler
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef Resampler                                          Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(Resampler, ImageToImageFilter);
  /** Template parameters typedefs */
  typedef TInputImage                           InputImageType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename TInputImage::PixelType       PixelType;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename OutputImageType::SizeType    SizeType;
  typedef typename OutputImageType::SpacingType SpacingType;
  /** Size parameter accessor */
  itkSetMacro(Size, SizeType);
  itkGetMacro(Size, SizeType);

protected:
  /** Constructor */
  Resampler();
  /** Destructor */
  ~Resampler() override {}
  /** Generate output information */
  void GenerateOutputInformation() override;
  /** Configure input requested region to be the largest possible region */
  void GenerateInputRequestedRegion() override;
  /** Configure output requested region to be the largest possible region */
  void EnlargeOutputRequestedRegion(itk::DataObject * itkNotUsed(output)) override;
  /** Main computation method */
  void GenerateData() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
private:
  Resampler(const Self &) = delete;
  void operator =(const Self&) = delete;
  /// Output image size
  SizeType m_Size;
};
} // End namespace MorphologicalPyramid
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidResampler.hxx"
#endif

#endif
