//********************************************************************
// Copyright (C) 2004 Intelligence-data, Inc.  All rights reserved.
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
// Author:  David Burken
//
//**********************************************************************
// $Id: ossimPoolObject.h,v 1.1 2004/04/20 21:26:05 dburken Exp $
#ifndef ossimPoolObject_HEADER
#define ossimPoolObject_HEADER


/**
 * Templated container for an object pointer and a flag.
 */
template <class T>
class ossimPoolObject
{
 public:
   ~ossimPoolObject()
   {
      if(theObjectPtr)
      {
         cout << "deleting obj:  " << (hex) << theObjectPtr << endl;
         delete theObjectPtr;
         theObjectPtr = NULL;
      }
      theAvailableFlag = false;  
   }


   bool theAvailableFlag;
   T*   theObjectPtr;
};
#endif /* #ifndef ossimPoolObject_HEADER */
