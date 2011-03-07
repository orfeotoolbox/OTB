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
#include "otbLambert2EtenduProjection.h"
#include "otbLambert3CartoSudProjection.h"
#include "otbLambert93Projection.h"
#include "otbEckert4MapProjection.h"
#include "otbTransMercatorMapProjection.h"
#include "otbSinusoidalMapProjection.h"
#include "otbMollweidMapProjection.h"
#include "otbSVY21MapProjection.h"

namespace otb
{

typedef MapProjection<ossimAlbersProjection, Transform::INVERSE>               AlbersInverseProjection;
typedef MapProjection<ossimAlbersProjection, Transform::FORWARD>               AlbersForwardProjection;
typedef MapProjection<ossimAzimEquDistProjection, Transform::INVERSE>          AzimEquDistInverseProjection;
typedef MapProjection<ossimAzimEquDistProjection, Transform::FORWARD>          AzimEquDistForwardProjection;
typedef MapProjection<ossimBngProjection, Transform::INVERSE>                  BngInverseProjection;
typedef MapProjection<ossimBngProjection, Transform::FORWARD>                  BngForwardProjection;
typedef MapProjection<ossimBonneProjection, Transform::INVERSE>                BonneInverseProjection;
typedef MapProjection<ossimBonneProjection, Transform::FORWARD>                BonneForwardProjection;
typedef MapProjection<ossimCadrgProjection, Transform::INVERSE>                CadrgInverseProjection;
typedef MapProjection<ossimCadrgProjection, Transform::FORWARD>                CadrgForwardProjection;
typedef MapProjection<ossimCassiniProjection, Transform::INVERSE>              CassiniInverseProjection;
typedef MapProjection<ossimCassiniProjection, Transform::FORWARD>              CassiniForwardProjection;
typedef MapProjection<ossimCylEquAreaProjection, Transform::INVERSE>           CylEquAreaInverseProjection;
typedef MapProjection<ossimCylEquAreaProjection, Transform::FORWARD>           CylEquAreaForwardProjection;
typedef Eckert4MapProjection<Transform::INVERSE>                               Eckert4InverseProjection;
typedef Eckert4MapProjection<Transform::FORWARD>                               Eckert4ForwardProjection;
typedef MapProjection<ossimEckert6Projection, Transform::INVERSE>              Eckert6InverseProjection;
typedef MapProjection<ossimEckert6Projection, Transform::FORWARD>              Eckert6ForwardProjection;
typedef MapProjection<ossimGnomonicProjection, Transform::INVERSE>             GnomonicInverseProjection;
typedef MapProjection<ossimGnomonicProjection, Transform::FORWARD>             GnomonicForwardProjection;
typedef LambertConformalConicMapProjection<Transform::INVERSE>                 LambertConformalConicInverseProjection;
typedef LambertConformalConicMapProjection<Transform::FORWARD>                 LambertConformalConicForwardProjection;
typedef Lambert2EtenduProjection<Transform::INVERSE>                           Lambert2EtenduInverseProjection;
typedef Lambert2EtenduProjection<Transform::FORWARD>                           Lambert2EtenduForwardProjection;
typedef Lambert3CartoSudProjection<Transform::INVERSE>                         Lambert3CartoSudInverseProjection;
typedef Lambert3CartoSudProjection<Transform::FORWARD>                         Lambert3CartoSudForwardProjection;
typedef Lambert93Projection<Transform::INVERSE>                                Lambert93InverseProjection;
typedef Lambert93Projection<Transform::FORWARD>                                Lambert93ForwardProjection;
typedef SVY21MapProjection<Transform::INVERSE>                                 SVY21InverseProjection;
typedef SVY21MapProjection<Transform::FORWARD>                                 SVY21ForwardProjection;
typedef MapProjection<ossimLlxyProjection, Transform::INVERSE>                 LlxyInverseProjection;
typedef MapProjection<ossimLlxyProjection, Transform::FORWARD>                 LlxyForwardProjection;
typedef MapProjection<ossimEquDistCylProjection, Transform::INVERSE>           EquDistCylInverseProjection;
typedef MapProjection<ossimEquDistCylProjection, Transform::FORWARD>           EquDistCylForwardProjection;
typedef MapProjection<ossimMercatorProjection, Transform::INVERSE>             MercatorInverseProjection;
typedef MapProjection<ossimMercatorProjection, Transform::FORWARD>             MercatorForwardProjection;
typedef MapProjection<ossimMillerProjection, Transform::INVERSE>               MillerInverseProjection;
typedef MapProjection<ossimMillerProjection, Transform::FORWARD>               MillerForwardProjection;
typedef MollweidMapProjection<Transform::INVERSE>                              MollweidInverseProjection;
typedef MollweidMapProjection<Transform::FORWARD>                              MollweidForwardProjection;
typedef MapProjection<ossimNewZealandMapGridProjection, Transform::INVERSE>    NewZealandMapGridInverseProjection;
typedef MapProjection<ossimNewZealandMapGridProjection, Transform::FORWARD>    NewZealandMapGridForwardProjection;
typedef MapProjection<ossimObliqueMercatorProjection, Transform::INVERSE>      ObliqueMercatorInverseProjection;
typedef MapProjection<ossimObliqueMercatorProjection, Transform::FORWARD>      ObliqueMercatorForwardProjection;
typedef MapProjection<ossimOrthoGraphicProjection, Transform::INVERSE>         OrthoGraphicInverseProjection;
typedef MapProjection<ossimOrthoGraphicProjection, Transform::FORWARD>         OrthoGraphicForwardProjection;
typedef MapProjection<ossimPolarStereoProjection, Transform::INVERSE>          PolarStereoInverseProjection;
typedef MapProjection<ossimPolarStereoProjection, Transform::FORWARD>          PolarStereoForwardProjection;
typedef MapProjection<ossimPolyconicProjection, Transform::INVERSE>            PolyconicInverseProjection;
typedef MapProjection<ossimPolyconicProjection, Transform::FORWARD>            PolyconicForwardProjection;
typedef SinusoidalMapProjection<Transform::INVERSE>                            SinusoidalInverseProjection;
typedef SinusoidalMapProjection<Transform::FORWARD>                            SinusoidalForwardProjection;
typedef MapProjection<ossimSpaceObliqueMercatorProjection, Transform::INVERSE> SpaceObliqueMercatorInverseProjection;
typedef MapProjection<ossimSpaceObliqueMercatorProjection, Transform::FORWARD> SpaceObliqueMercatorForwardProjection;
typedef MapProjection<ossimStereographicProjection, Transform::INVERSE>        StereographicInverseProjection;
typedef MapProjection<ossimStereographicProjection, Transform::FORWARD>        StereographicForwardProjection;
typedef MapProjection<ossimTransCylEquAreaProjection, Transform::INVERSE>      TransCylEquAreaInverseProjection;
typedef MapProjection<ossimTransCylEquAreaProjection, Transform::FORWARD>      TransCylEquAreaForwardProjection;
typedef TransMercatorMapProjection<Transform::INVERSE>                         TransMercatorInverseProjection;
typedef TransMercatorMapProjection<Transform::FORWARD>                         TransMercatorForwardProjection;
typedef MapProjection<ossimUpsProjection, Transform::INVERSE>                  UpsInverseProjection;
typedef MapProjection<ossimUpsProjection, Transform::FORWARD>                  UpsForwardProjection;
typedef UtmMapProjection<Transform::INVERSE>                                   UtmInverseProjection;
typedef UtmMapProjection<Transform::FORWARD>                                   UtmForwardProjection;
typedef MapProjection<ossimVanDerGrintenProjection, Transform::INVERSE>        VanDerGrintenInverseProjection;
typedef MapProjection<ossimVanDerGrintenProjection, Transform::FORWARD>        VanDerGrintenForwardProjection;

} //namespace otb
#endif
