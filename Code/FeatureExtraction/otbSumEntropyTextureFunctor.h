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
#ifndef __otbSumEntropyTextureFunctor_h
#define __otbSumEntropyTextureFunctor_h

#pragma message("SumEntropyTextureFunctor has been deprecated.  Please use otbScalarImageToAdvancedTexturesFilter instead")

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class SumEntropyTextureFunctor
 *  \brief <b>DEPRECATED<\b>
 *
 * \deprecated in OTB 3.2.2, please use
 * otbScalarImageToAdvancedTexturesFilter instead.
 */

template <class TScalarInputPixelType, class TScalarOutputPixelType>
class ITK_EXPORT SumEntropyTextureFunctor :
  public TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType>
{
public:
  /** Return the texture name */
  virtual std::string GetName() const
  {
    return "SumEntropyTexture";
  }

  SumEntropyTextureFunctor()
    {
    };
  virtual ~SumEntropyTextureFunctor(){}

  typedef TextureFunctorBase<TScalarInputPixelType, TScalarOutputPixelType> Superclass;
  typedef typename Superclass::NeighborhoodType                             NeighborhoodType;

  virtual double ComputeOverSingleChannel(const NeighborhoodType& neigh, const NeighborhoodType& neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
    double area = static_cast<double>(neigh.GetSize()[0] * neigh.GetSize()[1]);
    double areaInv = 1 / area;
    double out = 0.;
    // loop over bin neighborhood values
    for (unsigned sB = 0; sB < this->GetHisto()[0].size(); sB++)
      {
      double nCeil = (static_cast<double>(sB) + 0.5) * this->GetNeighBinLength();
      double nCeil2 = (static_cast<double>(sB) + this->GetHisto()[0].size() + 0.5) * this->GetNeighBinLength();
      double Px_y = 0.;
      double Px_y2 = 0.;
      for (unsigned r = 0; r < this->GetHisto().size(); ++r)
        {
        double rVal = (static_cast<double>(r) + 0.5) * this->GetOffsetBinLength();
        for (unsigned s = 0; s < this->GetHisto()[r].size(); ++s)
          {
          double sVal = (static_cast<double>(s) + 0.5) * this->GetNeighBinLength();
          if (vcl_abs(rVal + sVal - nCeil) < vcl_abs(this->GetNeighBinLength()))
            {
            Px_y +=  static_cast<double>(this->GetHisto()[r][s]) * areaInv;
            }
          if (vcl_abs(rVal + sVal - nCeil2) < vcl_abs(this->GetNeighBinLength()))
            {
            Px_y2 +=  static_cast<double>(this->GetHisto()[r][s]) * areaInv;
            }
          }
        }
      if (Px_y != 0.) out += Px_y * vcl_log(Px_y);
      if (Px_y2 != 0.) out += Px_y2 * vcl_log(Px_y2);
      }

    if (out != 0) out = -out;

    return out;
  }

};

} // namespace Functor
} // namespace otb

#endif
