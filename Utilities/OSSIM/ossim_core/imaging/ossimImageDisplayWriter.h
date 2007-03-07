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
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageWriter
//*******************************************************************
//  $Id: ossimImageDisplayWriter.h,v 1.13 2003/05/13 11:31:03 dburken Exp $
#ifndef ossimImageDisplayWriter_HEADER
#define ossimImageDisplayWriter_HEADER

#include "imaging/ossimImageWriter.h"
#include "base/data_types/ossimFilename.h"
#include "base/common/events/ossimConnectableObjectListener.h"
#include "base/common/ossimDisplayInterface.h"
#include "base/common/events/ossimObjectEvents.h"
#include "base/common/events/ossimProcessProgressEvent.h"

class ossimImageDisplayWriter :  public ossimImageWriter,
                                 public ossimDisplayInterface,
                                 public ossimConnectableObjectListener

{
public:
   virtual ~ossimImageDisplayWriter(){}
   

   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const
      {
         
         return (object&& PTR_CAST(ossimImageSourceInterface, object));
      }
   virtual void setOutputName(const ossimString& out)
      {
         setTitle(out);
      }

   /*!
    * Sets the title of a display.
    */
   virtual void setTitle( const ossimString& title )=0;

   /*!
    * will return true if its minimized
    */
   virtual bool isMinimized()const=0;

   virtual bool isMaximized()const=0;
   /*!
    * Will repaint the display
    */
   virtual void refreshDisplay()=0;

   
   virtual void minimize()=0;

   virtual void maximize()=0;
   /*!
    * Will show a display
    */
   virtual void show()=0;
   
protected:
   ossimImageDisplayWriter(ossimObject* owner=NULL);
   ossimImageDisplayWriter(ossimObject* owner,
                           ossim_uint32 numberOfInputs,
                           ossim_uint32 numberOfOutputs,
                           bool inputListIsFixed,
                           bool outputListIsFixed);
TYPE_DATA
};
#endif
