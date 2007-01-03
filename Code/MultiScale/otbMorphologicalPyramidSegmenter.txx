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
#ifndef _otbMorphologicalPyramidSegmenter_txx
#define _otbMorphologicalPyramidSegmenter_txx

#include "otbMorphologicalPyramidSegmenter.h"

#include "otbImage.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbThresholdImageToPointSetFilter.h"
#include "itkScalarImageToHistogramGenerator.h"

namespace otb
{
namespace morphologicalPyramid
{
/**
 * Constructor
 */
template <class TInputImage,class TOuputImage>
Segmenter<TInputImage, TOuputImage>
::Segmenter()
{
  this->SetNumberOfRequiredInputs(2);
	m_SeedsQuantile = 0.9;
	m_MinimumObjectSize = 10;
	m_SegmentDarkDetailsBool = false;
	m_NumberOfObjects = 0;
	m_ConnectedThresholdQuantile = 0.9;
}
/**
 * Set the details image.
 * \param detailsImage The details image from the morphological pyramid
 */
template <class TInputImage,class TOuputImage>
void
Segmenter<TInputImage, TOuputImage>
::SetDetailsImage(const InputImageType * detailsImage)
{
this->SetNthInput(0,const_cast<TInputImage *>(detailsImage));
}
  /**
   * Set the details image.
   * \return detailsImage The input details image.
   */
template <class TInputImage,class TOuputImage>
typename Segmenter<TInputImage, TOuputImage>::InputImageType * 
Segmenter<TInputImage, TOuputImage>
::GetDetailsImage(void)
{
  return const_cast<InputImageType *>(this->GetInput(0));
}
 /**
   * Set the original image.
   * \param originalImage The original image to segment.
   */
template <class TInputImage,class TOuputImage>
void
Segmenter<TInputImage, TOuputImage>
::SetOriginalImage(const InputImageType * originalImage)
{
  this->SetNthInput(1,const_cast<TInputImage *>(originalImage));
}
/**
 * Get the original image.
 * \return originalImage The original image to segment.
 */
template <class TInputImage,class TOuputImage>
typename Segmenter<TInputImage, TOuputImage>::InputImageType * 
Segmenter<TInputImage, TOuputImage>
::GetOriginalImage(void)
{
  return const_cast<InputImageType *>(this->GetInput(1));
}
/**
 * Configure the input datas.
 */
template <class TInputImage,class TOuputImage>
void
Segmenter<TInputImage, TOuputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the inputs
  InputImagePointerType  detailsPtr =
    const_cast< InputImageType * >( this->GetInput(0) );

  InputImagePointerType  origPtr = 
    const_cast< InputImageType * >( this->GetInput(1) );
  
  if ( !detailsPtr || !origPtr )
    {
    return;
    }

  // We need to
  // configure the inputs such that all the data is available.
  detailsPtr->SetRequestedRegion(detailsPtr->GetLargestPossibleRegion());
  origPtr->SetRequestedRegion(origPtr->GetLargestPossibleRegion());
}
/**
 * Configure the output data
 */
template <class TInputImage,class TOuputImage>
void
Segmenter<TInputImage, TOuputImage>
::EnlargeOutputRequestedRegion(void)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}

/**
 * Main computation method
 */
