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
#ifndef __otbMorphologicalPyramidAnalysisFilter_txx
#define __otbMorphologicalPyramidAnalysisFilter_txx

#include "otbMorphologicalPyramidAnalysisFilter.h"

#include "itkSubtractImageFilter.h"
#include "itkMaximumImageFilter.h"
#include "itkImageDuplicator.h"
#include "otbMorphologicalPyramidResampler.h"
#include "itkProgressAccumulator.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::MorphologicalPyramidAnalysisFilter()
{
  this->SetNumberOfRequiredOutputs(2);
  m_DecimationRatio = 2.0;
  m_NumberOfLevels = 4;
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
 * Destructor
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::~MorphologicalPyramidAnalysisFilter() {}
/**
 * Get The Analyse image at each level of the pyramid.
 * \return The analysed image at each level of the pyramid.
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
typename MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::OutputImageListType*
MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::GetOutput(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(2));
}
/**
 * Get The SupFilter details
 * \return The brighter details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
typename MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::OutputImageListType*
MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::GetSupFilter(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(0));
}
/**
 * Get The InfFilter details
 * \return The darker details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
typename MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::OutputImageListType*
MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::GetInfFilter(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(1));
}
/**
 * Get The SupDeci details
 * \return The brighter details extracted from the resampling operation.
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
typename MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::OutputImageListType*
MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::GetSupDeci(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(3));
}
/**
 * Get The InfDeci details
 * \return The brighter details extracted from the resampling operation.
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
typename MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::OutputImageListType*
MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::GetInfDeci(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(4));
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
void
MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::GenerateData(void)
{
  // Input image pointer
  OutputImageListType * OutputImageList   = this->GetOutput();

  // Output images pointers
  OutputImageListType * supFilter = this->GetSupFilter();
  OutputImageListType * infFilter = this->GetInfFilter();
  OutputImageListType * supDeci = this->GetSupDeci();
  OutputImageListType * infDeci = this->GetInfDeci();

  // typedefs of the filters
  typedef itk::SubtractImageFilter<InputImageType, InputImageType, OutputImageType> SubtractFilterType;
  typedef itk::MaximumImageFilter<InputImageType, InputImageType, InputImageType>   MaxFilterType;
  typedef itk::ImageDuplicator<InputImageType>                                      DuplicatorType;
  typedef otb::MorphologicalPyramid::Resampler<InputImageType, OutputImageType>     ResamplerType;

  // Input Image duplication to the currentImage Pointer
  typename DuplicatorType::Pointer duplicator = DuplicatorType::New();
  duplicator->SetInputImage(this->GetInput());
  duplicator->Update();
  typename InputImageType::Pointer currentImage = duplicator->GetOutput();
  typename InputImageType::Pointer upsampled;

  // Structuring element size computation
  const int structElementDimension = static_cast<int>(vcl_ceil(this->GetDecimationRatio() / 2.));

  // Structuring element creation
  KernelType structuringElement;
  structuringElement.SetRadius(structElementDimension);
  structuringElement.CreateStructuringElement();

  // Filters declarations
  typename MorphoFilterType::Pointer   morphoFilter;
  typename MaxFilterType::Pointer      max;
  typename SubtractFilterType::Pointer subtract1, subtract2, subtract3, subtract4;
  typename ResamplerType::Pointer      resampler1, resampler2;

  // Size declaration
  typename InputImageType::SizeType size;

  // local variables declarations and initializations
  int i = 0;
  int sizeTmp;
  //--------------------------------------------------------//
  //                      Main loop                         //
  //--------------------------------------------------------//
  // While the number of iterations is not reached
  otbMsgDevMacro(<< "Entering main loop");
  while (i < this->GetNumberOfLevels())
    {

    // morphological filtering
    morphoFilter = MorphoFilterType::New();
    morphoFilter->SetKernel(structuringElement);
    morphoFilter->SetInput(currentImage);
    morphoFilter->Update();

    // Maximum between current and filtered image
    max = MaxFilterType::New();
    max->SetInput1(morphoFilter->GetOutput());
    max->SetInput2(currentImage);
    max->Update();

    // SupFilter detail image computation
    subtract1 = SubtractFilterType::New();
    subtract1->SetInput1(max->GetOutput());
    subtract1->SetInput2(morphoFilter->GetOutput());
    subtract1->Update();
    otbMsgDevMacro(
      << "MorphologicalPyramidAnalysisFilter: subtract1 OK " <<
      subtract1->GetOutput()->GetLargestPossibleRegion().GetSize());
    supFilter->PushBack(subtract1->GetOutput());
    otbMsgDevMacro("MorphologicalPyramidAnalysisFilter: step " << i << " - Image appended to SupFilter");

    // InfFilter detail image computation
    subtract2 = SubtractFilterType::New();
    subtract2->SetInput1(max->GetOutput());
    subtract2->SetInput2(currentImage);
    subtract2->Update();
    otbMsgDevMacro(
      << "MorphologicalPyramidAnalysisFilter: subtract2 OK " <<
      subtract2->GetOutput()->GetLargestPossibleRegion().GetSize());
    infFilter->PushBack(subtract2->GetOutput());
    otbMsgDevMacro("MorphologicalPyramidAnalysisFilter: step " << i << " - Image appended to InfFilter");

    // New  Size
    size = morphoFilter->GetOutput()->GetLargestPossibleRegion().GetSize();
    for (int j = 0; j < InputImageType::ImageDimension; ++j)
      {
      sizeTmp = size[j];
      // As we knwow that our values will always be positive ones, we can simulate round by ceil(value+0.5)
      size[j] = static_cast<unsigned int>(vcl_ceil((static_cast<double>(sizeTmp) / this->GetDecimationRatio()) + 0.5));
      }
    otbMsgDevMacro(<< "New size: " << size);

    // Image subsampling
    // Current image becomes the newly subsampled image
    resampler1 = ResamplerType::New();
    resampler1->SetInput(morphoFilter->GetOutput());
    resampler1->SetSize(size);
    resampler1->Update();
    currentImage = resampler1->GetOutput();

    otbMsgDevMacro(
      << "MorphologicalPyramidAnalysisFilter: DownSampling OK " << currentImage->GetLargestPossibleRegion().GetSize());
    // New current image is appeneded to the output list
    OutputImageList->PushBack(currentImage);

    // Image upsampling
    resampler2 = ResamplerType::New();
    resampler2->SetInput(resampler1->GetOutput());
    resampler2->SetSize(morphoFilter->GetOutput()->GetLargestPossibleRegion().GetSize());
    resampler2->Update();

    otbMsgDevMacro(
      << "MorphologicalPyramidAnalysisFilter: UpSampling OK " <<
      resampler2->GetOutput()->GetLargestPossibleRegion().GetSize());
    // Computation of the details lost in the subsampling operation
    max = MaxFilterType::New();
    max->SetInput1(morphoFilter->GetOutput());
    max->SetInput2(resampler2->GetOutput());
    max->Update();
    otbMsgDevMacro(
      << "MorphologicalPyramidAnalysisFilter: Max OK " << max->GetOutput()->GetLargestPossibleRegion().GetSize());

    // InfDeci detail image computation
    subtract4 = SubtractFilterType::New();
    subtract4->SetInput1(max->GetOutput());
    subtract4->SetInput2(morphoFilter->GetOutput());
    subtract4->Update();
    otbMsgDevMacro(
      << "MorphologicalPyramidAnalysisFilter: subtract4 OK " <<
      subtract4->GetOutput()->GetLargestPossibleRegion().GetSize());
    infDeci->PushBack(subtract4->GetOutput());
    otbMsgDevMacro("MorphologicalPyramidAnalysisFilter: step " << i << " - Image appended to InfDeci");

    // SupDeci detail image computation
    subtract3 = SubtractFilterType::New();
    subtract3->SetInput1(max->GetOutput());
    subtract3->SetInput2(resampler2->GetOutput());
    subtract3->Update();
    otbMsgDevMacro(
      << "MorphologicalPyramidAnalysisFilter: subtract3 OK " <<
      subtract3->GetOutput()->GetLargestPossibleRegion().GetSize());
    supDeci->PushBack(subtract3->GetOutput());
    otbMsgDevMacro("MorphologicalPyramidAnalysisFilter: step " << i << " - Image appended to SupDeci");

    // Iteration ounter incrementation
    ++i;
    }
  otbMsgDevMacro(<< "Exiting main loop");
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputImage, class TMorphoFilter>
void
MorphologicalPyramidAnalysisFilter<TInputImage, TOutputImage, TMorphoFilter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "DecimationRatio: " << m_DecimationRatio << std::endl;
  os << indent << "NumberOfLevels: " << m_NumberOfLevels << std::endl;
}
} // End namespace otb
#endif
