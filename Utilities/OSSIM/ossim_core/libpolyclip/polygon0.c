/*

      poly_Boolean: a polygon clip library

      Copyright (C) 1997  Alexey Nikitin, Michael Leonov

      leonov@propro.ru



      This library is free software; you can redistribute it and/or

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



      polygon0.c

      (C) 1997 Alexey Nikitin, Michael Leonov

*/



#include <assert.h>

#include <math.h>

#include <string.h>

#include <stdlib.h>
#include <string.h>


#include "vectmatr.h"

#include "polyarea.h"



#define SETBITS(n,mask,s) (((n)->Flags & ~(mask)) | ((s) & mask))

#define GETBITS(n, mask) ((n)->Flags & mask)



static

void cntrbox_adjust(PLINE * c, Vector p)

{

    c->xmin = min(c->xmin, p[0]);

    c->xmax = max(c->xmax, p[0]);

    c->ymin = min(c->ymin, p[1]);

    c->ymax = max(c->ymax, p[1]);

}



static

int cntrbox_pointin(PLINE * c, Vector p)

{

    return (p[0] >= c->xmin && p[1] >= c->ymin &&

            p[0] <= c->xmax && p[1] <= c->ymax);



}



static

int cntrbox_inside(PLINE * c1, PLINE * c2)

{

    assert(c1 != NULL && c2 != NULL);

    return ((c1->xmin >= c2->xmin) &&

            (c1->ymin >= c2->ymin) &&

            (c1->xmax <= c2->xmax) &&

            (c1->ymax <= c2->ymax));

}



static

int node_neighbours(VNODE * a, VNODE * b)

{

    return (a == b) || (a->next == b) || (b->next == a) ||

    (a->next == b->next);

}



VNODE *poly_CreateNode(Vector v)

{

    VNODE *res;



    if (v == NULL)

        return NULL;

    res = (VNODE *) malloc(sizeof(VNODE));

    if (res == NULL)

        return NULL;

    res->Flags = 0;

    res->t.link = NULL;

    vect_copy(res->point, v);

    res->prev = res->next = res;

    return res;

}



void poly_IniContour(PLINE * c)

{

    if (c == NULL)

        return;

    c->next = NULL;

    c->Flags = 0;

    c->Count = 0;

    vect_copy(c->head.point, vect_zero);

    c->head.next = c->head.prev = &c->head;

    c->xmin = c->ymin = +1.0e+30f;

    c->xmax = c->ymax = -1.0e+30f;

}



PLINE *poly_NewContour(Vector v)

{

    PLINE *res;



    res = (PLINE *)calloc(1, sizeof(PLINE));

    if (res == NULL)

        return NULL;



    poly_IniContour(res);



    if (v != NULL)

    {

        vect_copy(res->head.point, v);

        cntrbox_adjust(res, v);

    }

    

    return res;

}



void poly_ClrContour(PLINE * c)

{

    VNODE *cur;



    assert(c != NULL);

    while ( ( cur = c->head.next ) != &c->head )

    {

        poly_ExclVertex(cur);

        free(cur);

    }

    poly_IniContour(c);

}



void poly_DelContour(PLINE ** c)

{

    if (*c == NULL)

        return;

    poly_ClrContour(*c);

    free(*c), *c = NULL;

}



#define IsZero(a, b) (fabs((a) - (b)) < EPSILON)



void poly_PreContour(PLINE *C, BOOL optimize)

{

    double  area = 0;

    VNODE	*p, *c;



    assert(C != NULL);



	if (optimize)

	{

		for (c = (p = &C->head)->next; c != &C->head; c = (p = c)->next)

		{

			if (IsZero(p->point[0], c->point[0]) &&

				IsZero(p->point[1], c->point[1]))

			{

				poly_ExclVertex(c);

				free(c);

				c = p;

			}

		}

	}

	C->Count = 0;

    C->xmin = C->xmax = C->head.point[0];

    C->ymin = C->ymax = C->head.point[1];



    p = (c = &C->head)->prev;

	if (c != p)

	{

		do {

			/* calculate area for orientation */

			area += (p->point[0] - c->point[0]) * (p->point[1] + c->point[1]);

			cntrbox_adjust(C, c->point);

			C->Count++;

		} while ((c = (p = c)->next) != &C->head);

	}

    if (C->Count > 2)

        C->Flags = SETBITS(c, PLF_ORIENT, area < 0 ? PLF_INV : PLF_DIR);

} /* poly_PreContour */



