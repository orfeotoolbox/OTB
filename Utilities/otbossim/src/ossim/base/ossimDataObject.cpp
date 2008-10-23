//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimDataObject.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/base/ossimDataObject.h>
#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimScalarTypeLut.h>

RTTI_DEF1(ossimDataObject, "ossimDataObject", ossimObject)
   
ossimDataObject::ossimDataObject(ossimSource* source,
                                 ossimDataObjectStatus status)
   :
      theOwner(source),
      theDataObjectStatus(status)
{
}

ossimDataObject::ossimDataObject(const ossimDataObject& rhs)
   : ossimObject(),
     theOwner(NULL),
     theDataObjectStatus(rhs.theDataObjectStatus)
{
}

ossimDataObject::~ossimDataObject()
{
}

void ossimDataObject::setOwner(ossimSource* aSource)
{
   theOwner = aSource;
}

ossimSource* ossimDataObject::getOwner()
{
   return theOwner;
}

const ossimSource* ossimDataObject::getOwner() const
{
   return theOwner;
}

void ossimDataObject::setDataObjectStatus(ossimDataObjectStatus status) const
{
   theDataObjectStatus = status;
}

ossimDataObjectStatus ossimDataObject::getDataObjectStatus()const
{
   return theDataObjectStatus;
}

ossimString ossimDataObject::getDataObjectStatusString() const
{
   ossimString s;
   
   switch (theDataObjectStatus)
   {
      case OSSIM_NULL:
         s = "OSSIM_NULL";
         break;

      case OSSIM_EMPTY:
         s = "OSSIM_EMPTY";
         break;

      case OSSIM_PARTIAL:
         s = "OSSIM_PARTIAL";
         break;

      case OSSIM_FULL:
         s = "OSSIM_FULL";
         break;
         
      case OSSIM_STATUS_UNKNOWN:
      default:
         s = "OSSIM_STATUS_UNKNOWN";
         break;
   }
   
   return s;
}

ossim_uint32 ossimDataObject::getObjectSizeInBytes()const
{
   return sizeof(theOwner);
}

bool ossimDataObject::isInitialize()const
{
   return (getDataObjectStatus()!=OSSIM_NULL);
}

bool ossimDataObject::operator!=(const ossimDataObject& rhs) const
{
   return ( theOwner               != rhs.theOwner ||
            theDataObjectStatus    != rhs.theDataObjectStatus );
}

bool ossimDataObject::operator==(const ossimDataObject& rhs) const
{
   return (theOwner            == rhs.theOwner &&
           theDataObjectStatus == rhs.theDataObjectStatus);
}

void ossimDataObject::assign(const ossimDataObject* rhs)
{
   if(rhs != this)
   {
      theOwner               = rhs->theOwner;
      theDataObjectStatus     = rhs->theDataObjectStatus;
   }
}

const ossimDataObject* ossimDataObject::operator=(const ossimDataObject* rhs)
{
   assign(rhs);
   return this;
}

const ossimDataObject& ossimDataObject::operator=(const ossimDataObject& rhs)
{
   assign(&rhs);
   return *this;
}

std::ostream& ossimDataObject::print(ostream& out) const
{
   out << "ossimDataObject::print:";
   
   if (theOwner)
   {
      out << "\ntheOwner->getClassName():  "
          << theOwner->getClassName()<<endl;
   }
   else
   {
      out << "\ntheOwner is null"
          <<endl;
   }
   out << "theDataObjectStatus: " << getDataObjectStatusString()
       << endl;

   return out;
}
