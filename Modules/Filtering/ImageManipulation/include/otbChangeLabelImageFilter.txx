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
#ifndef __otbChangeLabelImageFilter_txx
#define __otbChangeLabelImageFilter_txx

#include "otbChangeLabelImageFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
ChangeLabelImageFilter<TInputImage, TOutputImage>
::ChangeLabelImageFilter()
  : m_NumberOfComponentsPerPixel(0)
{
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ChangeLabelImageFilter<TInputImage, TOutputImage>
::SetChange(const InputPixelType& original, const OutputPixelType& result)
{
  OutputPixelType current = this->GetFunctor().GetChange(original);
  if (current != result)
    {
    this->GetFunctor().SetChange(original, result);
    this->Modified();
    }
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ChangeLabelImageFilter<TInputImage, TOutputImage>
::SetChangeMap(const ChangeMapType& changeMap)
{
  //If the whole map is being set then we assume that a real change is made
  this->GetFunctor().SetChangeMap(changeMap);
  this->Modified();
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ChangeLabelImageFilter<TInputImage, TOutputImage>
::ClearChangeMap()
{
  //If the whole map is being set then we assume that a real change is made
  this->GetFunctor().ClearChangeMap();
  this->Modified();
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ChangeLabelImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ChangeLabelImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of components per pixel: " << m_NumberOfComponentsPerPixel;

}

} // end namespace

#endif
