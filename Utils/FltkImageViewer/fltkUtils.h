/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkUtils.h,v $
  Language:  C++
  Date:      $Date: 2002/08/05 22:42:20 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef FLTKUTILS_H
#define FLTKUTILS_H

/**
 * Display an FLTK window that has n text fields. Each field
 * will be labeled by the corresponding mesg.  Each field will
 * have the default value given by the corresponding buffer.
 * Results will be stored in the corresponding buffer.
 * Returns 0 if cancel is hit. Returns 1 if okay is hit.
 **/
extern int ifuGetString(int n, const char **mesg, char **buffer);

/**
 * Display an FLTK window that has one text field. That field
 * will be labeled by the mesg.  That field will
 * have the default value given by the buffer.
 * Result will be stored in the buffer.
 * Returns 0 if cancel is hit. 
 * Returns 1 if okay is hit.
 **/
extern int ifuGetString(const char *mesg, char *buffer);

/**
 * Display an FLTK window that has 2 text fields. Each field
 * will be labeled by the corresponding mesg.  Each field will
 * have the default value given by the corresponding buffer.
 * Results will be stored in the corresponding buffer.
 * Returns 0 if cancel is hit. Returns 1 if okay is hit.
 **/
extern int ifuGetString(const char *mesg1, char *buffer1,
                        const char *mesg2, char *buffer2);

/**
 * Display an FLTK window that has 3 text fields. Each field
 * will be labeled by the corresponding mesg.  Each field will
 * have the default value given by the corresponding buffer.
 * Results will be stored in the corresponding buffer.
 * Returns 0 if cancel is hit. Returns 1 if okay is hit.
 **/
extern int ifuGetString(const char *mesg1, char *buffer1,
                        const char *mesg2, char *buffer2,
                        const char *mesg3, char *buffer3);

/**
 * Display an FLTK window that has 4 text fields. Each field
 * will be labeled by the corresponding mesg.  Each field will
 * have the default value given by the corresponding buffer.
 * Results will be stored in the corresponding buffer.
 * Returns 0 if cancel is hit. Returns 1 if okay is hit.
 **/
extern int ifuGetString(const char *mesg1, char *buffer1,
                        const char *mesg2, char *buffer2,
                        const char *mesg3, char *buffer3,
                        const char *mesg4, char *buffer4);

/**
 * Display an FLTK window that has a large text box. That box
 * will be labeled by the mesg.  That box will
 * have the default value given by the buffer.
 * Results will be stored in the buffer.
 * Returns 0 if cancel is hit. 
 * Returns 1 if okay is hit.
 **/
extern int ifuGetText(const char *mesg, char *buffer);

/**
 * Display an FLTK window that has a large text buffer. The
 * textg given will be displayed in that buffer.
 * Returns 0 if cancel is hit. Returns 1 if okay is hit.
 **/
extern int ifuShowText(const char *mesg);

#endif

