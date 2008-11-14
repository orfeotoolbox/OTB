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
#ifndef __otbMeanShiftImageFilterBase_txx
#define __otbMeanShiftImageFilterBase_txx

#include "otbMeanShiftImageFilterBase.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

#include "otbMacro.h"

namespace otb
{
  template <class TInputImage,class TOutputImage, class TClusterImage,class TPrecision>
  MeanShiftImageFilterBase<TInputImage,TOutputImage,TClusterImage, TPrecision>
  ::MeanShiftImageFilterBase()
  {
    this->SetNumberOfOutputs(2);
    this->SetNumberOfRequiredOutputs(1);

    //this->SetNthOutput(1,OutputImageType::New());
    this->SetNthOutput(1,ClusterImageType::New());

    m_MaxNumberOfIterations = 20;
    m_ConvergenceDistanceThreshold = 0.5;
    m_SpatialRadius      = 3;
    m_RangeRadius        = 10;
    m_UseImageSpacing    = false;
    m_InternalRadius.Fill(0);
    m_InternalSpacing.Fill(1.);
    m_Label.clear();
    
    // For debug purposes we set the number of threads to 1.
    // this->SetNumberOfThreads(1);
  }

  template <class TInputImage,class TOutputImage, class TClusterImage,class TPrecision>
  const typename MeanShiftImageFilterBase<TInputImage, TOutputImage, TClusterImage, TPrecision>::ClusterImageType * 
  MeanShiftImageFilterBase<TInputImage,TOutputImage,TClusterImage, TPrecision>
  ::GetClusterImage()const
  {
    if (this->GetNumberOfOutputs() < 2)
      {
    	return 0;

      }
    return static_cast<const ClusterImageType * >(this->itk::ProcessObject::GetOutput(1) );
  }
  
  /** Return the output image direction */  
  template <class TInputImage,class TOutputImage, class TClusterImage,class TPrecision>
  typename MeanShiftImageFilterBase<TInputImage, TOutputImage, TClusterImage, TPrecision>::ClusterImageType * 
  MeanShiftImageFilterBase<TInputImage,TOutputImage,TClusterImage, TPrecision>
  ::GetClusterImage()
  {
    if (this->GetNumberOfOutputs() < 2)
      {
    	return 0;
      }
    return static_cast<ClusterImageType * >
      (this->itk::ProcessObject::GetOutput(1) );
  }


  template <class TInputImage,class TOutputImage,class TClusterImage, class TPrecision>
  void
  MeanShiftImageFilterBase<TInputImage,TOutputImage,TClusterImage, TPrecision>
  ::AllocateOutputs()
  {
    // Filter Output
    OutputImagePointerType outputPtr = this->GetOutput(0);
    outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
    outputPtr->Allocate();

    // Cluster Output
    ClusterImagePointerType clusterPtr = this->GetClusterImage();
    clusterPtr->SetBufferedRegion( clusterPtr->GetRequestedRegion() );
    clusterPtr->Allocate();
  }

 template <class TInputImage,class TOutputImage,class TClusterImage, class TPrecision>
  void
  MeanShiftImageFilterBase<TInputImage,TOutputImage,TClusterImage, TPrecision>
  ::BeforeThreadedGenerateData()
  {
    LabelPerThreadType labelThread(1, 0);
    LabelVectorType m_label(this->GetNumberOfThreads(), labelThread);
  }


