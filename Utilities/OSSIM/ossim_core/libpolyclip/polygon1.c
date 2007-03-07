/*

      poly_Boolean: a polygon clip library

      Copyright (C) 1997  Alexey Nikitin, Michael Leonov

      leonov@propro.ru



  based on:

      nclip: a polygon clip library

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



      polygon1.c

      (C) 1997 Alexey Nikitin, Michael Leonov

      (C) 1993 Klamer Schutte



      all cases where original (Klamer Schutte) code is present

      are marked

*/



#include	<assert.h>

#include	<stdlib.h>

#include	<string.h>

#include	<setjmp.h>

#include	<math.h>



#include	"polyarea.h"

#include	"vectmatr.h"



#define ISECTED 0

#define SHARED  ISECTED

#define INSIDE  1

#define OUTSIDE 2

#define UNKNOWN 3



/* this means that current polygon is B and operation is A - B */

#define PBO_SUBSLAVE (-PBO_SUB)



#define SETBITS(n,mask,s) (((n)->Flags & ~(mask)) | ((s) & mask))

#define GETBITS(n, mask) ((n)->Flags & mask)

#define PLF_ESTATE(n)   GETBITS(n, PLF_STATUS)



#define error(code) longjmp(*(e), code)



#define MemGet(ptr, type) \
if (((ptr) = (type*)malloc(sizeof(type))) == NULL) \
   error(err_no_memory);



/***************************************************************/

/* routines for processing intersections */



/*

node_add

 (C) 1993 Klamer Schutte

 (C) 1997 Alexey Nikitin, Michael Leonov

*/

static

VNODE *node_add(VNODE * dest, Vector po, int *new_point)

{

    VNODE *chk = dest, *last, *p;

    double d;



    do

        if (vect_equal(po, chk->point))

            return chk;

    while ((chk = chk->next) != dest);



    d = vect_dist2(dest->point, po);

    for (p = (last = dest)->next; d > vect_dist2(dest->point, p->point);

         p = (last = p)->next) ;

    /* Go to next node, as next node is on same side as current node */

    p = poly_CreateNode(po);

    if ( p == NULL ) return NULL;

/*    *new_point++; */

    p->prev = last;

    p->next = last->next;

    p->t.link = NULL;

    p->Flags = UNKNOWN;

    return ( last->next = last->next->prev = p );

} /* node_add */



#define ISECT_BAD_PARAM (-1)

#define ISECT_NO_MEMORY (-2)



/*

node_add_point

 (C) 1993 Klamer Schutte

 (C) 1997 Alexey Nikitin, Michael Leonov

*/



static

int node_add_point(VNODE * a, VNODE * b, Vector p)

{

    int res = 0;



    VNODE *node_a = node_add(a, p, &res);

    VNODE *node_b = node_add(b, p, &res);



    if ( node_a == NULL || node_b == NULL )

        return ISECT_NO_MEMORY;

    if (node_a->t.link != 0 && node_a->t.link != node_b)

        return ISECT_BAD_PARAM;

    if (node_b->t.link != 0 && node_b->t.link != node_a)

        return ISECT_BAD_PARAM;



    node_a->t.link = node_b;

    node_b->t.link = node_a;

    return res;

} /* node_add_point */



/*



intersect

 (C) 1993 Klamer Schutte

 (C) 1997 Alexey Nikitin, Michael Leonov



 calculate all the intersections between a and b. Add the intersection points

 to a and b.

 returns total number of intersections found.*/

static

int intersect(PLINE * a, PLINE * b)

{

    VNODE *a_iter, *b_iter;

    int res = 0, res2 = 0, cnt, errc;

    Vector intersect1, intersect2;



    assert(a != NULL);

    assert(b != NULL);



    /* if their boxes do not isect return 0 intersections */

    if ( ( a->xmin > b->xmax ) || ( a->xmax < b->xmin ) ||

         ( a->ymin > b->ymax ) || ( a->ymax < b->ymin ) )

         return 0;



    a_iter = &a->head;

    do {

        b_iter = &b->head;

        do {

            cnt = vect_inters2(a_iter->point, a_iter->next->point,

                               b_iter->point, b_iter->next->point,

                               intersect1, intersect2);

            res += cnt;

            if (cnt > 0)

            {

                res2 += ( errc = node_add_point(a_iter, b_iter, intersect1) );

                if ( errc < 0 ) return errc;

                if (cnt >= 2)

                {

                    res2 += ( errc = node_add_point(a_iter, b_iter, intersect2) );

                    if ( errc < 0 ) return errc;

                }

            }

        } while ((b_iter = b_iter->next) != &b->head);

    } while ((a_iter = a_iter->next) != &a->head);



    if (res2 != 0)

    {

        if ( (errc = intersect(a, b)) < 0 )

            return errc;

        else

            return res + errc;

    }



    return res;

} /* intersect */



