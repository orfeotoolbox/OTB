/*
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

#ifndef otbLabelImageToOGRDataSourceFilter_hxx
#define otbLabelImageToOGRDataSourceFilter_hxx

#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbGdalDataTypeBridge.h"
#include "otbGDALDriverManagerWrapper.h"

// gdal libraries
#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdal_alg.h"

#include "stdint.h" //needed for uintptr_t

namespace otb
{
template <class TInputImage>
LabelImageToOGRDataSourceFilter<TInputImage>::LabelImageToOGRDataSourceFilter() : m_FieldName("DN"), m_Use8Connected(false)
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->ProcessObject::SetNthOutput(0, this->MakeOutput(0));
}


template <class TInputImage>
typename LabelImageToOGRDataSourceFilter<TInputImage>::DataObjectPointer
LabelImageToOGRDataSourceFilter<TInputImage>::MakeOutput(DataObjectPointerArraySizeType itkNotUsed(idx))
{
  return static_cast<DataObjectPointer>(OGRDataSourceType::New().GetPointer());
}

template <class TInputImage>
const typename LabelImageToOGRDataSourceFilter<TInputImage>::OGRDataSourceType* LabelImageToOGRDataSourceFilter<TInputImage>::GetOutput()
{
  return static_cast<const OGRDataSourceType*>(this->ProcessObject::GetOutput(0));
}

template <class TInputImage>
void LabelImageToOGRDataSourceFilter<TInputImage>::SetInput(const InputImageType* input)
{
  this->Superclass::SetNthInput(0, const_cast<InputImageType*>(input));
}

template <class TInputImage>
const typename LabelImageToOGRDataSourceFilter<TInputImage>::InputImageType* LabelImageToOGRDataSourceFilter<TInputImage>::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
  {
    return nullptr;
  }

  return static_cast<const InputImageType*>(this->Superclass::GetInput(0));
}

template <class TInputImage>
void LabelImageToOGRDataSourceFilter<TInputImage>::SetInputMask(const InputImageType* input)
{
  this->Superclass::SetNthInput(1, const_cast<InputImageType*>(input));
}

template <class TInputImage>
const typename LabelImageToOGRDataSourceFilter<TInputImage>::InputImageType* LabelImageToOGRDataSourceFilter<TInputImage>::GetInputMask(void)
{
  if (this->GetNumberOfInputs() < 2)
  {
    return nullptr;
  }

  return static_cast<const InputImageType*>(this->Superclass::GetInput(1));
}

template <class TInputImage>
void LabelImageToOGRDataSourceFilter<TInputImage>::GenerateInputRequestedRegion(void)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  typename InputImageType::Pointer input = const_cast<InputImageType*>(this->GetInput());

  if (!input)
  {
    return;
  }
  // The input is necessarily the largest possible region.
  input->SetRequestedRegionToLargestPossibleRegion();

  typename InputImageType::Pointer mask = const_cast<InputImageType*>(this->GetInputMask());
  if (!mask)
  {
    return;
  }
  // The input is necessarily the largest possible region.
  mask->SetRequestedRegionToLargestPossibleRegion();
}


template <class TInputImage>
void LabelImageToOGRDataSourceFilter<TInputImage>::GenerateData(void)
{
  if (this->GetInput()->GetRequestedRegion() != this->GetInput()->GetLargestPossibleRegion())
  {
    itkExceptionMacro(<< "Not streamed filter. ERROR : requested region is not the largest possible region.");
  }

  SizeType     size;
  unsigned int nbBands      = 0;
  unsigned int bytePerPixel = 0;

  /* Convert the input image into a GDAL raster needed by GDALPolygonize */
  size         = this->GetInput()->GetLargestPossibleRegion().GetSize();
  nbBands      = this->GetInput()->GetNumberOfComponentsPerPixel();
  bytePerPixel = sizeof(InputPixelType);

  GDALDatasetWrapper::Pointer dataset =
          GDALDriverManagerWrapper::GetInstance().OpenFromMemory(
              const_cast<InputPixelType*>(this->GetInput()->GetBufferPointer()),
              this->GetInput()->GetLargestPossibleRegion().GetSize()[0],
              this->GetInput()->GetLargestPossibleRegion().GetSize()[1],
              GdalDataTypeBridge::GetGDALDataType<InputPixelType>(), bytePerPixel, nbBands, bytePerPixel);

  // Set input Projection ref and Geo transform to the dataset.
  dataset->GetDataSet()->SetProjection(this->GetInput()->GetProjectionRef().c_str());

  unsigned int projSize = this->GetInput()->GetGeoTransform().size();
  double       geoTransform[6];

  // Set the geo transform of the input image (if any)
  // Reporting origin and spacing of the buffered region
  // the spacing is unchanged, the origin is relative to the buffered region
  IndexType  bufferIndexOrigin = this->GetInput()->GetBufferedRegion().GetIndex();
  OriginType bufferOrigin;
  this->GetInput()->TransformIndexToPhysicalPoint(bufferIndexOrigin, bufferOrigin);
  geoTransform[0] = bufferOrigin[0] - 0.5 * this->GetInput()->GetSignedSpacing()[0];
  geoTransform[3] = bufferOrigin[1] - 0.5 * this->GetInput()->GetSignedSpacing()[1];
  geoTransform[1] = this->GetInput()->GetSignedSpacing()[0];
  geoTransform[5] = this->GetInput()->GetSignedSpacing()[1];
  // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
  if (projSize == 0)
  {
    geoTransform[2] = 0.;
    geoTransform[4] = 0.;
  }
  else
  {
    geoTransform[2] = this->GetInput()->GetGeoTransform()[2];
    geoTransform[4] = this->GetInput()->GetGeoTransform()[4];
  }
  dataset->GetDataSet()->SetGeoTransform(geoTransform);

  // Create the output layer for GDALPolygonize().
  ogr::DataSource::Pointer ogrDS = ogr::DataSource::New();

  OGRLayerType outputLayer = ogrDS->CreateLayer("layer", nullptr, wkbPolygon);

  OGRFieldDefn field(m_FieldName.c_str(), OFTInteger);
  outputLayer.CreateField(field, true);

  // Call GDALPolygonize()
  char** options;
  options         = nullptr;
  char* option[2] = {nullptr, nullptr};
  if (m_Use8Connected == true)
  {
    std::string opt("8CONNECTED:8");
    option[0] = const_cast<char*>(opt.c_str());
    options   = option;
  }

  /* Convert the mask input into a GDAL raster needed by GDALPolygonize */
  typename InputImageType::ConstPointer inputMask = this->GetInputMask();
  if (!inputMask.IsNull())
  {
    nbBands      = this->GetInputMask()->GetNumberOfComponentsPerPixel();
    bytePerPixel = sizeof(InputPixelType);

    GDALDatasetWrapper::Pointer maskDataset =
            GDALDriverManagerWrapper::GetInstance().OpenFromMemory(
                const_cast<InputPixelType*>(this->GetInputMask()->GetBufferPointer()),
                this->GetInput()->GetLargestPossibleRegion().GetSize()[0],
                this->GetInput()->GetLargestPossibleRegion().GetSize()[1],
                GdalDataTypeBridge::GetGDALDataType<InputPixelType>(), bytePerPixel, nbBands, bytePerPixel);

    // Set input Projection ref and Geo transform to the dataset.
    maskDataset->GetDataSet()->SetProjection(this->GetInputMask()->GetProjectionRef().c_str());

    projSize = this->GetInputMask()->GetGeoTransform().size();

    // Set the geo transform of the input mask image (if any)
    // Reporting origin and spacing of the buffered region
    // the spacing is unchanged, the origin is relative to the buffered region
    bufferIndexOrigin = this->GetInputMask()->GetBufferedRegion().GetIndex();
    this->GetInputMask()->TransformIndexToPhysicalPoint(bufferIndexOrigin, bufferOrigin);
    geoTransform[0] = bufferOrigin[0] - 0.5 * this->GetInputMask()->GetSignedSpacing()[0];
    geoTransform[3] = bufferOrigin[1] - 0.5 * this->GetInputMask()->GetSignedSpacing()[1];
    geoTransform[1] = this->GetInputMask()->GetSignedSpacing()[0];
    geoTransform[5] = this->GetInputMask()->GetSignedSpacing()[1];
    // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
    if (projSize == 0)
    {
      geoTransform[2] = 0.;
      geoTransform[4] = 0.;
    }
    else
    {
      geoTransform[2] = this->GetInputMask()->GetGeoTransform()[2];
      geoTransform[4] = this->GetInputMask()->GetGeoTransform()[4];
    }
    maskDataset->GetDataSet()->SetGeoTransform(geoTransform);

    GDALPolygonize(dataset->GetDataSet()->GetRasterBand(1),
                   maskDataset->GetDataSet()->GetRasterBand(1),
                   &outputLayer.ogr(), 0,
                   options, nullptr, nullptr);
  }
  else
  {
    GDALPolygonize(dataset->GetDataSet()->GetRasterBand(1), nullptr,
                   &outputLayer.ogr(), 0, options, nullptr, nullptr);
  }

  this->SetNthOutput(0, ogrDS);
}


} // end namespace otb

#endif
