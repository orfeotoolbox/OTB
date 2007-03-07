/*

      poly_Boolean: a polygon clip library

      Copyright (C) 1997  Alexey Nikitin, Michael Leonov

      leonov@propro.iis.nsk.su



  based on:

      graphadd.cc: a polygon clip library

      Copyright (C) 1993  Klamer Schutte



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



      vectmatr.c

      (C) 1997 Alexey Nikitin, Michael Leonov

      (C) 1993,1997 Klamer Schutte



      all cases where original (Klamer Schutte) code is present

      are marked

*/



#include <stdlib.h>

#include <math.h>

#include <assert.h>



#include "vectmatr.h"



#ifndef min

double

min(double x, double y)

{

    if (x > y)

        return y;

    return x;

}

#endif



#ifndef max

double

max(double x, double y)

{

    if (y > x)

        return y;

    return x;

} 

#endif



Vector vect_zero = { (double)0, (double)0, (double)0 };



/*********************************************************************/

/*            F l o a t   V e c t o r   S t u f f                    */

/*********************************************************************/



void vect_init ( Vector v, double x, double y, double z )

{

    v[0] = (double)x;

    v[1] = (double)y;

    v[2] = (double)z;

} /* vect_init */



#define Vcopy(a,b) {(a)[0]=(b)[0];(a)[1]=(b)[1];(a)[2]=(b)[2];}

#define Vzero(a)   ((a)[0] == 0. && (a)[1] == 0. && (a)[2] == 0.)



#define Vsub(a,b,c) {(a)[0]=(b)[0]-(c)[0];(a)[1]=(b)[1]-(c)[1];(a)[2]=(b)[2]-(c)[2];}



void vect_copy ( Vector des, Vector sou ) { Vcopy( des, sou ) } /* vect_copy */

int vect_equal ( Vector v1, Vector v2 )

{

    return ( v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2] );

} /* vect_equal */





void vect_sub ( Vector res, Vector v1, Vector v2 ) { Vsub( res, v1, v2 ) } /* vect_sub */



void vect_min ( Vector v1, Vector v2, Vector v3 )

{

    v1[0] = (v2[0] < v3[0]) ? v2[0] : v3[0];

    v1[1] = (v2[1] < v3[1]) ? v2[1] : v3[1];

    v1[2] = (v2[2] < v3[2]) ? v2[2] : v3[2];

} /* vect_min */



void vect_max ( Vector v1, Vector v2, Vector v3 )

{

    v1[0] = (v2[0] > v3[0]) ? v2[0] : v3[0];

    v1[1] = (v2[1] > v3[1]) ? v2[1] : v3[1];

    v1[2] = (v2[2] > v3[2]) ? v2[2] : v3[2];

} /* vect_max */



/* ///////////////////////////////////////////////////////////////////////////// * /

/ *  2-Dimentional stuff

/ * ///////////////////////////////////////////////////////////////////////////// */





#define Vsub2(r,a,b)	{(r)[0] = (a)[0] - (b)[0]; (r)[1] = (a)[1] - (b)[1];}

#define Vadd2(r,a,b)	{(r)[0] = (a)[0] + (b)[0]; (r)[1] = (a)[1] + (b)[1];}

#define Vsca2(r,a,s)	{(r)[0] = (a)[0] * (s); (r)[1] = (a)[1] * (s);}

#define Vcpy2(r,a)		{(r)[0] = (a)[0]; (r)[1] = (a)[1];}

#define Vequ2(a,b)		((a)[0] == (b)[0] && (a)[1] == (b)[1])



#define Vadds(r,a,b,s)	{(r)[0] = ((a)[0] + (b)[0]) * (s); (r)[1] = ((a)[1] + (b)[1]) * (s);}



#define Vswp2(a,b) { double t; \
t = (a)[0], (a)[0] = (b)[0], (b)[0] = t; \
t = (a)[1], (a)[1] = (b)[1], (b)[1] = t; \
}



double vect_len2(Vector v)

{

    return sqrt(v[0] * v[0] + v[1] * v[1]);

}



double vect_dist2(Vector v1, Vector v2)

{

	double dx = v1[0] - v2[0];

	double dy = v1[1] - v2[1];



    return sqrt(dx * dx + dy * dy);

}



double vect_det2(Vector v1, Vector v2)

{

    return ((v1[0] * v2[1]) - (v2[0] * v1[1]));

}



static

double vect_m_dist(Vector v1, Vector v2)

{

	double dx = v1[0] - v2[0];

	double dy = v1[1] - v2[1];

    double dd = sqrt(dx * dx + dy * dy);

	

    if (dx > 0)  

        return +dd;

    if (dx < 0)

        return -dd;

    if (dy > 0)

        return +dd;

    return -dd;

} /* vect_m_dist */



/*

RecursiveIsect

 (C) 1993 Klamer Schutte

 (C) 1997 Michael Leonov, Alexey Nikitin

*/





