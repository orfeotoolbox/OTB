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
#ifndef __otbMorphologicalPyramidResampler_txx
#define __otbMorphologicalPyramidResampler_txx
#include "otbMorphologicalPyramidResampler.h"
#include "itkResampleImageFilter.h"
#include "itkScaleTransform.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkProgressAccumulator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

namespace otb
{
namespace morphologicalPyramid
{
/**
 * Constructor
 */
template <class TInputImage,class TOuputImage>
Resampler<TInputImage, TOuputImage>
::Resampler(){}
  /**
   * Configure input requested region to be the largest possible region.
   */
template <class TInputImage,class TOuputImage>
void
Resampler<TInputImage, TOuputImage>
::GenerateInputRequestedRegion()
{
  // Superclass method call
  Superclass::GenerateInputRequestedRegion();
  // Input and output image pointers retrieval
  InputImagePointer  inputPtr = const_cast<InputImageType *> (this->GetInput());
  OutputImagePointer outputPtr = this->GetOutput();
  // If the pointers are not correct
  if ( !inputPtr || !outputPtr )
    {
    // exit
    return;
    }
  // else
  // Configure input requested region to be the largest possible region
  inputPtr->SetRequestedRegion( inputPtr->GetLargestPossibleRegion());
}
/**
 * Configure output requested region to be the largest possible region
 */
template <class TInputImage,class TOuputImage>
void
Resampler<TInputImage, TOuputImage>
::EnlargeOutputRequestedRegion(itk::DataObject *)
{
  this->GetOutput()->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion());
}
/**
 * Generate output information
 */
template <class TInputImage, class TOutputImage>
void
Resampler<TInputImage,TOutputImage>
::GenerateOutputInformation()
{
  // Superclass method call
  Superclass::GenerateOutputInformation();
  // Input and output image pointers retrieval
  InputImageConstPointer  inputPtr  = this->GetInput();
  OutputImagePointer      outputPtr = this->GetOutput();
    // If the pointers are not correct
  if ( !inputPtr || !outputPtr )
    {
    // exit
    return;
    }
  unsigned int i;
  // Computing output spacing, size and index from input data
  const typename InputImageType::SpacingType& inputSpacing    = inputPtr->GetSpacing();
  const typename InputImageType::SizeType&    inputSize       = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename InputImageType::IndexType&   inputStartIndex = inputPtr->GetLargestPossibleRegion().GetIndex();
  typename OutputImageType::IndexType   outputStartIndex;
  typename OutputImageType::SpacingType spacing;  
   for (i = 0; i < OutputImageType::ImageDimension; i++)
     {
     outputStartIndex[i] =  inputStartIndex[i] ;
     }
  // Affectation du spacing à l'image de sortie
  outputPtr->SetSpacing( inputSpacing );
  // Génération de la région de l'image de sortie
  typename OutputImageType::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(this->GetSize());
  outputLargestPossibleRegion.SetIndex(outputStartIndex);
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);
}
/**
 * Main computation method
 */
