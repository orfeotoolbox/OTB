/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  
  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbReflectanceToSurfaceReflectanceImageFilter_txx
#define __otbReflectanceToSurfaceReflectanceImageFilter_txx



#include "otbReflectanceToSurfaceReflectanceImageFilter.h"



 namespace otb
{
  
  template <class TInputImage, class TOutputImage>
  ReflectanceToSurfaceReflectanceImageFilter<TInputImage,TOutputImage>
  ::ReflectanceToSurfaceReflectanceImageFilter()
  {
    m_AtmosphericRadiativeTerms = AtmosphericRadiativeTerms::New();
  }
  
 
  template <class TInputImage, class TOutputImage>
  ReflectanceToSurfaceReflectanceImageFilter<TInputImage,TOutputImage>
  ::~ReflectanceToSurfaceReflectanceImageFilter()
  {
  }
  
  
  template <class TInputImage, class TOutputImage>
  void
  ReflectanceToSurfaceReflectanceImageFilter<TInputImage,TOutputImage>
  ::BeforeThreadedGenerateData ()
  {	
    // Allocate output
    typename OutputImageType::Pointer output = this->GetOutput();
    typename InputImageType::Pointer  input  = const_cast<InputImageType *>(this->GetInput(0)); 
    
    double coef;
    double res;
    coef = static_cast<double>(m_AtmosphericRadiativeTerms->GetTotalGaseousTransmission()
			       * m_AtmosphericRadiativeTerms->GetDownwardTransmittance()
			       * m_AtmosphericRadiativeTerms->GetUpwardTransmittance()     );
    coef = 1 / coef;
    
    res = -m_AtmosphericRadiativeTerms->GetIntrinsicAtmosphericReflectance() * coef;
 

    // Functor initialisation
    this->GetFunctor().SetCoefficient(coef);
    this->GetFunctor().SetResidu(res);
    this->GetFunctor().SetSphericalAlbedo(static_cast<double>(m_AtmosphericRadiativeTerms->GetSphericalAlbedo()));
  }
  
   /**
   * ThreadedGenerateData Performs the pixel-wise addition
   */
  template <class TInputImage, class TOutputImage>
  void
  ReflectanceToSurfaceReflectanceImageFilter<TInputImage,TOutputImage>
  ::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId )
  {
    typename InputImageType::Pointer  inputPtr  = const_cast<InputImageType *>(this->GetInput(0));
    typename OutputImageType::Pointer outputPtr = static_cast<OutputImageType *>(this->GetOutput());
  
    // Define the portion of the input to walk for this thread, using
    // the CallCopyOutputRegionToInputRegion method allows for the input
    // and output images to be different dimensions
    
    InputImageRegionType inputRegionForThread;
    this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);
    
    // Define the iterators
    itk::ImageRegionConstIterator<InputImageType>  inputIt(inputPtr, inputRegionForThread);
    itk::ImageRegionIterator<OutputImageType>      outputIt(outputPtr, outputRegionForThread);
    
    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
    
    inputIt.GoToBegin();
    outputIt.GoToBegin();
  
    while( !inputIt.IsAtEnd() ) 
      {
	InputPixelType inPixel = inputIt.Get();
	OutputPixelType outPixel = outputIt.Get();
	
	for (unsigned int j=0; j<inPixel.GetSize(); j++)
	  {
	    outPixel[j] = this->GetFunctor()( inPixel[j] );
	  }	
	outputIt.Set(outPixel);
	++inputIt;
	++outputIt;
	progress.CompletedPixel();  // potential exception thrown here    
      }
    
  }

 } // end namespace otb

#endif


