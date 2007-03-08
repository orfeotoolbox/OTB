//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageWriter
//*******************************************************************
//  $Id: ossimImageDisplayWriter.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageDisplayWriter_HEADER
#define ossimImageDisplayWriter_HEADER

#include <ossim/imaging/ossimImageWriter.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimDisplayInterface.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimProcessProgressEvent.h>

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
