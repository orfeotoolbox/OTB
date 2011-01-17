/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $otbMLCToCoherencyDegreeImageFilter.txx$
  Language:  C++
  Date:      $Date: 24/10/2006 $
  Version:   $Version: 1.0 $

=========================================================================*/
#ifndef __MLCToCoherencyDegreeImageFilter_txx
#define __MLCToCoherencyDegreeImageFilter_txx

#include "otbMLCToCoherencyDegreeImageFilter.h"

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
  MLCToCoherencyDegreeImageFilter<TPixel>::MLCToCoherencyDegreeImageFilter()
  {
    // !! TInputImage should be MLCType and TOutputImage should be CoherencyDegreeType (otherwise fail)
  
  }

  template <class TPixel>
  void 
  MLCToCoherencyDegreeImageFilter<TPixel>
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
  MLCToCoherencyDegreeImageFilter< TPixel>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
			 int threadId)
  {
    float C1, C2, C3, C4, C5, C6, C7, C8, C9;
    
    itk::ImageRegionConstIterator<InputImageType> inIt (this->GetInput(),outputRegionForThread);
    itk::ImageRegionIterator<OutputImageType> outIt (this->GetOutput(),outputRegionForThread);
    CoherencyDegreeType vectorValue;
    
    std::cout << "MLCToCoherencyDegreeImageFilter -> Entrée dans ThreadedGenerateData()" << std::endl;
    
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

      if ((C1 >0.00001) && (C3 > 0.0001)) 
	{
	  vectorValue[0]= sqrt( C6*C6 + C7*C7 ) / sqrt (C1*C3); // |<hh.vv*|/sqrt(<hh.hh*><vv.vv*>)
	} else
	{ 
	  vectorValue[0]=0.;
        }

      if ((C2 > 0.00001) && (C3 > 0.00001)) 
	{
	  vectorValue[1]= sqrt( C8*C8 + C9*C9 ) / sqrt (C2*C3); // |<hv.vv*|/sqrt(<hv.hv*><vv.vv*>)
	} else
	{ 
	  vectorValue[1]=0.;
        }

      if ((C1 > 0.00001) && (C2 > 0.00001) )
	{
	  vectorValue[2]= sqrt( C4*C4 + C5*C5 ) / sqrt (C1*C2); // |<hh.hv*|/sqrt(<hh.hh*><hv.hv*>)
	} else
	{ 
	  vectorValue[2]=0.;
        }
      
      outIt.Set(vectorValue);

    
      numPix++;
    }
  }
  

  /**
   * Standard "PrintSelf" method
   */
  template <class TPixel>
  void
  MLCToCoherencyDegreeImageFilter<TPixel>
  ::PrintSelf(
	      std::ostream& os, 
	      itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
    
    
  }
  
} // end namespace otb

#endif
