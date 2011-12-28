//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef Arithmetic_h
#define Arithmetic_h

#include <cmath>
#include <cstdlib>

namespace ossimplugins
{


/*-------------------------------------------------------------------------*/
/*          "closest integer" function, returns long                      */
/*-------------------------------------------------------------------------*/

   long inint (double x)
   {
      return (x >= 0.0)  ?  (long)(x + 0.5)  :  (long)(x-0.5) ;
   }

/*-------------------------------------------------------------------------*/
/*          "closest integer" function, returns double                    */
/*-------------------------------------------------------------------------*/

   double fnint (double x)
   {
      return (x >= 0.0)  ?  (double)((long)(x + 0.5))  :  (double)((long)(x - 0.5));
   }

   
}  // End: namespace ossimplugins

#endif
