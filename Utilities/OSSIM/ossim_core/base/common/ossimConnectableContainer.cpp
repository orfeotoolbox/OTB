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
// Description:
//
//*************************************************************************
// $Id: ossimConnectableContainer.cpp,v 1.48 2005/11/22 19:40:35 dburken Exp $
#include <algorithm>
#include <base/common/ossimConnectableContainer.h>
#include <base/common/ossimIdManager.h>
#include <base/common/events/ossimObjectDestructingEvent.h>
#include <base/common/events/ossimPropertyEvent.h>
#include <base/common/events/ossimConnectionEvent.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/factory/ossimObjectFactoryRegistry.h>
#include <base/common/ossimTrace.h>
#include <base/context/ossimNotifyContext.h>

#include <stack>
using namespace std;

static ossimTrace traceDebug("ossimConnectableContainer:debug");

RTTI_DEF2(ossimConnectableContainer, "ossimConnectableContainer", ossimConnectableObject, ossimConnectableContainerInterface);

class ossimConnectableContainerChildListener : public ossimConnectableObjectListener
{
public:
   ossimConnectableContainerChildListener(ossimConnectableContainer* container)
      :theContainer(container),
       theEnabledFlag(true)
      {}
   void enableListener(){theEnabledFlag = true;}
   void disableListener(){theEnabledFlag = false;}

   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event)
      {
         ossimConnectableObject* object = PTR_CAST(ossimConnectableObject, event.getObject());
         if(object&&theContainer)
         {
            if(theContainer->removeChild(object))
            {
               object->removeListener((ossimConnectableObjectListener*) this);
            }
         }
         propagatePropertyEvent(event);

      }
   virtual void disconnectInputEvent(ossimConnectionEvent& event)
      {
         propagatePropertyEvent(event);
      }
   virtual void connectInputEvent(ossimConnectionEvent& event)
      {
         propagatePropertyEvent(event);
      }

   void propagatePropertyEvent(ossimEvent& event)
      {
         if(theEnabledFlag)
         {
            if(event.getObject())
            {
               ossimConnectableObject* connectable = PTR_CAST(ossimConnectableObject,
                                                             event.getObject());
               if(connectable)
               {
                  ossimPropertyEvent evt(event.getObject());
                  connectable->propagateEventToOutputs(evt);
               }
            }
         }
      }
   ossimConnectableContainer* theContainer;
   bool theEnabledFlag;

};

ossimConnectableContainer::ossimConnectableContainer(ossimConnectableObject* owner)
   :ossimConnectableObject(owner, 0, 0, true, true),
    ossimConnectableContainerInterface(NULL)
{
   ossimConnectableContainerInterface::theBaseObject = this;
   theChildListener = new ossimConnectableContainerChildListener(this);
}

ossimConnectableContainer::~ossimConnectableContainer()
{
   // will need to destory and detach all children
   //
   deleteAllChildren();

   if(theChildListener)
   {
      delete theChildListener;
      theChildListener = NULL;
   }
}

ossimConnectableObject* ossimConnectableContainer::findFirstObjectOfType(const RTTItypeid& typeInfo,
                                                                         bool recurse)
{
   ossimConnectableObject* result = NULL;
   connectablObjectMapType::iterator current;

   current = theObjectMap.begin();
   while(current != theObjectMap.end())
   {
      if(((*current).second)->canCastTo(typeInfo))
      {
         return (*current).second;
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();
      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second);

         if(child)
         {
            result = child->findFirstObjectOfType(typeInfo);
            if(result)
            {
               return result;
            }
         }
         ++current;
      }
   }

   return result;
}

ossimConnectableObject* ossimConnectableContainer::findFirstObjectOfType(const ossimString& className,
                                                                         bool recurse)
{
   ossimConnectableObject* result = NULL;
   connectablObjectMapType::iterator current;

   current = theObjectMap.begin();
   while(current != theObjectMap.end())
   {
      if( ((*current).second)->canCastTo(className) )
      {
         return (*current).second;
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();
      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second);

         if(child)
         {
            result = child->findFirstObjectOfType(className);
            if(result)
            {
               return result;
            }
         }
         ++current;
      }
   }

   return result;
}

