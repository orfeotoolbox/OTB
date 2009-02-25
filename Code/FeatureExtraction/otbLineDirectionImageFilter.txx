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
#ifndef __otbLineDirectionImageFilter_txx
#define __otbLineDirectionImageFilter_txx

#include "otbLineDirectionImageFilter.h"

#include "itkProgressReporter.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "otbMath.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
LineDirectionImageFilter<TInputImage,TOutputImage>
::LineDirectionImageFilter()
{  
  this->SetNumberOfRequiredInputs( 1 );
  m_Radius = this->GetSpatialThreshold();
  m_FunctorList.clear();
}


template <class TInputImage, class TOutputImage>
void
LineDirectionImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
  if(this->GetSpatialThreshold() < this->GetRatioMaxConsiderationNumber())
    {
      itkExceptionMacro(<<"Spatial Threshold ("<<this->GetSpatialThreshold()<<") is lower than Ration Max Consideration Number ("<<this->GetRatioMaxConsiderationNumber()<<") what is not allowed.");
    }
  for (int i =0; i<this->GetNumberOfThreads(); i++)
    {
      m_FunctorList.push_back(m_Functor);
    }
}

template <class TInputImage, class TOutputImage>
void
LineDirectionImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr =
    const_cast< TInputImage * >( this->GetInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
  {
    return;
  }
  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  InputImageSizeType rad;
  rad[0] = m_Radius;
  rad[1] = m_Radius;
  inputRequestedRegion.PadByRadius( rad );

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



template <class TInputImage, class TOutputImage>
void
LineDirectionImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(6);
}


template <class TInputImage, class TOutputImage>
void
LineDirectionImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId )
{
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;

// We use dynamic_cast since inputs are stored as DataObjects.  The
  // ImageToImageFilter::GetInput(int) always returns a pointer to a
  // TInputImage so it cannot be used for the second input.
  InputImagePointerType inputPtr
  = dynamic_cast<const TInputImage*>(ProcessObjectType::GetInput(0));
  OutputImagePointerType outputPtr = this->GetOutput(0);

  RadiusType r;
  r.Fill(this->GetRadius());
  NeighborhoodIteratorType neighInputIt;

  itk::ImageRegionIterator<TOutputImage> outputIt;
  FunctorOutputType outputFunctor;

  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage> bC;
  faceList = bC(inputPtr, outputRegionForThread, r);


  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType::iterator fit;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
  {
    neighInputIt = itk::ConstNeighborhoodIterator<TInputImage>(r, inputPtr, *fit);

    outputIt = itk::ImageRegionIterator<TOutputImage>(outputPtr, *fit);
    neighInputIt.OverrideBoundaryCondition(&nbc);
    neighInputIt.GoToBegin();

    while ( ! outputIt.IsAtEnd() )
    {
      OutputImagePixelType out;
      out.SetSize(6);
      outputFunctor = m_FunctorList[threadId]( neighInputIt);
      for(unsigned int i = 0; i<outputFunctor.size(); i++)
	{
	  out[i] = outputFunctor[i];
	}	
      outputIt.Set( out );

      ++neighInputIt;
      ++outputIt;
      progress.CompletedPixel();
    }
  }
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage>
void
LineDirectionImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );

  
  //os << indent << "Spatial Threshold             : "  << this->GetSpatialThreshold() << std::endl;
  //os << indent << "Spectral Threshold            : "  << this->GetSpectralThreshold() << std::endl;
  //os << indent << "Ratio Max Consideration Number: "  << this->GetRatioMaxConsiderationNumber() << std::endl;
  //os << indent << "Alpha                         : "  << this->GetAlpha() << std::endl;
  //os << indent << "Number Of Directions          : "  << this->GetNumberOfDirections() << std::endl;
  
}


} // end namespace otb


#endif
