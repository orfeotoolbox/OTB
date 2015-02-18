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
#ifndef __otbGeodesicMorphologyDecompositionImageFilter_txx
#define __otbGeodesicMorphologyDecompositionImageFilter_txx

#include "otbGeodesicMorphologyDecompositionImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>
::GeodesicMorphologyDecompositionImageFilter()
{
  this->SetNumberOfRequiredOutputs(3);
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputImageType::New());

  m_Radius.Fill(1);

  m_OpeningFilter  = OpeningFilterType::New();
  m_ClosingFilter  = ClosingFilterType::New();
  m_LevelingFilter = LevelingFilterType::New();
  m_ConvexFilter   = ConvexFilterType::New();
  m_ConcaveFilter  = ConcaveFilterType::New();

  m_FullyConnected = true;
  m_PreserveIntensities = true;
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
void
GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>
::GenerateData()
{
  StructuringElementType se;
  se.SetRadius(m_Radius);
  se.CreateStructuringElement();

  m_OpeningFilter->SetInput(this->GetInput());
  m_OpeningFilter->SetKernel(se);
  m_OpeningFilter->SetPreserveIntensities(m_PreserveIntensities);
  m_OpeningFilter->SetFullyConnected(m_FullyConnected);

  m_ClosingFilter->SetInput(this->GetInput());
  m_ClosingFilter->SetKernel(se);
  m_ClosingFilter->SetPreserveIntensities(m_PreserveIntensities);
  m_ClosingFilter->SetFullyConnected(m_FullyConnected);

  m_ConvexFilter->SetInput1(this->GetInput());
  m_ConvexFilter->SetInput2(m_OpeningFilter->GetOutput());

  m_ConcaveFilter->SetInput1(m_ClosingFilter->GetOutput());
  m_ConcaveFilter->SetInput2(this->GetInput());

  m_LevelingFilter->SetInput(this->GetInput());
  m_LevelingFilter->SetInputConvexMap(m_ConvexFilter->GetOutput());
  m_LevelingFilter->SetInputConcaveMap(m_ConcaveFilter->GetOutput());

  m_ConvexFilter->GraftOutput(this->GetConvexMap());
  m_ConvexFilter->Update();
  this->GraftNthOutput(1, m_ConvexFilter->GetOutput());

  m_ConcaveFilter->GraftOutput(this->GetConcaveMap());
  m_ConcaveFilter->Update();
  this->GraftNthOutput(2, m_ConcaveFilter->GetOutput());

  m_LevelingFilter->GraftOutput(this->GetOutput());
  m_LevelingFilter->Update();
  this->GraftOutput(m_LevelingFilter->GetOutput());
}

/**
 * Get the convex map
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
TOutputImage *
GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>
::GetConvexMap()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageType *>
           (this->itk::ProcessObject::GetOutput(1));
}

/**
 * Get the concave map
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
TOutputImage *
GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>
::GetConcaveMap()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<OutputImageType *>
           (this->itk::ProcessObject::GetOutput(2));
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
void
GeodesicMorphologyDecompositionImageFilter<TInputImage, TOutputImage, TStructuringElement>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
