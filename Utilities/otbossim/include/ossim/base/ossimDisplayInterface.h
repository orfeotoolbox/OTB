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
// $Id: ossimDisplayInterface.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimDisplayInterface_HEADER
#define ossimDisplayInterface_HEADER
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimString.h>

class OSSIMDLLEXPORT ossimDisplayInterface
{
public:
   ossimDisplayInterface();

   virtual ~ossimDisplayInterface();

   /*!
    * Returns the display back to the caller
    */
   virtual void* getDisplayDevice()=0;

   virtual void setTitle(const ossimString& title)=0;
   virtual void getTitle(ossimString& title)const=0;
   virtual ossimString getTitle()const;
   virtual void close()=0;
   virtual bool isOpen() const=0;
   virtual bool open()=0;
   virtual bool isMinimized()const=0;

TYPE_DATA
};

#endif
