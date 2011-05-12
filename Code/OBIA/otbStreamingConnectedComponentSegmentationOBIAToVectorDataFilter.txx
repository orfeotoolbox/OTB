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
#ifndef __otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter_txx
#define __otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter_txx

#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.h"

namespace otb {

template<class TVImage, class TLabelImage, class TMaskImage, class TOutputVectorData>
PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<TVImage, TLabelImage, TMaskImage, TOutputVectorData>
::PersistentConnectedComponentSegmentationOBIAToVectorDataFilter()
 : m_MinimumObjectSize(2)
{
}

template<class TVImage, class TLabelImage, class TMaskImage, class TOutputVectorData>
PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<TVImage, TLabelImage, TMaskImage, TOutputVectorData>
::~PersistentConnectedComponentSegmentationOBIAToVectorDataFilter()
{
}

template<class TVImage, class TLabelImage, class TMaskImage, class TOutputVectorData>
typename PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<TVImage, TLabelImage, TMaskImage, TOutputVectorData>::VectorDataPointerType
PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<TVImage, TLabelImage, TMaskImage, TOutputVectorData>
::ProcessTile(const VectorImageType* inputImage)
{
  typename MaskImageType::Pointer mask;
  if (!m_MaskExpression.empty())
    {
    // Compute the mask
    typename MaskMuParserFilterType::Pointer maskFilter;
    maskFilter = MaskMuParserFilterType::New();
    maskFilter->SetInput(inputImage);
    maskFilter->SetExpression(m_MaskExpression);
    maskFilter->Update();
    mask = maskFilter->GetOutput();
    }

  // Perform connected components segmentation
  typename ConnectedComponentFilterType::Pointer connected = ConnectedComponentFilterType::New();
  connected->SetInput(inputImage);

  if (mask.IsNotNull())
    connected->SetMaskImage(mask);
  connected->GetFunctor().SetExpression(m_ConnectedComponentExpression);
  connected->Update();

  // Relabel connected component output
  typename RelabelComponentFilterType::Pointer relabel = RelabelComponentFilterType::New();
  relabel->SetInput(connected->GetOutput());
  relabel->SetMinimumObjectSize(m_MinimumObjectSize);
  relabel->Update();

  //Attributes computation
  // LabelImage to Label Map transformation
  typename LabelImageToLabelMapFilterType::Pointer labelImageToLabelMap = LabelImageToLabelMapFilterType::New();
  labelImageToLabelMap->SetInput(relabel->GetOutput());
  labelImageToLabelMap->SetBackgroundValue(0);
  labelImageToLabelMap->Update();

  // shape attributes computation
  typename ShapeLabelMapFilterType::Pointer shapeLabelMapFilter = ShapeLabelMapFilterType::New();
  shapeLabelMapFilter->SetInput(labelImageToLabelMap->GetOutput());
  shapeLabelMapFilter->SetReducedAttributeSet(false);

  // band stat attributes computation
  typename RadiometricLabelMapFilterType::Pointer radiometricLabelMapFilter = RadiometricLabelMapFilterType::New();
  radiometricLabelMapFilter->SetInput(shapeLabelMapFilter->GetOutput());
  radiometricLabelMapFilter->SetFeatureImage(inputImage);
  radiometricLabelMapFilter->SetReducedAttributeSet(true);

  // OBIA Filtering using shape and radiometric object characteristics
  typename LabelObjectOpeningFilterType::Pointer opening = LabelObjectOpeningFilterType::New();
  opening->SetExpression(m_OBIAExpression);
  opening->SetInput(radiometricLabelMapFilter->GetOutput());

  // Transformation to VectorData
  typename LabelMapToVectorDataFilterType::Pointer labelMapToVectorDataFilter = LabelMapToVectorDataFilterType::New();
  labelMapToVectorDataFilter->SetInput(opening->GetOutput());
  labelMapToVectorDataFilter->Update();

  return labelMapToVectorDataFilter->GetOutput();
}

} // end namespace otb
#endif
