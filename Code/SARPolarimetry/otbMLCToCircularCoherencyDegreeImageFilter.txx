/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $otbMLCToCircularCoherencyDegreeImageFilter.txx$
  Language:  C++
  Date:      $Date: 24/10/2006 $
  Version:   $Version: 1.0 $

=========================================================================*/
#ifndef __MLCToCircularCoherencyDegreeImageFilter_txx
#define __MLCToCircularCoherencyDegreeImageFilter_txx

#include "otbMLCToCircularCoherencyDegreeImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

namespace otb
{

  template <class TPixel>
  MLCToCircularCoherencyDegreeImageFilter<TPixel>::MLCToCircularCoherencyDegreeImageFilter()
  {
    // !! TInputImage should be MLCType and TOutputImage should be CircularCoherencyDegreeType (otherwise fail)
  
  }

  template <class TPixel>
  void 
  MLCToCircularCoherencyDegreeImageFilter<TPixel>
  ::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
  {
    // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();
    
    // get pointers to the input and output
    typename Superclass::InputImagePointer inputPtr = 
      const_cast< InputImageType * >( this->GetInput() );
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
    
    if ( !inputPtr || !outputPtr )
      {
	return;
      }
    
    // get a copy of the input requested region (should equal the output
    // requested region)
    typename InputImageType::RegionType inputRequestedRegion;
    inputRequestedRegion = inputPtr->GetRequestedRegion();
    
   
    
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


  template< class TPixel>
  void
  MLCToCircularCoherencyDegreeImageFilter< TPixel>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
			 int threadId)
  {
    float C1, C2, C3, C4, C5, C6, C7, C8, C9;
    float llrrReel, llrrIm, ll2, rr2,lr2, lllrReel, lllrIm, rrlrReel, rrlrIm;
    
    itk::ImageRegionConstIterator<InputImageType> inIt (this->GetInput(),outputRegionForThread);
    itk::ImageRegionIterator<OutputImageType> outIt (this->GetOutput(),outputRegionForThread);
    CircularCoherencyDegreeType value;
    
    std::cout << "MLCToCircularCoherencyDegreeImageFilter -> Entrée dans ThreadedGenerateData()" << std::endl;
    
    int numPix =0;
    
    for ( inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, ++outIt)
    {
      C1 = inIt.Get()[0];
      C2 = inIt.Get()[1];
      C3 = inIt.Get()[2];
      C4 = inIt.Get()[3];
      C5 = inIt.Get()[4];
      C6 = inIt.Get()[5];
      C7 = inIt.Get()[6];
      C8 = inIt.Get()[7];
      C9 = inIt.Get()[8];

      llrrReel = 1/4*(C1+C3-4*C2-2*C6);
      llrrIm   = (-C4-C8);

      lllrReel = 1/4*(C1-C3-2*C5+2*C9);
      lllrIm = -1/2*(C7+C4+C8);

      rrlrReel = 1/4*(C1-C3+2*C5-2*C9);
      rrlrIm = 1/2*(C4+C8-C7);   

      ll2    = 1/4*( C1+C3+4*C2-2*C6-4*C5-4*C9);
      rr2    = 1/4*( C1+C3+4*C2-2*C6+4*C5+4*C9);
      lr2    = 1/4*( C1+C3+2*C6);
      
      if (ll2 <= 0.01) 
	{
          ll2=0.01;
	}

if (rr2 <= 0.01) 
	{
          rr2=0.01;
	}

if (lr2 <= 0.01) 
	{
          lr2=0.01;
	}

      value[0]= sqrt( llrrReel*llrrReel + llrrIm*llrrIm ) / sqrt (ll2*rr2); // |<ll.rr*>|/sqrt(<ll.ll*><rr.rr*>)

      value[1]= sqrt( lllrReel*lllrReel + lllrIm*lllrIm ) / sqrt (ll2*lr2); // |<ll.lr*>|/sqrt(<ll.ll*><lr.lr*>)

      value[2]= sqrt( rrlrReel*rrlrReel + rrlrIm*rrlrIm ) / sqrt (rr2*lr2); // |<rr.lr*>|/sqrt(<rr.rr*><lr.lr*>)
   
      outIt.Set(value);

    
      numPix++;
    }
  }
  

  /**
   * Standard "PrintSelf" method
   */
  template <class TPixel>
  void
  MLCToCircularCoherencyDegreeImageFilter<TPixel>
  ::PrintSelf(
	      std::ostream& os, 
	      itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
    
    
  }
  
} // end namespace otb

#endif
