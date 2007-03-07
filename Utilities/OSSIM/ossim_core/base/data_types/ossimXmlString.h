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
// $Id: ossimXmlString.h,v 1.3 2005/10/17 18:37:16 gpotts Exp $

#ifndef ossimXmlString_HEADER
#define ossimXmlString_HEADER

#include "base/common/ossimConstants.h"
#include "ossimString.h"

class OSSIM_DLL ossimXmlString : public ossimString
{
public:
   ossimXmlString()
      :
         ossimString()
      {}
   ossimXmlString(const ossimXmlString& src)
      :
         ossimString(src)
      {}
   ossimXmlString(const ossimString& src)
      :
         ossimString("")
      {
         assign(src);

      }
   ossimXmlString(const char* src)
      :
         ossimString("")
      {
         assign(src);
      }
   
   template <class Iter>
   ossimXmlString(Iter s, Iter e)
      :ossimString("")
      {
         assign(ossimString(s, e));
      }

   const ossimXmlString& operator =(const char* s)
      {
         return assign(ossimString(s));
      }
   	  const ossimXmlString& operator =(const ossimString& s)
      {
         return assign(ossimString(s));
      }
	  const ossimXmlString& operator =(const std::string& s)
      {
         return assign(ossimString(s));
      }

   const ossimXmlString& assign(const ossimString& s);
};

#endif
