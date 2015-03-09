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
#ifndef __otbSFSTexturesImageFilter_txx
#define __otbSFSTexturesImageFilter_txx

#include "otbSFSTexturesImageFilter.h"

#include "itkProgressReporter.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "otbMath.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
SFSTexturesImageFilter<TInputImage, TOutputImage>
::SFSTexturesImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(6);
  this->SetNumberOfRequiredOutputs(1);

  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputImageType::New());
  this->SetNthOutput(3, OutputImageType::New());
  this->SetNthOutput(4, OutputImageType::New());
  this->SetNthOutput(5, OutputImageType::New());

  m_Radius = this->GetSpatialThreshold();
  m_FunctorList.clear();

}
/************************************************************
 *
 *              OUTPUTS MANIPULATION
 *
 ************************************************************/
// Return output length image
template <class TInputImage, class TOutputImage>
const typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetLengthOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[0] == false)
    {
    itkExceptionMacro(<< "Impossible to create length image : texture not selected");
    }
  return static_cast<const OutputImageType *> (this->itk::ProcessObject::GetOutput(0));
}
template <class TInputImage, class TOutputImage>
typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetLengthOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[0] == false)
    {
    itkExceptionMacro(<< "Impossible to create length image : texture not selected");
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
}

// Return output width image
template <class TInputImage, class TOutputImage>
const typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetWidthOutput() const
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[1] == false)
    {
    itkExceptionMacro(<< "Impossible to create width image : texture not selected");
    }
  return static_cast<const OutputImageType *> (this->itk::ProcessObject::GetOutput(1));
}
template <class TInputImage, class TOutputImage>
typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetWidthOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[1] == false)
    {
    itkExceptionMacro(<< "Impossible to create width image : texture not selected");
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}

//Return output PSI image
template <class TInputImage, class TOutputImage>
const typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetPSIOutput() const
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[2] == false)
    {
    itkExceptionMacro(<< "Impossible to create PSI image : texture not selected");
    }
  return static_cast<const OutputImageType *> (this->itk::ProcessObject::GetOutput(2));
}
template <class TInputImage, class TOutputImage>
typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetPSIOutput()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[2] == false)
    {
    itkExceptionMacro(<< "Impossible to create PSI image : texture not selected");
    }

  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(2));
}

// Return output WMean image
template <class TInputImage, class TOutputImage>
const typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetWMeanOutput() const
{
  if (this->GetNumberOfOutputs() < 4)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[3] == false)
    {
    itkExceptionMacro(<< "Impossible to create W-Mean image : texture not selected");
    }
  return static_cast<const OutputImageType *> (this->itk::ProcessObject::GetOutput(3));
}
template <class TInputImage, class TOutputImage>
typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetWMeanOutput()
{
  if (this->GetNumberOfOutputs() < 4)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[3] == false)
    {
    itkExceptionMacro(<< "Impossible to create W-Mean image : texture not selected");
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(3));
}

// Return output ratio image
template <class TInputImage, class TOutputImage>
const typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetRatioOutput() const
{
  if (this->GetNumberOfOutputs() < 5)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[4] == false)
    {
    itkExceptionMacro(<< "Impossible to create Ratio image : texture not selected");
    }
  return static_cast<const OutputImageType *> (this->itk::ProcessObject::GetOutput(4));
}
template <class TInputImage, class TOutputImage>
typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetRatioOutput()
{
  if (this->GetNumberOfOutputs() < 5)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[4] == false)
    {
    itkExceptionMacro(<< "Impossible to create Ratio image : texture not selected");
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(4));
}

// Return output SD image
template <class TInputImage, class TOutputImage>
const typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetSDOutput() const
{
  if (this->GetNumberOfOutputs() < 6)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[5] == false)
    {
    itkExceptionMacro(<< "Impossible to create SD image : texture not selected");
    }
  return static_cast<const OutputImageType *> (this->itk::ProcessObject::GetOutput(5));
}
template <class TInputImage, class TOutputImage>
typename SFSTexturesImageFilter<TInputImage, TOutputImage>::OutputImageType *
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GetSDOutput()
{
  if (this->GetNumberOfOutputs() < 6)
    {
    return 0;
    }
  if (this->GetTexturesStatus()[5] == false)
    {
    itkExceptionMacro(<< "Impossible to create SD image : texture not selected");
    }

  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(5));
}

template <class TInputImage, class TOutputImage>
void
SFSTexturesImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
  if (this->GetSpatialThreshold() < this->GetRatioMaxConsiderationNumber())
    {
    itkExceptionMacro(<< "Spatial Threshold (" << this->GetSpatialThreshold()
                      << ") is lower than Ration Max Consideration Number ("
                      << this->GetRatioMaxConsiderationNumber() << ") what is not allowed.");
    }
  for (unsigned int i = 0; i < this->GetNumberOfThreads(); ++i)
    {
    m_FunctorList.push_back(m_Functor);
    }
  this->InitFeatureStatus(true);
}

