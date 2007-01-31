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
#ifndef _otbMorphologicalPyramidAnalyseFilter_txx
#define _otbMorphologicalPyramidAnalyseFilter_txx

#include <math.h>

#include "otbMorphologicalPyramidAnalyseFilter.h"

#include "itkSubtractImageFilter.h"
#include "itkMaximumImageFilter.h"
#include  "itkImageDuplicator.h"
#include "otbMorphologicalPyramidResampler.h"
#include "itkProgressAccumulator.h"
#include "otbMacro.h"

namespace otb
{
  /**
   * Constructor
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage, TMorphoFilter>
  ::MorphologicalPyramidAnalyseFilter()
  {
    this->SetNumberOfRequiredOutputs(2);
    m_SubSampleScale = 2.0;
    m_NumberOfIterations = 4;
    OutputImageListPointerType supFiltre = OutputImageListType::New();
    this->SetNthOutput(0,supFiltre.GetPointer());
    OutputImageListPointerType infFiltre = OutputImageListType::New();
    this->SetNthOutput(1,infFiltre.GetPointer());
    OutputImageListPointerType outputList = OutputImageListType::New();
    this->SetNthOutput(2,outputList.GetPointer());
    OutputImageListPointerType supDeci =   OutputImageListType::New();
    this->SetNthOutput(3,supDeci.GetPointer());
    OutputImageListPointerType infDeci =   OutputImageListType::New();
    this->SetNthOutput(4,infDeci.GetPointer());
  }
  /**
   * Destructor
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::~MorphologicalPyramidAnalyseFilter(){}
  /**
   * Get The Analyse image at each level of the pyramid.
   * \return The analysed image at each level of the pyramid.
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  typename MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::OutputImageListType*
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::GetOutput(void)
  {
    return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(2));
  }  
  /**
   * Get The SupFiltre details
   * \return The brighter details extracted from the filtering operation.
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  typename MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::OutputImageListType*
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::GetSupFiltre(void)
  {
    return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(0));
  }
  /**
   * Get The InfFiltre details
   * \return The darker details extracted from the filtering operation.
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  typename MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::OutputImageListType*
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::GetInfFiltre(void)
  {
    return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(1));
  }
  /**
   * Get The SupDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  typename MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::OutputImageListType*
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::GetSupDeci(void)
  {
    return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(3));
  }
  /**
   * Get The InfDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  typename MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::OutputImageListType*
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::GetInfDeci(void)
  {
    return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(4));
  }
  /**
   * Main computation method
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  void
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::GenerateData(void)
  { 
    // Input image pointer
    OutputImageListType *   OutputImageList   = this->GetOutput();

    // Output images pointers
    OutputImageListType * supFiltre = this->GetSupFiltre();
    OutputImageListType * infFiltre = this->GetInfFiltre();
    OutputImageListType * supDeci = this->GetSupDeci();
    OutputImageListType * infDeci = this->GetInfDeci();

    // typedefs of the filters
    typedef itk::SubtractImageFilter<InputImageType,InputImageType,OutputImageType> SubtractFilterType;
    typedef itk::MaximumImageFilter<InputImageType,InputImageType,InputImageType> MaxFilterType;
    typedef itk::ImageDuplicator<InputImageType> DuplicatorType;
    typedef otb::morphologicalPyramid::Resampler<InputImageType,OutputImageType> ResamplerType;  

    // Input Image duplication to the currentImage Pointer 
    typename DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(this->GetInput());
    duplicator->Update();
    typename InputImageType::Pointer currentImage =duplicator->GetOutput();
    typename InputImageType::Pointer upsampled;

    // Structuring element size computation
    const int structElementDimension=static_cast<int>(ceil(static_cast<double>(this->GetSubSampleScale()/2.)));
  
    // Structuring element creation
    KernelType structuringElement;
    structuringElement.SetRadius(structElementDimension);
    structuringElement.CreateStructuringElement();
  
    // Filters declarations
    typename MorphoFilterType::Pointer morphoFilter;
    typename MaxFilterType::Pointer max;
    typename SubtractFilterType::Pointer subtract1,subtract2,subtract3,subtract4;
    typename ResamplerType::Pointer resampler1, resampler2;
  
    // Size declaration
    typename InputImageType::SizeType size;

    // local variables declarations and initialisations
    int i = 0;
    int sizeTmp;
    //--------------------------------------------------------//
    //                      Main loop                         //
    //--------------------------------------------------------//
    // While the number of iterations is not reached
    otbMsgDebugMacro(<<"Entering main loop");
    while(i<this->GetNumberOfIterations())
      {

	// morphological filtering
	morphoFilter = MorphoFilterType::New();
	morphoFilter->SetKernel(structuringElement);
	morphoFilter->SetInput(currentImage);
	morphoFilter->Update();

	// Maximum between current and filtered image
	max=MaxFilterType::New();
	max->SetInput1(morphoFilter->GetOutput());
	max->SetInput2(currentImage);
	max->Update();

	// SupFiltre detail image computation
	subtract1 = SubtractFilterType::New();
	subtract1->SetInput1(max->GetOutput());
	subtract1->SetInput2(morphoFilter->GetOutput());
	subtract1->Update();
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: subtract1 OK "<<subtract1->GetOutput()->GetLargestPossibleRegion().GetSize());
	supFiltre->PushBack(subtract1->GetOutput());
	otbMsgDebugMacro("MorphologicalPyramidAnalyseFilter: step "<<i<<" - Image appended to SupFiltre");

	// InfFiltre detail image computation
	subtract2 = SubtractFilterType::New();
	subtract2->SetInput1(max->GetOutput());
	subtract2->SetInput2(currentImage);
	subtract2->Update();
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: subtract2 OK "<<subtract2->GetOutput()->GetLargestPossibleRegion().GetSize());
	infFiltre->PushBack(subtract2->GetOutput());
	otbMsgDebugMacro("MorphologicalPyramidAnalyseFilter: step "<<i<<" - Image appended to InfFiltre");
	
	// New  Size
	size = morphoFilter->GetOutput()->GetLargestPossibleRegion().GetSize();
	for (int j =0; j<InputImageType::ImageDimension;j++)
	  {
	    sizeTmp=size[j];
	    // As we knwow that our values will always be positive ones, we can simulate round by ceil(value+0.5)
	    size[j]=static_cast<unsigned int>(ceil((static_cast<double>(sizeTmp)/this->GetSubSampleScale())+0.5));
	  }
	otbMsgDebugMacro(<<"New size: "<<size);
    
	// Image subsampling 
	// Current image becomes the newly subsampled image
	resampler1 = ResamplerType::New();
	resampler1->SetInput(morphoFilter->GetOutput());
	resampler1->SetSize(size);
	resampler1->Update();
	currentImage=resampler1->GetOutput();
	
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: DownSampling OK "<<currentImage->GetLargestPossibleRegion().GetSize());
	// New current image is appeneded to the output list
	OutputImageList->PushBack(currentImage);

	// Image upsampling
	resampler2 = ResamplerType::New();
	resampler2->SetInput(resampler1->GetOutput());
	resampler2->SetSize(morphoFilter->GetOutput()->GetLargestPossibleRegion().GetSize());
	resampler2->Update();

	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: UpSampling OK "<<resampler2->GetOutput()->GetLargestPossibleRegion().GetSize());
	// Computation of the details lost in the subsampling operation
	max=MaxFilterType::New();
	max->SetInput1(morphoFilter->GetOutput());
	max->SetInput2(resampler2->GetOutput());
	max->Update();
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: Max OK "<<max->GetOutput()->GetLargestPossibleRegion().GetSize());

	// InfDeci detail image computation
	subtract4 = SubtractFilterType::New();
	subtract4->SetInput1(max->GetOutput());
	subtract4->SetInput2(morphoFilter->GetOutput());
	subtract4->Update();
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: subtract4 OK "<<subtract4->GetOutput()->GetLargestPossibleRegion().GetSize());
	infDeci->PushBack(subtract4->GetOutput());
	otbMsgDebugMacro("MorphologicalPyramidAnalyseFilter: step "<<i<<" - Image appended to InfDeci");

	// SupDeci detail image computation
	subtract3 = SubtractFilterType::New();
	subtract3->SetInput1(max->GetOutput());
	subtract3->SetInput2(resampler2->GetOutput());
	subtract3->Update();
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: subtract3 OK "<<subtract3->GetOutput()->GetLargestPossibleRegion().GetSize());
	supDeci->PushBack(subtract3->GetOutput());
	otbMsgDebugMacro("MorphologicalPyramidAnalyseFilter: step "<<i<<" - Image appended to SupDeci");

	// Iteration ounter incrementation
	i++;
      }
    otbMsgDebugMacro(<<"Exiting main loop");
  }
  /**
   * PrintSelf method
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  void
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "SubSampleScale: " << m_SubSampleScale << std::endl;
    os << indent << "NumberOfIterations: " << m_NumberOfIterations << std::endl;
  }
} // End namespace otb
#endif
