/*************************************************************************
 *
 *N  Module VPFMISC
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains miscellaneous routines used (potentially) by
 *     several other VPF modules.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                     DOS Turbo C
 *E
 *************************************************************************/

#ifdef __MSDOS__
#  include <graphics.h>
#  include <alloc.h>
#else
#  ifdef __APPLE__ 
#    include <sys/types.h>
#    include <sys/malloc.h>
#  else
#    ifndef __FreeBSD__
#      include <malloc.h>
#      include <string.h>
#    endif
#  endif
#endif


#include <ctype.h> /* For toupper function. */

#include <ossim/vpfutil/vpftidx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <fcntl.h>
#ifdef __MSDOS__
#  include <sys\stat.h>
#  include <io.h>
#  include <conio.h>
#  include <dos.h>
#  include <dir.h>
#else
#  include <sys/stat.h>
#endif
#include <math.h>
#include <errno.h>
#include <stdarg.h>
#ifdef __MSDOS__
#  include <process.h>
#  include <bios.h>
#  include "gui.h"
#endif
#include <ossim/vpfutil/vpfmisc.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfview.h>


#ifdef __MSDOS__
  extern color_type menucolor, menubordercolor, menutextcolor;
#endif

#if !defined(__MSDOS__) && !defined(_MSC_VER)
char* strrev(char* str)
{
  register int i,len;
  char *copy;

  len = strlen(str);
  copy = (char *) malloc(sizeof(char)*(len+1));
  (void) strcpy(copy,str);
  for(i=0;i<len;i++)
    str[i]=copy[(len-1)-i];
  free(copy);
  return str;
}

char* strupr(char* str)
{
  char* s = str;
  while (*s)
  {
	  *s = toupper(*s);
	  ++s;
  }
  return str;
}
#endif /* #ifndef __MSDOS__ */

/*************************************************************************
 *
 *N  vpfmalloc
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function allocates memory off of the heap and checks for
 *     success.  If not enough memory is available, this routine will abort
 *     the program with an error message.  Can be in graphics mode to call
 *     this procedure, not a necessity.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    size    <input> == (unsigned long) number of bytes to allocate.
 *    return <output> == (void *) pointer to the allocated memory.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1990    Original Version    DOS Turbo C
 *    Ronald Rozensky   Sept 1991 check for no memory, graphics mode
 *E
 *************************************************************************/
void *vpfmalloc( unsigned long size )
{
   void *p;
#ifdef __MSDOS__
   p = farmalloc( size );
   if (!p)
   {
      printf("Out of memory  %ld  %ld\n",size,farcoreleft());
      getch();

      if (graphic_mode())
	 closegraph();
      exit(1);
   }
#else
   p = malloc(size);
   if (!p)
   {
     printf("Out of memory %ld\n", size);
     exit(1);
   }
#endif
   return p;
}
#ifdef __MSDOS__
/*************************************************************************
 *
 *N  get_display_position
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function finds a suitable position to display a new window on
 *     the screen.  If the window fits, its upper left hand corner will be
 *     the current mouse position.  If not, it will be adjusted so that it
 *     will fit on the screen.  Should be in graphics mode to call this
 *     function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    x     <output> == (int *) pointer to the x display position.
 *    y     <output> == (int *) pointer to the y display position.
 *    window <input> == (window_type) window to be displayed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1990    Original Version    DOS Turbo C
 *E
 *************************************************************************/
void get_display_position( int *x, int *y, window_type window )
{
   int mbutton, delta;

   getmouseposition( x, y, &mbutton );
   delta = window.x2 - window.x1;
   if ((*x) + delta > getmaxx()-2)
      (*x) = getmaxx()-delta-10;
   if ((*x) < 2)
      (*x) = 2;
   delta = window.y2 - window.y1;
   if ((*y) + delta > getmaxy()-2)
      (*y) = getmaxy()-delta-10;
   if ((*y) < 2)
      (*y) = 2;
}


/*************************************************************************
 *
 *N  displayinfo
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays an array of text strings in a
 *     popup text window.  Must be in graphics mode to call this function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    text[] <input> == (char *) array of text strings to be displayed.
 *    nlines <input> == (int) number of text lines.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    June 1990    Original Version    DOS Turbo C
 *E
 *************************************************************************/
