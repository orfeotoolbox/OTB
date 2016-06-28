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
 * and used with his autorization.
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
