/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $otbStokesToMCLImageFilter.txx$
  Language:  C++
  Date:      $Date: 24/10/2006 $
  Version:   $Version: 1.0 $

=========================================================================*/
#ifndef __StokesToMLCImageFilter_txx
#define __StokesToMLCImageFilter_txx

#include "otbStokesToMLCImageFilter.h"

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
  StokesToMLCImageFilter<TPixel>::StokesToMLCImageFilter()
  {
    // !! TInputImage should be StokesType and TOutputImage should be MLCType (otherwise fail)
  
  }

  template <class TPixel>
  void 
  StokesToMLCImageFilter<TPixel>
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
  StokesToMLCImageFilter< TPixel>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
			 int threadId)
  {
    float M11, M12, M13, M14, M22, M23, M24, M33, M34, M44;
    float C1, C2, C3, C4, C5, C6, C7, C8, C9;
    itk::ImageRegionConstIterator<InputImageType> inIt (this->GetInput(),outputRegionForThread);
    itk::ImageRegionIterator<OutputImageType> outIt (this->GetOutput(),outputRegionForThread);
    MLCType vectorValue;
    
    
    int numPix =0;
    
    for ( inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, ++outIt)
    {
      M11 = inIt.Get()[0];
      M12 = inIt.Get()[1];
      M13 = inIt.Get()[2];
      M14 = inIt.Get()[3];
      M23 = inIt.Get()[4];
      M24 = inIt.Get()[5];
      M33 = inIt.Get()[6];
      M34 = inIt.Get()[7];
      M44 = inIt.Get()[8];
      M22 = inIt.Get()[9];

      

      vectorValue[0]=M11+M22+2.*M12;  // C1  <hh.hh*>
      vectorValue[1]=M11-M22;  // C2  <hv.hv*>
      vectorValue[2]=M11+M22-2.*M12; // C3  <vv.vv*>
      vectorValue[3]=M13+M23;      // C4  Re<hh.hv*>
      vectorValue[4]=-1.*(M14+M24);   // C5   Im<hh.hv*>
      vectorValue[5]=M33-M44;  // C6  Re<hh.vv*>
      vectorValue[6]=-2.*M34;   // C7  Im<hh.vv*>
      vectorValue[7]=M13-M23;  // C8  Re<hv.vv>
      vectorValue[8] = -1.*(M14-M24);  // C9  Im<hv.vv*>

      
      outIt.Set(vectorValue);

      //if (numPix== (189*2317+920))
      //{
      //  std::cout << "Reg : " << outputRegionForThread.GetIndex()[0] << "  "<< outputRegionForThread.GetIndex()[1] <<std::endl;
      //  std::cout << numPix << "----"<< M11 << " "<< M12 << " "<< M13<< " "<< M14 << std::endl;
      //}

      numPix++;
    }
  }
  

  /**
   * Standard "PrintSelf" method
   */
  template <class TPixel>
  void
  StokesToMLCImageFilter<TPixel>
  ::PrintSelf(
	      std::ostream& os, 
	      itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
    
    
  }
  
} // end namespace otb

#endif
