//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
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
// Author: David Burken  (dburken@imagelinks.com)
//
// Description:
// 
// Contains class definition for common error interfaces.
//
//*************************************************************************
// $Id: ossimErrorStatusInterface.cpp,v 1.3 2005/10/17 18:37:16 gpotts Exp $

#include "ossimErrorStatusInterface.h"
#include "base/data_types/ossimString.h"

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
