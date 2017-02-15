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
#ifndef otbCloudDetectionFilter_txx
#define otbCloudDetectionFilter_txx

#include "otbCloudDetectionFilter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::CloudDetectionFilter()
{
}

/**
 * Printself
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::BeforeThreadedGenerateData()
{
  unsigned int ReferencePixelNumberOfBands = 0;
  ReferencePixelNumberOfBands = this->GetReferencePixel().GetSize();

  if (ReferencePixelNumberOfBands != this->GetInput()->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(
      "The number of bands of the reference pixel is different from the number of bands of the input image. ");
    }
}

/**
 * SetVariance
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::SetVariance(double var)
{
  this->GetFunctor().SetVariance(var);
}

/**
 * GetVariance
 */
template <class TInputImage, class TOutputImage, class TFunction>
double
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::GetVariance()
{
  return this->GetFunctor().GetVariance();
}

/**
 * SetReferencePixel
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::SetReferencePixel(InputPixelType ref)
{
  this->GetFunctor().SetReferencePixel(ref);
}

/**
 * GetReferencePixel
 */
template <class TInputImage, class TOutputImage, class TFunction>
typename CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::InputPixelType
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::GetReferencePixel()
{
  return this->GetFunctor().GetReferencePixel();
}

/**
 * SetMinThreshold
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::SetMinThreshold(double threshold)
{
  this->GetFunctor().SetMinThreshold(threshold);
}

/**
 * SetMaxThreshold
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::SetMaxThreshold(double threshold)
{
  this->GetFunctor().SetMaxThreshold(threshold);
}

/**
 * GetMinThreshold
 */
template <class TInputImage, class TOutputImage, class TFunction>
double
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::GetMinThreshold()
{
  return this->GetFunctor().GetMinThreshold();
}

/**
 * GetMaxThreshold
 */
template <class TInputImage, class TOutputImage, class TFunction>
double
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>
::GetMaxThreshold()
{
  return this->GetFunctor().GetMaxThreshold();
}

}

#endif
