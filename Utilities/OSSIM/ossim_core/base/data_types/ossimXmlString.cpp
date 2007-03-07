//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License 
// as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU Library General Public License
// along with this library. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Ken Melero (kmelero@remotesensing.org)
// Description: This class provides manipulation of filenames.
//
//*************************************************************************
// $Id: ossimXmlString.cpp,v 1.1 2002/06/12 20:39:30 gpotts Exp $
#include "ossimXmlString.h"

const ossimXmlString& ossimXmlString::assign(const ossimString& s)
{
   ossimString result = s;
   
   size_type pos = 0;

   pos = result.find("&");
   // do ampersand first
   //
   while(pos != npos)
   {
      ossimString s1(result.begin() + pos,
                     result.begin()+pos+5);
      ossimString s2(result.begin() + pos,
                     result.begin()+pos+4);
      ossimString s3(result.begin() + pos,
                     result.begin()+pos+3);
      
      if((s2 != "&amp")&&
         (s1 != "&quot")&&
         (s3 != "&gt")&&
         (s3 != "&lt")&&
         (s1 != "&apos"))
      {
         result.replace(pos, 1, "&amp");
      }
      
      pos = result.find("&", pos+4);
   }

   result = result.substitute("\"",
                              "&quot",
                              true);
   result = result.substitute("<",
                              "&lt",
                              true);
   result = result.substitute(">",
                              "&gt",
                              true);
   result = result.substitute("'",
                              "&apos",
                              true);
                     
   *((ossimString*)this) = result;

   return *this;
}
