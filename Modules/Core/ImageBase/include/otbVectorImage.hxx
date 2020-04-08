/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
#include "otbImageMetadataInterfaceFactory.h"
#include "otbImageKeywordlist.h"
#include "itkMetaDataObject.h"

namespace otb
{

template <class TPixel, unsigned int VImageDimension>
VectorImage<TPixel, VImageDimension>::VectorImage()
{
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetProjectionRef(void) const
{
  return (this->GetMetaDataInterface()->GetProjectionRef());
}

template <class TPixel, unsigned int VImageDimension>
void VectorImage<TPixel, VImageDimension>::SetProjectionRef(const std::string& kwl)
{
  itk::EncapsulateMetaData<std::string>(this->GetMetaDataDictionary(), MetaDataKey::ProjectionRefKey, kwl);
}


template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPProjection(void) const
{
  return (this->GetMetaDataInterface()->GetGCPProjection());
}

template <class TPixel, unsigned int VImageDimension>
unsigned int VectorImage<TPixel, VImageDimension>::GetGCPCount(void) const
{
  return (this->GetMetaDataInterface()->GetGCPCount());
}

template <class TPixel, unsigned int VImageDimension>
OTB_GCP& VectorImage<TPixel, VImageDimension>::GetGCPs(unsigned int GCPnum)
{
  return (this->GetMetaDataInterface()->GetGCPs(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
const OTB_GCP& VectorImage<TPixel, VImageDimension>::GetGCPs(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPs(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPId(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPId(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPInfo(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPInfo(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPRow(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPRow(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPCol(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPCol(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPX(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPX(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPY(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPY(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPZ(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPZ(GCPnum));
}

template <class TPixel, unsigned int                      VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType VectorImage<TPixel, VImageDimension>::GetGeoTransform(void) const
{
  return (this->GetMetaDataInterface()->GetGeoTransform());
}

template <class TPixel, unsigned int                      VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType VectorImage<TPixel, VImageDimension>::GetUpperLeftCorner(void) const
{
  return (this->GetMetaDataInterface()->GetUpperLeftCorner());
}

template <class TPixel, unsigned int                      VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType VectorImage<TPixel, VImageDimension>::GetUpperRightCorner(void) const
{
  return (this->GetMetaDataInterface()->GetUpperRightCorner());
}

template <class TPixel, unsigned int                      VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType VectorImage<TPixel, VImageDimension>::GetLowerLeftCorner(void) const
{
  return (this->GetMetaDataInterface()->GetLowerLeftCorner());
}

template <class TPixel, unsigned int                      VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType VectorImage<TPixel, VImageDimension>::GetLowerRightCorner(void) const
{
  return (this->GetMetaDataInterface()->GetLowerRightCorner());
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

template <class TPixel, unsigned int                                VImageDimension>
typename VectorImage<TPixel, VImageDimension>::ImageKeywordlistType VectorImage<TPixel, VImageDimension>::GetImageKeywordlist(void)
{
  ImageKeywordlist kwl;

  itk::ExposeMetaData<ImageKeywordlist>(this->GetMetaDataDictionary(), MetaDataKey::OSSIMKeywordlistKey, kwl);
  return kwl;
}

template <class TPixel, unsigned int                                      VImageDimension>
const typename VectorImage<TPixel, VImageDimension>::ImageKeywordlistType VectorImage<TPixel, VImageDimension>::GetImageKeywordlist(void) const
{
  ImageKeywordlist kwl;

  itk::ExposeMetaData<ImageKeywordlist>(this->GetMetaDataDictionary(), MetaDataKey::OSSIMKeywordlistKey, kwl);
  return kwl;
}

template <class TPixel, unsigned int VImageDimension>
void VectorImage<TPixel, VImageDimension>::SetImageKeywordList(const ImageKeywordlistType& kwl)
{
  itk::EncapsulateMetaData<ImageKeywordlistType>(this->GetMetaDataDictionary(), MetaDataKey::OSSIMKeywordlistKey, kwl);
}


template <class TPixel, unsigned int VImageDimension>
void VectorImage<TPixel, VImageDimension>::CopyInformation(const itk::DataObject* data)
{
  Superclass::CopyInformation(data);
  this->itk::Object::SetMetaDataDictionary(data->GetMetaDataDictionary());
}

template <class TPixel, unsigned int                                             VImageDimension>
typename VectorImage<TPixel, VImageDimension>::ImageMetadataInterfacePointerType VectorImage<TPixel, VImageDimension>::GetMetaDataInterface() const
{
  if (m_ImageMetadataInterface.IsNull())
    m_ImageMetadataInterface = ImageMetadataInterfaceFactory::CreateIMI(this->GetMetaDataDictionary());
  return m_ImageMetadataInterface;
}

template <class TPixel, unsigned int VImageDimension>
void VectorImage<TPixel, VImageDimension>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  this->GetMetaDataInterface()->PrintMetadata(os, indent, this->GetMetaDataDictionary());
}

} // end namespace otb
#endif
