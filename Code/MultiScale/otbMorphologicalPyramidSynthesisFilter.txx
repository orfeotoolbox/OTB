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
#ifndef __otbMorphologicalPyramidSynthesisFilter_txx
#define __otbMorphologicalPyramidSynthesisFilter_txx

#include "otbMorphologicalPyramidSynthesisFilter.h"

#include "otbMorphologicalPyramidResampler.h"
#include "itkSubtractImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkImageDuplicator.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::MorphologicalPyramidSynthesisFilter()
{
  this->SetNumberOfRequiredInputs(5);
}
/**
 * Destructor
 */
template <class TInputImage, class TOutputImage>
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::~MorphologicalPyramidSynthesisFilter() {}
/**
 * Set The SupFilter details
 * \param imageList The brighter details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::SetSupFilter(InputImageListType * imageList)
{
  this->SetNthInput(1, const_cast<InputImageListType *>(imageList));
}
/**
 * Set The InfFilter details
 * \param imageList The darker details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::SetInfFilter(InputImageListType * imageList)
{
  this->SetNthInput(2, const_cast<InputImageListType *>(imageList));
}

/**
  * Set The SupDeci details
  * \param imageList The brighter details extracted from the filtering operation.
  */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::SetSupDeci(InputImageListType * imageList)
{
  this->SetNthInput(3, const_cast<InputImageListType *>(imageList));
}
/**
 * Set The InfDeci details
 * \param imageList The darker details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::SetInfDeci(InputImageListType * imageList)
{
  this->SetNthInput(4, const_cast<InputImageListType *>(imageList));
}

/**
 * Get The SupFilter details
 * \return The brighter details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::InputImageListType*
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::GetSupFilter(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(1));
}
/**
 * Get The InfFilter details
 * \return The darker details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::InputImageListType*
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::GetInfFilter(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(2));
}
/**
 * Get The SupDeci details
 * \return The brighter details extracted from the resampling operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::InputImageListType*
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::GetSupDeci(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(3));
}
/**
 * Get The InfDeci details
 * \return The darker details extracted from the resampling operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::InputImageListType*
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::GetInfDeci(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(4));
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::GenerateData(void)
{
  otbMsgDevMacro(<< "MorphologicalPyramidSynthesisFilter : Entering main method.");
  // Input image pointer
  InputImageListType * supFilter = this->GetSupFilter();
  InputImageListType * infFilter = this->GetInfFilter();
  InputImageListType * supDeci = this->GetSupDeci();
  InputImageListType * infDeci = this->GetInfDeci();

  // Output pointer
  OutputImageListType * OutputImageList   = this->GetOutput();

  // typedefs of the filters
  typedef itk::SubtractImageFilter<InputImageType, InputImageType, OutputImageType> SubtractFilterType;
  typedef itk::AddImageFilter<InputImageType, InputImageType, InputImageType>       AddFilterType;
  typedef otb::MorphologicalPyramid::Resampler<InputImageType, InputImageType>      ResamplerType;
  typedef itk::ImageDuplicator<InputImageType>                                      DuplicatorType;

  // Input Image duplication to the currentImage Pointer
  typename DuplicatorType::Pointer duplicator = DuplicatorType::New();
  duplicator->SetInputImage(this->GetInput());
  duplicator->Update();

  // Input Image duplication to the currentImage Pointer
  typename InputImageType::Pointer currentImage = duplicator->GetOutput();

  // Filters declarations
  typename AddFilterType::Pointer      add1, add2;
  typename SubtractFilterType::Pointer subtract1, subtract2;
  typename ResamplerType::Pointer      resampler;

  // Size vector computation
  SizeVectorType size;

  ImageListIterator it = supFilter->Begin();

  while (it != supFilter->End())
    {
    size.push_back(it.Get()->GetLargestPossibleRegion().GetSize());
    ++it;
    }
  otbMsgDevMacro(<< "MorphologicalPyramidSynthesisFilter : Size vector computation OK");

  // Iterators definition
  ImageListReverseIterator itinfFilter = infFilter->ReverseBegin();
  ImageListReverseIterator itsupFilter = supFilter->ReverseBegin();
  ImageListReverseIterator itinfDeci = infDeci->ReverseBegin();
  ImageListReverseIterator itsupDeci = supDeci->ReverseBegin();
  SizeReverseIterator      itSize = size.rbegin();

  int i = 1;
//--------------------------------------------------------//
//                      Main loop                         //
//--------------------------------------------------------//
  while (itinfFilter != infFilter->ReverseEnd()
         && itsupFilter != supFilter->ReverseEnd()
         && itinfDeci != infDeci->ReverseEnd()
         && itsupDeci != supDeci->ReverseEnd()
         && itSize != size.rend())
    {

    ++i;
    // Upsampling current image
    resampler = ResamplerType::New();
    resampler->SetSize(*itSize);
    resampler->SetInput(currentImage);
    resampler->Update();
    otbMsgDevMacro(<< "MorphologicalPyramidSynthesisFilter: step " << i << " Upsampling OK");
    // Adding *sup details from current level
    add1 = AddFilterType::New();
    add1->SetInput1(resampler->GetOutput());
    add1->SetInput2(itsupFilter.Get());
    add2 = AddFilterType::New();
    add2->SetInput1(add1->GetOutput());
    add2->SetInput2(itsupDeci.Get());

    // Adding *inf details from current level
    subtract1 = SubtractFilterType::New();
    subtract1->SetInput1(add2->GetOutput());
    subtract1->SetInput2(itinfFilter.Get());
    subtract2 = SubtractFilterType::New();
    subtract2->SetInput1(subtract1->GetOutput());
    subtract2->SetInput2(itinfDeci.Get());
    subtract2->Update();
    otbMsgDevMacro(<< "MorphologicalPyramidSynthesisFilter: step " << i << " Details addition OK");

    // Updating current image
    currentImage = subtract2->GetOutput();
    OutputImageList->PushBack(currentImage);

    // Iterators incrementation
    ++itsupFilter;
    ++itinfFilter;
    ++itsupDeci;
    ++itinfDeci;
    ++itSize;
    }
  otbMsgDevMacro(<< "MorphologicalPyramidSynthesisFilter: Exiting main method.");
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb

#endif
