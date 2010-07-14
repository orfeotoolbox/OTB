//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
// 
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimListenerManager.cpp 17193 2010-04-23 14:59:44Z dburken $

#include <algorithm>
#include <ossim/base/ossimListenerManager.h>
#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimListener.h>

RTTI_DEF(ossimListenerManager, "ossimListenerManager");

ossimListenerManager::ossimListenerManager()
{
   theFireEventFlag = false;
}

ossimListenerManager::ossimListenerManager(const ossimListenerManager& /* rhs */)
{
//    replaceListeners(rhs.getListeners());
}

ossimListenerManager::~ossimListenerManager()
{
   theListenerList.clear();
}

void ossimListenerManager::fireEvent(ossimEvent& event)
{
   // only process the event if it has not been consumed.
   event.setCurrentObject(PTR_CAST(ossimObject, this));
   if(event.isConsumed())
   {
      return;
   }
   theFireEventFlag = true;

   std::list<ossimListener*>::iterator currentIterator = theListenerList.begin();
   
   while(currentIterator != theListenerList.end())
   {
      // only fire if the event is not consumed
      if(!event.isConsumed())
      {
         if(*currentIterator)
         {
            if(theDelayedRemove.end()==std::find(theDelayedRemove.begin(),
                                                 theDelayedRemove.end(),
                                                 (*currentIterator)))
            {
               if((*currentIterator)->isListenerEnabled())
               {
                  (*currentIterator)->processEvent(event);
               }
            }
         }
      }
      else
      {
         // the event is now consumed so stop propagating.
         //
         theFireEventFlag = false;
         break;
      }
      ++currentIterator;
   }
   
   theFireEventFlag = false;
   
   if(theDelayedAdd.size())
   {
      for(std::list<ossimListener*>::iterator current = theDelayedAdd.begin();
          current != theDelayedAdd.end();++current)
      {
         addListener(*current);
      }
      theDelayedAdd.clear();
   }
   
   if(theDelayedRemove.size())
   {
      for(std::list<ossimListener*>::iterator current = theDelayedRemove.begin();
          current != theDelayedRemove.end();++current)
      {
         removeListener(*current);
      }
      theDelayedRemove.clear();
   }
}

bool ossimListenerManager::addListener(ossimListener* listener)
{
   if(theFireEventFlag)
   {
      theDelayedAdd.push_back(listener);
   }
   else
   {
      
      if(!findListener(listener))
      {
         theListenerList.push_back(listener);
      }
   }
   
   return true;
}

bool ossimListenerManager::removeListener(ossimListener* listener)
{
   
   if(theFireEventFlag)
   {
      theDelayedRemove.push_back(listener);
      return true;
   }
   
   std::list<ossimListener*>::iterator current=theListenerList.begin();
   while(current!=theListenerList.end())
   {
      if( (*current) == listener)
      {
         current = theListenerList.erase(current);
         break; // Should only be in list once... (drb)
      }
      else
      {
         ++current;
      }
   }
   
   return true;
}

bool ossimListenerManager::findListener(ossimListener* listener)
{
   bool result = false;

   if(listener)
   {
      std::list<ossimListener*>::iterator currentIter =
         std::find(theListenerList.begin(),
                   theListenerList.end(),
                   listener);
      result = (currentIter != theListenerList.end());
   }
   
   return result;
}