template <class TInputImage,class TOuputImage>
void
Segmenter<TInputImage, TOuputImage>
::GenerateData()
{
  // Input images pointers
  InputImagePointerType details = this->GetDetailsImage();
  InputImagePointerType original = this->GetOriginalImage();

  // Typedefs for details image enhancement
  typedef float FloatPixelType;
  typedef otb::Image<FloatPixelType,InputImageType::ImageDimension> FloatImageType;
  typedef itk::InvertIntensityImageFilter<InputImageType,InputImageType> InvertFilterType;
  typedef itk::MultiplyImageFilter<FloatImageType,FloatImageType,FloatImageType> MultiplyFilterType;
  typedef itk::CastImageFilter<InputImageType,FloatImageType> CastImageFilterType;
  typedef itk::RescaleIntensityImageFilter<FloatImageType,InputImageType> RescaleFilterType;

  //Typedefs for seeds extraction
  typedef itk::PointSet<InputPixelType,InputImageType::ImageDimension> PointSetType;
  typedef otb::ThresholdImageToPointSetFilter<InputImageType,PointSetType> PointSetFilterType;
  typedef typename PointSetType::PointsContainer::Iterator PointSetIteratorType;
  typedef typename PointSetType::PointType PointType;
  
  // Typedefs for segmentation
  typedef itk::ConnectedThresholdImageFilter<InputImageType,InputImageType> ConnectedFilterType;
  typedef itk::ConnectedComponentImageFilter<InputImageType,OutputImageType> LabelFilterType;
  typedef itk::RelabelComponentImageFilter<OutputImageType,OutputImageType> RelabelFilterType;
  typedef itk::ThresholdImageFilter<OutputImageType> ThresholdFilterType;
  
  // Typedefs for statistics computation
  typedef itk::Statistics::ScalarImageToHistogramGenerator<InputImageType> HistGeneratorType;
  typedef typename HistGeneratorType::HistogramType HistogramType;

  /////////////////////////////////////
  //// Details image enhancement //////
  /////////////////////////////////////

  // Filters instantiation
  typename InvertFilterType::Pointer invert;
  typename CastImageFilterType::Pointer cast1 = CastImageFilterType::New();
  typename CastImageFilterType::Pointer cast2 = CastImageFilterType::New();
  typename MultiplyFilterType::Pointer mult = MultiplyFilterType::New();
  typename RescaleFilterType::Pointer rescaler = RescaleFilterType::New();

  // Pipeline connection
  cast1->SetInput(details);
  // If we want to segment darker detail, the original image must have its itensity inverted
  if(m_SegmentDarkDetailsBool)
    {
      invert = InvertFilterType::New();
      invert->SetInput(original);
      cast2->SetInput(invert->GetOutput());
    }
  else
    {
      cast2->SetInput(original);
    }
  mult->SetInput1(cast1->GetOutput());
  mult->SetInput2(cast2->GetOutput());
  rescaler->SetInput(mult->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->Update();
  

  /////////////////////////////////////
  //// Thresholds computation /////////
  /////////////////////////////////////

  // Filter instantiation
  typename HistGeneratorType::Pointer histogram = HistGeneratorType::New();
  // Seeds Threshold is computed from the quantile
  histogram->SetInput(details);
  histogram->SetNumberOfBins(255);
  histogram->SetMarginalScale(10.0);
  histogram->Compute();
  InputPixelType  pointSetThreshold = 
    static_cast<InputPixelType>(histogram->GetOutput()->Quantile(0,m_SeedsQuantile));

  // Segmentation Threshold is computed from the quantile
  histogram = HistGeneratorType::New();
  histogram->SetInput(rescaler->GetOutput());
  histogram->SetNumberOfBins(255);
  histogram->SetMarginalScale(10.0);
  histogram->Compute();
  InputPixelType  connectedThresholdValue = 
    static_cast<InputPixelType>(histogram->GetOutput()->Quantile(0,m_ConnectedThresholdQuantile));

  /////////////////////////////////////
  //// Seeds extraction ///////////////
  /////////////////////////////////////

  typename PointSetFilterType::Pointer pointSetFilter = PointSetFilterType::New();
  pointSetFilter->SetInput(0,details);
  pointSetFilter->SetLowerThreshold(pointSetThreshold);
  pointSetFilter->Update();

  /////////////////////////////////////
  //// Segmentation ///////////////////
  /////////////////////////////////////

  // Filters instantiation
  typename ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
  typename LabelFilterType::Pointer labeler = LabelFilterType::New();
  typename RelabelFilterType::Pointer relabeler = RelabelFilterType::New();
  typename ThresholdFilterType::Pointer threshold = ThresholdFilterType::New();
  
  //Passing seeds to the connected filter
  connectedThreshold = ConnectedFilterType::New();
  connectedThreshold->ClearSeeds();
  connectedThreshold->SetInput(rescaler->GetOutput());
  PointSetIteratorType it = pointSetFilter->GetOutput()->GetPoints()->Begin();
  while(it!=pointSetFilter->GetOutput()->GetPoints()->End())
    {
      typename OutputImageType::IndexType index;
      index[0]=static_cast<long int>(it.Value()[0]*(rescaler->GetOutput()->GetSpacing()[0]));
      index[1]=static_cast<long int>(it.Value()[1]*(rescaler->GetOutput()->GetSpacing()[1]));
      connectedThreshold->AddSeed(index);
      it++;
    }

  // segmentation     
  connectedThreshold->SetLower(connectedThresholdValue);

  // labelling
  labeler = LabelFilterType::New();
  relabeler = RelabelFilterType::New();
  labeler->SetInput(connectedThreshold->GetOutput());
  relabeler->SetInput(labeler->GetOutput());
  relabeler->SetMinimumObjectSize(m_MinimumObjectSize);
  relabeler->Update();

  // In some cases it might happen that the whole extent of the image is segmented as a single region.
  // Since this is not desirable, we test this case here to avoid it. 
  threshold = ThresholdFilterType::New();
  threshold->SetInput(relabeler->GetOutput());
  OutputPixelType num = 0;
  if(relabeler->GetNumberOfObjects()==1)
    {
      unsigned int surface = rescaler->GetOutput()->GetLargestPossibleRegion().GetSize()[0]
	*rescaler->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
      if(relabeler->GetSizeOfObjectsInPixels()[0]==surface)
	{
	  num = 0;
	}
      else
	{        
	  num=1;
	}
    }
  else
    {
      num= static_cast<OutputPixelType>(relabeler->GetNumberOfObjects());
    }   
  threshold->ThresholdOutside(0,num);    

  // Output connection
  threshold->GraftOutput(this->GetOutput());
  threshold->Update();
  this->GraftOutput(threshold->GetOutput());
  m_NumberOfObjects = num;
}
/**
 * PrintSelf method
 */
template <class TInputImage,class TOuputImage>
void
Segmenter<TInputImage, TOuputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);

}
} // End namespace morphologicalPyramid
} // End namespace otb
#endif