template <class TInputImage, class TOutputImage>
void
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr = const_cast<TInputImage *>(this->GetInput());
  typename Superclass::OutputImagePointer outputPtr1 = this->GetOutput(0); //this->GetLengthOutput();
  typename Superclass::OutputImagePointer outputPtr2 = this->GetOutput(1); //this->GetWidthOutput();
  typename Superclass::OutputImagePointer outputPtr3 = this->GetOutput(2); //this->GetPSIOutput();
  typename Superclass::OutputImagePointer outputPtr4 = this->GetOutput(3); //this->GetWMeanOutput();
  typename Superclass::OutputImagePointer outputPtr5 = this->GetOutput(4); //this->GetRatioOutput();
  typename Superclass::OutputImagePointer outputPtr6 = this->GetOutput(5); //this->GetSDOutput();

  if (!inputPtr || !outputPtr1 || !outputPtr2 || !outputPtr3 || !outputPtr4 || !outputPtr5 || !outputPtr6)
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
  inputRequestedRegion.PadByRadius(rad);

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

template <class TInputImage, class TOutputImage>
void
SFSTexturesImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  //this->GetOutput()->SetNumberOfComponentsPerPixel(6);
}

template <class TInputImage, class TOutputImage>
void
SFSTexturesImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;

// We use dynamic_cast since inputs are stored as DataObjects.  The
// ImageToImageFilter::GetInput(int) always returns a pointer to a
// TInputImage so it cannot be used for the second input.
  InputImagePointerType  inputPtr = dynamic_cast<const TInputImage*>(ProcessObjectType::GetInput(0));
  OutputImagePointerType outputPtr1 = this->GetOutput(0);
  OutputImagePointerType outputPtr2 = this->GetOutput(1);
  OutputImagePointerType outputPtr3 = this->GetOutput(2);
  OutputImagePointerType outputPtr4 = this->GetOutput(3);
  OutputImagePointerType outputPtr5 = this->GetOutput(4);
  OutputImagePointerType outputPtr6 = this->GetOutput(5);

  RadiusType r;
  r.Fill(this->GetRadius());
  NeighborhoodIteratorType neighInputIt;

  itk::ImageRegionIterator<TOutputImage> outputIt1, outputIt2, outputIt3, outputIt4, outputIt5, outputIt6;
  FunctorOutputType                      outputFunctor;

  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>               bC;
  faceList = bC(inputPtr, outputRegionForThread, r);

  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType::iterator fit;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.

  std::vector<bool> textStatus = this->GetTexturesStatus();
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    {
    neighInputIt = itk::ConstNeighborhoodIterator<TInputImage>(r, inputPtr, *fit);

    outputIt1 = itk::ImageRegionIterator<TOutputImage>(outputPtr1, *fit);
    outputIt2 = itk::ImageRegionIterator<TOutputImage>(outputPtr2, *fit);
    outputIt3 = itk::ImageRegionIterator<TOutputImage>(outputPtr3, *fit);
    outputIt4 = itk::ImageRegionIterator<TOutputImage>(outputPtr4, *fit);
    outputIt5 = itk::ImageRegionIterator<TOutputImage>(outputPtr5, *fit);
    outputIt6 = itk::ImageRegionIterator<TOutputImage>(outputPtr6, *fit);

    std::vector<itk::ImageRegionIterator<TOutputImage> *> outItList;
    outItList.push_back(&outputIt1);
    outItList.push_back(&outputIt2);
    outItList.push_back(&outputIt3);
    outItList.push_back(&outputIt4);
    outItList.push_back(&outputIt5);
    outItList.push_back(&outputIt6);

    neighInputIt.OverrideBoundaryCondition(&nbc);
    neighInputIt.GoToBegin();

    for (unsigned int i = 0; i < outItList.size(); ++i)
      {
      (*outItList[i]).GoToBegin();
      }

    while (!outputIt1.IsAtEnd())
      {

      outputFunctor = m_FunctorList[threadId](neighInputIt);
      for (unsigned int i = 0; i < outItList.size(); ++i)
        {
        if (textStatus[i] == true) (*outItList[i]).Set(outputFunctor[i]);
        }

      ++neighInputIt;
      for (unsigned int i = 0; i < outItList.size(); ++i)
        {
        ++(*outItList[i]);
        }

      progress.CompletedPixel();
      }
    }
}

template <class TInputImage, class TOutputImage>
void
SFSTexturesImageFilter<TInputImage, TOutputImage>
::InitFeatureStatus(bool status)
{
  for (FeatureType id = LENGTH; id <= SD;
       id = static_cast<FeatureType>(id + 1))
    {
    this->SetFeatureStatus(static_cast<FeatureType>(id), status);
    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage>
void
SFSTexturesImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  //os << indent << "Spatial Threshold             : "  << this->GetSpatialThreshold() << std::endl;
  //os << indent << "Spectral Threshold            : "  << this->GetSpectralThreshold() << std::endl;
  //os << indent << "Ratio Max Consideration Number: "  << this->GetRatioMaxConsiderationNumber() << std::endl;
  //os << indent << "Alpha                         : "  << this->GetAlpha() << std::endl;
  //os << indent << "Number Of Directions          : "  << this->GetNumberOfDirections() << std::endl;

}

} // end namespace otb

#endif
