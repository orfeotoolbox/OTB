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

#ifndef otbBCOInterpolateImageFunction_hxx
#define otbBCOInterpolateImageFunction_hxx

#include "otbBCOInterpolateImageFunction.h"

#include "itkNumericTraits.h"

namespace otb
{

template <class TInputImage, class TCoordRep>
void BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
  os << indent << "Alpha: " << m_Alpha << std::endl;
}

template <class TInputImage, class TCoordRep>
void BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::SetRadius(unsigned int radius)
{
  if (radius < 2)
    {
    itkExceptionMacro(<< "Radius must be strictly greater than 1");
    }
  else
    {
    m_Radius = radius;
    m_WinSize = 2*m_Radius+1;
    }
}

template <class TInputImage, class TCoordRep>
unsigned int BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::GetRadius() const
{
  return m_Radius;
}

template <class TInputImage, class TCoordRep>
void BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::SetAlpha(double alpha)
{
  m_Alpha = alpha;
}

template <class TInputImage, class TCoordRep>
double BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::GetAlpha() const
{
  return m_Alpha;
}

template<class TInputImage, class TCoordRep>
typename BCOInterpolateImageFunctionBase< TInputImage, TCoordRep >
::CoefContainerType
BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::EvaluateCoef( const ContinuousIndexValueType & indexValue ) const
{
  // Init BCO coefficient container

  CoefContainerType BCOCoef(m_WinSize, 0.);
  double offset, dist, position, step;

  offset = indexValue - itk::Math::Floor<IndexValueType>(indexValue+0.5);

  // Compute BCO coefficients
  step = 4./static_cast<double>(2*m_Radius);
  position = - static_cast<double>(m_Radius) * step;

  double sum = 0.0;

  for ( unsigned int i = 0; i < m_WinSize; ++i)
    {

    // Compute the BCO coefficients according to alpha.
    dist = std::abs(position - offset*step);

    if( dist <= 2. )
      {
      if (dist <= 1.)
        {
        BCOCoef[i] = (m_Alpha + 2.)*std::abs(dist * dist * dist)
          - (m_Alpha + 3.)*dist*dist + 1;
        }
      else
        {
        BCOCoef[i] = m_Alpha*std::abs(dist * dist * dist) - 5
          *m_Alpha*dist*dist + 8*m_Alpha*std::abs(dist) - 4*m_Alpha;
        }
      }
    else
      {
      BCOCoef[i] = 0;
      }

    sum += BCOCoef[i];
    position += step;
    }

  for ( unsigned int i = 0; i < m_WinSize; ++i)
    BCOCoef[i] = BCOCoef[i] / sum;

  return BCOCoef;
}

template <class TInputImage, class TCoordRep>
void BCOInterpolateImageFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TInputImage, class TCoordRep>
typename BCOInterpolateImageFunction< TInputImage, TCoordRep >
::OutputType
BCOInterpolateImageFunction<TInputImage, TCoordRep>
::EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const
{

  unsigned int dim;

  IndexType baseIndex;
  IndexType neighIndex;

  RealType value = itk::NumericTraits<RealType>::Zero;

  CoefContainerType BCOCoefX = this->EvaluateCoef(index[0]);
  CoefContainerType BCOCoefY = this->EvaluateCoef(index[1]);

  // Compute base index = closet index
  for( dim = 0; dim < ImageDimension; dim++ )
    {
    baseIndex[dim] = itk::Math::Floor< IndexValueType >( index[dim]+0.5 );
    }

  for(unsigned int i = 0; i < this->m_WinSize; ++i )
    {
    RealType lineRes = 0.;
    for(unsigned int j = 0; j < this->m_WinSize; ++j )
      {
      // get neighbor index
      neighIndex[0] = baseIndex[0] + i - this->m_Radius;
      neighIndex[1] = baseIndex[1] + j - this->m_Radius;

      if( neighIndex[0] > this->m_EndIndex[0] )
        {
        neighIndex[0] = this->m_EndIndex[0];
        }
      if( neighIndex[0] < this->m_StartIndex[0] )
        {
        neighIndex[0] = this->m_StartIndex[0];
        }
      if( neighIndex[1] > this->m_EndIndex[1] )
        {
        neighIndex[1] = this->m_EndIndex[1];
        }
      if( neighIndex[1] < this->m_StartIndex[1] )
        {
        neighIndex[1] = this->m_StartIndex[1];
        }
      lineRes += static_cast<RealType>( this->GetInputImage()->GetPixel( neighIndex ) ) * BCOCoefY[j];
      }
    value += lineRes*BCOCoefX[i];
    }


  return ( static_cast<OutputType>( value ) );
}

template < typename TPixel, unsigned int VImageDimension, class TCoordRep >
void BCOInterpolateImageFunction< otb::VectorImage<TPixel, VImageDimension> , TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template < typename TPixel, unsigned int VImageDimension, class TCoordRep >
typename BCOInterpolateImageFunction< otb::VectorImage<TPixel, VImageDimension> , TCoordRep >
::OutputType
BCOInterpolateImageFunction< otb::VectorImage<TPixel, VImageDimension> , TCoordRep >
::EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const
{
  typedef typename itk::NumericTraits<InputPixelType>::ScalarRealType ScalarRealType;


  unsigned int dim;
  unsigned int componentNumber = this->GetInputImage()->GetNumberOfComponentsPerPixel();

  IndexType baseIndex;
  IndexType neighIndex;


  std::vector<ScalarRealType> lineRes(componentNumber);
  OutputType output(componentNumber);
  output.Fill(itk::NumericTraits<ScalarRealType>::Zero);

  CoefContainerType BCOCoefX = this->EvaluateCoef(index[0]);
  CoefContainerType BCOCoefY = this->EvaluateCoef(index[1]);

  //Compute base index = closet index
  for( dim = 0; dim < ImageDimension; dim++ )
    {
    baseIndex[dim] = itk::Math::Floor< IndexValueType >( index[dim]+0.5 );
    }

  for(unsigned int i = 0; i < this->m_WinSize; ++i )
    {
    std::fill(lineRes.begin(), lineRes.end(), itk::NumericTraits<ScalarRealType>::Zero);
    for(unsigned int j = 0; j < this->m_WinSize; ++j )
      {
      // get neighbor index
      neighIndex[0] = baseIndex[0] + i - this->m_Radius;
      neighIndex[1] = baseIndex[1] + j - this->m_Radius;
      if( neighIndex[0] > this->m_EndIndex[0] )
        {
        neighIndex[0] = this->m_EndIndex[0];
        }
      if( neighIndex[0] < this->m_StartIndex[0] )
        {
        neighIndex[0] = this->m_StartIndex[0];
        }
      if( neighIndex[1] > this->m_EndIndex[1] )
        {
        neighIndex[1] = this->m_EndIndex[1];
        }
      if( neighIndex[1] < this->m_StartIndex[1] )
        {
        neighIndex[1] = this->m_StartIndex[1];
        }

      const InputPixelType & pixel = this->GetInputImage()->GetPixel( neighIndex );
      for( unsigned int k = 0; k<componentNumber; ++k)
        {
        lineRes[k] += pixel.GetElement(k) * BCOCoefY[j];
        }
      }
    for( unsigned int k = 0; k<componentNumber; ++k)
      {
      output[k] += lineRes[k]*BCOCoefX[i];
      }
    }

  return ( output );
}

} //namespace otb

#endif
