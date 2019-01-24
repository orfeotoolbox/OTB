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

#ifndef otbMapProjections_h
#define otbMapProjections_h

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

} //namespace otb
#endif
