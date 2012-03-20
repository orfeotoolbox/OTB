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
#ifndef __otbMeanShiftConnectedComponentSegmentationFilter_txx
#define __otbMeanShiftConnectedComponentSegmentationFilter_txx

#include "otbMeanShiftConnectedComponentSegmentationFilter.h"
#include "itkExtractImageFilter.h"

namespace otb {

template<class TVImage,class TMaskImage, class TLabelImage>
MeanShiftConnectedComponentSegmentationFilter<TVImage,TMaskImage,  TLabelImage>
::MeanShiftConnectedComponentSegmentationFilter()
{
   m_MinimumObjectSize=2;
   m_MeanShiftFilter= MeanShiftFilterType::New();

}

template<class TVImage,class TMaskImage, class TLabelImage>
MeanShiftConnectedComponentSegmentationFilter<TVImage,TMaskImage,  TLabelImage>
::~MeanShiftConnectedComponentSegmentationFilter()
{
}

template<class TVImage,class TMaskImage, class TLabelImage>
void
MeanShiftConnectedComponentSegmentationFilter<TVImage,TMaskImage,  TLabelImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

template<class TVImage,class TMaskImage, class TLabelImage>
void
MeanShiftConnectedComponentSegmentationFilter<TVImage,TMaskImage,  TLabelImage>
::GenerateData()
{
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typedef itk::ExtractImageFilter<TVImage, TVImage> ExtractImageFilterType;
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput(this->GetInput());
  extract->SetExtractionRegion(this->GetOutput()->GetRequestedRegion());
  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionnary

  // meanshift filtering

  //not necessary
  /* if (this->m_MeanShiftFilter.IsNotNull())
   {
   itkExceptionMacro("MeanShiftFilter is null");
   }*/
  this->m_MeanShiftFilter->SetInput(extract->GetOutput());

  //this->m_MeanShiftFilter->SetInput(this->GetInput());


  typename MaskImageType::Pointer mask;
  if (!m_MaskExpression.empty())
    {
    // Compute the mask
    typename MaskMuParserFilterType::Pointer maskFilter;
    maskFilter = MaskMuParserFilterType::New();
    maskFilter->SetInput(this->m_MeanShiftFilter->GetOutput());
    maskFilter->SetExpression(m_MaskExpression);
    maskFilter->Update();
    mask = maskFilter->GetOutput();
    }

  // Perform connected components segmentation
  typename ConnectedComponentFilterType::Pointer connected = ConnectedComponentFilterType::New();
  connected->SetInput(this->m_MeanShiftFilter->GetOutput());

  if (mask.IsNotNull()) connected->SetMaskImage(mask);
  connected->GetFunctor().SetExpression(m_ConnectedComponentExpression);
  connected->Update();

  // Relabel connected component output
  typename RelabelComponentFilterType::Pointer relabel = RelabelComponentFilterType::New();
  relabel->SetInput(connected->GetOutput());
  relabel->SetMinimumObjectSize(m_MinimumObjectSize);
  relabel->Update();

  this->GraftOutput(relabel->GetOutput());

}

} // end namespace otb
#endif
