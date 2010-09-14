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
#ifndef __otbOrthoRectificationFilter_txx
#define __otbOrthoRectificationFilter_txx

#include "otbOrthoRectificationFilter.h"
#include "itkMetaDataObject.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision>
OrthoRectificationFilter<TInputImage, TOutputImage, TMapProjection, TInterpolatorPrecision>
::OrthoRectificationFilter()
{
  m_MapProjection = MapProjectionType::New();
  m_IsComputed = false;
}

template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision>
OrthoRectificationFilter<TInputImage, TOutputImage, TMapProjection, TInterpolatorPrecision>
::~OrthoRectificationFilter()
{
}

template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision>
void OrthoRectificationFilter<TInputImage, TOutputImage, TMapProjection, TInterpolatorPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "OrthoRectification" << "\n";
}

template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision>
void
OrthoRectificationFilter<TInputImage, TOutputImage, TMapProjection, TInterpolatorPrecision>
::GenerateInputRequestedRegion()
{
  this->ComputeResampleTransformationModel();
  Superclass::GenerateInputRequestedRegion();
}

template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision>
void
OrthoRectificationFilter<TInputImage, TOutputImage, TMapProjection, TInterpolatorPrecision>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // fill up the metadata information for ProjectionRef
  itk::MetaDataDictionary&  dict          = this->GetOutput()->GetMetaDataDictionary();
  std::string               projectionRef = m_MapProjection->GetWkt();
  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);

  // Fill the GenericRSTransform with those information
  this->SetOutputProjectionRef(projectionRef);
}

template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision>
void
OrthoRectificationFilter<TInputImage, TOutputImage, TMapProjection, TInterpolatorPrecision>
::ComputeResampleTransformationModel()
{
  if (m_IsComputed == false)
    {
    this->SetInputKeywordList(this->GetInput()->GetImageKeywordlist());
    m_IsComputed = true;
    }
}

} //namespace otb

#endif
