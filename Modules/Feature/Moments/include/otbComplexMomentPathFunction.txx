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
#ifndef __otbComplexMomentPathFunction_txx
#define __otbComplexMomentPathFunction_txx

#include "otbComplexMomentPathFunction.h"
#include "itkImageRegionIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbMacro.h"

#include <complex>
namespace otb
{

/**
   * Constructor
   */
template <class TInputPath, class TOutput, class TPrecision>
ComplexMomentPathFunction<TInputPath, TOutput, TPrecision>
::ComplexMomentPathFunction()
{
  m_P    = 0;
  m_Q    = 0;
}

/**
   *
   */
template <class TInputPath, class TOutput, class TPrecision>
void
ComplexMomentPathFunction<TInputPath, TOutput, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " p indice value      : "  << m_P << std::endl;
  os << indent << " q indice value      : "  << m_Q << std::endl;
}

template <class TInputPath, class TOutput, class TPrecision>
typename ComplexMomentPathFunction<TInputPath, TOutput, TPrecision>::ComplexPrecisionType
ComplexMomentPathFunction<TInputPath, TOutput, TPrecision>
::EvaluateComplexMomentAtIndex(VertexType index) const
{
  ComplexPrecisionType ValP;
  ComplexPrecisionType ValQ;
  ComplexPrecisionType Result;
  PrecisionType                                PixelValue(1.0);

  ValP = ComplexPrecisionType(1.0, 0.0);
  ValQ = ComplexPrecisionType(1.0, 0.0);
  unsigned int p  = m_P;
  while (p > 0)
    {
    ValP *= ComplexPrecisionType(index[0], index[1]);
    --p;
    }
  unsigned int q  = m_Q;
  while (q > 0)
    {
    ValQ *= ComplexPrecisionType(index[0], -index[1]);
    --q;
    }

  Result = ValP * ValQ * ComplexPrecisionType(static_cast<PrecisionType>(PixelValue), 0.0);
  return Result;
}

template <class TInputPath, class TOutput, class TPrecision>
typename ComplexMomentPathFunction<TInputPath,
    TOutput, TPrecision>::OutputType
ComplexMomentPathFunction<TInputPath, TOutput, TPrecision>
::Evaluate(const PathType& path) const
{
  // Retrieve the vertex list
  VertexListPointer vertexList =  path.GetVertexList();
  // Get the number of vertices in the path
  unsigned int pathSize        = vertexList->Size();

  // value will store the result
  ComplexPrecisionType value = static_cast<ComplexPrecisionType>(0.0);

  // Check if we there are enough vertices in the path to actually
  // compute something
  if (pathSize < 2)
    {
    return static_cast<OutputType>(value);
    }

  // First, we compute the centroid of the path so as to center the moment
  typename VertexListType::ConstIterator it = vertexList->Begin();
  VertexType                             centroid = it.Value();
  ++it;

  // Cumulate points
  while (it != vertexList->End())
    {
    centroid[0] += it.Value()[0];
    centroid[1] += it.Value()[1];
    ++it;
    }

  // Normalize
  centroid[0] /= static_cast<PrecisionType>(pathSize);
  centroid[1] /= static_cast<PrecisionType>(pathSize);

  // Second, we integrate along the edge
  it = vertexList->Begin();

  VertexType source = it.Value();
  source[0] -= centroid[0];
  source[1] -= centroid[1];
  ++it;

  PrecisionType ds;
  VertexType    dest;

  // This variable will be used to normalize the moment
  PrecisionType norm = 0.;

  while (it != vertexList->End())
    {
    dest = it.Value();

    // Get source and destination coordinates
    dest[0]  -= centroid[0];
    dest[1]  -= centroid[1];

    // Don't forget the ds part of the integration process
    ds = vcl_sqrt(vcl_pow(dest[0] - source[0], 2.) + vcl_pow(dest[1] - source[1], 2.));
    norm += ds;
    value += ds * EvaluateComplexMomentAtIndex(source);
    source = dest;
    ++it;
    }
  // Close the loop
  dest = vertexList->Begin().Value();
  dest[0]  -= centroid[0];
  dest[1]  -= centroid[1];
  ds = vcl_sqrt(vcl_pow(dest[0] - source[0], 2.) + vcl_pow(dest[1] - source[1], 2.));
  norm += ds;
  value += EvaluateComplexMomentAtIndex(source) * ds;
  norm = vcl_pow(norm, ((PrecisionType) m_P + (PrecisionType) m_Q) / 2.);

  // Normalize with edge perimeter
  value /= norm;

  return static_cast<OutputType>(value);

}

template <class TInputPath, class TOutput, class TPrecision>
typename ComplexMomentPathFunction<TInputPath,
    TOutput, TPrecision>::OutputType
ComplexMomentPathFunction<TInputPath, TOutput, TPrecision>
::Evaluate() const
{
  if (!this->GetInputPath())
    {
    otbMsgDevMacro(<< "Pb with GetInputPath");
    return static_cast<OutputType>(ComplexPrecisionType(itk::NumericTraits<PrecisionType>::Zero,
                                                        itk::NumericTraits<PrecisionType>::Zero));
    }

  OutputType Result =  Evaluate(*(this->GetInputPath()));

  return Result;
}

} // namespace otb

#endif
