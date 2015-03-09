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
#ifndef __otbMorphologicalPyramidSegmenter_txx
#define __otbMorphologicalPyramidSegmenter_txx

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
#include "itkMinimumMaximumImageCalculator.h"

namespace otb
{
namespace MorphologicalPyramid
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
Segmenter<TInputImage, TOutputImage>
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
template <class TInputImage, class TOutputImage>
void
Segmenter<TInputImage, TOutputImage>
::SetDetailsImage(const InputImageType * detailsImage)
{
  this->SetNthInput(0, const_cast<TInputImage *>(detailsImage));
}
/**
 * Set the details image.
 * \return detailsImage The input details image.
 */
template <class TInputImage, class TOutputImage>
typename Segmenter<TInputImage, TOutputImage>::InputImageType *
Segmenter<TInputImage, TOutputImage>
::GetDetailsImage(void)
{
  return const_cast<InputImageType *>(this->GetInput(0));
}
/**
  * Set the original image.
  * \param originalImage The original image to segment.
  */
template <class TInputImage, class TOutputImage>
void
Segmenter<TInputImage, TOutputImage>
::SetOriginalImage(const InputImageType * originalImage)
{
  this->SetNthInput(1, const_cast<TInputImage *>(originalImage));
}
/**
 * Get the original image.
 * \return originalImage The original image to segment.
 */
template <class TInputImage, class TOutputImage>
typename Segmenter<TInputImage, TOutputImage>::InputImageType *
Segmenter<TInputImage, TOutputImage>
::GetOriginalImage(void)
{
  return const_cast<InputImageType *>(this->GetInput(1));
}
/**
 * Configure the input datas.
 */
template <class TInputImage, class TOutputImage>
void
Segmenter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  InputImagePointerType detailsPtr =
    const_cast<InputImageType *>(this->GetInput(0));

  InputImagePointerType origPtr =
    const_cast<InputImageType *>(this->GetInput(1));