void displayinfo( char *text[],
		  int  nlines )
{
   int         i,maxw,height,x,y,pad;
   panel_type  panel;

   arrow_cursor();
   settextstyle( SMALL_FONT, HORIZ_DIR, 4 );
   maxw = 0;
   height = 0;
   for (i=0;i<nlines;i++) {
      if (textwidth(text[i]) > maxw) maxw = textwidth(text[i]);
      height = height + textheight(text[i]) + 5;
   }
   if (maxw < (textwidth("Continue") + 10))
      maxw = textwidth("Continue") + 10;
   pad = (maxw*10)/100;
   maxw = maxw + (2*pad);
   if (maxw > getmaxx())
     maxw = getmaxx() - 8;
   height = height + 2*(textheight("Continue") + 10);

   create_panel( &panel, maxw, height, menucolor, menubordercolor );
   x = pad;
   y = 0;
   for (i=0;i<nlines;i++) {
      y = y + textheight(text[i]) + 3;
      create_label( text[i], x, y, SMALL_FONT, 4, menutextcolor, &panel );
   }
   y = height - (textheight("Continue") + 6);
   x = (maxw - (textwidth("Continue") + 10))/2;
   create_button( 27, "Continue", x, y, SMALL_FONT, 4,
		  menutextcolor, menucolor, menubordercolor, RELIEVED,
		  &panel );

   get_display_position( &x, &y, panel.window );

   display_panel( &panel, x, y );
   i = process_panel( &panel, i );

   destroy_panel( &panel );
   close_panel( &panel );
}



/*************************************************************************
 *
 *N  displaymessage
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays a list of text strings in a
 *     popup text window.  The list must be NULL-terminated.  Must be in
 *     graphics mode to call this function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    string <input> == (char *) first text string to be displayed.
 *    ... <input> == (char *) variable list of text strings to be displayed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    August 1991                        DOS Turbo C
 *E
 *************************************************************************/
void displaymessage( char *string, ... )
{
   int         i,maxw,height,x,y,pad, nlines;
   panel_type  panel;
   char **text;
   va_list arglist;

   va_start(arglist,string);
   nlines = 1;
   while (va_arg(arglist,char *)) nlines++;
   va_end(arglist);

   text = (char **)vpfmalloc((nlines+1)*sizeof(char *));
   text[0] = string;
   va_start(arglist,string);
   for (i=1;i<nlines;i++) {
      text[i] = va_arg(arglist,char *);
   }
   va_end(arglist);

   settextstyle( SMALL_FONT, HORIZ_DIR, 4 );
   maxw = 0;
   height = 0;
   for (i=0;i<nlines;i++) {
      if (textwidth(text[i]) > maxw) maxw = textwidth(text[i]);
      height = height + textheight(text[i]) + 5;

   }
   if (maxw < (textwidth("Continue") + 10))
      maxw = textwidth("Continue") + 10;
   pad = (maxw*10)/100;
   maxw = maxw + (2*pad);
   if (maxw > getmaxx())
     maxw = getmaxx() - 8;
   height = height + 2*(textheight("Continue") + 5) + 5;

   create_panel( &panel, maxw, height, menucolor, menubordercolor );
   x = pad;
   y = 0;
   for (i=0;i<nlines;i++) {
      y = y + textheight(text[i]) + 5;
      create_label( text[i], x, y, SMALL_FONT, 4, menutextcolor, &panel );
   }
   y = height - (textheight("Continue") + 6);
   x = (maxw - (textwidth("Continue") + 10))/2;
   create_button( 27, "Continue", x, y, SMALL_FONT, 4,
		  menutextcolor, menucolor, menubordercolor, RELIEVED,
		  &panel );

   get_display_position( &x, &y, panel.window );

   display_panel( &panel, x, y );
   i = process_panel( &panel, i );

   destroy_panel( &panel );
   close_panel( &panel );

   free(text);
}



/*************************************************************************
 *
 *N  display_message
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays a one-line informational message to the
 *     screen and waits for the user to continue.  Must be in graphics mode
 *     to call this function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    str <input> == (char *) message to be displayed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    July 1990    Original Version    DOS Turbo C
 *E
 *************************************************************************/
void display_message(char *str)
{
   char **txt;

   txt = (char **)vpfmalloc( (unsigned long)(1*sizeof(char **)) );
   txt[0] = strdup(str);
   displayinfo( txt, 1 );
   free(txt[0]);
   free(txt);
}

#else

void displaymessage( char *s, ... )
{
   int         i, nlines;
   char **text;
   va_list arglist;

   va_start(arglist,s);
   nlines = 1;
   while (va_arg(arglist,char *)) nlines++;
   va_end(arglist);

   text = (char **)vpfmalloc((nlines+1)*sizeof(char *));
   text[0] = s;
   va_start(arglist,s);
   for (i=1;i<nlines;i++) {
      text[i] = va_arg(arglist,char *);
   }
   va_end(arglist);
   for (i=0;i<nlines;i++) {
       fprintf(stderr, "%s", text[i]);
   }
   free(text);
}

void display_message(char *str)
{
   fprintf(stderr, "%s", str);
}


#endif


/*************************************************************************
 *
 *N  float_to_dms
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function converts a floating point lat lon coordinate to
 *     degrees-minutes-seconds format.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    coord   <input> == (double) floating point lat lon coordinate.
 *    return <output> == (dms_type) degrees-minutes-seconds coordinate.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1990    Original Version    DOS Turbo C
 *E
 *************************************************************************/
