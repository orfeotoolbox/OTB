/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbFastICAInternalOptimizerVectorImageFilter_hxx
#define otbFastICAInternalOptimizerVectorImageFilter_hxx
#include "otbFastICAInternalOptimizerVectorImageFilter.h"

#include <itkMacro.h>
#include <itkImageRegionIterator.h>

#include <vnl/vnl_math.h>
#include <vnl/vnl_matrix.h>

namespace otb
{

template <class TInputImage, class TOutputImage>
FastICAInternalOptimizerVectorImageFilter<TInputImage, TOutputImage>::FastICAInternalOptimizerVectorImageFilter()
{
  this->SetNumberOfRequiredInputs(2);

  m_CurrentBandForLoop = 0;
  m_Beta               = 0.;
  m_Den                = 0.;

  m_NonLinearity           = [](double x) { return std::tanh(x); };
  m_NonLinearityDerivative = [](double x) { return 1 - std::pow(std::tanh(x), 2.); };

  m_TransformFilter = TransformFilterType::New();
}

template <class TInputImage, class TOutputImage>
void FastICAInternalOptimizerVectorImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput(0)->GetNumberOfComponentsPerPixel());
}

template <class TInputImage, class TOutputImage>
void FastICAInternalOptimizerVectorImageFilter<TInputImage, TOutputImage>::Reset()
{
  if (m_W.empty())
  {
    throw itk::ExceptionObject(__FILE__, __LINE__, "Give the initial W matrix", ITK_LOCATION);
  }

  m_BetaVector.resize(this->GetNumberOfThreads());
  m_DenVector.resize(this->GetNumberOfThreads());
  m_NbSamples.resize(this->GetNumberOfThreads());

  std::fill(m_BetaVector.begin(), m_BetaVector.end(), 0.);
  std::fill(m_DenVector.begin(), m_DenVector.end(), 0.);
  std::fill(m_NbSamples.begin(), m_NbSamples.end(), 0.);
}

template <class TInputImage, class TOutputImage>
void FastICAInternalOptimizerVectorImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const OutputRegionType& outputRegionForThread,
                                                                                                itk::ThreadIdType threadId)
{
  InputRegionType inputRegion;
  this->CallCopyOutputRegionToInputRegion(inputRegion, outputRegionForThread);

  itk::ImageRegionConstIterator<InputImageType> input0It(this->GetInput(0), inputRegion);
  itk::ImageRegionConstIterator<InputImageType> input1It(this->GetInput(1), inputRegion);
  itk::ImageRegionIterator<OutputImageType>     outputIt(this->GetOutput(), outputRegionForThread);

  unsigned int nbBands = this->GetInput(0)->GetNumberOfComponentsPerPixel();
  input0It.GoToBegin();
  input1It.GoToBegin();
  outputIt.GoToBegin();

  double beta     = 0.;
  double den      = 0.;
  double nbSample = 0.;

  while (!input0It.IsAtEnd() && !input1It.IsAtEnd() && !outputIt.IsAtEnd())
  {
    double x   = static_cast<double>(input1It.Get()[GetCurrentBandForLoop()]);
    double g_x = m_NonLinearity(x);

    double x_g_x = x * g_x;
    beta += x_g_x;

    double gp = m_NonLinearityDerivative(x);
    den += gp;

    nbSample += 1.;

    typename OutputImageType::PixelType z(nbBands);
    for (unsigned int bd = 0; bd < nbBands; bd++)
      z[bd]              = g_x * input0It.Get()[bd];
    outputIt.Set(z);

    ++input0It;
    ++input1It;
    ++outputIt;
  } // end while loop

  m_BetaVector[threadId] += beta;
  m_DenVector[threadId] += den;
  m_NbSamples[threadId] += nbSample;
}

template <class TInputImage, class TOutputImage>
void FastICAInternalOptimizerVectorImageFilter<TInputImage, TOutputImage>::Synthetize()
{
  double beta     = 0;
  double den      = 0.;
  double nbSample = 0;

  for (itk::ThreadIdType i = 0; i < this->GetNumberOfThreads(); ++i)
  {
    beta += m_BetaVector[i];
    den += m_DenVector[i];
    nbSample += m_NbSamples[i];
  }

  m_Beta = beta / nbSample;
  m_Den  = den / nbSample - m_Beta;
}

} // end of namespace otb

#endif