template <class TInputImage,class TOuputImage>
void
Resampler<TInputImage, TOuputImage>
::GenerateData()
{
  unsigned int i;
  typename OutputImageType::Pointer result;

  // Filters typedefs
  typedef itk::ResampleImageFilter<InputImageType,OutputImageType> ResampleFilterType;
  typedef itk::ScaleTransform<double,InputImageType::ImageDimension> TransformType;
  typedef itk::LinearInterpolateImageFunction<InputImageType,double> InterpolatorType;
  typedef itk::ImageRegionConstIterator<OutputImageType> ConstIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType> IteratorType;
  
  // Resampling filter creation
  typename ResampleFilterType::Pointer resampler = ResampleFilterType::New();
  typename InterpolatorType::Pointer interpolator = InterpolatorType::New();
  typename TransformType::Pointer transform = TransformType::New();

  // Scale parameters computation
  typename TransformType::ParametersType scales(2);
  typename InputImageType::SizeType inputSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
  typename InputImageType::SpacingType inputSpacing = this->GetInput()->GetSpacing();
  scales[0]=static_cast<double>(inputSize[0])/static_cast<double>(m_Size[0]);
  scales[1]=static_cast<double>(inputSize[1])/static_cast<double>(m_Size[1]);
  transform->SetParameters(scales);

  // Resampling filter set up
  resampler->SetTransform(transform);
  resampler->SetInterpolator(interpolator);
  resampler->SetOutputOrigin(this->GetInput()->GetOrigin());
  resampler->SetSize(this->GetSize());
  resampler->SetOutputSpacing(inputSpacing);  
  resampler->ReleaseDataFlagOn();

  // Progress accumulator 
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(resampler, .5f);
  
  // Input image connexion
  resampler->SetInput(this->GetInput());
  resampler->Update();
  result = resampler->GetOutput();

 //  // variable definition for the following cases
//   typename OutputImageType::RegionType::IndexType inputStart;
//   typename OutputImageType::RegionType::IndexType outputStart;
//   typename OutputImageType::RegionType::SizeType  size;
//   typename OutputImageType::RegionType inputRegion;
//   typename OutputImageType::RegionType outputRegion;

//   // Gestion d'un cas particulier qui fait apparaître une bande
//   // noire sur un des bords de l'image
//   // Dans ce cas on duplique la dernière ligne de l'image
//   if(m_Spacing[0]*static_cast<float>(m_Size[0]-1)>=(this->GetInput()->GetSpacing()[0]*static_cast<float>(this->GetInput()->GetLargestPossibleRegion().GetSize()[0]-1)))
//       {
//       inputStart[0]=m_Size[0]-2;
//       inputStart[1]=0;
//       outputStart[0]=m_Size[0]-1;
//       outputStart[1]=0;
//       size[0]=1;
//       size[1]=m_Size[1];
//       inputRegion.SetSize(size);
//       outputRegion.SetSize(size);
//       inputRegion.SetIndex(inputStart);
//       outputRegion.SetIndex(outputStart);
//       ConstIteratorType inputIt(result, inputRegion);
//       IteratorType outputIt(result,outputRegion);
//       // Duplication de la dernière ligne
//       for(inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();++inputIt,++outputIt)
// 	{
// 	outputIt.Set(inputIt.Get());
// 	}      
//       }
//      // Gestion d'un cas particulier qui fait apparaître une bande
//      // noire sur un des bords de l'image
//      // Dans ce cas on duplique la dernière colonne de l'image
//      if(m_Spacing[1]*static_cast<float>(m_Size[1]-1)>=(this->GetInput()->GetSpacing()[1]*static_cast<float>(this->GetInput()->GetLargestPossibleRegion().GetSize()[1]-1)))
//       {
//       typename OutputImageType::RegionType::IndexType inputStart;
//       typename OutputImageType::RegionType::IndexType outputStart;
//       typename OutputImageType::RegionType::SizeType  size;
//       typename OutputImageType::RegionType inputRegion;
//       typename OutputImageType::RegionType outputRegion;
//       inputStart[1]=m_Size[1]-2;
//       inputStart[0]=0;
//       outputStart[1]=m_Size[1]-1;
//       outputStart[0]=0;
//       size[1]=1;
//       size[0]=m_Size[0];
//       inputRegion.SetSize(size);
//       outputRegion.SetSize(size);
//       inputRegion.SetIndex(inputStart);
//       outputRegion.SetIndex(outputStart);
//       ConstIteratorType inputIt(result, inputRegion);
//       IteratorType outputIt(result,outputRegion);
//       // Duplication de la dernière colonne
//       for(inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();++inputIt,++outputIt)
// 	{
// 	outputIt.Set(inputIt.Get());
// 	}      
//      }
  /** Output filter connexion */
  this->GraftOutput(result);
}
/**
 * PrintSelf method
 */
template <class TInputImage,class TOuputImage>
void
Resampler<TInputImage, TOuputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Size: " << m_Size << std::endl;
}
} // End namespace morphologicalPyramid
} // End namespace otb
#endif
