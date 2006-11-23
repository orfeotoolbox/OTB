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

#include "otbMorphologicalPyramidAnalyseFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkMaximumImageFilter.h"
#include  "itkImageDuplicator.h"
#include "itkResampleImageFilter.h"
#include "itkIdentityTransform.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkProgressAccumulator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
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
    m_SubSampleScale = 2.0;
    m_NumberOfIterations = 4;
    m_SupFiltre = OutputImageListType::New();
    m_InfFiltre = OutputImageListType::New();
    m_SupDeci =   OutputImageListType::New();
    m_InfDeci =   OutputImageListType::New();
  }
  /**
   * Destructor
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::~MorphologicalPyramidAnalyseFilter(){}

  // template <class TInputImage, class TOutputImage, class TMorphoFilter>
  // std::vector<typename TOutputImage::SpacingType>
  // MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  // ::GetSpacing(void)
  // {
  //   return m_Spacing;
  // }
  // template <class TInputImage, class TOutputImage, class TMorphoFilter>
  // std::vector<typename TOutputImage::SizeType>
  // MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  // ::GetSize(void)
  // {
  //   return m_Size;
  // }

  /**
   * Resample an image according to specific size and spacing.
   * \param image The image to down sample.
   * \param size The new size of the image.
   * \param spacing The new spacing of the image.
   * \return The resampled image.
   */
  template <class TInputImage, class TOutputImage, class TMorphoFilter>
  typename MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::InputImagePointerType
  MorphologicalPyramidAnalyseFilter<TInputImage,TOutputImage,TMorphoFilter>
  ::ResampleImage(InputImagePointerType image, SizeType size, SpacingType spacing)
  {
    otbMsgDebugMacro(<<"Call to the ResampleImage method");
    // local variables
    unsigned int i;
    InputImagePointerType result;
  
    // Filters definition
    typedef itk::ResampleImageFilter<InputImageType,InputImageType> ResampleFilterType;
    typedef itk::IdentityTransform<double,InputImageType::ImageDimension> TransformType;
    typedef itk::LinearInterpolateImageFunction<InputImageType,double> InterpolatorType;
    typedef itk::ImageRegionConstIterator<InputImageType> ConstIteratorType;
    typedef itk::ImageRegionIterator<InputImageType> IteratorType;
  
    // Resampling Filter instantiation
    typename ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    typename InterpolatorType::Pointer interpolator = InterpolatorType::New();
    typename TransformType::Pointer transform = TransformType::New();
    resampler->SetTransform(transform);
    resampler->SetInterpolator(interpolator);

    // Resampling filter set up
    resampler->SetOutputSpacing(spacing);
    resampler->SetOutputOrigin(image->GetOrigin());
    resampler->SetSize(size);
    //resampler->ReleaseDataFlagOn();

    // Compute the resampled image
    resampler->SetInput(image);
    resampler->Update();
    result = resampler->GetOutput();
    otbMsgDebugMacro(<<"Resampling done.");

    // // Spacing, scale and size variables declarations 
//     // For special cases management
//     typename InputImageType::RegionType::IndexType inputStart;
//     typename InputImageType::RegionType::IndexType outputStart;
//     typename InputImageType::RegionType::SizeType  inputSize;
//     typename InputImageType::RegionType inputRegion;
//     typename InputImageType::RegionType outputRegion;

//     // Spmetimes the last line is black
//     // In this case, we simply duplicate it
//     if(spacing[0]*static_cast<float>(size[0]-1)>=(this->GetInput()->GetSpacing()[0]
// 						      *static_cast<float>(image->GetLargestPossibleRegion().GetSize()[0]-1)))
//       {
// 	inputStart[0]=size[0]-2;
// 	inputStart[1]=0;
// 	outputStart[0]=size[0]-1;
// 	outputStart[1]=0;
// 	inputSize[0]=1;
// 	inputSize[1]=size[1];
// 	inputRegion.SetSize(inputSize);
// 	outputRegion.SetSize(inputSize);
// 	inputRegion.SetIndex(inputStart);
// 	outputRegion.SetIndex(outputStart);
// 	ConstIteratorType inputIt(result, inputRegion);
// 	IteratorType outputIt(result,outputRegion);
// 	// Last line duplication
// 	for(inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();++inputIt,++outputIt)
// 	  {
// 	    outputIt.Set(inputIt.Get());
// 	  }
//       }
//     // Spmetimes the last column is black
//     // In this case, we simply duplicate it
//     if(spacing[1]*static_cast<float>(size[1]-1)>=(this->GetInput()->GetSpacing()[1]
// 						      *static_cast<float>(image->GetLargestPossibleRegion().GetSize()[1]-1)))
//       {
// 	inputStart[1]=size[1]-2;
// 	inputStart[0]=0;
// 	outputStart[1]=size[1]-1;
// 	outputStart[0]=0;
// 	inputSize[1]=1;
// 	inputSize[0]=size[0];
// 	inputRegion.SetSize(inputSize);
// 	outputRegion.SetSize(inputSize);
// 	inputRegion.SetIndex(inputStart);
// 	outputRegion.SetIndex(outputStart);
// 	ConstIteratorType inputIt(result, inputRegion);
// 	IteratorType outputIt(result,outputRegion);
// 	// Last column duplication
// 	for(inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();++inputIt,++outputIt)
// 	  {
// 	    outputIt.Set(inputIt.Get());
// 	  }
//       }
    return result;
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
    // typedefs of the filters
    typedef itk::SubtractImageFilter<InputImageType,InputImageType,OutputImageType> SubtractFilterType;
    typedef itk::MaximumImageFilter<InputImageType,InputImageType,InputImageType> MaxFilterType;
    typedef itk::ImageDuplicator<InputImageType> DuplicatorType;
  
    // Input Image duplication to the currentImage Pointer 
    typename DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(this->GetInput());
    duplicator->Update();
    typename InputImageType::Pointer currentImage =duplicator->GetOutput();
    typename InputImageType::Pointer upsampled;

    // Structuring element size computation
    const int structElementDimension=static_cast<int>(ceilf(this->GetSubSampleScale()/2.));
  
    // Structuring element creation
    KernelType structuringElement;
    structuringElement.SetRadius(structElementDimension);
    structuringElement.CreateStructuringElement();
  
    // Filters declarations
    typename MorphoFilterType::Pointer morphoFilter;
    typename MaxFilterType::Pointer max;
    typename SubtractFilterType::Pointer subtract1,subtract2,subtract3,subtract4;
  
    // Spacing and size vectors declarations
    typename InputImageType::SpacingType spacing;
    typename InputImageType::SizeType size;

    // local variables declarations and initialisations
    int i = 0;
    int j;
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
	m_SupFiltre->PushBack(subtract1->GetOutput());
	otbMsgDebugMacro("MorphologicalPyramidAnalyseFilter: step "<<i<<" - Image appended to m_SupFiltre");

	// InfFiltre detail image computation
	subtract2 = SubtractFilterType::New();
	subtract2->SetInput1(max->GetOutput());
	subtract2->SetInput2(currentImage);
	subtract2->Update();
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: subtract2 OK "<<subtract2->GetOutput()->GetLargestPossibleRegion().GetSize());
	m_InfFiltre->PushBack(subtract2->GetOutput());
	otbMsgDebugMacro("MorphologicalPyramidAnalyseFilter: step "<<i<<" - Image appended to m_InfFiltre");
	// New  Size/Spacing computation
	spacing = morphoFilter->GetOutput()->GetSpacing();
	size = morphoFilter->GetOutput()->GetLargestPossibleRegion().GetSize();
	m_Spacing.push_back(spacing);
	m_Size.push_back(size);
	otbMsgDebugMacro(<<"New size and spacing :");
	for (int j =0; j<InputImageType::ImageDimension;j++)
	  {
	    sizeTmp=size[j];
	    size[j]=static_cast<int>(round(static_cast<double>(sizeTmp)/this->GetSubSampleScale()));
	    spacing[j]=spacing[j]*((static_cast<double>(sizeTmp))/(static_cast<double>(size[j])));
	    otbMsgDebugMacro(<<" "<<size[j]<<", "<<spacing[j]);
	  }
    
	// Image subsampling 
	// Current image becomes the newly subsampled image
	currentImage = ResampleImage(morphoFilter->GetOutput(),size,spacing);
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: DownSampling OK "<<currentImage->GetLargestPossibleRegion().GetSize());
	// New current image is appeneded to the output list
	OutputImageList->PushBack(currentImage);

	// Image upsampling
	upsampled = ResampleImage(currentImage,m_Size.back(),m_Spacing.back());
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: UpSampling OK "<<upsampled->GetLargestPossibleRegion().GetSize());
	// Computation of the details lost in the subsampling operation
	max=MaxFilterType::New();
	max->SetInput1(morphoFilter->GetOutput());
	max->SetInput2(upsampled);
	max->Update();
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: Max OK "<<max->GetOutput()->GetLargestPossibleRegion().GetSize());

	// InfDeci detail image computation
	subtract4 = SubtractFilterType::New();
	subtract4->SetInput1(max->GetOutput());
	subtract4->SetInput2(morphoFilter->GetOutput());
	subtract4->Update();
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: subtract4 OK "<<subtract4->GetOutput()->GetLargestPossibleRegion().GetSize());
	m_InfDeci->PushBack(subtract4->GetOutput());
	otbMsgDebugMacro("MorphologicalPyramidAnalyseFilter: step "<<i<<" - Image appended to m_InfDeci");

	// SupDeci detail image computation
	subtract3 = SubtractFilterType::New();
	subtract3->SetInput1(max->GetOutput());
	subtract3->SetInput2(upsampled);
	subtract3->Update();
	otbMsgDebugMacro(<<"MorphologicalPyramidAnalyseFilter: subtract3 OK "<<subtract3->GetOutput()->GetLargestPossibleRegion().GetSize());
	m_SupDeci->PushBack(subtract3->GetOutput());
	otbMsgDebugMacro("MorphologicalPyramidAnalyseFilter: step "<<i<<" - Image appended to m_SupDeci");

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
