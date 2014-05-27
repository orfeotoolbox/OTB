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
#ifndef __otbStereoSensorModelToElevationMapFilter_txx
#define __otbStereoSensorModelToElevationMapFilter_txx

#include "otbStereoSensorModelToElevationMapFilter.h"

#include "itkImageRegionIteratorWithIndex.h"
#include "itkProgressReporter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbDEMHandler.h"

namespace otb
{
template <class TInputImage, class TOutputHeight>
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::StereoSensorModelToElevationFilter()
{
  // Filter has two inputs
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(1, OutputImageType::New());

  // Default interpolator
  m_Interpolator = itk::LinearInterpolateImageFunction<TInputImage>::New();

  // Default correlation radius
  m_Radius.Fill(3);

  // Height exploration setup
  m_LowerElevation = -20;
  m_HigherElevation = 20;
  m_ElevationStep = 1.;
  m_CorrelationThreshold = 0.7;
  m_VarianceThreshold = 4;
  m_SubtractInitialElevation = false;

  // DEM handling (deprecated)
  m_UseDEM = false;
}

template <class TInputImage, class TOutputHeight>
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::~StereoSensorModelToElevationFilter()
{}

template <class TInputImage, class TOutputHeight>
void
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::SetMasterInput(const TInputImage * image)
{
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TOutputHeight>
void
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::SetSlaveInput(const TInputImage * image)
{
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TOutputHeight>
const TInputImage *
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::GetMasterInput() const
{
  if(this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TOutputHeight>
const TInputImage *
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::GetSlaveInput() const
{
  if(this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputHeight>
TOutputHeight *
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::GetCorrelationOutput()
{
  if(this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<TOutputHeight *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputHeight>
void StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::GenerateInputRequestedRegion()
{
  // Call the superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve pointers
  InputImageType  * masterPtr  = const_cast<InputImageType *>(this->GetMasterInput());
  InputImageType  * slavePtr   = const_cast<InputImageType*>(this->GetSlaveInput());
  OutputImageType * outputPtr  = this->GetOutput();

  if ( !masterPtr || !slavePtr || !outputPtr )
    {
    return;
    }

  // get a copy of the fixed requested region (should equal the output
  // requested region)
  typename InputImageType::RegionType masterRequestedRegion, slaveRequestedRegion;
  masterRequestedRegion = outputPtr->GetRequestedRegion();

  // pad the master requested region by the operator radius
  masterRequestedRegion.PadByRadius( m_Radius );

  // Find corners of the master requested region
  typename InputImageType::IndexType mul, mur, mll, mlr;
  mul = masterRequestedRegion.GetIndex();
  mur = masterRequestedRegion.GetIndex();
  mur[0] += masterRequestedRegion.GetSize()[0]-1;
  mll = masterRequestedRegion.GetIndex();
  mll[1] += masterRequestedRegion.GetSize()[1]-1;
  mlr = masterRequestedRegion.GetIndex();
  mlr[0] += masterRequestedRegion.GetSize()[0]-1;
  mlr[1] += masterRequestedRegion.GetSize()[1]-1;

  // Transform to physical space
  typename InputImageType::PointType mpul, mpur, mpll, mplr;
  masterPtr->TransformIndexToPhysicalPoint(mul, mpul);
  masterPtr->TransformIndexToPhysicalPoint(mur, mpur);
  masterPtr->TransformIndexToPhysicalPoint(mll, mpll);
  masterPtr->TransformIndexToPhysicalPoint(mlr, mplr);

  // Build the transform to switch from the master to the slave image
  typename GenericRSTransformType::Pointer transform = GenericRSTransformType::New();
  transform->SetInputKeywordList(masterPtr->GetImageKeywordlist());
  transform->SetOutputKeywordList(slavePtr->GetImageKeywordlist());

  transform->InstanciateTransform();

  typename GenericRSTransformType::ParametersType params(1);

  // Build minimum height and maximum height points corresponding to
  // corners of the master requested region
  typename InputImageType::PointType sMinPul, sMinPur, sMinPll, sMinPlr, sMaxPul, sMaxPur, sMaxPll, sMaxPlr;

  // Lower case
  params[0] = m_LowerElevation;
  transform->SetParameters(params);

  sMinPul = transform->TransformPoint(mpul);
  sMinPur = transform->TransformPoint(mpur);
  sMinPll = transform->TransformPoint(mpll);
  sMinPlr = transform->TransformPoint(mplr);

  // Higher case
  params[0] = m_HigherElevation;
  transform->SetParameters(params);

  sMaxPul = transform->TransformPoint(mpul);
  sMaxPur = transform->TransformPoint(mpur);
  sMaxPll = transform->TransformPoint(mpll);
  sMaxPlr = transform->TransformPoint(mplr);

  // Transform to index
  typename InputImageType::IndexType sMinul, sMinur, sMinll, sMinlr, sMaxul, sMaxur, sMaxll, sMaxlr;

  slavePtr->TransformPhysicalPointToIndex(sMinPul, sMinul);
  slavePtr->TransformPhysicalPointToIndex(sMaxPul, sMaxul);
  slavePtr->TransformPhysicalPointToIndex(sMinPur, sMinur);
  slavePtr->TransformPhysicalPointToIndex(sMaxPur, sMaxur);
  slavePtr->TransformPhysicalPointToIndex(sMinPll, sMinll);
  slavePtr->TransformPhysicalPointToIndex(sMaxPll, sMaxll);
  slavePtr->TransformPhysicalPointToIndex(sMinPlr, sMinlr);
  slavePtr->TransformPhysicalPointToIndex(sMaxPlr, sMaxlr);

  // Find the corresponding bounding box
  typename InputImageType::IndexType sul, slr;

  sul[0] = std::min(std::min(std::min(sMinul[0], sMaxul[0]), std::min(sMinur[0], sMaxur[0])),
                    std::min(std::min(sMinll[0], sMaxll[0]), std::min(sMinlr[0], sMaxlr[0])));
  sul[1] = std::min(std::min(std::min(sMinul[1], sMaxul[1]), std::min(sMinur[1], sMaxur[1])),
                    std::min(std::min(sMinll[1], sMaxll[1]), std::min(sMinlr[1], sMaxlr[1])));
  slr[0] = std::max(std::max(std::max(sMinul[0], sMaxul[0]), std::max(sMinur[0], sMaxur[0])),
                    std::max(std::max(sMinll[0], sMaxll[0]), std::max(sMinlr[0], sMaxlr[0])));
  slr[1] = std::max(std::max(std::max(sMinul[1], sMaxul[1]), std::max(sMinur[1], sMaxur[1])),
                    std::max(std::max(sMinll[1], sMaxll[1]), std::max(sMinlr[1], sMaxlr[1])));

  // Build the slave requested region
  slaveRequestedRegion.SetIndex(sul);
  typename InputImageType::SizeType ssize;
  ssize[0] = slr[0] - sul[0]+1;
  ssize[1] = slr[1] - sul[1]+1;
  slaveRequestedRegion.SetSize(ssize);

  // crop the master region at the master's largest possible region
  if ( masterRequestedRegion.Crop(masterPtr->GetLargestPossibleRegion()))
    {
    masterPtr->SetRequestedRegion( masterRequestedRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    masterPtr->SetRequestedRegion( masterRequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << this->GetNameOfClass()
                << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(masterPtr);
    throw e;
    }

  // crop the slave region at the slave's largest possible region
  if ( slaveRequestedRegion.Crop(slavePtr->GetLargestPossibleRegion()))
    {
    slavePtr->SetRequestedRegion( slaveRequestedRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region). This case might happen so we do not throw any exception but
    // request a null region instead
    typename InputImageType::SizeType slaveSize;
    slaveSize.Fill(0);
    slaveRequestedRegion.SetSize(slaveSize);
    typename InputImageType::IndexType slaveIndex;
    slaveIndex.Fill(0);
    slaveRequestedRegion.SetIndex(slaveIndex);

    // store what we tried to request (prior to trying to crop)
    slavePtr->SetRequestedRegion(slaveRequestedRegion);
    }
  return;
}

template <class TInputImage, class TOutputHeight>
void
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::BeforeThreadedGenerateData()
{
  // Wire the interpolator
  m_Interpolator->SetInputImage(this->GetSlaveInput());
  this->GetCorrelationOutput()->FillBuffer(0.);

  // Initialize with average elevation
  this->GetOutput()->FillBuffer(otb::DEMHandler::Instance()->GetDefaultHeightAboveEllipsoid());

  // Initialize with DEM elevation (not threadable because of some
  // mutex in ossim)
  OutputImageType * outputPtr  = this->GetOutput();

  typename GenericRSTransformType::Pointer rsTransform = GenericRSTransformType::New();
  rsTransform->SetInputKeywordList(outputPtr->GetImageKeywordlist());
  rsTransform->InstanciateTransform();

  // Fill ouptut
  itk::ImageRegionIteratorWithIndex<OutputImageType> outputIt(outputPtr, outputPtr->GetBufferedRegion());

  for(outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt)
    {
    // Retrieve physical point
    OutputPointType outputPoint, geoPoint;
    outputPtr->TransformIndexToPhysicalPoint(outputIt.GetIndex(), outputPoint);

    // Transform to geo point
    geoPoint = rsTransform->TransformPoint(outputPoint);
    outputIt.Set(otb::DEMHandler::Instance()->GetHeightAboveEllipsoid(geoPoint));
    }

  const InputImageType* masterPtr  = this->GetMasterInput();
  const InputImageType* slavePtr   = this->GetSlaveInput();

  // Set-up the forward-inverse sensor model transform
  m_MasterToSlave = GenericRSTransform3DType::New();
  m_MasterToSlave->SetInputKeywordList(masterPtr->GetImageKeywordlist());
  m_MasterToSlave->SetOutputKeywordList(slavePtr->GetImageKeywordlist());
  m_MasterToSlave->InstanciateTransform();

}

template <class TInputImage, class TOutputHeight>
void
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::ThreadedGenerateData(const OutputRegionType & outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve pointers
  const InputImageType* masterPtr  = this->GetMasterInput();
  const InputImageType* slavePtr   = this->GetSlaveInput();
  OutputImageType* outputPtr  = this->GetOutput();
  OutputImageType* correlPtr = this->GetCorrelationOutput();

  // TODO: Uncomment when model optimization pushed
  // // GCP refinement
  // unsigned int gcpCount1 = this->GetMasterInput()->GetGCPCount();
  // unsigned int gcpCount2 = this->GetSlaveInput()->GetGCPCount();


  // transform->ClearInputTiePoints();
  // transform->ClearOutputTiePoints();

  // for(unsigned int i = 0; i < gcpCount1; ++i)
  //   {
  //   typename GenericRSTransformType::InputPointType imagePoint, groundPoint, groundPointWGS84;
  //   // Transform GCP ground part to WGS84
  //   groundPoint[0] = this->GetMasterInput()->GetGCPs(i).m_GCPX;
  //   groundPoint[1] = this->GetMasterInput()->GetGCPs(i).m_GCPY;

  //   typename GenericRSTransformType::Pointer gcpTransform = GenericRSTransformType::New();
  //   gcpTransform->SetInputProjectionRef(this->GetInput()->GetGCPProjection());
  //   gcpTransform->InstanciateTransform();

  //   groundPointWGS84 = gcpTransform->TransformPoint(groundPoint);

  //   imagePoint[0] = this->GetMasterInput()->GetGCPs(i).m_GCPCol;
  //   imagePoint[1] = this->GetMasterInput()->GetGCPs(i).m_GCPRow;

  //   transform->AddInputTiePoint(imagePoint, groundPointWGS84);
  //   transform->OptimizeInputTransformOn();
  //   }

  // for(unsigned int i = 0; i < gcpCount2; ++i)
  //   {
  //   typename GenericRSTransformType::InputPointType imagePoint, groundPoint, groundPointWGS84;
  //   // Transform GCP ground part to WGS84
  //   groundPoint[0] = this->GetSlaveInput()->GetGCPs(i).m_GCPX;
  //   groundPoint[1] = this->GetSlaveInput()->GetGCPs(i).m_GCPY;

  //   typename GenericRSTransformType::Pointer gcpTransform = GenericRSTransformType::New();
  //   gcpTransform->SetInputProjectionRef(this->GetInput()->GetGCPProjection());
  //   gcpTransform->InstanciateTransform();

  //   groundPointWGS84 = gcpTransform->TransformPoint(groundPoint);

  //   imagePoint[0] = this->GetSlaveInput()->GetGCPs(i).m_GCPCol;
  //   imagePoint[1] = this->GetSlaveInput()->GetGCPs(i).m_GCPRow;

  //   transform->AddOutputTiePoint(imagePoint, groundPointWGS84);
  //   transform->OptimizeOutputTransformOn();
  //   }


  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define an iterator on the output elevation map
  itk::ConstNeighborhoodIterator<InputImageType> inputIt(m_Radius, masterPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr, outputRegionForThread);
  itk::ImageRegionIteratorWithIndex<OutputImageType> correlIt(correlPtr, outputRegionForThread);

  // Start visiting buffer again
  outputIt.GoToBegin();
  correlIt.GoToBegin();
  inputIt.GoToBegin();


  // This will hold the master patch
  std::vector<double> master;
  master.reserve(inputIt.Size());
  master = std::vector<double>(inputIt.Size(), 0);

  // And this will hold the slave patch
  std::vector<double> slave;
  slave.reserve(inputIt.Size());
  slave = std::vector<double>(inputIt.Size(), 0);


  // Walk the output map
  while(!outputIt.IsAtEnd() && !inputIt.IsAtEnd())
    {
    // Define some loop variables
    typename InputImageType::PointType inPoint, outPoint, currentPoint, optimalPoint;
    typename GenericRSTransform3DType::InputPointType in3DPoint, out3DPoint;
    typename InputImageType::IndexType index;

    // Retrieve intial height
    double initHeight = outputIt.Get();
    double optimalHeight = initHeight;
    double optimalCorrelation = 0;

    // Check if there is an height info
    if(initHeight != -32768)
      {
      // These are used to estimate master patch variance
      double masterSum = 0;
      double masterVariance = 0;

      // Fill master patch
      for(unsigned int i = 0; i < inputIt.Size(); ++i)
        {
        // Add to the master vector
        double value = inputIt.GetPixel(i);
        master[i] = value;

        // Cumulate for mean
        masterSum += value;
        }

      // Finalize mean
      masterSum /= inputIt.Size();

      // Complete pooled variance computation
      for(unsigned int i = 0; i < inputIt.Size(); ++i)
        {
        masterVariance += (master[i] - masterSum) * (master[i] - masterSum);
        }
      masterVariance /= (inputIt.Size()-1);

      // Check for high enough variance so that correlation will be reliable
      if(masterVariance > m_VarianceThreshold)
        {
        // Explore candidate heights
        for(double height = initHeight + m_LowerElevation;
            height < initHeight + m_HigherElevation;
            height += m_ElevationStep)
          {

          // Interpolate slave patch
          for(unsigned int i = 0; i < inputIt.Size(); ++i)
            {
            // Retrieve the current index
            index = inputIt.GetIndex(i);

            // Retrieve master physical point
            masterPtr->TransformIndexToPhysicalPoint(index, inPoint);
            in3DPoint[0] = inPoint[0];
            in3DPoint[1] = inPoint[1];
            in3DPoint[2] = height;

            // Transform to slave
            out3DPoint = m_MasterToSlave->TransformPoint(in3DPoint);
            outPoint[0] = out3DPoint[0];
            outPoint[1] = out3DPoint[1];

            // Interpolate
            if(m_Interpolator->IsInsideBuffer(outPoint))
              {
              // And fill slave vector
              slave[i] = m_Interpolator->Evaluate(outPoint);
              }
            else
              {
              // If out of buffer, fill with 0.
              slave[i] = 0.;
              }
            }

          // Now that we have the master and slave patches, call the
          // correlation function
          double correlationValue = this->Correlation(master, slave);

          // Check if a better correlation was found
          if(correlationValue > optimalCorrelation)
            {
            // If found, update values
            optimalCorrelation = correlationValue;
            optimalHeight = height;
            }
          }
        }
        // TODO: refinenement step ?
      }

    // Final check on best correlation value
    double finalOffset = 0.;

    // Switch to subtract initial elevation mode
    if(m_SubtractInitialElevation)
      {
      finalOffset = initHeight;
      }

    if(optimalCorrelation > m_CorrelationThreshold)
      {
      outputIt.Set(optimalHeight-finalOffset);
      correlIt.Set(optimalCorrelation);
      }
    else
      {
      outputIt.Set(initHeight - finalOffset);
      correlIt.Set(0);
      }

    // Update progress
    progress.CompletedPixel();

    // And iterators
    ++inputIt;
    ++outputIt;
    ++correlIt;
    }
}

template <class TInputImage, class TOutputHeight>
double
StereoSensorModelToElevationFilter<TInputImage, TOutputHeight>
::Correlation(const std::vector<double>& master, const std::vector<double>& slave) const
{
  double meanSlave = 0;
  double meanMaster = 0;
  double correlationValue = 0;

  // Compute means
  for(unsigned int i = 0; i < master.size(); ++i)
    {
    meanSlave += slave[i];
    meanMaster += master[i];
    }
  meanSlave /= slave.size();
  meanMaster /= master.size();

  double crossProd = 0.;
  double squareSumMaster = 0.;
  double squareSumSlave = 0.;


  // Compute correlation
  for(unsigned int i = 0; i < master.size(); ++i)
    {
    crossProd += (slave[i]-meanSlave) * (master[i]-meanMaster);
    squareSumSlave += (slave[i]-meanSlave) * (slave[i]-meanSlave);
    squareSumMaster += (master[i]-meanMaster) * (master[i]-meanMaster);
    }

  correlationValue = vcl_abs(crossProd/(vcl_sqrt(squareSumSlave)*vcl_sqrt(squareSumMaster)));

  return correlationValue;
}

} // end namespace otb

#endif
