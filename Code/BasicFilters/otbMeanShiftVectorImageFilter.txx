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
#ifndef _otbMeanShiftVectorImageFilter_txx
#define _otbMeanShiftVectorImageFilter_txx

#include "otbMeanShiftVectorImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

#include "otbMacro.h"

namespace otb
{
  template <class TInputImage,class TOutputImage>
  MeanShiftVectorImageFilter<TInputImage,TOutputImage>
  ::MeanShiftVectorImageFilter()
  {
    m_MaxNumberOfIterations = 20;
    m_ConvergenceDistanceThreshold = 0.5;
    m_SpatialRadius      = 3;
    m_RangeRadius        = 10;
    m_UseImageSpacing    = false;
    m_InternalRadius.Fill(0);
  }


  template <class TInputImage,class TOutputImage>
  void
  MeanShiftVectorImageFilter<TInputImage,TOutputImage>
  ::GenerateInputRequestedRegion()
  {
    // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();
    
    // get pointers to the input and output
    typename Superclass::InputImagePointer inputPtr = 
      const_cast< TInputImage * >( this->GetInput() );
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
    
    if ( !inputPtr || !outputPtr )
      {
	return;
      }
    
    // get a copy of the input requested region (should equal the output
    // requested region)
    typename TInputImage::RegionType inputRequestedRegion;
    inputRequestedRegion = inputPtr->GetRequestedRegion();
    
    //Compute the radius we will need to execute the filter:
    if(m_UseImageSpacing)
      {
	for(unsigned int dimension = 0; dimension < InputImageType::ImageDimension;++dimension)
	  {
	    m_InternalRadius[dimension] = static_cast<unsigned long>(vcl_floor(m_MaxNumberOfIterations * m_SpatialRadius / inputPtr->GetSpacing()[0]+0.5));
	  }
      }
    else
      {
	m_InternalRadius.Fill(static_cast<unsigned long>(vcl_floor(m_MaxNumberOfIterations * m_SpatialRadius+0.5)));
      }
    std::cout<<"MeanShiftVectorImageFilter: Internal radius = "<<m_InternalRadius<<std::endl;


    // pad the input requested region by the operator radius
    inputRequestedRegion.PadByRadius( m_InternalRadius );
    
    // crop the input requested region at the input's largest possible region
    if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
      {
	inputPtr->SetRequestedRegion( inputRequestedRegion );
	return;
      }
    else
      {
	// Couldn't crop the region (requested region is outside the largest
	// possible region).  Throw an exception.

	// store what we tried to request (prior to trying to crop)
	inputPtr->SetRequestedRegion( inputRequestedRegion );
	
	// build an exception
	itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
	e.SetLocation(ITK_LOCATION);
	e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
	e.SetDataObject(inputPtr);
	throw e;
      }
  }
  
  template <class TInputImage,class TOutputImage>
  void
  MeanShiftVectorImageFilter<TInputImage,TOutputImage>
  ::ThreadedGenerateData(const RegionType & outputRegionForThread,int threadId)
  {
    std::cout<<"Call to threaded generate data, threadId: "<<threadId<<", region: "<<outputRegionForThread<<std::endl;
    // TODO: update this to handle the UseImageSpacing option.

    // Set up the progress reporter
    itk::ProgressReporter progress(this,threadId,outputRegionForThread.GetNumberOfPixels());

    // Input and output pointers
    typename OutputImageType::Pointer outputPtr = this->GetOutput();
    typename  InputImageType::ConstPointer inputPtr  = this->GetInput();

    // Iterators
    itk::ConstNeighborhoodIterator<InputImageType> inputIt(m_InternalRadius,inputPtr,outputRegionForThread);
    itk::ConstNeighborhoodIterator<InputImageType> internalIt;
    itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr,outputRegionForThread);
   
    // Boundary conditions
    itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;
    inputIt.OverrideBoundaryCondition(&nbc);

    //TODO: Work with a floating point pixel type

    // local declarations 
    unsigned int nbIterations;
    typedef typename itk::ConstNeighborhoodIterator<InputImageType>::OffsetType OffsetType;

    OffsetType     offset;
    PointType      maxDensityPoint,convergencePoint;
    InputPixelType maxDensityValue, convergenceValue,diff,current;
    unsigned int   nbPixelsIntoAccount;

    unsigned int nbComponentsPerPixel = inputPtr->GetNumberOfComponentsPerPixel();
    bool goesOn = true;
    long startx,stopx,starty,stopy;
    
    // intialize iterators 
    inputIt.GoToBegin();
    outputIt.GoToBegin();
    
