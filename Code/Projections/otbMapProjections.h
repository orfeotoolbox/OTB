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

#include "otbMapProjectionDeprecated.h"

#include "otbUtmMapProjection.h"
#include "otbLambert2EtenduProjection.h"
#include "otbLambert3CartoSudProjection.h"
#include "otbLambert93Projection.h"
#include "otbEckert4MapProjection.h"
#include "otbSinusoidalMapProjection.h"
#include "otbMollweidMapProjection.h"
#include "otbSVY21MapProjection.h"

namespace otb
{

typedef Eckert4MapProjection<TransformDirection::INVERSE>                               Eckert4InverseProjection;
typedef Eckert4MapProjection<TransformDirection::FORWARD>                               Eckert4ForwardProjection;
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
typedef MollweidMapProjection<TransformDirection::INVERSE>                              MollweidInverseProjection;
typedef MollweidMapProjection<TransformDirection::FORWARD>                              MollweidForwardProjection;
typedef SinusoidalMapProjection<TransformDirection::INVERSE>                            SinusoidalInverseProjection;
typedef SinusoidalMapProjection<TransformDirection::FORWARD>                            SinusoidalForwardProjection;
typedef TransMercatorMapProjection<TransformDirection::INVERSE>                         TransMercatorInverseProjection;
typedef TransMercatorMapProjection<TransformDirection::FORWARD>                         TransMercatorForwardProjection;
typedef UtmMapProjection<TransformDirection::INVERSE>                                   UtmInverseProjection;
typedef UtmMapProjection<TransformDirection::FORWARD>                                   UtmForwardProjection;

// Definitions to maintain backward compatibility, but the GenericMapProjection
// should be used instead.
typedef MapProjectionDeprecated<MapProjectionType::ALBERS, TransformDirection::INVERSE>               AlbersInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::ALBERS, TransformDirection::FORWARD>               AlbersForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::AZIMEQUDIST, TransformDirection::INVERSE>          AzimEquDistInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::AZIMEQUDIST, TransformDirection::FORWARD>          AzimEquDistForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::BNG, TransformDirection::INVERSE>                  BngInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::BNG, TransformDirection::FORWARD>                  BngForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::BONNE, TransformDirection::INVERSE>                BonneInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::BONNE, TransformDirection::FORWARD>                BonneForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::CADRG, TransformDirection::INVERSE>                CadrgInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::CADRG, TransformDirection::FORWARD>                CadrgForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::CASSINI, TransformDirection::INVERSE>              CassiniInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::CASSINI, TransformDirection::FORWARD>              CassiniForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::CYLEQUAREA, TransformDirection::INVERSE>           CylEquAreaInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::CYLEQUAREA, TransformDirection::FORWARD>           CylEquAreaForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::ECKERT6, TransformDirection::INVERSE>              Eckert6InverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::ECKERT6, TransformDirection::FORWARD>              Eckert6ForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::GNOMONIC, TransformDirection::INVERSE>             GnomonicInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::GNOMONIC, TransformDirection::FORWARD>             GnomonicForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::LLXY, TransformDirection::INVERSE>                 LlxyInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::LLXY, TransformDirection::FORWARD>                 LlxyForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::EQUDISTCYL, TransformDirection::INVERSE>           EquDistCylInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::EQUDISTCYL, TransformDirection::FORWARD>           EquDistCylForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::MERCATOR, TransformDirection::INVERSE>             MercatorInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::MERCATOR, TransformDirection::FORWARD>             MercatorForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::MILLER, TransformDirection::INVERSE>               MillerInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::MILLER, TransformDirection::FORWARD>               MillerForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::NEWZEALANDMAPGRID, TransformDirection::INVERSE>    NewZealandMapGridInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::NEWZEALANDMAPGRID, TransformDirection::FORWARD>    NewZealandMapGridForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::OBLIQUEMERCATOR, TransformDirection::INVERSE>      ObliqueMercatorInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::OBLIQUEMERCATOR, TransformDirection::FORWARD>      ObliqueMercatorForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::ORTHOGRAPHIC, TransformDirection::INVERSE>         OrthoGraphicInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::ORTHOGRAPHIC, TransformDirection::FORWARD>         OrthoGraphicForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::POLARSTEREO, TransformDirection::INVERSE>          PolarStereoInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::POLARSTEREO, TransformDirection::FORWARD>          PolarStereoForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::POLYCONIC, TransformDirection::INVERSE>            PolyconicInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::POLYCONIC, TransformDirection::FORWARD>            PolyconicForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::SPACEOBLIQUEMERCATOR, TransformDirection::INVERSE> SpaceObliqueMercatorInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::SPACEOBLIQUEMERCATOR, TransformDirection::FORWARD> SpaceObliqueMercatorForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::STEREOGRAPHIC, TransformDirection::INVERSE>        StereographicInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::STEREOGRAPHIC, TransformDirection::FORWARD>        StereographicForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::TRANSCYLEQUAREA, TransformDirection::INVERSE>      TransCylEquAreaInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::TRANSCYLEQUAREA, TransformDirection::FORWARD>      TransCylEquAreaForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::UPS, TransformDirection::INVERSE>                  UpsInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::UPS, TransformDirection::FORWARD>                  UpsForwardProjection;
typedef MapProjectionDeprecated<MapProjectionType::VANDERGRINTEN, TransformDirection::INVERSE>        VanDerGrintenInverseProjection;
typedef MapProjectionDeprecated<MapProjectionType::VANDERGRINTEN, TransformDirection::FORWARD>        VanDerGrintenForwardProjection;

} //namespace otb
#endif
