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


#ifndef otbDataSpecP5B_h
#define otbDataSpecP5B_h

namespace otb
{
/** \struct DataSpec
 * \brief This struct contains the specific absorption coefficients of each constituent and the refractive index of leaf materia
 *
 * [1] = wavelength (nm)
 * [2] = refractive index of leaf material
 * [3] = specific absorption coefficient of chlorophyll (a+b) (cm2.microg-1)
 * [4] = specific absorption coefficient of carotenoids (cm2.microg-1)
 * [5] = specific absorption coefficient of brown pigments (arbitrary units)
 * [6] = specific absorption coefficient of water (cm-1)
 * [7] = specific absorption coefficient of dry matter (g.cm-1)
 * [8] = direct light
 * [9] = diffuse light
 * [10] = dry soil
 * [11] = wet soil
 * Feret et al. (2008), PROSPECT-4 and 5: Advances in the Leaf Optical
 * Properties Model Separating Photosynthetic Pigments, Remote Sensing of
 * Environment
 * The specific absorption coefficient corresponding to brown pigment is
 * provided by Frederic Baret (EMMAH, INRA Avignon, baret@avignon.inra.fr)
 * and used with his authorization.
 */

struct DataSpec
{
  double lambda;
  double refLeafMatInd;
  double chlAbsCoef;
  double carAbsCoef;
  double brownAbsCoef;
  double waterAbsCoef;
  double dryAbsCoef;
  double directLight;
  double diffuseLight;
  double drySoil;
  double wetSoil;
};

extern const DataSpec DataSpecP5B[2101];
}

#endif /* otbDataSpecP5B_h */
