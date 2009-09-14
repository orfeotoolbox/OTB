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
/**************************************************************************
 ***************** TerraSarCalibrationImageFunctor *******************
 **************************************************************************/
// namespace Functor
// {
// /** Constructor */
// template <class TInputIt, class TOutput>
// TerraSarCalibrationImageFunctor<TInputIt, TOutput>
// ::TerraSarCalibrationImageFunctor()
// {
//     m_CalFactor = 1.;
//     m_NoiseRangeValidityMin = 0.;
//     m_NoiseRangeValidityMax = 0.;
//     m_NoiseRangeValidityRef = 0.;
//     m_LocalIncidentAngle = 0.;
//     m_NoisePolynomialCoefficientsList.clear();
//     m_ImageSize.Fill(0);
//     m_UseFastCalibrationMethod = true;
//     m_TimeUTC.clear();
//     m_PRF = 1.;
// }

  
// template <class TInputIt, class TOutput>
// double
// TerraSarCalibrationImageFunctor<TInputIt, TOutput>
// ::ComputeCurrentNoise( unsigned int colId )
//   {
//     double curRange = 0.;
//     double width_2 = static_cast<double>(m_ImageSize[0])/2.;

//     // Use +1 because image start index is 0
//     if( colId < static_cast<unsigned int>(width_2) )
//       {
// 	curRange = m_NoiseRangeValidityMin + ( m_NoiseRangeValidityRef-m_NoiseRangeValidityMin )/width_2 * static_cast<double>(colId+1);
//       }
//     else
//       {
// 	curRange = m_NoiseRangeValidityRef + ( m_NoiseRangeValidityMax-m_NoiseRangeValidityRef )/width_2 * (static_cast<double>(colId+1) - width_2 );
//       }
 
//     return curRange;
//   }

   
// template <class TInputIt, class TOutput>
// typename TerraSarCalibrationImageFunctor<TInputIt, TOutput>::DoubleVectorType
// TerraSarCalibrationImageFunctor<TInputIt, TOutput>
// ::ComputeCurrentCoeffs( unsigned int lineId )
//   {
//     DoubleVectorType curCoeffs;
//     if(m_UseFastCalibrationMethod)
//       { 
// 	curCoeffs = m_NoisePolynomialCoefficientsList[0];
//       }
//     else
//       {
// 	// m_ImageSize[1]-(lineId+1) because the first acquisition line is the last image one.
// 	// line+1 because image starts to 0.
// 	double interval =  static_cast<double>(m_ImageSize[1]) / static_cast<double>(m_NoisePolynomialCoefficientsList.size());
// 	// compute utc time of the line
// 	double currTimeUTC = m_TimeUTC[0] + static_cast<double>(m_ImageSize[1]-(lineId-1))*m_InvPRF;
// 	unsigned int id = 0;
// 	bool go = true;
// 	// deduct the corresponding noise acquisition index
// 	while( id<m_TimeUTC.size() && go)
// 	  { 
// 	    if( currTimeUTC < m_TimeUTC[id] )
// 		go = false;
// 	    id++;
// 	  }
// 	id--;

// 	double timeCoef = 1. / (m_TimeUTC[id]- m_TimeUTC[id-1]) * (currTimeUTC-m_TimeUTC[id-1]);
// 	for(unsigned int j=0; j<m_NoisePolynomialCoefficientsList.size(); j++)
// 	  {
// 	    curCoeffs.push_back( m_NoisePolynomialCoefficientsList[id-1][j] + (m_NoisePolynomialCoefficientsList[id][j] - m_NoisePolynomialCoefficientsList[id-1][j]) * timeCoef );
// 	  }
//     }


//     return curCoeffs;
//   }

// template <class TInputIt, class TOutput>
// inline TOutput
// TerraSarCalibrationImageFunctor<TInputIt, TOutput>
// ::operator() (const TInputIt & inIt)
// {
//   double diffCurRange = ComputeCurrentNoise( static_cast<unsigned int>(inIt.GetIndex()[0]) ) - m_NoiseRangeValidityRef;
//   DoubleVectorType curCoeff = ComputeCurrentCoeffs( static_cast<unsigned int>(inIt.GetIndex()[1]) );
  
//   TOutput outRadBr = m_RadarBrightness( inIt.GetCenterPixel() );

//   double NEBN = 0.;
//   for(int i=0; i<curCoeff.size(); i++)
//     {
//       NEBN += curCoeff[i]*vcl_pow( diffCurRange, i);
//     }
//   double sigma = ( outRadBr - m_CalFactor*NEBN ) * m_SinLocalIncidentAngle;
  
//   return static_cast<TOutput>(sigma);
// }
  
// }// namespace Functor
  
/**************************************************************************
 **************** otbTerraSarCalibrationImageFilter ******************
 **************************************************************************/
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::TerraSarCalibrationImageFilter()
{
  this->SetRadius(0);
  this->SetNumberOfThreads(1);
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
