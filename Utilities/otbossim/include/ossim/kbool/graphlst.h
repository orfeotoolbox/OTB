/*! \file kbool/include/kbool/graphlst.h
    \author Probably Klaas Holwerda

    Copyright: 2001-2004 (C) Probably Klaas Holwerda

    Licence: wxWidgets Licence

    RCS-ID: $Id: graphlst.h 9094 2006-06-13 19:12:40Z dburken $
*/

/* @@(#) $Source$ $Revision: 9094 $ $Date: 2006-06-13 21:12:40 +0200 (mar. 13 juin 2006) $ */

/*
Program	GRAPHLST.H
Purpose	Implements a list of graphs (header)
Last Update	11-03-1996
*/

#ifndef GRAPHLIST_H
#define GRAPHLIST_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

#include <ossim/kbool/booleng.h>

#include <ossim/kbool/_lnk_itr.h>

#include <ossim/kbool/graph.h>

class Debug_driver;


class A2DKBOOLDLLEXP GraphList: public DL_List<void*>
{
protected:
   Bool_Engine* _GC;
public:
   
   GraphList(Bool_Engine* GC);
   
   GraphList( GraphList* other ); 
   
   ~GraphList();
   
   void clearList();
   void clearListAndBoolEngine();
   void	MakeOneGraph(Graph *total);
   
   void	Prepare(Graph *total);
   void MakeRings();
   void Correction();
   
   void	Simplify( double marge);
   void Smoothen( double marge);
   void Merge();
   void Boolean(BOOL_OP operation, int intersectionRunsMax );
   
   void WriteGraphs();
   void WriteGraphsKEY( Bool_Engine* GC );
   
protected:
   void				Renumber();
   void				UnMarkAll();
};


#endif

