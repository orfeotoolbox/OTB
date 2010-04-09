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

#include <vnl/algo/vnl_lsqr.h>
#include <vnl/vnl_sparse_matrix_linear_system.h>
#include <vnl/vnl_least_squares_function.h>

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::TerraSarCalibrationImageFilter() : m_CalibrationFactor(itk::NumericTraits<double>::Zero),
  m_PRF(1.),
  m_OriginalProductSize(),
  m_UseFastCalibration(false),
  m_ResultsInDecibels(false),
  m_NoiseRecords(),
  m_DefaultValue(0.00001),
  m_IncidenceAngleRecords(),
  m_IncidenceAngleAx(0.),
  m_IncidenceAngleAy(0.),
  m_IncidenceAngleOffset(0.),
  m_ParametersUpToDate(false)
{}

template <class TInputImage, class TOutputImage>
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::~TerraSarCalibrationImageFilter()
{
  // Clear any noise records
  this->ClearNoiseRecords();
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::Modified()
{
  // Call Superclass implementation
  Superclass::Modified();
  m_ParametersUpToDate = false;
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::AddNoiseRecord(double utcAcquisitionTime, const ImageNoiseType& record)
{
  // Create the pair
  NoiseRecordType newRecord(utcAcquisitionTime, record);

  // Add it to the list
  m_NoiseRecords.push_back(newRecord);

  // Call modified method
  this->Modified();
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::ClearNoiseRecords()
{
  m_NoiseRecords.clear();

  // Call modified method
  this->Modified();
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::AddIncidenceAngleRecord(const IndexType& index, double angle)
{
  IncidenceAngleRecordType record(index, angle);
  m_IncidenceAngleRecords.push_back(record);

  // Call modified method
  this->Modified();
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::ClearIncidenceAngleRecords()
{
  m_IncidenceAngleRecords.clear();

  // Call modified method
  this->Modified();
}

template <class TInputImage, class TOutputImage>
bool
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::CompareNoiseRecords(const NoiseRecordType& record1, const NoiseRecordType& record2)
{
  return (record1.first > record2.first);
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  // Check if we need to update parameters
  if (m_ParametersUpToDate)
    {
    return;
    }

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
      itkExceptionMacro(<< "Invalid input image. Only TerraSar images are supported");
      }
    }

  // Noise records
  if (m_NoiseRecords.empty())
    {
    if (mdIsAvailable)
      {
      // Retrieve the number of noise records
      unsigned int nbNoiseRecords = lImageMetadata->GetNumberOfNoiseRecords(this->GetInput()->GetMetaDataDictionary());

      // Retrieve the noise records
      ossimplugins::Noise * noiseRecords = lImageMetadata->GetNoise(this->GetInput()->GetMetaDataDictionary());

      // Retrieve corresponding times
      std::vector<double> noiseTimes = lImageMetadata->GetNoiseTimeUTCList(this->GetInput()->GetMetaDataDictionary());

      if (!m_UseFastCalibration && (noiseTimes.empty() || nbNoiseRecords == 0))
        {
        itkExceptionMacro(<< "No noise records found. Consider using fast calibration.");
        }

      // For each noise record, add it
      for (unsigned int i = 0; i < nbNoiseRecords; ++i)
        {
        this->AddNoiseRecord(noiseTimes[i], noiseRecords->get_imageNoise()[i]);
        }

      // Free memory
      delete noiseRecords;
      }
    else
      {
      itkExceptionMacro(<< "Invalid input image. Only TerraSar images are supported");
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
      itkExceptionMacro(<< "Invalid input image. Only TerraSar images are supported");
      }
    }

  // Incident Angle
  if (m_IncidenceAngleRecords.empty())
    {
    if (mdIsAvailable)
      {
      // Retrieve center incidence angle
      double    centerAngle = lImageMetadata->GetCenterIncidenceAngle(this->GetInput()->GetMetaDataDictionary());
      IndexType centerIndex = lImageMetadata->GetCenterIncidenceAngleIndex(this->GetInput()->GetMetaDataDictionary());
      this->AddIncidenceAngleRecord(centerIndex, centerAngle);

      // Retrieve corners incidence angle
      std::vector<double> cangles = lImageMetadata->GetCornersIncidenceAngles(
        this->GetInput()->GetMetaDataDictionary());
      std::vector<IndexType> cindex = lImageMetadata->GetCornersIncidenceAnglesIndex(
        this->GetInput()->GetMetaDataDictionary());

      std::vector<double>::const_iterator             angIt = cangles.begin();
      typename std::vector<IndexType>::const_iterator indIt = cindex.begin();

      // Add each corner angle record as well
      while (angIt != cangles.end() && indIt != cindex.end())
        {
        this->AddIncidenceAngleRecord(*indIt, *angIt);
        ++angIt;
        ++indIt;
        }
      }
    else
      {
      itkExceptionMacro(<< "Invalid input image. Only TerraSar images are supported");
      }
    }
  // Ensure that noise records are sorted by decreasing acquisition
  // date
  std::sort(m_NoiseRecords.begin(), m_NoiseRecords.end(), &Self::CompareNoiseRecords);

  // Estimate angular plane parameters
  this->EstimateAngularPlaneParameters();

  // Mark parameters as up-to-date
  m_ParametersUpToDate = true;
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::EstimateAngularPlaneParameters()
{
  // Check if there are any incidence angle records
  if (m_IncidenceAngleRecords.empty())
    {
    itkExceptionMacro(<< "No incidence angle records found, can not perform calibration!");
    }

  // If we do not have enough point to do the least square regression,
  // set default value as a constant mean angle
  if (m_IncidenceAngleRecords.size() < 3)
    {
    m_IncidenceAngleAx = 0.;
    m_IncidenceAngleAy = 0.;
    m_IncidenceAngleOffset = 0.;

    typename IncidenceAngleRecordVectorType::const_iterator
      aIt = m_IncidenceAngleRecords.begin();

    // Compute the mean incidence angle
    while (aIt != m_IncidenceAngleRecords.end())
      {
      m_IncidenceAngleOffset += aIt->second;
      ++aIt;
      }
    // Set the offset to the mean angle
    m_IncidenceAngleOffset /= m_IncidenceAngleRecords.size();
    m_IncidenceAngleOffset *= M_PI / 180.;
    return;
    }

  // Perform the plane least square estimation
  unsigned int nbRecords = m_IncidenceAngleRecords.size();
  vnl_sparse_matrix<double> a(nbRecords, 3);
  vnl_vector<double> b(nbRecords), bestParams(3);

  // Fill the linear system
  for (unsigned int i = 0; i < nbRecords; ++i)
    {
    a(i, 0) = m_IncidenceAngleRecords.at(i).first[0];
    a(i, 1) = m_IncidenceAngleRecords.at(i).first[1];
    a(i, 2) = 1.;
    b[i] = m_IncidenceAngleRecords.at(i).second * M_PI / 180.;
    }

  // Create the linear system
  vnl_sparse_matrix_linear_system<double> linearSystem(a, b);
  vnl_lsqr linearSystemSolver(linearSystem);

  // And solve it
  linearSystemSolver.minimize(bestParams);

  m_IncidenceAngleAx     = bestParams[0];
  m_IncidenceAngleAy     = bestParams[1];
  m_IncidenceAngleOffset = bestParams[2];
}

template <class TInputImage, class TOutputImage>
double
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::ComputeIncidenceAngle(const IndexType& index) const
{
  // Apply the regression
  double angle = m_IncidenceAngleAx * index[0] + m_IncidenceAngleAy * index[1] + m_IncidenceAngleOffset;
  return angle;
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
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
  calibrationFunctor.SetUseFastCalibration(m_UseFastCalibration);
  calibrationFunctor.SetResultsInDecibels(m_ResultsInDecibels);
  calibrationFunctor.SetOriginalProductSize(m_OriginalProductSize);
  calibrationFunctor.SetDefaultValue(m_DefaultValue);

  // Set up progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  assert(!m_NoiseRecords.empty());

  // The acquisition time of the first line of OutputRegionForThread.
  // Adapt frequency to julien day (1s <-> 1/24*60*60 = 1/86400 julienDay)
  double invPRF = (1. / m_PRF * 86400.);
  double currentAzimuthPosition = m_NoiseRecords.back().first + invPRF
                                  * (m_OriginalProductSize[1] - inputIt.GetIndex()[1] - 1);

  // Look for the first noise record to be used (remember we sorted
  // m_NoiseRecords by decreasing time)
  NoiseRecordVectorType::const_iterator currentNoiseRecordIt = m_NoiseRecords.begin();

  unsigned int nrIndex = 0;

  // Iterate until we find it
  while (currentNoiseRecordIt != m_NoiseRecords.end() && currentNoiseRecordIt->first > currentAzimuthPosition)
    {
    ++nrIndex;
    ++currentNoiseRecordIt;
    }

  assert(currentNoiseRecordIt != m_NoiseRecords.end());

  // Store the current noise record azimuth position
  double currentNoiseRecordAzimuthPosition = currentNoiseRecordIt->first;
  calibrationFunctor.SetNoiseRecord(currentNoiseRecordIt->second);

  // Store current line index
  typename OutputImageRegionType::IndexType::IndexValueType currentLine = inputIt.GetIndex()[1];

  // Iterate on the input and output buffer
  while (!inputIt.IsAtEnd())
    {
    // Check if we changed line
    if (currentLine != inputIt.GetIndex()[1])
      {
      // If so, update the current azimuth time
      currentAzimuthPosition -= invPRF;

      // Update line counter
      currentLine = inputIt.GetIndex()[1];

      // And check if we changed of NoiseRecord
      if (currentAzimuthPosition < currentNoiseRecordAzimuthPosition)
        {
        // Update the iterator
        ++currentNoiseRecordIt;
        // If we are not at the last record
        if (currentNoiseRecordIt != m_NoiseRecords.end())
          {
          // Update the functor noise record
          currentNoiseRecordAzimuthPosition = currentNoiseRecordIt->first;
          calibrationFunctor.SetNoiseRecord(currentNoiseRecordIt->second);
          }
        }
      }
    // Apply the calibration functor
    outputIt.Set(calibrationFunctor(inputIt.Get(), inputIt.GetIndex(),
                                    this->ComputeIncidenceAngle(inputIt.GetIndex())));
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}

template <class TInputImage, class TOutputImage>
void
TerraSarCalibrationImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Calibration Factor:           " << m_CalibrationFactor  << std::endl;
  os << indent << "PRF:                          " << m_PRF << std::endl;
  os << indent << "Original product size:        " << m_OriginalProductSize << std::endl;
  os << indent << "Fast calibration:             " << (m_UseFastCalibration ? "On" : "Off") << std::endl;
  os << indent << "Results in decibels:          " << (m_ResultsInDecibels ? "Yes" : "No") << std::endl;
  os << indent << "Number of noise records:      " << m_NoiseRecords.size() << std::endl;
  os << indent << "Number of angle records:      " << m_IncidenceAngleRecords.size() << std::endl;
  os << indent << "Angle regression:             " << m_IncidenceAngleAx
     << "* col + " << m_IncidenceAngleAx
     << " * row +" << m_IncidenceAngleOffset << std::endl;
}

} // namespace otb
#endif
