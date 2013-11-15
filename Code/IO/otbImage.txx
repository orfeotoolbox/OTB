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
#ifndef __otbImage_txx
#define __otbImage_txx


#include "otbImage.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbImageKeywordlist.h"
#include "itkMetaDataObject.h"

namespace otb
{

template <class TPixel, unsigned int VImageDimension>
Image<TPixel, VImageDimension>::Image()
{
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetProjectionRef(void) const
{
  return (this->GetMetaDataInterface()->GetProjectionRef());
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetGCPProjection(void) const
{
  return (this->GetMetaDataInterface()->GetGCPProjection());
}

template <class TPixel, unsigned int VImageDimension>
unsigned int Image<TPixel, VImageDimension>::GetGCPCount(void) const
{
  return (this->GetMetaDataInterface()->GetGCPCount());
}

template <class TPixel, unsigned int VImageDimension>
OTB_GCP& Image<TPixel, VImageDimension>::GetGCPs(unsigned int GCPnum)
{
  return (this->GetMetaDataInterface()->GetGCPs(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
const OTB_GCP& Image<TPixel, VImageDimension>::GetGCPs(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPs(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetGCPId(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPId(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
std::string Image<TPixel, VImageDimension>::GetGCPInfo(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPInfo(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPRow(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPRow(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPCol(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPCol(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPX(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPX(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPY(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPY(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double Image<TPixel, VImageDimension>::GetGCPZ(unsigned int GCPnum) const
{
  return (this->GetMetaDataInterface()->GetGCPZ(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType
Image<TPixel, VImageDimension>::GetGeoTransform(void) const
{
  return (this->GetMetaDataInterface()->GetGeoTransform());
}

template <class TPixel, unsigned int VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType
Image<TPixel, VImageDimension>::GetUpperLeftCorner(void) const
{
  return (this->GetMetaDataInterface()->GetUpperLeftCorner());
}

template <class TPixel, unsigned int VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType
Image<TPixel, VImageDimension>::GetUpperRightCorner(void) const
{
  return (this->GetMetaDataInterface()->GetUpperRightCorner());
}

template <class TPixel, unsigned int VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType
Image<TPixel, VImageDimension>::GetLowerLeftCorner(void) const
{
  return (this->GetMetaDataInterface()->GetLowerLeftCorner());
}

template <class TPixel, unsigned int VImageDimension>
typename Image<TPixel, VImageDimension>::VectorType
Image<TPixel, VImageDimension>::GetLowerRightCorner(void) const
{
  return (this->GetMetaDataInterface()->GetLowerRightCorner());
}

template <class TPixel, unsigned int VImageDimension>
typename Image<TPixel, VImageDimension>::ImageKeywordlistType
Image<TPixel, VImageDimension>::GetImageKeywordlist(void)
{
    ImageKeywordlist kwl;

  itk::ExposeMetaData<ImageKeywordlist>(this->GetMetaDataDictionary(),
                                        MetaDataKey::OSSIMKeywordlistKey,
                                        kwl);
  return  kwl;
}

template <class TPixel, unsigned int VImageDimension>
const typename Image<TPixel, VImageDimension>::ImageKeywordlistType
Image<TPixel, VImageDimension>::GetImageKeywordlist(void) const
{
  ImageKeywordlist kwl;

  itk::ExposeMetaData<ImageKeywordlist>(this->GetMetaDataDictionary(),
                                        MetaDataKey::OSSIMKeywordlistKey,
                                        kwl);
  return  kwl;
}

template <class TPixel, unsigned int VImageDimension>
void
Image<TPixel, VImageDimension>
::CopyInformation(const itk::DataObject * data)
{
  Superclass::CopyInformation(data);
  this->itk::Object::SetMetaDataDictionary(data->GetMetaDataDictionary());
}

template <class TPixel, unsigned int VImageDimension>
typename Image<TPixel, VImageDimension>::ImageMetadataInterfacePointerType
Image<TPixel, VImageDimension>
::GetMetaDataInterface() const
{
  if (m_ImageMetadataInterface.IsNull())
    m_ImageMetadataInterface = ImageMetadataInterfaceFactory::CreateIMI(this->GetMetaDataDictionary());
  return m_ImageMetadataInterface;
}

template <class TPixel, unsigned int VImageDimension>
void
Image<TPixel, VImageDimension>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  this->GetMetaDataInterface()->PrintSelf(os, indent, this->GetMetaDataDictionary());
}

} // end namespace otb
#endif
