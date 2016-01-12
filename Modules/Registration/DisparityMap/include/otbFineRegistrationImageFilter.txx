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
  m_SubPixelAccuracy = 0.1;

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
  typename TranslationType::ParametersType params(2), optParams(2), tmpOptParams(2);

  // Final displacement value
  DisplacementValueType displacementValue;
  displacementValue[0] = m_InitialOffset[0];
  displacementValue[1] = m_InitialOffset[1];

  // Local initial offset: enable the possibility of a different initial offset for each pixel
  SpacingType localOffset = m_InitialOffset;

  // Get fixed image spacing
  SpacingType fixedSpacing = fixedPtr->GetSpacing();

    //totnbcall
    int totnbcall1=0;
    int totnbcall2=0;
    double nbpixels=0;
    int nbfails=0;
    int nbbigfails=0;
    int nbbigwins=0;


  // Walk the images
  while (!outputIt.IsAtEnd() && !outputDfIt.IsAtEnd() )
    {
		std::cout << "------------------" << std::endl;
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

    //nbcall
    int nbcall1=0;
    int nbcall2=0;

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
          nbcall1++;
          nbcall2++;
          //std::cout << currentMetric << std::endl;

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
//std::cout << "optMetric = " << optMetric << std::endl;
    // Dichotomic sub-pixel
    SpacingType subPixelSpacing = fixedSpacing;
    SpacingType optsubPixelSpacing;
    
    double optMetricold=itk::NumericTraits<double>::max();
    double diff1=itk::NumericTraits<double>::max();
    int nbiter1=0,maxnbiter1=5;
    
    //while(subPixelSpacing[0] > m_SubPixelAccuracy || subPixelSpacing[1] > m_SubPixelAccuracy)
    while  ((diff1>0.001) )//&& (nbiter1<maxnbiter1))
      {
      // Perform 1 step of dichotomic search
      subPixelSpacing /= 2.;

      // Store last opt params
      tmpOptParams = optParams;

      for(int i = -1; i <= 1; i+=2)
        {
        for(int j = -1; j <= 1; j+=2)
          {
          params = tmpOptParams;
          params[0] += static_cast<double>(i*subPixelSpacing[0]);
          params[1] += static_cast<double>(j*subPixelSpacing[1]);

          nbcall1++;

          try
          {
            // compute currentMetric
            currentMetric = m_Metric->GetValue(params);
//std::cout << m_SubPixelAccuracy << " " << subPixelSpacing << " " << currentMetric << std::endl;
            // Check for maximum
            if((m_Minimize && (currentMetric < optMetric)) || (!m_Minimize && (currentMetric > optMetric)))
              {
				  optsubPixelSpacing =	  subPixelSpacing;
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
	   diff1= fabs(optMetric-optMetricold);
	   optMetricold=optMetric;
	   nbiter1++;
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
    
    
   /* //derivative
    double h=0.00001;
    double step=5.0;
    typename TranslationType::ParametersType paramsg1(2),paramsd1(2),paramsg2(2),paramsd2(2),init(2),temp(2);
    init[0]=tmpOptParams[0];
    init[1]=tmpOptParams[1];
    //init[0]=localOffset[0];
    //init[1]=localOffset[1];
    double res;
    nbcall2 ++;
    double bestval=m_Metric->GetValue(init);
    double bestvalold=itk::NumericTraits<double>::max();
    double diff=itk::NumericTraits<double>::max();
    int nbiter=0,maxnbiter=4;

	while  ((diff>0.001))// && (nbiter<maxnbiter))
    {
		  nbcall2 += 5;
		    
		  try
		  {
			  paramsg1[0] = init[0] - h;
			  paramsg1[1] = init[1];
			  
			  paramsg2[0] = init[0];
			  paramsg2[1] = init[1] - h;
			  
			  
			  double deriv1 = (bestval - m_Metric->GetValue(paramsg1))/(h);
			  double deriv2 = (bestval - m_Metric->GetValue(paramsg2))/(h);
			  
			  for(step=0.01;step<=1.0;step *= 10)
			  {
				  if (m_Minimize)
				  {
					temp[0] = init[0] - step*deriv1;
					temp[1] = init[1] - step*deriv2;
				  }
				  else
				  {
					temp[0] = init[0] + step*deriv1;
					temp[1] = init[1] + step*deriv2;
				  }
				  
				  //nbcall2++;
				  res=m_Metric->GetValue(temp);
				  
				  //std::cout << optMetric << " gradient " << step << " " << bestval << std::endl;
				  if ((m_Minimize && (res<bestval)) || (!m_Minimize && (res>bestval)) )
				  {
					  bestval=res;
					  optParams[0] = temp[0];
					  optParams[1] = temp[1];
				  }
			   }
			   init[0] = optParams[0];
			   init[1] = optParams[1];
			   
			   diff= fabs(bestval-bestvalold);
			   //std::cout << " diff " << diff << std::endl;
			   bestvalold=bestval;
		   
		  }
		  catch(itk::ExceptionObject& err)
				{
					diff=0;
					itkWarningMacro(<<err.GetDescription());
				}
		   
		   nbiter++;
		   
    }*/
    
    //golden search
    typename TranslationType::ParametersType paramsgn(2);
    double gn=(sqrt(5.0)-1.0)/2.0;
    /*double ax=-static_cast<double>(m_SearchRadius[0]);
    double bx=static_cast<double>(m_SearchRadius[0]);
    double ay=-static_cast<double>(m_SearchRadius[1]);
    double by=static_cast<double>(m_SearchRadius[1]);*/
    subPixelSpacing = fixedSpacing/2.0;
    double ax=tmpOptParams[0]-static_cast<double>(subPixelSpacing[0]);
    double ay=tmpOptParams[1]-static_cast<double>(subPixelSpacing[1]);
    double bx=tmpOptParams[0]+static_cast<double>(subPixelSpacing[0]);
    double by=tmpOptParams[1]+static_cast<double>(subPixelSpacing[1]);

    
    double bestval=m_Metric->GetValue(tmpOptParams); //Don't count this one : just for testing purpose

    double cx=bx-gn*(bx-ax);
    double cy=tmpOptParams[1];//by-gn*(by-ay);
    double dx=ax+gn*(bx-ax);
    double dy=tmpOptParams[1];//ay+gn*(by-ay);
    double fc,fd;
    
    nbcall2+=2;
    
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
    
    bool exit=false;
    double bestvalold=bestval;
    double diff=itk::NumericTraits<double>::max();
    
    //while(fabs(cx-dx) > m_SubPixelAccuracy || fabs(cy-dy) > m_SubPixelAccuracy)
    while ((diff>0.001) && (!exit)) //( ((fabs(cx-dx)>tol) || (fabs(cy-dy)>tol)) && (!exit) )
    {
		nbcall2 +=4;
		try
		{
		
			if (fc<fd)
			{
				if (bestval>fc)
				    bestval=fc;
				
				bx=dx;
				//by=dy;
				
				dx=cx;
				//dy=cy;
				dy=ay+gn*(by-ay);
				
				//cx=bx-gn*(bx-ax);
				cy=by-gn*(by-ay);
				
				//fd=fc;
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
				    bestval=fd;
				
				ax=cx;
				//ay=cy;
				
				cx=dx;
				//cy=dy;
				cy=by-gn*(by-ay);
				
				//dx=ax+gn*(bx-ax);
				dy=ay+gn*(by-ay);
				
				//fc=fd;
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
			if (bestval>fd)
				    bestval=fd;
			if (bestval>fc)
				    bestval=fc;
			
			if (fc<fd)
			{
				if (bestval>fc)
				    bestval=fc;
				
				//bx=dx;
				by=dy;
				
				//dx=cx;
				dx=ax+gn*(bx-ax);
				dy=cy;
				
				cx=bx-gn*(bx-ax);
				//cy=by-gn*(by-ay);
				
				//fd=fc;
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
				    bestval=fd;
				
				//ax=cx;
				ay=cy;
				
				//cx=dx;
				cx=bx-gn*(bx-ax);
				cy=dy;
				
				dx=ax+gn*(bx-ax);
				//dy=ay+gn*(by-ay);
				
				//fc=fd;
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
			
			if (bestval>fd)
				    bestval=fd;
			if (bestval>fc)
				    bestval=fc;
			
			if(!m_Minimize)
                bestval=-bestval;
			//std::cout << "diff = " << bx-ax<< std::endl;
			//paramsgn[0]=(cx+dx)/2.0;
	        //paramsgn[1]=(cy+dy)/2.0;
            //bestval=m_Metric->GetValue(paramsgn); //Don't count this one : just for testing purpose
    
		}
		catch(itk::ExceptionObject& err)
		{
			exit=true;
			itkWarningMacro(<<err.GetDescription());
		}
        
        diff= fabs(bestval-bestvalold);
        //std::cout <<"diff =" << diff << std::endl;
        bestvalold=bestval;
    
	}

    totnbcall1 += nbcall1;
    totnbcall2 += nbcall2;
    nbpixels++;
    
    if ( (m_Minimize && (optMetric<bestval)) || (!m_Minimize && (optMetric>bestval)) )
    {
		nbfails++;
		if (fabs(optMetric-bestval)>0.01)
		  nbbigfails++;
	}
	else
	{
		if (fabs(optMetric-bestval)>0.01)
		nbbigwins++;
	}
    
    std::cout << nbcall1 << " " << totnbcall1/nbpixels << "  " << optMetric << std::endl;
    std::cout << nbcall2 << " " << totnbcall2/nbpixels << "  " << bestval << std::endl;
    std::cout << totnbcall2/( (double) (totnbcall1))*100. << "%" << std::endl;
    std::cout << nbfails/nbpixels*100. << " " << nbbigfails/nbpixels*100. << "  " << 100-nbfails/nbpixels*100. << " " << nbbigwins/nbpixels*100. << "  " << nbpixels << std::endl;
    std::cout << "------------------" << std::endl;
    
    
    
    // Update iterators
    ++outputIt;
    ++outputDfIt;

    // Update progress
    progress.CompletedPixel();
    }
 }
} // end namespace otb

#endif
