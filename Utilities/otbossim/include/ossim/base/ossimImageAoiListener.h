//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageAoiListener.h 14799 2009-06-30 08:54:44Z dburken $
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

   virtual void imageRectangleEvent(ossimImageRectangleEvent& event)
      {}
   virtual void imagePolygonEvent(ossimImagePolygonEvent& event)
      {}
};
#endif
