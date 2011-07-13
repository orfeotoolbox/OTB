#ifndef __dataSpecP5B_h
#define __dataSpecP5B_h


// ***********************************************************************
// dataSpec_P5.m - (february, 25th 2008)
// ***********************************************************************
// [1] = wavelength (nm)
// [2] = refractive index of leaf material
// [3] = specific absorption coefficient of chlorophyll (a+b) (cm2.microg-1)
// [4] = specific absorption coefficient of carotenoids (cm2.microg-1)
// [5] = specific absorption coefficient of brown pigments (arbitrary units)
// [6] = specific absorption coefficient of water (cm-1)
// [7] = specific absorption coefficient of dry matter (g.cm-1)
// [8] = direct light
// [9] = diffuse light
// [10] = dry soil
// [11] = wet soil
// Feret et al. (2008), PROSPECT-4 and 5: Advances in the Leaf Optical
// Properties Model Separating Photosynthetic Pigments, Remote Sensing of
// Environment
// The specific absorption coefficient corresponding to brown pigment is
// provided by Frederic Baret (EMMAH, INRA Avignon, baret@avignon.inra.fr)
// and used with his autorization.
// ***********************************************************************

struct dataSpec {
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

#define DATASPECSIZE 2101

#ifndef INDATASPEC_P5B_CXX_FILE
extern dataSpec dataSpecP5B[DATASPECSIZE];
#endif /* INDATASPEC_P5B_CXX_FILE */

#endif /* __dataSpecP5B_h */
