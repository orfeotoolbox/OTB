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
#ifndef _otbMorphologicalPyramidSynthesisFilter_txx
#define _otbMorphologicalPyramidSynthesisFilter_txx

#include "otbMorphologicalPyramidSynthesisFilter.h"

#include "otbMorphologicalPyramidResampler.h"
#include "itkSubtractImageFilter.h"
#include "itkAddImageFilter.h"
#include  "itkImageDuplicator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::MorphologicalPyramidSynthesisFilter() {}
/**
 * Destructor
 */
template <class TInputImage, class TOutputImage>
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::~MorphologicalPyramidSynthesisFilter() {}
/**
 * Get the vector of sizes
 * \param size The vector of sizes
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::SetSize(std::vector<typename TOutputImage::SizeType> size)
{
  m_Size=size;
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::GenerateData(void)
{
  // Input image pointer
  OutputImageListType *   OutputImageList   = this->GetOutput();
  // typedefs of the filters
  typedef itk::SubtractImageFilter<InputImageType,InputImageType,OutputImageType> SubtractFilterType;
  typedef itk::AddImageFilter<InputImageType,InputImageType,InputImageType> AddFilterType;
  typedef otb::morphologicalPyramid::Resampler<InputImageType,InputImageType> ResamplerType;
  typedef itk::ImageDuplicator<InputImageType> DuplicatorType;
  
  // Input Image duplication to the currentImage Pointer 
  typename DuplicatorType::Pointer duplicator = DuplicatorType::New();
  duplicator->SetInputImage(this->GetInput());
  duplicator->Update();

  // Input Image duplication to the currentImage Pointer 
  typename InputImageType::Pointer currentImage =duplicator->GetOutput();
  
  // Filters declarations
  typename AddFilterType::Pointer add1, add2;
  typename SubtractFilterType::Pointer subtract1,subtract2;
  typename ResamplerType::Pointer resampler;

  // Iterators definition  
  ImageListReverseIterator itInfFiltre = m_InfFiltre->ReverseBegin();
  ImageListReverseIterator itSupFiltre = m_SupFiltre->ReverseBegin();
  ImageListReverseIterator itInfDeci = m_InfDeci->ReverseBegin();
  ImageListReverseIterator itSupDeci = m_SupDeci->ReverseBegin();
  SizeReverseIterator itSize = m_Size.rbegin();

 //--------------------------------------------------------//
  //                      Main loop                         //
  //--------------------------------------------------------//
  while(   itInfFiltre!=m_InfFiltre->ReverseEnd()
	&& itSupFiltre!=m_SupFiltre->ReverseEnd()
	&& itInfDeci!=m_InfDeci->ReverseEnd()
	&& itSupDeci!=m_SupDeci->ReverseEnd()
	&& itSize!=m_Size.rend())
    {
    
      // Upsampling current image
    resampler = ResamplerType::New();
    resampler->SetSize(*itSize);
    resampler->SetInput(currentImage);

    // Adding *Sup details from current level
    add1= AddFilterType::New();
    add1->SetInput1(resampler->GetOutput());
    add1->SetInput2(itSupFiltre.Get());
    add2= AddFilterType::New();
    add2->SetInput1(add1->GetOutput());
    add2->SetInput2(itSupDeci.Get());

    // Adding *Inf details from current level
    subtract1= SubtractFilterType::New();
    subtract1->SetInput1(add2->GetOutput());
    subtract1->SetInput2(itInfFiltre.Get());
    subtract2= SubtractFilterType::New();
    subtract2->SetInput1(subtract1->GetOutput());
    subtract2->SetInput2(itInfDeci.Get());
    subtract2->Update();
    
    // Updating current image
    currentImage=subtract2->GetOutput();
    OutputImageList->PushBack(currentImage);
    
    // Iterators incrementation
    ++itSupFiltre;
    ++itInfFiltre;
    ++itSupDeci;
    ++itInfDeci;
    ++itSize;
    } 
  }
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb

#endif

