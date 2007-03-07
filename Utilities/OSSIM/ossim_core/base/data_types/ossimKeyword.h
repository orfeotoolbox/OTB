//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Ken Melero
// Description: This class defines a keyword.
//              NOTE:  The keywordlist class cleans up all allocated
//                     memory from instantiating a keyword.
// 
//********************************************************************
// $Id: ossimKeyword.h,v 1.5 2005/10/17 18:37:16 gpotts Exp $
#ifndef ossimKeyword_HEADER
#define ossimKeyword_HEADER

#include <base/data_types/ossimString.h>

class OSSIMDLLEXPORT ossimKeyword
{
public:
   ossimKeyword();
   ossimKeyword(const char* key, const char* description);
   ossimKeyword(const char* key, const char* description, const char* value);
   ossimKeyword(const ossimKeyword& src);
   ~ossimKeyword();

   const ossimKeyword& operator=(const ossimKeyword& kw);
   operator const char*() const;

   const char* key()               const;
   const char* description()       const;
   const char* value()             const;
   ossimString keyString()         const;
   ossimString descriptionString() const;
   ossimString valueString()       const;
   
   void setValue(const char* val);
   void setValue(const ossimString& val);
   void setDescription(const char* desc);
   void setDescription(const ossimString& desc);
   

   friend OSSIM_DLL std::ostream& operator<<(std::ostream& os, const ossimKeyword& kw);

private:
   ossimString theKey;
   ossimString theDesc;
   ossimString theValue;
};

#endif
