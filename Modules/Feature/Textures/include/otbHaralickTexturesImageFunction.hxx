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

#ifndef otbHaralickTexturesImageFunction_hxx
#define otbHaralickTexturesImageFunction_hxx

#include "otbHaralickTexturesImageFunction.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNumericTraits.h"
#include <complex>
#include <cmath>

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
HaralickTexturesImageFunction<TInputImage, TCoordRep>
::HaralickTexturesImageFunction() :
 m_NeighborhoodRadius(10),
 m_Offset(),
 m_NumberOfBinsPerAxis(8),
 m_InputImageMinimum(0),
 m_InputImageMaximum(255)
{}

template <class TInputImage, class TCoordRep>
void
HaralickTexturesImageFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " Neighborhood radius value   : "  << m_NeighborhoodRadius << std::endl;
  os << indent << " Input image minimum value   : "  << m_InputImageMinimum << std::endl;
  os << indent << " Input Image maximum value   : "  << m_InputImageMaximum << std::endl;
  os << indent << " Number of bins per axis     : "  << m_NumberOfBinsPerAxis << std::endl;
  os << indent << " Offset                      : "  << m_Offset << std::endl;
}

template <class TInputImage, class TCoordRep>
typename HaralickTexturesImageFunction<TInputImage, TCoordRep>::OutputType
HaralickTexturesImageFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  // Build textures vector
  OutputType textures;

  // Initialize textures
  textures.Fill( itk::NumericTraits< ScalarRealType >::Zero );

  // Check for input image
  if( !this->GetInputImage() )
    {
    return textures;
    }

  // Check for out of buffer
  if ( !this->IsInsideBuffer( index ) )
    {
    return textures;
    }

  const double log2 = vcl_log(2.0);
  // Retrieve the input pointer
  InputImagePointerType inputPtr = const_cast<InputImageType *> (this->GetInputImage());

  // Compute the region on which co-occurence will be estimated
  typename InputRegionType::IndexType inputIndex;
  typename InputRegionType::SizeType inputSize;

  for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
    {
    inputIndex[dim] = index[dim] - m_NeighborhoodRadius;
    inputSize[dim] = 2 * m_NeighborhoodRadius + 1;
    }

  // Build the input  region
  InputRegionType inputRegion;
  inputRegion.SetIndex(inputIndex);
  inputRegion.SetSize(inputSize);
  inputRegion.Crop(inputPtr->GetRequestedRegion());

  CooccurrenceIndexedListPointerType GLCIList = CooccurrenceIndexedListType::New();
  GLCIList->Initialize(m_NumberOfBinsPerAxis, m_InputImageMinimum, m_InputImageMaximum);

  // Next, find the minimum radius that encloses all the offsets.
  unsigned int minRadius = 0;
  for ( unsigned int i = 0; i < m_Offset.GetOffsetDimension(); i++ )
    {
    unsigned int distance = vcl_abs(m_Offset[i]);
    if ( distance > minRadius )
      {
      minRadius = distance;
      }
    }
  SizeType radius;
  radius.Fill(minRadius);


  typedef itk::ConstNeighborhoodIterator< InputImageType > NeighborhoodIteratorType;
  NeighborhoodIteratorType neighborIt;
  neighborIt = NeighborhoodIteratorType(radius, inputPtr, inputRegion);
  for ( neighborIt.GoToBegin(); !neighborIt.IsAtEnd(); ++neighborIt )
    {
    const InputPixelType centerPixelIntensity = neighborIt.GetCenterPixel();
    bool pixelInBounds;
    const InputPixelType pixelIntensity =  neighborIt.GetPixel(m_Offset, pixelInBounds);
    if ( !pixelInBounds )
      {
      continue; // don't put a pixel in the value if it's out-of-bounds.
      }
    GLCIList->AddPixelPair(centerPixelIntensity, pixelIntensity);
    }


  double pixelMean = 0.;
  double marginalMean;
  double marginalDevSquared = 0.;
  double pixelVariance = 0.;

  //Create and Initialize marginalSums
  std::vector<double> marginalSums(m_NumberOfBinsPerAxis, 0);

  //get co-occurrence vector and totalfrequency
  VectorType glcVector = GLCIList->GetVector();
  double totalFrequency = static_cast<double> (GLCIList->GetTotalFrequency());

  //Normalize the co-occurrence indexed list and compute mean, marginalSum
  typename VectorType::iterator it = glcVector.begin();
  while( it != glcVector.end())
    {
    double frequency = (*it).second / totalFrequency;
    CooccurrenceIndexType index2 = (*it).first;
    pixelMean += index2[0] * frequency;
    marginalSums[index2[0]] += frequency;
    ++it;
    }

  /* Now get the mean and deviaton of the marginal sums.
     Compute incremental mean and SD, a la Knuth, "The  Art of Computer
     Programming, Volume 2: Seminumerical Algorithms",  section 4.2.2.
     Compute mean and standard deviation using the recurrence relation:
     M(1) = x(1), M(k) = M(k-1) + (x(k) - M(k-1) ) / k
     S(1) = 0, S(k) = S(k-1) + (x(k) - M(k-1)) * (x(k) - M(k))
     for 2 <= k <= n, then
     sigma = vcl_sqrt(S(n) / n) (or divide by n-1 for sample SD instead of
     population SD).
  */
  std::vector<double>::const_iterator msIt = marginalSums.begin();
  marginalMean = *msIt;
  //Increment iterator to start with index 1
  ++msIt;
  for(int k= 2; msIt != marginalSums.end(); ++k, ++msIt)
    {
    double M_k_minus_1 = marginalMean;
    double S_k_minus_1 = marginalDevSquared;
    double x_k = *msIt;
    double M_k = M_k_minus_1 + ( x_k - M_k_minus_1 ) / k;
    double S_k = S_k_minus_1 + ( x_k - M_k_minus_1 ) * ( x_k - M_k );
    marginalMean = M_k;
    marginalDevSquared = S_k;
    }
  marginalDevSquared = marginalDevSquared / m_NumberOfBinsPerAxis;

  VectorConstIteratorType constVectorIt;
  constVectorIt = glcVector.begin();
  while( constVectorIt != glcVector.end())
    {
    RelativeFrequencyType frequency = (*constVectorIt).second / totalFrequency;
    CooccurrenceIndexType        index2 = (*constVectorIt).first;
    pixelVariance += ( index2[0] - pixelMean ) * ( index2[0] - pixelMean ) * frequency;
    ++constVectorIt;
    }

  double pixelVarianceSquared = pixelVariance * pixelVariance;
  // Variance is only used in correlation. If variance is 0, then (index[0] - pixelMean) * (index[1] - pixelMean)
  // should be zero as well. In this case, set the variance to 1. in order to
  // avoid NaN correlation.
  if(pixelVarianceSquared < GetPixelValueTolerance())
    {
    pixelVarianceSquared = 1.;
    }

  //Compute textures
  constVectorIt = glcVector.begin();
  while( constVectorIt != glcVector.end())
    {
    CooccurrenceIndexType index2 = (*constVectorIt).first;
    RelativeFrequencyType frequency = (*constVectorIt).second / totalFrequency;
    textures[0] += frequency * frequency;
    textures[1] -= ( frequency > GetPixelValueTolerance() ) ? frequency *vcl_log(frequency) / log2:0;
    textures[2] += ( ( index2[0] - pixelMean ) * ( index2[1] - pixelMean ) * frequency ) / pixelVarianceSquared;
    textures[3] += frequency / ( 1.0 + ( index2[0] - index2[1] ) * ( index2[0] - index2[1] ) );
    textures[4] += ( index2[0] - index2[1] ) * ( index2[0] - index2[1] ) * frequency;
    textures[5] += vcl_pow( ( index2[0] - pixelMean ) + ( index2[1] - pixelMean ), 3 ) * frequency;
    textures[6] += vcl_pow( ( index2[0] - pixelMean ) + ( index2[1] - pixelMean ), 4 ) * frequency;
    textures[7] += index2[0] * index2[1] * frequency;
    ++constVectorIt;
    }
  textures[7] = (fabs(marginalDevSquared) > 1E-8) ?  ( textures[7] - marginalMean * marginalMean )  / marginalDevSquared : 0;

  // Return result
  return textures;
}

} // namespace otb

#endif
