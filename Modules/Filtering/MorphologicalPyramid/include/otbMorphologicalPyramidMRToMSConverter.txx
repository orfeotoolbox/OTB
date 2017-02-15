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
#ifndef otbMorphologicalPyramidMRToMSConverter_txx
#define otbMorphologicalPyramidMRToMSConverter_txx

#include "otbMorphologicalPyramidMRToMSConverter.h"
#include "otbMorphologicalPyramidResampler.h"
#include "otbMacro.h"

namespace otb
{
namespace MorphologicalPyramid
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
MRToMSConverter<TInputImage, TOutputImage>
::MRToMSConverter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(2);
  OutputImageListPointerType supFilter = OutputImageListType::New();
  this->SetNthOutput(0, supFilter.GetPointer());
  OutputImageListPointerType infFilter = OutputImageListType::New();
  this->SetNthOutput(1, infFilter.GetPointer());
  OutputImageListPointerType outputList = OutputImageListType::New();
  this->SetNthOutput(2, outputList.GetPointer());
  OutputImageListPointerType supDeci =   OutputImageListType::New();
  this->SetNthOutput(3, supDeci.GetPointer());
  OutputImageListPointerType infDeci =   OutputImageListType::New();
  this->SetNthOutput(4, infDeci.GetPointer());
}
/**
 * Get The inputList at full resolution.
 * \return The analysed image at each pyramid level
 * resampled at full resolution.
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::OutputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetOutput(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(2));
}
/**
 * Get The SupFilter details at full resolution.
 * \return The brighter details extracted from the filtering operation.
 * resampled at full resolution.
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::OutputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetSupFilterFullResolution(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(0));
}
/**
 * Get The InfFilter details at full resolution.
 * \return The darker details extracted from the filtering operation.
 * resampled at full resolution.
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::OutputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetInfFilterFullResolution(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(1));
}
/**
 * Get The SupDeci details at full resolution.
 * \return The brighter details extracted from the resampling operation.
 * resampled at full resolution.
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::OutputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetSupDeciFullResolution(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(3));
}
/**
 * Get The InfDeci details  at full resolution.
 * \return The brighter details extracted from the resampling operation.
 * resampled at full resolution.
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::OutputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetInfDeciFullResolution(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(4));
}
/**
 * Set The input list.
 * \param imageList The analysed image at each level of the pyramid.
 */
template <class TInputImage, class TOutputImage>
void
MRToMSConverter<TInputImage, TOutputImage>
::SetInput(const InputImageListType * imageList)
{
  this->SetNthInput(2, const_cast<InputImageListType *>(imageList));
}
/**
 * Set The SupFilter details
 * \param imageList The brighter details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
void
MRToMSConverter<TInputImage, TOutputImage>
::SetSupFilter(InputImageListType * imageList)
{
  this->SetNthInput(0, const_cast<InputImageListType *>(imageList));
}
/**
 * Set The InfFilter details
 * \param imageList The darker details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
void
MRToMSConverter<TInputImage, TOutputImage>
::SetInfFilter(InputImageListType * imageList)
{
  this->SetNthInput(1, const_cast<InputImageListType *>(imageList));
}
/**
 * Set The SupDeci details
 * \param imageList The brighter details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
void
MRToMSConverter<TInputImage, TOutputImage>
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
MRToMSConverter<TInputImage, TOutputImage>
::SetInfDeci(InputImageListType * imageList)
{
  this->SetNthInput(4, const_cast<InputImageListType *>(imageList));
}
/**
 * Get the input list.
 * \return The list of the analysed image at each pyramid level.
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::InputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetInput(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(2));
}
/**
 * Get The SupFilter details.
 * \return The brighter details extracted from the filtering operation
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::InputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetSupFilter(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(0));
}
/**
 * Get The InfFilter details.
 * \return The darker details extracted from the filtering operation
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::InputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetInfFilter(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(1));
}
/**
 * Get The SupDeci details.
 * \return The brighter details extracted from the resampling operation
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::InputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetSupDeci(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(3));
}
/**
 * Get The InfDeci details.
 * \return The darker details extracted from the resampling operation.
 */