std::vector<ossimConnectableObject*> ossimConnectableContainer::findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                                                     bool recurse)
{
   std::vector<ossimConnectableObject*> result;

   connectablObjectMapType::iterator current;

   current = theObjectMap.begin();
   while(current != theObjectMap.end())
   {
      if(((*current).second)->canCastTo(typeInfo))
      {
         result.push_back( (*current).second);
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();
      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second);

         if(child)
         {
            vector<ossimConnectableObject*> temp;
            temp = child->findAllObjectsOfType(typeInfo, recurse);
            for(long index=0; index < (long)temp.size();++index)
            {
               result.push_back(temp[index]);
            }
         }
         ++current;
      }
   }
   return result;
}

std::vector<ossimConnectableObject*> ossimConnectableContainer::findAllObjectsOfType(const ossimString& className,
                                                                                     bool recurse)
{
   std::vector<ossimConnectableObject*> result;

   connectablObjectMapType::iterator current;

   current = theObjectMap.begin();
   while(current != theObjectMap.end())
   {
      if(((*current).second)->canCastTo(className))
      {
         result.push_back( (*current).second);
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();
      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second);

         if(child)
         {
            vector<ossimConnectableObject*> temp;
            temp = child->findAllObjectsOfType(className, true);
            for(long index=0; index < (long)temp.size();++index)
            {
               result.push_back(temp[index]);
            }
         }
         ++current;
      }
   }
   return result;
}

ossimConnectableObject* ossimConnectableContainer::findObject(const ossimId& id,
                                                              bool recurse)
{
   connectablObjectMapType::iterator current;

   current = theObjectMap.begin();

   while(current != theObjectMap.end())
   {
      if((*current).second && ((*current).second->getId()==id))
      {
         return (*current).second;
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();

      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second);

         if(child)
         {
            ossimConnectableObject* object = child->findObject(id, true);

            if(object) return object;
         }
         ++current;
      }
   }
   return NULL;
}

ossimConnectableObject* ossimConnectableContainer::findObject(const ossimConnectableObject* obj,
                                                              bool recurse)
{
   connectablObjectMapType::iterator current;

   current = theObjectMap.begin();

   while(current != theObjectMap.end())
   {
      if((*current).second && ((*current).second==obj))
      {
         return (*current).second;
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();

      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second);

         if(child)
         {
            ossimConnectableObject* object = child->findObject(obj, true);

            if(object) return object;
         }
         ++current;
      }
   }

   return NULL;
}

void ossimConnectableContainer::makeUniqueIds()
{
   connectablObjectMapType::iterator current;
   vector<ossimConnectableObject*> objectList;


   current = theObjectMap.begin();

   setId(ossimIdManager::instance()->generateId());
   while(current != theObjectMap.end())
   {
      objectList.push_back((*current).second);
      ++current;
   }
   theObjectMap.clear();

   for(long index = 0; index < (long)objectList.size(); ++index)
   {
      ossimConnectableContainerInterface* container = PTR_CAST(ossimConnectableContainerInterface,
                                                               objectList[index]);
      if(container)
      {
         container->makeUniqueIds();
         theObjectMap.insert(make_pair(objectList[index]->getId().getId(),
                                       objectList[index]));
      }
      else
      {
         objectList[index]->setId(ossimIdManager::instance()->generateId());
         theObjectMap.insert(make_pair(objectList[index]->getId().getId(),
                             objectList[index]));
      }
   }
}

ossim_uint32 ossimConnectableContainer::getNumberOfObjects(bool recurse)const
{
   ossim_uint32 result = 0;

   connectablObjectMapType::const_iterator current;

   current = theObjectMap.begin();

   while(current != theObjectMap.end())
   {
      ++result;

      if(recurse)
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second);
         if(child)
         {
            result += child->getNumberOfObjects(true);
         }
      }
      ++current;
   }

   return result;
}

bool ossimConnectableContainer::addChild(ossimConnectableObject* object)
{
   if(object)
   {
      // if it's not already a child
      if(!findObject(object->getId()))
      {
         object->changeOwner(this);
         theObjectMap.insert(make_pair(object->getId().getId(), object));
//         object->addListener((ossimConnectableObjectListener*)this);
         object->addListener(theChildListener);
      }

      return true;
   }

   return false;
}

