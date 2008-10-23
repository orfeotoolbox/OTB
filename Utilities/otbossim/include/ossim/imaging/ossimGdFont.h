//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimGdFont.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimGdFont_HEADER
#define ossimGdFont_HEADER
/* gd.h: declarations file for the graphic-draw module.
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  This software is provided "AS IS." Thomas Boutell and
 * Boutell.Com, Inc. disclaim all warranties, either express or implied, 
 * including but not limited to implied warranties of merchantability and 
 * fitness for a particular purpose, with respect to this code and accompanying
 * documentation. */

typedef struct {
	/* # of characters in font */
	int nchars;
	/* First character is numbered... (usually 32 = space) */
	int offset;
	/* Character width and height */
	int w;
	int h;
	/* Font data; array of characters, one row after another.
		Easily included in code, also easily loaded from
		data files. */
	char *data;
} ossimGdFont;

/* Text functions take these. */
typedef ossimGdFont *ossimGdFontPtr;

#endif
