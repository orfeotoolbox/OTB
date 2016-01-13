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
#include "itkMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class T0utputCorrelation, class TOutputDisplacementField>
FineRegistrationImageFilter<TInputImage, T0utputCorrelation, TOutputDisplacementField>
::FineRegistrationImageFilter()
 {
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(1, TOutputDisplacementField::New());

  // Default radius
  m_Radius.Fill(2);
  m_SearchRadius.Fill(4);

  // Default sub-pixel precision
  m_ConvergenceAccuracy = 0.01;
  
  // Max number of iterations
  m_MaxIter = 100;

  // Flags
  m_UseSpacing = true;
  m_Minimize   = true;

  // Default currentMetric
  m_Metric     = itk::NormalizedCorrelationImageToImageMetric
      <TInputImage, TInputImage>::New();

  // Default interpolator
  m_Interpolator = itk::LinearInterpolateImageFunction<TInputImage, double>::New();

  // Translation
  m_Translation = TranslationType::New();

  // Grid Step
  m_GridStep.Fill(1);

  // Default offset
  m_InitialOffset.Fill(0);

  m_Transform = NULL;
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDisplacementField>
void
FineRegistrationImageFilter<TInputImage, T0utputCorrelation, TOutputDisplacementField>
::SetFixedInput( const TInputImage * image )
 {
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDisplacementField>
void
FineRegistrationImageFilter<TInputImage, T0utputCorrelation, TOutputDisplacementField>
::SetMovingInput( const TInputImage * image)
 {
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDisplacementField>
const TInputImage *
FineRegistrationImageFilter<TInputImage, T0utputCorrelation, TOutputDisplacementField>
::GetFixedInput()
 {
  if (this->GetNumberOfInputs()<1)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDisplacementField>
const TInputImage *
FineRegistrationImageFilter<TInputImage, T0utputCorrelation, TOutputDisplacementField>
::GetMovingInput()
 {
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
 }

template <class TInputImage, class T0utputCorrelation, class TOutputDisplacementField>
TOutputDisplacementField *
FineRegistrationImageFilter<TInputImage, T0utputCorrelation, TOutputDisplacementField>
::GetOutputDisplacementField()
 {
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<TOutputDisplacementField *>(this->itk::ProcessObject::GetOutput(1));
 }

template <class TInputImage, class TOutputCorrelation, class TOutputDisplacementField>
void
FineRegistrationImageFilter<TInputImage, TOutputCorrelation, TOutputDisplacementField>
::GenerateOutputInformation()
 {
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  // Retrieve output pointers
  TOutputCorrelation * outputPtr = this->GetOutput();
  TOutputDisplacementField *outputFieldPtr = this->GetOutputDisplacementField();

  // Update size and spacing according to grid step
  InputImageRegionType largestRegion  = outputPtr->GetLargestPossibleRegion();
  SizeType outputSize       = largestRegion.GetSize();
  SpacingType outputSpacing = outputPtr->GetSpacing();

  for(unsigned int dim = 0; dim < TOutputCorrelation::ImageDimension; ++dim)
    {
    outputSize[dim] /= m_GridStep[dim];
    outputSpacing[dim] *= m_GridStep[dim];
    }

  // Set spacing
  outputPtr->SetSpacing(outputSpacing);
  outputFieldPtr->SetSpacing(outputSpacing);

  // Set largest region size
  largestRegion.SetSize(outputSize);
  outputPtr->SetLargestPossibleRegion(largestRegion);
  outputFieldPtr->SetLargestPossibleRegion(largestRegion);
 }

template <class TInputImage, class TOutputCorrelation, class TOutputDisplacementField>
void
FineRegistrationImageFilter<TInputImage, TOutputCorrelation, TOutputDisplacementField>
::GenerateInputRequestedRegion()
 {
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  TInputImage * fixedPtr  = const_cast< TInputImage * >( this->GetFixedInput());
  TInputImage * movingPtr = const_cast< TInputImage * >( this->GetMovingInput());

  TOutputCorrelation * outputPtr = this->GetOutput();

  if ( !fixedPtr || !movingPtr || !outputPtr )
    {
    return;
    }

  // get a copy of the fixed requested region (should equal the output
  // requested region)
  InputImageRegionType fixedRequestedRegion, movingRequestedRegion;
  fixedRequestedRegion = outputPtr->GetRequestedRegion();

  // Apply grid step
  SizeType fixedRequestedSize = fixedRequestedRegion.GetSize();
  IndexType fixedRequestedIndex = fixedRequestedRegion.GetIndex();

  for(unsigned int dim = 0; dim < TOutputCorrelation::ImageDimension; ++dim)
      {
      fixedRequestedSize [dim] *= m_GridStep[dim];
      fixedRequestedIndex[dim] *= m_GridStep[dim];
      }

  fixedRequestedRegion.SetSize(fixedRequestedSize);
  fixedRequestedRegion.SetIndex(fixedRequestedIndex);

  // pad the input requested region by the operator radius
  fixedRequestedRegion.PadByRadius( m_Radius );


  // get a copy of the moving requested region (should equal the output
  // requested region)
  InputImageRegionType searchFixedRequestedRegion = fixedRequestedRegion;
  searchFixedRequestedRegion.PadByRadius(m_SearchRadius);


  // Find corners of the search window
   IndexType ulIndex = searchFixedRequestedRegion.GetIndex();

   IndexType lrIndex;
   for(unsigned int dim = 0; dim < TInputImage::ImageDimension; ++dim)
     {
     lrIndex[dim]= searchFixedRequestedRegion.GetIndex()[dim]
                 + searchFixedRequestedRegion.GetSize()[dim]-1;
     }

   // Transform to physical space
   PointType ulPoint, lrPoint;
   fixedPtr->TransformIndexToPhysicalPoint(lrIndex, lrPoint);
   fixedPtr->TransformIndexToPhysicalPoint(ulIndex, ulPoint);

   // Apply default offset
   lrPoint += m_InitialOffset;
   ulPoint += m_InitialOffset;

   // Transform back into moving region index space
   IndexType movingIndex1, movingIndex2, movingIndex;
   movingPtr->TransformPhysicalPointToIndex(ulPoint, movingIndex1);
   movingPtr->TransformPhysicalPointToIndex(lrPoint, movingIndex2);

   // Find requested region
   SizeType movingSize;

   for(unsigned int dim = 0; dim < TInputImage::ImageDimension; ++dim)
     {
       movingIndex[dim] = std::min(movingIndex1[dim], movingIndex2[dim]);
       movingSize[dim] = std::max(movingIndex1[dim], movingIndex2[dim]) - movingIndex[dim] + 1;
     }

   movingRequestedRegion.SetIndex(movingIndex);
   movingRequestedRegion.SetSize(movingSize);

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
    std::ostringstream msg;
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
    // possible region). This case might happen so we do not throw any exception but
    // request a null region instead
    movingSize.Fill(0);
    movingRequestedRegion.SetSize(movingSize);
    movingIndex.Fill(0);
    movingRequestedRegion.SetIndex(movingIndex);

    // store what we tried to request (prior to trying to crop)
    movingPtr->SetRequestedRegion(movingRequestedRegion);
    }
  return;
 }

template <class TInputImage, class TOutputCorrelation, class TOutputDisplacementField>
void
FineRegistrationImageFilter<TInputImage, TOutputCorrelation, TOutputDisplacementField>
::GenerateData()
 {
  // Allocate outputs
  this->AllocateOutputs();

  // Get the image pointers
  const TInputImage * fixedPtr = this->GetFixedInput();
  const TInputImage * movingPtr = this->GetMovingInput();
  TOutputCorrelation * outputPtr = this->GetOutput();
  TOutputDisplacementField * outputDfPtr = this->GetOutputDisplacementField();

  // Wire currentMetric
  m_Interpolator->SetInputImage(this->GetMovingInput());
  m_Metric->SetTransform(m_Translation);
  m_Metric->SetInterpolator(m_Interpolator);
  m_Metric->SetFixedImage(fixedPtr);
  m_Metric->SetMovingImage(movingPtr);
  m_Metric->SetComputeGradient(false);

  /** Output iterators */
  itk::ImageRegionIteratorWithIndex<TOutputCorrelation> outputIt(outputPtr, outputPtr->GetRequestedRegion());
  itk::ImageRegionIterator<TOutputDisplacementField> outputDfIt(outputDfPtr, outputPtr->GetRequestedRegion());
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
  typename TranslationType::ParametersType params(2), optParams(2);

  // Final displacement value
  DisplacementValueType displacementValue;
  displacementValue[0] = m_InitialOffset[0];
  displacementValue[1] = m_InitialOffset[1];

  // Local initial offset: enable the possibility of a different initial offset for each pixel
  SpacingType localOffset = m_InitialOffset;

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

    // Apply grid step
    IndexType currentIndex = outputIt.GetIndex();
    for(unsigned int dim = 0; dim < TInputImage::ImageDimension; ++dim)
      {
      currentIndex[dim] *= m_GridStep[dim];
      }
    currentMetricRegion.SetIndex(currentIndex);
    SizeType size;
    size.Fill(1);
    currentMetricRegion.SetSize(size);
    currentMetricRegion.PadByRadius(m_Radius);
    currentMetricRegion.Crop(fixedPtr->GetLargestPossibleRegion());
    m_Metric->SetFixedImageRegion(currentMetricRegion);
    m_Metric->Initialize();


    // Compute the local offset if required (and the transform was specified)
    if (m_Transform.IsNotNull())
      {
      PointType inputPoint, outputPoint;
      for(unsigned int dim = 0; dim < TInputImage::ImageDimension; ++dim)
        {
        inputPoint[dim] = currentIndex[dim];
        }
      outputPoint = m_Transform->TransformPoint(inputPoint);
      for(unsigned int dim = 0; dim < TInputImage::ImageDimension; ++dim)
        {
        localOffset[dim] = outputPoint[dim] - inputPoint[dim]; //FIXME check the direction
        }
      }

    // Compute the correlation at each location
    for(int i = -static_cast<int>(m_SearchRadius[0]); i <= static_cast<int>(m_SearchRadius[0]); ++i)
      {
      for(int j = -static_cast<int>(m_SearchRadius[1]); j <= static_cast<int>(m_SearchRadius[1]); ++j)
        {
        params[0] = localOffset[0] + static_cast<double>(i*fixedSpacing[0]);
        params[1] = localOffset[1] + static_cast<double>(j*fixedSpacing[1]);

        try
        {
          // compute currentMetric
          currentMetric = m_Metric->GetValue(params);

          // Check for maximum
          if((m_Minimize && (currentMetric < optMetric)) || (!m_Minimize && (currentMetric > optMetric)))
            {
            optMetric = currentMetric;
            optParams = params;
            }
        }
        catch(itk::ExceptionObject& err)
        {
          itkWarningMacro(<<err.GetDescription());
        }
        }
      }
  
    

    //golden section search
    typename TranslationType::ParametersType paramsgn(2);
    double gn=(sqrt(5.0)-1.0)/2.0;
    SpacingType subPixelSpacing = fixedSpacing;
    double ax=optParams[0]-static_cast<double>(subPixelSpacing[0]);
    double ay=optParams[1]-static_cast<double>(subPixelSpacing[1]);
    double bx=optParams[0]+static_cast<double>(subPixelSpacing[0]);
    double by=optParams[1]+static_cast<double>(subPixelSpacing[1]);

    
    double cx=bx-gn*(bx-ax);
    double cy=optParams[1];//by-gn*(by-ay);
    double dx=ax+gn*(bx-ax);
    double dy=optParams[1];//ay+gn*(by-ay);
    double fc,fd;
    bool exit=false;
    int nbIter=0;
    double bestvalold=itk::NumericTraits<double>::max(),bestval=optMetric;
    double diff=itk::NumericTraits<double>::max();
    
    //init
    try
	{
        paramsgn[0]=cx;
        paramsgn[1]=cy;
        fc=m_Metric->GetValue(paramsgn);
            
        paramsgn[0]=dx;
        paramsgn[1]=dy;
        fd=m_Metric->GetValue(paramsgn);
        
        if(!m_Minimize)
            {
                fc = -fc;
                fd = -fd;
            }
    }
	catch(itk::ExceptionObject& err)
	{
			exit=true;
			itkWarningMacro(<<err.GetDescription());
	}
        
    
    while ((diff>m_ConvergenceAccuracy) && (!exit) && (nbIter<=m_MaxIter)) 
    {
        nbIter++;
    
		try
		{
		    // x direction
			if (fc<fd)
			{
				if (bestval>fc)
				   {
                     bestval=fc;
                     optParams[0]=cx;
                     optParams[1]=cy;
                   }
				
				bx=dx;
				
				dx=cx;
				dy=ay+gn*(by-ay);
				
				cy=by-gn*(by-ay);
				
				paramsgn[0]=dx;
	            paramsgn[1]=dy;
	            fd=m_Metric->GetValue(paramsgn);
				
				paramsgn[0]=cx;
	            paramsgn[1]=cy;
	            fc=m_Metric->GetValue(paramsgn);
				
			}
			else
			{
                if (bestval>fd)
				   {
                     bestval=fd;
                     optParams[0]=dx;
                     optParams[1]=dy;
                   }
				
				ax=cx;
				
				cx=dx;
				cy=by-gn*(by-ay);
				
				dy=ay+gn*(by-ay);
				
				paramsgn[0]=cx;
	            paramsgn[1]=cy;
	            fc=m_Metric->GetValue(paramsgn);
				
				paramsgn[0]=dx;
	            paramsgn[1]=dy;
	            fd=m_Metric->GetValue(paramsgn);
			}
			
			if(!m_Minimize)
            {
				fc = -fc;
				fd = -fd;
			}
            
	        // y direction
			if (fc<fd)
			{
                if (bestval>fc)
				   {
                     bestval=fc;
                     optParams[0]=cx;
                     optParams[1]=cy;
                   }
				
				by=dy;
				
				dx=ax+gn*(bx-ax);
				dy=cy;
				
				cx=bx-gn*(bx-ax);
				
				paramsgn[0]=dx;
	            paramsgn[1]=dy;
	            fd=m_Metric->GetValue(paramsgn);
				
				paramsgn[0]=cx;
	            paramsgn[1]=cy;
	            fc=m_Metric->GetValue(paramsgn);
				
			}
			else
			{
				if (bestval>fd)
				   {
                     bestval=fd;
                     optParams[0]=dx;
                     optParams[1]=dy;
                   }
				
				ay=cy;
				
				cx=bx-gn*(bx-ax);
				cy=dy;
				
				dx=ax+gn*(bx-ax);

				paramsgn[0]=cx;
	            paramsgn[1]=cy;
	            fc=m_Metric->GetValue(paramsgn);
				
				paramsgn[0]=dx;
	            paramsgn[1]=dy;
	            fd=m_Metric->GetValue(paramsgn);
			}
			
			if(!m_Minimize)
			{
				fc = -fc;
				fd = -fd;
			}
			
            // Before eventual exit, take benefit from the last evaluations of fd and fc
            if (bestval>fd)
				   {
                     bestval=fd;
                     optParams[0]=dx;
                     optParams[1]=dy;
                   }
            if (bestval>fc)
				   {
                     bestval=fc;
                     optParams[0]=cx;
                     optParams[1]=cy;
                   }
			
			if(!m_Minimize)
                bestval=-bestval;
    
		}
		catch(itk::ExceptionObject& err)
		{
			exit=true;
			itkWarningMacro(<<err.GetDescription());
		}
        
        diff= fabs(bestval-bestvalold);
        bestvalold=bestval;
    
	}

    // Store the offset and the correlation value
    outputIt.Set(optMetric);
    if(m_UseSpacing)
      {
      displacementValue[0] = optParams[0];
      displacementValue[1] = optParams[1];
      }
    else
      {
      displacementValue[0] = optParams[0]/fixedSpacing[0];
      displacementValue[1] = optParams[1]/fixedSpacing[1];
      }
    outputDfIt.Set(displacementValue);
    
    
    // Update iterators
    ++outputIt;
    ++outputDfIt;

    // Update progress
    progress.CompletedPixel();
    }
 }
} // end namespace otb

#endif
