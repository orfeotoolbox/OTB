#ifndef Arithmetic_h
#define Arithmetic_h

#include <math.h>
#include <stdlib.h>

/*-------------------------------------------------------------------------*/
/*          fonction "plus proche entier" renvoyant long                   */
/*-------------------------------------------------------------------------*/

long inint (double x)
{
  return (x >= 0.0)  ?  (long)(x + 0.5)  :  (long)(x-0.5) ; 
}

/*-------------------------------------------------------------------------*/
/*          fonction "plus proche entier" renvoyant double                 */
/*-------------------------------------------------------------------------*/

double fnint (double x)
{
  return (x >= 0.0)  ?  (double)((long)(x + 0.5))  :  (double)((long)(x - 0.5));
}

/*-------------------------------------------------------------------------*/
/*          fonction Plus Grand Commun Diviseur                            */
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
/*          fonction Plus Petit Commun Multiple                            */
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

#endif