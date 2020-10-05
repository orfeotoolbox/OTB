/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbCBAMI_h
#define otbCBAMI_h

#include <vector>
#include "itkMath.h"

namespace otb
{

namespace Functor
{

template <class TInput1, class TInput2, class TOutput>
class CBAMI
{
public:
  typedef typename std::vector<TOutput>         VectorType;
  typedef typename VectorType::iterator         IteratorType;
  typedef typename std::vector<VectorType>      VectorOfVectorType;
  typedef typename VectorOfVectorType::iterator VecOfVecIteratorType;

  CBAMI()
  {
  }
  virtual ~CBAMI()
  {
  }
  inline TOutput operator()(const TInput1& itA, const TInput2& itB) const
  {
    double     epsilon = 0.01;
    VectorType vecA;
    VectorType vecB;

    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
    {

      vecA.push_back(static_cast<double>(itA.GetPixel(pos)));
      vecB.push_back(static_cast<double>(itB.GetPixel(pos)));
    }

    normalizeInPlace(vecA);
    normalizeInPlace(vecB);

    return static_cast<TOutput>(-std::log(static_cast<double>(PhiMI(vecA, vecB) + epsilon)));
  }

protected:
  inline void normalizeInPlace(VectorType vx) const
  {

    TOutput Ex = 0.0;

    IteratorType itx;

    for (itx = vx.begin(); itx < vx.end(); ++itx)
    {
      Ex += static_cast<TOutput>(*itx);
    }

    Ex /= (vx.size());

    TOutput Vx = 0.0;

    for (itx = vx.begin(); itx < vx.end(); ++itx)
    {
      Vx += static_cast<TOutput>(std::pow(static_cast<double>((*itx) - Ex), 2));
    }

    Vx /= (vx.size());

    for (itx = vx.begin(); itx < vx.end(); ++itx)
    {
      (*itx) = ((*itx) - Ex) / static_cast<TOutput>(std::sqrt(static_cast<double>(Vx)));
    }
  }
  inline TOutput Exyc(VectorType vx, VectorType vy) const
  {

    TOutput Exy = 0.0;
    TOutput Ex  = 0.0;
    TOutput Ey  = 0.0;

    IteratorType itx;
    IteratorType ity;

    for (itx = vx.begin(), ity = vy.begin(); itx < vx.end(); ++itx, ++ity)
    {
      // Ex  += (*itx);
      // Ey  += (*ity);
      Exy += (*itx) * (*ity);
    }

    // Ex /= (vx.size());
    // Ey /= (vy.size());
    Exy /= (vx.size());

    return Exy - Ex * Ey;
  }

  inline TOutput Exyztc(VectorType vx, VectorType vy, VectorType vz, VectorType vt) const
  {

    TOutput Exyzt = 0.0;

    TOutput Exyz = 0.0;
    TOutput Exyt = 0.0;
    TOutput Exzt = 0.0;
    TOutput Eyzt = 0.0;

    TOutput Exy = 0.0;
    TOutput Exz = 0.0;
    TOutput Ext = 0.0;
    TOutput Eyz = 0.0;
    TOutput Eyt = 0.0;
    TOutput Ezt = 0.0;

    TOutput Ex = 0.0;
    TOutput Ey = 0.0;
    TOutput Ez = 0.0;
    TOutput Et = 0.0;

    IteratorType itx;
    IteratorType ity;
    IteratorType itz;
    IteratorType itt;

    for (itx = vx.begin(), ity = vy.begin(), itz = vz.begin(), itt = vt.begin(); itx < vx.end(); ++itx, ++ity, itz++, itt++)
    {
      // Ex  += (*itx);
      // Ey  += (*ity);
      // Ez  += (*itz);
      // Et  += (*itt);

      Exy += (*itx) * (*ity);
      Exz += (*itx) * (*itz);
      Ext += (*itx) * (*itt);
      Eyz += (*ity) * (*itz);
      Eyt += (*ity) * (*itt);
      Ezt += (*itz) * (*itt);

      Exyz += (*itx) * (*ity) * (*itz);
      Exyt += (*itx) * (*ity) * (*itt);
      Exzt += (*itx) * (*itz) * (*itt);
      Eyzt += (*ity) * (*itz) * (*itt);

      Exyzt += (*itx) * (*ity) * (*itz) * (*itt);
    }

    /*Ex  /= (vx.size());
    Ey  /= (vx.size());
    Ez  /= (vx.size());
    Et  /= (vx.size()); */

    Exy /= (vx.size());
    Exz /= (vx.size());
    Ext /= (vx.size());
    Eyz /= (vx.size());
    Eyt /= (vx.size());
    Ezt /= (vx.size());

    Exyz /= (vx.size());
    Exyt /= (vx.size());
    Exzt /= (vx.size());
    Eyzt /= (vx.size());

    TOutput result = Exyzt - Exyz * Et - Exyt * Ez - Exzt * Ey - Eyzt * Ex + Exy * Ez * Et + Exz * Et * Ey + Ext * Ey * Ez + Eyz * Et * Ex + Eyt * Ex * Ez +
                     Ezt * Ex * Ey - 3 * Ex * Ey * Ez * Et;

    return result;
  }

  inline TOutput Rxy(VectorType va, VectorType vb) const
  {

    return Exyc(va, vb);
  }

  inline TOutput Qxijkl(VectorType va, VectorType vb, VectorType vc, VectorType vd) const
  {
    //    IteratorType ita;
    //    IteratorType itb;
    //    IteratorType itc;
    //    IteratorType itd;

    TOutput Eabcd_c = Exyztc(va, vb, vc, vd);

    TOutput Eab_c = Exyc(va, vb);
    TOutput Eac_c = Exyc(va, vc);
    TOutput Ead_c = Exyc(va, vd);
    TOutput Ecd_c = Exyc(vc, vd);
    TOutput Ebd_c = Exyc(vb, vd);
    TOutput Ebc_c = Exyc(vb, vc);

    return Eabcd_c - Eab_c * Ecd_c - Eac_c * Ebd_c - Ead_c * Ebc_c;
  }

  inline TOutput PhiMI(VectorType v1, VectorType v2) const
  {

    VectorOfVectorType donnees;
    donnees.push_back(v1);
    donnees.push_back(v2);

    VecOfVecIteratorType iti;
    VecOfVecIteratorType itj;
    VecOfVecIteratorType itk;
    VecOfVecIteratorType itl;

    TOutput termeR = 0.0;
    TOutput termeQ = 0.0;

    for (iti = donnees.begin(); iti < donnees.end(); ++iti)
      for (itj = donnees.begin(); itj < donnees.end(); ++itj)
      {
        if (iti != itj)
          termeR += static_cast<TOutput>(std::pow(static_cast<double>(Rxy((*iti), (*itj))), 2));

        for (itk = donnees.begin(); itk < donnees.end(); ++itk)
          for (itl = donnees.begin(); itl < donnees.end(); itl++)
          {
            if ((iti != itj) || (iti != itk) || (iti != itl))
              termeQ += static_cast<TOutput>(std::pow(static_cast<double>(Qxijkl((*iti), (*itj), (*itk), (*itl))), 2));
          }
      }

    return 1.0 / 4.0 * termeR + 1.0 / 48.0 * termeQ;
  }
};
}
}

#endif
