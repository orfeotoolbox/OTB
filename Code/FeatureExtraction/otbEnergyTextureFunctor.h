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
#ifndef __otbEnergyTextureFunctor_h
#define __otbEnergyTextureFunctor_h

#include "otbTextureFunctorBase.h"


namespace otb
{
namespace Functor
{
/** \class EnergyTextureFunctor
 *  \brief This functor calculates the energy image texture.
 *
 *  The formula is:
 *  \f[ \frac{1}{N}\sum_{i}\sum_{j}(i-j)^2 \f]
 *
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */
template <class TScalarInputPixelType, class TScalarOutputPixelType>
class ITK_EXPORT EnergyTextureFunctor :
public TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType>
{
public:
  EnergyTextureFunctor(){};
  virtual ~EnergyTextureFunctor(){};

  typedef TScalarInputPixelType                  InputScalarType;
  typedef TScalarOutputPixelType                 OutputScalarType;
  typedef TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType> Superclass;
  typedef typename Superclass::OffsetType        OffsetType;
  typedef typename Superclass::RadiusType        RadiusType;
  typedef typename Superclass::NeighborhoodType  NeighborhoodType;
 

  double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    RadiusType radius = neigh.GetRadius();
    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);

    OffsetType offset;
    offset.Fill(0);
    OffsetType offsetOff;
    OffsetType offsetOffInit;

    offsetOffInit[0] = -radius[0]+this->GetOffset()[0]-1;
    offsetOffInit[1] = -radius[1]+this->GetOffset()[1]-1;

    double temp = 0.;
    double norm = 0.;

    offsetOff = offsetOffInit;
    for ( int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++ )
      {
        offsetOff[0]++;
        offsetOff[1] = offsetOffInit[1];
        offset[0] = l;
        for ( int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); k++)
          {
            offsetOff[1]++;
            offset[1] = k;
            norm = vcl_pow(static_cast<double>( ( neigh[offset] - neighOff[neighOff.GetCenterNeighborhoodIndex()] ) ), 2);
            temp += norm;
          }
      }
    temp /= area;
    return vcl_pow(temp, 2);
  }

};


} // namespace Functor
} // namespace otb

#endif

