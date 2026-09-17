/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbImage_hxx
#define otbImage_hxx


#include "otbImage.h"
#include "itkMetaDataObject.h"
#include <cassert>
#include <stdexcept>

namespace otb
{

template <class TPixel, unsigned int VImageDimension>
Image<TPixel, VImageDimension>::Image()
{
}


template <class TPixel, unsigned int                 VImageDimension>
typename Image<TPixel, VImageDimension>::SpacingType Image<TPixel, VImageDimension>::GetSignedSpacing() const
{
  auto spacing = this->GetSpacing();
  for (unsigned int i = 0; i < Image::ImageDimension; ++i)
  {
    if (this->m_Direction[i][i] < 0)
      spacing[i] = -spacing[i];
  }
  return spacing;
}

template <class TPixel, unsigned int VImageDimension>
void Image<TPixel, VImageDimension>::SetSignedSpacing(SpacingType spacing)
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
void Image<TPixel, VImageDimension>::SetSignedSpacing(double spacing[VImageDimension])
{
  SpacingType s(spacing);
  this->SetSignedSpacing(s);
}

template <class TPixel, unsigned int VImageDimension>
void Image<TPixel, VImageDimension>::CopyInformation(const itk::DataObject* data)
{
  Superclass::CopyInformation(data);
  this->itk::Object::SetMetaDataDictionary(data->GetMetaDataDictionary());
  const ImageCommons* imc = dynamic_cast<const ImageCommons*>(data);
  if (imc != nullptr)
  {
    const auto & imd = imc->GetImageMetadata();

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

namespace details
{

/** Everything here is meant to work around images of 1 dimension in C++ < 17.
 * Indeed, `ROIdataConversion` defines `Image<PixelType, 1>`, but `otb::Image::GetGeoTransform`
 * always accesses origin, spacing and direction at second index while these arrays will have a size
 * of one.
 * Incurs a very troubling warning.
 *
 * A simple, C++17, solution would have been:
 * @code
 * if constexpr(VImageDimension < 2)
 *     throw std::logic_error("this cannot work");
 * @endcode
 *
 * Unfortunately OTB is still in C++14.
 *
 * As this workaround is very specific to this usage, it's kept in `otbImage.hxx`.
 * Let's deprecate it in C++17.
 */
namespace emulate_cpp17
{
template <bool Cond>
struct StaticAssume {
  static constexpr void OrThrow() {}
};

template <>
struct StaticAssume<false> {
  static void OrThrow() {
    assert(!"Unexpected situation: Image<TP, 1>::GetGeoTransform() cannot work");
    throw std::logic_error("Unexpected situation: Image<TP, 1>::GetGeoTransform() cannot work");
  }
};
} // emulate_cpp17 namespace

} // details namespace

template <class TPixel, unsigned int                VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType Image<TPixel, VImageDimension>::GetGeoTransform(void) const
{
  details::emulate_cpp17::StaticAssume<VImageDimension >=2>::OrThrow();
  Image<TPixel, VImageDimension>::VectorType geoTransform(6);

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
  return geoTransform;
}


template <class TPixel, unsigned int                VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType
Image<TPixel, VImageDimension>::GetUpperLeftCorner(void) const
{
  PointType physicalPoint;
  itk::ContinuousIndex<double, VImageDimension> index;
  index.Fill(-0.5);
  this->TransformContinuousIndexToPhysicalPoint (index, physicalPoint) ;
  return {physicalPoint[0],physicalPoint[1]};
}

template <class TPixel, unsigned int                VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType
Image<TPixel, VImageDimension>::GetUpperRightCorner(void) const
{
  PointType physicalPoint;
  itk::ContinuousIndex<double, VImageDimension> index;
  index.Fill(-0.5);
  index[0] = -0.5 + this->GetLargestPossibleRegion().GetSize()[0];
  this->TransformContinuousIndexToPhysicalPoint (index, physicalPoint) ;
  return {physicalPoint[0],physicalPoint[1]};
}

template <class TPixel, unsigned int                VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType
Image<TPixel, VImageDimension>::GetLowerLeftCorner(void) const
{
  PointType physicalPoint;
  itk::ContinuousIndex<double, VImageDimension> index;
  index.Fill(-0.5);
  index[1] = -0.5 + this->GetLargestPossibleRegion().GetSize()[1];
  this->TransformContinuousIndexToPhysicalPoint (index, physicalPoint) ;
  return {physicalPoint[0],physicalPoint[1]};
}

template <class TPixel, unsigned int                VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType
Image<TPixel, VImageDimension>::GetLowerRightCorner(void) const
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
void Image<TPixel, VImageDimension>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << this->GetImageMetadata();
}

} // end namespace otb
#endif
