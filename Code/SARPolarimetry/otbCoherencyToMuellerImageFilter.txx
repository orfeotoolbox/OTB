/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $otbCoherencyToMuellerImageFilter.txx$
  Language:  C++
  Date:      $Date: 24/10/2006 $
  Version:   $Version: 1.0 $

=========================================================================*/
#ifndef __CoherencyToMuellerImageFilter_txx
#define __CoherencyToMuellerImageFilter_txx

#include "otbCoherencyToMuellerImageFilter.h"

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
  CoherencyToMuellerImageFilter<TPixel>::CoherencyToMuellerImageFilter()
  {
    // !! TInputImage should be StokesType and TOutputImage should be MLCType (otherwise fail)
  
  }

  template <class TPixel>
  void 
  CoherencyToMuellerImageFilter<TPixel>
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
  CoherencyToMuellerImageFilter< TPixel>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
			 int threadId)
  {
    float T1, T2, T3, T4, T5, T6, T7, T8, T9;
    float M1, M2, M3, M4, M5, M6, M7, M8, M9, M10;
    itk::ImageRegionConstIterator<InputImageType> inIt (this->GetInput(),outputRegionForThread);
    itk::ImageRegionIterator<OutputImageType> outIt (this->GetOutput(),outputRegionForThread);
    MuellerType vectorValue;
    
    
    int numPix =0;
    
    for ( inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, ++outIt)
    {
      T1 = inIt.Get()[0];
      T2 = inIt.Get()[1];
      T3 = inIt.Get()[2];
      T4 = inIt.Get()[3];
      T5 = inIt.Get()[4];
      T6 = inIt.Get()[5];
      T7 = inIt.Get()[6];
      T8 = inIt.Get()[7];
      T9 = inIt.Get()[8];

      

      vectorValue[0]=(T1+T2+T3)/2.;  //A0+B0 
      vectorValue[1]=(T1+T2-T3)/2.;  //A0+B
      vectorValue[2]=(T1-T2+T3)/2.;  //A0-B
      vectorValue[3]=(-T1+T2+T3)/2.; //-A0+B0
      vectorValue[4]=T4;             //C
      vectorValue[5]=T6;             //H               
      vectorValue[6]=T9;             //F              
      vectorValue[7]=T8;             //E              
      vectorValue[8]=T7;             //G
      vectorValue[9]=-T5;            //D

      
      outIt.Set(vectorValue);

    
      numPix++;
    }
  }
  

  /**
   * Standard "PrintSelf" method
   */
  template <class TPixel>
  void
  CoherencyToMuellerImageFilter<TPixel>
  ::PrintSelf(
	      std::ostream& os, 
	      itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
    
    
  }
  
} // end namespace otb

#endif
