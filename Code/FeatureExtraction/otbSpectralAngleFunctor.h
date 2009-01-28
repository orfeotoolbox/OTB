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
#ifndef __otbSpectralAngleFunctor_h
#define __otbSpectralAngleFunctor_h

#include "otbMath.h"

namespace otb
{
  /** \class SpectralAngleFunctor
   *  \brief This functor computes the spectral angle according to a reference pixel.
   */
namespace Functor
{
template<class TInput,class TOutputValue>
class SpectralAngleFunctor
{
 public:
  SpectralAngleFunctor()
    { 
      m_ReferencePixel.SetSize(4);
      m_ReferencePixel.Fill(1);
    };

  ~SpectralAngleFunctor(){};
  inline TOutputValue operator()(const TInput& inPix)
    {
     TOutputValue out;

      double dist=0.0;
      double scalarProd=0.0;
      double normProd=0.0;
      double normProd1=0.0;
      for (unsigned int i=0; i<std::min(inPix.Size(),m_ReferencePixel.Size()); i++)
    {
      scalarProd += inPix[i]*m_ReferencePixel[i];
      normProd1 += inPix[i]*inPix[i];
    }
      normProd = normProd1 * m_RefNorm*m_RefNorm;
   
      if ( normProd == 0.0)
    {
      dist = 0.0;
    }
      else
    {
      dist = vcl_acos(scalarProd/vcl_sqrt(normProd));
    }
     
      out = static_cast<TOutputValue>(dist);
      return out;

    }
  
  void SetReferencePixel( TInput ref )
    { 
      m_ReferencePixel = ref;
      m_RefNorm = 0.0;
      for(unsigned int i = 0; i<ref.Size(); i++)
    {
      m_RefNorm += ref[i]*ref[i];
    }
      m_RefNorm = vcl_sqrt(static_cast<double>(m_RefNorm)); 
    };
  TInput GetReferencePixel(){ return m_ReferencePixel; };
  
 protected:
  TInput m_ReferencePixel;
  double m_RefNorm;
};

} // end namespace functor
} // end namespace otb


#endif
