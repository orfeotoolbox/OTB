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
#include "otbTerraSarImageMetadataInterface.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::TerraSarCalibrationImageFilter()
{}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::BeforeThreadedGenerateData()
{
  // Set the image size
  if (this->GetImageSize()[0] == 0 && this->GetImageSize()[1] == 0)
    {
    this->SetImageSize( this->GetInput()->GetLargestPossibleRegion().GetSize() );
    }
  
  // Build the metadata interface
  TerraSarImageMetadataInterface::Pointer lImageMetadata = otb::TerraSarImageMetadataInterface::New();

  // Check availability
  bool mdIsAvailable = lImageMetadata->CanRead(this->GetInput()->GetMetaDataDictionary());

  // If the user did not set the data AND the metadata is available, set the data
  
  // CalibrationFactor
  if (this->GetCalibrationFactor() == itk::NumericTraits<double>::min()) 
  {
    if (mdIsAvailable)
      {
        this->SetCalibrationFactor( lImageMetadata->GetCalibrationFactor(this->GetInput()->GetMetaDataDictionary()) );
      }
    else
      {
        itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supported");
      }
  }

  // Noise records
  if(this->GetFunctor().GetNumberOfNoiseRecords() == 0)
    {
    if (mdIsAvailable)
      {
      // Retrieve the number of noise records
      unsigned int nbNoiseRecords = lImageMetadata->GetNumberOfNoiseRecords(this->GetInput()->GetMetaDataDictionary());
      
      // Retrieve the noise records
      ossimplugins::Noise * noiseRecords = lImageMetadata->GetNoise(this->GetInput()->GetMetaDataDictionary());
      
      // Retrieve corresponding times
      std::vector<double> noiseTimes = lImageMetadata->GetNoiseTimeUTCList(this->GetInput()->GetMetaDataDictionary());

      if(noiseTimes.empty() || nbNoiseRecords)
	{
	itkExceptionMacro(<<"No noise records found.");
	}

      // Set the acquisition start time
      this->GetFunctor().SetAcquisitionStartTime(noiseTimes[0]);
      
      for(unsigned int i = 0; i < nbNoiseRecords;++i)
	{
	this->GetFunctor().AddNoiseRecord(noiseTimes[i],noiseRecords->get_imageNoise()[i]);
	}

      // Free memory
      delete noiseRecords;
      }
    else
      {
        itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supported");
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
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supported");
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
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supported");
    }
  }
}

/******************************* ACCESSORS ********************************/
template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetCalibrationFactor( double val )
{
  this->GetFunctor().SetCalibrationFactor( val );
  this->Modified();
}

template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetCalibrationFactor() const
{
  return this->GetFunctor().GetCalibrationFactor();
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
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::SetImageSize( SizeType size )
{
  this->GetFunctor().SetImageSize( size );
  this->Modified();
}


template <class TInputImage, class TOutputImage>
const typename TerraSarCalibrationImageFilter<TInputImage,TOutputImage>::SizeType& 
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::GetImageSize() const
{
  return this->GetFunctor().GetImageSize();
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

/**PrintSelf method */
template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << "Calibration Factor          : " << this->GetCalibrationFactor()  << std::endl;
  os << "Sensor local incident angle : " << this->GetLocalIncidentAngle() << std::endl;
}



} // namespace otb
#endif
