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

/*-------------------------------------------------------------------------*/
/*          Greatest Common Divisor                                    */
/*-------------------------------------------------------------------------*/

   long pgcd ( long n , long p )
   {
      long k ;
      long n1 , p1 ;
      long m0 , m1 , m ;

      n1 = labs(n) ;
      p1 = labs(p) ;
      if (n1 > p1)
      {
         m0 = n1 ;
         m1 = p1 ;
      }
      else
      {
         m0 = p1 ;
         m1 = n1 ;
      }
      if (m1 != 0)
      {
         while (m1 != 0)
         {
            m = m0 - (m0/m1) * m1 ;
            m0 = m1 ;
            m1 = m ;
         }
         k = m0 ;
      }
      else k = 1 ;

      return k ;
   }

/*-------------------------------------------------------------------------*/
/*          Least Common Multiple                                     */
/*-------------------------------------------------------------------------*/

   long ppcm ( long n , long p )
   {
      long k ;
      long n1 , p1 ;
      long m0 , m1 , m ;

      n1 = labs(n) ;
      p1 = labs(p) ;
      if (n1>p1)
      {
         m0 = n1 ;
         m1 = p1 ;
      }
      else
      {
         m0 = p1 ;
         m1 = n1 ;
      }
      if (m1 != 0)
      {
         m = m0 ;
         while ((m % m1) != 0) m += m0 ;
         k = m ;
      }
      else k = 0 ;

      return k ;
   }
   
}  // End: namespace ossimplugins

#endif

