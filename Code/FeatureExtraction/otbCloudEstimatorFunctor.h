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
#ifndef __otbCloudEstimatorFunctor_h
#define __otbCloudEstimatorFunctor_h

#include "otbMath.h"
#include "otbSpectralAngleFunctor.h"

namespace otb
{
  /** \class CloudEstimatorFunctor
   *  \brief This functor first computes the spectral angle according to a reference pixel.
   *  \brief Then multiplies the result by a gaussian coefficient
   *  \brief Binarise the image with a threshold
   *  \brief And reverse the pixel values.
   */
namespace Functor
{
template<class TInput,class TOutputValue>
class CloudEstimatorFunctor
{
 public:
  typedef SpectralAngleFunctor<TInput, TOutputValue> SpectralAngleFunctorType;

  CloudEstimatorFunctor()
    { 
      m_ReferencePixel.SetSize(4);
      m_ReferencePixel.Fill(1);
      m_RefNorm = 2.0;
      m_Variance = 1.0;
      m_Denom = 1.0;
      m_Coef = 1.0;

maxigauss = 0;
maxi = 0;
minigauss = 100000;
mini = 100000;

    };

  ~CloudEstimatorFunctor(){};
  inline TOutputValue operator()(const TInput& inPix)
    {

      TOutputValue lOut;
      double lRes = 0.0;
      double lCurPixNorm = 0.0;
      double lGaussianCoef = 1.0;

      for (unsigned int i=0; i<std::min(inPix.Size(),m_ReferencePixel.Size()); i++)
      {
        lCurPixNorm += inPix[i]*inPix[i];
      }
      lCurPixNorm = vcl_sqrt(static_cast<double>(lCurPixNorm));
      lGaussianCoef = m_Coef * vcl_exp(- vcl_pow(((lCurPixNorm-m_RefNorm)/m_RefNorm),2) / m_Denom ) ;
      lRes =  lGaussianCoef * (M_PI-m_SpectralAngleFunctor(inPix));

if (lGaussianCoef > maxigauss){ maxigauss=lGaussianCoef; std::cout << maxigauss << std::endl;}
if (lRes > maxi){ maxi=lRes; std::cout << maxi << std::endl;}

if (lGaussianCoef < minigauss){ minigauss=lGaussianCoef; std::cout << minigauss << std::endl;}
if (lRes < mini){ mini=lRes; std::cout << mini << std::endl;}

//       lRes = ( (-1)*(lRes/M_PI) ) +1 ;

      lOut = static_cast<TOutputValue>(lRes);
      return lOut;

    }

  void SetReferencePixel( TInput ref )
    { 
      m_ReferencePixel = ref;
      m_SpectralAngleFunctor.SetReferencePixel(ref);
      m_RefNorm = 0.0;
      for(unsigned int i = 0; i<ref.Size(); i++)
    {
      m_RefNorm += ref[i]*ref[i];
    }
      m_RefNorm = vcl_sqrt(static_cast<double>(m_RefNorm)); 
    };
  void SetVariance(double variance)
    {
      m_Variance = variance;
      m_Denom = 2 * variance * variance;
      m_Coef = 1/( variance * vcl_sqrt(2*M_PI) );
    }

  TInput GetReferencePixel(){ return m_ReferencePixel; };
  double GetVariance(){ return m_Variance; };

  
 protected:
  SpectralAngleFunctorType m_SpectralAngleFunctor;
  TInput m_ReferencePixel;
  double m_RefNorm;
  double m_Variance;
  double m_Denom;
  double m_Coef;
double  maxi;
double maxigauss;
double  mini;
double minigauss;
};

} // end namespace functor
} // end namespace otb


#endif
