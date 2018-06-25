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

#ifndef otbMorphologicalPyramidResampler_hxx
#define otbMorphologicalPyramidResampler_hxx
#include "otbMorphologicalPyramidResampler.h"
#include "itkResampleImageFilter.h"
#include "itkScalableAffineTransform.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkProgressAccumulator.h"

namespace otb
{
namespace MorphologicalPyramid
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
Resampler<TInputImage, TOutputImage>
::Resampler()
{
  for (unsigned int i = 0; i < InputImageType::ImageDimension; ++i)
    {
    m_Size[i] = 0;
    }
}
/**
 * Configure input requested region to be the largest possible region.
 */
template <class TInputImage, class TOutputImage>
void
Resampler<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // Superclass method call
  Superclass::GenerateInputRequestedRegion();
  // Input and output image pointers retrieval
  InputImagePointer  inputPtr = const_cast<InputImageType *> (this->GetInput());
  OutputImagePointer outputPtr = this->GetOutput();
  // If the pointers are not correct
  if (!inputPtr || !outputPtr)
    {
    // exit
    return;
    }
  // else
  // Configure input requested region to be the largest possible region
  inputPtr->SetRequestedRegion(inputPtr->GetLargestPossibleRegion());
}
/**
 * Configure output requested region to be the largest possible region
 */
template <class TInputImage, class TOutputImage>
void
Resampler<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(itk::DataObject *itkNotUsed(output))
{
  this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
}
/**
 * Generate output information
 */
template <class TInputImage, class TOutputImage>
void
Resampler<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  // Superclass method call
  Superclass::GenerateOutputInformation();
  // Input and output image pointers retrieval
  InputImageConstPointer inputPtr  = this->GetInput();
  OutputImagePointer     outputPtr = this->GetOutput();
  // If the pointers are not correct
  if (!inputPtr || !outputPtr)
    {
    // exit
    return;
    }
  unsigned int i;
  // Computing output spacing, size and index from input data
  const typename InputImageType::SpacingType& inputSpacing    = inputPtr->GetSignedSpacing();
  const typename InputImageType::IndexType&   inputStartIndex = inputPtr->GetLargestPossibleRegion().GetIndex();
  typename OutputImageType::IndexType         outputStartIndex;
  typename OutputImageType::SpacingType       spacing;
  for (i = 0; i < OutputImageType::ImageDimension; ++i)
    {
    outputStartIndex[i] =  inputStartIndex[i];
    }

  outputPtr->SetSignedSpacing(inputSpacing);
  typename OutputImageType::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(this->GetSize());
  outputLargestPossibleRegion.SetIndex(outputStartIndex);
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void
Resampler<TInputImage, TOutputImage>
::GenerateData()
{
  typename OutputImageType::Pointer result;

  // Filters typedefs
  typedef itk::ResampleImageFilter<InputImageType, OutputImageType>   ResampleFilterType;
  typedef itk::ScalableAffineTransform<double, InputImageType::ImageDimension> TransformType;
  typedef itk::LinearInterpolateImageFunction<InputImageType, double> InterpolatorType;

  // Resampling filter creation
  typename ResampleFilterType::Pointer resampler = ResampleFilterType::New();
  typename InterpolatorType::Pointer   interpolator = InterpolatorType::New();
  typename TransformType::Pointer      transform = TransformType::New();

  // Scale parameters computation
  typename TransformType::InputVectorType scales;
  typename InputImageType::SizeType    inputSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
  typename InputImageType::SpacingType inputSpacing = this->GetInput()->GetSignedSpacing();
  scales[0] = static_cast<double>(inputSize[0]) / static_cast<double>(m_Size[0]);
  scales[1] = static_cast<double>(inputSize[1]) / static_cast<double>(m_Size[1]);
  transform->SetScale(scales);
  transform->SetCenter(this->GetInput()->GetOrigin());
  typename TransformType::OutputVectorType translation;
  translation[0] = 0.5 * inputSpacing[0] * (scales[0] - 1.0);
  translation[1] = 0.5 * inputSpacing[1] * (scales[1] - 1.0);
  transform->SetTranslation(translation);


  // Resampling filter set up
  resampler->SetTransform(transform);
  resampler->SetInterpolator(interpolator);
  resampler->SetOutputOrigin(this->GetInput()->GetOrigin());
  resampler->SetSize(this->GetSize());
  resampler->SetOutputSpacing( this->GetInput()->GetSpacing() );
  resampler->SetOutputDirection( this->GetInput()->GetDirection() );
  resampler->ReleaseDataFlagOn();

  // Progress accumulator
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(resampler, .5f);

  // Input image connexion
  resampler->SetInput(this->GetInput());
  resampler->Update();
  result = resampler->GetOutput();

  /** Output filter connexion */
  this->GraftOutput(result);
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputImage>
void
Resampler<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Size: " << m_Size << std::endl;
}
} // End namespace MorphologicalPyramid
} // End namespace otb
#endif
