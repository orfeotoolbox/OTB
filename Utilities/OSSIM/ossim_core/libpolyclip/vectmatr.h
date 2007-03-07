/*

      poly_Boolean: a polygon clip library

      Copyright (C) 1997  Alexey Nikitin, Michael Leonov

      leonov@propro.iis.nsk.su



      This library is free software; you can redistribute it and or

      modify it under the terms of the GNU Library General Public

      License as published by the Free Software Foundation; either

      version 2 of the License, or (at your option) any later version.



      This library is distributed in the hope that it will be useful,

      but WITHOUT ANY WARRANTY; without even the implied warranty of

      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU

      Library General Public License for more details.



      You should have received a copy of the GNU Library General Public

      License along with this library; if not, write to the Free

      Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.



      vectmatr.h

      (C) 1997 Alexey Nikitin, Michael Leonov

      (C) 1993,1997 Klamer Schutte



*/



#ifndef VECTOR_MATRIX_H

#define VECTOR_MATRIX_H





#ifdef __cplusplus

extern "C" {

#endif



typedef int		BOOL;



#ifndef FALSE

enum {

	FALSE = 0,

	TRUE  = 1

};

#endif



#ifndef min

double min(double x, double y);

#endif



#ifndef max

double max(double x, double y);

#endif





typedef double vertex[3];  /* floating point representation of

                             coordinates */

typedef vertex Vector;



#define VertexEqu(a,b) (memcmp((a),(b),sizeof(Vector))==0)

#define VertexCpy(a,b) memcpy((a),(b),sizeof(Vector))





extern Vector vect_zero;



/*********************************************************************/

/*            F l o a t   V e c t o r   S t u f f                    */

/*********************************************************************/



int    vect_equal ( Vector v1, Vector v2 );

void   vect_copy ( Vector des, Vector sou );

void   vect_init ( Vector v, double x, double y, double z);

void   vect_sub( Vector res, Vector v2, Vector v3 );



void   vect_min( Vector res, Vector v2, Vector v3 );

void   vect_max( Vector res, Vector v2, Vector v3 );



#define TNY 1e-6



#ifndef EPSILON

#define EPSILON TNY

#endif



double vect_dist2(Vector v1, Vector v2);

double vect_det2(Vector v1, Vector v2);

double vect_len2(Vector v1);



int vect_inters2(Vector A, Vector B, Vector C, Vector D, Vector S1, Vector S2);



/****************************************************************************

* Calcs the intersection point of AB and BC. Returns if they hit each other 

* and the position of the hit(s) (S1 (and S2))

*****************************************************************************/



#ifdef __cplusplus

}

#endif



#endif /* VECTOR_MATRIX_H */

