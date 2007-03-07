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
// Author: Garrett Potts (gpotts@imagelinks)
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimListenerManager.h,v 1.16 2005/09/14 15:05:03 gpotts Exp $
#ifndef ossimListenerManager_HEADER
#define ossimListenerManager_HEADER
#include "base/common/ossimConstants.h"
#include <list>
#include "base/common/ossimRtti.h"

class OSSIMDLLEXPORT ossimListener;
class OSSIMDLLEXPORT ossimEvent;

class OSSIMDLLEXPORT ossimListenerManager
{
public:
   /*!
    * Default constructor and needs no initialization.
    */
   ossimListenerManager();

   /*!
    * Default destructor.  Note the Manager doesn't own the listeners
    * and will not delete them
    */
   virtual ~ossimListenerManager();

   /*!
    * Traverses through all listeners and fires an event to them.
    * if any listener sets the consumed flag on the event it
    * will stop traversing the list.
    */
   virtual void fireEvent(ossimEvent& event);
   
   /*!
    * Appends the listener onto the list.
    */
   virtual bool addListener(ossimListener* listener);

   /*!
    * Will push the lister to the front of the list.
    */
  //   virtual bool insertFrontListener(ossimListener* listener);

   /*!
    * Finds and removes the listener.
    */
   virtual bool removeListener(ossimListener* listener);

   
   /*!
    * Will find the listener passed in as the first argument
    * and insert the passed in listener before it.  If not
    * found then it will default to a push to front.
    */
//    virtual bool insertBeforeListener(ossimListener* listenerSearchKey,
//                                      ossimListener* listener);

   /*!
    * Will find the listener passed in as the first argument
    * and insert the passed in listener after it.  If not
    * found then it will default to an append.
    */
//    virtual bool insertAfterListener(ossimListener* listenerSearchKey,
//                                     ossimListener* listener);
   
   /*!
    * Will insert the listener before the specified
    * listener.  If the index is invalid it defaults
    * to a push to front.  An invalid index is any
    * index < 0 or larger than the number of items
    * currently in the list.
    */
//    virtual bool insertBeforeListener(ossimListener* listener,
//                                      long index);

   /*!
    * Will insert the listener after the specified
    * listener.  If the index is invalid it defaults
    * to an addListener.  An invalid index is any
    * index < 0 or larger than the number of items
    * currently in the list.
    */
//    virtual bool insertAfterListener(ossimListener* listener,
//                                     long index);

   /*!
    * Searches the list and sees if a listener is found
    */
  virtual bool findListener(ossimListener* listener);
  
   /*!
    * Will search the list and return the index if found else
    * returns a negative value.
    */
  //   virtual long getIndexOfListener(ossimListener* listener);

  //   const std::vector<ossimListener*>& getListeners()const{return theListenerList;}

   
protected:
  ossimListenerManager(const ossimListenerManager& rhs);
  
  /*!
   *  Holds the list of listeners.
   */
  std::list<ossimListener*> theListenerList;
  bool theEnabledFlag;
  mutable std::list<ossimListener*> theDelayedAdd;
  mutable std::list<ossimListener*> theDelayedRemove;
  mutable bool theFireEventFlag;
//  mutable std::list<ossimListener*>::iterator theCurrentIterator;
  
  
TYPE_DATA
};

#endif
