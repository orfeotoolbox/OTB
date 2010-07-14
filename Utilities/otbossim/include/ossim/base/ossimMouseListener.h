//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimMouseListener.h 17195 2010-04-23 17:32:18Z dburken $
#ifndef ossimMouseListener_HEADER
#define ossimMouseListener_HEADER
#include <ossim/base/ossimListener.h>
#include <ossim/base/ossimMouseEvent.h>

class OSSIMDLLEXPORT ossimMouseListener : public ossimListener
{
public:
   ossimMouseListener():ossimListener(){}
   virtual ~ossimMouseListener(){}
   
   virtual void processEvent(ossimEvent& event);

   /*!
    * processes all mouse events
    */
   virtual void mouseEvent(ossimMouseEvent& /* event */)
      {}

TYPE_DATA
};
#endif