static

void M_POLYAREA_intersect(jmp_buf *e, POLYAREA * afst, POLYAREA * bfst)

{

    POLYAREA *a = afst, *b = bfst;

    PLINE *curcA, *curcB;

    int locisect;



    if ( a == NULL || b == NULL )

        error(err_bad_parm);

    do {

        do {

            for (curcA = a->contours; curcA != NULL; curcA = curcA->next)

                for (curcB = b->contours; curcB != NULL; curcB = curcB->next)

                    if ((locisect = intersect(curcA, curcB)) > 0)

                    {

                        curcA->Flags = SETBITS(curcA, PLF_STATUS, ISECTED);

                        curcB->Flags = SETBITS(curcB, PLF_STATUS, ISECTED);

                    }

                    else if ( locisect == ISECT_BAD_PARAM )

                        error(err_bad_parm);

                    else if ( locisect == ISECT_NO_MEMORY )

                        error(err_no_memory);

        } while ((a = a->f) != afst);

    } while ((b = b->f) != bfst);

} /* M_POLYAREA_intersect */



/*****************************************************************/

/* Routines for making labels */



/* cntr_in_M_POLYAREA

returns poly is inside outfst ? TRUE : FALSE */

static

int cntr_in_M_POLYAREA(PLINE * poly, POLYAREA * outfst)

{

    PLINE  *curc;

    POLYAREA *outer = outfst;



    assert(poly != NULL);

    assert(outer != NULL);



    do {

        if (poly_ContourInContour(outer->contours, poly))

        {

            for (curc = outer->contours->next; curc != NULL; curc = curc->next)

                if (poly_ContourInContour(curc, poly))

                    continue;

            return TRUE;

        }

    } while ((outer = outer->f) != outfst);

    return FALSE;

} /* cntr_in_M_POLYAREA */



/*

node_label

 (C) 1993 Klamer Schutte

 (C) 1997 Alexey Nikitin, Michael Leonov

*/



static

unsigned int node_label(Vector p2, VNODE * pn)

{

    Vector

    cdir, ndir, pdir;

    int p_c, n_c, p_n;



    assert(pn != NULL);

    vect_sub(cdir, p2, pn->point);

    vect_sub(pdir, pn->point, pn->prev->point);

    vect_sub(ndir, pn->next->point, pn->point);



    p_c = vect_det2(pdir, cdir) >= 0;

    n_c = vect_det2(ndir, cdir) >= 0;

    p_n = vect_det2(pdir, ndir) >= 0;



    if ((p_n && p_c && n_c) || ((!p_n) && (p_c || n_c)))

        return INSIDE;

    else

        return OUTSIDE;



} /* node_label */



/*

label_contour

 (C) 1993 Klamer Schutte

 (C) 1997 Alexey Nikitin, Michael Leonov

*/



static

void label_contour(PLINE * a)

{

    VNODE *cur = &a->head, *b1, *b2;



    do {

        if ( PLF_ESTATE(cur) != OUTSIDE && PLF_ESTATE(cur) != UNKNOWN )

            continue;



        b1 = cur->t.link;

        b2 = cur->next->t.link;



        if ( b1 != NULL && b2 != NULL &&

            /* Seems SHARED -- but check

               that b1 and b2 are also connected! */

           ( b1->next == b2 || b2->next == b1 ) )

        {

                cur->Flags = SETBITS(cur, PLF_STATUS, SHARED);

                continue;

        }

        /* else isected by different contours */

        if (b1 != NULL)

            cur->Flags = SETBITS(cur, PLF_STATUS, node_label(cur->next->point, b1));

        else if (b2 != NULL)

            cur->Flags = SETBITS(cur, PLF_STATUS, node_label(cur->point, b2));

    } while ((cur = cur->next) != &a->head);

} /* label_contour */



static

void cntr_label_POLYAREA(PLINE * poly, POLYAREA * ppl)

{

    assert(ppl != NULL && ppl->contours != NULL);

    if (GETBITS(poly, PLF_STATUS) == ISECTED)

        label_contour(poly);

    else if ( cntr_in_M_POLYAREA( poly, ppl ) )

        poly->Flags = SETBITS( poly, PLF_STATUS, INSIDE);

    else

        poly->Flags = SETBITS( poly, PLF_STATUS, OUTSIDE);

} /* cntr_label_POLYAREA */



