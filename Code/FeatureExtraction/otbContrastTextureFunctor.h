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
#ifndef __otbContrastTextureFunctor_h
#define __otbContrastTextureFunctor_h

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class ContrastTextureFunctor
 *  \brief This functor calculates the contrast image texture according to Haralick descriptors.
 *
 *  Computes contrast using joint histogram (neighborhood and offset neighborhood).
 *  The formula is:
 *  \f[ \sum_{n}n^2.p_{x-y}(n)^2 \f]
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */

template <class TScalarInputPixelType, class TScalarOutputPixelType>
class ITK_EXPORT ContrastTextureFunctor :
      public TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType>
{
public:
  ContrastTextureFunctor() {};
  virtual ~ContrastTextureFunctor() {};

  typedef TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType> Superclass;
  typedef typename Superclass::NeighborhoodType                             NeighborhoodType;

  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
    double out = 0.;

    // loop over bin neighborhood values
    for (unsigned sB = 0; sB<this->GetHisto()[0].size(); sB++)
    {
      double nCeil = (static_cast<double>(sB)+0.5)*this->GetNeighBinLength();
      double nCeilSquare = vcl_pow( nCeil, 2);
      for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
        double rVal = (static_cast<double>(r)+0.5)*this->GetOffsetBinLength();
        for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
        {
          if ( vcl_abs((static_cast<double>(s)+0.5)*this->GetNeighBinLength() - rVal - nCeil) < vcl_abs(this->GetNeighBinLength()) )
          {
            double p =  static_cast<double>(this->GetHisto()[r][s])*areaInv;
            out += nCeilSquare * p;
          }
        }
      }
    }

    return out;
  }

};




} // namespace Functor
} // namespace otb

#endif

