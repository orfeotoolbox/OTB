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
#ifndef otbCloudEstimatorFilter_txx
#define otbCloudEstimatorFilter_txx

#include "otbCloudEstimatorFilter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::CloudEstimatorFilter()
{
}

/**
 * Printself
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
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
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::SetVariance(double var)
{
  this->GetFunctor().SetVariance(var);
  this->Modified();
}

/**
 * GetVariance
 */
template <class TInputImage, class TOutputImage, class TFunction>
double
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::GetVariance() const
{
  return this->GetFunctor().GetVariance();
}

/**
 * SetReferencePixel
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::SetReferencePixel(InputPixelType ref)
{
  this->GetFunctor().SetReferencePixel(ref);
  this->Modified();
}

/**
 * GetReferencePixel
 */
template <class TInputImage, class TOutputImage, class TFunction>
typename CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>::InputPixelType
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::GetReferencePixel() const
{
  return this->GetFunctor().GetReferencePixel();
}

}

#endif