static

void M_POLYAREA_label(POLYAREA * afst, POLYAREA * b)

{

    POLYAREA *a = afst;

    PLINE *curc;



    assert(a != NULL);

    do {

    for (curc = a->contours; curc != NULL; curc = curc->next)

        cntr_label_POLYAREA(curc, b);

    } while ((a = a->f) != afst);

}



/****************************************************************/



/* routines for temporary storing resulting contours */

static

void InsCntr(jmp_buf *e, PLINE * c, POLYAREA ** dst)

{

    POLYAREA *newp;



    if (*dst == NULL)

    {

        MemGet(*dst, POLYAREA);

        (*dst)->f = (*dst)->b = *dst;

        newp = *dst;

    }

    else

    {

        MemGet(newp, POLYAREA);

        newp->f = *dst;

        newp->b = (*dst)->b;

        newp->f->b = newp->b->f = newp;

    }

    newp->contours = c;

    c->next = NULL;

} /* InsCntr */



static

void PutContour(jmp_buf *e, PLINE * cntr, POLYAREA ** contours, PLINE ** holes)

{

    PLINE *cur;



    assert(cntr != NULL);

    cntr->next = NULL;

    if (GETBITS(cntr, PLF_ORIENT) == PLF_DIR)

        InsCntr(e, cntr, contours);



    /* put hole into temporary list */

    else if (*holes == NULL)

        *holes = cntr; /* let cntr be 1st hole in list */

    else

    {

        for (cur = *holes; cur->next != NULL; cur = cur->next);

        cur->next = cntr;

    }

} /* PutContour */



static

void InsertHoles(jmp_buf *e, POLYAREA * dest, PLINE **src)

{

    POLYAREA *curc, *container;

    PLINE *curh, *nexth;



    if (*src == NULL) return; /* empty hole list */

    if (dest == NULL) error(err_bad_parm); /* empty contour list */

    

    while ((curh = *src) != NULL)

    {

        *src = curh->next;



        container = NULL;

        curc = dest;

        do

        {

            /* find the smallest container for the hole */

            if (poly_ContourInContour(curc->contours, curh) &&

               (container == NULL ||

                poly_ContourInContour(container->contours, curc->contours)))

                container = curc;

        } while ((curc = curc->f) != dest);

        nexth = curh->next;

        curh->next = NULL;

        if (container == NULL)

        {

            /* bad input polygons were given */

            poly_DelContour(&curh);

            error(err_bad_parm);

        }

        else

            poly_InclContour(container, curh);

    }

} /* InsertHoles */





/****************************************************************/

/* routines for collecting result */



typedef enum

{

    FORW, BACKW

} DIRECTION;



/* Start Rule */

typedef int (*S_Rule) (VNODE *, DIRECTION *);



/* Jump Rule  */

typedef int (*J_Rule) (VNODE *, DIRECTION *);



static

int UniteS_Rule(VNODE * cur, DIRECTION * initdir)

{

    *initdir = FORW;

    return (PLF_ESTATE(cur) == OUTSIDE) ||

           (PLF_ESTATE(cur) == SHARED && cur->next->t.link == cur->t.link->next);

}



static

int IsectS_Rule(VNODE * cur, DIRECTION * initdir)

{

    *initdir = FORW;

    return (PLF_ESTATE(cur) == INSIDE) ||

           (PLF_ESTATE(cur) == SHARED && cur->next->t.link == cur->t.link->next);

}





static

int SubS_Rule(VNODE * cur, DIRECTION * initdir)

{

    *initdir = FORW;

    return (PLF_ESTATE(cur) == OUTSIDE) ||

           (PLF_ESTATE(cur) == SHARED && cur->next->t.link == cur->t.link->prev);

}



static

int SubSlaveS_Rule(VNODE * cur, DIRECTION * initdir)

{

    *initdir = BACKW;

    return (PLF_ESTATE(cur) == INSIDE);

}



static

int IsectJ_Rule(VNODE * cur, DIRECTION * cdir)

{

    assert(*cdir == FORW);

    return (PLF_ESTATE(cur->t.link) == INSIDE);

}



static

int UniteJ_Rule(VNODE * cur, DIRECTION * cdir)

{

    assert(*cdir == FORW);

    return (PLF_ESTATE(cur) != OUTSIDE && PLF_ESTATE(cur->t.link) == OUTSIDE);

}



