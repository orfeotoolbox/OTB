/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbBSplineDecompositionImageFilter_hxx
#define otbBSplineDecompositionImageFilter_hxx
#include "otbBSplineDecompositionImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "itkVector.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::BSplineDecompositionImageFilter()
{
  m_SplineOrder = 0;
  int SplineOrder = 3;
  m_Tolerance = 1e-10;   // Need some guidance on this one...what is reasonable?
  m_IteratorDirection = 0;
  this->SetSplineOrder(SplineOrder);
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Spline Order: " << m_SplineOrder << std::endl;

}

template <class TInputImage, class TOutputImage>
bool
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::DataToCoefficients1D()
{

  // See Unser, 1993, Part II, Equation 2.5,
  //   or Unser, 1999, Box 2. for an explanation.

  double c0 = 1.0;

  if (m_DataLength[m_IteratorDirection] == 1) //Required by mirror boundaries
    {
    return false;
    }

  // Compute overall gain
  for (int k = 0; k < m_NumberOfPoles; ++k)
    {
    // Note for cubic splines lambda = 6
    c0 = c0 * (1.0 - m_SplinePoles[k]) * (1.0 - 1.0 / m_SplinePoles[k]);
    }

  // apply the gain
  for (unsigned int n = 0; n < m_DataLength[m_IteratorDirection]; ++n)
    {
    m_Scratch[n] *= c0;
    }

  // loop over all poles
  for (int k = 0; k < m_NumberOfPoles; ++k)
    {
    // causal initialization
    this->SetInitialCausalCoefficient(m_SplinePoles[k]);
    // causal recursion
    for (unsigned int n = 1; n < m_DataLength[m_IteratorDirection]; ++n)
      {
      m_Scratch[n] += m_SplinePoles[k] * m_Scratch[n - 1];
      }

    // anticausal initialization
    this->SetInitialAntiCausalCoefficient(m_SplinePoles[k]);
    // anticausal recursion
    for (int n = m_DataLength[m_IteratorDirection] - 2; 0 <= n; n--)
      {
      m_Scratch[n] = m_SplinePoles[k] * (m_Scratch[n + 1] - m_Scratch[n]);
      }
    }
  return true;

}

template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::SetSplineOrder(unsigned int SplineOrder)
{
  if (SplineOrder == m_SplineOrder)
    {
    return;
    }
  m_SplineOrder = SplineOrder;
  this->SetPoles();
  this->Modified();

}

template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::SetPoles()
{
  /* See Unser, 1997. Part II, Table I for Pole values */
  // See also, Handbook of Medical Imaging, Processing and Analysis, Ed. Isaac N. Bankman,
  //  2000, pg. 416.
  switch (m_SplineOrder)
    {
    case 3:
      m_NumberOfPoles = 1;
      m_SplinePoles[0] = std::sqrt(3.0) - 2.0;
      break;
    case 0:
      m_NumberOfPoles = 0;
      break;
    case 1:
      m_NumberOfPoles = 0;
      break;
    case 2:
      m_NumberOfPoles = 1;
      m_SplinePoles[0] = std::sqrt(8.0) - 3.0;
      break;
    case 4:
      m_NumberOfPoles = 2;
      m_SplinePoles[0] = std::sqrt(664.0 - std::sqrt(438976.0)) + std::sqrt(304.0) - 19.0;
      m_SplinePoles[1] = std::sqrt(664.0 + std::sqrt(438976.0)) - std::sqrt(304.0) - 19.0;
      break;
    case 5:
      m_NumberOfPoles = 2;
      m_SplinePoles[0] = std::sqrt(135.0 / 2.0 - std::sqrt(17745.0 / 4.0)) + std::sqrt(105.0 / 4.0)
                         - 13.0 / 2.0;
      m_SplinePoles[1] = std::sqrt(135.0 / 2.0 + std::sqrt(17745.0 / 4.0)) - std::sqrt(105.0 / 4.0)
                         - 13.0 / 2.0;
      break;
    default:
      // SplineOrder not implemented yet.
      itk::ExceptionObject err(__FILE__, __LINE__);
      err.SetLocation(ITK_LOCATION);
      err.SetDescription("SplineOrder must be between 0 and 5. Requested spline order has not been implemented yet.");
      throw err;
      break;
    }
}

template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::SetInitialCausalCoefficient(double z)
{
  /* beginning InitialCausalCoefficient */
  /* See Unser, 1999, Box 2 for explanation */

  double        sum, zn, z2n, iz;
  unsigned long horizon;

  /* this initialization corresponds to mirror boundaries */
  horizon = m_DataLength[m_IteratorDirection];
  zn = z;
  if (m_Tolerance > 0.0)
    {
    horizon = (long) std::ceil(log(m_Tolerance) / std::log(fabs(z)));
    }
  if (horizon < m_DataLength[m_IteratorDirection])
    {
    /* accelerated loop */
    sum = m_Scratch[0];   // verify this
    for (unsigned int n = 1; n < horizon; ++n)
      {
      sum += zn * m_Scratch[n];
      zn *= z;
      }
    m_Scratch[0] = sum;
    }
  else
    {
    /* full loop */
    iz = 1.0 / z;
    z2n = std::pow(z, (double) (m_DataLength[m_IteratorDirection] - 1L));
    sum = m_Scratch[0] + z2n * m_Scratch[m_DataLength[m_IteratorDirection] - 1L];
    z2n *= z2n * iz;
    for (unsigned int n = 1; n <= (m_DataLength[m_IteratorDirection] - 2); ++n)
      {
      sum += (zn + z2n) * m_Scratch[n];
      zn *= z;
      z2n *= iz;
      }
    m_Scratch[0] = sum / (1.0 - zn * zn);
    }
}

