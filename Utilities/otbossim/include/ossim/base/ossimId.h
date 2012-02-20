//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimId.h 20314 2011-12-01 21:46:37Z oscarkramer $
#ifndef ossimId_HEADER
#define ossimId_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>

class OSSIM_DLL ossimId
{
public:
   friend OSSIM_DLL std::ostream& operator <<(std::ostream &out, const ossimId& data);
   
   explicit ossimId(ossim_int64 id = 0)
      :theId(id)
      {}
   virtual ~ossimId(){}

   virtual void print(std::ostream& out)const;

   void setId(ossim_int64 id){theId = id;}
   ossim_int64 getId()const{return theId;}

   bool operator ==(const ossimId& rhs)const
   {
      return (theId == rhs.theId);
   }
   bool operator !=(const ossimId& rhs)const
   {
      return (theId != rhs.theId);
   }
  
   bool operator == (ossim_int64 rhs) const
   {
     return (theId == rhs);
   }
   bool operator != (ossim_int64 rhs) const
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

   bool operator <(ossim_int64 rhs)const
   {
      return (theId < rhs);
   }
   bool operator >(ossim_int64 rhs)const
   {
      return (theId > rhs);
   }
   bool operator <=(ossim_int64 rhs)const
   {
      return (theId <= rhs);
   }
   bool operator >=(ossim_int64 rhs)const
   {
      return (theId >= rhs);
   }

   static const ossim_int64 INVALID_ID;
protected:
   ossim_int64 theId;
};

#endif
