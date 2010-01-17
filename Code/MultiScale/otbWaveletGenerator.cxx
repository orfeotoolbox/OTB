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

//#ifndef __otbWaveletGenerator_txx
//#define __otbWaveletGenerator_txx
#include "otbWaveletGenerator.h"

#include <vcl_cmath.h>
#include "itkExceptionObject.h"

namespace otb {

/**
 * Standart (useless) definition for unknown wavelet
 */
template < MotherWaveletOperatorEnum TMotherWaveletOperator >
const char *
WaveletGenerator< TMotherWaveletOperator >
::GetWaveletName () const
{
  itk::OStringStream msg;
  msg << "The mother wavelet ID " << TMotherWaveletOperator;
  msg << " is unkown or has to be implemented...\n";
  throw itk::ExceptionObject( __FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION );

  return "Unknown";
}

template < MotherWaveletOperatorEnum TMotherWaveletOperator >
void
WaveletGenerator< TMotherWaveletOperator >
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  itk::OStringStream msg;
  msg << "The mother wavelet ID " << TMotherWaveletOperator;
  msg << " is unkown or has to be implemented...\n";
  throw itk::ExceptionObject( __FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION );

  CoefficientVector vector;
  return vector;
}

template < MotherWaveletOperatorEnum TMotherWaveletOperator >
void
WaveletGenerator< TMotherWaveletOperator >
::GetHighPassCoefficientVector ( CoefficientVector & coeff ) const
{
  itk::OStringStream msg;
  msg << "The mother wavelet ID " << TMotherWaveletOperator;
  msg << " is unkown or has to be implemented (check the txx file)...\n";
  throw itk::ExceptionObject( __FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION );

  CoefficientVector vector;
  return vector;
}

/**
 * Template specialization for the Haar wavelet
 */
template <>
const char * WaveletGenerator<HAAR>::GetWaveletName () const
{
  static const std::string waveletName ( "Haar" );
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<HAAR>
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  static const double sqrt2 = ::vcl_sqrt( 2.0 );

  coeff.resize(3);
  // stands for z^{-1}
  coeff[0] = 0.0;
  // stands for z^0
  coeff[1] = 0.5 * sqrt2;
  // stands for z^1
  coeff[2] = 0.5 * sqrt2;
}

template <>
void
WaveletGenerator<HAAR>
::GetHighPassCoefficientVector ( CoefficientVector & coeff ) const
{
  static const double sqrt2 = ::vcl_sqrt( 2.0 );

  coeff.resize(3);
  // stands for z^{-1}
  coeff[0] = 0.;
  // stands for z^0
  coeff[1] = -0.5 * sqrt2;
  // stands for z^1
  coeff[2] = 0.5 * sqrt2;
}

/**
 * Template specialization for the Daubechies 4 wavelet
 */
template <>
const char * WaveletGenerator<DAUBECHIES4>::GetWaveletName () const
{
  static const std::string waveletName ( "Daubechies_4" );
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<DAUBECHIES4>
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  static const double sqrt2 = ::vcl_sqrt(2.);
  static const double sqrt3 = ::vcl_sqrt(3.);

  coeff.push_back(0.);
  coeff.push_back( (1 + sqrt3) / 8.0 * sqrt2 );
  coeff.push_back( (3 + sqrt3) / 8.0 * sqrt2 );
  coeff.push_back( (3 - sqrt3) / 8.0 * sqrt2 );
  coeff.push_back( (1 - sqrt3) / 8.0 * sqrt2 );
  coeff.resize(5);
}

template <>
void
WaveletGenerator<DAUBECHIES4>
::GetHighPassCoefficientVector ( CoefficientVector & coeff ) const
{
  static const double sqrt2 = ::vcl_sqrt(2.);
  static const double sqrt3 = ::vcl_sqrt(3.);

  coeff.push_back(0);
  coeff.push_back( -(1 - sqrt3) / 8.0 * sqrt2 );
  coeff.push_back(  (3 - sqrt3) / 8.0 * sqrt2 );
  coeff.push_back( -(3 + sqrt3) / 8.0 * sqrt2 );
  coeff.push_back(  (1 + sqrt3) / 8.0 * sqrt2 );
  coeff.resize(5);
}

