/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $otbMLCToCoherencyImageFilter.txx$
  Language:  C++
  Date:      $Date: 24/10/2006 $
  Version:   $Version: 1.0 $

=========================================================================*/
#ifndef __MLCToCoherencyImageFilter_txx
#define __MLCToCoherencyImageFilter_txx

#include "otbMLCToCoherencyImageFilter.h"

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
  MLCToCoherencyImageFilter<TPixel>::MLCToCoherencyImageFilter()
  {
    // !! TInputImage should be StokesType and TOutputImage should be MLCType (otherwise fail)
  
  }

  template <class TPixel>
  void 
  MLCToCoherencyImageFilter<TPixel>
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
  MLCToCoherencyImageFilter< TPixel>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
			 int threadId)
  {
    float C1, C2, C3, C4, C5, C6, C7, C8, C9;
    float T1, T2, T3, T4, T5, T6, T7, T8, T9;
    itk::ImageRegionConstIterator<InputImageType> inIt (this->GetInput(),outputRegionForThread);
    itk::ImageRegionIterator<OutputImageType> outIt (this->GetOutput(),outputRegionForThread);
    MLCType vectorValue;
    
    std::cout << "MLCToCoherencyImageFilter -> Entrée dans ThreadedGenerateData()" << std::endl;
    
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

      

      vectorValue[0]= 0.5*(C1+C3+2*C6);   // <(hh+vv)(hh+vv)*>
      vectorValue[1]=0.5*(C1+C3-2*C6);    // <(hh-vv)(hh-vv)*>
      vectorValue[2]=2.*C2;               // <4hv.hv*>
      vectorValue[3]=0.5*(C1-C3);         // Re<(hh+vv)(hh-vv)*>
      vectorValue[4]=-C7;                 // Im<(hh+vv)(hh-vv)*>
      vectorValue[5]=C4+C8;               // Re<(hh+vv)(2hv)*>
      vectorValue[6]=C5-C9;               // Im<(hh+vv)(2hv)*>
      vectorValue[7]=C4-C8;               // Re<(hh-vv)(2hv)*>
      vectorValue[8]=C5+C9;               // Im<(hh-vv)(2hv)*>

      
      outIt.Set(vectorValue);

    
      numPix++;
    }
  }
  

  /**
   * Standard "PrintSelf" method
   */
  template <class TPixel>
  void
  MLCToCoherencyImageFilter<TPixel>
  ::PrintSelf(
	      std::ostream& os, 
	      itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
    
    
  }
  
} // end namespace otb

#endif