static

int SubJ_Rule(VNODE * cur, DIRECTION * cdir)

{

    if (*cdir == FORW)

    {

        if (PLF_ESTATE(cur->t.link->prev) == INSIDE)

        {

            *cdir = BACKW;

            return TRUE;

        }

    }

    else if (PLF_ESTATE(cur->t.link) == OUTSIDE ||

             PLF_ESTATE(cur->t.link) == SHARED)

    {

        *cdir = FORW;

        return TRUE;

    }

    return FALSE;

}



static

int Gather(VNODE * start, PLINE ** result, J_Rule j_rule, DIRECTION initdir)

{

    VNODE *cur = start, *newn, *cn;

    DIRECTION dir = initdir;

    *result = poly_NewContour(start->point);

    if ( *result == NULL ) return err_no_memory;

    do

    {

        cn = (dir == FORW ? cur : cur->prev);

        cn->Flags = SETBITS( cn, PLF_MARK, PLF_MARK);



        /* for SHARED edge mark its neighbour, if the direction

        is the same */

        if (PLF_ESTATE(cn) == SHARED && cn->t.link->next == cn->next->t.link)

            cn->t.link->Flags |= PLF_MARK;



        cur = (dir == FORW ? cur->next : cur->prev);



        if (cur->t.link != NULL && j_rule(cur, &dir))

            cur = cur->t.link; /* jump */

        if (!vect_equal(cur->point, start->point))

        {

            if ( (newn = poly_CreateNode(cur->point)) == NULL)

                return err_no_memory;

            poly_InclVertex((*result)->head.prev, newn);

        }

    } while (!vect_equal(cur->point, start->point));

    return err_ok;

} /* Gather */



static

void Collect(jmp_buf *e, PLINE * a, POLYAREA ** contours, PLINE ** holes,

              S_Rule c_rule, J_Rule j_rule)

{

    VNODE *cur;

    PLINE *p = NULL;

    int errc = err_ok;

    DIRECTION dir;



    cur = &a->head;

    do

        if ((cur->Flags & PLF_MARK) == 0 && c_rule(cur, &dir))

        {

            p = NULL; /* start making contour */

            if ( (errc = Gather(dir == FORW ? cur : cur->next, &p, j_rule, dir)) != err_ok)

            {

                if (p != NULL) poly_DelContour(&p);

                error(errc);

            }

            poly_PreContour(p, TRUE);

            PutContour(e, p, contours, holes);

        }

    while ((cur = cur->next) != &a->head);

} /* Collect */



static

void cntr_Collect(jmp_buf *e, PLINE * A, POLYAREA ** contours, PLINE ** holes,

                   int action)

{

    PLINE *tmprev;



    if (GETBITS(A, PLF_STATUS) == ISECTED)

    {

        switch (action)

        {

        case PBO_UNITE:

            Collect(e, A, contours, holes, UniteS_Rule, UniteJ_Rule);

            break;

        case PBO_ISECT:

            Collect(e, A, contours, holes, IsectS_Rule, IsectJ_Rule);

            break;

        case PBO_XOR:

        case PBO_SUBSLAVE:

            Collect(e, A, contours, holes, SubSlaveS_Rule, SubJ_Rule);

            if (action == PBO_SUBSLAVE)

                break;

        case PBO_SUB:

            Collect(e, A, contours, holes, SubS_Rule, SubJ_Rule);

            break;

        };

    }

    else

    {

        switch (action)

        {

        case PBO_ISECT:

            if (GETBITS(A, PLF_STATUS)  == INSIDE)

            {

                if (!poly_CopyContour(&tmprev, A) )

                    error(err_no_memory);

                poly_PreContour(tmprev, TRUE);

                PutContour(e, tmprev, contours, holes);

            }

            break;

        case PBO_XOR:

        case PBO_SUBSLAVE:

            if (GETBITS(A, PLF_STATUS)  == INSIDE)

            {

                if ( !poly_CopyContour(&tmprev, A) )

                    error(err_no_memory);

                poly_PreContour(tmprev, TRUE);

                poly_InvContour(tmprev);

                PutContour(e, tmprev, contours, holes);

            }

            if (action == PBO_SUBSLAVE)

                break;

        case PBO_UNITE:

        case PBO_SUB:

            if (GETBITS(A, PLF_STATUS)  == OUTSIDE)

            {

                if ( !poly_CopyContour(&tmprev, A) )

                    error(err_no_memory);

                poly_PreContour(tmprev, TRUE);

                PutContour(e, tmprev, contours, holes);

            }

            break;

        }

    }

} /* cntr_Collect */





