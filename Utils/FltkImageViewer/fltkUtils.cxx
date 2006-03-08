
/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkUtils.cxx,v $
  Language:  C++
  Date:      $Date: 2005/02/03 14:51:47 $
  Version:   $Revision: 1.8 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Multiline_Output.H>

#include "fltkUtils.h"
#include <string>
#include <iostream>

/*
 *
 */
int ifuGetString(int n, const char **mesg, char **buffer)
  {
  int i;
  int l = strlen(mesg[0]);
  for(i=1; i<n; i++)
    if(strlen(mesg[i])>(unsigned int)l)
      l = strlen(mesg[i]);

  Fl_Window window(270+l*8, 10+(10+25)*(n+1));
  Fl_Input *input[100];
  for(i=0; i<n; i++)
    {
    input[i] = new(Fl_Input)(10+l*8, 10+(25+10)*i, 250, 25, mesg[i]);
    input[i]->value(buffer[i]);
    }
  Fl_Button cancel((int)(0.333*(270+l*8)), 10+(25+10)*n, 80, 25, "Cancel");
  Fl_Return_Button ok((int)(0.6666*(270+l*8)), 10+(25+10)*n, 80, 25, "OK");
  window.hotspot(&ok);
  window.end();
  window.set_modal();
  window.show();
  for (;;)
    {
    Fl::wait();
    Fl_Widget *o;
    while ((o = Fl::readqueue()))
      {
      if (o == &ok)
        {
        for(i=0; i<n; i++)
          strcpy(buffer[i],input[i]->value());
        return 1;
        }
      else
        {
        if (o == &cancel || o == &window)
          {
          return 0;
          }
        }
      }
    }
  }


/*
 *
 */
int ifuGetString(const char *mesg, char *buffer)
  {
  int l = strlen(mesg);
  Fl_Window window(270+l*8,75);
  Fl_Input input(10+l*8, 10, 250, 25, mesg);
  input.value(buffer);
  Fl_Button cancel(60, 40, 80, 25, "Cancel");
  Fl_Return_Button ok(150, 40, 80, 25, "OK");
  window.hotspot(&ok);
  window.end();
  window.set_modal();
  window.show();
  for (;;)
    {
    Fl::wait();
    Fl_Widget *o;
    while ((o = Fl::readqueue()))
      {
      if (o == &ok)
        {
        strcpy(buffer,input.value());
        return 1;
        }
      else
        {
        if (o == &cancel || o == &window)
          {
          return 0;
          }
        }
      }
    }
  }

/*
 *
 */
int ifuGetString(const char *mesg1, char *buffer1, const char *mesg2, char *buffer2)
  {
  int l = strlen(mesg1);
  if((int)strlen(mesg2)>l)
     l = strlen(mesg2);
  Fl_Window window(270+l*8,10+25+10+25+10+35);
  Fl_Input input1(10+l*8, 10, 250, 25, mesg1);
  input1.value(buffer1);
  Fl_Input input2(10+l*8, 10+25+10, 250, 25, mesg2);
  input2.value(buffer2);
  Fl_Button cancel(60, 10+25+10+25+10, 80, 25, "Cancel");
  Fl_Return_Button ok(150, 10+25+10+25+10, 80, 25, "OK");
  window.hotspot(&ok);
  window.end();
  window.set_modal();
  window.show();
  for (;;)
    {
    Fl::wait();
    Fl_Widget *o;
    while ((o = Fl::readqueue()))
      {
      if (o == &ok)
        {
        strcpy(buffer1,input1.value());
        strcpy(buffer2,input2.value());
        return 1;
        }
      else
        {
        if (o == &cancel || o == &window)
          {
          return 0;
          }
        }
      }
    }
  }

/*
 *
 */
