/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbWaveletGenerator.h"

#include <vcl_cmath.h>
#include "itkMacro.h"
#include "otbMath.h"

namespace otb {

/**
 * Standart (useless) definition for unknown wavelet
 */
template <Wavelet::Wavelet TMotherWaveletOperator>
const char *
WaveletGenerator<TMotherWaveletOperator>
::GetWaveletName() const
{
  std::ostringstream msg;
  msg << "The mother wavelet ID " << TMotherWaveletOperator;
  msg << " is unkown or has to be implemented...\n";
  throw itk::ExceptionObject(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);

  return "Unknown";
}

template <Wavelet::Wavelet TMotherWaveletOperator>
void
WaveletGenerator<TMotherWaveletOperator>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  std::ostringstream msg;
  msg << "The mother wavelet ID " << TMotherWaveletOperator;
  msg << " is unkown or has to be implemented...\n";
  throw itk::ExceptionObject(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
}

template <Wavelet::Wavelet TMotherWaveletOperator>
void
WaveletGenerator<TMotherWaveletOperator>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  std::ostringstream msg;
  msg << "The mother wavelet ID " << TMotherWaveletOperator;
  msg << " is unkown or has to be implemented (check the txx file)...\n";
  throw itk::ExceptionObject(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
}

/**
 * Template specialization for the Haar wavelet
 */
template <>
const char * WaveletGenerator<Wavelet::HAAR>::GetWaveletName() const
{
  static const std::string waveletName ("Haar");
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<Wavelet::HAAR>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(3);
  // stands for z^{-1}
  coeff[0] = 0.0;
  // stands for z^0
  coeff[1] = 0.5 * CONST_SQRT2;
  // stands for z^1
  coeff[2] = 0.5 * CONST_SQRT2;
}

template <>
void
WaveletGenerator<Wavelet::HAAR>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(3);
  // stands for z^{-1}
  coeff[0] = 0.;
  // stands for z^0
  coeff[1] = -0.5 * CONST_SQRT2;
  // stands for z^1
  coeff[2] = 0.5 * CONST_SQRT2;
}

/**
 * Template specialization for the Daubechies 4 wavelet
 */
template <>
const char * WaveletGenerator<Wavelet::DAUBECHIES4>::GetWaveletName() const
{
  static const std::string waveletName ("Daubechies_4");
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES4>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(5);
  coeff[0] = 0.;
  coeff[1] = (1 + CONST_SQRT3) / 8.0 * CONST_SQRT2;
  coeff[2] = (3 + CONST_SQRT3) / 8.0 * CONST_SQRT2;
  coeff[3] = (3 - CONST_SQRT3) / 8.0 * CONST_SQRT2;
  coeff[4] = (1 - CONST_SQRT3) / 8.0 * CONST_SQRT2;
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES4>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(5);
  coeff[0] = 0;
  coeff[1] = -(1 - CONST_SQRT3) / 8.0 * CONST_SQRT2;
  coeff[2] = (3 - CONST_SQRT3) / 8.0 * CONST_SQRT2;
  coeff[3] = -(3 + CONST_SQRT3) / 8.0 * CONST_SQRT2;
  coeff[4] = (1 + CONST_SQRT3) / 8.0 * CONST_SQRT2;
}

/**
 * Template specialization for the Daubechies 6 tap wavelet
 */
template <>
const char * WaveletGenerator<Wavelet::DAUBECHIES6>::GetWaveletName() const
{
  static const std::string waveletName ("Daubechies_6");
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES6>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(7);
  coeff[0] = 0.;
  coeff[1] = 0.332670552950;
  coeff[2] = 0.806891509311;
  coeff[3] = 0.459877502118;
  coeff[4] = -0.135011020010;
  coeff[5] = -0.085441273882;
  coeff[6] = 0.035226291882;
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES6>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(7);
  coeff[0] = 0.;
  coeff[1] = -0.035226291882;
  coeff[2] = -0.085441273882;
  coeff[3] = 0.135011020010;
  coeff[4] = 0.459877502118;
  coeff[5] = -0.806891509311;
  coeff[6] = 0.332670552950;
}

/**
 * Template specialization for the Daubechies 8 wavelet
 */
template <>
const char * WaveletGenerator<Wavelet::DAUBECHIES8>::GetWaveletName() const
{
  static const std::string waveletName ("Daubechies_8");
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES8>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(9);
  coeff[0] = 0.;
  coeff[1] = 0.2303778133090;
  coeff[2] = 0.7148465705530;
  coeff[3] = 0.630880767930;
  coeff[4] = -0.0279837694170;
  coeff[5] = -0.1870348117190;
  coeff[6] = 0.0308413818360;
  coeff[7] = 0.0328830116670;
  coeff[8] = -0.0105974017850;
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES8>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(9);
  coeff[0] = 0.;
  coeff[1] = 0.0105974017850;
  coeff[2] = 0.0328830116670;
  coeff[3] = -0.0308413818360;
  coeff[4] = -0.1870348117190;
  coeff[5] = 0.0279837694170;
  coeff[6] = 0.630880767930;
  coeff[7] = -0.7148465705530;
  coeff[8] = 0.2303778133090;
}

