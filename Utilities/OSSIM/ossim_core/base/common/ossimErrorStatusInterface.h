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
// Contains class declaration for common error interfaces.
//
//*************************************************************************
// $Id: ossimErrorStatusInterface.h,v 1.7 2004/05/21 21:49:19 dburken Exp $

#ifndef ossimErrorStatusInterface_HEADER
#define ossimErrorStatusInterface_HEADER

#include <iostream>

#include "ossimConstants.h"
#include "base/common/ossimRtti.h"
#include "base/common/ossimErrorCodes.h"

/*!
 *  class ossimErrorStatusInterface
 *  Provides common interfaces for error handling.
 *
 *  Note: For error codes and associated strings use the ossimErrorCodes class.
 */
class OSSIMDLLEXPORT ossimErrorStatusInterface
{
public:
   ossimErrorStatusInterface();
   
   virtual ~ossimErrorStatusInterface();

   /*!  return theErrorStatus */
   virtual ossimErrorCode getErrorStatus() const;

   /*!  return theErrorStatus as an ossimString */
   virtual ossimString getErrorStatusString() const;

   /*! Sets theErrorStatus to error_status. */
   virtual void  setErrorStatus(ossimErrorCode error_status) const;

   /*! Set theErrorStatus to ossimErrorCodes::OSSIM_ERROR. */
   virtual void  setErrorStatus() const;

   /*! Clears theErrorStatus by setting to ossimErrorCodes::OSSIM_OK or 0. */
   virtual void  clearErrorStatus() const;

   /**
    * Outputs theErrorStatus as an ossimErrorCode and an ossimString.
    * 
    * @return std::ostream&
    *
    * @note  Derived classes should only have to implement a virtual print,
    *        not an operator<< funtion as it's implemented here.
    */
   virtual std::ostream& print(std::ostream& out) const;

   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& out,
                                                  const ossimErrorStatusInterface& obj);
   
protected:
   mutable ossimErrorCode theErrorStatus;
   
TYPE_DATA
};

#endif
