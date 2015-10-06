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
#ifndef __otbTwoNRIBandsImageToNComplexBandsImage_txx
#define __otbTwoNRIBandsImageToNComplexBandsImage_txx

#include "otbTwoNRIBandsImageToNComplexBandsImage.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "itkVariableLengthVector.h"




namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>::TwoNRIBandsImageToNComplexBandsImage()
{
	//this->SetNumberOfThreads(1);
}

/**
 * GenerateOutputInformation
 */
template<class TInputImage, class TOutputImage>
void
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  
  unsigned int nbCompo = this->GetInput()->GetNumberOfComponentsPerPixel();
  
  if ( (nbCompo % 2) != 0 )
  {
	itkExceptionMacro("Number of bands of the input images must be an even number");
  }
  else
	this->GetOutput()->SetNumberOfComponentsPerPixel(nbCompo/2);
	
	std::cout << "GenerateOutputInformation : " << this->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
  
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage>
void
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>
::BeforeThreadedGenerateData(void)
{
	unsigned int nbCompo = this->GetInput()->GetNumberOfComponentsPerPixel();
			
	if ( (nbCompo % 2) != 0 )
	  itkExceptionMacro("Number of bands of the input images must be an even number");
		
}

/**
 * ThreadedGenerateData
 */
template<class TInputImage, class TOutputImage>
void TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>::ThreadedGenerateData(
  const OutputImageRegionType&     outputRegionForThread,
  itk::ThreadIdType threadId
  )
{
 
  unsigned int nbCompo = this->GetInput()->GetNumberOfComponentsPerPixel();
  
  itk::VariableLengthVector< std::complex< typename InputPixelType::ValueType > > vlv(nbCompo/2);
  
 
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());	

  itk::ImageRegionConstIterator<TInputImage>  inIt;
  inIt = itk::ImageRegionConstIterator<TInputImage>(this->GetInput(), outputRegionForThread);	
	
  itk::ImageRegionIterator<OutputImageType>      outIt;
  outIt = itk::ImageRegionIterator<OutputImageType>(this->GetOutput(), outputRegionForThread);
  

  inIt.GoToBegin();
  outIt.GoToBegin();

  while (!outIt.IsAtEnd())
  {
	  
	  unsigned int k=0;
	  for (unsigned int i=0; i<nbCompo-1; i=i+2)
	  {
		  vlv[k] = std::complex< typename InputPixelType::ValueType >(inIt.Get()[i],inIt.Get()[i+1]);
		  k++;
	  }

	  
	  outIt.Set(vlv);
	  

	  ++inIt;
	  ++outIt;
	  
	  progress.CompletedPixel();
  }
  
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
