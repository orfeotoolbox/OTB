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
#ifndef __otbVectorImage_txx
#define __otbVectorImage_txx

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbVectorImage.h"
#include "otbImageMetadataInterfaceFactory.h"

namespace otb
{

template <class TPixel, unsigned int VImageDimension>
VectorImage<TPixel, VImageDimension>::VectorImage()
{
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetProjectionRef(void) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetProjectionRef());
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPProjection(void) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPProjection());
}

template <class TPixel, unsigned int VImageDimension>
unsigned int VectorImage<TPixel, VImageDimension>::GetGCPCount(void) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPCount());
}

template <class TPixel, unsigned int VImageDimension>
OTB_GCP& VectorImage<TPixel, VImageDimension>::GetGCPs(unsigned int GCPnum)
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPs(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPId(unsigned int GCPnum) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPId(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
std::string VectorImage<TPixel, VImageDimension>::GetGCPInfo(unsigned int GCPnum) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPInfo(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPRow(unsigned int GCPnum) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPRow(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPCol(unsigned int GCPnum) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPCol(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPX(unsigned int GCPnum) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPX(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPY(unsigned int GCPnum) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPY(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
double VectorImage<TPixel, VImageDimension>::GetGCPZ(unsigned int GCPnum) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGCPZ(GCPnum));
}

template <class TPixel, unsigned int VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType
VectorImage<TPixel, VImageDimension>::GetGeoTransform(void) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetGeoTransform());
}

template <class TPixel, unsigned int VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType
VectorImage<TPixel, VImageDimension>::GetUpperLeftCorner(void) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetUpperLeftCorner());
}

template <class TPixel, unsigned int VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType
VectorImage<TPixel, VImageDimension>::GetUpperRightCorner(void) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetUpperRightCorner());
}

template <class TPixel, unsigned int VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType
VectorImage<TPixel, VImageDimension>::GetLowerLeftCorner(void) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetLowerLeftCorner());
}

template <class TPixel, unsigned int VImageDimension>
typename VectorImage<TPixel, VImageDimension>::VectorType
VectorImage<TPixel, VImageDimension>::GetLowerRightCorner(void) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetLowerRightCorner());
}

template <class TPixel, unsigned int VImageDimension>
typename VectorImage<TPixel, VImageDimension>::ImageKeywordlistType
VectorImage<TPixel, VImageDimension>::GetImageKeywordlist(void)
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetImageKeywordlist());
}

template <class TPixel, unsigned int VImageDimension>
const typename VectorImage<TPixel, VImageDimension>::ImageKeywordlistType
VectorImage<TPixel, VImageDimension>::GetImageKeywordlist(void) const
{
  if (m_ImageMetadataInterface.IsNull())
    itkExceptionMacro("No metadata currently available");
  return (m_ImageMetadataInterface->GetImageKeywordlist());
}

template <class TPixel, unsigned int VImageDimension>
void
VectorImage<TPixel, VImageDimension>
::CopyInformation(const itk::DataObject * data)
{
  Superclass::CopyInformation(data);
  this->itk::Object::SetMetaDataDictionary(data->GetMetaDataDictionary());
}

template <class TPixel, unsigned int VImageDimension>
typename VectorImage<TPixel, VImageDimension>::ImageMetadataInterfacePointerType
VectorImage<TPixel, VImageDimension>
::GetMetaDataInterface() const
{
  if (m_ImageMetadataInterface.IsNull())
    m_ImageMetadataInterface = ImageMetadataInterfaceFactory::CreateIMI(this->GetMetaDataDictionary());
  return m_ImageMetadataInterface;
}

template <class TPixel, unsigned int VImageDimension>
void
VectorImage<TPixel, VImageDimension>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  m_ImageMetadataInterface->PrintSelf(os, indent, this->GetMetaDataDictionary());
}

} // end namespace otb
#endif