static 

void RecursiveIsect(Vector p1, Vector p2, Vector q1, Vector q2, Vector ret)

{

    /* Find intersection point of p1-p2 and q1-q2 by iteratively taking

       the middle of p1-p2 */



    Vector q, s1, s2, m, tmp;

    double inp;



    Vsub2(q, q2, q1);



    if (vect_len2(q) < vect_dist2(p2, p1))

    {

        RecursiveIsect(q1, q2, p1, p2, ret);

        return;

    }



    Vsub2(s1, p1, q1);

    Vsub2(s2, p2, q1);

    Vadds(m, s1, s2, 0.5f);



    inp = vect_det2(q, s1);



    if (inp > 0)

    {

        Vcpy2(tmp, s1);

        Vcpy2(s1, s2);

        Vcpy2(s2, tmp);

    }



    while (!Vequ2(m, s1) && !Vequ2(m, s2))

    {

/*         assert(vect_det2(q, s1) <= 0); */

/*         assert(vect_det2(q, s2) >= 0); */

/*          if(vect_det2(q, s1) > 0) */
/*          { */
/*             break; */
/*          } */

/*          if(vect_det2(q, s2) > 0) */
/*          { */
/*             break; */
/*          } */



        if (vect_det2(q, m) <= 0)

            Vcpy2(s1, m)

        else

            Vcpy2(s2, m)



        Vadds(m, s1, s2, 0.5f);

    }

/*    assert(vect_len2(m) <= vect_dist2(q2, q1)); */



    Vadd2(ret, m, q1);

} /* RecursiveIsect */



/*

vect_inters2

 (C) 1993 Klamer Schutte

 (C) 1997 Michael Leonov, Alexey Nikitin

*/



int vect_inters2(Vector p1, Vector p2, Vector q1, Vector q2, 

				 Vector S1, Vector S2)