void poly_InvContour(PLINE * c)

{

    VNODE *cur, *next;



    assert(c != NULL);

    cur = &c->head;

    do

    {

        next = cur->next;

        cur->next = cur->prev;

        cur->prev = next;

    } while ((cur = next) != &c->head);

    c->Flags ^= PLF_ORIENT;

}



void poly_InclVertex(VNODE * after, VNODE * node)

{

    assert(after != NULL);

    assert(node != NULL);



    node->prev = after;

    node->next = after->next;

    after->next = after->next->prev = node;

}



void poly_ExclVertex(VNODE * node)

{

    assert (node != NULL);



    node->prev->next = node->next;

    node->next->prev = node->prev;

}



BOOL poly_CopyContour(PLINE ** dst, PLINE * src)

{

    VNODE *cur, *newnode;



    assert(src != NULL);

    *dst = NULL;

    *dst = poly_NewContour(src->head.point);

    if (*dst == NULL)

        return FALSE;



    (*dst)->Count = src->Count;

    (*dst)->Flags = src->Flags;

    (*dst)->xmin = src->xmin, (*dst)->xmax = src->xmax;

    (*dst)->ymin = src->ymin, (*dst)->ymax = src->ymax;



    for (cur = src->head.next; cur != &src->head; cur = cur->next)

    {

        if ( (newnode = poly_CreateNode(cur->point) ) == NULL)

            return FALSE;

        newnode->Flags = cur->Flags;

        poly_InclVertex((*dst)->head.prev, newnode);

    }

    return TRUE;

}



/**********************************************************************/

/* polygon routines */



BOOL poly_Copy0(POLYAREA ** dst, const POLYAREA * src)

{

    *dst = NULL;

    if (src != NULL)

        *dst = (POLYAREA*)calloc(1, sizeof(POLYAREA));

    if (*dst == NULL)

        return FALSE;



    return poly_Copy1(*dst, src);

}



BOOL poly_Copy1(POLYAREA * dst, const POLYAREA * src)

{

    PLINE *cur, **last = &dst->contours;



    *last = NULL;

    dst->f = dst->b = dst;



    for (cur = src->contours; cur != NULL; cur = cur->next)

    {

        if (!poly_CopyContour(last, cur)) 

            return FALSE;

        last = &(*last)->next;

    }

    return TRUE;

}



void poly_M_Incl(POLYAREA **list, POLYAREA *a)

{

    if (*list == NULL)

        a->f = a->b = a, *list = a;

    else

    {

        a->f = *list;

        a->b = (*list)->b;

        (*list)->b = (*list)->b->f = a;

    }

}



BOOL poly_M_Copy0(POLYAREA ** dst, const POLYAREA * srcfst)

{

    const POLYAREA *src = srcfst;

    POLYAREA *di;



    *dst = NULL;

    if (src == NULL)

        return FALSE;

    do {

        if ((di = poly_Create()) == NULL ||

            !poly_Copy1(di, src))

            return FALSE;

        poly_M_Incl(dst, di);

    } while ((src = src->f) != srcfst);

    return TRUE;

}



BOOL poly_InclContour(POLYAREA * p, PLINE * c)

{

    PLINE *tmp;



    if ((c == NULL) || (p == NULL))

        return FALSE;

    if (GETBITS(c, PLF_ORIENT) == PLF_DIR)

    {

        if (p->contours != NULL)

            return FALSE;

        p->contours = c;

    }

    else

    {

        if (p->contours == NULL)

            return FALSE;

        for (tmp = p->contours; tmp->next != NULL; tmp = tmp->next);

        tmp->next = c;

        c->next = NULL;

    }

    return TRUE;

}



