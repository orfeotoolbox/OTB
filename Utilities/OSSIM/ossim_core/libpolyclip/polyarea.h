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



      polyarea.h

      (C) 1997 Alexey Nikitin, Michael Leonov

      (C) 1997 Klamer Schutte (minor patches)

*/



#ifndef	POLY_H

#define	POLY_H



#include "vectmatr.h"



#ifdef __cplusplus

extern "C" {

#endif



enum

{

    err_no_memory = 2,

    err_bad_parm = 3,

    err_ok = 0

};



#define PLF_STATUS      0x0003

#define PLF_CYCLINK     0x0004

#define PLF_MARK		0x0008

#define PLF_RESERVED	0x00FF



#define PLF_ORIENT      0x0100

#define PLF_DIR         0x0100

#define PLF_INV         0x0000



typedef struct VNODE VNODE;

struct VNODE

{

    VNODE *next, *prev;

    unsigned int Flags;

    Vector point;

    union

    {

        VNODE *link;

        void *v;

        int i;

    } t;

};



typedef struct PLINE PLINE;

struct PLINE

{

   PLINE *next;
   
   VNODE head;
   
   unsigned int Count;
   
   unsigned int Flags;
   
   double xmin, ymin, xmax, ymax;
   
};



PLINE *poly_NewContour(Vector v);



void poly_IniContour(PLINE *  c);

void poly_ClrContour(PLINE *  c);  /* clears list of vertices */

void poly_DelContour(PLINE ** c);



BOOL poly_CopyContour(PLINE ** dst, PLINE * src);



void poly_PreContour(PLINE * c, BOOL optimize); /* prepare contour */

void poly_InvContour(PLINE * c);  /* invert contour */



VNODE *poly_CreateNode(Vector v);



void poly_InclVertex(VNODE * after, VNODE * node);

void poly_ExclVertex(VNODE * node);



/**********************************************************************/



typedef struct POLYAREA POLYAREA;

struct POLYAREA

{

    POLYAREA *f, *b;

    PLINE *contours;

};



/************** POLYMAP ****************/



#define MAXLABEL 128



typedef struct POLYMAP  POLYMAP;

struct POLYMAP

{

    POLYMAP  *next;

    POLYAREA *area;

    char     *label;

};



BOOL LoadPOLYMAP(POLYMAP **m, char *fname);

void SavePOLYMAP(POLYMAP  *m, char *fname);

BOOL poly_Overlay(POLYMAP *R, POLYMAP *B, POLYMAP **r, POLYMAP **b, POLYMAP **p);

/* R and B are input maps, the output maps are:

r = R - B,

b = B - R,

p = R & B */



BOOL poly_AddMap(POLYMAP **m, POLYAREA *area, char *label);

void poly_DelMap(POLYMAP **m);



/***************************************/



BOOL poly_M_Copy0(POLYAREA ** dst, const POLYAREA * srcfst);

void poly_M_Incl(POLYAREA **list, POLYAREA *a);



BOOL poly_Copy0(POLYAREA **dst, const POLYAREA *src);

BOOL poly_Copy1(POLYAREA  *dst, const POLYAREA *src);



BOOL poly_InclContour(POLYAREA * p, PLINE * c);

BOOL poly_ExclContour(POLYAREA * p, PLINE * c);





BOOL poly_ChkContour(PLINE * a);



BOOL poly_CheckInside(POLYAREA * c, Vector v0);



/**********************************************************************/



/* tools for clipping */



/* checks whether point lies within contour

independently of its orientation */



int poly_InsideContour(PLINE *c, Vector v);

int poly_ContourInContour(PLINE * poly, PLINE * inner);

POLYAREA *poly_Create(void);



void poly_Free(POLYAREA **p);

void poly_Init(POLYAREA  *p);

void poly_Clear(POLYAREA *p);

BOOL poly_Valid(POLYAREA *p);



enum PolygonBooleanOperation {

	PBO_UNITE,

	PBO_ISECT,

	PBO_SUB,

	PBO_XOR

};



int poly_Boolean(const POLYAREA * a, const POLYAREA * b, POLYAREA ** res, int action);



#ifdef __cplusplus

}

#endif





#endif /* POLY_H */

