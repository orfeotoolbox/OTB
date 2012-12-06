//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimConnectableContainer.cpp 21850 2012-10-21 20:09:55Z dburken $

#include <ossim/base/ossimConnectableContainer.h>
#include <ossim/base/ossimIdManager.h>
#include <ossim/base/ossimObjectDestructingEvent.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimConnectionEvent.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimVisitor.h>
#include <algorithm>
#include <stack>

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
   }
   theChildListener = 0;
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
         return (*current).second.get();
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();
      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second.get());

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
         return (*current).second.get();
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();
      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second.get());

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

ossimConnectableObject::ConnectableObjectList ossimConnectableContainer::findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                                                     bool recurse)
{
   ossimConnectableObject::ConnectableObjectList result;

   connectablObjectMapType::iterator current;

   current = theObjectMap.begin();
   while(current != theObjectMap.end())
   {
      if(((*current).second)->canCastTo(typeInfo))
      {
         result.push_back( (*current).second.get());
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();
      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second.get());

         if(child)
         {
            ConnectableObjectList temp;
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

ossimConnectableObject::ConnectableObjectList ossimConnectableContainer::findAllObjectsOfType(const ossimString& className,
                                                                                     bool recurse)
{
   ossimConnectableObject::ConnectableObjectList result;

   connectablObjectMapType::iterator current;

   current = theObjectMap.begin();
   while(current != theObjectMap.end())
   {
      if(((*current).second)->canCastTo(className))
      {
         result.push_back( (*current).second.get());
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();
      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second.get());

         if(child)
         {
            ossimConnectableObject::ConnectableObjectList temp;
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
      if((*current).second.valid() && ((*current).second->getId()==id))
      {
         return (*current).second.get();
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();

      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second.get());

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
      if((*current).second.valid() && ((*current).second==obj))
      {
         return (*current).second.get();
      }
      ++current;
   }

   if(recurse)
   {
      current = theObjectMap.begin();

      while(current != theObjectMap.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second.get());

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
   std::vector<ossimRefPtr<ossimConnectableObject> > objectList;


   current = theObjectMap.begin();

   setId(ossimIdManager::instance()->generateId());
   while(current != theObjectMap.end())
   {
      objectList.push_back((*current).second.get());
      ++current;
   }
   theObjectMap.clear();

   for(long index = 0; index < (long)objectList.size(); ++index)
   {
      ossimConnectableContainerInterface* container = PTR_CAST(ossimConnectableContainerInterface,
                                                               objectList[index].get());
      if(container)
      {
         container->makeUniqueIds();
         theObjectMap.insert(std::make_pair(objectList[index]->getId().getId(),
                                            objectList[index].get()));
      }
      else
      {
         objectList[index]->setId(ossimIdManager::instance()->generateId());
         theObjectMap.insert(std::make_pair(objectList[index]->getId().getId(),
                             objectList[index].get()));
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
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).second.get());
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
      
      ossimIdVisitor visitor(
         object->getId(),
         (ossimVisitor::VISIT_INPUTS|ossimVisitor::VISIT_CHILDREN) );
      accept( visitor );
      
      // if(!findObject(object->getId()))
      if ( !visitor.getObject() )      
      {
         object->changeOwner(this);
         theObjectMap.insert(std::make_pair(object->getId().getId(), object));
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
         result = (*childIter).second.get();
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

   std::map<ossimId, std::vector<ossimId> > idMapping;
   result = addAllObjects(idMapping, kwl, prefix);
   if(!result)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " error: " << "problems adding sources" << std::endl;
   }
   result = connectAllObjects(idMapping);
   if(!result)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " error: " << "problems connecting sources\n";
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
      if((*current).second.valid())
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

void ossimConnectableContainer::getChildren(std::vector<ossimConnectableObject*>& children,
                                               bool immediateChildrenOnlyFlag)
{
   connectablObjectMapType::iterator current;
   current = theObjectMap.begin();
   std::vector<ossimConnectableObject*> temp;

   while(current != theObjectMap.end())
   {
      temp.push_back((*current).second.get());
   }
   ossim_uint32 i;
   for(i = 0; i < temp.size();++i)
   {

      if(!immediateChildrenOnlyFlag)
      {
         ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                             (*current).second.get());
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
                                                             (*current).second.get());

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
      temp = (*current).second.get();
      if(temp)
      {
         temp->removeListener(theChildListener);
         temp->disconnect();
         (*current).second = NULL;
      }
      theObjectMap.erase(current);
   }
}

void ossimConnectableContainer::removeAllListeners()
{
   connectablObjectMapType::iterator current;
   ossimConnectableObject* temp;
   current = theObjectMap.begin();

   while(current != theObjectMap.end())
   {
      temp = (*current).second.get();
      if(temp)
      {
         temp->removeListener(theChildListener);
//         temp->removeListener((ossimConnectableObjectListener*) this);
      }
      ++current;
   }
}

bool ossimConnectableContainer::addAllObjects(std::map<ossimId,
                                              std::vector<ossimId> >& idMapping,
                                              const ossimKeywordlist& kwl,
                                              const char* prefix)
{
   ossimString copyPrefix = prefix;

   std::vector<ossimId> inputConnectionIds;

   ossimString regExpression =  ossimString("^(") + copyPrefix + "object[0-9]+.)";
   std::vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   long numberOfSources = (long)keys.size();

   int offset = (int)(copyPrefix+"object").size();
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
      ossimRefPtr<ossimObject> object = ossimObjectFactoryRegistry::instance()->createObject(kwl,
                                                                                 newPrefix.c_str());
      if(object.valid())
      {
         ossimConnectableObject* connectable = PTR_CAST(ossimConnectableObject, object.get());
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
      }
   }

   return true;
}

void ossimConnectableContainer::findInputConnectionIds(std::vector<ossimId>& result,
                                                       const ossimKeywordlist& kwl,
                                                       const char* prefix)
{
   ossimString newPrefix = prefix;

   ossim_uint32 counter = 0;
   ossim_uint32 numberOfMatches = 0;

   ossimString regExpression =  ossimString("^") + newPrefix + "input_connection[0-9]+";
   ossim_uint32 numberOfKeys = kwl.getNumberOfSubstringKeys( regExpression);
   std::vector<ossimString> l = kwl.getSubstringKeyList(regExpression);
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

bool ossimConnectableContainer::connectAllObjects(const std::map<ossimId, std::vector<ossimId> >& idMapping)
{
   if(idMapping.size())
   {
      
      ossimIdVisitor visitor( (ossimVisitor::VISIT_INPUTS|ossimVisitor::VISIT_CHILDREN) );

      std::map<ossimId, std::vector<ossimId> >::const_iterator iter = idMapping.begin();

      while(iter != idMapping.end())
      {
         visitor.setId( (*iter).first );
         accept( visitor );
         ossimConnectableObject* currentObject = visitor.getObject();
         // ossimConnectableObject* currentObject = findObject((*iter).first);

         if(currentObject)
         {
            long upperBound = (long)(*iter).second.size();
            for(long index = 0; index < upperBound; ++index)
            {
               visitor.reset();
               visitor.setId( (*iter).second[index] );
               accept( visitor );
               ossimConnectableObject* inputObject = visitor.getObject();
               if ( inputObject )
               {
                  currentObject->connectMyInputTo(index, inputObject);
               }
               // ossimConnectableObject* inputObject = findObject((*iter).second[index]);
               // currentObject->connectMyInputTo(index, inputObject);
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "ossimConnectableContainer::connectAllObjects, Could not find "
               << (*iter).first << " for source: \n";
            return false;
         }
         visitor.reset();
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
         return (*current).second.get();
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

void ossimConnectableContainer::accept(ossimVisitor& visitor)
{
   if(!visitor.hasVisited(this))
   {
      visitor.visit(this);
      ossimVisitor::VisitorType currentType = visitor.getVisitorType();
      // lets make sure inputs and outputs are turned off for we are traversing all children and we should not have
      // to have that enabled
      //
      visitor.turnOffVisitorType(ossimVisitor::VISIT_INPUTS|ossimVisitor::VISIT_OUTPUTS);
      if(visitor.getVisitorType() & ossimVisitor::VISIT_CHILDREN)
      {
         connectablObjectMapType::iterator current = theObjectMap.begin();
         while((current != theObjectMap.end())&&!visitor.stopTraversal())
         {
            ossimRefPtr<ossimConnectableObject> currentObject = (current->second);
            if(currentObject.valid()&&!visitor.hasVisited(currentObject.get())) currentObject->accept(visitor);
            ++current;
         }
      }
      visitor.setVisitorType(currentType);
   }
   
   ossimConnectableObject::accept(visitor);
   
}

//**************************************************************************************************
// Inserts all of this object's children and inputs into the container provided. Since this is
// itself a container, this method will consolidate this with the argument container. Therefore 
// this object will not be represented in the argument container (but its children will be).
// Returns TRUE if successful.
//**************************************************************************************************
bool ossimConnectableContainer::fillContainer(ossimConnectableContainer& container)
{
   connectablObjectMapType::iterator current;
   current = theObjectMap.begin();
   // ossim_uint32 i = 0;
   // bool fill_ok;
   while(current != theObjectMap.end())
   {
      ossimRefPtr<ossimConnectableObject> currentObject = current->second;
      if (currentObject.valid())
         container.addChild(currentObject.get());
      current++;
   }
   return true;
}
