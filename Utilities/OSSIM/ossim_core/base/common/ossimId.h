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
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimId.h,v 1.5 2002/11/25 18:45:47 gpotts Exp $
#ifndef ossimId_HEADER
#define ossimId_HEADER
#include <iostream>
using namespace std;
#include "ossimConstants.h"

class OSSIMDLLEXPORT ossimId
{
public:
   friend ostream& operator <<(ostream &out,
                               const ossimId& data)
      {
         data.print(out);
         return out;
      }
   
   explicit ossimId(long id = 0)
      :theId(id)
      {}
   virtual ~ossimId(){}

   virtual void print(ostream& out)const
      {
         out << "id: " << theId;
      }
   void setId(long id){theId = id;}
   long getId()const{return theId;}

   bool operator ==(const ossimId& rhs)const
      {
         return (theId == rhs.theId);
      }
   bool operator !=(const ossimId& rhs)const
      {
         return (theId != rhs.theId);
      }
  
   bool operator == (int rhs)
   {
     return (theId == rhs);
   }
   bool operator != (int rhs)
   {
     return (theId != rhs);
   }

   bool operator <(const ossimId& rhs)const
      {
         return (theId < rhs.theId);
      }
   bool operator <=(const ossimId& rhs)const
      {
         return (theId <= rhs.theId);
      }
   bool operator >(const ossimId& rhs)const
      {
         return (theId > rhs.theId);
      }
   bool operator >=(const ossimId& rhs)const
      {
         return (theId >= rhs.theId);
      }

   bool operator <(int rhs)const
      {
         return (theId < rhs);
      }
   bool operator >(int rhs)const
      {
         return (theId > rhs);
      }
   bool operator <=(int rhs)const
      {
         return (theId <= rhs);
      }
   bool operator >=(int rhs)const
      {
         return (theId >= rhs);
      }

   static const long INVALID_ID;
protected:
   long theId;
};

#endif