template <class TInputImage, class TOutputImage>
typename MRToMSConverter<TInputImage, TOutputImage>
::InputImageListType*
MRToMSConverter<TInputImage, TOutputImage>
::GetInfDeci(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(4));
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void
MRToMSConverter<TInputImage, TOutputImage>
::GenerateData()
{
  // Input images lists pointers
  InputImageListPointerType supFilter = this->GetSupFilter();
  InputImageListPointerType infFilter = this->GetInfFilter();
  InputImageListPointerType supDeci = this->GetSupDeci();
  InputImageListPointerType infDeci = this->GetInfDeci();
  InputImageListPointerType inputList = this->GetInput();

  // Output images lists pointers
  OutputImageListPointerType supFilterFullResolution = this->GetSupFilterFullResolution();
  OutputImageListPointerType infFilterFullResolution = this->GetInfFilterFullResolution();
  OutputImageListPointerType supDeciFullResolution = this->GetSupDeciFullResolution();
  OutputImageListPointerType infDeciFullResolution = this->GetInfDeciFullResolution();
  OutputImageListPointerType outputList = this->GetOutput();

  // typedef of the resampling filter
  typedef otb::MorphologicalPyramid::Resampler<InputImageType, OutputImageType> ResamplerType;

  // Definition of the resampler filters
  typename ResamplerType::Pointer resampler;

  // Full resolution size
  typename InputImageType::SizeType frsize = supFilter->Front()->GetLargestPossibleRegion().GetSize();
  otbMsgDevMacro(<< "MRToMSConverter: Full resolution size: " << frsize);

  // SupFilter resampling
  otbMsgDevMacro(<< "MRToMSConverter: SupFilter resampling.");
  InputImageListIteratorType it = supFilter->Begin();
  // The first image does not need any resampling
  supFilterFullResolution->PushBack(it.Get());
  ++it;
  for (; it != supFilter->End(); ++it)
    {
    resampler = ResamplerType::New();
    resampler->SetSize(frsize);
    resampler->SetInput(it.Get());
    resampler->Update();
    supFilterFullResolution->PushBack(resampler->GetOutput());
    }
  otbMsgDevMacro(<< "MRToMSConverter: InfFilter resampling.");
  // InfFilter resampling
  it = infFilter->Begin();
  // The first image does not need any resampling
  infFilterFullResolution->PushBack(it.Get());
  ++it;
  for (; it != infFilter->End(); ++it)
    {
    resampler = ResamplerType::New();
    resampler->SetSize(frsize);
    resampler->SetInput(it.Get());
    resampler->Update();
    infFilterFullResolution->PushBack(resampler->GetOutput());
    }

  if (inputList)
    {
    otbMsgDevMacro(<< "MRToMSConverter: Optional inputList resampling.");
    // Analyse image resampling
    it = inputList->Begin();
    // The first image does not need any resampling
    outputList->PushBack(it.Get());
    ++it;
    for (; it != inputList->End(); ++it)
      {
      resampler = ResamplerType::New();
      resampler->SetSize(frsize);
      resampler->SetInput(it.Get());
      resampler->Update();
      outputList->PushBack(resampler->GetOutput());
      }
    }

  if (supDeci)
    {
    otbMsgDevMacro(<< "MRToMSConverter: Optional supDeci resampling.");
    // SupDeci resampling
    it = supDeci->Begin();
    // The first image does not need any resampling
    supDeciFullResolution->PushBack(it.Get());
    ++it;
    for (; it != supDeci->End(); ++it)
      {
      resampler = ResamplerType::New();
      resampler->SetSize(frsize);
      resampler->SetInput(it.Get());
      resampler->Update();
      supDeciFullResolution->PushBack(resampler->GetOutput());
      }
    }

  if (infDeci)
    {
    otbMsgDevMacro(<< "MRToMSConverter: Optional infDeci resampling.");
    // InfDeci resampling
    it = infDeci->Begin();
    // The first image does not need any resampling
    infDeciFullResolution->PushBack(it.Get());
    ++it;
    for (; it != infDeci->End(); ++it)
      {
      resampler = ResamplerType::New();
      resampler->SetSize(frsize);
      resampler->SetInput(it.Get());
      resampler->Update();
      infDeciFullResolution->PushBack(resampler->GetOutput());
      }
    }
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputImage>
void
MRToMSConverter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace MorphologicalPyramid
} // End namespace otb
#endif