dms_type float_to_dms( double coord )
{
   dms_type dms_coord;

   dms_coord.degrees = (int)coord;
   dms_coord.minutes = (int)((double)(coord-(int)coord)*60.0);
   dms_coord.seconds = (float)((((double)(coord-(int)coord)* 60.0) -
			(double)dms_coord.minutes) * 60.0);
   dms_coord.minutes = abs(dms_coord.minutes);
   dms_coord.seconds = (float)(fabs(dms_coord.seconds));

   if (dms_coord.minutes == 60) {
      if (dms_coord.degrees > 0)
	 dms_coord.degrees++;
      else
	 dms_coord.degrees--;
      dms_coord.minutes = 0;
   }

   if ((dms_coord.degrees == 0)&&(coord < 0.0)) dms_coord.minutes *= -1;

   return dms_coord;
}


/*************************************************************************
 *
 *N  dms_to_float
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function converts a coordinate in degrees-minutes-seconds format
 *     to a floating point coordinate.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    coord   <input> == (dms_type) degrees-minutes-seconds coordinate.
 *    return <output> == (double) floating point lat lon coordinate.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1990    Original Version    DOS Turbo C
 *E
 *************************************************************************/
double dms_to_float( dms_type coord )
{
   return ( (double)coord.degrees +
	    ((double)coord.minutes / 60.0) +
	    (coord.seconds/3600.0) );
}



/*************************************************************************
 *
 *N  dms_string
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function converts a coordinate in degrees-minutes-seconds format
 *     to a character string.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    coord   <input> == (dms_type) degrees-minutes-seconds coordinate.
 *    seconds <input> == (int) flag to indicate if seconds are to be
 *                       displayed.
 *    return <output> == (char *) character string.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1990    Original Version    DOS Turbo C
 *E
 *************************************************************************/
char *dms_string( dms_type coord, int seconds )
{
   char *str,deg[5],min[3],sec[3];

   str = (char *)vpfmalloc( (20*sizeof(char))+
			    sizeof(deg)+sizeof(min)+sizeof(sec) );

   itoa( coord.degrees, deg, 10 );
   itoa( abs(coord.minutes), min, 10 );
   if (seconds) itoa( ((int)floor(coord.seconds)), sec, 10 );
   if ((coord.degrees==0)&&(coord.minutes < 0)) {
      strcpy(str,"-0");
      coord.minutes *= -1;
   } else {
      strcpy(str,deg);
   }
   strcat(str," deg ");
   strcat(str,min);
   strcat(str," min ");
   if (seconds) {
      strcat(str,sec);
      strcat(str," sec");
   }

   return(str);
}
#ifdef __MSDOS__
/*************************************************************************
 *
 *N  out_dms
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays the given degree-minutes-seconds coordinate
 *     at the specified location on the screen.  The direction is given
 *     for placement along the edges of the screen.  Must be in graphics
 *     mode to call this function.  User must make sure that all text will
 *     fit on the screen; this program will not adjust size, but will adjust
 *     location to make sure that all text that CAN fit on the screen WILL
 *     fit on the screen.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    coord   <input> == (dms_type) degrees-minutes-seconds coordinate.
 *    x       <input> == (int) x location.
 *    y       <input> == (int) y location.
 *    dir     <input> == (int) dir.  HORIZ_DIR=0, VERT_DIR=1
 *    seconds <input> == (int) flag indicating whether to display seconds.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    October 1990    Original Version    DOS Turbo C
 *    Ronald Rozensky   Sept 1991 x,y adjustments to make text fit on screen
 *E
 *************************************************************************/
