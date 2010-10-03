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
#ifndef __otbBCOInterpolateImageFunction_txx
#define __otbBCOInterpolateImageFunction_txx

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
void
BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::EvaluateCoef( const ContinuousIndexType & index ) const
{
  // Init BCO coefficient container
  unsigned int winSize = 2*m_Radius+1;
  CoefContainerType BCOCoefX = CoefContainerType(winSize, 0.);
  CoefContainerType BCOCoefY = CoefContainerType(winSize, 0.);
  double offsetX, offsetY, distX, distY, position, step;

  offsetX = index[0] - itk::Math::Floor<IndexValueType>(index[0]+0.5);
  offsetY = index[1] - itk::Math::Floor<IndexValueType>(index[1]+0.5);

  // Compute BCO coefficients
  step = 4./static_cast<double>(2*m_Radius);
  position = - double(m_Radius) * step;

  for ( int i = -m_Radius; i <= m_Radius; i++)
    {
    // Compute the BCO coefficients according to alpha.
    distX = vcl_abs(position - offsetX*step);
    distY = vcl_abs(position - offsetY*step);

    if( distX <= 2. )
      {
      if (distX <= 1.)
        {
        BCOCoefX[m_Radius+i] = (m_Alpha + 2.)*vcl_abs(vcl_pow(distX, 3))
          - (m_Alpha + 3.)*vcl_pow(distX, 2) + 1;
        }
      else
        {
        BCOCoefX[m_Radius+i] = m_Alpha*vcl_abs(vcl_pow(distX, 3)) - 5
          *m_Alpha*vcl_pow(distX, 2) + 8*m_Alpha*vcl_abs(distX) - 4*m_Alpha;
        }
      }
    else
      {
      BCOCoefX[m_Radius+i] = 0;
      }

    if( distY <= 2. )
      {
      if (distY <= 1.)
        {
        BCOCoefY[m_Radius+i] = (m_Alpha + 2.)*vcl_abs(vcl_pow(distY, 3))
          - (m_Alpha + 3.)*vcl_pow(distY, 2) + 1;
        }
      else
        {
        BCOCoefY[m_Radius+i] = m_Alpha*vcl_abs(vcl_pow(distY, 3)) - 5
          *m_Alpha*vcl_pow(distY, 2) + 8*m_Alpha*vcl_abs(distY) - 4*m_Alpha;
        }
      }
    else
      {
      BCOCoefY[m_Radius+i] = 0;
      }
    position += step;
    }
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
  double radius = this->GetRadius();
  unsigned int winSize = 2*radius+1;
  CoefContainerType BCOCoefX = CoefContainerType(winSize, 0.);
  CoefContainerType BCOCoefY = CoefContainerType(winSize, 0.);
  double offsetX, offsetY, distX, distY, position, step, alpha, norma;
  unsigned int dim;

  IndexType baseIndex;
  IndexType neighIndex;

  std::vector<RealType> lineRes(winSize, 0.);

  RealType value = itk::NumericTraits<RealType>::Zero;

  offsetX = index[0] - itk::Math::Floor<IndexValueType>(index[0]+0.5);
  offsetY = index[1] - itk::Math::Floor<IndexValueType>(index[1]+0.5);

  step = 4./static_cast<double>(2*radius);
  position = - double(radius) * step;

  alpha = this->GetAlpha();

  for ( int i = -radius; i <= radius; i++)
    {
    distX = vcl_abs(position - offsetX*step);
    distY = vcl_abs(position - offsetY*step);

    if( distX <= 2. )
      {
      if (distX <= 1.)
        {
        BCOCoefX[radius+i] = (alpha + 2.)*vcl_abs(vcl_pow(distX, 3))
          - (alpha + 3.)*vcl_pow(distX, 2) + 1;
        }
      else
        {
        BCOCoefX[radius+i] = alpha*vcl_abs(vcl_pow(distX, 3))
          - 5*alpha*vcl_pow(distX, 2) + 8*alpha*vcl_abs(distX) - 4*alpha;
        }
      }
    else
      {
      BCOCoefX[radius+i] = 0;
      }

    if( distY <= 2. )
      {
      if (distY <= 1.)
        {
        BCOCoefY[radius+i] = (alpha + 2.)*vcl_abs(vcl_pow(distY, 3))
          - (alpha + 3.)*vcl_pow(distY, 2) + 1;
        }
      else
        {
        BCOCoefY[radius+i] = alpha*vcl_abs(vcl_pow(distY, 3)) - 5
          *alpha*vcl_pow(distY, 2) + 8*alpha*vcl_abs(distY) - 4*alpha;
        }
      }
    else
      {
      BCOCoefY[radius+i] = 0;
      }
     position += step;
    }

  // Compute base index = closet index
  for( dim = 0; dim < ImageDimension; dim++ )
    {
    baseIndex[dim] = itk::Math::Floor< IndexValueType >( index[dim]+0.5 );
    }

  for( int i = -radius; i <= radius; i++ )
    {
    for( int j = -radius; j <= radius; j++ )
      {
      // get neighbor index
      neighIndex[0] = baseIndex[0] + i;
      neighIndex[1] = baseIndex[1] + j;
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
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
#endif

      lineRes[i+radius] = lineRes[i+radius]
        + static_cast<RealType>( this->GetInputImage()->GetPixel( neighIndex ) ) * BCOCoefY[j+radius];
      }
    value += lineRes[i+radius]*BCOCoefX[i+radius];
    }

  norma = (vcl_log(radius)/vcl_log(2.0));
  norma = norma * norma;

  return ( static_cast<OutputType>( value/norma ) );
}

