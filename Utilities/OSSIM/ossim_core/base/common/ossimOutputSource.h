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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Contains class declaration for ossimImageWriter
//*******************************************************************
//  $Id: ossimOutputSource.h,v 1.11 2003/05/13 11:31:01 dburken Exp $
//
#ifndef ossimOutputSource_HEADER
#define ossimOutputSource_HEADER
#include "base/common/ossimConstants.h"
#include "base/common/ossimSource.h"
#include "base/common/ossimCommon.h"

class OSSIMDLLEXPORT ossimOutputSource : public ossimSource
{
public:
   ossimOutputSource(ossimObject* owner=NULL)
      :ossimSource(owner)
      {}
   ossimOutputSource(ossimObject* owner,
                     ossim_uint32 inputListSize,
                     ossim_uint32 outputListSize,
                     bool inputListIsFixedFlag=true,
                     bool outputListIsFixedFlag=true)
      :ossimSource(owner,
                   inputListSize,
                   outputListSize,
                   inputListIsFixedFlag,
                  outputListIsFixedFlag)
      {
      }

   virtual ~ossimOutputSource(){}
   
   virtual bool isOpen()const = 0;
   
   /*!
    * Allows a generic interface to open a consumer for writing.  If
    * you are a display writer then you would probably show the window or
    * init the device context for writing.
    */
   virtual bool open()=0;

   /*!
    * Will set the output name and open the output device.
    */
   virtual bool open(const ossimString& outputName)
      {
         setOutputName(outputName);
         return open();
      }
   
   /*!
    * This is a generic interface to allow one to set the output name of
    * the writer.  If its a Visual display this could be used to
    * set the title.  If this is a file output then this could be a
    * directory or a file name.
    */
  virtual void setOutputName(const ossimString& outputName)
  {
    theOutputName = outputName;
  }

  virtual const ossimString& getOutputName()
  {
    return theOutputName;
  }
   /*!
    * Generic interface to allow one to close a consumer.  For displays
    * this might minimize the window and free up internal pointers for
    * file writers it will make sure the file stream is closed and flushed.
    */
   virtual void close()=0;
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

protected:
  ossimString theOutputName;


TYPE_DATA
};

#endif