{

    double rpx, rpy, rqx, rqy, t, deel;

    Vector p, pq1, pq2, q, qp1, qp2, s, qs;

	int c1, c2, c1s, c2s;



    if (max(p1[0], p2[0]) < min(q1[0], q2[0]) ||

		max(q1[0], q2[0]) < min(p1[0], p2[0]))

        return 0;



    S1[2] = S2[2] = 0.0F;

    rpx = p2[0] - p1[0];

    rpy = p2[1] - p1[1];

    rqx = q2[0] - q1[0];

    rqy = q2[1] - q1[1];



    deel = rpx * rqy - rpy * rqx; /* vect_det(rp,rq); */



	/* every value below EPSILON is considered as being 0. 

	 * Hence, we do not introduce numerical inaccuracies

	 */



    if (fabs(deel) < EPSILON) /* parallel */

    {

		double inpr, dc1, dc2, d1, d2, h;	/* Check too see whether p1-p2 and q1-q2 are on the same line */

	    Vector hp1, hq1, hp2, hq2, q1p1, q1q2;



        Vsub2(q1p1, q1, p1);

        Vsub2(q1q2, q1, q2);



        inpr = vect_det2(q1p1, q1q2);



        /* If this product is not zero then p1 is not on q1-q2! */

        if (fabs(inpr) > EPSILON)

            return 0;

        dc1 = 0;  /* m_len(p1 - p1) */



        dc2 = vect_m_dist(p1, p2);

        d1 = vect_m_dist(p1, q1);

        d2 = vect_m_dist(p1, q2);



/* Sorting the independent points from small to large: */

        Vcpy2(hp1, p1);

        Vcpy2(hp2, p2);

        Vcpy2(hq1, q1);

        Vcpy2(hq2, q2);

        if (dc1 > dc2)

        { /* hv and h are used as help-variable. */

            Vswp2(hp1, hp2);

            h = dc1, dc1 = dc2, dc2 = h;

        }

        if (d1 > d2)

        {

            Vswp2(hq1, hq2);

            h = d1, d1 = d2, d2 = h;

        }



/* Now the line-pieces are compared: */



        if (dc1 < d1)

        {

            if (dc2 < d1)

                return 0;

            if (dc2 < d2)	{ Vcpy2(S1, hp2); Vcpy2(S2, hq1); }

            else			{ Vcpy2(S1, hq1); Vcpy2(S2, hq2); };

        }

        else

        {

            if (dc1 > d2)

                return 0;

            if (dc2 < d2)	{ Vcpy2(S1, hp1); Vcpy2(S2, hp2); }

            else			{ Vcpy2(S1, hp1); Vcpy2(S2, hq2); };

        }

        return (Vequ2(S1, S2) ? 1 : 2);

    }

    else

    {  /* not parallel */

		/*

		 * We have the lines:

		 * l1: p1 + s(p2 - p1)

		 * l2: q1 + t(q2 - q1)

		 * And we want to know the intersection point.

		 * Calculate t:

		 * p1 + s(p2-p1) = q1 + t(q2-q1)

		 * which is similar to the two equations:

		 * p1x + s * rpx = q1x + t * rqx

		 * p1y + s * rpy = q1y + t * rqy

		 * Multiplying these by rpy resp. rpx gives:

		 * rpy * p1x + s * rpx * rpy = rpy * q1x + t * rpy * rqx

		 * rpx * p1y + s * rpx * rpy = rpx * q1y + t * rpx * rqy

		 * Subtracting these gives:

		 * rpy * p1x - rpx * p1y = rpy * q1x - rpx * q1y + t * ( rpy * rqx - rpx * rqy )

		 * So t can be isolated:

		 * t = (rpy * ( p1x - q1x ) + rpx * ( - p1y + q1y )) / ( rpy * rqx - rpx * rqy )

		 * and deel = rpx * rqy - rpy * rqx

		 */

		double inp1, inp2, inp3, inp4, inp1s, inp2s, inp3s, inp4s;





        if (Vequ2(q1, p1) || Vequ2(q1, p2)) Vcpy2(S1, q1) else 

		if (Vequ2(q2, p1) || Vequ2(q2, p2)) Vcpy2(S1, q2) else

        {

            t = -(rpy * (-q1[0] + p1[0]) + rpx * (q1[1] - p1[1])) / deel;

            S1[0] = (double) (q1[0] + t * rqx);

            S1[1] = (double) (q1[1] + t * rqy);

        }

/*

 * The intersection point is valid if it is

 * 1) on q1-q2 --> t >= 0 && t <= 1

 * 2) on p1-p2 --> p1 must be on the other side of q1-q2 as p2

 *    This is so if the difference of the x coordinate of p1-s1 has the

 *    opposite sign as the x coordinate of p2-s2. So the multiplication of

 *    these two must be negative. This might fail if p1-p2 is a vertical line;

 *    this can be solved by adding the same product for the y coordinates

 */

        Vsub2(p, p2, p1);

        Vsub2(pq1, q1, p1);

        Vsub2(pq2, q2, p1);

        inp1 = vect_det2(p, pq1);

        inp2 = vect_det2(p, pq2);

        c1 = inp1 * inp2 <= 0;



        Vsub2(q, q2, q1);

        Vsub2(qp1, p1, q1);

        Vsub2(qp2, p2, q1);



        inp3 = vect_det2(q, qp1);

        inp4 = vect_det2(q, qp2);

        c2 = inp3 * inp4 <= 0;



        {

			double l_q, l_p;

            /* Say that *S1 equals one of the points if the relative

               distance is smaller than EPSILON */

            l_q = vect_dist2(q1, q2);

            l_p = vect_dist2(p1, p2);

            if (EPSILON > vect_dist2(S1, p2) / l_p)

            {

                *S1 = *p2;

                c2 = 2;

            }

            else if (EPSILON > vect_dist2(S1, p1) / l_p)

            {

                *S1 = *p1;

                c2 = 2;

            }

            else

            {

                if (EPSILON > vect_dist2(S1, q2) / l_q)

                {

                    *S1 = *q2;

                    c1 = 2;

                }

                else if (EPSILON > vect_dist2(S1, q1) / l_q)

                {

                    *S1 = *q1;

                    c1 = 2;

                }

            }

        }

        Vsub2(s, S1, p1);

        inp1s = vect_det2(s, pq1);

        inp2s = vect_det2(s, pq2);

        c1s = Vequ2(S1, p1) ? -1 : inp1s * inp2s <= 0;



        Vsub2(qs, S1, q1);

        inp3s = vect_det2(qs, qp1);

        inp4s = vect_det2(qs, qp2);

        c2s = Vequ2(S1, q1) ? -1 : inp3s * inp4s <= 0;

		{

			int failed = 0; /* Roundig errors might make the statements below untrue */



			if (!((c1 == 0) || (c2 == 0) || (c1s == (c1 != 0)) || (c1s == -1)))

				failed = 1;

			else if (!((c1 == 0) || (c2 == 0) || (c2s == (c2 != 0)) || (c2s == -1)))

				failed = 2;

			else if (c1 && c2 && (vect_dist2(S1, q1) > vect_dist2(q2, q1)))

				failed = 3;

			else if (c1 && c2 && (vect_dist2(S1, q2) > vect_dist2(q2, q1)))

				failed = 4;

			else if (c1 && c2 && (vect_dist2(S1, p1) > vect_dist2(p2, p1)))

				failed = 5;

			else if (c1 && c2 && (vect_dist2(S1, p2) > vect_dist2(p2, p1)))

				failed = 6;

			if ((failed >= 3) && (c1 == 2 || c2 == 2))

			{

				failed = -1;

				c1 = c2 = 0;

			}

			if (failed > 0)

				RecursiveIsect(p1, p2, q1, q2, S1);

			return (c1 && c2);

		}

    }

} /* vect_inters2 */



/* end of VECTMATR.C */

