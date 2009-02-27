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
#ifndef __otbDifferenceVarianceTextureFunctor_h
#define __otbDifferenceVarianceTextureFunctor_h

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class DifferenceVarianceTextureFunctor
 *  \brief This functor calculates the difference variance image texture according to Haralick descriptors.
 *
 *  Computes  difference variance using joint histogram (neighborhood and offset neighborhood).
 *  The formula is:
 *  variance of \f$ p_{x-y} \f$
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */

template <class TScalarInputPixelType, class TScalarOutputPixelType>
class ITK_EXPORT DifferenceVarianceTextureFunctor :
      public TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType>
{
public:
  DifferenceVarianceTextureFunctor() {};
  virtual ~DifferenceVarianceTextureFunctor() {};

  typedef TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType> Superclass;
  typedef typename Superclass::NeighborhoodType                             NeighborhoodType;

  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;

    double MeanPx_y = 0.;
    // Computes mean Px_y
    for (unsigned sB = 0; sB<this->GetHisto()[0].size(); sB++)
    {
      double nCeil = (static_cast<double>(sB)+0.5)*this->GetNeighBinLength();
      for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
        double rVal = (static_cast<double>(r)+0.5)*this->GetOffsetBinLength();
        for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
        {
          if ( vcl_abs((static_cast<double>(s)+0.5)*this->GetNeighBinLength() - rVal - nCeil) < vcl_abs(this->GetNeighBinLength()) )
          {
            MeanPx_y +=  static_cast<double>(this->GetHisto()[r][s])*areaInv;
          }
        }
      }
    }
    MeanPx_y /= static_cast<double>(this->GetHisto()[0].size());

    // Computes variance
    double varPx_y = 0.;
    for (unsigned sB = 0; sB<this->GetHisto()[0].size(); sB++)
    {
      double Px_y = 0.;
      double nCeil = (static_cast<double>(sB)+0.5)*this->GetNeighBinLength();
      for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
        double rVal = (static_cast<double>(r)+0.5)*this->GetOffsetBinLength();
        for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
        {
          if ( vcl_abs((static_cast<double>(s)+0.5)*this->GetNeighBinLength() - rVal - nCeil) < vcl_abs(this->GetNeighBinLength()) )
          {
            Px_y +=  static_cast<double>(this->GetHisto()[r][s])*areaInv;
          }
        }
      }
      varPx_y += vcl_pow((Px_y - MeanPx_y), 2);
    }

    return varPx_y/this->GetHisto()[0].size();
  }

};


} // namespace Functor
} // namespace otb

#endif

