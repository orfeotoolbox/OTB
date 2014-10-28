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

#ifndef __otbDrawPathListFilter_txx
#define __otbDrawPathListFilter_txx

#include "otbDrawPathListFilter.h"

#include "itkImageRegionIterator.h"
#include "otbPolyLineImageIterator.h"
#include "itkMetaDataObject.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TInputPath, class TOutputImage>
DrawPathListFilter<TInputImage, TInputPath, TOutputImage>
::DrawPathListFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  m_PathValue = static_cast<OutputImagePixelType>(255);
  m_UseInternalPathValue = false;
  m_AddValue = false;
}

template <class TInputImage, class TInputPath, class TOutputImage>
void
DrawPathListFilter<TInputImage, TInputPath, TOutputImage>
::SetInputPath(InputPathListType *path)
{
  this->ProcessObjectType::SetNthInput(1, const_cast<InputPathListType *>(path));
}

template <class TInputImage, class TInputPath, class TOutputImage>
const typename DrawPathListFilter<TInputImage, TInputPath, TOutputImage>::InputPathListType *
DrawPathListFilter<TInputImage, TInputPath, TOutputImage>
::GetInputPath(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return static_cast<const InputPathListType *>(this->ProcessObjectType::GetInput(1));
}

/**
 * Main computation method
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void
DrawPathListFilter<TInputImage, TInputPath, TOutputImage>
::GenerateData(void)
{
  OutputImagePointerType        outputPtr = this->GetOutput();
  InputImageConstPointerType    inputPtr = this->GetInput();
  InputPathListConstPointerType pathListPtr = this->GetInputPath();
  outputPtr->SetRegions(inputPtr->GetLargestPossibleRegion());
  outputPtr->Allocate();
  outputPtr->FillBuffer(itk::NumericTraits<OutputImagePixelType>::Zero);

  // First, we copy input to output
  typedef itk::ImageRegionIterator<OutputImageType>             OutputIteratorType;
  typedef itk::ImageRegionConstIterator<InputImageType>         InputIteratorType;
  typedef typename InputPathListType::ConstIterator             PathListIteratorType;
  typedef PolyLineImageIterator<OutputImageType, InputPathType> PolyLineIteratorType;

  OutputIteratorType outIt(outputPtr, outputPtr->GetLargestPossibleRegion());
  InputIteratorType inIt(inputPtr, inputPtr->GetLargestPossibleRegion());

  for (outIt.GoToBegin(), inIt.GoToBegin();
       (!outIt.IsAtEnd() && !inIt.IsAtEnd());
       ++outIt, ++inIt)
    {
    outIt.Set(static_cast<OutputImagePixelType>(inIt.Get()));
    }

  // Then we use otb::PolyLineImageIterator to draw polylines
  for (PathListIteratorType plIt = pathListPtr->Begin(); plIt != pathListPtr->End(); ++plIt)
    {
    OutputImagePixelType value = itk::NumericTraits<OutputImagePixelType>::Zero;
    if (m_UseInternalPathValue && plIt.Get()->GetMetaDataDictionary().HasKey("Value"))
      {
      itk::ExposeMetaData<OutputImagePixelType>(plIt.Get()->GetMetaDataDictionary(), "Value", value);
      }
    else
      {
      value = static_cast<OutputImagePixelType>(m_PathValue);
      }
    PolyLineIteratorType imageIt(outputPtr, plIt.Get());
    for (imageIt.GoToBegin(); !imageIt.IsAtEnd(); ++imageIt)
      {
      if (m_AddValue)
        {
        imageIt.Set(imageIt.Get() + 1);
        }
      else
        {
        imageIt.Set(value);
        }
      }
    }
}

/**
 * Printself method
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void
DrawPathListFilter<TInputImage, TInputPath, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
