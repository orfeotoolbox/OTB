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
#ifndef __otbAngularSecondMomentumTextureFunctor_h
#define __otbAngularSecondMomentumTextureFunctor_h

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class AngularSecondMomentumTextureFunctor
 *  \brief This functor calculates the cluster shade image texture.
 *
 *  Computes angular second moment using joint histogram (neighborhood and offset neighborhood) .
 *  The formula is:
 *  \f[ sum_{i}\sum_{j}p(i,j)^2 \f]
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */
template <class TScalarInputPixelType, class TScalarOutputPixelType>
class ITK_EXPORT AngularSecondMomentumTextureFunctor :
public TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType>
{
public:
  AngularSecondMomentumTextureFunctor(){};
  virtual ~AngularSecondMomentumTextureFunctor(){};

  typedef TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType> Superclass;
  typedef typename Superclass::NeighborhoodType  NeighborhoodType;


  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);

    double out = 0.;

    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
      for (unsigned r = 0; r<this->GetHisto().size(); r++)
        {
          for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
            {
              double p = static_cast<double>(this->GetHisto()[r][s]) * areaInv;
              out += vcl_pow( p, 2 );
            }
        }
      return out;
  }

};

} // namespace Functor
} // namespace otb

#endif