static

void M_POLYAREA_Collect(jmp_buf *e, POLYAREA * afst, POLYAREA ** contours, PLINE ** holes,

                      int action)

{

    POLYAREA *a = afst;

    PLINE *cur;



    assert(a != NULL);

    do {

        for (cur = a->contours; cur != NULL; cur = cur->next)

            cntr_Collect(e, cur, contours, holes, action);

    } while ((a = a->f) != afst);

}



/************************************************************************/

/* prepares polygon for algorithm, sets UNKNOWN labels and clears MARK bits */

static

void M_InitPolygon(POLYAREA * afst)

{

    PLINE *curc;

    VNODE *curn;

    POLYAREA *a = afst;



    assert(a != NULL);

    do {

        for (curc = a->contours; curc != NULL; curc = curc->next)

        {

            poly_PreContour(curc, TRUE);

            curc->Flags = SETBITS( curc, PLF_STATUS, UNKNOWN );

            curn = &curc->head;

            do {

                /*

                PLF_CYCLINK = 0, PLF_MARK = 0, PLF_STATUS = UNKNOWN

                */

                curn->Flags = SETBITS(curn, PLF_RESERVED, UNKNOWN);

                curn->t.link = NULL;

            } while ((curn = curn->next) != &curc->head);

        }

    } while ((a = a->f) != afst);

} /* M_InitPolygon */



/* the main clipping routine */

int poly_Boolean(const POLYAREA * a_org, const POLYAREA * b_org, POLYAREA ** res, int action)

{

    POLYAREA    *a = NULL, *b = NULL;

    PLINE       *p, *holes = NULL;

    jmp_buf     e;

    int         code;



    *res = NULL;



    if ((code = setjmp(e)) == 0)

    {

        if (!poly_M_Copy0(&a, a_org) || !poly_M_Copy0(&b, b_org))

            longjmp(e, err_no_memory);



        /* prepare polygons */

        M_InitPolygon(a);

        M_InitPolygon(b);



        /*M_POLYAREA_cyclink(a);

        M_POLYAREA_cyclink(b);*/



        M_POLYAREA_intersect(&e, a, b);



        M_POLYAREA_label(a, b);

        M_POLYAREA_label(b, a);



        M_POLYAREA_Collect(&e, a, res, &holes, action);



        if (action == PBO_SUB)

            action = PBO_SUBSLAVE;

        M_POLYAREA_Collect(&e, b, res, &holes, action);



        InsertHoles(&e, *res, &holes);

    }

    /* delete holes */

    while ((p = holes) != NULL)

    {

        holes = p->next;

        poly_DelContour(&p);

    }

    poly_Free(&a);

    poly_Free(&b);



    if (code)

        poly_Free(res);

    return code;

} /* poly_Boolean */



/********************* MAP OVERLAY **********************/



static

BOOL SubMap(POLYMAP *R, POLYMAP *B, POLYMAP **r)

{

    POLYMAP *ri;

    for (ri = R; ri != NULL; ri = ri->next)

    {

        POLYAREA *sub = NULL;

        POLYMAP  *bi;

        if (!poly_M_Copy0(&sub, ri->area))

            return 1;

        for (bi = B; bi != NULL; bi = bi->next)

        {

            POLYAREA *t = NULL;

            if (poly_Boolean(sub, bi->area, &t, PBO_SUB))

                return 1;

            poly_Free(&sub), sub = t;

        }

        if (poly_AddMap(r, sub, ri->label))

            return 1;

    }

    return 0;

} /* SubMap */



BOOL poly_Overlay(POLYMAP *R, POLYMAP *B,

                  POLYMAP **r, POLYMAP **b, POLYMAP **p)

{

    char    res[MAXLABEL + 1];

    POLYMAP *ri;

    if (R == NULL || B == NULL) return 1;



    /* first, find the purple polygons */

    for (ri = R; ri != NULL; ri = ri->next)

    {

        POLYMAP *bi;

        for (bi = B; bi != NULL; bi = bi->next)

        {

            POLYAREA *rp = NULL;

            if (poly_Boolean(ri->area, bi->area, &rp, PBO_ISECT))

                return 1;



            strcat(strcat(strcpy(res, ri->label), " & "), bi->label);

            if (poly_AddMap(p, rp, res))

                return 1;

        }

    }



    /* find pure red and blue polygons */

    return SubMap(R, B, r) || SubMap(B, R, b);

} /* poly_Overlay */