void out_dms( dms_type coord, int x, int y, int dir, int seconds )
{
   char deg[8],min[8],sec[8];
   int x1,x2,y1,y2, width, adjust, minutes;

   settextjustify(LEFT_TEXT,BOTTOM_TEXT);

   itoa( coord.degrees, deg, 10 );
   if ((coord.seconds >= 30.0)&&(!seconds)) {
      /* Round up */
      minutes = abs(coord.minutes)+1;
      if (minutes == 60) {
	 /* Rounded up to next degree */
	 strcpy(min,"0");
	 if (coord.degrees > 0)
	    coord.degrees++;
	 else if (coord.degrees < 0)
	    coord.degrees--;
	 else {
	    if (coord.minutes < 0 || coord.seconds < 0.0)
	       coord.degrees++;
	    else
	       coord.degrees--;
	 }
      } else {
	 itoa( minutes, min, 10 );
      }
      itoa( coord.degrees, deg, 10 );
   } else {
      itoa( abs(coord.minutes), min, 10 );
   }
   if (seconds) itoa( floor(abs(coord.seconds+0.5)), sec, 10 );
   if ((coord.degrees==0)&&(coord.minutes < 0)) {
      strcpy(deg,"-0");
   }
   if ((coord.degrees==0)&&(coord.minutes==0)&&(coord.seconds<0)) {
      strcpy(deg,"-0");
   }
   strcat(min,"'");
   if (seconds) strcat(sec,"''");

   if (dir==HORIZ_DIR) {
      width = textwidth(deg)+3;
      if ((coord.minutes > 0)||(seconds)) {
	 width += textwidth(min)+3;
      }
      if (seconds) {
	 width += textwidth(sec)+3;
      }
      x1 = x - width/2;
      x2 = x1 + width;
      y1 = y-textheight(deg)-1;
      y2 = y+1;
   } else {
      width = textwidth(deg)+3;
      if ((coord.minutes > 0)||(seconds)) {
	 width += textwidth(min)+3;
      }
      if (seconds) {
	 width += textwidth(sec)+3;
      }
      x1 = x;
      x2 = x1 + width;
      y1 = y - (textheight(deg)+4)/2;
      y2 = y + (textheight(deg)+4)/2;
   }

   setfillstyle(SOLID_FILL,DARKGRAY);
   if (x1 <0)
     {
     adjust = 0-x1;
     x1 = 0;
     x2 = x2 += adjust;
     }
   if (y1 <0)
     {
     adjust = 0-y1;
     y1 = 0;
     y2 = y2 += adjust;
     }
   if ((y1-y2 < getmaxy()) && (y2 > getmaxy()))
     {
     adjust = y2-getmaxy();
     y2 = getmaxy();
     y1 -= adjust;
     }
   if ((x1-x2 < getmaxx()) && (x2 > getmaxx()))
     {
     adjust = x2-getmaxx();
     x2 = getmaxx();
     x1 -= adjust;
     }
   bar(x1,y1,x2,y2);
   outtextxy(x1,y2,deg);
   circle(x1+textwidth(deg)+1, y2-textheight(deg), 2);
   if ((coord.minutes > 0)||(seconds))
      outtextxy(x1+textwidth(deg)+5,y2-1,min);
   if (seconds)
      outtextxy(x1+textwidth(deg)+3+textwidth(min)+3,y2-1,sec);

}
#endif


/*************************************************************************
 *
 *N  strpos
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the character array position of the first
 *     occurrence of the given character. (-1 if not present)
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    str     <input> == (char *) character string to be searched.
 *    ch      <input> == (char) character to search for.
 *    return <output> == (int) position of character (-1 if not present).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1990    Original Version    DOS Turbo C
 *E
 *************************************************************************/
int strpos( char *str, char ch )
{
   register int i;

   for (i=0;(unsigned int)i<strlen(str);i++)
      if (str[i] == ch) return i;
   return -1;
}




/*************************************************************************
 *
 *N  fwithin
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether the input (x,y) coordinate lies
 *     within the given rectangular extent.  It returns either TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    x        <input> == (float) x coordinate of the test point.
 *    y        <input> == (float) y coordinate of the test point.
 *    extent   <input> == (extent_type) rectangular area to be tested.
 *    fwithin <output> == (VPF_BOOLEAN) VPF_BOOLEAN:
 *                               TRUE if (x,y) lies within area
 *                               FALSE if (x,y) lies outside of area
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1990   Original Version   DOS Turbo C
 *E
 *************************************************************************/
VPF_BOOLEAN fwithin( float x,
		 float y,
		 extent_type extent )
{
   if ((x >= extent.x1) && (x <= extent.x2) &&
      (y >= extent.y1) && (y <= extent.y2))
      return TRUE;
   else
      return FALSE;
}



/*************************************************************************
 *
 *N  contained
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether an input rectangular 'extent1' is
 *     contained within another rectangular 'extent2'.  It returns either
 *     TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    extent1 <input> == (extent_type) rectangular area to be tested within.
 *    extent2 <input> == (extent_type) rectangular area to be tested against.
 *    contained <output> == (VPF_BOOLEAN) VPF_BOOLEAN:
 *                               TRUE if extent2 contains extent1
 *                               FALSE if extent2 does not contain extent1
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1990   Original Version   DOS Turbo C
 *E
 *************************************************************************/
