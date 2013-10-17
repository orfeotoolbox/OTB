//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: David Burken
//
// Description:
// 
// Contains class declaration for common error interfaces.
//
//*************************************************************************
// $Id: ossimErrorStatusInterface.h 22149 2013-02-11 21:36:10Z dburken $

#ifndef ossimErrorStatusInterface_HEADER
#define ossimErrorStatusInterface_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimErrorCodes.h>

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

   /*! @return true if theErrorStatus != ossimErrorCodes::OSSIM_OK. */
   bool hasError() const;

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
