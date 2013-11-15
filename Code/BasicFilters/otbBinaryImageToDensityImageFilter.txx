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
#ifndef __otbBinaryImageToDensityImageFilter_txx
#define __otbBinaryImageToDensityImageFilter_txx

#include "otbBinaryImageToDensityImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkNeighborhoodAlgorithm.h"

#include "otbMacro.h"

namespace otb
{
/** Constructor */
template <class TInputImage, class TOutputImage, class TCountFunction>
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::BinaryImageToDensityImageFilter()
{
  m_NeighborhoodRadius.Fill(1);
  m_CountFunction = CountFunctionType::New();
}

/** Destructor */
template <class TInputImage, class TOutputImage, class TCountFunction>
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::~BinaryImageToDensityImageFilter()
{}

template <class TInputImage, class TOutputImage, class TCountFunction>
void
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  InputImagePointerType  inputPtr = const_cast<TInputImage *>(this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }
  // get a copy of the input requested region (should equal the output
  // requested region)
  InputImageRegionType inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius(m_NeighborhoodRadius);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << this->GetNameOfClass()
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

template <class TInputImage, class TOutputImage, class TCountFunction>
void
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  m_CountFunction->SetInputImage(this->GetInput());
  m_CountFunction->SetNeighborhoodRadius(m_NeighborhoodRadius);
}

/** Main computation method */
template <class TInputImage, class TOutputImage, class TCountFunction>
void
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::ThreadedGenerateData(const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  InputImagePointerType  inputPtr = const_cast<InputImageType *> (this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;
  RadiusType                                         r;
  r[0] = m_NeighborhoodRadius[0];
  r[1] = m_NeighborhoodRadius[1];

  NeighborhoodIteratorType                                                                     it;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>               bC;
  faceList = bC(inputPtr, outputRegionForThread, r);
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType::iterator fit;

  itk::ImageRegionIterator<OutputImageType>     itOut(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  typename InputImageType::IndexType index;

  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    {
    it = itk::ConstNeighborhoodIterator<TInputImage>(r, inputPtr, *fit);

    itOut = itk::ImageRegionIterator<TOutputImage>(outputPtr, *fit);
    it.OverrideBoundaryCondition(&nbc);
    it.GoToBegin();

    while (!itOut.IsAtEnd())
      {
      index = it.GetIndex();

      if (outputRegionForThread.IsInside(index))
        {
        itOut.Set(m_CountFunction->EvaluateAtIndex(index));
        }

      ++itOut;
      ++it;
      progress.CompletedPixel();     // potential exception thrown here
      }
    }
}

/** PrintSelf method */
template <class TInputImage, class TOutputImage, class TCountFunction>
void
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Neighborhood Radius : " << m_NeighborhoodRadius << std::endl;
}
} // End namespace otb
#endif