VPF_BOOLEAN contained( extent_type extent1,
		   extent_type extent2 )
{
   /* Test each of the four corners of extent1 */
   /*   	  ____________
		  |          |
		  |  2       |
		  |    ______+___
		  |    |     |
		  |    |     |
		  |    |  1  |
		  |    |     |
		  ------------
		       |
		       |
   */
   if ((extent1.x1 >= extent2.x1) && (extent1.x1 <= extent2.x2) &&
      (extent1.y1 >= extent2.y1) && (extent1.y1 <= extent2.y2))
      return TRUE;
   /*             ____________
		  |          |
		  |  2       |
		  |          |
	       ---+------    |
		  |     |    |
		  |  1  |    |
		  |     |    |
		  ------------
			|
			|
   */
   if ((extent1.x2 >= extent2.x1) && (extent1.x2 <= extent2.x2) &&
      (extent1.y1 >= extent2.y1) && (extent1.y1 <= extent2.y2))
      return TRUE;
   /*
		       |
		  _____|______
		  |    |     |
		  |  1 |     |
		  |    |     |
		--+-----     |
		  |          |
		  |    2     |
		  |__________|
   */
   if ((extent1.x2 >= extent2.x1) && (extent1.x2 <= extent2.x2) &&
      (extent1.y2 >= extent2.y1) && (extent1.y2 <= extent2.y2))
      return TRUE;
   /*			|
			|
		  ______|_____
		  |     |    |
		  |     | 1  |
		  |     |    |
		  | 2   -----+--
		  |          |
		  |          |
		  ------------
   */
   if ((extent1.x1 >= extent2.x1) && (extent1.x1 <= extent2.x2) &&
      (extent1.y2 >= extent2.y1) && (extent1.y2 <= extent2.y2))
      return TRUE;


   /* Test for overlaps */
   /*	 ________             --------------
	 |      |             |     2      |
	 |  1   |             | ---------- |
     ----+------+----         | |        | |
     | 2 |      |   |         | |        | |
     |   |      |   |         | |   1    | |
     ----+------+----         | |        | |
	 |      |             | |        | |
	 |      |             | ---------- |
	 --------             |            |
			      --------------
   */
   if ((extent1.y1 >= extent2.y1) && (extent1.y2 <= extent2.y2) &&
       (extent1.x1 <= extent2.x2) && (extent1.x2 >= extent2.x1))
      return TRUE;
   /*	 ________             --------------
	 |      |             |     1      |
	 |  2   |             | ---------- |
     ----+------+----         | |        | |
     | 1 |      |   |         | |        | |
     |   |      |   |         | |   2    | |
     ----+------+----         | |        | |
	 |      |             | |        | |
	 |      |             | ---------- |
	 --------             |            |
			      --------------
   */
   if ((extent1.x1 >= extent2.x1) && (extent1.x2 <= extent2.x2) &&
       (extent1.y1 <= extent2.y2) && (extent1.y2 >= extent2.y1))
      return TRUE;
   return FALSE;
}


#ifdef __MSDOS__
/*************************************************************************
 *
 *N  info_window
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays a temporary window to the screen with a
 *     single string of text.  Must be in graphics mode to call this
 *     function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    text    <input> == (char *)message to display.
 *    return <output> == (window_type) window created and displayed.
 *                       [Will need to be deleted]
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990   Original Version   DOS Turbo C
 *E
 *************************************************************************/
window_type info_window( char *text )
{
   window_type w;
   int x,y;

   settextstyle(SMALL_FONT,HORIZ_DIR,4);
   settextjustify(LEFT_TEXT,BOTTOM_TEXT);
   if (textwidth(text) > getmaxx())
     x = getmaxx()-20;
    else
     x = textwidth(text) + 20;
   create_window( &w, x, textheight(text)+10,
		  menucolor,menubordercolor );
   get_display_position( &x, &y, w );
   open_window( &w, x,y );
   setcolor(menutextcolor);
   hidemousecursor();
   outtextxy( 10,textheight(text)+5,text );
   showmousecursor();
   return w;
}
#endif


/*************************************************************************
 *
 *N  rightjust
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function right justifies the given string and removes the
 *     trailing newline character (if one exists).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    str <inout> == (char *) string to be justified.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   November 1990   Original Version   DOS Turbo C
 *E
 *************************************************************************/
char *rightjust( char *str )
{
   return strrev(leftjust(strrev(str)));
}

/*************************************************************************
 *
 *N  leftjust
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function left justifies the given string and removes the
 *     trailing newline character (if one exists)
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    str <inout> == (char *) string to be justified.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   November 1990   Original Version   DOS Turbo C
 *E
 *************************************************************************/
char *leftjust(char * str)
{
   register char * eol;

   strcpy(str, str + strspn(str, " \t\n\b"));

   if ((eol = strchr(str, '\n')) != NULL)
     *eol = 0;

   return str;
}

#ifdef __MSDOS__
/*************************************************************************
 *
 *N  displayerror
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays an error message when a disk error is detected.
 *     It displays the given lines of text in a popup panel and waits for
 *     the user to click on either retry or cancel.  It returns 1 for retry
 *     and 0 for cancel.  Must be in graphics mode to call this function.
 *
 *     text strings may contain embedded newlines, in which case text to the
 *     right of the newline will be displayed on (what else?) a new line.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    text[]  <input> == (char *) array of text strings to be displayed.
 *    nlines  <input> == (int) number of lines of text (this count ignores
 *                             newline characters embedded in the text
 *                             strings
 *    return <output> == (VPF_BOOLEAN) 1 => retry,
 *                                 0 => cancel.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    July 1990    Prototype 3    DOS Turbo C
 *E
 *************************************************************************/
