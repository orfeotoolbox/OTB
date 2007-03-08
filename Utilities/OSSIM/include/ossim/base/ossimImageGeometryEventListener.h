//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimImageGeometryEventListener.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageGeometryEventListener_HEADER
#define ossimImageGeometryEventListener_HEADER
#include <ossim/base/ossimListener.h>
#include <ossim/base/ossimImageGeometryEvent.h>

class ossimImageGeometryEventListener : public ossimListener
{
public:
   ossimImageGeometryEventListener():ossimListener(){}
   virtual ~ossimImageGeometryEventListener(){}
   virtual void processEvent(ossimEvent& event);

   virtual void imageGeometryEvent(ossimImageGeometryEvent& /* event */)
      {}
   
TYPE_DATA
};

#endif
