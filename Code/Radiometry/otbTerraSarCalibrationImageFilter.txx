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
#ifndef __otbTerraSarCalibrationImageFilter_txx
#define __otbTerraSarCalibrationImageFilter_txx

#include "otbTerraSarCalibrationImageFilter.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbImageMetadataInterfaceBase.h"


namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::TerraSarCalibrationImageFilter()
{
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  if(this->GetFunctor().GetNoisePolynomialCoefficientsList().size() == 0)
    itkExceptionMacro(<<"Empty NoisePolynomialCoefficientsList.");
  
  if( this->GetFunctor().GetUseFastCalibrationMethod() == false )
    {
      unsigned int size =  this->GetFunctor().GetNoisePolynomialCoefficientsList()[0].size();
      for(unsigned int i=1; i<this->GetFunctor().GetNoisePolynomialCoefficientsList().size(); i++)
	{
	  if( this->GetFunctor().GetNoisePolynomialCoefficientsList()[i].size() != size )
	    itkExceptionMacro(<<"Wrong noise polynomial coefficient, degrees mismatch.");
	}
      
      if( this->GetFunctor().GetTimeUTC().size() != this->GetFunctor().GetNoisePolynomialCoefficientsList().size() )
	itkExceptionMacro(<<"Number of Time UTC and number of noise polygonls mismatch."<<this->GetFunctor().GetTimeUTC().size()<<"  "<<this->GetFunctor().GetNoisePolynomialCoefficientsList().size());

      if(this->GetFunctor().GetPRF() == 0.)
	itkExceptionMacro(<<"PRF can't be null.");
      
    }

  this->GetFunctor().SetImageSize( this->GetInput()->GetLargestPossibleRegion().GetSize() );
}


/******************************* ACCESSORS ********************************/

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetCalFactor( double val )
{ 
  this->GetFunctor().SetCalFactor( val );
  this->Modified();
}

template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetCalFactor() const
{ 
  return this->GetFunctor().GetCalFactor(); 
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetNoiseRangeValidityMin( double val ) 
{ 
  this->GetFunctor().SetNoiseRangeValidityMin( val );
  this->Modified();
}
  
template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetNoiseRangeValidityMin() const 
{ 
  return this->GetFunctor().GetNoiseRangeValidityMin(); 
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetNoiseRangeValidityMax( double val ) 
{ 
  this->GetFunctor().SetNoiseRangeValidityMax( val );
  this->Modified();
}
 
template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetNoiseRangeValidityMax() const
{ 
  return this->GetFunctor().GetNoiseRangeValidityMax();
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetNoiseRangeValidityRef( double val ) 
{ 
  this->GetFunctor().SetNoiseRangeValidityRef( val );
  this->Modified();
}


template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetNoiseRangeValidityRef() const
{ 
  return this->GetFunctor().GetNoiseRangeValidityRef(); 
}


template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetLocalIncidentAngle( double val )
{ 
  this->GetFunctor().SetLocalIncidentAngle( val ); 
  this->Modified();
}

template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetLocalIncidentAngle() const
{ 
  return this->GetFunctor().GetLocalIncidentAngle();
}

template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetSinLocalIncidentAngle() const
{
  return this->GetFunctor().GetSinLocalIncidentAngle(); 
}
 
template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetNoisePolynomialCoefficientsList( DoubleVectorVectorType vect )
{
  this->GetFunctor().SetNoisePolynomialCoefficientsList( vect );
  this->Modified();
}


template <class TInputImage, class TOutputImage>
typename TerraSarCalibrationImageFilter<TInputImage,TOutputImage>::DoubleVectorVectorType
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetNoisePolynomialCoefficientsList() const
{ 
  return this->GetFunctor().GetNoisePolynomialCoefficientsList();
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetUseFastCalibrationMethod( bool b )
{
  this->GetFunctor().SetUseFastCalibrationMethod( b ); 
  this->Modified();
}
 
template <class TInputImage, class TOutputImage>
bool
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetUseFastCalibrationMethod() const 
{ 
  return this->GetFunctor().GetUseFastCalibrationMethod();
}


template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetTimeUTC( LIntVectorType vect ) 
{ 
  this->GetFunctor().SetTimeUTC( vect );
  this->Modified();
}
  

template <class TInputImage, class TOutputImage>
typename TerraSarCalibrationImageFilter<TInputImage,TOutputImage>::LIntVectorType
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetTimeUTC() const
{ 
  return this->GetFunctor().GetTimeUTC();
}


template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetPRF( double val ) 
{ 
  this->GetFunctor().SetPRF( val ); 
  this->Modified();
}


template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetPRF() const
{ 
  return this->GetFunctor().GetPRF();
}


template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetInvPRF() const
{
  return this->GetFunctor().GetInvPRF(); 
}

/**PrintSelf method */
template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << "Calibration Factor          : " << this->GetCalFactor()  << std::endl;
  os << "Sensor local incident angle : " << this->GetLocalIncidentAngle() << std::endl;
  os << "Noise minimal range validity: " << this->GetNoiseRangeValidityMin() << std::endl;
  os << "Noise maximal range validity: " << this->GetNoiseRangeValidityMax() << std::endl;
  os << "Noise reference range       : " << this->GetNoiseRangeValidityRef() << std::endl;
  
  if(this->GetUseFastCalibrationMethod())
    {
      os << "Noise polinomial coefficient: [   ";
      for (unsigned int i=0; i<this->GetNoisePolynomialCoefficientsList()[0].size(); ++i)
	{
	  os << this->GetNoisePolynomialCoefficientsList()[0][i] << "   ";
	}
      os << "]" << std::endl;
    }
  else
    {
      os << "Pulse Repetition Frequency  : " << this->GetPRF() << std::endl;
      os << "Noise acquisitions          : " << this->GetNoisePolynomialCoefficientsList().size() << std::endl;
      for (unsigned int i=0; i<this->GetNoisePolynomialCoefficientsList().size(); ++i)
	{
	  os << "Noise acquisition "<< i << ":" << std::endl;
	  os << "Noise TimeUTC           : " << this->GetTimeUTC()[i] << std::endl;
	  os << "Noise polinomial coefficient: [   ";
	  for (unsigned int j=0; j<this->GetNoisePolynomialCoefficientsList()[j].size(); ++j)
	    {
	      os << this->GetNoisePolynomialCoefficientsList()[i][j] << "   ";
	    }
      os << "]" << std::endl;
	}
    }  
}



} // namespace otb
#endif
