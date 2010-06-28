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
#ifndef __otbInformationMeasureOfCorrelation1TextureFunctor_h
#define __otbInformationMeasureOfCorrelation1TextureFunctor_h

#include "#vcl_deprecated_header.h#"
#include "otbEntropyTextureFunctor.h"

namespace otb
{
namespace Functor
{
/** \class InformationMeasureOfCorrelation1TextureFunctor
 *  \brief <b>DEPRECATED<\b>
 *
 * \deprecated in OTB 3.2.2, please use
 * otbScalarImageToAdvancedTexturesFilter instead.
 */

template  <class TScalarInputPixelType, class TScalarOutputPixelType>
class ITK_EXPORT InformationMeasureOfCorrelation1TextureFunctor :
  public EntropyTextureFunctor<TScalarInputPixelType, TScalarOutputPixelType>
{
public:
  /** Return the texture name */
  virtual std::string GetName() const
  {
    return "InformationMeasureOfCorrelation1Texture";
  }

  InformationMeasureOfCorrelation1TextureFunctor()
    {
    };
  virtual ~InformationMeasureOfCorrelation1TextureFunctor(){}

  typedef EntropyTextureFunctor<TScalarInputPixelType, TScalarOutputPixelType> Superclass;
  typedef typename Superclass::NeighborhoodType                                NeighborhoodType;

  virtual double ComputeOverSingleChannel(const NeighborhoodType& neigh, const NeighborhoodType& neighOff)
  {
    double HXY = Superclass::ComputeOverSingleChannel(neigh, neighOff);

    double area = static_cast<double>(neigh.GetSize()[0] * neigh.GetSize()[1]);
    double areaInv = 1 / area;
    double out = 0.;
    // Stores marginal proba values
    std::vector<double> PxVector;
    std::vector<double> PyVector;

    // Computes HX
    double HX = 0.;
    for (unsigned r = 0; r < this->GetHisto()[0].size(); ++r)
      {
      double sumTemp = 0.;
      for (unsigned s = 0; s < this->GetHisto().size(); ++s)
        {
        sumTemp += this->GetHisto()[s][r] * areaInv;
        }
      PxVector.push_back(sumTemp);
      if (sumTemp != 0.) HX +=  sumTemp * vcl_log(sumTemp);
      }
    if (HX != 0.) HX = -HX;

    // Computes HY
    double HY = 0.;
    for (unsigned r = 0; r < this->GetHisto().size(); ++r)
      {
      double sumTemp = 0.;
      for (unsigned s = 0; s < this->GetHisto()[r].size(); ++s)
        {
        sumTemp += this->GetHisto()[r][s] * areaInv;
        }
      PyVector.push_back(sumTemp);
      if (sumTemp != 0.) HY +=  sumTemp * vcl_log(sumTemp);
      }
    if (HY != 0.) HY = -HY;

    // Computes HXY1
    double HXY1 = 0.;
    for (unsigned r = 0; r < this->GetHisto().size(); ++r)
      {
      for (unsigned s = 0; s < this->GetHisto()[r].size(); ++s)
        {
        if (PyVector[r] * PxVector[s] != 0.)
          {
          double p = this->GetHisto()[r][s] * areaInv;
          HXY1 += p * vcl_log(PyVector[r] * PxVector[s]);
          }

        }
      }
    if (HXY1 != 0.) HXY1 = -HXY1;

    if (std::max(HX, HY) != 0.) out = (HXY - HXY1) / std::max(HX, HY);

    return out;
  }

};

} // namespace Functor
} // namespace otb

#endif
