/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter_hxx
#define otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter_hxx

#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.h"
#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

namespace otb {

template<class TVImage, class TLabelImage, class TMaskImage, class TOutputVectorData>
PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<TVImage, TLabelImage, TMaskImage, TOutputVectorData>
::PersistentConnectedComponentSegmentationOBIAToVectorDataFilter()
 : m_MinimumObjectSize(2), m_ShapeReducedSetOfAttributes(false), m_StatsReducedSetOfAttributes(false),
m_ComputeFlusser(false), m_ComputePolygon(false), m_ComputeFeretDiameter(false), m_ComputePerimeter(false)
{
}

template<class TVImage, class TLabelImage, class TMaskImage, class TOutputVectorData>
PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<TVImage, TLabelImage, TMaskImage, TOutputVectorData>
::~PersistentConnectedComponentSegmentationOBIAToVectorDataFilter()
{
}

template<class TVImage, class TLabelImage, class TMaskImage, class TOutputVectorData>
void
PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<TVImage, TLabelImage, TMaskImage, TOutputVectorData>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

template<class TVImage, class TLabelImage, class TMaskImage, class TOutputVectorData>
typename PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<TVImage, TLabelImage, TMaskImage, TOutputVectorData>::VectorDataPointerType
PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<TVImage, TLabelImage, TMaskImage, TOutputVectorData>
::ProcessTile()
{
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput( this->GetInput() );
  extract->SetExtractionRegion( this->GetOutput()->GetRequestedRegion() );
  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionary

  typename MaskImageType::Pointer mask;
  if (!m_MaskExpression.empty())
    {
    // Compute the mask
    typename MaskMuParserFilterType::Pointer maskFilter;
    maskFilter = MaskMuParserFilterType::New();
    maskFilter->SetInput(extract->GetOutput());
    maskFilter->SetExpression(m_MaskExpression);
    maskFilter->Update();
    mask = maskFilter->GetOutput();
    }

  // Perform connected components segmentation
  typename ConnectedComponentFilterType::Pointer connected = ConnectedComponentFilterType::New();
  connected->SetInput(extract->GetOutput());

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

  typename AttributesLabelMapType::Pointer labelMap = labelImageToLabelMap->GetOutput();

  if (!m_OBIAExpression.empty())
    {
    // shape attributes computation
    typename ShapeLabelMapFilterType::Pointer shapeLabelMapFilter = ShapeLabelMapFilterType::New();
    shapeLabelMapFilter->SetInput(labelImageToLabelMap->GetOutput());
    shapeLabelMapFilter->SetReducedAttributeSet(m_ShapeReducedSetOfAttributes);
    shapeLabelMapFilter->SetComputePolygon(m_ComputePolygon);
    shapeLabelMapFilter->SetComputePerimeter(m_ComputePerimeter);
    shapeLabelMapFilter->SetComputeFeretDiameter(m_ComputeFeretDiameter);
    shapeLabelMapFilter->SetComputeFlusser(m_ComputeFlusser);

    // band stat attributes computation
    typename RadiometricLabelMapFilterType::Pointer radiometricLabelMapFilter = RadiometricLabelMapFilterType::New();
    radiometricLabelMapFilter->SetInput(shapeLabelMapFilter->GetOutput());
    radiometricLabelMapFilter->SetFeatureImage(extract->GetOutput());
    radiometricLabelMapFilter->SetReducedAttributeSet(m_StatsReducedSetOfAttributes);

    // OBIA Filtering using shape and radiometric object characteristics
    typename LabelObjectOpeningFilterType::Pointer opening = LabelObjectOpeningFilterType::New();
    opening->SetExpression(m_OBIAExpression);
    opening->SetInput(radiometricLabelMapFilter->GetOutput());
    opening->Update();

    labelMap = opening->GetOutput();
    }

  // Transformation to VectorData
  typename LabelMapToVectorDataFilterType::Pointer labelMapToVectorDataFilter = LabelMapToVectorDataFilterType::New();
  labelMapToVectorDataFilter->SetInput(labelMap);
  labelMapToVectorDataFilter->Update();

  // The VectorData in output of the chain is in image index coordinate,
  // and the projection information is lost
  // Apply an affine transform to apply image origin and spacing,
  // and arbitrarily set the ProjectionRef to the input image ProjectionRef

  typedef itk::AffineTransform<typename VectorDataType::PrecisionType, 2> TransformType;
  typedef VectorDataTransformFilter<VectorDataType, VectorDataType> VDTransformType;

  typename TransformType::ParametersType params;
  params.SetSize(6);
  params[0] = this->GetInput()->GetSignedSpacing()[0];
  params[1] = 0;
  params[2] = 0;
  params[3] = this->GetInput()->GetSignedSpacing()[1];
  params[4] = this->GetInput()->GetOrigin()[0];
  params[5] = this->GetInput()->GetOrigin()[1];

  typename TransformType::Pointer transform = TransformType::New();
  transform->SetParameters(params);

  typename VDTransformType::Pointer vdTransform = VDTransformType::New();
  vdTransform->SetTransform(transform);
  vdTransform->SetInput(labelMapToVectorDataFilter->GetOutput());
  vdTransform->Update();
  vdTransform->GetOutput()->SetProjectionRef(this->GetInput()->GetProjectionRef());

  return vdTransform->GetOutput();
}

} // end namespace otb
#endif