    // Walk the images 
    while(!inputIt.IsAtEnd() && !outputIt.IsAtEnd())
      {
	nbIterations = 0;
	goesOn = true;
	convergencePoint.Fill(0);
	convergenceValue = inputIt.GetCenterPixel();
	//std::cout<<"Processing pixel: "<<inputIt.GetIndex()<<", value: "<<convergenceValue<<std::endl;

	
	// While the max number of iterations has not been reached and the convergence index is still moving
	while(nbIterations < m_MaxNumberOfIterations && goesOn)
	  {
	    //std::cout<<"Loop: "<<nbIterations<<", goesOn: "<<goesOn<<std::endl;
	    maxDensityPoint.Fill(0);
	    maxDensityValue.SetSize(nbComponentsPerPixel);
	    maxDensityValue.Fill(0);
	    nbPixelsIntoAccount = 0;

	    startx = vcl_floor(convergencePoint[0]-m_SpatialRadius);
	    stopx = vcl_ceil(convergencePoint[0]+m_SpatialRadius);
	    starty = vcl_floor(convergencePoint[1]-m_SpatialRadius);
	    stopy = vcl_ceil(convergencePoint[1]+m_SpatialRadius);

	    // loop on the neighborhood
	    for(long i = startx;i<=stopx;++i)
	      {
		for(long j =starty;j<=stopy;++j)
		  {
		    // Setting the offset
		    offset[0]=i;
		    offset[1]=j;

		    // verify that the current offset is inside or spatial circle
		    if(vcl_sqrt(vcl_pow(static_cast<double>(convergencePoint[0]-offset[0]),2)+vcl_pow(static_cast<double>(convergencePoint[1]-offset[1]),2))<m_SpatialRadius)
		      {
			//std::cout<<"Point: "<<offset<<" is inside spatial circle"<<std::endl;
			current = inputIt.GetPixel(offset);
			diff    = convergenceValue - current;
			//std::cout<<"Difference with current pixel: "<<diff<<std::endl;
			// Check if we are inside the spectral - spatial circle
			if(diff.GetSquaredNorm()+vcl_pow(static_cast<double>(offset[0]),2)+vcl_pow(static_cast<double>(offset[1]),2)<nbComponentsPerPixel * vcl_pow(m_RangeRadius,2) + vcl_pow(m_SpatialRadius,2))
			  {
			    //std::cout<<"Point: "<<offset<<" is inside the spatial/spectral disk"<<std::endl;
			    maxDensityPoint[0]+=offset[0];
			    maxDensityPoint[1]+=offset[1];
			    maxDensityValue+=current;
			    ++nbPixelsIntoAccount;
			  }			
		      }
		  }
	      }
	    // If no inside pixels
	    if(nbPixelsIntoAccount == 0)
	      {
		maxDensityPoint.Fill(0);
		maxDensityValue = inputIt.GetCenterPixel();
	      }
	    else
	      {
		maxDensityPoint[0]/=nbPixelsIntoAccount;
		maxDensityPoint[1]/=nbPixelsIntoAccount;
		maxDensityValue/=nbPixelsIntoAccount;
	      }
	    //std::cout<<"MaxDensityValue: "<<maxDensityValue<<std::endl;
	    convergenceValue = maxDensityValue;

	    //std::cout<<"Nb pixels into account: "<<nbPixelsIntoAccount<<", convergence point: "<<convergencePoint<<", maxDensityPoint: "<<maxDensityPoint<<", convergenceValue: "<<convergenceValue<<std::endl;
	    
	    // Check if we are still significantly moving
	    double updatedDistance = vcl_sqrt(vcl_pow(static_cast<double>(maxDensityPoint[0]-convergencePoint[0]),2)+vcl_pow(static_cast<double>(maxDensityPoint[1]-convergencePoint[1]),2));
	    if(updatedDistance < m_ConvergenceDistanceThreshold)
	      {
		goesOn = false;
	      }
	    else
	      {
		convergencePoint = maxDensityPoint;
	      }
	    ++nbIterations;
	  }
	// Set the output value
	// TODO: missing value cast here
	outputIt.Set(convergenceValue);
	// Update progress
	progress.CompletedPixel();
	// Increment iterators
	++inputIt;
	++outputIt;
      }
  }

  template <class TInputImage,class TOutputImage>
  void
  MeanShiftVectorImageFilter<TInputImage,TOutputImage>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os<<indent<<"Spatial radius: "<<m_SpatialRadius<<std::endl;
    os<<indent<<"Range radius: "<<m_RangeRadius<<std::endl;
    os<<indent<<"Maximum number of iterations: "<<m_MaxNumberOfIterations<<std::endl;
    os<<indent<<"Internal radius: "<<m_InternalRadius<<std::endl;
    os<<indent<<"Use image spacing: "<<m_UseImageSpacing<<std::endl;
    os<<indent<<"Convergence distance threshold: "<<m_ConvergenceDistanceThreshold<<std::endl;

  }

} // end namespace otb

#endif
