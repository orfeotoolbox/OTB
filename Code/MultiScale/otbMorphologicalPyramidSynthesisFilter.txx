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
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::MorphologicalPyramidSynthesisFilter() 
{
  this->SetNumberOfRequiredInputs(5);
}
/**
 * Destructor
 */
template <class TInputImage, class TOutputImage>
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::~MorphologicalPyramidSynthesisFilter() {}
/**
 * Set The SupFiltre details
 * \param imageList The brighter details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
void 
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::SetSupFiltre(InputImageListType * imageList)
{
  this->SetNthInput(1,const_cast<InputImageListType *>(imageList));
}
  /**
   * Set The InfFiltre details
   * \param imageList The darker details extracted from the filtering operation.
   */
template <class TInputImage, class TOutputImage>
void 
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::SetInfFiltre(InputImageListType * imageList)
{
this->SetNthInput(2,const_cast<InputImageListType *>(imageList));
}

 /**
   * Set The SupDeci details
   * \param imageList The brighter details extracted from the filtering operation.
   */
template <class TInputImage, class TOutputImage>
void 
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::SetSupDeci(InputImageListType * imageList)
{
this->SetNthInput(3,const_cast<InputImageListType *>(imageList));
}
  /**
   * Set The InfDeci details
   * \param imageList The darker details extracted from the filtering operation.
   */
template <class TInputImage, class TOutputImage>
void 
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::SetInfDeci(InputImageListType * imageList)
{
this->SetNthInput(4,const_cast<InputImageListType *>(imageList));
}

/**
 * Get The SupFiltre details
 * \return The brighter details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::InputImageListType*
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::GetSupFiltre(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(1));
}
/**
 * Get The InfFiltre details
 * \return The darker details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::InputImageListType*
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::GetInfFiltre(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(2));
}
/**
 * Get The SupDeci details
 * \return The brighter details extracted from the resampling operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::InputImageListType*
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::GetSupDeci(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(3));
}
/**
 * Get The InfDeci details
 * \return The darker details extracted from the resampling operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::InputImageListType*
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::GetInfDeci(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(4));
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSynthesisFilter<TInputImage,TOutputImage>
::GenerateData(void)
{
  otbMsgDebugMacro(<<"MorphologicalPyramidSynthesisFilter : Entering main method.");
  // Input image pointer
  InputImageListType * supFiltre = this->GetSupFiltre();
  InputImageListType * infFiltre = this->GetInfFiltre();
  InputImageListType * supDeci = this->GetSupDeci();
  InputImageListType * infDeci = this->GetInfDeci();
  
  // Output pointer
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


  // Size vector computation
  SizeVectorType size;

  ImageListIterator it = supFiltre->Begin();
  
  while(it!=supFiltre->End())
    {
      size.push_back(it.Get()->GetLargestPossibleRegion().GetSize());
      ++it;
    }
  otbMsgDebugMacro(<<"MorphologicalPyramidSynthesisFilter : Size vector computation OK");

  // Iterators definition  
  ImageListReverseIterator itinfFiltre = infFiltre->ReverseBegin();
  ImageListReverseIterator itsupFiltre = supFiltre->ReverseBegin();
  ImageListReverseIterator itinfDeci = infDeci->ReverseBegin();
  ImageListReverseIterator itsupDeci = supDeci->ReverseBegin();
  SizeReverseIterator itSize = size.rbegin();

  int i =1;
 //--------------------------------------------------------//
  //                      Main loop                         //
  //--------------------------------------------------------//
  while(   itinfFiltre!=infFiltre->ReverseEnd()
	&& itsupFiltre!=supFiltre->ReverseEnd()
	&& itinfDeci!=infDeci->ReverseEnd()
	&& itsupDeci!=supDeci->ReverseEnd()
	&& itSize!=size.rend())
    {

      ++i;
      // Upsampling current image
    resampler = ResamplerType::New();
    resampler->SetSize(*itSize);
    resampler->SetInput(currentImage);
    resampler->Update();
    otbMsgDebugMacro(<<"MorphologicalPyramidSynthesisFilter: step "<<i<<" Upsampling OK");
    // Adding *sup details from current level
    add1= AddFilterType::New();
    add1->SetInput1(resampler->GetOutput());
    add1->SetInput2(itsupFiltre.Get());
    add2= AddFilterType::New();
    add2->SetInput1(add1->GetOutput());
    add2->SetInput2(itsupDeci.Get());

    // Adding *inf details from current level
    subtract1= SubtractFilterType::New();
    subtract1->SetInput1(add2->GetOutput());
    subtract1->SetInput2(itinfFiltre.Get());
    subtract2= SubtractFilterType::New();
    subtract2->SetInput1(subtract1->GetOutput());
    subtract2->SetInput2(itinfDeci.Get());
    subtract2->Update();
    otbMsgDebugMacro(<<"MorphologicalPyramidSynthesisFilter: step "<<i<<" Details addition OK");
    
    
    // Updating current image
    currentImage=subtract2->GetOutput();
    OutputImageList->PushBack(currentImage);
    
    // Iterators incrementation
    ++itsupFiltre;
    ++itinfFiltre;
    ++itsupDeci;
    ++itinfDeci;
    ++itSize;
    } 
  otbMsgDebugMacro(<<"MorphologicalPyramidSynthesisFilter: Exiting main method.");
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

