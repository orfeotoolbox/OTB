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
   * \ingroup Textures
 */
template <class TScalarInputPixelType, class TScalarOutputPixelType>
class ITK_EXPORT EnergyTextureFunctor :
  public TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType>
{
public:
  /** Return the texture name */
  virtual std::string GetName() const
  {
    return "EnergyTexture";
  }

  EnergyTextureFunctor(){};
  virtual ~EnergyTextureFunctor(){}

  typedef TScalarInputPixelType                                             InputScalarType;
  typedef TScalarOutputPixelType                                            OutputScalarType;
  typedef TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType> Superclass;
  typedef typename Superclass::OffsetType                                   OffsetType;
  typedef typename Superclass::RadiusType                                   RadiusType;
  typedef typename Superclass::NeighborhoodType                             NeighborhoodType;

  double ComputeOverSingleChannel(const NeighborhoodType& neigh, const NeighborhoodType& neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
    
    double area = static_cast<double>(neigh.GetSize()[0] * neigh.GetSize()[1]);
    double areaInv = 1 / area;
    double out = 0.;
    double p = 0.;
    for (unsigned r = 0; r < this->GetHisto().size(); ++r)
      {
      for (unsigned s = 0; s < this->GetHisto()[r].size(); ++s)
        {
        p = static_cast<double>(this->GetHisto()[r][s]) * areaInv;
        out += p*p;
        }
      }
    return out;
    }

};

} // namespace Functor
} // namespace otb

#endif