int ifuGetString(const char *mesg1, char *buffer1, const char *mesg2, char *buffer2,
     const char *mesg3, char *buffer3)
  {
  int l = strlen(mesg1);
  if((int)strlen(mesg2)>l)
    {
    l = strlen(mesg2);
    }
  if((int)strlen(mesg3)>l)
    {
    l = strlen(mesg3);
    }

  Fl_Window window(270+l*8,10+25+10+25+10+25+10+35);
  Fl_Input input1(10+l*8, 10, 250, 25, mesg1);
  input1.value(buffer1);
  Fl_Input input2(10+l*8, 10+25+10, 250, 25, mesg2);
  input2.value(buffer2);
  Fl_Input input3(10+l*8, 10+25+10+25+10, 250, 25, mesg3);
  input3.value(buffer3);
  Fl_Button cancel(60, 10+25+10+25+10+25+10, 80, 25, "Cancel");
  Fl_Return_Button ok(150, 10+25+10+25+10+25+10, 80, 25, "OK");
  window.hotspot(&ok);
  window.end();
  window.set_modal();
  window.show();
  for (;;)
    {
    Fl::wait();
    Fl_Widget *o;
    while ((o = Fl::readqueue()))
      {
      if (o == &ok)
        {
        strcpy(buffer1,input1.value());
        strcpy(buffer2,input2.value());
        strcpy(buffer3,input3.value());
        return 1;
        }
      else
        {
        if (o == &cancel || o == &window)
          {
          return 0;
          }
        }
      }
    }
  }

/*
 *
 */
int ifuGetString(const char *mesg1, char *buffer1, const char *mesg2, char *buffer2,
     const char *mesg3, char *buffer3, const char *mesg4, char *buffer4)
  {
  int l = strlen(mesg1);
  if((int)strlen(mesg2)>l)
    {
    l = strlen(mesg2);
    }
  if((int)strlen(mesg3)>l)
    {
    l = strlen(mesg3);
    }
  if((int)strlen(mesg4)>l)
    {
    l = strlen(mesg4);
    }

  Fl_Window window(270+l*8,10+25+10+25+10+25+10+25+10+35);
  Fl_Input input1(10+l*8, 10, 250, 25, mesg1);
  input1.value(buffer1);
  Fl_Input input2(10+l*8, 10+25+10, 250, 25, mesg2);
  input2.value(buffer2);
  Fl_Input input3(10+l*8, 10+25+10+25+10, 250, 25, mesg3);
  input3.value(buffer3);
  Fl_Input input4(10+l*8, 10+25+10+25+10+25+10, 250, 25, mesg4);
  input4.value(buffer4);
  Fl_Button cancel(60, 10+25+10+25+10+25+10+25+10, 80, 25, "Cancel");
  Fl_Return_Button ok(150, 10+25+10+25+10+25+10+25+10, 80, 25, "OK");
  window.hotspot(&ok);
  window.end();
  window.set_modal();
  window.show();
  for (;;)
    {
    Fl::wait();
    Fl_Widget *o;
    while ((o = Fl::readqueue()))
      {
      if (o == &ok)
        {
        strcpy(buffer1,input1.value());
        strcpy(buffer2,input2.value());
        strcpy(buffer3,input3.value());
        strcpy(buffer4,input4.value());
        return 1;
        }
      else
        {
        if (o == &cancel || o == &window)
          {
          return 0;
          }
        }
      }
    }
  }

/*
 *
 */
int ifuGetText(const char *mesg, char *buffer)
  {
  Fl_Window window(520, 560);
  Fl_Multiline_Input input(10, 10, 500, 500, mesg);
  input.value(buffer);
  Fl_Button cancel(360, 540, 80, 25, "Cancel");
  Fl_Return_Button ok(450, 540, 80, 25, "OK");
  window.hotspot(&ok);
  window.end();
  window.set_modal();
  window.show();
  for (;;)
    {
    Fl::wait();
    Fl_Widget *o;
    while ((o = Fl::readqueue()))
      {
      if (o == &ok)
        {
        strcpy(buffer,input.value());
        return 1;
        }
      else
        {
        if (o == &cancel || o == &window)
          {
          return 0;
          }
        }
      }
    }
  }

/*
 *
 */
int ifuShowText(const char *mesg)
  {
  Fl_Window window(532,577);
  Fl_Multiline_Output txt(10, 10, 512, 512);
  txt.value(mesg);
  Fl_Return_Button ok(150, 540, 80, 25, "OK");
  window.hotspot(&ok);
  window.end();
  window.set_modal();
  window.show();
  for (;;)
    {
    Fl::wait();
    Fl_Widget *o;
    while ((o = Fl::readqueue()))
      {
      if (o == &ok || o == &window)
        {
        return 1;
        }
      }
    }
  }
