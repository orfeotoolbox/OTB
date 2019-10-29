/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbLineRatioDetectorImageFilter_hxx
#define otbLineRatioDetectorImageFilter_hxx

#include "otbLineRatioDetectorImageFilter.h"
#include "otbMacro.h"

#include "itkDataObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
LineRatioDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>::LineRatioDetectorImageFilter()
{
  this->m_Radius.Fill(1);
  this->m_LengthLine = 1;
  this->m_WidthLine  = 0;
  this->m_FaceList.Fill(0);
}

template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
double LineRatioDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>::ComputeMeasure(std::vector<double>* m1,
                                                                                                                     std::vector<double>* m2,
                                                                                                                     std::vector<double>* m3)
{

  double M1 = 0.0;
  double M2 = 0.0;
  double M3 = 0.0;

  std::vector<double>::iterator m1It  = m1->begin();
  std::vector<double>::iterator m1End = m1->end();

  std::vector<double>::iterator m2It  = m2->begin();
  std::vector<double>::iterator m2End = m2->end();

  std::vector<double>::iterator m3It  = m3->begin();
  std::vector<double>::iterator m3End = m3->end();

  while (m1It != m1End && m2It != m2End && m3It != m3End)
  {

    M1 += (*m1It);
    ++m1It;

    M2 += (*m2It);
    ++m2It;

    M3 += (*m3It);
    ++m3It;
  }

  M1 /= m1->size();
  M2 /= m2->size();
  M3 /= m3->size();

  double R12 = 0.0;
  double R13 = 0.0;

  double epsilon = 0.0000000001;
  if ((std::abs(M1) > epsilon) && (std::abs(M2) > epsilon))
    R12 = static_cast<double>(1 - std::min((M1 / M2), (M2 / M1)));
  else if ((std::abs(M1) > epsilon) || (std::abs(M2) > epsilon))
    R12 = 1.0;
  else
    R12 = 0.;

  if ((std::abs(M1) > epsilon) && (std::abs(M3) > epsilon))
    R13 = static_cast<double>(1 - std::min((M1 / M3), (M3 / M1)));
  else if ((std::abs(M1) > epsilon) || (std::abs(M3) > epsilon))
    R13 = 1.0;
  else
    R13 = 0.;

  // Determination of the minimum intensity of detection between R12 et R13
  return static_cast<double>(std::min(R12, R13));
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
void LineRatioDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
