/*

      poly_Boolean: a polygon clip library

      Copyright (C) 1997  Alexey Nikitin, Michael Leonov

      leonov@propro.ru



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



      polyio.c

      (C) 1997 Michael Leonov

*/



#include <stdio.h>

#include <string.h>



#include "polyarea.h"

#include "vectmatr.h"



static

PLINE *LoadPLINE(FILE * f)

{

    int cnt, i;

    PLINE *res = NULL;

    Vector v;



    fscanf(f, "%d", &cnt);

    if (cnt < 3) return NULL;

    for ( i = 0; i < cnt; i++ )

    {

        fscanf(f, "%lf %lf", v, v + 1);

        v[2] = 0.0f;

        if ( res == NULL )

        {

            if ((res = poly_NewContour(v)) == NULL)

                return NULL;

        }

        else

            poly_InclVertex(res->head.prev, poly_CreateNode(v));

    }

    poly_PreContour(res, TRUE);

    return res;

} /* LoadPLINE */



static

size_t til_eol(const char *str)

{

    char *eoln;

    eoln = strchr(str, '\n');

    return eoln != NULL ? (size_t)(eoln - str) : MAXLABEL;

} /* til_eol */



BOOL LoadPOLYMAP(POLYMAP **m, char *fname)

{

    int nMaps, k;

    char label[MAXLABEL + 1];

    FILE *f = fopen( fname, "r" );

    if (f == NULL) return 1;

    fscanf(f, "%d", &nMaps);

    if (nMaps < 1) return 1;

    for (k = 0; k < nMaps; k++)

    {

        int cnt, pcnt, i, j;

        PLINE *cntr;

        POLYAREA *p;

        POLYAREA *area = NULL;



        fgets(label, MAXLABEL, f);

        fgets(label, MAXLABEL, f);

        label[til_eol(label)] = 0;

        fscanf(f, "%d", &pcnt);

        if (pcnt < 1) return 1;

        for (j = 0; j < pcnt; j++)

        {

            fscanf(f, "%d", &cnt);

            if (cnt < 1) return err_bad_parm;

            p = NULL;

            for (i = 0; i < cnt; i++)

            {

                if ( (cntr = LoadPLINE(f)) == NULL)

                    return err_bad_parm;

                if ( (cntr->Flags & PLF_ORIENT) != (unsigned int)(i ? PLF_INV : PLF_DIR) )

                    poly_InvContour(cntr);

                if (p == NULL) p = poly_Create();

                if (p == NULL) return 1;

                poly_InclContour(p, cntr);

                cntr = NULL;

                if (!poly_Valid(p)) return err_bad_parm;

            }

            poly_M_Incl(&area, p);

        }

        if (poly_AddMap(m, area, label))

            return 1;

    }

    fclose(f);

    return 0;

} /* LoadPOLYMAP */



static

void SavePLINE(FILE * f, PLINE * res)

{

    VNODE *cur;



    fprintf(f, "%d\n", res->Count);

    cur = &res->head;

    do {

        fprintf(f, "%f %f\n", cur->point[0], cur->point[1] );

    } while ( ( cur = cur->next ) != &res->head );

} /* SavePLINE */





void SavePOLYMAP(POLYMAP *m, char *fname)

{

    POLYMAP *mi;

    int     i;

    FILE *f = fopen(fname, "w");

    if (f == NULL) return;

    for (mi = m, i = 0; mi != NULL; mi = mi->next)

        i++;

    fprintf(f, "%d\n", i);

    for (mi = m; mi != NULL; mi = mi->next)

    {

        PLINE *cntr;

        POLYAREA * curpa;



        i = 0, curpa = mi->area;

        do {

            i++;

        } while ( (curpa = curpa->f) != mi->area);



        fprintf(f, "%s\n", mi->label);

        fprintf(f, "%d\n", i);

        curpa = mi->area; 

        do {

            for (cntr = curpa->contours, i = 0; cntr != NULL; cntr = cntr->next)

                i++;

            fprintf(f, "%d\n", i);

            for (cntr = curpa->contours; cntr != NULL; cntr = cntr->next)

                SavePLINE(f, cntr);

        }  while ((curpa = curpa->f) != mi->area);

    }

} /* SavePOLYMAP */



