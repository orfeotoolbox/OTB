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
#ifndef __otbProlateInterpolateImageFunction_txx
#define __otbProlateInterpolateImageFunction_txx

#include "otbBCOInterpolateImageFunction.h"

#include "itkNumericTraits.h"

namespace otb
{

/** Constructor */
template <class TInputImage, class TCoordRep>
BCOInterpolateImageFunction<TInputImage, TCoordRep>
::BCOInterpolateImageFunction()
{
  m_Radius = 1;
  m_Alpha  = -0.5;
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
  unsigned int neighborNumber = (2*m_Radius+1) * (2*m_Radius+1);


  /**
   * Compute base index = closet index below point
   * Compute distance from point to base index
   */
  IndexType baseIndex;
  double distance[ImageDimension];

  for( dim = 0; dim < ImageDimension; dim++ )
    {
    baseIndex[dim] = itk::Math::Floor< IndexValueType >( index[dim] );
    }

  /*
  std::cout << "index : " << index << std::endl;
  std::cout << "baseIndex : " << baseIndex << std::endl;
  */
  
  RealType value = 0;

  
  IndexType neighIndex;
  neighIndex.Fill(0);

  vnl_vector<RealType> lineRes(2*m_Radius+1, 0.);
  
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
    // Proceed column
    BCOCoefIndex = static_cast<int>(counter - floor(counter/(2*m_Radius+1))*(2*m_Radius+1));
    lineRes[BCOCoefIndex] = lineRes[BCOCoefIndex]
      + static_cast<RealType>( this->GetInputImage()->GetPixel( neighIndex ) ) * m_BCOCoef(BCOCoefIndex);
    }
  
  //Proceed line
  for(unsigned int l=0; l<lineRes.size(); l++)
    {
    value = value + lineRes[l]*m_BCOCoef(l);
    }
     
  std::cout << "Fin  " << value /  (2*m_Radius * 2*m_Radius) << std::endl;
  return ( static_cast<OutputType>( value / (2*m_Radius * 2*m_Radius) ) );
}


template <class TInputImage, class TCoordRep>
void BCOInterpolateImageFunction<TInputImage, TCoordRep>
::SetRadius(unsigned int radius)
{
  if (radius == 0)
    {
    itkExceptionMacro(<< "Radius Must Be Strictly Superior to 0");
    }
  else 
    {
    m_Radius = radius;
    }
}

template <class TInputImage, class TCoordRep>
unsigned int BCOInterpolateImageFunction<TInputImage, TCoordRep>
::GetRadius() const
{
  return m_Radius;
}

template <class TInputImage, class TCoordRep>
void BCOInterpolateImageFunction<TInputImage, TCoordRep>
::SetAlpha(double alpha)
{
  m_Alpha = alpha;
}

template <class TInputImage, class TCoordRep>
double BCOInterpolateImageFunction<TInputImage, TCoordRep>
::GetAlpha() const
{
  return m_Alpha;
}

template<class TInputImage, class TCoordRep>
void
BCOInterpolateImageFunction<TInputImage, TCoordRep>
::EvaluateCoef()
{
  // Init BCO coefficient container
  unsigned int size = 2*m_Radius+1;
  m_BCOCoef = CoefContainerType(size, 0.);
  double val = 0.;
  //double step = 1./static_cast<double>(m_Radius);
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
      // the filter is symtric
      m_BCOCoef[m_Radius-i] = m_BCOCoef[m_Radius+i];
      }
     val = val + step;
    }
  std::cout << m_BCOCoef << std::endl;

 }


} //namespace otb

#endif
