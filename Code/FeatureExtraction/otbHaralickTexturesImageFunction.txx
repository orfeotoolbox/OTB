/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbHaralickTexturesImageFunction_txx
#define __otbHaralickTexturesImageFunction_txx

#include "otbHaralickTexturesImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNumericTraits.h"
#include "itkMacro.h"
#include <complex>

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
 m_InputImageMaximum(256)
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

  // Build the co-occurence matrix generator
  CoocurrenceMatrixGeneratorPointerType coOccurenceMatrixGenerator = CoocurrenceMatrixGeneratorType::New();
  coOccurenceMatrixGenerator->SetInput(this->GetInputImage());
  coOccurenceMatrixGenerator->SetOffset(m_Offset);
  coOccurenceMatrixGenerator->SetNumberOfBinsPerAxis(m_NumberOfBinsPerAxis);
  coOccurenceMatrixGenerator->SetPixelValueMinMax(m_InputImageMinimum, m_InputImageMaximum);

  // Compute the region on which co-occurence will be estimated
  typename InputRegionType::IndexType inputIndex = index;
  for(unsigned int dim = 0; dim<InputImageType::ImageDimension; ++dim)
    {
    inputIndex[dim]-= m_NeighborhoodRadius;
    }
  typename InputRegionType::SizeType  inputSize;
  inputSize.Fill(2*m_NeighborhoodRadius+1);

  // Build the input  region
  InputRegionType inputRegion;
  inputRegion.SetIndex(inputIndex);
  inputRegion.SetSize(inputSize);

  // Compute the co-occurence matrix
  coOccurenceMatrixGenerator->SetRegion(inputRegion);
  coOccurenceMatrixGenerator->Update();

  // Build the texture calculator
  TextureCoefficientsCalculatorPointerType texturesCalculator = TextureCoefficientsCalculatorType::New();

  // Compute textures indices
  texturesCalculator->SetInput(coOccurenceMatrixGenerator->GetOutput());
  texturesCalculator->Update();

  // Fill the output vector
  textures[0]=texturesCalculator->GetEnergy();
  textures[1]=texturesCalculator->GetEntropy();
  textures[2]=texturesCalculator->GetCorrelation();
  textures[3]=texturesCalculator->GetInverseDifferenceMoment();
  textures[4]=texturesCalculator->GetInertia();
  textures[5]=texturesCalculator->GetClusterShade();
  textures[6]=texturesCalculator->GetClusterProminence();
  textures[7]=texturesCalculator->GetHaralickCorrelation();

  // Return result
  return textures;
}

} // namespace otb

#endif
