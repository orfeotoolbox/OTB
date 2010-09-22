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

/** Constructor */
template <class TInputImage, class TCoordRep>
BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::BCOInterpolateImageFunctionBase()
{
  m_Radius = 1;
  m_Alpha  = -0.5;
  this->EvaluateCoef();
}

/** Destructor */
template <class TInputImage, class TCoordRep>
BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::~BCOInterpolateImageFunctionBase()
{
}

template <class TInputImage, class TCoordRep>
void BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TInputImage, class TCoordRep>
void BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::SetRadius(unsigned int radius)
{
  if (radius == 0)
    {
    itkExceptionMacro(<< "Radius Must Be Strictly Superior to 0");
    }
  else 
    {
    m_Radius = radius;
    this->EvaluateCoef();
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
  this->EvaluateCoef();
}

template <class TInputImage, class TCoordRep>
double BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::GetAlpha() const
{
  return m_Alpha;
}

template <class TInputImage, class TCoordRep>
double BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::GetBCOCoef(unsigned int idx) const
{
  return m_BCOCoef[idx];
}

template<class TInputImage, class TCoordRep>
void
BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
::EvaluateCoef()
{
  // Init BCO coefficient container
  unsigned int size = 2*m_Radius+1;
  m_BCOCoef = CoefContainerType(size, 0.);
  double val = 0.;
  double step = 1./static_cast<double>(size);

  // Compute BCO coefficients
  for (unsigned int i = 0; i <= m_Radius; i++)
    {
    // Compute the distance according to alpha.
    double d = val;
    if(  d <= 2. )
      {
      if (d <= 1.)
        {
        m_BCOCoef[m_Radius+i] = (m_Alpha + 2.)*vcl_abs(vcl_pow(d, 3)) - (m_Alpha + 3.)*vcl_pow(d, 2) + 1;
        }
      else
        {
        m_BCOCoef[m_Radius+i] = m_Alpha*vcl_abs(vcl_pow(d, 3)) - 5*m_Alpha*vcl_pow(d, 2) + 8*m_Alpha*vcl_abs(d) - 4*m_Alpha;
        }
      }
    else
      {
      m_BCOCoef[m_Radius+i] = 0;
      }
    
    if(i>0)
      {
      // the filter is symteric
      m_BCOCoef[m_Radius-i] = m_BCOCoef[m_Radius+i];
      }
     val = val + step;
    }
 }



/** Constructor */
template <class TInputImage, class TCoordRep>
BCOInterpolateImageFunction<TInputImage, TCoordRep>
::BCOInterpolateImageFunction()
{
}

/** Destructor */
template <class TInputImage, class TCoordRep>
BCOInterpolateImageFunction<TInputImage, TCoordRep>
::~BCOInterpolateImageFunction()
{
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
  unsigned int dim;  // index over dimension
  unsigned int neighborNumber = (2*this->GetRadius()+1) * (2*this->GetRadius()+1);
  IndexType baseIndex;
  IndexType neighIndex;
  RealType value = itk::NumericTraits<RealType>::Zero;
  vnl_vector<RealType> lineRes(2*this->GetRadius()+1, 0.);

  /**
   * Compute base index = closet index below point
   */
  for( dim = 0; dim < ImageDimension; dim++ )
    {
    baseIndex[dim] = itk::Math::Floor< IndexValueType >( index[dim] );
    }

  for( unsigned int counter = 0; counter < neighborNumber; counter++ )
    {
    
    unsigned int upper = counter; // each bit indicates upper/lower neighbour
    double BCOCoefIndex; //link beetween counter and BCO Coeff.

    // get neighbor index
    for( dim = 0; dim < ImageDimension; dim++ )
      {
      if ( upper & 1 )
        {
        neighIndex[dim] = baseIndex[dim] + 1;
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
        // Take care of the case where the pixel is just
        // in the outer upper boundary of the image grid.
        if( neighIndex[dim] > this->m_EndIndex[dim] )
          {
          neighIndex[dim] = this->m_EndIndex[dim];
          }
#endif
        }
      else
        {
        neighIndex[dim] = baseIndex[dim];
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
        // Take care of the case where the pixel is just
        // in the outer lower boundary of the image grid.
        if( neighIndex[dim] < this->m_StartIndex[dim] )
          {
          neighIndex[dim] = this->m_StartIndex[dim];
          }
#endif
        }   
      
      upper >>= 1;
      } 
    BCOCoefIndex = static_cast<int>(counter - floor(counter/(2*this->GetRadius()+1))*(2*this->GetRadius()+1));

    // Proceed column
    lineRes[BCOCoefIndex] = lineRes[BCOCoefIndex]
    + static_cast<RealType>( this->GetInputImage()->GetPixel( neighIndex ) ) * this->GetBCOCoef(BCOCoefIndex) ;
    }
  //Proceed line
  for(unsigned int l=0; l<lineRes.size(); l++)
    {
    value = value + lineRes[l]*this->GetBCOCoef(l);
    }

  return ( static_cast<OutputType>( value / neighborNumber ) );
}



/** Constructor */
template < typename TPixel, unsigned int VImageDimension, class TCoordRep >
BCOInterpolateImageFunction< otb::VectorImage<TPixel,VImageDimension> , TCoordRep >
::BCOInterpolateImageFunction()
{
}

/** Destructor */
template < typename TPixel, unsigned int VImageDimension, class TCoordRep >
BCOInterpolateImageFunction< otb::VectorImage<TPixel,VImageDimension> , TCoordRep >
::~BCOInterpolateImageFunction()
{
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

  unsigned int dim;  // index over dimension
  unsigned int neighborNumber = (2*this->GetRadius()+1) * (2*this->GetRadius()+1);
  unsigned int componentNumber = this->GetInputImage()->GetNumberOfComponentsPerPixel();
  IndexType baseIndex;
  IndexType neighIndex;
  std::vector< std::vector< ScalarRealType > > lineRes;
  lineRes.resize(2*this->GetRadius()+1);
  for( unsigned int i = 0; i<2*this->GetRadius()+1; i++)
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
  output.SetSize(1);

  /**
   * Compute base index = closet index below point
   */
  for( dim = 0; dim < ImageDimension; dim++ )
    {
    baseIndex[dim] = itk::Math::Floor< IndexValueType >( index[dim] );
    }
  

  for( unsigned int counter = 0; counter < neighborNumber; counter++ )
    {
    
    unsigned int upper = counter; // each bit indicates upper/lower neighbour
    double BCOCoefIndex; //link beetween counter and BCO Coeff.

    // get neighbor index
    for( dim = 0; dim < ImageDimension; dim++ )
      {
      if ( upper & 1 )
        {
        neighIndex[dim] = baseIndex[dim] + 1;
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
        // Take care of the case where the pixel is just
        // in the outer upper boundary of the image grid.
        if( neighIndex[dim] > this->m_EndIndex[dim] )
          {
          neighIndex[dim] = this->m_EndIndex[dim];
          }
#endif
        }
      else
        {
        neighIndex[dim] = baseIndex[dim];
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
        // Take care of the case where the pixel is just
        // in the outer lower boundary of the image grid.
        if( neighIndex[dim] < this->m_StartIndex[dim] )
          {
          neighIndex[dim] = this->m_StartIndex[dim];
          }
#endif
        }   
      
      upper >>= 1;
      }
    BCOCoefIndex = static_cast<int>(counter - floor(counter/(2*this->GetRadius()+1))*(2*this->GetRadius()+1));
   
    // Proceed column
    for( unsigned int j = 0; j<componentNumber; j++)
      {
      lineRes.at(BCOCoefIndex).at(j) = lineRes.at(BCOCoefIndex).at(j) 
        + this->GetInputImage()->GetPixel( neighIndex ).GetElement(j) * this->GetBCOCoef(BCOCoefIndex);
      }
    }
  
  //Proceed line
  for(unsigned int l=0; l<2*this->GetRadius()+1; l++)
    {
    for( unsigned int j = 0; j<componentNumber; j++)
      {
      value.at(j) = value.at(j) + lineRes.at(l).at(j)*this->GetBCOCoef(l);
      }
    }
  
  for( unsigned int j = 0; j<componentNumber; j++)
    {
    value.at(j) = value.at(j) / neighborNumber;
    output.SetElement(j, value.at(j));
    }
  
  return ( output );
}


} //namespace otb

#endif
