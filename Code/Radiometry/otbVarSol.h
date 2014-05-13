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
#ifndef __otbVarSol_h
#define __otbVarSol_h

#include "otbMath.h"

namespace otb
{

/** \class VarSol
   * \brief VarSol operations.
   *
   * Call VarSol main function
   *
   * [1] Vermote, E., Tanré, D., Deuze, J., Herman, M., Morcette, J., 1997. 
   * Second simulation of the satellite signal in the solar spectrum, 6S: An overview. 
   * IEEE Transactions on Geoscience and Remote Sensing 35
   * \ingroup Radiometry
   *
 */
  class ITK_EXPORT VarSol
  {
  public:
    
    /** Call the varSol function*/
    static double GetVarSol(const int day, const int month)
    {
      /* System generated locals */
      double d__1;
      
      /* Builtin functions */
      //double acos(double), cos(double);
      
      /* Local variables */
      int j;
      double om;
      
      /* calculation of the variability of the solar constant during the year. */
      /* day is the number of the day in the month */
      if (month <= 2) 
        j = (month - 1) * 31 + day;
      else if (month > 8) 
        j = (month - 1) * 31 - (month - 2) / 2 - 2 + day;
      else
        j = (month - 1) * 31 - (month - 1) / 2 - 2 + day;
      
      om = (double) (j - 4) * .9856 * CONST_PI_180;
      /* Computing 2nd power */
      d__1 = 1. - vcl_cos(om) * .01673;
      return 1. / (d__1 * d__1);
    }
  };
} //end namespace otb
#endif

