/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVegetationIndicesFunctor_h
#define otbVegetationIndicesFunctor_h

#include "otbMath.h"
#include "otbRadiometricIndice.h"

namespace otb
{

namespace Functor
{
/** \class NDVI
 *  \brief This functor computes the Normalized Difference Vegetation Index (NDVI)
 *
 *  [Rouse et al., 1973]
 * 
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class NDVI : public RadiometricIndice<TInput,TOutput>
{
public:
  NDVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    return static_cast<TOutput>(Compute(red,nir));
  }  

  // This static compute will be used in indices derived from NDVI
  static double Compute(const double & red, const double & nir)
  {
    if (std::abs(nir + red) < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return 0.;
      }

    return (nir - red) / (nir + red);

  }
};

/** \class RVI
 *  \brief This functor computes the Ratio Vegetation Index (RVI)
 *
 *  [Pearson et Miller, 1972]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class RVI : public RadiometricIndice<TInput,TOutput> 
{
public:
  RVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    if (std::abs(red)  < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>(nir / red));
  }
};

/** \class PVI
 *  \brief This functor computes the Perpendicular Vegetation Index (PVI)
 *
 *  [Richardson et Wiegand, 1977]
 *
 * Default values for A and B are from
 * C. L. Wiegand, A. J. Richardson, D. E. Escobar, and A. H. Gerbermann,
 * "Vegetation Indices in Crop Assessments", REMOTE SENS. ENVIRON. 35:105-119 (1991)
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class PVI : public RadiometricIndice<TInput,TOutput>
{
public:
  PVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    return (static_cast<TOutput>((nir - A * red - B) * C));
  }

  /** A and B parameters */
  static constexpr double A = 0.90893;
  static constexpr double B = 7.46216;
  static constexpr double C = 9.74;

};

/** \class SAVI
 *  \brief This functor computes the Soil Adjusted Vegetation Index (SAVI)
 *
 *  [Huete, 1988]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class SAVI : public RadiometricIndice<TInput,TOutput>
{
public:
  SAVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    if (std::abs(nir + red + L)  < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>(((nir - red) * (1 + L)) / (nir + red + L)));
  }

  /** L correction */
  static constexpr double L = 0.5;

};

/** \class TSAVI
 *  \brief This functor computes the Transformed Soil Adjusted Vegetation Index (TSAVI)
 *
 *  [Baret et al. 1989, Baret et Guyot, 1991]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class TSAVI : public RadiometricIndice<TInput,TOutput>
{
public:
  TSAVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    double denominator = A * nir + red + X * (1. + A * A);

    if (std::abs(denominator) < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((A * (nir - A * red - S)) / denominator));
  }

  /** A and S parameters */
  static constexpr double A = 0.7;
  static constexpr double S = 0.9;
  /** X parameter */
  static constexpr double X = 0.08;
};

/** \class WDVI
 *  \brief This functor computes the Weighted Difference Vegetation Index (WDVI)
 *
 *  [Clevers, 1988]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class WDVI : public RadiometricIndice<TInput,TOutput>
{
public:
  /// Constructor
  WDVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    return static_cast<TOutput>(Compute(red,nir));
  }

  static double Compute(const double & red, const double & nir)
  {
    return (nir - S * red);
  }

  /** Slope of soil line */
  static constexpr double S = 0.4;
};

/** \class MSAVI
 *  \brief This functor computes the Modified Soil Adjusted Vegetation Index (MSAVI)
 *
 *  [Qi et al., 1994]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */

template <class TInput, class TOutput>
class MSAVI : public RadiometricIndice<TInput,TOutput>
{
public:
  MSAVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    double ndvi = NDVI<TInput,TOutput>::Compute(red,nir);
    double wdvi = WDVI<TInput,TOutput>::Compute(red, nir);

    double L = 1 - 2 * S * ndvi * wdvi;

    double denominator = nir + red + L;

    if (std::abs(denominator)  < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return static_cast<TOutput>(0.);
      }

    return (static_cast<TOutput>(((nir - red) * (1 + L)) / denominator));
  }

private:
  /** Slope of soil line */
  static constexpr double S = 0.4;
};

