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
#ifndef __otbVarianceTextureFunctor_h
#define __otbVarianceTextureFunctor_h

#include "vcl_deprecated_header.h"
#include "otbMeanTextureFunctor.h"

namespace otb
{
namespace Functor
{
/** \class VarianceTextureFunctor
 *  \brief <b>DEPRECATED<\b>
 *
 * \deprecated in OTB 3.2.2, please use
 * otbScalarImageToTexturesFilter instead.
 */

template <class TScalarInputPixelType, class TScalarOutputPixelType>
class ITK_EXPORT VarianceTextureFunctor :
  public MeanTextureFunctor<TScalarInputPixelType, TScalarOutputPixelType>
{
public:
  /** Return the texture name */
  virtual std::string GetName() const
  {
    return "VarianceTexture";
  }

  VarianceTextureFunctor()
    {
    };

  virtual ~VarianceTextureFunctor(){}

  typedef MeanTextureFunctor<TScalarInputPixelType, TScalarOutputPixelType> Superclass;
  typedef typename Superclass::NeighborhoodType                             NeighborhoodType;

  virtual double ComputeOverSingleChannel(const NeighborhoodType& neigh, const NeighborhoodType& neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
    double mean = Superclass::ComputeOverSingleChannel(neigh, neighOff);
    double area = static_cast<double>(neigh.GetSize()[0] * neigh.GetSize()[1]);
    double areaInv = 1 / area;
    double out = 0.;
    for (unsigned r = 0; r < this->GetHisto().size(); ++r)
      {
      for (unsigned s = 0; s < this->GetHisto()[r].size(); ++s)
        {
        double p = static_cast<double>(this->GetHisto()[r][s]) * areaInv;
        double square = vcl_pow((((static_cast<double>(s) + 0.5) * this->GetNeighBinLength()) - mean), 2);
        out += square * p;
        }
      }

    return out;
  }
};

} // namespace Functor
} // namespace otb

#endif
