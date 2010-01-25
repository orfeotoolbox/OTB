//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id
#ifndef ossimDisplayEventListener_HEADER
#define ossimDisplayEventListener_HEADER

#include <ossim/base/ossimListener.h>

class ossimDisplayListEvent;
class ossimDisplayRefreshEvent;

class OSSIMDLLEXPORT ossimDisplayEventListener : public ossimListener
{
public:
   ossimDisplayEventListener();
   virtual ~ossimDisplayEventListener();
   virtual void processEvent(ossimEvent& event);
   virtual void displayListEvent(ossimDisplayListEvent&);
   virtual void displayRefreshEvent(ossimDisplayRefreshEvent&);

TYPE_DATA
};

#endif /* End of "#ifndef ossimDisplayEventListener_HEADER". */