/**
 * Template specialization for the Daubechies 12 tap wavelet
 */
template <>
const char * WaveletGenerator<Wavelet::DAUBECHIES12>::GetWaveletName() const
{
  static const std::string waveletName ("Daubechies_12");
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES12>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(13);
  coeff[0] = 0.;
  coeff[1] = 0.1115407433501095;
  coeff[2] = 0.4946238903984533;
  coeff[3] = 0.7511339080210959;
  coeff[4] = 0.3152503517091982;
  coeff[5] = -0.2262646939654400;
  coeff[6] = -0.1297668675672625;
  coeff[7] = 0.0975016055873225;
  coeff[8] = 0.0275228655303053;
  coeff[9] = -0.0315820393184862;
  coeff[10] = 0.0005538422011614;
  coeff[11] = 0.0047772575119455;
  coeff[12] = -0.0010773010853085;
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES12>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(13);
  coeff[0] = 0.;
  coeff[1] = 0.0010773010853085;
  coeff[2] = 0.0047772575119455;
  coeff[3] = -0.0005538422011614;
  coeff[4] = -0.0315820393184862;
  coeff[5] = -0.0275228655303053;
  coeff[6] = 0.0975016055873225;
  coeff[7] = 0.1297668675672625;
  coeff[8] = -0.2262646939654400;
  coeff[9] = -0.3152503517091982;
  coeff[10] = 0.7511339080210959;
  coeff[11] = -0.4946238903984533;
  coeff[12] = 0.1115407433501095;
}

/**
 * Template specialization for the Daubechies 20 tap wavelet
 */
template <>
const char * WaveletGenerator<Wavelet::DAUBECHIES20>::GetWaveletName() const
{
  static const std::string waveletName ("Daubechies_20");
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES20>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(21);
  coeff[0] = 0.;
  coeff[1] = 0.026670057901;
  coeff[2] = 0.188176800078;
  coeff[3] = 0.527201188932;
  coeff[4] = 0.688459039454;
  coeff[5] = 0.281172343661;
  coeff[6] = -0.249846424327;
  coeff[7] = -0.195946274377;
  coeff[8] = 0.127369340336;
  coeff[9] = 0.093057364604;
  coeff[10] = -0.071394147166;
  coeff[11] = -0.029457536822;
  coeff[12] = 0.033212674059;
  coeff[13] = 0.003606553567;
  coeff[14] = -0.010733175483;
  coeff[15] = 0.001395351747;
  coeff[16] = 0.001992405295;
  coeff[17] = -0.000685856695;
  coeff[18] = -0.000116466855;
  coeff[19] = 0.000093588670;
  coeff[20] = -0.000013264203;
}

template <>
void
WaveletGenerator<Wavelet::DAUBECHIES20>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(21);
  coeff[0] = 0.;
  coeff[1] = 0.000013264203;
  coeff[2] = 0.000093588670;
  coeff[3] = 0.000116466855;
  coeff[4] = -0.000685856695;
  coeff[5] = -0.001992405295;
  coeff[6] = 0.001395351747;
  coeff[7] = 0.010733175483;
  coeff[8] = 0.003606553567;
  coeff[9] = -0.033212674059;
  coeff[10] = -0.029457536822;
  coeff[11] = 0.071394147166;
  coeff[12] = 0.093057364604;
  coeff[13] = -0.127369340336;
  coeff[14] = -0.195946274377;
  coeff[15] = 0.249846424327;
  coeff[16] = 0.281172343661;
  coeff[17] = -0.688459039454;
  coeff[18] = 0.527201188932;
  coeff[19] = -0.188176800078;
  coeff[20] = 0.026670057901;
}

/**
 * Template specialization for the Spline Biorthogonal Wavelet.
 *
 *       M.Antonini, M.Barlaud, P.Mathieu, I.Daubechies
 *       Image Coding Using Wavelet Transform
 *       IEEE Transactions on Image Processing,
 *       vol 1(2), pp:205-220, April 1992
 */
template <>
const char * WaveletGenerator<Wavelet::SPLINE_BIORTHOGONAL_2_4>::GetWaveletName() const
{
  static const std::string waveletName ("SplineBiorthoonal_2_4");
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<Wavelet::SPLINE_BIORTHOGONAL_2_4>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(9);
  coeff[0] = 3.0 / 128.0 * CONST_SQRT2;
  coeff[1] = -3.0 /  64.0 * CONST_SQRT2;
  coeff[2] = -1.0 /   8.0 * CONST_SQRT2;
  coeff[3] = 19.0 /  64.0 * CONST_SQRT2;
  coeff[4] = 45.0 /  64.0 * CONST_SQRT2;
  coeff[5] = 19.0 /  64.0 * CONST_SQRT2;
  coeff[6] = -1.0 /   8.0 * CONST_SQRT2;
  coeff[7] = -3.0 /  64.0 * CONST_SQRT2;
  coeff[8] = 3.0 / 128.0 * CONST_SQRT2;
}

