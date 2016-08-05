/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are covered by the IMT copyright.
  See IMTCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbKullbackLeiblerProfileImageFilter_txx
#define otbKullbackLeiblerProfileImageFilter_txx

#include <vector>

#include "otbKullbackLeiblerProfileImageFilter.h"
#include "otbMath.h"

namespace otb
{

/* *******************************************************************
*  Classe CumulantsForEdgeworthProfile
* ********************************************************************
*/
template <class TInput>
CumulantsForEdgeworthProfile<TInput>
::CumulantsForEdgeworthProfile
  (const TInput& input, std::vector<itk::Array2D<int> >& mask)
{
  m_debug = MakeSumAndMoments(input, mask);
  MakeCumulants();
}

/* ===================== Kullback-Leibler Profile ==================== */

template <class TInput>
template <class TInput2>
itk::VariableLengthVector<double>
CumulantsForEdgeworthProfile<TInput>
::KL_profile(CumulantsForEdgeworthProfile<TInput2>& cumulants)
{
  itk::VariableLengthVector<double> resu(fCum.size());

  Iterator iter1 = fCum.begin();
  Iterator iter2 = cumulants.fCum.begin();

  for (unsigned int level = 0; level < resu.GetSize(); level++)
    resu[level] = KL_profile((*iter1++), (*iter2++));

  return resu;
}

/* =========== Kullback-Leibler divergence at a given scale ========== */

template <class TInput>
double
CumulantsForEdgeworthProfile<TInput>
::KL_profile
  (const CumulantType& cumulants1, const CumulantType& cumulants2)
{
  double cum1 = cumulants1[0];
  double cum2 = cumulants1[1];
  double cum3 = cumulants1[2];
  // double cum4 = cumulants1[3]; // unused

  double sqrt_cum2 = sqrt(cum2);
  double cum2_3 = cum2 * cum2 * cum2;
  double cum3_2 = cum3 * cum3;

  double tilde_cum1 = cumulants2[0];
  double tilde_cum2 = cumulants2[1];
  double tilde_cum3 = cumulants2[2];
  double tilde_cum4 = cumulants2[3];

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

  if (vnl_math_isnan(resu) || resu > 1e3) resu = 1e3;

  return resu < 0.0 ? 0.0 : resu;
}

/* ====== Moments estimation from nested neighborhoods ==== */

template <class TInput>
int
CumulantsForEdgeworthProfile<TInput>
::MakeSumAndMoments
  (const TInput& input, std::vector<itk::Array2D<int> >& mask)
{
  fMu.resize(mask.size());
  std::vector<itk::Array2D<int> >::iterator iter = mask.begin();

  if (InitSumAndMoments (input, (*iter++))) return 1;

  for (unsigned int level = 1; level < mask.size(); level++)
    if (ReInitSumAndMoments(input, (*iter++), level)) return 1;

  return 0;
}

/* ===================== Moments estimation ====================== */
/* =============== from the small window size =========== */

template <class TInput>
int
CumulantsForEdgeworthProfile<TInput>
::InitSumAndMoments
  (const TInput& input, itk::Array2D<int>& mask)
{
  fSum0 = fSum1 = fSum2 = fSum3 = fSum4 = 0.0;
  fMu[0].Fill(0.0);

  unsigned int  i, j;
  unsigned long k = 0;
  double        pixel, pixel_2;

  // for ( unsigned long i = 0; i < input.Size(); ++i )
  for (i = 0; i < mask.rows(); ++i)
    {
    for (j = 0; j < mask.cols(); ++j)
      {
      // std::cerr << "(" << i << "," << j << ") k=" << k << " ";
      if (mask.get(i, j) == 1)
        {
        pixel = static_cast<double> (input.GetPixel(k));
        pixel_2 = pixel * pixel;

        fSum0 += 1.0;
        fSum1 += pixel;
        fSum2 += pixel_2;
        fSum3 += pixel_2 * pixel;
        fSum4 += pixel_2 * pixel_2;
        // std::cerr << "*\n";
        }
      ++k;
      }
    }
  if (fSum0 == 0.0)
    {
    fDataAvailable = false;
    return 1;
    }

  double mu1, mu2;

  mu1 = fSum1 / fSum0;
  mu2 = fSum2 / fSum0 - mu1 * mu1;

  if (mu2 == 0.0)
    {
    fDataAvailable = false;
    return 1;
    }

  double sigma = sqrt(mu2);

  itk::VariableLengthVector<double> tab(input.Size());
  double * x = const_cast<double *> (tab.GetDataPointer());
  for (unsigned long cp = 0; cp < input.Size(); ++cp)
    *x++ = (static_cast<double> (input.GetPixel(cp)) - mu1) / sigma;

  double mu3 = 0.0;
  double mu4 = 0.0;
  x = const_cast<double *> (tab.GetDataPointer());

  // for ( unsigned long i = 0; i < input.Size(); ++i )
  for (i = 0; i < mask.rows(); ++i)
    {
    for (j = 0; j < mask.cols(); ++j)
      {
      if (mask.get(i, j) == 1)
        {
        pixel = *x++;
        pixel_2 = pixel * pixel;

        mu3 += pixel * pixel_2;
        mu4 += pixel_2 * pixel_2;
        }
      else ++x;
      }
    }

  mu3 /= fSum0;
  mu4 /= fSum0;

  if (vnl_math_isnan(mu3) || vnl_math_isnan(mu4))
    {
    fDataAvailable = false;
    return 1;
    }

  fMu[0][0] = mu1;
  fMu[0][1] = mu2;
  fMu[0][2] = mu3;
  fMu[0][3] = mu4;

  fDataAvailable = true;

  return 0;
}

/* ================ Window size growth ============ */

template <class TInput>
int
CumulantsForEdgeworthProfile<TInput>
::ReInitSumAndMoments
  (const TInput& input, itk::Array2D<int>& mask, int level)
{
  fMu[level].Fill(0.0);
  // mise a jour du comptage...
  double sum0 = 0.0,
         sum1 = 0.0,
         sum2 = 0.0,
         sum3 = 0.0,
         sum4 = 0.0;

  double pixel, pixel_2;

  unsigned int  i, j;
  unsigned long k = 0L;

  // for ( unsigned long i = 0; i < input.Size(); ++i )
  for (i = 0; i < mask.rows(); ++i)
    {
    for (j = 0; j < mask.cols(); ++j)
      {
      if (mask.get(i, j) == 1)
        {
        pixel = static_cast<double> (input.GetPixel(k));
        pixel_2 = pixel * pixel;

        sum0 += 1.0;
        sum1 += pixel;
        sum2 += pixel_2;
        sum3 += pixel * pixel_2;
        sum4 += pixel_2 * pixel_2;
        }
      ++k;
      }
    }

  fSum0 += sum0;
  fSum1 += sum1;
  fSum2 += sum2;
  fSum3 += sum3;
  fSum4 += sum4;

  double mu = fSum1 / fSum0;
  double mu_2 = mu * mu;
  double mu_3 = mu_2 * mu;
  double mu_4 = mu_2 * mu_2;

  fMu[level][0] = mu;
  fMu[level][1] = fSum2 / fSum0 - mu_2;

  double sigma = sqrt(fSum2);
  double sigma_2 = fSum2;
  double sigma_3 = sigma * sigma_2;
  double sigma_4 = sigma_2 * sigma_2;

  fMu[level][2] = (fSum3 - 3.0 * mu * fSum2 + 3.0 * mu_2 * fSum1 - fSum0 * mu_3)
                  / (sigma_3 * fSum0);
  fMu[level][3] = (fSum4 - 4.0 * mu * fSum3 + 6.0 * mu_2 * fSum2
                   - 4.0 * mu_3 * fSum1 + fSum0 * mu_4)
                  / (sigma_4 * fSum0);

  return 0;
}

/* =========== transformation moment -> cumulants ==================== */

template <class TInput>
int
CumulantsForEdgeworthProfile<TInput>
::MakeCumulants()
{
  if (!IsDataAvailable()) return 1;

  fCum.resize(fMu.size());
  fCum = fMu;

  for (unsigned int i = 0; i < fCum.size(); ++i)
    fCum[i][3] -= 3.0;

  return 0;
}

/* *******************************************************************
*
*  Functor
*
* ********************************************************************
*/

namespace Functor
{

template<class TInput1, class TInput2, class TOutput>
KullbackLeiblerProfile<TInput1, TInput2, TOutput>
::KullbackLeiblerProfile ()
{
  m_RadiusMin = 1;
  m_RadiusMax = 3;
}

/* =========== Gives the radius min and max of neighborhood ========== */

template<class TInput1, class TInput2, class TOutput>
void
KullbackLeiblerProfile<TInput1, TInput2, TOutput>
::SetRadius(const unsigned char& min, const unsigned char& max)
{
  m_RadiusMin = min < max ? min : max;
  m_RadiusMax = max > min ? max : min;
  MakeMultiscaleProfile();
}

template<class TInput1, class TInput2, class TOutput>
unsigned char
KullbackLeiblerProfile<TInput1, TInput2, TOutput>
::GetRadiusMin(void)
{
  return m_RadiusMin;
}

template<class TInput1, class TInput2, class TOutput>
unsigned char
KullbackLeiblerProfile<TInput1, TInput2, TOutput>
::GetRadiusMax(void)
{
  return m_RadiusMax;
}

/* ====== Make the set of masks to play the increase in window size == */

template<class TInput1, class TInput2, class TOutput>
void
KullbackLeiblerProfile<TInput1, TInput2, TOutput>
::MakeMultiscaleProfile()
{
  m_mask.resize(m_RadiusMax - m_RadiusMin + 1);
  int lenMax = 2 * m_RadiusMax + 1;
  int i, j, middle = m_RadiusMax;

  // let's begin by the smaller neighborhood
  std::vector<itk::Array2D<int> >::iterator outer_iter = m_mask.begin();
  (*outer_iter).SetSize(lenMax, lenMax);
  (*outer_iter).fill(0);
  for (i = middle - m_RadiusMin; i <= middle + m_RadiusMin; ++i)
    for (j = middle - m_RadiusMin; j <= middle + m_RadiusMin; ++j)
      (*outer_iter).put(i, j, 1);

  // std::cerr << "outerIter = " << (*outer_iter) << std::endl;

  // let's continue with increasing neighborhoods
  ++outer_iter;
  for (int radius = m_RadiusMin + 1; radius <= m_RadiusMax; ++radius)
    {
    (*outer_iter).SetSize(lenMax, lenMax);
    (*outer_iter).fill(0);

    for (i = middle - radius; i <= middle + radius; ++i)
      {
      (*outer_iter).put(i, middle - radius, 1);
      (*outer_iter).put(i, middle + radius, 1);
      (*outer_iter).put(middle - radius, i, 1);
      (*outer_iter).put(middle + radius, i, 1);
      }

    // std::cerr << "outerIter = " << (*outer_iter) << std::endl;
    ++outer_iter;
    }
}

/* ========================== Functor ================================ */

template<class TInput1, class TInput2, class TOutput>
TOutput
KullbackLeiblerProfile<TInput1, TInput2, TOutput>
::operator()
  (const TInput1 &it1, const TInput2 &it2)
  {
  CumulantsForEdgeworthProfile<TInput1> cum1(it1, m_mask);

  if (cum1.m_debug)
    {
    itk::VariableLengthVector<double> resu(m_RadiusMax - m_RadiusMin + 1);
    resu.Fill(1e3);
    return static_cast<TOutput> (resu);
    }

  CumulantsForEdgeworthProfile<TInput2> cum2(it2, m_mask);

  if (cum2.m_debug)
    {
    itk::VariableLengthVector<double> resu(m_RadiusMax - m_RadiusMin + 1);
    resu.Fill(1e3);
    return static_cast<TOutput> (resu);
    }

  return static_cast<TOutput> (cum1.KL_profile(cum2) + cum2.KL_profile(cum1));
  }

} // Functor

} // namespace otb

#endif
