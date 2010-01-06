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

#include <algorithm>
#include "otbTerraSarCalibrationImageFilter.h"
//#include "otbImageMetadataInterfaceFactory.h"
//#include "otbImageMetadataInterfaceBase.h"
#include "otbTerraSarImageMetadataInterface.h"

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

    // If the user doesn't set it AND the metadata is available, set calFactor using image metadata
//  std::cout<<this->GetCalFactor()<<std::endl;
//   if (this->GetCalFactor() == itk::NumericTraits<double>::min()) 
//     {
//       /** TODO : use a factory for RADAR image metadata interface */
//       TerraSarImageMetadataInterface::Pointer lImageMetadata = otb::TerraSarImageMetadataInterface::New();
//       if( !lImageMetadata->CanRead(this->GetInput()->GetMetaDataDictionary()) )
// 	{
// 	  itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
// 	}
//       this->SetCalFactor( lImageMetadata->GetCalibrationFactor(this->GetInput()->GetMetaDataDictionary()) );
//     }

  // Load metadata
  TerraSarImageMetadataInterface::Pointer lImageMetadata = otb::TerraSarImageMetadataInterface::New();
  bool mdIsAvailable = lImageMetadata->CanRead(this->GetInput()->GetMetaDataDictionary());

  // If the user doesn't set the data AND the metadata is available, set the data
  
  // CalFactor
  if (this->GetCalFactor() == itk::NumericTraits<double>::min()) 
  {
    if (mdIsAvailable)
      {
        this->SetCalFactor( lImageMetadata->GetCalibrationFactor(this->GetInput()->GetMetaDataDictionary()) );
      }
    else
      {
        itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
      }
  }
  
  // NoiseRangeValidityMin
  if (this->GetNoiseRangeValidityMin() == itk::NumericTraits<double>::min()) 
  {
    if (mdIsAvailable)
    {
      DoubleVectorType vecTmp = lImageMetadata->GetNoiseValidityRangeMinList(this->GetInput()->GetMetaDataDictionary());
      std::sort(vecTmp.begin(), vecTmp.end());
      this->SetNoiseRangeValidityMin(vecTmp[0]);
    }
    else
    {
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
    }
  }
  
  // NoiseRangeValidityMax
  if (this->GetNoiseRangeValidityMax() == itk::NumericTraits<double>::max()) 
  {
    if (mdIsAvailable)
    {
      DoubleVectorType vecTmp = lImageMetadata->GetNoiseValidityRangeMaxList(this->GetInput()->GetMetaDataDictionary());
      std::sort(vecTmp.begin(), vecTmp.end());
      this->SetNoiseRangeValidityMax(vecTmp[vecTmp.size()-1]);
    }
    else
    {
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
    }
  }

  // NoiseRangeValidityRef
  if (this->GetNoiseRangeValidityRef() == itk::NumericTraits<double>::Zero) 
  {
    if (mdIsAvailable)
    {
      double sum = 0;
      // Get vector
      DoubleVectorType noiseRefList = lImageMetadata->GetNoiseReferencePointList(this->GetInput()->GetMetaDataDictionary());
      // Mean computation
      for (int i=0; i<noiseRefList.size(); i++)
      {
	sum += noiseRefList[i];
      }
      
      this->SetNoiseRangeValidityRef( sum / noiseRefList.size() );
    }
    else
    {
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
    }
  }
  
  // NoisePolynomialCoefficient
  if ( ((this->GetNoisePolynomialCoefficientsList())[0][0] == itk::NumericTraits<double>::Zero)
	 && (this->GetNoisePolynomialCoefficientsList().size() == 1)
	 &&  (this->GetNoisePolynomialCoefficientsList()[0].size() == 1) )
  {
    if (mdIsAvailable)
    {
      this->SetNoisePolynomialCoefficientsList(lImageMetadata->GetNoisePolynomialCoefficientsList(this->GetInput()->GetMetaDataDictionary()));
    }
    else
    {
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
    }
  }
  
  // Time UTC
  if ((this->GetTimeUTC()[0] == itk::NumericTraits<double>::Zero) && (this->GetTimeUTC()[1] == 1.) && (this->GetTimeUTC().size() == 2)) 
  {
    if (mdIsAvailable)
    {
      this->SetTimeUTC(lImageMetadata->GetNoiseTimeUTCList(this->GetInput()->GetMetaDataDictionary()));
    }
    else
    {
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
    }
  }
  
  // Radar frequency (PRF)
  if (this->GetPRF() == 1.) 
  {
    if (mdIsAvailable)
    {
      this->SetPRF(lImageMetadata->GetRadarFrequency(this->GetInput()->GetMetaDataDictionary()));
    }
    else
    {
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
    }
  }
  
  // Incidence Angle
  if (this->GetLocalIncidentAngle() == itk::NumericTraits<double>::Zero) 
  {
    if (mdIsAvailable)
    {
      double mean = lImageMetadata->GetMeanIncidenceAngles(this->GetInput()->GetMetaDataDictionary());
      this->SetLocalIncidentAngle(mean);
    }
    else
    {
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supproted");
    }
  }
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
::SetTimeUTC( DoubleVectorType vect )
{
  this->GetFunctor().SetTimeUTC( vect );
  this->Modified();
}
  

template <class TInputImage, class TOutputImage>
typename TerraSarCalibrationImageFilter<TInputImage,TOutputImage>::DoubleVectorType
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
