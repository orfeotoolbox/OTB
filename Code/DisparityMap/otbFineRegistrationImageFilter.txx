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
#ifndef __otbFineRegistrationImageFilter_txx
#define __otbFineRegistrationImageFilter_txx

#include "otbFineRegistrationImageFilter.h"

#include "itkProgressReporter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
FineRegistrationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::FineRegistrationImageFilter()
 {
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfOutputs(2);
  this->SetNthOutput(1,TOutputDeformationField::New());

  // Default radius
  m_Radius.Fill(2);
  m_SearchRadius.Fill(4);

  // Default sub-pixel precision
  m_SubPixelAccuracy = 0.1;

  // Flags
  m_UseSpacing = true;
  m_Minimize   = true;

  // Default currentMetric
  m_Metric     = itk::NormalizedCorrelationImageToImageMetric
      <TInputImage,TInputImage>::New();

  // Default interpolator
  m_Interpolator = itk::LinearInterpolateImageFunction<TInputImage,double>::New();

  // Translation
  m_Translation = TranslationType::New();
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
void
FineRegistrationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::SetFixedInput( const TInputImage * image )
 {
  // Process object is not const-correct so the const casting is required.
  SetNthInput(0, const_cast<TInputImage *>( image ));
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
void
FineRegistrationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::SetMovingInput( const TInputImage * image)
 {
  // Process object is not const-correct so the const casting is required.
  SetNthInput(1, const_cast<TInputImage *>( image ));
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
const TInputImage *
FineRegistrationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::GetFixedInput()
 {
  if (this->GetNumberOfInputs()<1)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
const TInputImage *
FineRegistrationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::GetMovingInput()
 {
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
TOutputDeformationField *
FineRegistrationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::GetOutputDeformationField()
 {
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<TOutputDeformationField *>(this->itk::ProcessObject::GetOutput(1));
 }


template <class TInputImage, class TOutputCorrelation, class TOutputDeformationField>
void
FineRegistrationImageFilter<TInputImage,TOutputCorrelation,TOutputDeformationField>
::GenerateInputRequestedRegion()
 {
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  TInputImage * fixedPtr =  const_cast< TInputImage * >( this->GetFixedInput());
  TInputImage * movingPtr = const_cast< TInputImage * >( this->GetMovingInput());

  TOutputCorrelation * outputPtr = this->GetOutput();

  if ( !fixedPtr || !movingPtr || !outputPtr )
    {
    return;
    }

  // get a copy of the fixed requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType fixedRequestedRegion;
  fixedRequestedRegion = fixedPtr->GetRequestedRegion();
  // pad the input requested region by the operator radius
  fixedRequestedRegion.PadByRadius( m_Radius );

  // get a copy of the moving requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType movingRequestedRegion;
  movingRequestedRegion = movingPtr->GetRequestedRegion();
  // pad the input requested region by the operator radius
  movingRequestedRegion.PadByRadius( m_SearchRadius + m_Radius );

  // crop the fixed region at the fixed's largest possible region
  if ( fixedRequestedRegion.Crop(fixedPtr->GetLargestPossibleRegion()))
    {
    fixedPtr->SetRequestedRegion( fixedRequestedRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    fixedPtr->SetRequestedRegion( fixedRequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
            << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(fixedPtr);
    throw e;
    }

  // crop the moving region at the moving's largest possible region
  if ( movingRequestedRegion.Crop(movingPtr->GetLargestPossibleRegion()))
    {
    movingPtr->SetRequestedRegion( movingRequestedRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    movingPtr->SetRequestedRegion( movingRequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
            << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(movingPtr);
    throw e;
    }

  return;
 }

template <class TInputImage, class TOutputCorrelation, class TOutputDeformationField>
void
FineRegistrationImageFilter<TInputImage,TOutputCorrelation,TOutputDeformationField>
::GenerateData()
 {
  // Allocate outputs
  this->AllocateOutputs();

  // Get the image pointers
  const TInputImage * fixedPtr = this->GetFixedInput();
  const TInputImage * movingPtr = this->GetMovingInput();
  TOutputCorrelation * outputPtr = this->GetOutput();
  TOutputDeformationField * outputDfPtr = this->GetOutputDeformationField();

  // Wire currentMetric
  m_Interpolator->SetInputImage(this->GetMovingInput());
  m_Metric->SetTransform(m_Translation);
  m_Metric->SetInterpolator(m_Interpolator);
  m_Metric->SetFixedImage(this->GetFixedInput());
  m_Metric->SetMovingImage(this->GetMovingInput());
  m_Metric->SetComputeGradient(false);

  /** Output iterators */
  itk::ImageRegionIteratorWithIndex<TOutputCorrelation> outputIt(outputPtr,outputPtr->GetRequestedRegion());
  itk::ImageRegionIterator<TOutputDeformationField> outputDfIt(outputDfPtr,outputPtr->GetRequestedRegion());
  outputIt.GoToBegin();
  outputDfIt.GoToBegin();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, 0, outputPtr->GetRequestedRegion().GetNumberOfPixels());

  // GoToBegin
  outputIt.GoToBegin();
  outputDfIt.GoToBegin();

  // Correl, max correl, maxPosition
  double currentMetric, optMetric;

  // Optimal translation parameters
  typename TranslationType::ParametersType params(2), optParams(2), tmpOptParams(2);

  // Final deformation value
  DeformationValueType deformationValue;

  // Get fixed image spacing
  SpacingType fixedSpacing = fixedPtr->GetSpacing();

  // Walk the images
  while (!outputIt.IsAtEnd() && !outputDfIt.IsAtEnd() )
    {
    // Reset
    if(m_Minimize)
      {
      optMetric = itk::NumericTraits<double>::max();
      }
    else
      {
      optMetric = itk::NumericTraits<double>::NonpositiveMin();
      }
    optParams.Fill(0);

    // Build the region on which to compute the currentMetric
    InputImageRegionType currentMetricRegion;
    currentMetricRegion.SetIndex(outputIt.GetIndex());
    SizeType size;
    size.Fill(1);
    currentMetricRegion.SetSize(size);
    currentMetricRegion.PadByRadius(m_Radius);
    currentMetricRegion.Crop(fixedPtr->GetLargestPossibleRegion());
    m_Metric->SetFixedImageRegion(currentMetricRegion);
    m_Metric->Initialize();

    // Compute the correlation at each location
    for(int i =-(int)m_SearchRadius[0]; i<=(int)m_SearchRadius[0];++i)
      {
      for(int j=-(int)m_SearchRadius[1]; j<=(int)m_SearchRadius[1];++j)
        {
        params[0]=static_cast<double>(i*fixedSpacing[0]);
        params[1]=static_cast<double>(j*fixedSpacing[1]);

        // compute currentMetric
        currentMetric = m_Metric->GetValue(params);

        // Check for maximum
        if((m_Minimize && (currentMetric < optMetric)) || (!m_Minimize && (currentMetric > optMetric)))
          {
          optMetric = currentMetric;
          optParams = params;
          }
        }
      }

//      // Exhaustive sub-pixel
//      tmpOptParams = optParams;
//      for(double dx = -vcl_abs(fixedSpacing[0]);dx<vcl_abs(fixedSpacing[0]);dx+=m_SubPixelAccuracy)
//        {
//        for(double dy = -vcl_abs(fixedSpacing[1]);dy<vcl_abs(fixedSpacing[1]);dy+=m_SubPixelAccuracy)
//          {
//          params[0] = tmpOptParams[0]+dx;
//          params[1] = tmpOptParams[1]+dy;
//
//          // compute currentMetric
//          currentMetric = m_Metric->GetValue(params);
//
//          // Check for maximum
//          if((m_Minimize && (currentMetric < optMetric)) || (!m_Minimize && (currentMetric > optMetric)))
//            {
//            optMetric = currentMetric;
//            optParams = params;
//            }
//          }
//        }

    // Dichotomic sub-pixel
    SpacingType subPixelSpacing = fixedSpacing;
    while(subPixelSpacing[0]> m_SubPixelAccuracy || subPixelSpacing[1]> m_SubPixelAccuracy)
      {
      // Perform 1 step of dichotomic search
      subPixelSpacing/=2.;

      // Store last opt params
      tmpOptParams = optParams;

      for(int i =-1; i<=1;i+=2)
        {
        for(int j=-1; j<=1;j+=2)
          {
          params = tmpOptParams;
          params[0]+=static_cast<double>(i*subPixelSpacing[0]);
          params[1]+=static_cast<double>(j*subPixelSpacing[1]);

          // compute currentMetric
          currentMetric = m_Metric->GetValue(params);

          // Check for maximum
          if((m_Minimize && (currentMetric < optMetric)) || (!m_Minimize && (currentMetric > optMetric)))
            {
            optMetric = currentMetric;
            optParams = params;
            }
          }
        }
      }

    // Store the offset and the correlation value
    outputIt.Set(optMetric);
    deformationValue[0] = optParams[0];
    deformationValue[1] = optParams[1];
    outputDfIt.Set(deformationValue);
    // Update iterators
    ++outputIt;
    ++outputDfIt;

    // Update progress
    progress.CompletedPixel();
    }
  }
} // end namespace otb

#endif