VPF_BOOLEAN displayerror( char *text[],
		      int  nlines )
{
   register int i;
   int          maxw,
		height,
		x,
		y,
		pad,
		choice,
		n_real_lines;
   panel_type   panel;
   int          retry_button  = 'r',
		cancel_button = 'c',
		msg_ar_sz     = 10;
   char       * walker,
	     ** msgs;
   struct viewporttype view;

   getviewsettings( &view );
   setviewport(0,0,getmaxx(),getmaxy(),1);


   settextstyle( SMALL_FONT, HORIZ_DIR, 4 );
   msgs = (char **) vpfmalloc(msg_ar_sz * sizeof(char *));
   maxw = height = 0;

   for (n_real_lines = i = 0; i < nlines; i++) {
     walker = text[i];
     while (1) {
	size_t substr_len = strcspn(walker, "\n");
	char   plug;

	maxw                 = max(maxw, textwidth(walker));
	height               = height + textheight(walker) + 5;
	plug                 = walker[substr_len];
	walker[substr_len]   = '\0';
	msgs[n_real_lines++] = strdup(walker);

	if (n_real_lines == msg_ar_sz)
	  msgs = (char **) realloc(msgs, (msg_ar_sz += 5) * sizeof(char *));
	if (plug == 0)
	  break;

	walker[substr_len] = plug;
	walker            += substr_len + 1;
     }
   }

   if (maxw < (textwidth("Retry") + textwidth("Cancel") + 20))
      maxw = textwidth("Retry") + textwidth("Cancel") + 20;

   pad    = (maxw*10)/100;
   maxw   = maxw + (2*pad);
   height = height + 2*(textheight("Retry") + 5) + 1;
   maxw   = min(getmaxx(), maxw);
   height = min(getmaxy()-10, height);

   create_panel( &panel, maxw, height, menucolor, menubordercolor );

   for (y = i = 0; i < n_real_lines; i++) {
     create_label(msgs[i], pad, y, SMALL_FONT, 4, menutextcolor, &panel );
     y += textheight(msgs[i]) + 3;
   }

   y = height-15;

   create_button( retry_button,
		  "Retry",
		  3,
		  y,
		  SMALL_FONT,
		  4,
		  menutextcolor,
		  menucolor,
		  menubordercolor,
		  RELIEVED,
		  &panel );
   create_button( cancel_button,
		  "Cancel",
		  maxw - (textwidth("Cancel") + 13),
		  y,
		  SMALL_FONT,
		  4,
		  menutextcolor,
		  menucolor,
		  menubordercolor,
		  RELIEVED,
		  &panel );

   get_display_position( &x, &y, panel.window );

   display_panel( &panel, x,y );
   showmousecursor();
   arrow_cursor();
   choice = process_panel( &panel, 0 );
   hidemousecursor();

   destroy_panel( &panel );
   close_panel( &panel );

   setviewport(view.left,view.top,view.right,view.bottom,view.clip);

   for (i = 0; i < n_real_lines; i++)
     free(msgs[i]);
   free(msgs);

   return (choice == retry_button) ? 1 : 0;
}



/*************************************************************************
 *
 *N  getcursorposition
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the cursor position when either the mouse
 *     has been moved or an arrow key has been pressed.  Must have called
 *     showmousecursor() before calling this routine.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    x       <inout> == (int *) x position of the cursor.
 *    y       <inout> == (int *) y position of the cursor.
 *    button  <inout> == (int *) mouse button pressed (or simulated with
 *                               the keyboard).
 *    return <output> == (int) escape status:
 *                             FALSE -> escape pressed
 *                             TRUE  -> escape not pressed
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   November 1990   Original Version   DOS Turbo C
 *E
 *************************************************************************/
int getcursorposition( int *x,
		       int *y,
		       int *button )
{
   char ch,ch2;
   int  xpos,ypos, ok = TRUE;

   if (kbhit()) {
      xpos = *x;
      ypos = *y;
      ch = getch();
      switch (ch) {
	 case 13:                         /* Enter key */
	    *button = LEFT_BUTTON_DOWN;
	    break;
	 case '8':
	    ypos -= 10;
	    break;
	 case '2':
	    ypos += 10;
	    break;
	 case '4':
	    xpos -= 10;
	    break;
	 case '6':
	    xpos += 10;
	    break;
	 case 27:
	    ok = FALSE;
	    break;
      }
      if (ch==0) {
	 ch2 = getch();
	 switch (ch2) {
	    case 72:
	       ypos--;
	       break;
	    case 80:
	       ypos++;
	       break;
	    case 75:
	       xpos--;
	       break;
	    case 77:
	       xpos++;
	       break;
	    case 59: /* F1 key */
	       *button = RIGHT_BUTTON_DOWN;
	       break;
	 }
      }
      setmouseposition(xpos,ypos);
      *x = xpos;
      *y = ypos;
   } else getmouseposition(x,y,button);
   return ok;
}



