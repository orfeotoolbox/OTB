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
#ifndef __otbCloudDetectionFilter_txx
#define __otbCloudDetectionFilter_txx

#include "otbCloudDetectionFilter.h"


namespace otb
{

/**
 * Constructor
 */
template <class TInputImage,class TOutputImage>
CloudDetectionFilter<TInputImage,TOutputImage>
::CloudDetectionFilter()
{

  this->SetThreshold(1000);
  this->SetVariance(1);

}

/**
 * Printself
 */
template <class TInputImage,class TOutputImage>
void
CloudDetectionFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{

  this->Superclass::PrintSelf(os,indent);
  os << indent << "Variance  : " << m_Variance  <<std::endl;
  os << indent << "Threshold : " << m_Threshold <<std::endl;

}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage,class TOutputImage >
void
CloudDetectionFilter<TInputImage,TOutputImage>
::BeforeThreadedGenerateData()
{

  unsigned int ReferencePixelNumberOfBands = 0;
  ReferencePixelNumberOfBands = this->GetReferencePixel().GetSize();

  if ( ReferencePixelNumberOfBands != this->GetInput()->GetNumberOfComponentsPerPixel() )
  {
    itkExceptionMacro("The number of bands of the reference pixel is different from the number of bands of the input image. ");
  }

}


}

#endif