/** \class MSAVI2
 *  \brief This functor computes the Modified Soil Adjusted Vegetation Index (MSAVI2)
 *
 *  [Qi et al., 1994]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class MSAVI2 : public RadiometricIndice<TInput,TOutput>
{
public:

  MSAVI2() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    double sqrt_value = (2 * nir + 1) * (2 * nir + 1) - 8 * (nir - red);
    if (sqrt_value < 0.)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((2 * nir + 1 - std::sqrt(sqrt_value)) / 2.));
  }

};

/** \class GEMI
 *  \brief This functor computes the Global Environment Monitoring Index (GEMI)
 *
 *  [Pinty & Verstraete , 1992]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class GEMI : public RadiometricIndice<TInput,TOutput>
{
public:
  GEMI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red = this->Value(CommonBandNames::RED,input);
    auto nir = this->Value(CommonBandNames::NIR,input);

    double nu;
    double num_nu;
    double denom_nu = nir + red + 0.5;

    if (std::abs(denom_nu)  < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      nu = 0;
      }
    else
      {
      num_nu = 2 * (nir * nir - red * red) + 1.5 * nir + 0.5 * red;
      nu = num_nu / denom_nu;
      }

    double denom_GEMI = 1 - red;
    if (std::abs(denom_GEMI)  < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((nu * (1 - 0.25 * nu) - (red - 0.125)) / denom_GEMI));
  }

};

/** \class AVI
 *  \brief This functor computes the Angular Vegetation Index (AVI)
 *
 *  This vegetation index use three inputs channels
 *
 *  [Plummer & al., 1994]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class AVI: public RadiometricIndice<TInput,TOutput>
{
public:
  AVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::GREEN, CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto green = this->Value(CommonBandNames::GREEN,input);
    auto red   = this->Value(CommonBandNames::RED,input);
    auto nir   = this->Value(CommonBandNames::NIR,input);

    constexpr double dfact1 = (LambdaNir - LambdaR) / LambdaR;
    constexpr double dfact2 = (LambdaR - LambdaG) / LambdaR;
    double dterm1;
    double dterm2;
    if (std::abs(nir - red)  < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      dterm1 = 0;
      }
    else
      {
      dterm1 = std::atan(dfact1 / (nir - red));
      }

    if (std::abs(green - red)  < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      dterm2 = 0;
      }
    else
      {
      dterm2 = std::atan(dfact2 / (green - red));
      }

    return static_cast<TOutput>(dterm1 + dterm2);

  }

  /**  Central wavelength of the green channel (=Lambda1) */
  static constexpr double LambdaG = 560;

  /**  Central wavelength of the red channel (=Lambda2) */
  static constexpr double LambdaR = 660;

  /**  Central wavelength of the nir channel (=Lambda3) */
  static constexpr double LambdaNir = 830;
};

/** \class ARVI
 *  \brief This functor computes the Atmospherically Resistant Vegetation Index (ARVI)
 *
 *  This vegetation index use three inputs channels
 *
 *  [Yoram J. Kaufman and Didier Tanre, 1992]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class ARVI : public RadiometricIndice<TInput,TOutput>
{
public:

  ARVI(): RadiometricIndice<TInput,TOutput>({CommonBandNames::BLUE, CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto blue  = this->Value(CommonBandNames::BLUE,input);
    auto red   = this->Value(CommonBandNames::RED,input);
    auto nir   = this->Value(CommonBandNames::NIR,input);

    double RHOrb = red - Gamma * (blue - red);
    double denominator = nir + RHOrb;
    if (std::abs(denominator)  < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((nir - RHOrb) / denominator));
  }

  /** Gamma parameter */
  static constexpr double Gamma = 0.5;
};

/** \class EVI
 *  \brief This functor computes the Enhanced Vegetation Index (EVI)
 *
 *  This vegetation index use three inputs channels
 *
 *  [Huete, Justice, & Liu, 1994; Huete, Liu, Batchily, & van Leeuwen, 1997]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class EVI : public RadiometricIndice<TInput,TOutput>
{
public:

  EVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::BLUE, CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto blue  = this->Value(CommonBandNames::BLUE,input);
    auto red   = this->Value(CommonBandNames::RED,input);
    auto nir   = this->Value(CommonBandNames::NIR,input);

    double denominator = nir + C1 * red - C2 * blue + L;
    if (std::abs(denominator) < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return (static_cast<TOutput>(0.));
      }
    return (static_cast<TOutput>(G * (nir - red) / denominator));
  }

  /** Gain factor */
  static constexpr double G = 2.5;

  /** Coefficient of the aerosol resistance term */
  static constexpr double C1 = 6.0;

  /** Coefficient of the aerosol resistance term */
  static constexpr double C2 = 7.5;

  /** Canopy background adjustment */
  static constexpr double L = 1.0;
};

