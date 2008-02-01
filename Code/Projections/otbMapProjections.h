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

namespace otb
{
  
  typedef MapProjection<ossimAlbersProjection,INVERSE>               AlbersInverseProjection;
  typedef MapProjection<ossimAlbersProjection,FORWARD>               AlbersForwardProjection;
  typedef MapProjection<ossimAzimEquDistProjection,INVERSE>          AzimEquDistInverseProjection;
  typedef MapProjection<ossimAzimEquDistProjection,FORWARD>          AzimEquDistForwardProjection;	
  typedef MapProjection<ossimBngProjection,INVERSE>                  BngInverseProjection;
  typedef MapProjection<ossimBngProjection,FORWARD>                  BngForwardProjection;
  typedef MapProjection<ossimBonneProjection,INVERSE>                BonneInverseProjection;
  typedef MapProjection<ossimBonneProjection,FORWARD>                BonneForwardProjection;	
  typedef MapProjection<ossimCadrgProjection,INVERSE>                CadrgInverseProjection;
  typedef MapProjection<ossimCadrgProjection,FORWARD>                CadrgForwardProjection;	
  typedef MapProjection<ossimCassiniProjection,INVERSE>              CassiniInverseProjection;
  typedef MapProjection<ossimCassiniProjection,FORWARD>              CassiniForwardProjection;
  typedef MapProjection<ossimCylEquAreaProjection,INVERSE>           CylEquAreaInverseProjection;
  typedef MapProjection<ossimCylEquAreaProjection,FORWARD>           CylEquAreaForwardProjection;
  typedef Eckert4MapProjection<INVERSE>                              Eckert4InverseProjection;
  typedef Eckert4MapProjection<FORWARD>                              Eckert4ForwardProjection;
  typedef MapProjection<ossimEckert6Projection,INVERSE>              Eckert6InverseProjection;
  typedef MapProjection<ossimEckert6Projection,FORWARD>              Eckert6ForwardProjection;
  typedef MapProjection<ossimGnomonicProjection,INVERSE>             GnomonicInverseProjection;
  typedef MapProjection<ossimGnomonicProjection,FORWARD>             GnomonicForwardProjection;
  typedef LambertConformalConicMapProjection<INVERSE>                LambertConformalConicInverseProjection;
  typedef LambertConformalConicMapProjection<FORWARD>                LambertConformalConicForwardProjection;	
  typedef MapProjection<ossimLlxyProjection,INVERSE>                 LlxyInverseProjection;
  typedef MapProjection<ossimLlxyProjection,FORWARD>                 LlxyForwardProjection;
  typedef MapProjection<ossimEquDistCylProjection,INVERSE>           EquDistCylInverseProjection;
  typedef MapProjection<ossimEquDistCylProjection,FORWARD>           EquDistCylForwardProjection;
  typedef MapProjection<ossimMercatorProjection,INVERSE>             MercatorInverseProjection;
  typedef MapProjection<ossimMercatorProjection,FORWARD>             MercatorForwardProjection;
  typedef MapProjection<ossimMillerProjection,INVERSE>               MillerInverseProjection;
  typedef MapProjection<ossimMillerProjection,FORWARD>               MillerForwardProjection;
  typedef MollweidMapProjection<INVERSE>                             MollweidInverseProjection;
  typedef MollweidMapProjection<FORWARD>                             MollweidForwardProjection;
  typedef MapProjection<ossimNewZealandMapGridProjection,INVERSE>    NewZealandMapGridInverseProjection;
  typedef MapProjection<ossimNewZealandMapGridProjection,FORWARD>    NewZealandMapGridForwardProjection;
  typedef MapProjection<ossimObliqueMercatorProjection,INVERSE>      ObliqueMercatorInverseProjection;
  typedef MapProjection<ossimObliqueMercatorProjection,FORWARD>      ObliqueMercatorForwardProjection;
  typedef MapProjection<ossimOrthoGraphicProjection,INVERSE>         OrthoGraphicInverseProjection;
  typedef MapProjection<ossimOrthoGraphicProjection,FORWARD>         OrthoGraphicForwardProjection;
  typedef MapProjection<ossimPolarStereoProjection,INVERSE>          PolarStereoInverseProjection;
  typedef MapProjection<ossimPolarStereoProjection,FORWARD>          PolarStereoForwardProjection;
  typedef MapProjection<ossimPolyconicProjection,INVERSE>            PolyconicInverseProjection;
  typedef MapProjection<ossimPolyconicProjection,FORWARD>            PolyconicForwardProjection;
  typedef SinusoidalMapProjection<INVERSE>                           SinusoidalInverseProjection;  
  typedef SinusoidalMapProjection<FORWARD>                           SinusoidalForwardProjection;
  typedef MapProjection<ossimSpaceObliqueMercatorProjection,INVERSE> SpaceObliqueMercatorInverseProjection;
  typedef MapProjection<ossimSpaceObliqueMercatorProjection,FORWARD> SpaceObliqueMercatorForwardProjection;
  typedef MapProjection<ossimStereographicProjection,INVERSE>        StereographicInverseProjection;
  typedef MapProjection<ossimStereographicProjection,FORWARD>        StereographicForwardProjection;
  typedef MapProjection<ossimTransCylEquAreaProjection,INVERSE>      TransCylEquAreaInverseProjection;
  typedef MapProjection<ossimTransCylEquAreaProjection,FORWARD>      TransCylEquAreaForwardProjection;
  typedef TransMercatorMapProjection<INVERSE>                        TransMercatorInverseProjection;
  typedef TransMercatorMapProjection<FORWARD>                        TransMercatorForwardProjection;
  typedef MapProjection<ossimUpsProjection,INVERSE>                  UpsInverseProjection;
  typedef MapProjection<ossimUpsProjection,FORWARD>                  UpsForwardProjection;
  typedef UtmMapProjection<INVERSE>                                  UtmInverseProjection;
  typedef UtmMapProjection<FORWARD>                                  UtmForwardProjection;
  typedef MapProjection<ossimVanDerGrintenProjection,INVERSE>        VanDerGrintenInverseProjection;
  typedef MapProjection<ossimVanDerGrintenProjection,FORWARD>        VanDerGrintenForwardProjection;
  
}//namespace otb
#endif