bool ossimConnectableContainer::removeChild(ossimConnectableObject* object)
{
   ossimConnectableObject* result = NULL;

   if(object)
   {
      connectablObjectMapType::iterator childIter = theObjectMap.find(object->getId().getId());
      if(childIter != theObjectMap.end())
      {
         result = (*childIter).second;
         if(result->getOwner() == this)
         {
            result->changeOwner(NULL);
            result->removeListener(theChildListener);
//            result->removeListener(this);
         }
         theObjectMap.erase(childIter);
      }
   }

   return (result!=NULL);
}

bool ossimConnectableContainer::loadState(const ossimKeywordlist& kwl,
                                               const char* prefix)
{
   static const char* MODULE = "ossimConnectableContainer::loadState(kwl, prefix)";

   ossimString copyPrefix = prefix;
   bool result = ossimConnectableObject::loadState(kwl, copyPrefix.c_str());

   map<ossimId, vector<ossimId> > idMapping;
   result = addAllObjects(idMapping, kwl, prefix);
   if(!result)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " error: " << "problems adding sources" << endl;
   }
   result = connectAllObjects(idMapping);
   if(!result)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " error: " << "problems connecting sources\n";
   }

   return result;
}

bool ossimConnectableContainer::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   bool result = true;
   connectablObjectMapType::const_iterator current;

   ossim_int32 childIndex = 1;
   current = theObjectMap.begin();
   while(current != theObjectMap.end())
   {
     ossimString newPrefix = ( (ossimString(prefix) +
				ossimString("object") +
				ossimString::toString(childIndex) + "."));
     if((*current).second)
       {
	 bool test = ((*current).second)->saveState(kwl, newPrefix);
	 if(!test)
	   {
	     result = false;
	   }
       }
     ++childIndex;
     ++current;
   }
   if(result)
   {
      result = ossimConnectableObject::saveState(kwl, prefix);
   }

   return result;
}

void ossimConnectableContainer::getChildren(vector<ossimConnectableObject*>& children,
                                               bool immediateChildrenOnlyFlag)
{
   connectablObjectMapType::iterator current;
   current = theObjectMap.begin();
   vector<ossimConnectableObject*> temp;

   while(current != theObjectMap.end())
   {
      temp.push_back((*current).second);
   }
   ossim_uint32 i;
   for(i = 0; i < temp.size();++i)
   {

      if(!immediateChildrenOnlyFlag)
      {
         ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                             (*current).second);
         if(!inter)
         {
            children.push_back(temp[i]);
         }
      }
      else
      {
         children.push_back(temp[i]);
      }
   }

   if(!immediateChildrenOnlyFlag)
   {
      for(i = 0; i < temp.size(); ++i)
      {
         ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                             (*current).second);

         if(inter)
         {
            inter->getChildren(children,
                               immediateChildrenOnlyFlag);
         }
      }
   }
}

void ossimConnectableContainer::deleteAllChildren()
{
   connectablObjectMapType::iterator current;
   ossimConnectableObject* temp;
   current = theObjectMap.begin();
   removeAllListeners();

   while(theObjectMap.size())
   {
      current = theObjectMap.begin();
      temp = (*current).second;
      if(temp)
      {
//         temp->removeListener((ossimConnectableObjectListener*) this);
         temp->removeListener(theChildListener);
         (*current).second = NULL;
      }
      theObjectMap.erase(current);
      if(temp)
      {
         delete temp;
      }
   }
}

void ossimConnectableContainer::removeAllListeners()
{
   connectablObjectMapType::iterator current;
   ossimConnectableObject* temp;
   current = theObjectMap.begin();

   while(current != theObjectMap.end())
   {
      temp = (*current).second;
      if(temp)
      {
         temp->removeListener(theChildListener);
//         temp->removeListener((ossimConnectableObjectListener*) this);
      }
      ++current;
   }
}

