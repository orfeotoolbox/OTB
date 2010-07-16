//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageAoiListener.h 17195 2010-04-23 17:32:18Z dburken $
#ifndef ossimImageAoiListener_HEADER
#define ossimImageAoiListener_HEADER
#include <ossim/base/ossimListener.h>

class ossimImageRectangleEvent;
class ossimImagePolygonEvent;

class OSSIMDLLEXPORT ossimImageAoiListener : public ossimListener
{
public:
   ossimImageAoiListener():ossimListener(){}
   virtual ~ossimImageAoiListener(){}
   
   virtual void processEvent(ossimEvent& event);

   virtual void imageRectangleEvent(ossimImageRectangleEvent& /* event */)
      {}
   virtual void imagePolygonEvent(ossimImagePolygonEvent& /* event */)
      {}
};
#endif
