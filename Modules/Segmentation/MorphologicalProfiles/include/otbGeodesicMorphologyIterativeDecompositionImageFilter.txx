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
#ifndef __otbGeodesicMorphologyIterativeDecompositionImageFilter_txx
#define __otbGeodesicMorphologyIterativeDecompositionImageFilter_txx

#include "otbGeodesicMorphologyIterativeDecompositionImageFilter.h"

#include "itkProgressAccumulator.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TImage, class TStructuringElement>
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::GeodesicMorphologyIterativeDecompositionImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(3);
  m_NumberOfIterations  = 2;
  m_InitialValue = 1;
  m_Step = 1;
  OutputImageListPointerType convList = OutputImageListType::New();
  this->SetNthOutput(0, convList.GetPointer());
  OutputImageListPointerType concList = OutputImageListType::New();
  this->SetNthOutput(1, concList.GetPointer());
  OutputImageListPointerType outputList = OutputImageListType::New();
  this->SetNthOutput(2, outputList.GetPointer());
}

template <class TImage, class TStructuringElement>
typename GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::OutputImageListType*
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::GetOutput(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(2));
}

template <class TImage, class TStructuringElement>
typename GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::OutputImageListType*
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::GetConvexOutput(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(0));
}

template <class TImage, class TStructuringElement>
typename GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::OutputImageListType*
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::GetConcaveOutput(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(1));
}

template <class TImage, class TStructuringElement>
void
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::GenerateOutputInformation(void)
{
  // Retrieving input/output pointers
  InputImagePointerType      inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();
  OutputImageListPointerType convOutputPtr = this->GetConvexOutput();
  OutputImageListPointerType concOutputPtr = this->GetConcaveOutput();
  if (outputPtr)
    {
    if (outputPtr->Size() != m_NumberOfIterations)
      {
      // in this case, clear the list
      outputPtr->Clear();
      for (unsigned int i = 0; i < m_NumberOfIterations; ++i)
        {
        //Create the output image
        outputPtr->PushBack(OutputImageType::New());
        }
      }
    // For each output image
    typename OutputImageListType::Iterator outputListIt = outputPtr->Begin();
    while (outputListIt != outputPtr->End())
      {
      //Set the image information
      outputListIt.Get()->CopyInformation(this->GetInput());
      outputListIt.Get()->SetRequestedRegion(this->GetInput()->GetLargestPossibleRegion());
      ++outputListIt;
      }
    }
  if (convOutputPtr)
    {
    if (convOutputPtr->Size() != m_NumberOfIterations)
      {
      // in this case, clear the list
      convOutputPtr->Clear();
      for (unsigned int i = 0; i < m_NumberOfIterations; ++i)
        {
        //Create the output image
        convOutputPtr->PushBack(OutputImageType::New());
        }
      }
    // For each output image
    typename OutputImageListType::Iterator outputListIt = convOutputPtr->Begin();
    while (outputListIt != convOutputPtr->End())
      {
      //Set the image information
      outputListIt.Get()->CopyInformation(this->GetInput());
      outputListIt.Get()->SetRequestedRegion(this->GetInput()->GetLargestPossibleRegion());
      ++outputListIt;
      }
    }
  if (concOutputPtr)
    {
    if (concOutputPtr->Size() != m_NumberOfIterations)
      {
      // in this case, clear the list
      concOutputPtr->Clear();
      for (unsigned int i = 0; i < m_NumberOfIterations; ++i)
        {
        //Create the output image
        concOutputPtr->PushBack(OutputImageType::New());
        }
      }
    // For each output image
    typename OutputImageListType::Iterator outputListIt = concOutputPtr->Begin();
    while (outputListIt != concOutputPtr->End())
      {
      //Set the image information
      outputListIt.Get()->CopyInformation(this->GetInput());
      outputListIt.Get()->SetRequestedRegion(this->GetInput()->GetLargestPossibleRegion());
      ++outputListIt;
      }
    }
}

template <class TImage, class TStructuringElement>
void
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::GenerateInputRequestedRegion(void)
{
  // Retrieving input/output pointers
  InputImagePointerType      inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();

  // For each output image
  typename OutputImageListType::Iterator outputListIt = outputPtr->Begin();
  inputPtr->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
}

/**
 * Main computation method
 */
template <class TImage, class TStructuringElement>
void
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::GenerateData(void)
{
  //Input image pointer
  InputImagePointerType      current = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();
  OutputImageListPointerType convOutputPtr = this->GetConvexOutput();
  OutputImageListPointerType concOutputPtr = this->GetConcaveOutput();

  unsigned int i = 0;

  DecompositionFilterPointerType filter;

  while (i < m_NumberOfIterations)
    {
    filter = DecompositionFilterType::New();
    typename StructuringElementType::RadiusType radius;
    radius.Fill(m_InitialValue + i * m_Step);
    filter->SetRadius(radius);
    filter->SetInput(current);
    filter->GetOutput()->UpdateOutputInformation();
    filter->GetOutput()->SetRequestedRegion(outputPtr->GetNthElement(i)->GetRequestedRegion());
    filter->GetOutput()->PropagateRequestedRegion();
    filter->GetOutput()->UpdateOutputData();

    outputPtr->SetNthElement(i, filter->GetOutput());
    concOutputPtr->SetNthElement(i, filter->GetConvexMap());
    convOutputPtr->SetNthElement(i, filter->GetConcaveMap());

    current = filter->GetOutput();

    ++i;
    }

}
/**
 * PrintSelf method
 */
template <class TImage, class TStructuringElement>
void
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "NumberOfIterations: " << m_NumberOfIterations << std::endl;
  os << indent << "IntialValue: " << m_InitialValue << std::endl;
  os << indent << "Step: " << m_Step << std::endl;
}
} // End namespace otb
#endif
