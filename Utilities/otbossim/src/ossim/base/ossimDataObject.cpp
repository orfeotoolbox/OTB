//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimDataObject.cpp 19931 2011-08-10 11:53:25Z gpotts $
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
     theOwner(0),
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
   if (this != &rhs)
   {
      theOwner            = rhs.theOwner;
      theDataObjectStatus = rhs.theDataObjectStatus; 
   }
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

bool ossimDataObject::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   kwl.add(prefix, "data_object_status", getDataObjectStatusString(), true);
   return ossimObject::saveState(kwl, prefix);
}

bool ossimDataObject::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   ossimString data_object_status = kwl.find(prefix, "data_object_status");
   if(!data_object_status.empty())
   {
      data_object_status = data_object_status.upcase();
      if(data_object_status == "OSSIM_FULL")
      {
         theDataObjectStatus = OSSIM_FULL;
      }
      else if(data_object_status == "OSSIM_PARTIAL")
      {
         theDataObjectStatus = OSSIM_PARTIAL;
      }
      else if(data_object_status == "OSSIM_EMPTY")
      {
         theDataObjectStatus = OSSIM_EMPTY;
      }
      else if(data_object_status == "OSSIM_STATUS_UNKNOWN")
      {
         theDataObjectStatus = OSSIM_STATUS_UNKNOWN;
      }
   }
   
   return ossimObject::loadState(kwl, prefix);
}                     