  template <class TInputImage,class TOutputImage,class TClusterImage, class TPrecision>
  void
  MeanShiftImageFilterBase<TInputImage,TOutputImage,TClusterImage, TPrecision>
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
	    m_InternalSpacing[dimension] = vcl_abs(inputPtr->GetSpacing()[dimension]);
	  }
      }
    otbMsgDevMacro(<<"MeanShiftImageFilterBase: Internal Spacing = "<<m_InternalSpacing);

    for(unsigned int dimension = 0; dimension < InputImageType::ImageDimension;++dimension)
      {
	m_InternalRadius[dimension] = static_cast<unsigned long>(vcl_floor(m_MaxNumberOfIterations * m_SpatialRadius 
									   / m_InternalSpacing[dimension]+0.5));
      }
      
    otbMsgDevMacro(<<"MeanShiftImageFilterBase: Internal radius = "<<m_InternalRadius);

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
  
  template <class TInputImage,class TOutputImage,class TClusterImage, class TPrecision>
  void
  MeanShiftImageFilterBase<TInputImage,TOutputImage,TClusterImage,TPrecision>
  ::ThreadedGenerateData(const RegionType & outputRegionForThread,int threadId)
  {
    // Set up the progress reporter
    itk::ProgressReporter progress(this,threadId,outputRegionForThread.GetNumberOfPixels());

    // Input and output pointers
    typename  InputImageType::ConstPointer inputPtr  = this->GetInput();
    typename OutputImageType::Pointer outputPtr = this->GetOutput();
    typename ClusterImageType::Pointer clusterPtr = this->GetClusterImage();
    

    // Iterators
    itk::ImageRegionConstIteratorWithIndex<InputImageType> inputIt(inputPtr,outputRegionForThread);
    itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr,outputRegionForThread);
    itk::ImageRegionIterator<ClusterImageType> clusterIt(clusterPtr,outputRegionForThread);

    // local declarations 
    unsigned int nbIterations;
    IndexType      offset;
    PointType      maxDensityPoint,convergencePoint;
    PrecisionPixelType maxDensityValue, convergenceValue,diff,current;
    unsigned int   nbPixelsIntoAccount;
    bool goesOn = true;
    long startx,stopx,starty,stopy,i,j;
    double squaredSpatialDistance, squaredSpectralDistance, squaredUpdateDistance;

    // Const values computed here to reduce complexity
    const unsigned int nbComponentsPerPixel = this->GetNumberOfComponentsPerPixel();
    const double spatialThreshold = vcl_pow(m_SpatialRadius,2);
    const double spectralThreshold = vcl_pow(m_RangeRadius,2);
    const double spatialAndRangeThreshold = nbComponentsPerPixel * vcl_pow(m_RangeRadius,2) + spatialThreshold;
    const double spatialConvergenceThreshold = vcl_pow(m_ConvergenceDistanceThreshold,2);

    typedef MeanShiftCluster<PrecisionPixelType,PointType> ClusterType;
    typedef std::vector<ClusterType> ClusterVectorType;

    ClusterVectorType clusterVector;
    
    // intialize iterators 
    inputIt.GoToBegin();
    outputIt.GoToBegin();
    clusterIt.GoToBegin();

    // Walk the images 
    while(!inputIt.IsAtEnd() && !outputIt.IsAtEnd() && !clusterIt.IsAtEnd())
      {
	nbIterations = 0;
	goesOn = true;
	convergencePoint[0]=static_cast<double>(inputIt.GetIndex()[0])*m_InternalSpacing[0];
	convergencePoint[1]=static_cast<double>(inputIt.GetIndex()[1])*m_InternalSpacing[1];
	convergenceValue = this->CastInputPixelToPrecisionPixel(inputIt.Get());
	// While the max number of iterations has not been reached and the convergence index is still moving
	while(nbIterations < m_MaxNumberOfIterations && goesOn)
	  {
	    maxDensityPoint.Fill(0);
	    this->InitValue(maxDensityValue,nbComponentsPerPixel);
	    nbPixelsIntoAccount = 0;

	    startx = static_cast<long>(vcl_floor((convergencePoint[0]-m_SpatialRadius)/m_InternalSpacing[0]));
	    stopx  = static_cast<long>(vcl_ceil ((convergencePoint[0]+m_SpatialRadius)/m_InternalSpacing[0]));
	    starty = static_cast<long>(vcl_floor((convergencePoint[1]-m_SpatialRadius)/m_InternalSpacing[1]));
	    stopy  = static_cast<long>(vcl_ceil ((convergencePoint[1]+m_SpatialRadius)/m_InternalSpacing[1]));

	    // loop on the neighborhood
	    for(i = startx;i<=stopx;++i)
	      {
		for(j =starty;j<=stopy;++j)
		  {
		    // Setting the offset
		    offset[0]=i;
		    offset[1]=j;
		    squaredSpatialDistance = vcl_pow(static_cast<double>(convergencePoint[0]-offset[0]*m_InternalSpacing[0]),2)
		      + vcl_pow(static_cast<double>(convergencePoint[1]-offset[1]*m_InternalSpacing[1]),2);
		    // verify that the current offset is inside or spatial circle
		    if(inputPtr->GetBufferedRegion().IsInside(offset) 
		       && squaredSpatialDistance <spatialThreshold)
		      {
			current = inputPtr->GetPixel(offset);
			diff    = convergenceValue - current;;
			// Check if we are inside the spectral - spatial circle
			if(this->SquaredNorm(diff)+squaredSpatialDistance<spatialAndRangeThreshold)
			  {
			    maxDensityPoint[0]+=offset[0]*m_InternalSpacing[0];
			    maxDensityPoint[1]+=offset[1]*m_InternalSpacing[1];
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
		maxDensityValue = inputIt.Get();
	      }
	    else
	      {
		// Mean density location and value
		maxDensityPoint[0]/=nbPixelsIntoAccount;
		maxDensityPoint[1]/=nbPixelsIntoAccount;
		maxDensityValue/=nbPixelsIntoAccount;
	      }   
	    convergenceValue = maxDensityValue;
	    // Check if we are still significantly moving
	    squaredUpdateDistance = vcl_pow(static_cast<double>(maxDensityPoint[0]-convergencePoint[0]),2)
	    +vcl_pow(static_cast<double>(maxDensityPoint[1]-convergencePoint[1]),2);
	    if(squaredUpdateDistance <spatialConvergenceThreshold)
	      {
		// we reached the maximum density point, exit
		goesOn = false;
	      }
	    else
	      {
		// Update the convergence point and loop again
		convergencePoint = maxDensityPoint;
	    
	      }
	    ++nbIterations;
	  }
	// Set the output value
	outputIt.Set(this->CastPrecisionPixelToOutputPixel(convergenceValue));

	bool clusterFound = false;

	// Look for an already existing cluster
	ClusterPixelType label = 0;
	while(label<clusterVector.size() && !clusterFound)
	  {
	    squaredSpatialDistance = vcl_pow(clusterVector[label].m_SpatialCenter[0]-convergencePoint[0],2)
	      +vcl_pow(clusterVector[label].m_SpatialCenter[1]-convergencePoint[1],2);
	    squaredSpectralDistance = this->SquaredNorm(clusterVector[label].m_SpectralCenter-convergenceValue);

	    if(squaredSpatialDistance<spatialThreshold
	       && squaredSpectralDistance<spectralThreshold)
	      {
		clusterFound = true;
		clusterVector[label].AddPixel(convergencePoint,convergenceValue);
	      }
	    else
	      {
		++label;
	      }
	  }

	// if we found a good cluster, labelize pixel
	if(clusterFound)
	  {
	    clusterIt.Set(label+1);
	  }
	// else create a new cluster for that pixel and labelize it
	else
	  {
	    clusterVector.push_back(ClusterType(convergencePoint,convergenceValue));
	    clusterIt.Set(static_cast<ClusterPixelType>(clusterVector.size()));
	  }
	// Update progress
	progress.CompletedPixel();
	// Increment iterators
	++inputIt;
	++outputIt;
	++clusterIt;
      }
  }

  template <class TInputImage,class TOutputImage,class TClusterImage, class TPrecision>
  void
  MeanShiftImageFilterBase<TInputImage,TOutputImage,TClusterImage, TPrecision>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os<<indent<<"Spatial radius: "                <<m_SpatialRadius               <<std::endl;
    os<<indent<<"Range radius: "                  <<m_RangeRadius                 <<std::endl;
    os<<indent<<"Maximum number of iterations: "  <<m_MaxNumberOfIterations       <<std::endl;
    os<<indent<<"Internal radius: "               <<m_InternalRadius              <<std::endl;
    os<<indent<<"Use image spacing: "             <<m_UseImageSpacing             <<std::endl;
    os<<indent<<"Internal spacing: "              <<m_InternalSpacing             <<std::endl;
    os<<indent<<"Convergence distance threshold: "<<m_ConvergenceDistanceThreshold<<std::endl;

  }

} // end namespace otb

#endif