template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::SetInitialAntiCausalCoefficient(double z)
{
  // this initialization corresponds to mirror boundaries
  /* See Unser, 1999, Box 2 for explanation */
  //  Also see erratum at http://bigwww.epfl.ch/publications/unser9902.html
  m_Scratch[m_DataLength[m_IteratorDirection] - 1] =
    (z / (z * z - 1.0)) *
    (z * m_Scratch[m_DataLength[m_IteratorDirection] - 2] + m_Scratch[m_DataLength[m_IteratorDirection] - 1]);
}

template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::DataToCoefficientsND()
{
  OutputImagePointer output = this->GetOutput();

  itk::Size<ImageDimension> size = output->GetBufferedRegion().GetSize();

  unsigned int count = output->GetBufferedRegion().GetNumberOfPixels() / size[0] * ImageDimension;

  itk::ProgressReporter progress(this, 0, count, 10);

  // Initialize coeffient array
  this->CopyImageToImage();   // Coefficients are initialized to the input data

  for (unsigned int n = 0; n < ImageDimension; ++n)
    {
    m_IteratorDirection = n;
    // Loop through each dimension

    // Initialize iterators
    OutputLinearIterator CIterator(output, output->GetBufferedRegion());
    CIterator.SetDirection(m_IteratorDirection);
    // For each data vector
    while (!CIterator.IsAtEnd())
      {
      // Copy coefficients to scratch
      this->CopyCoefficientsToScratch(CIterator);

      // Perform 1D BSpline calculations
      this->DataToCoefficients1D();

      // Copy scratch back to coefficients.
      // Brings us back to the end of the line we were working on.
      CIterator.GoToBeginOfLine();
      this->CopyScratchToCoefficients(CIterator);   // m_Scratch = m_Image;
      CIterator.NextLine();
      progress.CompletedPixel();
      }
    }
}

/**
 * Copy the input image into the output image
 */
template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::CopyImageToImage()
{

  typedef itk::ImageRegionConstIteratorWithIndex<TInputImage> InputIterator;
  typedef itk::ImageRegionIterator<TOutputImage>              OutputIterator;
  typedef typename TOutputImage::PixelType                    OutputPixelType;

  InputIterator inIt(this->GetInput(), this->GetInput()->GetBufferedRegion());
  OutputIterator outIt(this->GetOutput(), this->GetOutput()->GetBufferedRegion());

  inIt.GoToBegin();
  outIt.GoToBegin();

  while (!outIt.IsAtEnd())
    {
    outIt.Set(static_cast<OutputPixelType>(inIt.Get()));
    ++inIt;
    ++outIt;
    }

}

/**
 * Copy the scratch to one line of the output image
 */
template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::CopyScratchToCoefficients(OutputLinearIterator& Iter)
{
  typedef typename TOutputImage::PixelType OutputPixelType;
  unsigned long j = 0;
  while (!Iter.IsAtEndOfLine())
    {
    Iter.Set(static_cast<OutputPixelType>(m_Scratch[j]));
    ++Iter;
    ++j;
    }

}

/**
 * Copy one line of the output image to the scratch
 */
template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::CopyCoefficientsToScratch(OutputLinearIterator& Iter)
{
  unsigned long j = 0;
  while (!Iter.IsAtEndOfLine())
    {
    m_Scratch[j] = static_cast<double>(Iter.Get());
    ++Iter;
    ++j;
    }
}

/**
 * Generate data
 */
template <class TInputImage, class TOutputImage>
void
BSplineDecompositionImageFilter<TInputImage, TOutputImage>
::GenerateData()
{

  // Allocate scratch memory
  InputImageConstPointer inputPtr = this->GetInput();
  m_DataLength = inputPtr->GetBufferedRegion().GetSize();

  unsigned long maxLength = 0;
  for (unsigned int n = 0; n < ImageDimension; ++n)
    {
    if (m_DataLength[n] > maxLength)
      {
      maxLength = m_DataLength[n];
      }
    }
  m_Scratch.resize(maxLength);

  // Allocate memory for output image
  OutputImagePointer outputPtr = this->GetOutput();
  outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
  outputPtr->Allocate();

  // Calculate actual output
  this->DataToCoefficientsND();

  // Clean up
  m_Scratch.clear();

}

} // namespace otb

#endif