bool ossimConnectableContainer::addAllObjects(map<ossimId,
                                              vector<ossimId> >& idMapping,
                                              const ossimKeywordlist& kwl,
                                              const char* prefix)
{
   ossimString copyPrefix = prefix;

   vector<ossimId> inputConnectionIds;

   ossimString regExpression =  ossimString("^(") + copyPrefix + "object[0-9]+.)";
   vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   long numberOfSources = keys.size();

   int offset = (copyPrefix+"object").size();
   int idx = 0;
   std::vector<int> theNumberList(numberOfSources);
   for(idx = 0; idx < (int)theNumberList.size();++idx)
     {
       ossimString numberStr(keys[idx].begin() + offset,
			     keys[idx].end());
       theNumberList[idx] = numberStr.toInt();
     }
   std::sort(theNumberList.begin(), theNumberList.end());
   for(idx=0;idx < (int)theNumberList.size();++idx)
   {
      ossimString newPrefix = copyPrefix;
      newPrefix += ossimString("object");
      newPrefix += ossimString::toString(theNumberList[idx]);
      newPrefix += ossimString(".");

      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "trying to create source with prefix: " << newPrefix << "\n";
      }
      ossimObject* object = ossimObjectFactoryRegistry::instance()->createObject(kwl,
                                                                                 newPrefix.c_str());
      if(object)
      {
         ossimConnectableObject* connectable = PTR_CAST(ossimConnectableObject, object);
         if(connectable)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "Created source with prefix: " << newPrefix << "\n";
            }
            // we did find a source so include it in the count
            ossimId id = connectable->getId();
            inputConnectionIds.clear();

            findInputConnectionIds(inputConnectionIds,
                                   kwl,
                                   newPrefix);

            if(inputConnectionIds.size() != 0)
            {
               idMapping.insert(std::make_pair(id, inputConnectionIds));
            }
            addChild(connectable);
         }
         else
         {
            delete object;
            object = NULL;
         }
      }
   }

   return true;
}

void ossimConnectableContainer::findInputConnectionIds(vector<ossimId>& result,
                                                       const ossimKeywordlist& kwl,
                                                       const char* prefix)
{
   ossimString newPrefix = prefix;
   long counter = 0;

   long numberOfMatches = 0;

   ossimString regExpression =  ossimString("^") + newPrefix + "input_connection[0-9]+";
   long numberOfKeys = kwl.getNumberOfSubstringKeys( regExpression);
   vector<ossimString> l = kwl.getSubstringKeyList(regExpression);
   if(numberOfKeys > 0)
   {
      newPrefix += "input_connection";

      while(numberOfMatches < numberOfKeys)
      {
         const char* lookup = kwl.find(newPrefix,
                                       ossimString::toString(counter));
         if(lookup)
         {
            ++numberOfMatches;
            long id = ossimString(lookup).toLong();
            if(id != ossimId::INVALID_ID)
            {
               result.push_back(ossimId(id));
            }
         }
         ++counter;
      }
   }
}

bool ossimConnectableContainer::connectAllObjects(const map<ossimId, vector<ossimId> >& idMapping)
{
   if(idMapping.size())
   {
      map<ossimId, vector<ossimId> >::const_iterator iter = idMapping.begin();

      while(iter != idMapping.end())
      {
         ossimConnectableObject* currentObject = findObject((*iter).first);

         if(currentObject)
         {
            long upperBound = (*iter).second.size();
            for(long index = 0; index < upperBound; ++index)
            {
               ossimConnectableObject* inputObject = findObject((*iter).second[index]);

               currentObject->connectMyInputTo(index, inputObject);
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_FATAL) << "ossimConnectableContainer::connectAllObjects, Could not find " << (*iter).first << " for source: \n";
            return false;
         }
         ++iter;
      }
   }
   return true;
}

ossimObject* ossimConnectableContainer::getObject()
{
   return this;
}

const ossimObject* ossimConnectableContainer::getObject()const
{
   return this;
}

ossimConnectableObject* ossimConnectableContainer::getConnectableObject(
   ossim_uint32 index)
{
   ossimConnectableObject* result = NULL;

   if (index > theObjectMap.size())
   {
      return result;
   }

   connectablObjectMapType::iterator current;
   current = theObjectMap.begin();
   ossim_uint32 i = 0;
   while(current != theObjectMap.end())
   {
      if (i == index)
      {
         return (*current).second;
      }
      ++current;
      ++i;
   }
   
   return result; 
}

bool ossimConnectableContainer::canConnectMyInputTo(ossim_int32,
                                                    const ossimConnectableObject*) const
{
   return false;
}

bool ossimConnectableContainer::canConnectMyOutputTo(ossim_int32,
                                                     const ossimConnectableObject*) const
{
   return false;
}
