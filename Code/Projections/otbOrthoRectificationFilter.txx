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
#ifndef _otbOrthoRectificationFilter_txx
#define _otbOrthoRectificationFilter_txx

#include "otbOrthoRectificationFilter.h"


namespace otb
{


template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision>
OrthoRectificationFilter<TInputImage, TOutputImage, TMapProjection, TInterpolatorPrecision>
::OrthoRectificationFilter() : otb::StreamingResampleImageFilter<TInputImage, TOutputImage,TInterpolatorPrecision>()
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
::ComputeResampleTransformationModel()
{
	if (m_IsComputed == false)
	{
		otbMsgDevMacro(<< "COMPUTE RESAMPLE TRANSFORMATION MODEL");
		typename TOutputImage::Pointer output = this->GetOutput();
		
		// Get OSSIM sensor model from image keywordlist
 	 	m_SensorModel->SetImageGeometry(output->GetImageKeywordlist());
		
		m_CompositeTransform->SetFirstTransform(m_MapProjection);
		m_CompositeTransform->SetSecondTransform(m_SensorModel);
		
		this->SetTransform(m_CompositeTransform);		
		
		m_IsComputed = true;
	}
}

} //namespace otb

#endif
