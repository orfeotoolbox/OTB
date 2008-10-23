//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
//         Orginally written by Jamie Moyers (jmoyers@geeks.com)
//         Adapted from the package KDEM.
// Description: This class provides some simple utilities for DEMs.
//
//********************************************************************
// $Id: ossimDemUtil.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <stdlib.h>
#include <iostream>

#include <ossim/support_data/ossimDemUtil.h>

using namespace std;

bool
ossimDemUtil::getRecord(istream& s, string& strbuf, long reclength)
{
   char* buf = new char[reclength + 1];

   bool flag = ossimDemUtil::getRecord(s,buf,reclength);
   if (flag == true)
      strbuf = buf;

   delete [] buf;

   return flag;
}

bool
ossimDemUtil::getRecord(istream& s, char* buf, long reclength)
{
   // buf is assumed to be at least reclength+1 in size.

   if (!s)
      return false;

   long curpos = 0;
   buf[curpos] = s.get();
   while ((buf[curpos] != EOF) && 
          (buf[curpos] != '\n') &&
          (curpos < reclength-1))
   {
      curpos++;
      buf[curpos] = s.get();
   }
   buf[curpos] = '\0';

   if (s.peek() == '\n')
      s.get();

   return true;

}

bool
ossimDemUtil::getDouble(string const& strbuf,
                   long const startpos,
                   long const width,
                   double& val)
{
   if ((startpos + width - 1) > (long)(strbuf.length()))
      return false;

   // Convert FORTRAN 'D' exponent indicator to 'E'.
   string tempbuf(strbuf.substr(startpos,width));
   for (unsigned int i = 0; i < tempbuf.length(); i++)
      if (tempbuf[i] == 'D')
         tempbuf[i] = 'E';

   val = atof(tempbuf.c_str());
   return true;
}
