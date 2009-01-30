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
#ifndef __otbFunctionWithNeighborhoodToImageFilter_txx
#define __otbFunctionWithNeighborhoodToImageFilter_txx

#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "otbMacro.h"

namespace otb
{

/**
 * Constructor
 */
template<class TInputImage, class TOutputImage, class TFunction >
FunctionWithNeighborhoodToImageFilter<TInputImage,TOutputImage,TFunction>
::FunctionWithNeighborhoodToImageFilter()
{
  m_Radius.Fill(0);
  m_Offset.Fill(0);
}


template <class TInputImage, class TOutputImage, class TFunction  >
void
FunctionWithNeighborhoodToImageFilter<TInputImage,TOutputImage,TFunction>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  m_Radius = this->GetFunction()->GetRadius();
  m_Offset = this->GetFunction()->GetOffset(); 
}

template <class TInputImage, class TOutputImage, class TFunction  >
void
FunctionWithNeighborhoodToImageFilter<TInputImage,TOutputImage,TFunction>
::GenerateInputRequestedRegion()
{ 
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  InputImagePointer inputPtr = const_cast< TInputImage * >( this->GetInput());
  OutputImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }
  // get a copy of the input requested region (should equal the output
  // requested region)
  InputImageRegionType inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  InputImageSizeType maxRad;
  maxRad[0] = m_Radius[0] + vcl_abs(m_Offset[0]);
  maxRad[1] = m_Radius[0] + vcl_abs(m_Offset[1]); 
  inputRequestedRegion.PadByRadius( maxRad );

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
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}


/**
 * ThreadedGenerateData function. Performs the pixel-wise addition
 */
template<class TInputImage, class TOutputImage, class TFunction >
void
FunctionWithNeighborhoodToImageFilter<TInputImage,TOutputImage,TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{
  // We use dynamic_cast since inputs are stored as DataObjects.
  InputImageConstPointer inputPtr = dynamic_cast<const TInputImage*>((itk::ProcessObject::GetInput(0)));

  OutputImagePointer outputPtr = this->GetOutput(0);

  itk::ImageRegionConstIterator<TInputImage> inputIt(inputPtr, outputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while( !inputIt.IsAtEnd() )
    {
      //std::cout<<"MOTHER "<<threadId<<" : "<<inputIt.GetIndex()<<" "<<inputIt.Get()<<std::endl;
      outputIt.Set( static_cast<OutputImagePixelType>(this->GetFunction()->EvaluateAtIndex(inputIt.GetIndex())) );
      ++inputIt;
      ++outputIt;

      progress.CompletedPixel(); // potential exception thrown here
    }
}
} // end namespace otb

#endif
