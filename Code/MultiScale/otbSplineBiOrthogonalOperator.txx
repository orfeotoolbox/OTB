/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSplieBiOrthogonalOperator_txx
#define __otbSplieBiOrthogonalOperator_txx

#include "otbSplineBiOrthogonalOperator.h"

namespace otb {

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::LowPassSplineBiOrthogonalOperator() 
{
  this->SetWavelet( "9/7" );
  switch ( DirectionOfTransformation )
  {
    case FORWARD:
      this->SetRadius(4);
      this->CreateToRadius(4);
      break;
    case INVERSE:
      this->SetRadius(3);
      this->CreateToRadius(3);
      break;
    default:
      itkExceptionMacro(<<"Spline BiOrthogonal Wavelet operator has to be INVERSE or FORWARD only!!");
      break;
  }
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::LowPassSplineBiOrthogonalOperator 
( const LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator > & other )
  : WaveletOperator<TPixel, VDimension, TAllocator>(other) 
{
  this->SetWavelet( "9/7" );
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator > &
LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::operator= 
( const LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator > & other )
{
  Superclass::operator=(other);
  return *this;
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
void
LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::PrintSelf ( std::ostream &os, itk::Indent i ) const  
{
  Superclass::PrintSelf(os, i.GetNextIndent());
  os << i << "LowPassSplineBiOrthogonalOperator {this=" << this << "}" << std::endl;
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
typename LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >::CoefficientVector 
LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::GenerateCoefficients ()
{
  CoefficientVector coeff;

  if ( !strcmp( this->GetWavelet(), "9/7" ) )
  {
    switch ( DirectionOfTransformation )
    {
      case FORWARD:
      {
        coeff.push_back( 0.026748757411);
        coeff.push_back(-0.016864118443); 
        coeff.push_back(-0.078223266529); 
        coeff.push_back( 0.266864118443); 
        coeff.push_back( 0.602949018236); 
        coeff.push_back( 0.266864118443); 
        coeff.push_back(-0.078223266529); 
        coeff.push_back(-0.016864118443); 
        coeff.push_back( 0.026748757411);
        break;
      }
      case INVERSE:
      {
        typedef HighPassSplineBiOrthogonalOperator< FORWARD, TPixel, VDimension, TAllocator > 
          HighPassOperatorType;
        HighPassOperatorType highPassOperator;
        highPassOperator.SetDirection( 0 );
        highPassOperator.CreateDirectional();

        CoefficientVector highPassCoeff;
        highPassCoeff.resize( highPassOperator.GetSize()[0] );

        for ( typename HighPassOperatorType::ConstIterator iter = highPassOperator.Begin();
              iter != highPassOperator.End(); ++iter )
          highPassCoeff.push_back( *iter );

        coeff = this->GetInverseLowPassFilterFromForwardHighPassFilter( highPassCoeff );
        break;
      }
      default:
      {
        itkExceptionMacro(<<"Spline BiOrthogonal Wavelet operator has to be INVERSE or FORWARD only!!");
        break;
      }
    }
  }

  /** 
   * There is no 'else'.
   * Actually, we do nothing since Wavelet() != "9/7" corresponds to the first calls of 
   * GenerateCoefficients from the constructors
   */

#if 0
  std::cerr << "Coeff H(" << this->GetWavelet();
  if ( (int) DirectionOfTransformation == (int) FORWARD )
    std::cerr << " Forward ) = ";
  else
    std::cerr << " Inverse ) = ";
  for ( typename CoefficientVector::const_iterator iter = coeff.begin(); iter < coeff.end(); ++iter )
    std::cerr << *iter << " ";
  std::cerr << "\n";
#endif

  return Superclass::UpSamplingCoefficients( coeff );
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
void
LowPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::Fill(const CoefficientVector& coeff)
{    
  this->FillCenteredDirectional(coeff);  
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::HighPassSplineBiOrthogonalOperator ()
{
  this->SetWavelet( "9/7" );
  switch ( DirectionOfTransformation )
  {
    case FORWARD:
      this->SetRadius(3);
      this->CreateToRadius(3);
      break;
    case INVERSE:
      this->SetRadius(4);
      this->CreateToRadius(4);
      break;
    default:
      itkExceptionMacro(<<"Spline BiOrthogonal Wavelet operator has to be INVERSE or FORWARD only!!");
      break;
  }
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator > 
::HighPassSplineBiOrthogonalOperator
( const HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator > & other)
  : WaveletOperator<TPixel, VDimension, TAllocator>(other)
{
  this->SetWavelet( "9/7" );
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator > 
HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator > & 
HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::operator=
( const HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator > & other)
{
  Superclass::operator=(other);
  return *this;
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
void
HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::PrintSelf ( std::ostream &os, itk::Indent i ) const
{ 
  Superclass::PrintSelf(os, i.GetNextIndent());
  os << i << "HighPassSplineBiOrthogonalOperator {this=" << this << "}" << std::endl;
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
typename HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
  ::CoefficientVector
HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::GenerateCoefficients ()
{
  CoefficientVector coeff;

  if ( !strcmp( this->GetWavelet(), "9/7" ) )
  {
    switch ( DirectionOfTransformation )
    {
      case FORWARD:
      {
        coeff.push_back( 0.045635881557 );
        coeff.push_back(-0.028771763114 );
        coeff.push_back(-0.295635881557 );
        coeff.push_back( 0.557543526229 );
        coeff.push_back(-0.295635881557 );
        coeff.push_back(-0.028771763114 );
        coeff.push_back( 0.045635881557 );
        break;
      }
      case INVERSE:
      {
        typedef LowPassSplineBiOrthogonalOperator< FORWARD, TPixel, VDimension, TAllocator >
          LowPassOperatorType;
        LowPassOperatorType lowPassOperator;
        lowPassOperator.SetDirection(0);
        lowPassOperator.CreateDirectional();

        CoefficientVector lowPassCoeff;
        lowPassCoeff.resize( lowPassOperator.GetSize()[0] );

        for ( typename LowPassOperatorType::ConstIterator iter = lowPassOperator.Begin();
            iter != lowPassOperator.End(); ++iter )
          lowPassCoeff.push_back( *iter );
        
        coeff = this->GetInverseHighPassFilterFromForwardLowPassFilter( lowPassCoeff );
        break;
      }
      default:
        itkExceptionMacro(<<"Spline BiOrthogonal Wavelet operator has to be INVERSE or FORWARD only!!");
        break;
    }
  }

  /** 
   * There is no 'else'.
   * Actually, we do nothing since Wavelet() != "9/7" corresponds to the first calls of 
   * GenerateCoefficients from the constructors
   */

#if 0
  std::cerr << "Coeff G(" << this->GetWavelet();
  if ( (int) DirectionOfTransformation == (int) FORWARD )
    std::cerr << " Forward ) = ";
  else
    std::cerr << " Inverse ) = ";
  for ( typename CoefficientVector::const_iterator iter = coeff.begin(); iter < coeff.end(); ++iter )
    std::cerr << *iter << " ";
  std::cerr << "\n";
#endif

  return Superclass::UpSamplingCoefficients( coeff );
}

template < InverseOrForwardTransformationEnum TDirectionOfTransformation, 
  class TPixel, unsigned int VDimension, class TAllocator >
void
HighPassSplineBiOrthogonalOperator< TDirectionOfTransformation, TPixel, VDimension, TAllocator >
::Fill(const CoefficientVector& coeff)
{    
  this->FillCenteredDirectional(coeff);  
}



} // end of namespace otb

#endif



