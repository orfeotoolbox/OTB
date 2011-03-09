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

typedef MapProjection<ossimAlbersProjection, TransformDirection::INVERSE>               AlbersInverseProjection;
typedef MapProjection<ossimAlbersProjection, TransformDirection::FORWARD>               AlbersForwardProjection;
typedef MapProjection<ossimAzimEquDistProjection, TransformDirection::INVERSE>          AzimEquDistInverseProjection;
typedef MapProjection<ossimAzimEquDistProjection, TransformDirection::FORWARD>          AzimEquDistForwardProjection;
typedef MapProjection<ossimBngProjection, TransformDirection::INVERSE>                  BngInverseProjection;
typedef MapProjection<ossimBngProjection, TransformDirection::FORWARD>                  BngForwardProjection;
typedef MapProjection<ossimBonneProjection, TransformDirection::INVERSE>                BonneInverseProjection;
typedef MapProjection<ossimBonneProjection, TransformDirection::FORWARD>                BonneForwardProjection;
typedef MapProjection<ossimCadrgProjection, TransformDirection::INVERSE>                CadrgInverseProjection;
typedef MapProjection<ossimCadrgProjection, TransformDirection::FORWARD>                CadrgForwardProjection;
typedef MapProjection<ossimCassiniProjection, TransformDirection::INVERSE>              CassiniInverseProjection;
typedef MapProjection<ossimCassiniProjection, TransformDirection::FORWARD>              CassiniForwardProjection;
typedef MapProjection<ossimCylEquAreaProjection, TransformDirection::INVERSE>           CylEquAreaInverseProjection;
typedef MapProjection<ossimCylEquAreaProjection, TransformDirection::FORWARD>           CylEquAreaForwardProjection;
typedef Eckert4MapProjection<TransformDirection::INVERSE>                               Eckert4InverseProjection;
typedef Eckert4MapProjection<TransformDirection::FORWARD>                               Eckert4ForwardProjection;
typedef MapProjection<ossimEckert6Projection, TransformDirection::INVERSE>              Eckert6InverseProjection;
typedef MapProjection<ossimEckert6Projection, TransformDirection::FORWARD>              Eckert6ForwardProjection;
typedef MapProjection<ossimGnomonicProjection, TransformDirection::INVERSE>             GnomonicInverseProjection;
typedef MapProjection<ossimGnomonicProjection, TransformDirection::FORWARD>             GnomonicForwardProjection;
typedef LambertConformalConicMapProjection<TransformDirection::INVERSE>                 LambertConformalConicInverseProjection;
typedef LambertConformalConicMapProjection<TransformDirection::FORWARD>                 LambertConformalConicForwardProjection;
typedef Lambert2EtenduProjection<TransformDirection::INVERSE>                           Lambert2EtenduInverseProjection;
typedef Lambert2EtenduProjection<TransformDirection::FORWARD>                           Lambert2EtenduForwardProjection;
typedef Lambert3CartoSudProjection<TransformDirection::INVERSE>                         Lambert3CartoSudInverseProjection;
typedef Lambert3CartoSudProjection<TransformDirection::FORWARD>                         Lambert3CartoSudForwardProjection;
typedef Lambert93Projection<TransformDirection::INVERSE>                                Lambert93InverseProjection;
typedef Lambert93Projection<TransformDirection::FORWARD>                                Lambert93ForwardProjection;
typedef SVY21MapProjection<TransformDirection::INVERSE>                                 SVY21InverseProjection;
typedef SVY21MapProjection<TransformDirection::FORWARD>                                 SVY21ForwardProjection;
typedef MapProjection<ossimLlxyProjection, TransformDirection::INVERSE>                 LlxyInverseProjection;
typedef MapProjection<ossimLlxyProjection, TransformDirection::FORWARD>                 LlxyForwardProjection;
typedef MapProjection<ossimEquDistCylProjection, TransformDirection::INVERSE>           EquDistCylInverseProjection;
typedef MapProjection<ossimEquDistCylProjection, TransformDirection::FORWARD>           EquDistCylForwardProjection;
typedef MapProjection<ossimMercatorProjection, TransformDirection::INVERSE>             MercatorInverseProjection;
typedef MapProjection<ossimMercatorProjection, TransformDirection::FORWARD>             MercatorForwardProjection;
typedef MapProjection<ossimMillerProjection, TransformDirection::INVERSE>               MillerInverseProjection;
typedef MapProjection<ossimMillerProjection, TransformDirection::FORWARD>               MillerForwardProjection;
typedef MollweidMapProjection<TransformDirection::INVERSE>                              MollweidInverseProjection;
typedef MollweidMapProjection<TransformDirection::FORWARD>                              MollweidForwardProjection;
typedef MapProjection<ossimNewZealandMapGridProjection, TransformDirection::INVERSE>    NewZealandMapGridInverseProjection;
typedef MapProjection<ossimNewZealandMapGridProjection, TransformDirection::FORWARD>    NewZealandMapGridForwardProjection;
typedef MapProjection<ossimObliqueMercatorProjection, TransformDirection::INVERSE>      ObliqueMercatorInverseProjection;
typedef MapProjection<ossimObliqueMercatorProjection, TransformDirection::FORWARD>      ObliqueMercatorForwardProjection;
typedef MapProjection<ossimOrthoGraphicProjection, TransformDirection::INVERSE>         OrthoGraphicInverseProjection;
typedef MapProjection<ossimOrthoGraphicProjection, TransformDirection::FORWARD>         OrthoGraphicForwardProjection;
typedef MapProjection<ossimPolarStereoProjection, TransformDirection::INVERSE>          PolarStereoInverseProjection;
typedef MapProjection<ossimPolarStereoProjection, TransformDirection::FORWARD>          PolarStereoForwardProjection;
typedef MapProjection<ossimPolyconicProjection, TransformDirection::INVERSE>            PolyconicInverseProjection;
typedef MapProjection<ossimPolyconicProjection, TransformDirection::FORWARD>            PolyconicForwardProjection;
typedef SinusoidalMapProjection<TransformDirection::INVERSE>                            SinusoidalInverseProjection;
typedef SinusoidalMapProjection<TransformDirection::FORWARD>                            SinusoidalForwardProjection;
typedef MapProjection<ossimSpaceObliqueMercatorProjection, TransformDirection::INVERSE> SpaceObliqueMercatorInverseProjection;
typedef MapProjection<ossimSpaceObliqueMercatorProjection, TransformDirection::FORWARD> SpaceObliqueMercatorForwardProjection;
typedef MapProjection<ossimStereographicProjection, TransformDirection::INVERSE>        StereographicInverseProjection;
typedef MapProjection<ossimStereographicProjection, TransformDirection::FORWARD>        StereographicForwardProjection;
typedef MapProjection<ossimTransCylEquAreaProjection, TransformDirection::INVERSE>      TransCylEquAreaInverseProjection;
typedef MapProjection<ossimTransCylEquAreaProjection, TransformDirection::FORWARD>      TransCylEquAreaForwardProjection;
typedef TransMercatorMapProjection<TransformDirection::INVERSE>                         TransMercatorInverseProjection;
typedef TransMercatorMapProjection<TransformDirection::FORWARD>                         TransMercatorForwardProjection;
typedef MapProjection<ossimUpsProjection, TransformDirection::INVERSE>                  UpsInverseProjection;
typedef MapProjection<ossimUpsProjection, TransformDirection::FORWARD>                  UpsForwardProjection;
typedef UtmMapProjection<TransformDirection::INVERSE>                                   UtmInverseProjection;
typedef UtmMapProjection<TransformDirection::FORWARD>                                   UtmForwardProjection;
typedef MapProjection<ossimVanDerGrintenProjection, TransformDirection::INVERSE>        VanDerGrintenInverseProjection;
typedef MapProjection<ossimVanDerGrintenProjection, TransformDirection::FORWARD>        VanDerGrintenForwardProjection;

} //namespace otb
#endif