/** \class IPVI
 *  \brief This functor computes the Infrared Percentage Vegetation Index (IPVI)
 *
 *  [Crippen, 1990]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class IPVI : public RadiometricIndice<TInput,TOutput>
{
public:
  IPVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red   = this->Value(CommonBandNames::RED,input);
    auto nir   = this->Value(CommonBandNames::NIR,input);

    if (std::abs(nir + red)  < RadiometricIndice<TInput,TOutput>::Epsilon)
      {
      return static_cast<TOutput>(0.);
      }
    else
      {
      return (static_cast<TOutput>(nir / (nir + red)));
      }
  }
};

/** \class TNDVI
 *  \brief This functor computes the Transformed NDVI (TNDVI)
 *
 *  [Deering, 1975]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class TNDVI : public RadiometricIndice<TInput,TOutput>
{
public:
  TNDVI() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red   = this->Value(CommonBandNames::RED,input);
    auto nir   = this->Value(CommonBandNames::NIR,input);

    double val = NDVI<TInput,TOutput>::Compute(red,nir) + 0.5;

    if (val < 0)
      {
      return  (static_cast<TOutput>(0));
      }
    else
      {
      return (static_cast<TOutput>(std::sqrt(val)));
      }
  }
};

/** \class LAIFromNDVILogarithmic
 *  \brief computes the LAI from NDVI
 *
 *  This functor computes the LAI from NDVI using a
 *  logarithmic relationship. Asrar et al. (1984), Baret and Guyot
 *  (1991) and Wilson and Meyers (2007). Default values for the
 *  parameters are taken from A. Bsaibes et al. / Remote Sensing of
 *  Environment 113 (2009) 716-729
 *
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 * \ingroup VegetationIndices
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class LAIFromNDVILogarithmic : public RadiometricIndice<TInput,TOutput>
{
public:
  LAIFromNDVILogarithmic() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}),
                             m_NdviSoil(0.1),
                             m_NdviInf(0.89),
                             m_ExtinctionCoefficient(0.71) {}

  void SetNdviSoil(const double & val)
  {
    m_NdviSoil = val;
  }

  const double & GetNdviSoil() const
  {
    return m_NdviSoil;
  }

  void SetNdviInf(const double & val)
  {
    m_NdviInf = val;
  }

  const double & GetNdviInf() const
  {
    return m_NdviInf;
  }

  void SetExtinctionCoefficient(const double & val)
  {
    m_ExtinctionCoefficient = val;
  }

  const double & GetExtionctionCoefficient() const
  {
    return m_ExtinctionCoefficient;
  }

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red   = this->Value(CommonBandNames::RED,input);
    auto nir   = this->Value(CommonBandNames::NIR,input);

    double val = NDVI<TInput,TOutput>::Compute(red,nir);

    if (val < 0)
      {
      return  (static_cast<TOutput>(0));
      }
    else
      {
      return static_cast<TOutput>(
                -(1.0/m_ExtinctionCoefficient)*std::log((val- m_NdviInf)/(m_NdviSoil-m_NdviInf)));
      }
  }

  double m_NdviSoil;
  double m_NdviInf;
  double m_ExtinctionCoefficient;
};


/** \class LAIFromReflectancesLinear
 * \brief computes the LAI from reflectances
 *
 *  This functor computes the LAI from reflectances using a
 *  linear relationship.
 *  \f$ LAI = \beta_0 + \sum_j \beta_j \rho_j \f$ where \f$ \rho \f$ are the
 *  reflectances
 *  Default values for the parameters are taken from A. Bsaibes et
 *  al. / Remote Sensing of Environment 113 (2009) 716-729
 *
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 * \ingroup VegetationIndices
 *
 * \ingroup OTBIndices
 */
template <class TInput, class TOutput>
class LAIFromReflectancesLinear : public RadiometricIndice<TInput,TOutput>
{
public:


  LAIFromReflectancesLinear() : RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}),
                                m_RedCoef(-17.91),
                                m_NirCoef(12.26) {}

  void SetRedCoef(const double & val)
  {
    m_RedCoef = val;
  }

  const double & GetRedCoef() const
  {
    return m_RedCoef;
  }

  void SetNirCoef(const double & val)
  {
    m_NirCoef = val;
  }

  const double & GetNirCoef() const
  {
    return m_NirCoef;
  }

  TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
  {
    auto red   = this->Value(CommonBandNames::RED,input);
    auto nir   = this->Value(CommonBandNames::NIR,input);

    return (static_cast<TOutput>(m_RedCoef*red+m_NirCoef*nir));
  }

  double m_RedCoef;
  double m_NirCoef;
};


/** \class LAIFromNDVIFormosat2Functor
 *
 * \brief use red and nir image band to compute LAI image using formula
 *   a*(exp(nir-red)/((red+nir)*b)-exp(c*b));
 *   with a = 0.1519 b = 3.9443 c = 0.13
 *  this formula is only valid for Formosat 2 reflectance TOC
 *
 *  This relationship have been presented by :
 *  M. Claverie, V. Demarez, B. Duchemin,
 *  Spatialization of crop leaf area index and biomass by combining a simple crop model
 *  safy and high spatial and temporal resolutions remote sensing data.
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
  */


  template <class TInput, class TOutput>
  class LAIFromNDVIFormosat2Functor : public RadiometricIndice<TInput,TOutput>
  {
  public:
    
    LAIFromNDVIFormosat2Functor(): RadiometricIndice<TInput,TOutput>({CommonBandNames::RED, CommonBandNames::NIR}) {}
    
    TOutput operator()(const itk::VariableLengthVector<TInput> & input) const override
    {
      auto red   = this->Value(CommonBandNames::RED,input);
      auto nir   = this->Value(CommonBandNames::NIR,input);

      if (std::abs(nir + red) < RadiometricIndice<TInput,TOutput>::Epsilon)
        {
        return static_cast<TOutput>(0.);
        }

      return  static_cast<TOutput>(A*((std::exp((nir-red)/((red+nir)*B))-std::exp(C*B))));
    }

    static constexpr double A = 0.1519;
    static constexpr double B = 3.9443;
    static constexpr double C = 0.13;
};


} // namespace Functor
} // namespace otb

#endif
