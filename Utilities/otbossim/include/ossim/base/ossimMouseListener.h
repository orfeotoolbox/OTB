//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimMouseListener.h 9968 2006-11-29 14:01:53Z gpotts $
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
   virtual void mouseEvent(ossimMouseEvent& event)
      {}

TYPE_DATA
};
#endif
