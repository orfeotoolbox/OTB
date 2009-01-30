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
::OrthoRectificationFilter() : StreamingResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>()
{

  m_SensorModel = SensorModelType::New();
  m_MapProjection = MapProjectionType::New();
  m_CompositeTransform = CompositeTransformType::New();
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
  typename TOutputImage::Pointer output = this->GetOutput();
  itk::MetaDataDictionary & dict = output->GetMetaDataDictionary();

  std::string projectionRef = m_MapProjection->GetWkt();

  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef );

}


template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision>
void
OrthoRectificationFilter<TInputImage, TOutputImage, TMapProjection, TInterpolatorPrecision>
::ComputeResampleTransformationModel()
{
  if (m_IsComputed == false)
  {
    otbMsgDevMacro(<< "COMPUTE RESAMPLE TRANSFORMATION MODEL");

    typename TInputImage::ConstPointer input = this->GetInput();

    // Get OSSIM sensor model from image keywordlist
    m_SensorModel->SetImageGeometry(input->GetImageKeywordlist());

    // Initialize Map Projection if needed
    /*     PointType geoPoint;
          geoPoint[1] = this->GetOutputOrigin()[1] + this->GetSize()[0]*this->GetOutputSpacing()[0]/2;
          geoPoint[0] = this->GetOutputOrigin()[0] + this->GetSize()[1]*this->GetOutputSpacing()[1]/2;
          m_MapProjection->Initialize(geoPoint);      */

    m_CompositeTransform->SetFirstTransform(m_MapProjection);
    m_CompositeTransform->SetSecondTransform(m_SensorModel);
    this->SetTransform(m_CompositeTransform);
    m_IsComputed = true;
  }
}

} //namespace otb

#endif
