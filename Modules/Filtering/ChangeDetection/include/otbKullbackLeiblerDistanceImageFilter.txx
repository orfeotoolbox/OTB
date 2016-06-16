/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbKullbackLeiblerDistanceImageFilter_txx
#define otbKullbackLeiblerDistanceImageFilter_txx

#include <vector>

#include "itkImageRegionConstIterator.h"

#include "otbMacro.h"

namespace otb
{

/* *******************************************************************
 *  CumulantsForEdgeworth
 *********************************************************************
 */
template <class TInput>
CumulantsForEdgeworth<TInput>
::CumulantsForEdgeworth (const TInput& input)
{
  MakeSumAndMoments(input);
  MakeCumulants();
}

template <class TInput>
CumulantsForEdgeworth<TInput>
::CumulantsForEdgeworth (const itk::Image<typename TInput::ImageType::PixelType, 1> * input)
{
  MakeSumAndMoments(input);
  MakeCumulants();
}

/* ========================== Divergence de KL ======================= */

template <class TInput>
template <class TInput2>
double
CumulantsForEdgeworth<TInput>
::Divergence(const CumulantsForEdgeworth<TInput2>& cumulants)
{
  double cum1 = GetMean();
  double cum2 = GetVariance();
  double cum3 = GetSkewness();

  double sqrt_cum2 = sqrt(cum2);
  double cum2_3 = cum2 * cum2 * cum2;
  double cum3_2 = cum3 * cum3;

  double tilde_cum1 = cumulants.GetMean();
  double tilde_cum2 = cumulants.GetVariance();
  double tilde_cum3 = cumulants.GetSkewness();
  double tilde_cum4 = cumulants.GetKurtosis();

  double tilde_cum2_2 = cum2 * cum2;
  double tilde_cum2_3 = cum2 * tilde_cum2_2;
  double tilde_cum2_6 = tilde_cum2_3 * tilde_cum2_3;
  double tilde_cum3_2 = tilde_cum3 * tilde_cum3;

  double beta = sqrt_cum2 / tilde_cum2;
  double alpha = (cum1 - tilde_cum1) / tilde_cum2;

  double alpha_2 = alpha * alpha;
  double alpha_4 = alpha_2 * alpha_2;
  double alpha_6 = alpha_2 * alpha_4;

  double beta_2 = beta * beta;
  double beta_4 = beta_2 * beta_2;
  double beta_6 = beta_2 * beta_4;

  double c2 = alpha_2 + beta_2;
  double c3 = alpha * (alpha_2 + 3.0 * beta_2);
  double c4 = alpha_4 + 6.0 * alpha_2 * beta_2 + 3.0 * beta_4;
  double c6 = alpha_6 + 15.0 * alpha_4 * beta_2 + 45.0 * alpha_2 * beta_4 + 15.0 * beta_6;

  double a1 = c3 - 3.0 * alpha / tilde_cum2;
  double a2 = c4 - 6.0 * c2 / tilde_cum2 + 3.0 / tilde_cum2_2; // Watch for tilde_cum2_2 mistake in the article!
  double a3 = c6 - 15.0 * c4 / tilde_cum2 + 45.0 * c2 / tilde_cum2_2 - 15.0 / tilde_cum2_3;

  double tmp = cum1 - tilde_cum1 + sqrt_cum2;
  double resu = cum3_2 / (12.0 * cum2_3)
                + (log(tilde_cum2 / cum2)
                   - 1.0 + tmp * tmp / tilde_cum2) / 2.0
                - (tilde_cum3 * a1 / 6.0 + tilde_cum4 * a2 / 24.0 + tilde_cum3_2 * a3 / 72.0)
                - tilde_cum3_2 * (c6 - 6.0 * c4 / cum2 + 9.0 * c2 / tilde_cum2_2) / 72.0
                - 10.0 * cum3 * tilde_cum3 * (cum1 - tilde_cum1) * (cum2 - tilde_cum2) / tilde_cum2_6;

  return resu < 0.0 ? 0.0 : resu;
}

/* ========== Moment estimation from initial neighborhood ============ */

template <class TInput>
void
CumulantsForEdgeworth<TInput>
::MakeSumAndMoments(const TInput& input)
{

  fSum0 = fSum1 = fSum2 = fSum3 = fSum4 = 0.0;
  double pixel, pixel_2;

  for (unsigned long i = 0; i < input.Size(); ++i)
    {
    pixel = static_cast<double> (input.GetPixel(i));
    pixel_2 = pixel * pixel;

    fSum0 += 1.0;
    fSum1 += pixel;
    fSum2 += pixel_2;
    fSum3 += pixel_2 * pixel;
    fSum4 += pixel_2 * pixel_2;
    }

  fMu1 = fSum1 / fSum0;
  fMu2 = fSum2 / fSum0 - fMu1 * fMu1;

  if (fMu2 <= 0.0)
    {
    //otbGenericMsgDebugMacro( << "Potential NAN detected in function MakeSumAndMoments.");
    fMu3 = 0.0;
    fMu4 = 4.0;
    fDataAvailable = false;
    return;
    }

  double sigma = sqrt(fMu2);

  itk::VariableLengthVector<double> tab(input.Size());
  double * x = const_cast<double *> (tab.GetDataPointer());
  for (unsigned long i = 0; i < input.Size(); ++i)
    *x++ = (static_cast<double> (input.GetPixel(i)) - fMu1) / sigma;

  fMu3 = fMu4 = 0.0;
  x = const_cast<double *> (tab.GetDataPointer());
  for (unsigned long i = 0; i < input.Size(); ++i)
    {
    pixel = *x++;
    pixel_2 = pixel * pixel;

    fMu3 += pixel * pixel_2;
    fMu4 += pixel_2 * pixel_2;
    }

  fMu3 /= fSum0;
  fMu4 /= fSum0;

  fDataAvailable = true;
}

/* ================= Moment estimation from raw data ================= */

template <class TInput>
void
CumulantsForEdgeworth<TInput>
::MakeSumAndMoments(const itk::Image<typename TInput::ImageType::PixelType, 1> * input)
{

  fSum0 = fSum1 = fSum2 = fSum3 = fSum4 = 0.0;
  double pixel, pixel_2;

  typedef itk::Image<typename TInput::ImageType::PixelType, 1> LocalImageType;
  typedef itk::ImageRegionConstIterator<LocalImageType>        ImageRegionConstIteratorType;

  ImageRegionConstIteratorType inputIter(input, input->GetRequestedRegion());
  inputIter.GoToBegin();
  unsigned int inputSize = 0;

  while (!inputIter.IsAtEnd())
    {
    pixel = static_cast<double> (inputIter.Get());
    pixel_2 = pixel * pixel;

    fSum0 += 1.0;
    fSum1 += pixel;
    fSum2 += pixel_2;
    fSum3 += pixel_2 * pixel;
    fSum4 += pixel_2 * pixel_2;

    ++inputIter;
    ++inputSize;
    }

  fMu1 = fSum1 / fSum0;
  fMu2 = fSum2 / fSum0 - fMu1 * fMu1;

  if (fMu2 <= 0.0)
    {
    //otbGenericMsgDebugMacro( << "Potential NAN detected in function MakeSumAndMoments.");
    fMu3 = 0.0;
    fMu4 = 4.0;

    fDataAvailable = false;

    return;
    }

  double sigma = sqrt(fMu2);

  std::vector<double> tab(inputSize);
  std::vector<double>::iterator iterTab = tab.begin();

  inputIter.GoToBegin();

  while (!inputIter.IsAtEnd())
    {
    *iterTab = (static_cast<double> (inputIter.Get()) - fMu1) / sigma;

    ++inputIter;
    ++iterTab;
    }

  fMu3 = fMu4 = 0.0;
  for (unsigned int i = 0; i < inputSize; ++i)
    {
    pixel = tab[i];
    pixel_2 = pixel * pixel;

    fMu3 += pixel * pixel_2;
    fMu4 += pixel_2 * pixel_2;
    }

  fMu3 /= fSum0;
  fMu4 /= fSum0;

  otbGenericMsgDebugMacro(<< "Moments: " << fMu1 << ",\t" << fMu2 << ",\t" << fMu3 << ",\t" << fMu4);

  fDataAvailable = true;
  //return 0;
}

/* ================= moments -> cumulants transformation ============= */

template <class TInput>
void
CumulantsForEdgeworth<TInput>
::MakeCumulants()
{
  if (fDataAvailable)
    {
    fMean = fMu1;
    fVariance = fMu2;
    fSkewness = fMu3;
    fKurtosis = fMu4 - 3.0;
    }
  //return 0;
}

} // end of namespace otb

#endif
