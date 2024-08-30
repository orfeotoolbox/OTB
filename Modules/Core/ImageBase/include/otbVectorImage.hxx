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

#ifndef otbVectorImage_hxx
#define otbVectorImage_hxx


#include "otbVectorImage.h"
#include "itkMetaDataObject.h"

namespace otb
{

template <class TPixel, unsigned int VImageDimension>
VectorImage<TPixel, VImageDimension>::VectorImage()
{
}


template <class TPixel, unsigned int                       VImageDimension>
typename VectorImage<TPixel, VImageDimension>::SpacingType VectorImage<TPixel, VImageDimension>::GetSignedSpacing() const
{
  auto spacing = this->GetSpacing();
  for (unsigned int i = 0; i < VImageDimension; ++i)
  {
    if (this->m_Direction[i][i] < 0)
      spacing[i] = -spacing[i];
  }
  return spacing;
}

template <class TPixel, unsigned int VImageDimension>
void VectorImage<TPixel, VImageDimension>::SetSignedSpacing(SpacingType spacing)
{

  for (unsigned int i = 0; i < VImageDimension; i++)
  {
    if (spacing[i] < 0.0)
    {
      if (this->m_Direction[i][i] > 0)
      {
        for (unsigned j = 0; j < VImageDimension; ++j)
        {
          this->m_Direction[j][i] = -this->m_Direction[j][i];
        }
      }
      spacing[i] = -spacing[i];
    }
  }
  this->SetSpacing(spacing);
  this->ComputeIndexToPhysicalPointMatrices();
  this->Modified();
}

template <class TPixel, unsigned int VImageDimension>
void VectorImage<TPixel, VImageDimension>::SetSignedSpacing(double spacing[VImageDimension])
{
  SpacingType s(spacing);
  this->SetSignedSpacing(s);
}

template <class TPixel, unsigned int VImageDimension>
void VectorImage<TPixel, VImageDimension>::CopyInformation(const itk::DataObject* data)
{
  Superclass::CopyInformation(data);
  this->itk::Object::SetMetaDataDictionary(data->GetMetaDataDictionary());
  const ImageCommons* imc = dynamic_cast<const ImageCommons*>(data);
  if (imc != nullptr)
  {
    const auto & imd = imc->GetImageMetadata();
    // std::cout << "hello " << this->GetNumberOfComponentsPerPixel() << std::endl;
    if (imd.Bands.size() > 0 && imd.Bands.size() != this->GetNumberOfComponentsPerPixel())
    {
      SetImageMetadata(ImageMetadata(imd.GeometryKeys, imd.NumericKeys, imd.StringKeys, imd.LUT1DKeys,
                    imd.LUT2DKeys, imd.TimeKeys, imd.ExtraKeys, ImageMetadata::ImageMetadataBandsType(this->GetNumberOfComponentsPerPixel())));
    }
    else
    {
      SetImageMetadata(imd);
    }
  }
}


template< typename TPixel, unsigned int VImageDimension >
void
VectorImage< TPixel, VImageDimension >::SetNumberOfComponentsPerPixel(unsigned int n)
{
  if (this->GetNumberOfComponentsPerPixel() != n)
  {
    SetBandImageMetadata(ImageMetadata::ImageMetadataBandsType(n));
  }
  
  Superclass::SetNumberOfComponentsPerPixel(n);
}

template <class TPixel, unsigned int                VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType VectorImage<TPixel, VImageDimension>::GetGeoTransform(void) const
{
  VectorImage<TPixel, VImageDimension>::VectorType geoTransform(6);
  
  auto origin = this->GetOrigin();
  auto spacing = this->GetSpacing();
  auto direction = this->GetDirection();
  
  // Note : OTB does not handle rotated geoTransform (see GDALImageIO)
  geoTransform[0] = origin[0] - 0.5 * spacing[0] * direction[0][0];
  geoTransform[3] = origin[1] - 0.5 * spacing[1] * direction[1][1];
  geoTransform[1] = spacing[0] * direction[0][0];
  geoTransform[5] = spacing[1] * direction[1][1];
  geoTransform[2] = 0.;
  geoTransform[4] = 0.;
  return (geoTransform);
}


template <class TPixel, unsigned int                VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType 
VectorImage<TPixel, VImageDimension>::GetUpperLeftCorner(void) const
{
  PointType physicalPoint;
  itk::ContinuousIndex<double, VImageDimension> index;
  index.Fill(-0.5);
  this->TransformContinuousIndexToPhysicalPoint (index, physicalPoint) ;
  return {physicalPoint[0],physicalPoint[1]};
}

template <class TPixel, unsigned int                VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType 
VectorImage<TPixel, VImageDimension>::GetUpperRightCorner(void) const
{
  PointType physicalPoint;
  itk::ContinuousIndex<double, VImageDimension> index;
  index.Fill(-0.5);
  index[0] = -0.5 + this->GetLargestPossibleRegion().GetSize()[0];
  this->TransformContinuousIndexToPhysicalPoint (index, physicalPoint) ;
  return {physicalPoint[0],physicalPoint[1]};
}

template <class TPixel, unsigned int                VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType 
VectorImage<TPixel, VImageDimension>::GetLowerLeftCorner(void) const
{
  PointType physicalPoint;
  itk::ContinuousIndex<double, VImageDimension> index;
  index.Fill(-0.5);
  index[1] = -0.5 + this->GetLargestPossibleRegion().GetSize()[1];
  this->TransformContinuousIndexToPhysicalPoint (index, physicalPoint) ;
  return {physicalPoint[0],physicalPoint[1]};
}

template <class TPixel, unsigned int                VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType 
VectorImage<TPixel, VImageDimension>::GetLowerRightCorner(void) const
{
  PointType physicalPoint;
  itk::ContinuousIndex<double, VImageDimension> index;
  index.Fill(-0.5);
  index[0] = -0.5 + this->GetLargestPossibleRegion().GetSize()[0];
  index[1] = -0.5 + this->GetLargestPossibleRegion().GetSize()[1];
  this->TransformContinuousIndexToPhysicalPoint (index, physicalPoint) ;
  return {physicalPoint[0],physicalPoint[1]};
}

template <class TPixel, unsigned int VImageDimension>
void VectorImage<TPixel, VImageDimension>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << this->GetImageMetadata();
}

} // end namespace otb
#endif
