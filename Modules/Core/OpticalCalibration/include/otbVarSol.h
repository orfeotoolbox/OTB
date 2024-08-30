/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVarSol_h
#define otbVarSol_h

#include "itkMacro.h"
#include "otbMath.h"

namespace otb
{

/** \class VarSol
   * \brief VarSol operations.
   *
   * Call VarSol main function
   *
   * [1] Vermote, E., Tanre, D., Deuze, J., Herman, M., Morcette, J., 1997.
   * Second simulation of the satellite signal in the solar spectrum, 6S: An overview.
   * IEEE Transactions on Geoscience and Remote Sensing 35
   * \ingroup Radiometry
   *
 *
 * \ingroup OTBOpticalCalibration
 */
class ITK_EXPORT VarSol
{
public:
  /** Call the varSol function*/
  static double GetVarSol(const int day, const int month)
  {
    /* System generated locals */
    double d__1;

    /* Local variables */
    int    j;
    double om;

    /* calculation of the variability of the solar constant during the year. */
    /* day is the number of the day in the month */
    if (month <= 2)
      j = (month - 1) * 31 + day;
    else if (month > 8)
      j = (month - 1) * 31 - (month - 2) / 2 - 2 + day;
    else
      j = (month - 1) * 31 - (month - 1) / 2 - 2 + day;

    om = (double)(j - 4) * .9856 * CONST_PI_180;
    /* Computing 2nd power */
    d__1 = 1. - std::cos(om) * .01673;
    return 1. / (d__1 * d__1);
  }
};
} // end namespace otb
#endif
