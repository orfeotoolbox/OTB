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
#ifndef __otbMapProjections_h
#define __otbMapProjections_h

#include "projection/ossimAlbersProjection.h"
#include "projection/ossimAzimEquDistProjection.h"
#include "projection/ossimBngProjection.h"
#include "projection/ossimBonneProjection.h"
#include "projection/ossimCadrgProjection.h"
#include "projection/ossimCassiniProjection.h"
#include "projection/ossimCylEquAreaProjection.h"
#include "projection/ossimEckert4Projection.h"
#include "projection/ossimEckert6Projection.h"
#include "projection/ossimGnomonicProjection.h"
#include "projection/ossimLambertConformalConicProjection.h"
#include "projection/ossimLlxyProjection.h"
#include "projection/ossimEquDistCylProjection.h"
#include "projection/ossimMercatorProjection.h"
#include "projection/ossimMillerProjection.h"
#include "projection/ossimMollweidProjection.h"
#include "projection/ossimNewZealandMapGridProjection.h"
#include "projection/ossimObliqueMercatorProjection.h"
#include "projection/ossimOrthoGraphicProjection.h"
#include "projection/ossimPolarStereoProjection.h"
#include "projection/ossimPolyconicProjection.h"
#include "projection/ossimSinusoidalProjection.h"
#include "projection/ossimSpaceObliqueMercatorProjection.h"
#include "projection/ossimStereographicProjection.h"
#include "projection/ossimTransCylEquAreaProjection.h"
#include "projection/ossimTransMercatorProjection.h"
#include "projection/ossimUpsProjection.h"
#include "projection/ossimUtmProjection.h"
#include "projection/ossimVanDerGrintenProjection.h"
#include "otbUtmMapProjection.h"
#include "otbLambertConformalConicMapProjection.h"
#include "otbEckert4MapProjection.h"
#include "otbTransMercatorMapProjection.h"
#include "otbSinusoidalMapProjection.h"
#include "otbMollweidMapProjection.h"

namespace otb{

typedef MapProjection<ossimAlbersProjection> AlbersProjection;
typedef MapProjection<ossimAzimEquDistProjection> AzimEquDistProjection;
typedef MapProjection<ossimBngProjection> BngProjection;
typedef MapProjection<ossimBonneProjection> BonneProjection;
typedef MapProjection<ossimCadrgProjection> CadrgProjection;
typedef MapProjection<ossimCassiniProjection> CassiniProjection;
typedef MapProjection<ossimCylEquAreaProjection> CylEquAreaProjection;
//typedef MapProjection<ossimEckert4Projection> Eckert4Projection;
typedef Eckert4MapProjection Eckert4Projection;
typedef MapProjection<ossimEckert6Projection> Eckert6Projection;
typedef MapProjection<ossimGnomonicProjection> GnomonicProjection;
//typedef MapProjection<ossimLambertConformalConicProjection> LambertConformalConicProjection;
typedef LambertConformalConicMapProjection LambertConformalConicProjection;
typedef MapProjection<ossimLlxyProjection> LlxyProjection;
typedef MapProjection<ossimEquDistCylProjection> EquDistCylProjection;
typedef MapProjection<ossimMercatorProjection> MercatorProjection;
typedef MapProjection<ossimMillerProjection> MillerProjection;
//typedef MapProjection<ossimMollweidProjection> MollweidProjection;
typedef MollweidMapProjection MollweidProjection;
typedef MapProjection<ossimNewZealandMapGridProjection> NewZealandMapGridProjection;
typedef MapProjection<ossimObliqueMercatorProjection> ObliqueMercatorProjection;
typedef MapProjection<ossimOrthoGraphicProjection> OrthoGraphicProjection;
typedef MapProjection<ossimPolarStereoProjection> PolarStereoProjection;
typedef MapProjection<ossimPolyconicProjection> PolyconicProjection;
//typedef MapProjection<ossimSinusoidalProjection> SinusoidalProjection;
typedef SinusoidalMapProjection SinusoidalProjection;
typedef MapProjection<ossimSpaceObliqueMercatorProjection> SpaceObliqueMercatorProjection;
typedef MapProjection<ossimStereographicProjection> StereographicProjection;
typedef MapProjection<ossimTransCylEquAreaProjection> TransCylEquAreaProjection;
//typedef MapProjection<ossimTransMercatorProjection> TransMercatorProjection;
typedef TransMercatorMapProjection TransMercatorProjection;
typedef MapProjection<ossimUpsProjection> UpsProjection;
//typedef MapProjection<ossimUtmProjection> UtmProjection;
typedef UtmMapProjection UtmProjection;
typedef MapProjection<ossimVanDerGrintenProjection> VanDerGrintenProjection;
}//namespace otb
#endif
