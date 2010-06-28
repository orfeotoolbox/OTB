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
#ifndef __otbEntropyTextureFunctor_h
#define __otbEntropyTextureFunctor_h

#pragma message("EntropyTextureFunctor has been deprecated.  Please use otbScalarImageToTexturesFilter instead")

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class EntropyTextureFunctor
 *  \brief <b>DEPRECATED<\b>
 *
 * \deprecated in OTB 3.2.2, please use
 * otbScalarImageToTexturesFilter instead.
 */


template <class TScalarInputPixelType, class TScalarOutputPixelType>
class ITK_EXPORT EntropyTextureFunctor :
  public TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType>
{
public:
  /** Return the texture name */
  virtual std::string GetName() const
  {
    return "EntropyTexture";
  }

  EntropyTextureFunctor()
    {
    };

  virtual ~EntropyTextureFunctor(){}

  typedef TScalarInputPixelType                                             InputScalarType;
  typedef TScalarOutputPixelType                                            OutputScalarType;
  typedef TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType> Superclass;
  typedef typename Superclass::OffsetType                                   OffsetType;
  typedef typename Superclass::RadiusType                                   RadiusType;
  typedef typename Superclass::NeighborhoodType                             NeighborhoodType;

  virtual double ComputeOverSingleChannel(const NeighborhoodType& neigh, const NeighborhoodType& neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
    double area = static_cast<double>(neigh.GetSize()[0] * neigh.GetSize()[1]);
    double areaInv = 1 / area;
    double out = 0.;
    for (unsigned r = 0; r < this->GetHisto().size(); ++r)
      {
      for (unsigned s = 0; s < this->GetHisto()[r].size(); ++s)
        {
        double p = static_cast<double>(this->GetHisto()[r][s]) * areaInv;
        if (p != 0) out += (p * vcl_log(p));
        }
      }
    if (out != 0.) out = -(out);

    return out;
  }
};

} // namespace Functor
} // namespace otb

#endif