/*************************************************************************
 *
 *N  yes_no
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function displays a panel asking the user a yes or no
 *     question and returns the response.  Must be in graphics mode to call
 *     this function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    text     <input>==(char *) question string.
 *    return  <output>==(int) yes (TRUE) or no (FALSE).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                    DOS Turbo C
 *E
 *************************************************************************/
int yes_no( char *text )
{
   panel_type  panel;
   int         no_id = 27;
   int         yes_id = 13;
   int         x,y,width,height,maxheight,id=0;

   settextstyle( SMALL_FONT, HORIZ_DIR, 4 );

   width = 0;
   maxheight = 0;
   width = textwidth(text);
   maxheight = textheight(text);

   width = width + 50;
   if (width > getmaxx())
      width = getmaxx()-10;
   height = maxheight * 5;

   create_panel( &panel,width,height,menucolor,menubordercolor );

   y = maxheight;
   create_label( text,CENTER,y,SMALL_FONT,4,menutextcolor,&panel );
   y += maxheight;

   create_button( yes_id, "Yes",
		  3, height-textheight("Y")-6,
		  SMALL_FONT, 4,
		  menutextcolor, menucolor, menubordercolor, RELIEVED,
		  &panel );
   create_button( no_id, "No",
		  width-textwidth("No")-13,
		  height-textheight("No")-6,
		  SMALL_FONT, 4,
		  menutextcolor, menucolor, menubordercolor, RELIEVED,
		  &panel );

   get_display_position( &x, &y, panel.window );

   display_panel( &panel, x,y );

   id = process_panel(&panel, id);

   destroy_panel( &panel );
   close_panel( &panel );

   if (id == yes_id)
      return TRUE;
   else
      return FALSE;
}



/*************************************************************************
 *
 *N  printer_ready
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether or not the attached
 *     printer is ready.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    return <output> == (int) TRUE if ready, FALSE if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   November 1990   Original Version   DOS Turbo C
 *E
 *************************************************************************/
int printer_ready( void )
{
   union REGS reg;

   reg.h.ah = 2;
   reg.x.dx = 0;
   int86(0x17, &reg, &reg);

   /* this is not busy           not an io error      not out of paper */
   return (reg.h.ah & 0x80) && (!(reg.h.ah & 8)) && (!(reg.h.ah & 32));
}


/*************************************************************************
 *
 *N  printer_ok
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether the printer is ready and allows
 *     the user to keep retrying or cancel.  Must be in graphics mode to call
 *     this function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    return <output> == (int) TRUE if ready, FALSE if cancel.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   November 1990   Original Version   DOS Turbo C
 *E
 *************************************************************************/
int printer_ok( void )
{
   int retry;
   char *msg[] = {"Printer not ready"};

   while (!printer_ready()) {
      hidemousecursor();
      retry = displayerror(msg,1);
      showmousecursor();
      if (!retry) return FALSE;
   }
   return TRUE;
}




/*************************************************************************
 *
 *N  coprocessor_present
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether a math coprocessor chip is present
 *     on the current computer configuration.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    return <output> == (int) TRUE if present, FALSE if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1991                          DOS Turbo C
 *E
 *************************************************************************/
int coprocessor_present( void )
{
   int stat;

   stat = biosequip();

   /* If bit 1 of stat is on, a coprocessor is present */
   if (stat & 2)
      return TRUE;
   else
      return FALSE;
}
#endif




