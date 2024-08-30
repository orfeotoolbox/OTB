/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStreamingImageToOGRLayerSegmentationFilter_hxx
#define otbStreamingImageToOGRLayerSegmentationFilter_hxx

#include "otbStreamingImageToOGRLayerSegmentationFilter.h"

#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

#include "otbStopwatch.h"
#include "otbMacro.h"
#include <cassert>

namespace otb
{

template <class TImageType, class TSegmentationFilter>
PersistentImageToOGRLayerSegmentationFilter<TImageType, TSegmentationFilter>::PersistentImageToOGRLayerSegmentationFilter()
  : m_TileMaxLabel(0),
    m_StartLabel(0),
    m_SegmentationFilter(),
    m_FieldName("DN"),
    m_Use8Connected(false),
    m_FilterSmallObject(false),
    m_MinimumObjectSize(1),
    m_Simplify(false),
    m_SimplificationTolerance(0.3)
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(1);
  m_SegmentationFilter = SegmentationFilterType::New();
  m_TileNumber         = 1;
}

template <class TImageType, class TSegmentationFilter>
PersistentImageToOGRLayerSegmentationFilter<TImageType, TSegmentationFilter>::~PersistentImageToOGRLayerSegmentationFilter()
{
}

template <class TImageType, class TSegmentationFilter>
void PersistentImageToOGRLayerSegmentationFilter<TImageType, TSegmentationFilter>::SetInputMask(const LabelImageType* mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<LabelImageType*>(mask));
}

template <class TImageType, class TSegmentationFilter>
const typename PersistentImageToOGRLayerSegmentationFilter<TImageType, TSegmentationFilter>::LabelImageType*
PersistentImageToOGRLayerSegmentationFilter<TImageType, TSegmentationFilter>::GetInputMask(void)
{
  return static_cast<const LabelImageType*>(this->itk::ProcessObject::GetInput(1));
}


template <class TImageType, class TSegmentationFilter>
typename PersistentImageToOGRLayerSegmentationFilter<TImageType, TSegmentationFilter>::OGRDataSourcePointerType
PersistentImageToOGRLayerSegmentationFilter<TImageType, TSegmentationFilter>::ProcessTile()
{
  otbMsgDebugMacro(<< "tile number : " << m_TileNumber);
  ++m_TileNumber;

  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typedef itk::ExtractImageFilter<InputImageType, InputImageType> ExtractImageFilterType;
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput(this->GetInput());
  extract->SetExtractionRegion(this->GetInput()->GetRequestedRegion());
  extract->Update();

  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionary
  extract->GetOutput()->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());

  const unsigned int labelImageIndex = LabeledOutputAccessor<SegmentationFilterType>::LabeledOutputIndex;

  typename LabelImageToOGRDataSourceFilterType::Pointer labelImageToOGRDataFilter = LabelImageToOGRDataSourceFilterType::New();

  otb::Stopwatch chrono = otb::Stopwatch::StartNew();
  m_SegmentationFilter->SetInput(extract->GetOutput());
  m_SegmentationFilter->UpdateLargestPossibleRegion();

  otbMsgDebugMacro(<< "segmentation took " << chrono.GetElapsedMilliseconds() / 1000 << " sec");

  chrono.Restart();
  typename LabelImageType::ConstPointer inputMask = this->GetInputMask();
  if (!inputMask.IsNull())
  {
    // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
    typedef itk::ExtractImageFilter<LabelImageType, LabelImageType> ExtractLabelImageFilterType;
    typename ExtractLabelImageFilterType::Pointer maskExtract = ExtractLabelImageFilterType::New();
    maskExtract->SetInput(this->GetInputMask());
    maskExtract->SetExtractionRegion(this->GetInput()->GetRequestedRegion());
    maskExtract->Update();
    // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionary
    maskExtract->GetOutput()->SetMetaDataDictionary(this->GetInputMask()->GetMetaDataDictionary());

    labelImageToOGRDataFilter->SetInputMask(maskExtract->GetOutput());
  }

  labelImageToOGRDataFilter->SetInput(dynamic_cast<LabelImageType*>(m_SegmentationFilter->GetOutputs().at(labelImageIndex).GetPointer()));
  labelImageToOGRDataFilter->SetFieldName(m_FieldName);
  labelImageToOGRDataFilter->SetUse8Connected(m_Use8Connected);
  labelImageToOGRDataFilter->Update();

  otbMsgDebugMacro(<< "vectorization took " << chrono.GetElapsedMilliseconds() / 1000 << " sec");

  // Relabeling & simplication of geometries & filtering small objects
  chrono.Restart();
  OGRDataSourcePointerType tmpDS    = const_cast<OGRDataSourceType*>(labelImageToOGRDataFilter->GetOutput());
  OGRLayerType             tmpLayer = tmpDS->GetLayer(0);

  const typename InputImageType::SpacingType inSpacing = this->GetInput()->GetSignedSpacing();
  const double                               tol       = m_SimplificationTolerance * std::max(std::abs(inSpacing[0]), std::abs(inSpacing[1]));

  typename OGRLayerType::iterator featIt = tmpLayer.begin();
  for (featIt = tmpLayer.begin(); featIt != tmpLayer.end(); ++featIt)
  {
    ogr::Field field = (*featIt)[0];
    // field.Unset();
    field.SetValue(m_TileMaxLabel);
    m_TileMaxLabel++;

    // Simplify the geometry
    if (m_Simplify)
    {
      const OGRGeometry* geom = (*featIt).GetGeometry();
      assert(geom && "geometry is NULL ! Can't simplify it.");

      (*featIt).SetGeometryDirectly(ogr::Simplify(*geom, tol));
    }
    // Need to rewrite the feature otherwise changes are not considered.
    tmpLayer.SetFeature(*featIt);

    // Filter small objects.
    if (m_FilterSmallObject)
    {
      double area = static_cast<const OGRPolygon*>((*featIt).GetGeometry())->get_Area();
      // convert into pixel coordinates
      typename InputImageType::SpacingType spacing    = this->GetInput()->GetSignedSpacing();
      double                               pixelsArea = area / (std::abs(spacing[0] * spacing[1]));
      otbMsgDebugMacro(<< "DN = " << field.GetValue<int>() << ", area = " << pixelsArea);
      if (pixelsArea < m_MinimumObjectSize)
      {
        tmpLayer.DeleteFeature((*featIt).GetFID());
      }
    }
  }
  chrono.Stop();
  otbMsgDebugMacro(<< "relabeling, filtering small objects and simplifying geometries took " << chrono.GetElapsedMilliseconds() / 1000 << " sec");

  return tmpDS;
}


} // end namespace otb
#endif
