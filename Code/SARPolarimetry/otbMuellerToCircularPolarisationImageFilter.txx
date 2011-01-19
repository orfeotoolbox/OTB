/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $otbMuellerToCircularPolarisationImageFilter.txx$
  Language:  C++
  Date:      $Date: 24/10/2006 $
  Version:   $Version: 1.0 $

=========================================================================*/
#ifndef __MuellerToCircularPolarisationImageFilter_txx
#define __MuellerToCircularPolarisationImageFilter_txx

#include "otbMuellerToCircularPolarisationImageFilter.h"

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
  MuellerToCircularPolarisationImageFilter<TPixel>::MuellerToCircularPolarisationImageFilter()
  {
    // !! TInputImage should be MuellerType and TOutputImage should be CircularType (otherwise fail)
  
  }

  template <class TPixel>
  void 
  MuellerToCircularPolarisationImageFilter<TPixel>
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
  MuellerToCircularPolarisationImageFilter< TPixel>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
			 int threadId)
  {

    

  float M1, M2, M3, M4, M5, M6, M7, M8, M9, M10;
    
  itk::ImageRegionConstIterator<InputImageType> inIt (this->GetInput(),outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType> outIt (this->GetOutput(),outputRegionForThread);
    CircularType vectorValue;
    
    
    int numPix =0;
    
    for ( inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, ++outIt)
    {
      M1 = inIt.Get()[0];
      M2 = inIt.Get()[1];
      M3 = inIt.Get()[2];
      M4 = inIt.Get()[3];
      M5 = inIt.Get()[4];
      M6 = inIt.Get()[5];
      M7 = inIt.Get()[6];
      M8 = inIt.Get()[7];
      M9 = inIt.Get()[8];
      M10= inIt.Get()[9];
      

   //  if (numPix>(128*128/2-128)) {std::cout << M1 << M2 << M3 << M4 << M5 << M6 << M7 << M8 << M9 << M10 <<  std::endl;}

      vectorValue[0]= M1+2*M7+M4;   // LL
      vectorValue[1]= M1-2*M7+M4;   // RR
      vectorValue[2]= M1-M4;        // LR
      

      
      outIt.Set(vectorValue);

    
      numPix++;
    }
  }
  

  /**
   * Standard "PrintSelf" method
   */
  template <class TPixel>
  void
  MuellerToCircularPolarisationImageFilter<TPixel>
  ::PrintSelf(
	      std::ostream& os, 
	      itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
    
   }
  
} // end namespace otb

#endif