template < typename TPixel, unsigned int VImageDimension, class TCoordRep >
void BCOInterpolateImageFunction< otb::VectorImage<TPixel,VImageDimension> , TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template < typename TPixel, unsigned int VImageDimension, class TCoordRep >
typename BCOInterpolateImageFunction< otb::VectorImage<TPixel,VImageDimension> , TCoordRep >
::OutputType
BCOInterpolateImageFunction< otb::VectorImage<TPixel,VImageDimension> , TCoordRep >
::EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const
{
  typedef typename itk::NumericTraits<InputPixelType>::ScalarRealType ScalarRealType;

  double radius = this->GetRadius();
  unsigned int winSize = 2*radius+1;
  CoefContainerType BCOCoefX = CoefContainerType(winSize, 0.);
  CoefContainerType BCOCoefY = CoefContainerType(winSize, 0.);
  double offsetX, offsetY, distX, distY, position, step, alpha, norma;
  unsigned int dim;
  unsigned int componentNumber = this->GetInputImage()->GetNumberOfComponentsPerPixel();

  IndexType baseIndex;
  IndexType neighIndex;

  std::vector< std::vector< ScalarRealType > > lineRes;

  lineRes.resize(winSize);
  for( unsigned int i = 0; i<winSize; i++)
    {
    lineRes.at(i).resize(componentNumber);
    for( unsigned int j = 0; j<componentNumber; j++)
      {
      lineRes.at(i).at(j) = itk::NumericTraits<ScalarRealType>::Zero;
      }
    }

  std::vector< ScalarRealType > value;

  value.resize(componentNumber);
  for( unsigned int j = 0; j<componentNumber; j++)
    {
    value.at(j) = itk::NumericTraits<ScalarRealType>::Zero;
    }

  OutputType output;

  output.SetSize(componentNumber);

  offsetX = index[0] - itk::Math::Floor<IndexValueType>(index[0]+0.5);
  offsetY = index[1] - itk::Math::Floor<IndexValueType>(index[1]+0.5);

  step = 4./static_cast<double>(2*radius);
  position = - double(radius) * step;

  alpha = this->GetAlpha();

  for ( int i = -radius; i <= radius; i++)
    {
    distX = vcl_abs(position - offsetX*step);
    distY = vcl_abs(position - offsetY*step);

    if( distX <= 2. )
      {
      if (distX <= 1.)
        {
        BCOCoefX[radius+i] = (alpha + 2.)*vcl_abs(vcl_pow(distX, 3))
          - (alpha + 3.)*vcl_pow(distX, 2) + 1;
        }
      else
        {
        BCOCoefX[radius+i] = alpha*vcl_abs(vcl_pow(distX, 3))
          - 5*alpha*vcl_pow(distX, 2) + 8*alpha*vcl_abs(distX) - 4*alpha;
        }
      }
    else
      {
      BCOCoefX[radius+i] = 0;
      }

    if( distY <= 2. )
      {
      if (distY <= 1.)
        {
        BCOCoefY[radius+i] = (alpha + 2.)*vcl_abs(vcl_pow(distY, 3))
          - (alpha + 3.)*vcl_pow(distY, 2) + 1;
        }
      else
        {
        BCOCoefY[radius+i] = alpha*vcl_abs(vcl_pow(distY, 3)) - 5
          *alpha*vcl_pow(distY, 2) + 8*alpha*vcl_abs(distY) - 4*alpha;
        }
      }
    else
      {
      BCOCoefY[radius+i] = 0;
      }
     position += step;
    }

  //Compute base index = closet index
  for( dim = 0; dim < ImageDimension; dim++ )
    {
    baseIndex[dim] = itk::Math::Floor< IndexValueType >( index[dim]+0.5 );
    }

  for( int i = -radius; i <= radius; i++ )
    {
    for( int j = -radius; j <= radius; j++ )
      {
      // get neighbor index
      neighIndex[0] = baseIndex[0] + i;
      neighIndex[1] = baseIndex[1] + j;
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
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
#endif

      for( unsigned int k = 0; k<componentNumber; k++)
        {
        lineRes[i+radius].at(k) = lineRes[i+radius].at(k)
          + this->GetInputImage()->GetPixel( neighIndex ).GetElement(k) * BCOCoefY[j+radius];
        }
      }
    for( unsigned int k = 0; k<componentNumber; k++)
      {
      value.at(k) += lineRes[i+radius].at(k)*BCOCoefX[i+radius];
      }
    }

  norma = (vcl_log(radius)/vcl_log(2.0));
  norma = norma * norma;

  for( unsigned int k = 0; k<componentNumber; k++)
    {
    output.SetElement(k, value.at(k)/norma);
    }

  return ( output );
}

} //namespace otb

#endif