/**
 * Template specialization for the Daubechies 6 tap wavelet
 */
template <>
const char * WaveletGenerator<DAUBECHIES6>::GetWaveletName () const
{
  static const std::string waveletName ( "Daubechies_6" );
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<DAUBECHIES6>
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back( 0.332670552950 );
  coeff.push_back( 0.806891509311 );
  coeff.push_back( 0.459877502118 );
  coeff.push_back(-0.135011020010 );
  coeff.push_back(-0.085441273882 );
  coeff.push_back( 0.035226291882 );
  coeff.resize(7);
}

template <>
void
WaveletGenerator<DAUBECHIES6>
::GetHighPassCoefficientVector ( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back(-0.035226291882 );
  coeff.push_back(-0.085441273882 );
  coeff.push_back( 0.135011020010 );
  coeff.push_back( 0.459877502118 );
  coeff.push_back(-0.806891509311 );
  coeff.push_back( 0.332670552950 );
  coeff.resize(7);
}

/**
 * Template specialization for the Daubechies 8 wavelet
 */
template <>
const char * WaveletGenerator<DAUBECHIES8>::GetWaveletName () const
{
  static const std::string waveletName ( "Daubechies_8" );
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<DAUBECHIES8>
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back( 0.2303778133090 );
  coeff.push_back( 0.7148465705530 );
  coeff.push_back( 0.6308807667930 );
  coeff.push_back(-0.0279837694170 );
  coeff.push_back(-0.1870348117190 );
  coeff.push_back( 0.0308413818360 );
  coeff.push_back( 0.0328830116670 );
  coeff.push_back(-0.0105974017850 );
  coeff.resize(9);
}

template <>
void
WaveletGenerator<DAUBECHIES8>
::GetHighPassCoefficientVector ( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back( 0.0105974017850 );
  coeff.push_back( 0.0328830116670 );
  coeff.push_back(-0.0308413818360 );
  coeff.push_back(-0.1870348117190 );
  coeff.push_back( 0.0279837694170 );
  coeff.push_back( 0.6308807667930 );
  coeff.push_back(-0.7148465705530 );
  coeff.push_back( 0.2303778133090 );
  coeff.resize(9);
}

/**
 * Template specialization for the Daubechies 12 tap wavelet
 */
template <>
const char * WaveletGenerator<DAUBECHIES12>::GetWaveletName () const
{
  static const std::string waveletName ( "Daubechies_12" );
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<DAUBECHIES12>
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back( 0.1115407433501095 );
  coeff.push_back( 0.4946238903984533 );
  coeff.push_back( 0.7511339080210959 );
  coeff.push_back( 0.3152503517091982 );
  coeff.push_back(-0.2262646939654400 );
  coeff.push_back(-0.1297668675672625 );
  coeff.push_back( 0.0975016055873225 );
  coeff.push_back( 0.0275228655303053 );
  coeff.push_back(-0.0315820393184862 );
  coeff.push_back( 0.0005538422011614 );
  coeff.push_back( 0.0047772575119455 );
  coeff.push_back(-0.0010773010853085 );
  coeff.resize(13);
}

template <>
void
WaveletGenerator<DAUBECHIES12>
::GetHighPassCoefficientVector ( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back( 0.0010773010853085 );
  coeff.push_back( 0.0047772575119455 );
  coeff.push_back(-0.0005538422011614 );
  coeff.push_back(-0.0315820393184862 );
  coeff.push_back(-0.0275228655303053 );
  coeff.push_back( 0.0975016055873225 );
  coeff.push_back( 0.1297668675672625 );
  coeff.push_back(-0.2262646939654400 );
  coeff.push_back(-0.3152503517091982 );
  coeff.push_back( 0.7511339080210959 );
  coeff.push_back(-0.4946238903984533 );
  coeff.push_back( 0.1115407433501095 );
  coeff.resize(13);
}

