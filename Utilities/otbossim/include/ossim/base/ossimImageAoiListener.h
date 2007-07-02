//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageAoiListener.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageAoiListener_HEADER
#define ossimImageAoiListener_HEADER
#include <ossim/base/ossimListener.h>

class OSSIMDLLEXPORT ossimImageRectangleEvent;
class OSSIMDLLEXPORT ossimImagePolygonEvent;

class OSSIMDLLEXPORT ossimImageAoiListener : public ossimListener
{
public:
   ossimImageAoiListener():ossimListener(){}
   virtual ~ossimImageAoiListener(){}
   
   virtual void processEvent(ossimEvent& event);

   virtual void imageRectangleEvent(ossimImageRectangleEvent& event)
      {}
   virtual void imagePolygonEvent(ossimImagePolygonEvent& event)
      {}
};
#endif