BOOL poly_M_InclContour(POLYAREA ** p, PLINE * c)

{

    POLYAREA *a, *t;



    if (c == NULL)

        return FALSE;

    if (GETBITS(c, PLF_ORIENT) == PLF_DIR)

    {

        t = poly_Create();

        if (*p != NULL)

        {

            a = (*p)->b;

            (((t->f = a->f)->b = t)->b = a)->f = t;

        }

        else

            *p = t;

        return poly_InclContour(t, c);

    }

    else

    {

        if (*p == NULL)

            return FALSE;

        a = *p;

        t = NULL;

        do {

            /* find the smallest container for the hole */

            if (poly_ContourInContour(a->contours, c) &&

               (t == NULL ||

                poly_ContourInContour(t->contours, a->contours)))

                t = a;

        } while ((a = a->f) != *p);

        if (t == NULL)

            return FALSE;

        else

            return poly_InclContour(t, c);

    }

}



BOOL poly_ExclContour(POLYAREA * p, PLINE * c)

{

    PLINE *tmp;



    if ((c == NULL) || (p == NULL) || (p->contours == NULL))

        return FALSE;

    if (GETBITS(c, PLF_ORIENT) == PLF_DIR)

    {

        if (c != p->contours)

            return FALSE;

        assert(c->next == NULL);

        p->contours = NULL;

    }

    else

    {

        for (tmp = p->contours; (tmp->next != c) && (tmp->next != NULL);

             tmp = tmp->next) {}

        if (tmp->next == NULL)

            return FALSE;

        tmp->next = c->next;

    }

    return TRUE;

}





BOOL poly_M_ExclContour(POLYAREA * p, PLINE * c)

{

    POLYAREA *a = p;



    if (p == NULL || c == NULL)

        return FALSE;

    do {

        if (poly_ExclContour(a, c))

            return TRUE;

    } while ((a = a->f) != p);

    return FALSE;

}



int poly_InsideContour(PLINE * c, Vector p)

{

    int f = 0;

    VNODE *cur;



    if (!cntrbox_pointin(c, p))

        return FALSE;



    cur = &c->head;

    do

    {

        if ((

             ((cur->point[1] <= p[1]) && (p[1] < cur->prev->point[1])) ||

             ((cur->prev->point[1] <= p[1]) && (p[1] < cur->point[1]))

             ) && (p[0] <

           (cur->prev->point[0] - cur->point[0]) * (p[1] - cur->point[1])

                 / (cur->prev->point[1] - cur->point[1]) + cur->point[0])

            )

            f = !f;

    } while ((cur = cur->next) != &c->head);

    return f;

}





BOOL poly_CheckInside(POLYAREA * p, Vector v0)

{

    PLINE *cur;



    if ((p == NULL) || (v0 == NULL) || (p->contours == NULL))

        return FALSE;

    cur = p->contours;

    if (poly_InsideContour(cur, v0))

    {

        for (cur = cur->next; cur != NULL; cur = cur->next)

            if (poly_InsideContour(cur, v0))

                return FALSE;

        return TRUE;

    }

    return FALSE;

}



BOOL poly_M_CheckInside(POLYAREA * p, Vector v0)

{

    POLYAREA *cur;



    if ((p == NULL) || (v0 == NULL))

        return FALSE;

    cur = p;

    do {

        if (poly_CheckInside(cur, v0))

            return TRUE;

    } while ((cur = cur->f) != p);

    return FALSE;

}



int poly_ContourInContour(PLINE * poly, PLINE * inner)

{

    assert(poly != NULL);

    assert(inner != NULL);

    if (!cntrbox_inside(inner, poly))

        return FALSE;

    return poly_InsideContour(poly, inner->head.point);

}



void poly_Init(POLYAREA  *p)

{

    p->f = p->b = p;

    p->contours = NULL;

}



POLYAREA *poly_Create(void)

{

    POLYAREA *res;



    if ((res = (POLYAREA*)malloc(sizeof(POLYAREA))) != NULL)

		poly_Init(res);

    return res;

}