  if (!detailsPtr || !origPtr)
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
template <class TInputImage, class TOutputImage>
void
Segmenter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(void)
{
  this->GetOutput()
  ->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void
Segmenter<TInputImage, TOutputImage>
::GenerateData()
{
  // Input images pointers
  InputImagePointerType details = this->GetDetailsImage();
  InputImagePointerType original = this->GetOriginalImage();

  // Typedefs for details image enhancement
  typedef double                                                                   FloatPixelType;
  typedef otb::Image<FloatPixelType, InputImageType::ImageDimension>               FloatImageType;
  typedef itk::InvertIntensityImageFilter<InputImageType, InputImageType>          InvertFilterType;
  typedef itk::MultiplyImageFilter<FloatImageType, FloatImageType, InputImageType> MultiplyFilterType;
  typedef itk::CastImageFilter<InputImageType, FloatImageType>                     CastImageFilterType;
  typedef itk::MinimumMaximumImageCalculator<InputImageType>                       MinMaxCalculatorType;

  //Typedefs for seeds extraction
  typedef itk::PointSet<InputPixelType, InputImageType::ImageDimension>     PointSetType;
  typedef otb::ThresholdImageToPointSetFilter<InputImageType, PointSetType> PointSetFilterType;
  typedef typename PointSetType::PointsContainer::Iterator                  PointSetIteratorType;

  // Typedefs for segmentation
  typedef itk::ConnectedThresholdImageFilter<InputImageType, InputImageType>  ConnectedFilterType;
  typedef itk::ConnectedComponentImageFilter<InputImageType, OutputImageType> LabelFilterType;
  typedef itk::RelabelComponentImageFilter<OutputImageType, OutputImageType>  RelabelFilterType;
  typedef itk::ThresholdImageFilter<OutputImageType>                          ThresholdFilterType;

  // Typedefs for statistics computation
  typedef itk::Statistics::ScalarImageToHistogramGenerator<InputImageType> HistGeneratorType;

  /////////////////////////////////////
  //// Details image enhancement //////
  /////////////////////////////////////

  // Filters instantiation
  typename InvertFilterType::Pointer     invert;
  typename CastImageFilterType::Pointer  cast1 = CastImageFilterType::New();
  typename CastImageFilterType::Pointer  cast2 = CastImageFilterType::New();
  typename MultiplyFilterType::Pointer   mult = MultiplyFilterType::New();
  typename MinMaxCalculatorType::Pointer minMax =  MinMaxCalculatorType::New();

  // Pipeline connection
  cast1->SetInput(details);

  minMax->SetImage(original);
  minMax->ComputeMaximum();

  // If we want to segment darker detail, the original image must have its itensity inverted
  if (m_SegmentDarkDetailsBool)
    {
    invert = InvertFilterType::New();
    invert->SetInput(original);
    invert->SetMaximum(minMax->GetMaximum());
    cast2->SetInput(invert->GetOutput());
    }
  else
    {
    cast2->SetInput(original);
    }
  mult->SetInput1(cast1->GetOutput());
  mult->SetInput2(cast2->GetOutput());
  mult->Update();

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
  InputPixelType pointSetThreshold =
    static_cast<InputPixelType>(histogram->GetOutput()->Quantile(0, m_SeedsQuantile));

  // Segmentation Threshold is computed from the quantile
  histogram = HistGeneratorType::New();
  histogram->SetInput(mult->GetOutput());
  histogram->SetNumberOfBins(255);
  histogram->SetMarginalScale(10.0);
  histogram->Compute();
  InputPixelType connectedThresholdValue =
    static_cast<InputPixelType>(histogram->GetOutput()->Quantile(0, m_ConnectedThresholdQuantile));

  /////////////////////////////////////
  //// Seeds extraction ///////////////
  /////////////////////////////////////

  typename PointSetFilterType::Pointer pointSetFilter = PointSetFilterType::New();
  pointSetFilter->SetInput(0, details);
  pointSetFilter->SetLowerThreshold(pointSetThreshold);
  pointSetFilter->Update();

  /////////////////////////////////////
  //// Segmentation ///////////////////
  /////////////////////////////////////

  // Filters instantiation
  typename ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
  typename LabelFilterType::Pointer     labeler = LabelFilterType::New();
  typename RelabelFilterType::Pointer   relabeler = RelabelFilterType::New();
  typename ThresholdFilterType::Pointer threshold = ThresholdFilterType::New();

  //Passing seeds to the connected filter
  connectedThreshold = ConnectedFilterType::New();
  connectedThreshold->ClearSeeds();
  connectedThreshold->SetInput(mult->GetOutput());
  PointSetIteratorType it = pointSetFilter->GetOutput()->GetPoints()->Begin();
  while (it != pointSetFilter->GetOutput()->GetPoints()->End())
    {
    typename OutputImageType::IndexType index;
    index[0] = static_cast<long int>(it.Value()[0]);
    index[1] = static_cast<long int>(it.Value()[1]);
    connectedThreshold->AddSeed(index);
    ++it;
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
  if (relabeler->GetNumberOfObjects() == 1)
    {
    unsigned int surface = mult->GetOutput()->GetLargestPossibleRegion().GetSize()[0]
                           * mult->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
    if (relabeler->GetSizeOfObjectsInPixels()[0] == surface)
      {
      num = 0;
      }
    else
      {
      num = 1;
      }
    }
  else
    {
    num = static_cast<OutputPixelType>(relabeler->GetNumberOfObjects());
    }
  threshold->ThresholdOutside(0, num);

  // Output connection
  threshold->GraftOutput(this->GetOutput());
  threshold->Update();
  this->GraftOutput(threshold->GetOutput());
  m_NumberOfObjects = num;
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputImage>
void
Segmenter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}
} // End namespace MorphologicalPyramid
} // End namespace otb
#endif
