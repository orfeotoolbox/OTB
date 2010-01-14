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
#include <iomanip>

#include "otbTerraSarCalibrationImageFilter.h"
#include "otbTerraSarImageMetadataInterface.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::TerraSarCalibrationImageFilter() : m_CalibrationFactor(itk::NumericTraits<double>::Zero),
                                 m_LocalIncidentAngle(itk::NumericTraits<double>::Zero),
                                 m_PRF(1.),
                                 m_OriginalProductSize(),
                                 m_UseFastCalibration(false),
                                 m_ResultsInDecibels(false),
                                 m_NoiseRecords(),
                                 m_DefaultValue(0.00001)
                                 
{}

template <class TInputImage, class TOutputImage>
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::~TerraSarCalibrationImageFilter()
{
  // Clear any noise records
  this->ClearNoiseRecords();
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::AddNoiseRecord(double utcAcquisitionTime, const ImageNoiseType& record)
{
  // Create the pair
  NoiseRecordType newRecord(utcAcquisitionTime,record);
  
  // Add it to the list
  m_NoiseRecords.push_back(newRecord);
  }

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::ClearNoiseRecords()
{
  m_NoiseRecords.clear();
}

template <class TInputImage, class TOutputImage>
bool
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::CompareNoiseRecords(const NoiseRecordType& record1, const NoiseRecordType& record2)
{
  return (record1.first > record2.first);
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::BeforeThreadedGenerateData()
{
  // Set the product original size
  if (m_OriginalProductSize[0] == 0 && m_OriginalProductSize[1] == 0)
    {
    m_OriginalProductSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
    }
  
  // Build the metadata interface
  TerraSarImageMetadataInterface::Pointer lImageMetadata = otb::TerraSarImageMetadataInterface::New();

  // Check availability
  bool mdIsAvailable = lImageMetadata->CanRead(this->GetInput()->GetMetaDataDictionary());

  // If the user did not set the data AND the metadata is available, set the data
  
  // CalibrationFactor
  if (m_CalibrationFactor == itk::NumericTraits<double>::Zero) 
  {
    if (mdIsAvailable)
      {
        m_CalibrationFactor = lImageMetadata->GetCalibrationFactor(this->GetInput()->GetMetaDataDictionary());
      }
    else
      {
        itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supported");
      }
  }

  // Noise records
  if(m_NoiseRecords.empty())
    {
    if (mdIsAvailable)
      {
      // Retrieve the number of noise records
      unsigned int nbNoiseRecords = lImageMetadata->GetNumberOfNoiseRecords(this->GetInput()->GetMetaDataDictionary());
      
      // Retrieve the noise records
      ossimplugins::Noise * noiseRecords = lImageMetadata->GetNoise(this->GetInput()->GetMetaDataDictionary());
      
      // Retrieve corresponding times
      std::vector<double> noiseTimes = lImageMetadata->GetNoiseTimeUTCList(this->GetInput()->GetMetaDataDictionary());

      if(!m_UseFastCalibration && ( noiseTimes.empty() || nbNoiseRecords == 0))
       {
       itkExceptionMacro(<<"No noise records found. Consider using fast calibration.");
       }
      
      // For each noise record, add it
      for(unsigned int i = 0; i < nbNoiseRecords;++i)
       {
       this->AddNoiseRecord(noiseTimes[i],noiseRecords->get_imageNoise()[i]);
       }
      
      // Free memory
      delete noiseRecords;
      }
    else
      {
        itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supported");
      }
    }
  
  // PRF
  if (this->GetPRF() == 1.) 
  {
    if (mdIsAvailable)
    {
      m_PRF = lImageMetadata->GetPRF(this->GetInput()->GetMetaDataDictionary());
    }
    else
    {
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supported");
    }
  }
  
  // Incident Angle
  if (m_LocalIncidentAngle == itk::NumericTraits<double>::Zero) 
  {
    if (mdIsAvailable)
    {
      m_LocalIncidentAngle =  lImageMetadata->GetMeanIncidenceAngles(this->GetInput()->GetMetaDataDictionary());
    }
    else
    {
      itkExceptionMacro(<<"Invalid input image. Only TerraSar images are supported");
    }
  }
  // Ensure that noise records are sorted by decreasing acquisition
  // date
  std::sort(m_NoiseRecords.begin(),m_NoiseRecords.end(), &Self::CompareNoiseRecords);
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{
  // Retrieve input and output pointer
  typename InputImageType::ConstPointer inputPtr = this->GetInput();
  typename OutputImageType::Pointer     outputPtr = this->GetOutput(0);
  
  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIteratorWithIndex<TInputImage>  inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  // Initialize iterators
  inputIt.GoToBegin();
  outputIt.GoToBegin();
  
  // Set up the functor
  CalibrationFunctorType calibrationFunctor;
  calibrationFunctor.SetCalibrationFactor(m_CalibrationFactor);
  calibrationFunctor.SetLocalIncidentAngle(m_LocalIncidentAngle);
  calibrationFunctor.SetUseFastCalibration(m_UseFastCalibration);
  calibrationFunctor.SetResultsInDecibels(m_ResultsInDecibels);
  calibrationFunctor.SetOriginalProductSize(m_OriginalProductSize);
  calibrationFunctor.SetDefaultValue(m_DefaultValue);

  // Set up progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  
  for(unsigned int i = 0; i < m_NoiseRecords.size();++i)
    {
      std::cout<<std::fixed<<std::setprecision(10)<<"t"<<i<<": "<<  m_NoiseRecords[i].first << std::endl;
    }

  assert(!m_NoiseRecords.empty());

  // The acquisition time of the first line of OutputRegionForThread.
  //double PRFJulian = m_PRF * (0.864 / 1e-5);
  double invPRF = /*(0.864/1e-5) * */1/m_PRF;
  double currentAzimuthPosition = m_NoiseRecords.back().first + invPRF 
    * (m_OriginalProductSize[1]- inputIt.GetIndex()[1] -1);
   
  std::cout<<"Product size: "<<m_OriginalProductSize<<std::endl;
  std::cout<<"Start index: "<<inputIt.GetIndex()<<std::endl;
  std::cout<<"Current position: "<<currentAzimuthPosition<<std::endl;
  std::cout<<"PRF: "<<m_PRF<<std::endl;


  // Look for the first noise record to be used (remember we sorted
  // m_NoiseRecords by decreasing time)
  NoiseRecordVectorType::const_iterator currentNoiseRecordIt = m_NoiseRecords.begin();
  
  unsigned int nrIndex = 0;

  // Iterate until we find it
  while(currentNoiseRecordIt != m_NoiseRecords.end() && currentNoiseRecordIt->first > currentAzimuthPosition)
    {
      ++nrIndex;
      ++currentNoiseRecordIt;
    }

  assert(currentNoiseRecordIt != m_NoiseRecords.end());

  // Store the current noise record azimuth position
  double currentNoiseRecordAzimuthPosition = currentNoiseRecordIt->first;
  calibrationFunctor.SetNoiseRecord(currentNoiseRecordIt->second);
  std::cout<<nrIndex<<" degree: "<<currentNoiseRecordIt->second.get_polynomialDegree()<<std::endl;

  // Store current line index
  typename OutputImageRegionType::IndexType::IndexValueType currentLine = inputIt.GetIndex()[1];
 
  // Iterate on the input and output buffer
  while( !inputIt.IsAtEnd() ) 
    {
    // Check if we changed line
    if(currentLine != inputIt.GetIndex()[1])
      {
      // If so, update the current azimuth time
      currentAzimuthPosition -= invPRF;

      // Update line counter
      currentLine = inputIt.GetIndex()[1];
            
      // And check if we changed of NoiseRecord
      if(currentAzimuthPosition < currentNoiseRecordAzimuthPosition)
       {
       // Update the iterator
       ++currentNoiseRecordIt;
       ++nrIndex;
       std::cout<<"NrIndex increment"<<std::endl;
       // If we are not at the last record
       if(currentNoiseRecordIt != m_NoiseRecords.end())
         {
         // Update the functor noise record
         currentNoiseRecordAzimuthPosition = currentNoiseRecordIt->first;
         calibrationFunctor.SetNoiseRecord(currentNoiseRecordIt->second);
	 std::cout<<nrIndex<<" degree: "<<currentNoiseRecordIt->second.get_polynomialDegree()<<std::endl;
         }
       }
      }
    // Apply the calibration functor
    outputIt.Set( calibrationFunctor( inputIt.Get(), inputIt.GetIndex() ) );
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}


template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Calibration Factor:           " << m_CalibrationFactor  << std::endl;
  os << indent << "PRF:                          "<<m_PRF <<std::endl;
  os << indent << "Original product size:        "<<m_OriginalProductSize << std::endl;
  os << indent << "Sensor local incidence angle: " << m_LocalIncidentAngle << std::endl;
  os << indent << "Fast calibration:             " << (m_UseFastCalibration ? "On" : "Off")<<std::endl;
  os << indent << "Results in decibels:          " << (m_ResultsInDecibels ? "Yes" : "No") << std::endl;
  os << indent << "Number of noise records:      " << m_NoiseRecords.size() <<std::endl;
}



} // namespace otb
#endif
