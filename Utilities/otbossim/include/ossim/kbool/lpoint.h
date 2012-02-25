/*! \file kbool/include/kbool/lpoint.h
    \author Probably Klaas Holwerda

    Copyright: 2001-2004 (C) Probably Klaas Holwerda

    Licence: wxWidgets Licence

    RCS-ID: $Id: lpoint.h 9094 2006-06-13 19:12:40Z dburken $
*/

/* @@(#) $Source$ $Revision: 9094 $ $Date: 2006-06-13 21:12:40 +0200 (mar. 13 juin 2006) $ */

/*
Program	LPOINT.H
Purpose	Definition of GDSII pointtype structure
Last Update	12-12-1995
*/

#ifndef LPOINT_H
#define LPOINT_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

#include <ossim/kbool/booleng.h>

class A2DKBOOLDLLEXP LPoint
{
	public:
		LPoint();
		LPoint(B_INT const ,B_INT const);
		LPoint(LPoint* const);

		void		Set(const B_INT,const B_INT);
		void		Set(const LPoint &);

		LPoint		GetPoint();
		B_INT		GetX();
		B_INT		GetY();
		void 		SetX(B_INT);
		void		SetY(B_INT);
		bool		Equal(const LPoint a_point, B_INT Marge );
		bool		Equal(const B_INT,const B_INT , B_INT Marge);
		bool		ShorterThan(const LPoint a_point, B_INT marge);
		bool		ShorterThan(const B_INT X, const B_INT Y, B_INT);

		LPoint	&operator=(const LPoint &);
		LPoint	&operator+(const LPoint &);
		LPoint	&operator-(const LPoint &);

		LPoint	&operator*(int);
		LPoint	&operator/(int);

		int		operator==(const LPoint &) const;
		int		operator!=(const LPoint &) const;

	protected:
		B_INT _x;
		B_INT _y;

};

#endif