/**
 * Template specialization for the Daubechies 20 tap wavelet
 */
template <>
const char * WaveletGenerator<DAUBECHIES20>::GetWaveletName () const
{
  static const std::string waveletName ( "Daubechies_20" );
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<DAUBECHIES20>
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back(0.026670057901 );
  coeff.push_back( 0.188176800078 );
  coeff.push_back( 0.527201188932 );
  coeff.push_back( 0.688459039454 );
  coeff.push_back( 0.281172343661 );
  coeff.push_back(-0.249846424327 );
  coeff.push_back(-0.195946274377 );
  coeff.push_back( 0.127369340336 );
  coeff.push_back( 0.093057364604 );
  coeff.push_back(-0.071394147166 );
  coeff.push_back(-0.029457536822 );
  coeff.push_back( 0.033212674059 );
  coeff.push_back( 0.003606553567 );
  coeff.push_back(-0.010733175483 );
  coeff.push_back( 0.001395351747 );
  coeff.push_back( 0.001992405295 );
  coeff.push_back(-0.000685856695 );
  coeff.push_back(-0.000116466855 );
  coeff.push_back( 0.000093588670 );
  coeff.push_back(-0.000013264203 );
  coeff.resize(21);
}

template <>
void
WaveletGenerator<DAUBECHIES20>
::GetHighPassCoefficientVector ( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back( 0.000013264203 );
  coeff.push_back( 0.000093588670 );
  coeff.push_back( 0.000116466855 );
  coeff.push_back(-0.000685856695 );
  coeff.push_back(-0.001992405295 );
  coeff.push_back( 0.001395351747 );
  coeff.push_back( 0.010733175483 );
  coeff.push_back( 0.003606553567 );
  coeff.push_back(-0.033212674059 );
  coeff.push_back(-0.029457536822 );
  coeff.push_back( 0.071394147166 );
  coeff.push_back( 0.093057364604 );
  coeff.push_back(-0.127369340336 );
  coeff.push_back(-0.195946274377 );
  coeff.push_back( 0.249846424327 );
  coeff.push_back( 0.281172343661 );
  coeff.push_back(-0.688459039454 );
  coeff.push_back( 0.527201188932 );
  coeff.push_back(-0.188176800078 );
  coeff.push_back( 0.026670057901 );
  coeff.resize(21);
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
const char * WaveletGenerator<SPLINE_BIORTHOGONAL_2_4>::GetWaveletName () const
{
  static const std::string waveletName ( "SplineBiorthoonal_2_4" );
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<SPLINE_BIORTHOGONAL_2_4>
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  static const double sqrt2 = ::vcl_sqrt( 2.0 );

  coeff.push_back(   3.0 / 128.0 * sqrt2 );
  coeff.push_back( - 3.0 /  64.0 * sqrt2 );
  coeff.push_back( - 1.0 /   8.0 * sqrt2 );
  coeff.push_back(  19.0 /  64.0 * sqrt2 );
  coeff.push_back(  45.0 /  64.0 * sqrt2 );
  coeff.push_back(  19.0 /  64.0 * sqrt2 );
  coeff.push_back( - 1.0 /   8.0 * sqrt2 );
  coeff.push_back( - 3.0 /  64.0 * sqrt2 );
  coeff.push_back(   3.0 / 128.0 * sqrt2 );
  coeff.resize(9);

}

template <>
void
WaveletGenerator<SPLINE_BIORTHOGONAL_2_4>
::GetHighPassCoefficientVector ( CoefficientVector & coeff ) const
{
  static const double sqrt2 = ::vcl_sqrt( 2.0 );

  coeff.push_back(-1.0 / 4.0 * sqrt2 );
  coeff.push_back( 1.0 / 2.0 * sqrt2 );
  coeff.push_back(-1.0 / 4.0 * sqrt2 );
  coeff.push_back( 0. );
  coeff.push_back( 0. );
  coeff.resize(5);
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
const char * WaveletGenerator<SPLINE_BIORTHOGONAL_4_4>::GetWaveletName () const
{
  static const std::string waveletName ( "SplineBiorthoonal_4_4" );
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<SPLINE_BIORTHOGONAL_4_4>
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  static const double sqrt2 = ::vcl_sqrt( 2.0 );

  coeff.push_back( 0.026748757411 * sqrt2 );
  coeff.push_back(-0.016864118443 * sqrt2 );
  coeff.push_back(-0.078223266529 * sqrt2 );
  coeff.push_back( 0.266864118443 * sqrt2 );
  coeff.push_back( 0.602949018236 * sqrt2 );
  coeff.push_back( 0.266864118443 * sqrt2 );
  coeff.push_back(-0.078223266529 * sqrt2 );
  coeff.push_back(-0.016864118443 * sqrt2 );
  coeff.push_back( 0.026748757411 * sqrt2 );
  coeff.resize(9);
}

template <>
void
WaveletGenerator<SPLINE_BIORTHOGONAL_4_4>
::GetHighPassCoefficientVector ( CoefficientVector & coeff ) const
{
  static const double sqrt2 = ::vcl_sqrt( 2.0 );

  coeff.push_back( 0.045635881557 * sqrt2 );
  coeff.push_back(-0.028771763114 * sqrt2 );
  coeff.push_back(-0.295635881557 * sqrt2 );
  coeff.push_back( 0.557543526229 * sqrt2 );
  coeff.push_back(-0.295635881557 * sqrt2 );
  coeff.push_back(-0.028771763114 * sqrt2 );
  coeff.push_back( 0.045635881557 * sqrt2 );
  coeff.push_back( 0. );
  coeff.push_back( 0. );
  coeff.resize(9);
}

/**
 * Template specialization for the Symlet 8 tap orthogonal wavelet
 */
template <>
const char * WaveletGenerator<SYMLET8>::GetWaveletName () const
{
  static const std::string waveletName ( "Symlet_8" );
  return waveletName.c_str();
}

template <>
void
WaveletGenerator<SYMLET8>
::GetLowPassCoefficientVector ( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back(-0.075765714789273);
  coeff.push_back(-0.029635527645999);
  coeff.push_back( 0.497618667632015);
  coeff.push_back( 0.803738751805916);
  coeff.push_back( 0.297857795605277);
  coeff.push_back(-0.099219543576847);
  coeff.push_back(-0.012603967262038);
  coeff.push_back( 0.032223100604043);
  coeff.resize(9);
}

template <>
void
WaveletGenerator<SYMLET8>
::GetHighPassCoefficientVector( CoefficientVector & coeff ) const
{
  coeff.push_back( 0. );
  coeff.push_back(-0.032223100604043);
  coeff.push_back(-0.012603967262038);
  coeff.push_back( 0.099219543576847);
  coeff.push_back( 0.297857795605277);
  coeff.push_back(-0.803738751805916);
  coeff.push_back( 0.497618667632015);
  coeff.push_back( 0.029635527645999);
  coeff.push_back(-0.075765714789273);
  coeff.resize(9);
}

/*
 *  Forcing instantiation
 */

template class WaveletGenerator< HAAR >;
template class WaveletGenerator< DB4 >;
template class WaveletGenerator< DB6 >;
template class WaveletGenerator< DB8 >;
template class WaveletGenerator< DB12 >;
template class WaveletGenerator< DB20 >;
template class WaveletGenerator< SPLINE_BIORTHOGONAL_2_4 >;
template class WaveletGenerator< SPLINE_BIORTHOGONAL_4_4 >;
template class WaveletGenerator< SYMLET8 >;

} // end of namespace otb

//#endif





