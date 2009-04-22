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
#ifndef __otbUrbanAreaDetectionFilter_txx
#define __otbUrbanAreaDetectionFilter_txx

#include "otbUrbanAreaDetectionImageFilter.h"


namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
UrbanAreaDetectionImageFilter<TInputImage, TOutputImage, TFunction>
::UrbanAreaDetectionImageFilter()
{
  m_UrbanAreaExtractionFilter = UrbanAreaExtractionFilterType::New();
  m_ErodeFilter = ErodeFilterType::New();
  m_DilateFilter = DilateFilterType::New();
  m_ErodeFilter2 = ErodeFilterType::New();
  m_DilateFilter2 = DilateFilterType::New();
  m_MaskImageFilter = MaskImageFilterType::New();

  m_ThresholdValue = 0.5;
  m_ThresholdValue2 = 0.1;
}

/**
 * Init the pipeline
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UrbanAreaDetectionImageFilter<TInputImage, TOutputImage, TFunction>
::GenerateData()
{


  // Intensity Image of the input image
  IntensityFilterPointerType lIntensityFilter = IntensityFilterType::New();
  lIntensityFilter->SetInput(this->GetInput());

  // Edge Density
  EdgeDensityFilterType::Pointer lEdgeDetectorFilter = EdgeDensityFilterType::New();
  SobelDetectorType::Pointer lSobelFilter = SobelDetectorType::New();
  lSobelFilter->SetLowerThreshold(-200.0);
  lSobelFilter->SetUpperThreshold(200.0);
  SizeType lSize;
  lSize[0] = static_cast<unsigned int>(10);
  lSize[1] = static_cast<unsigned int>(10);
  lEdgeDetectorFilter->SetInput(lIntensityFilter->GetOutput());
  lEdgeDetectorFilter->SetDetector(lSobelFilter);
  lEdgeDetectorFilter->SetNeighborhoodRadius(lSize);
  //lEdgeDetectorFilter->Update();

  // Threshold
  ThresholdFilterPointerType lThresholder = ThresholdFilterType::New();
  lThresholder->SetInput(lEdgeDetectorFilter->GetOutput());
  lThresholder->SetInsideValue(0);
  lThresholder->SetOutsideValue(1);
  lThresholder->SetLowerThreshold( 0. );
  lThresholder->SetUpperThreshold( m_ThresholdValue2 );
  //lThresholder->Update();

// TEST 
//   typedef otb::ImageFileWriter<BinaryImageType>       BinaryWriterType;
//   BinaryWriterType::Pointer writer = BinaryWriterType::New();
//   writer->SetFileName("mask1.tif");
//   writer->SetInput(lThresholder->GetOutput());
//   writer->Update();

  // FIRST MASK : HIGH EDGE DENSITY AREAS

  // Appli the mask on the input image
  m_MaskImageFilter->SetOutsideValue(0);
  m_MaskImageFilter->SetInput1(this->GetInput());
  m_MaskImageFilter->SetInput2(lThresholder->GetOutput());
  //m_MaskImageFilter->Update();

  // Parameters for the UrbanAreaExtractionFilter
  m_UrbanAreaExtractionFilter->SetInput(this->GetInput());
  this->SetThreshold(m_ThresholdValue);
  //m_UrbanAreaExtractionFilter->Update();


//   BinaryWriterType::Pointer writer2 = BinaryWriterType::New();
//   writer2->SetFileName("mask2.tif");
//   writer2->SetInput(m_UrbanAreaExtractionFilter->GetOutput());
//   writer2->Update();


  m_UrbanAreaExtractionFilter->SetInput(m_MaskImageFilter->GetOutput());
  this->SetThreshold(m_ThresholdValue);
  m_UrbanAreaExtractionFilter->Update();

  // Erode/Dilate 2 times
//   StructuringElementType  structuringElement;
//   structuringElement.SetRadius( 1 );  // 3x3 structuring element
//   structuringElement.CreateStructuringElement();
// 
//   m_DilateFilter->SetInput(m_UrbanAreaExtractionFilter->GetOutput());
//   m_DilateFilter->SetDilateValue(1);
//   m_DilateFilter->SetKernel( structuringElement );
// 
//   m_ErodeFilter->SetInput(m_DilateFilter->GetOutput());
//   m_ErodeFilter->SetErodeValue(1);
//   m_ErodeFilter->SetKernel(  structuringElement );
//   m_ErodeFilter->Update();
// 
//   m_ErodeFilter2->SetInput(m_ErodeFilter->GetOutput());
//   m_ErodeFilter2->SetErodeValue(1);
//   m_ErodeFilter2->SetKernel(  structuringElement );
// 
//   m_DilateFilter2->SetInput(m_ErodeFilter2->GetOutput());
//   m_DilateFilter2->SetDilateValue(1);
//   m_DilateFilter2->SetKernel( structuringElement );
//   m_DilateFilter2->Update();

  // SECOND MASK : NON WATER, NON VEGETATION


  /** GraftOutput */
  this->GraftOutput(m_UrbanAreaExtractionFilter->GetOutput());

}

} // end namespace otb


#endif