template <>
void
WaveletGenerator<Wavelet::SPLINE_BIORTHOGONAL_2_4>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(9);
  coeff[0] = 0.;
  coeff[1] = 0.;
  coeff[2] = 0.;
  coeff[3] = 0.;
  coeff[4] = -1.0 / 4.0 * CONST_SQRT2;
  coeff[5] = 1.0 / 2.0 * CONST_SQRT2;
  coeff[6] = -1.0 / 4.0 * CONST_SQRT2;
  coeff[7] = 0.;
  coeff[8] = 0.;
}

/**
 * Template specialization for the Spline Biorthogonal Wavelet.
 *
 *       M.Antonini, M.Barlaud, P.Mathieu, I.Daubechies
 *       Image Coding Using Wavelet Transform
 *       IEEE Transactions on Image Processing,
 *       vol 1(2), pp:205-220, April 1992
 *
 *       Also in Daubechies's "Ten Lectures on Wavelet": in
 *       less disparate filter lengths, pp 278--279.
 *
 *       Also known as 7/9 filter.
 */
template <>
const char * WaveletGenerator<Wavelet::SPLINE_BIORTHOGONAL_4_4>::GetWaveletName() const
{
  static const std::string waveletName ("SplineBiorthoonal_4_4");
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<Wavelet::SPLINE_BIORTHOGONAL_4_4>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(9);
  coeff[0] = 0.026748757411 * CONST_SQRT2;
  coeff[1] = -0.016864118443 * CONST_SQRT2;
  coeff[2] = -0.078223266529 * CONST_SQRT2;
  coeff[3] = 0.266864118443 * CONST_SQRT2;
  coeff[4] = 0.602949018236 * CONST_SQRT2;
  coeff[5] = 0.266864118443 * CONST_SQRT2;
  coeff[6] = -0.078223266529 * CONST_SQRT2;
  coeff[7] = -0.016864118443 * CONST_SQRT2;
  coeff[8] = 0.026748757411 * CONST_SQRT2;
}

template <>
void
WaveletGenerator<Wavelet::SPLINE_BIORTHOGONAL_4_4>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(9);
  coeff[0] = 0.;
  coeff[1] = 0.;
  coeff[2] = 0.045635881557 * CONST_SQRT2;
  coeff[3] = -0.028771763114 * CONST_SQRT2;
  coeff[4] = -0.295635881557 * CONST_SQRT2;
  coeff[5] = 0.557543526229 * CONST_SQRT2;
  coeff[6] = -0.295635881557 * CONST_SQRT2;
  coeff[7] = -0.028771763114 * CONST_SQRT2;
  coeff[8] = 0.045635881557 * CONST_SQRT2;
}

/**
 * Template specialization for the Symlet 8 tap orthogonal wavelet
 */
template <>
const char * WaveletGenerator<Wavelet::SYMLET8>::GetWaveletName() const
{
  static const std::string waveletName ("Symlet_8");
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<Wavelet::SYMLET8>
::GetLowPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(9);
  coeff[0] = 0.;
  coeff[1] = -0.075765714789273;
  coeff[2] = -0.029635527645999;
  coeff[3] = 0.497618667632015;
  coeff[4] = 0.803738751805916;
  coeff[5] = 0.297857795605277;
  coeff[6] = -0.099219543576847;
  coeff[7] = -0.012603967262038;
  coeff[8] = 0.032223100604043;
}

template <>
void
WaveletGenerator<Wavelet::SYMLET8>
::GetHighPassCoefficientVector(CoefficientVector& coeff) const
{
  coeff.resize(9);
  coeff[0] = 0.;
  coeff[1] = -0.032223100604043;
  coeff[2] = -0.012603967262038;
  coeff[3] = 0.099219543576847;
  coeff[4] = 0.297857795605277;
  coeff[5] = -0.803738751805916;
  coeff[6] = 0.497618667632015;
  coeff[7] = 0.029635527645999;
  coeff[8] = -0.075765714789273;
}

/*
 *  Forcing instantiation
 */

template class WaveletGenerator<Wavelet::HAAR>;
template class WaveletGenerator<Wavelet::DB4>;
template class WaveletGenerator<Wavelet::DB6>;
template class WaveletGenerator<Wavelet::DB8>;
template class WaveletGenerator<Wavelet::DB12>;
template class WaveletGenerator<Wavelet::DB20>;
template class WaveletGenerator<Wavelet::SPLINE_BIORTHOGONAL_2_4>;
template class WaveletGenerator<Wavelet::SPLINE_BIORTHOGONAL_4_4>;
template class WaveletGenerator<Wavelet::SYMLET8>;

} // end of namespace otb
