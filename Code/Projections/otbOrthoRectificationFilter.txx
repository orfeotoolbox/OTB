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
  
template <      class TInputImage, 
	    	class TOutputImage, 
	    	class TMapProjection, 
	    	class TInterpolatorPrecision>
OrthoRectificationFilter<       TInputImage, 
			   	TOutputImage, 
			   	TMapProjection, 
			   	TInterpolatorPrecision>
::OrthoRectificationFilter() : itk::ImageToImageFilter<TInputImage, TOutputImage>()
{
        m_OrthoRectificationFilter=OrthoRectificationFilterBaseType::New();
        m_ChangeInfoFilter = ChangeInfoFilterType::New();
        m_OrthoRectificationFilter->SetInput(m_ChangeInfoFilter->GetOutput());
	PointType originNull;
	originNull.Fill(0);
        m_ChangeInfoFilter->ChangeOriginOn();
	m_ChangeInfoFilter->SetOutputOrigin(originNull);
	SpacingType spacing;
	spacing.Fill(1);
	m_ChangeInfoFilter->SetOutputSpacing(spacing);
}
  
template <      class TInputImage, 
	    	class TOutputImage, 
	    	class TMapProjection, 
	    	class TInterpolatorPrecision>
OrthoRectificationFilter<       TInputImage, 
			   	TOutputImage, 
			   	TMapProjection, 
			   	TInterpolatorPrecision>
::~OrthoRectificationFilter()
{

}
  
template <      class TInputImage, 
	    	class TOutputImage, 
	    	class TMapProjection, 
	    	class TInterpolatorPrecision>
void
OrthoRectificationFilter<       TInputImage, 
			   	TOutputImage, 
			   	TMapProjection, 
			   	TInterpolatorPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
        Superclass::PrintSelf(os, indent);
        os << indent << "m_OrthoRectification:\n" << m_OrthoRectificationFilter<<"\n";
        os << indent << "m_ChangeInfoFilter:\n" << m_ChangeInfoFilter <<"\n";
}

template <      class TInputImage, 
	    	class TOutputImage, 
	    	class TMapProjection, 
	    	class TInterpolatorPrecision>
void
OrthoRectificationFilter<       TInputImage, 
			   	TOutputImage, 
			   	TMapProjection, 
			   	TInterpolatorPrecision>
::GenerateOutputInformation()
{
        // This must be done so that the orthoRectificationFilter can generate its ouptut information correctly.
        m_ChangeInfoFilter->SetInput(this->GetInput());
        m_OrthoRectificationFilter->GenerateOutputInformation();
        this->GetOutput()->CopyInformation(m_OrthoRectificationFilter->GetOutput());
}




template <      class TInputImage, 
	    	class TOutputImage, 
	    	class TMapProjection, 
	    	class TInterpolatorPrecision>
void
OrthoRectificationFilter<       TInputImage, 
			   	TOutputImage, 
			   	TMapProjection, 
			   	TInterpolatorPrecision>
::GenerateData(void)
{
        // This is done here instead of inside the GenerateData() method so that the pipeline negociation
        // use the minipipeline instead of default ITK methods.
        m_OrthoRectificationFilter->GraftOutput(this->GetOutput());
        //m_OrthoRectificationFilter->GetOutput()->UpdateOutputInformation();
        //m_OrthoRectificationFilter->GetOutput()->PropagateRequestedRegion();
        //m_OrthoRectificationFilter->GetOutput()->UpdateOutputData();
        m_OrthoRectificationFilter->Update();
        this->GraftOutput(m_OrthoRectificationFilter->GetOutput());
        //m_OrthoRectificationFilter->Update();
}
  
} //namespace otb

#endif
