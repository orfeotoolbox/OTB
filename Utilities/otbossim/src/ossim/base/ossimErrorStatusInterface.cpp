//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: David Burken  (dburken@imagelinks.com)
//
// Description:
// 
// Contains class definition for common error interfaces.
//
//*************************************************************************
// $Id: ossimErrorStatusInterface.cpp 22149 2013-02-11 21:36:10Z dburken $

#include <iostream>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimString.h>

RTTI_DEF(ossimErrorStatusInterface, "ossimErrorStatusInterface");

ossimErrorStatusInterface::ossimErrorStatusInterface()
   :
   theErrorStatus(ossimErrorCodes::OSSIM_OK)
{}
   
ossimErrorStatusInterface::~ossimErrorStatusInterface()
{}

ossimErrorCode ossimErrorStatusInterface::getErrorStatus() const
{
   return theErrorStatus;
}

ossimString ossimErrorStatusInterface::getErrorStatusString() const
{
   return ossimErrorCodes::instance()->getErrorString(theErrorStatus);
}

void ossimErrorStatusInterface::setErrorStatus(ossimErrorCode error_status) const
{
   theErrorStatus = error_status;
}

void  ossimErrorStatusInterface::setErrorStatus() const
{
   theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
}

void  ossimErrorStatusInterface::clearErrorStatus() const
{
   theErrorStatus = ossimErrorCodes::OSSIM_OK;
}

bool ossimErrorStatusInterface::hasError() const
{
   return ( theErrorStatus != ossimErrorCodes::OSSIM_OK );
}

std::ostream& ossimErrorStatusInterface::print(std::ostream& out) const
{
   out << "ossimErrorStatusInterface::print"
       << "\ntheErrorStatus:         " << theErrorStatus
       << "\ntheErrorStatus string:  " << getErrorStatusString()
       << std::endl;
   return out;
}

std::ostream& operator<<(std::ostream& out,
                         const ossimErrorStatusInterface& obj)
{
   return obj.print(out);
}
