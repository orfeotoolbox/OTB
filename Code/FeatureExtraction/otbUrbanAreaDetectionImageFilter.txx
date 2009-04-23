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

//   m_ErodeFilter = ErodeFilterType::New();
//   m_DilateFilter = DilateFilterType::New();
//   m_ErodeFilter2 = ErodeFilterType::New();
//   m_DilateFilter2 = DilateFilterType::New();
  m_IntensityFilter = IntensityFilterType::New();
  m_EdgeDetectorFilter = EdgeDensityFilterType::New();
  m_SobelFilter = SobelDetectorType::New();
  m_Thresholder = ThresholdFilterType::New();
  m_MaskImageFilter = MaskImageFilterType::New();
  m_UrbanAreaExtractionFilter = UrbanAreaExtractionFilterType::New();

  /** Init the Pipeline */
  // Intensity Image of the input image
//   m_IntensityFilter->SetInput(this->GetInput());
  // EdgeDensity
  m_EdgeDetectorFilter->SetInput(m_IntensityFilter->GetOutput());
  m_EdgeDetectorFilter->SetDetector(m_SobelFilter);
  // Threshold
  m_Thresholder->SetInput(m_EdgeDetectorFilter->GetOutput());
  // Mask Image
//   m_MaskImageFilter->SetInput1(this->GetInput());
  m_MaskImageFilter->SetInput2(m_Thresholder->GetOutput());
  // NonVegetationNonWaterIndex
  m_UrbanAreaExtractionFilter->SetInput(m_MaskImageFilter->GetOutput());

  m_ThresholdValue = 0.5;
  m_ThresholdDensity = 0.1;
}

/**
 * Parameters the filters of the pipeline
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UrbanAreaDetectionImageFilter<TInputImage, TOutputImage, TFunction>
::GenerateData()
{

  m_IntensityFilter->SetInput(this->GetInput());
  m_MaskImageFilter->SetInput1(this->GetInput());


  // Edge Density
  m_SobelFilter->SetLowerThreshold(-100.0);
  m_SobelFilter->SetUpperThreshold(100.0);
  SizeType lSize;
  lSize[0] = static_cast<unsigned int>(10);
  lSize[1] = static_cast<unsigned int>(10);
  m_EdgeDetectorFilter->SetNeighborhoodRadius(lSize);

  // Threshold
  m_Thresholder->SetInsideValue(0);
  m_Thresholder->SetOutsideValue(1);
  m_Thresholder->SetLowerThreshold( 0. );
  m_Thresholder->SetUpperThreshold( m_ThresholdDensity );

  // Appli the mask on the input image
  m_MaskImageFilter->SetOutsideValue(0);

  // Give a threshold to urbanAreaFilter
  m_UrbanAreaExtractionFilter->GetFunctor().SetLowerThreshold( m_ThresholdValue );  


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


  /** GraftOutput */
  m_UrbanAreaExtractionFilter->GraftOutput(this->GetOutput());
  m_UrbanAreaExtractionFilter->Update();
  this->GraftOutput(m_UrbanAreaExtractionFilter->GetOutput());

}

} // end namespace otb


#endif