/*************************************************************************
 *
 *N  is_primitive
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether the given file name specifies a
 *     VPF primitive table (Edge, Face, Entity Node, or Text).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    name    <input> == (char *) file name to be tested.
 *    return <output> == (int) TRUE if primitive, FALSE if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
int is_primitive( char *name )
{
   strupr(name);
   if (strstr(name,"END")) return TRUE;
   if (strstr(name,"CND")) return TRUE;
   if (strstr(name,"EDG")) return TRUE;
   if (strstr(name,"FAC")) return TRUE;
   if (strstr(name,"TXT")) return TRUE;
   return FALSE;
}



/*************************************************************************
 *
 *N  is_simple_feature
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether the given file name specifies a
 *     VPF simple feature table (Point, Line, or Area feature table).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    name    <input> == (char *) file name to be tested.
 *    return <output> == (int) TRUE if simple feature, FALSE if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
int is_simple_feature( char *name )
{
   strupr(name);
   if (strstr(name,"PFT")) return TRUE;
   if (strstr(name,"LFT")) return TRUE;
   if (strstr(name,"AFT")) return TRUE;
   return FALSE;
}



/*************************************************************************
 *
 *N  is_complex_feature
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether the given file name specifies a
 *     VPF complex feature table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    name    <input> == (char *) file name to be tested.
 *    return <output> == (int) TRUE if complex feature, FALSE if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
int is_complex_feature( char *name )
{
   strupr(name);
   if (strstr(name,"CFT")) return TRUE;
   return FALSE;
}




/*************************************************************************
 *
 *N  is_feature
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether the given file name specifies a
 *     VPF feature table (Point, Line, Area, or Complex feature table).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    name    <input> == (char *) file name to be tested.
 *    return <output> == (int) TRUE if feature, FALSE if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
int is_feature( char *name )
{
   strupr(name);
   if (strstr(name,"PFT")) return TRUE;
   if (strstr(name,"LFT")) return TRUE;
   if (strstr(name,"AFT")) return TRUE;
   if (strstr(name,"TFT")) return TRUE;
   if (strstr(name,"CFT")) return TRUE;
   return FALSE;
}




/*************************************************************************
 *
 *N  feature_type
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the type of feature the given table name
 *     refers to (POINT, LINE, AREA, ANNO, or COMPLEX_FEATURE).  If the table
 *     name does not specify a feature type, the function returns FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    name    <input> == (char *) file name to be tested.
 *    return <output> == (int) feature type (FALSE if invalid name).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
int feature_type( char *name )
{
   strupr(name);
   if (strstr(name,"PFT")) return VPF_POINT;
   if (strstr(name,"LFT")) return VPF_LINE;
   if (strstr(name,"AFT")) return VPF_AREA;
   if (strstr(name,"TXT")) return VPF_ANNO;
   if (strstr(name,"TFT")) return VPF_ANNO;
   if (strstr(name,"CFT")) return VPF_COMPLEX_FEATURE;
   return FALSE;
}




/*************************************************************************
 *
 *N  is_join
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether the given file name specifies a
 *     VPF join table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    name    <input> == (char *) file name to be tested.
 *    return <output> == (int) TRUE if join, FALSE if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
int is_join( char *name )
{
   char *ptr;

   strupr(name);
   ptr = strstr(name,".");
   if (strstr(ptr,"JT")) return TRUE;
   return FALSE;
}



/*************************************************************************
 *
 *N  primitive_class
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the feature type the given primitive table
 *     name refers to (POINT, LINE, AREA, or ANNO).  If the table
 *     name is not a valid primitive table name, the function returns NULL.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    name    <input> == (char *) file name to be tested.
 *    return <output> == (int) feature type (NULL if invalid primitive
 *                             name).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
int primitive_class( char *name )
{
   strupr(name);
   if (strstr(name,"END")) return VPF_ENTITY_NODE;
   if (strstr(name,"CND")) return VPF_CONNECTED_NODE;
   if (strstr(name,"EDG")) return VPF_EDGE;
   if (strstr(name,"FAC")) return VPF_FACE;
   if (strstr(name,"TXT")) return VPF_TEXT;
#ifdef __MSDOS__
   return NULL;
#else
   return 0;
#endif
}

int ossim_strcasecmp(const char *s1, const char *s2)
{
   size_t s1_size;
   size_t s2_size;
   unsigned int i;

   /* Check for one string being null, one not. */
   if (!s1 && s2)
   {
      return -1;
   }
   else if (s1 && !s2)
   {
      return 1;
   }

   /* Check for both strings being null.  Consider this equal? */
   if (!s1 && !s2)
   {
      return 0;
   }

   /* Check for size differences. */
   s1_size = strlen(s1);
   s2_size = strlen(s2);
   if (s1_size < s2_size)
   {
      return -1;
   }
   else if (s1_size > s2_size)
   {
      return 1;
   }

   /* Check the strings. */
   for (i=0; i<s1_size; i++)
   {
      if ( toupper(s1[i]) != toupper(s2[i]) )
      {
         return ( (toupper(s1[i]) < toupper(s2[i]) ) ? -1 : 1);
      }
   }

      /* Equal */   
   return 0;
}

int ossim_strncasecmp(const char *s1, const char *s2, unsigned int n)
{
   size_t s1_size;
   size_t s2_size;
   unsigned int i;

   /* Check for one string being null, one not. */
   if (!s1 && s2)
   {
      return -1;
   }
   else if (s1 && !s2)
   {
      return 1;
   }

   /* Check for both strings being null.  Consider this equal? */
   if (!s1 && !s2)
   {
      return 0;
   }

   /* Check for size. */
   if (n == 0)
   {
      return 0;
   }
   
   s1_size = strlen(s1);
   s2_size = strlen(s2);
   if (n > s1_size)
   {
      return -1;
   }
   else if (n > s2_size )
   {
      return 1;
   }

   /* Check the strings. */
   for (i=0; i<n; i++)
   {
      if ( toupper(s1[i]) != toupper(s2[i]) )
      {
         return ( (toupper(s1[i]) < toupper(s2[i]) ) ? -1 : 1);
      }
   }

      /* Equal */   
   return 0;
}