void poly_Clear(POLYAREA *P)

{

    PLINE *p;



    assert(P != NULL);

    while ((p = P->contours) != NULL)

    {

        P->contours = p->next;

        poly_DelContour(&p);

    }

}



void poly_Free(POLYAREA ** p)

{

    POLYAREA *cur;



    if (*p == NULL)

        return;

    for (cur = (*p)->f; cur != *p; cur = (*p)->f)

    {

        poly_Clear(cur);

        cur->f->b = cur->b;

        cur->b->f = cur->f;

        free(cur);

    }

    poly_Clear(cur);

    free(*p), *p = NULL;

}



static

BOOL inside_sector(VNODE * pn, Vector p2)

{

    Vector cdir, ndir, pdir;

    int p_c, n_c, p_n;



    assert(pn != NULL);

    vect_sub(cdir, p2, pn->point);

    vect_sub(pdir, pn->point, pn->prev->point);

    vect_sub(ndir, pn->next->point, pn->point);



    p_c = vect_det2(pdir, cdir) >= 0;

    n_c = vect_det2(ndir, cdir) >= 0;

    p_n = vect_det2(pdir, ndir) >= 0;



    if ((p_n && p_c && n_c) || ((!p_n) && (p_c || n_c)))

        return TRUE;

    else

        return FALSE;

} /* inside_sector */



/* returns TRUE if bad contour */

BOOL poly_ChkContour(PLINE * a)

{

    VNODE *a1, *a2, *hit1, *hit2;

    Vector i1, i2;

    int    icnt;



    assert(a != NULL);

    a1 = &a->head;

    do

    {

        a2 = a1;

        do

        {

            if (!node_neighbours(a1, a2) &&

                (icnt = vect_inters2(a1->point, a1->next->point,

                              a2->point, a2->next->point, i1, i2)) > 0)

            {

                if (icnt > 1)

                    return TRUE;



                if (vect_dist2(i1, a1->point) < EPSILON)

                    hit1 = a1;

                else if (vect_dist2(i1, a1->next->point) < EPSILON)

                    hit1 = a1->next;

                else

                    return TRUE;



                if (vect_dist2(i1, a2->point) < EPSILON)

                    hit2 = a2;

                else if (vect_dist2(i1, a2->next->point) < EPSILON)

                    hit2 = a2->next;

                else

                    return TRUE;



                /* now check if they are inside each other */

                if (inside_sector(hit1, hit2->prev->point) ||

                    inside_sector(hit1, hit2->next->point) ||

                    inside_sector(hit2, hit1->prev->point) ||

                    inside_sector(hit2, hit1->next->point))

                    return TRUE;

            }

        }

        while ((a2 = a2->next) != &a->head);

    }

    while ((a1 = a1->next) != &a->head);

    return FALSE;

}





BOOL poly_Valid(POLYAREA * p)

{

    PLINE *c;



    if ((p == NULL) || (p->contours == NULL))

        return FALSE;



    if (GETBITS(p->contours, PLF_ORIENT) == PLF_INV ||

        poly_ChkContour(p->contours))

        return FALSE;

    for (c = p->contours->next; c != NULL; c = c->next)

    {

        if (GETBITS(c, PLF_ORIENT) == PLF_DIR ||

            poly_ChkContour(c) ||

            !poly_ContourInContour(p->contours, c) )

            return FALSE;

    }

    return TRUE;

}



/******************* POLYMAP ************************/



BOOL poly_AddMap(POLYMAP **m, POLYAREA *area, char *label)

{

    POLYMAP *n;

    if (area == NULL) return 0;

    if ((n = (POLYMAP*)calloc(1, sizeof(POLYMAP))) == NULL)

        return 1;

    n->area = area;

    n->label = strdup(label);

    n->next = *m, *m = n;

    return 0;

} /* poly_AddMap */



void poly_DelMap(POLYMAP **m)

{

    assert(m != NULL);

    while (*m != NULL)

    {

        poly_Free(&(*m)->area);

        free((*m)->label);

        *m = (*m)->next;

    }

} /* poly_DelMap */



