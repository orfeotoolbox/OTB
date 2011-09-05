//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageChain.cpp 19968 2011-08-16 19:26:20Z gpotts $
#include <algorithm>
#include <iostream>
#include <iterator>

#include <ossim/imaging/ossimImageChain.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimIdManager.h>
#include <ossim/base/ossimVisitor.h>

static ossimTrace traceDebug("ossimImageChain");


RTTI_DEF3(ossimImageChain, "ossimImageChain", ossimImageSource,
          ossimConnectableObjectListener, ossimConnectableContainerInterface);

class ossimImageChainChildListener : public ossimConnectableObjectListener
{
public:
   ossimImageChainChildListener(ossimImageChain* owner)
      :theChain(owner)
      {
      }
   virtual void processEvent(ossimEvent& event);
//   virtual void propertyEvent(ossimPropertyEvent& event);
//   virtual void refreshEvent(ossimRefreshEvent& /* event */);
   ossimImageChain* theChain;
};

void ossimImageChainChildListener::processEvent(ossimEvent& event)
{
   if(!theChain) return;
   ossimConnectableObjectListener::processEvent(event);
   ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject,
                                          event.getCurrentObject());
   
   if((ossimConnectableObject*)theChain->getFirstSource() == obj)
   {
      if(event.isPropagatingToOutputs())
      {
         ossimConnectableObject::ConnectableObjectList& outputList = theChain->getOutputList();
         ossim_uint32 idx = 0;
         for(idx = 0; idx < outputList.size();++idx)
         {
            if(outputList[idx].valid())
            {
               outputList[idx]->fireEvent(event);
               outputList[idx]->propagateEventToOutputs(event);
            }
         }
      }
   }
}
#if 0
void ossimImageChainChildListener::propertyEvent(ossimPropertyEvent& event)
{
   ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject,
                                          event.getObject());
   
   
   // if it originated from within my chain then propagate up to parent
   // only if I have no outputs
   //
   if(obj)
   {
      if(!theChain->findObject(PTR_CAST(ossimConnectableObject,event.getObject())))
      {
         ossimConnectableObject* interface = theChain->findObject(obj->getId(), false);
         
         if(interface)
         {
            ossimConnectableObject* parent = PTR_CAST(ossimConnectableObject,
                                                      theChain->getOwner());
            if(parent&&
               !theChain->getNumberOfOutputs())
            {
               parent->propagateEventToOutputs(event);
            }
         }
      }
      else
      {
         if(!theChain->thePropagateEventFlag)
         {
            theChain->propagateEventToOutputs(event);
         }
      }
   }
}
#endif

ossimImageChain::ossimImageChain()
:ossimImageSource(0,
                  0, // number of inputs
                  0, // number of outputs
                  false, // input's fixed
                  false), // outputs are not fixed
    ossimConnectableContainerInterface((ossimObject*)NULL),
    theBlankTile(NULL),
    theLoadStateFlag(false)
{
   ossimConnectableContainerInterface::theBaseObject = this;
   //thePropagateEventFlag = false;
   addListener((ossimConnectableObjectListener*)this);
   theChildListener = new ossimImageChainChildListener(this);
}

ossimImageChain::~ossimImageChain()
{
   removeListener((ossimConnectableObjectListener*)this);
   deleteList();

   if(theChildListener)
   {
      delete theChildListener;
      theChildListener = 0;
   }
}

bool ossimImageChain::addFirst(ossimConnectableObject* obj)
{
   ossimConnectableObject* rightOfThisObj =
      (ossimConnectableObject*)getFirstObject();

   return insertRight(obj, rightOfThisObj);
}

bool ossimImageChain::addLast(ossimConnectableObject* obj)
{
   if(theImageChainList.size() > 0)
   {
      ossimConnectableObject* lastSource = theImageChainList[ theImageChainList.size() -1].get();
//      if(dynamic_cast<ossimImageSource*>(obj)&&lastSource)
         if(lastSource)
      {
//         obj->disconnect();
         ossimConnectableObject::ConnectableObjectList tempIn = getInputList();
         lastSource->disconnectAllInputs();
         lastSource->connectMyInputTo(obj);
         obj->changeOwner(this);
         obj->connectInputList(tempIn);

         tempIn = obj->getInputList();
         theInputListIsFixedFlag = obj->getInputListIsFixedFlag();
         setNumberOfInputs(obj->getNumberOfInputs());
         theImageChainList.push_back(obj);
         
         obj->addListener((ossimConnectableObjectListener*)this);
         // Send an event to any listeners.
         ossimContainerEvent event((ossimObject*)this,
                                   OSSIM_EVENT_ADD_OBJECT_ID);
         event.setObjectList(obj);
         fireEvent(event);
         return true;
      }
   }
   else
   {
      return add(obj);
   }

   return false;;
}

ossimImageSource* ossimImageChain::getFirstSource()
{
   if(theImageChainList.size()>0)
   {
      return dynamic_cast<ossimImageSource*>(theImageChainList[0].get());
   }

   return 0;
}

ossimObject* ossimImageChain::getFirstObject()
{
   if(theImageChainList.size()>0)
   {
      return dynamic_cast<ossimImageSource*>(theImageChainList[0].get());
   }

   return 0;
}

ossimImageSource* ossimImageChain::getLastSource()
{
   if(theImageChainList.size()>0)
   {
      return dynamic_cast<ossimImageSource*>((*(theImageChainList.end()-1)).get());
   }

   return NULL;
}

ossimObject* ossimImageChain::getLastObject()
{
   if(theImageChainList.size()>0)
   {
      return dynamic_cast<ossimImageSource*>((*(theImageChainList.end()-1)).get());
   }

   return 0;
}

ossimConnectableObject* ossimImageChain::findObject(const ossimId& id,
                                                    bool /* recurse */)
{
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).get())
      {
         if(id == (*current)->getId())
         {
            return (*current).get();
         }
      }
      
      ++current;
   }
   
   current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface,
                                                         (*current).get());
      
      if(child)
      {
         ossimConnectableObject* object = child->findObject(id, true);

         if(object) return object;
      }
      ++current;
   }
   
   return NULL;
}

ossimConnectableObject* ossimImageChain::findObject(const ossimConnectableObject* obj,
                                                    bool /* recurse */)
{
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid())
      {
         if(obj == (*current).get())
         {
            return (*current).get();
         }
      }
      
      ++current;
   }
   
   current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).get());
      
      if(child)
      {
         ossimConnectableObject* object = child->findObject(obj, true);

         if(object) return object;
      }
      ++current;
   }
   
   return 0;
}

ossimConnectableObject* ossimImageChain::findFirstObjectOfType(const RTTItypeid& typeInfo,
                                                               bool recurse)
{
   vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid()&&
         (*current)->canCastTo(typeInfo))
      {
         return (*current).get();
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, 
                                                            (*current).get());
         
         if(child)
         {
            ossimConnectableObject* temp = child->findFirstObjectOfType(typeInfo, recurse);
            if(temp)
            {
               return temp;
            }
         }
         ++current;
      }
   }
   
   return (ossimConnectableObject*)NULL;   
}

ossimConnectableObject* ossimImageChain::findFirstObjectOfType(const ossimString& className,
                                                               bool recurse)
{
   vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid()&&
         (*current)->canCastTo(className) )
      {
         return (*current).get();
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).get());
         
         if(child)
         {
            ossimConnectableObject* temp = child->findFirstObjectOfType(className, recurse);
            if(temp)
            {
               return temp;
            }
         }
         ++current;
      }
   }
   
   return (ossimConnectableObject*)0;   
}

ossimConnectableObject::ConnectableObjectList ossimImageChain::findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                                           bool recurse)
{
   ossimConnectableObject::ConnectableObjectList result;
   ossimConnectableObject::ConnectableObjectList::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid()&&
         (*current)->canCastTo(typeInfo))
      {
         ossimConnectableObject::ConnectableObjectList::iterator iter = std::find(result.begin(), 
                                                                                      result.end(), 
                                                                                      (*current).get());
         if(iter == result.end())
         {
            result.push_back((*current).get());
         }
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).get());
         
         if(child)
         {
            ossimConnectableObject::ConnectableObjectList temp;
            temp = child->findAllObjectsOfType(typeInfo, recurse);
            for(long index=0; index < (long)temp.size();++index)
            {
               ossimConnectableObject::ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), temp[index]);
               if(iter == result.end())
               {
                  result.push_back(temp[index]);
               }
            }
         }
         ++current;
      }
   }
   
   return result;
   
}

ossimConnectableObject::ConnectableObjectList ossimImageChain::findAllObjectsOfType(const ossimString& className,
                                                                           bool recurse)
{
   ossimConnectableObject::ConnectableObjectList result;
   ossimConnectableObject::ConnectableObjectList::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current).valid()&&
         (*current)->canCastTo(className))
      {
         ossimConnectableObject::ConnectableObjectList::iterator iter = std::find(result.begin(), 
                                                                         result.end(), 
                                                                         (*current).get());
         if(iter == result.end())
         {
            result.push_back((*current).get());
         }
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current).get());
         
         if(child)
         {
            ossimConnectableObject::ConnectableObjectList temp;
            temp = child->findAllObjectsOfType(className, recurse);
            for(long index=0; index < (long)temp.size();++index)
            {
               ossimConnectableObject::ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), temp[index]);
               if(iter == result.end())
               {
                  result.push_back(temp[index]);
               }
            }
         }
         ++current;
      }
   }
   
   return result;
   
}

void ossimImageChain::makeUniqueIds()
{
   setId(ossimIdManager::instance()->generateId());
   for(long index = 0; index < (long)theImageChainList.size(); ++index)
   {
      ossimConnectableContainerInterface* container = PTR_CAST(ossimConnectableContainerInterface,
                                                               theImageChainList[index].get());
      if(container)
      {
         container->makeUniqueIds();
      }
      else
      {
         if(theImageChainList[index].valid())
         {
            theImageChainList[index]->setId(ossimIdManager::instance()->generateId());
         }
      }
   }
}

ossim_uint32 ossimImageChain::getNumberOfObjects(bool recurse)const
{
   ossim_uint32 result = (ossim_uint32)theImageChainList.size();
   
   if(recurse)
   {
      for(ossim_uint32 i = 0; i < theImageChainList.size(); ++i)
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, theImageChainList[i].get());
         if(child)
         {
            result += child->getNumberOfObjects(true);
         }
      }
   }
   
   return result;   
}

ossim_uint32 ossimImageChain::getNumberOfSources() const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimImageChain::getNumberOfSources is deprecated!"
      << "\nUse: ossimImageChain::getNumberOfObjects(false)"
      << std::endl;
   return getNumberOfObjects(false);
}

bool ossimImageChain::addChild(ossimConnectableObject* object)
{
   return add(object);
}

bool ossimImageChain::removeChild(ossimConnectableObject* object)
{
   bool result = false;
   vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  std::find(theImageChainList.begin(), theImageChainList.end(), object);
   
   
   if(current!=theImageChainList.end())
   {
      result = true;
      object->removeListener((ossimConnectableObjectListener*)this);
      if(current == theImageChainList.begin())
      {
         object->removeListener((ossimConnectableObjectListener*)theChildListener);
      }
      if(theImageChainList.size() == 1)
      {
         object->changeOwner(0);
         current = theImageChainList.erase(current);
      }
      else 
      {
         ossimConnectableObject::ConnectableObjectList input  = object->getInputList();
         ossimConnectableObject::ConnectableObjectList output = object->getOutputList();
         object->changeOwner(0);// set the owner to 0
         bool erasingBeginning = (current == theImageChainList.begin());
         bool erasingEnd = (current+1) == theImageChainList.end();
         current = theImageChainList.erase(current);
         object->disconnect();
         
         if(!theImageChainList.empty())
         {
            if(erasingBeginning) // the one that receives the first getTile
            {
               (*theImageChainList.begin())->addListener(theChildListener);
            }
            
            else if(current==theImageChainList.end()) // one that receives the last getTIle
            {
               current = theImageChainList.begin()+(theImageChainList.size()-1);
               (*current)->connectInputList(input);
               theInputObjectList = (*current)->getInputList();
               theInputListIsFixedFlag = (*current)->getInputListIsFixedFlag();
            }
            else
            {
               // prepare interior setup and removal and connect surrounding nodes
               // take the  outputs of the node we are removing and connect them to the old inputs 
               ossim_uint32 outIndex = 0;
               for(outIndex = 0; outIndex < output.size();++outIndex)
               {
                  output[outIndex]->connectInputList(input);
               }
            }
         }
      }

      // Send an event to any listeners.
      ossimContainerEvent event((ossimObject*)this,
                                OSSIM_EVENT_REMOVE_OBJECT_ID);
      event.setObjectList(object);
      fireEvent(event);
   }
   
   return result;
}

ossimConnectableObject* ossimImageChain::removeChild(const ossimId& id)
{
   ossimConnectableObject* obj = findObject(id, true);

   removeChild(obj);
   
   return obj;
}

void ossimImageChain::getChildren(vector<ossimConnectableObject*>& children,
                                  bool immediateChildrenOnlyFlag)
{
   ossim_uint32 i = 0;
   
   vector<ossimConnectableObject*> temp;
   for(i = 0; i < theImageChainList.size();++i)
   {
      temp.push_back(theImageChainList[i].get());
   }

   for(i = 0; i < temp.size();++i)
   {
      ossimConnectableContainerInterface* interface = PTR_CAST(ossimConnectableContainerInterface,
                                                               temp[i]);
      if(immediateChildrenOnlyFlag)
      {
         children.push_back(temp[i]);
      }
      else if(!interface)
      {
         children.push_back(temp[i]);         
      }
   }
   
   if(!immediateChildrenOnlyFlag)
   {
      for(i = 0; i < temp.size();++i)
      {
         ossimConnectableContainerInterface* interface = PTR_CAST(ossimConnectableContainerInterface,
                                                                  temp[i]);
         if(interface)
         {
            interface->getChildren(children, false);
         }
      }
   }
}

bool ossimImageChain::add(ossimConnectableObject* source)
{
   bool result = false;
//   if(PTR_CAST(ossimImageSource, source))
   {
     source->changeOwner(this);
     if(theImageChainList.size() > 0)
     {
        source->disconnectAllOutputs();
        theOutputListIsFixedFlag = source->getOutputListIsFixedFlag();
        theImageChainList[0]->removeListener(theChildListener);
        theImageChainList.insert(theImageChainList.begin(), source);
        theImageChainList[0]->addListener(theChildListener);
        source->addListener((ossimConnectableObjectListener*)this);
        theImageChainList[0]->connectMyInputTo(theImageChainList[1].get());
        result = true;
     }
     else
     {
        theInputListIsFixedFlag = false;
        theOutputListIsFixedFlag = false;
        
        if(!theInputObjectList.empty())
        {
           source->connectInputList(getInputList());
        }
        theInputObjectList = source->getInputList();
        theInputListIsFixedFlag = source->getInputListIsFixedFlag();
     //   theOutputObjectList     = source->getOutputList();
     //   theOutputListIsFixedFlag= source->getOutputListIsFixedFlag();
        theImageChainList.push_back(source);
        source->addListener((ossimConnectableObjectListener*)this);
        source->addListener(theChildListener);
        result = true;
     }
   }

   if (result && source)
   {
      ossimContainerEvent event(this, OSSIM_EVENT_ADD_OBJECT_ID);
      event.setObjectList(source);
      fireEvent(event);
   }
   
   return result;
}

bool ossimImageChain::insertRight(ossimConnectableObject* newObj,
                                  ossimConnectableObject* rightOfThisObj)
{
   if(!newObj&&!rightOfThisObj) return false;
   if(!theImageChainList.size())
   {
      return add(newObj);
   }
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator iter = std::find(theImageChainList.begin(), theImageChainList.end(), rightOfThisObj);
   
   if(iter!=theImageChainList.end())
   {
      if(iter == theImageChainList.begin())
      {
         return add(newObj);
      }
      else //if(PTR_CAST(ossimImageSource, newObj))
      {
         ossimConnectableObject::ConnectableObjectList outputList = rightOfThisObj->getOutputList();
         rightOfThisObj->disconnectAllOutputs();

         // Core dump fix.  Connect input prior to outputs. (drb)
         newObj->connectMyInputTo(rightOfThisObj); 
         newObj->connectOutputList(outputList);
         newObj->changeOwner(this);
         newObj->addListener((ossimConnectableObjectListener*)this);
         theImageChainList.insert(iter, newObj);
         // Send event to any listeners.
         ossimContainerEvent event(this, OSSIM_EVENT_ADD_OBJECT_ID);
         event.setObjectList(newObj);
         fireEvent(event);
         return true;
      }
   }

   return false;
}

bool ossimImageChain::insertRight(ossimConnectableObject* newObj,
                                  const ossimId& id)
{
   ossimConnectableObject* obj = findObject(id, false);
   if(obj)
   {
      return insertRight(newObj, obj);
   }

   return false;
}

bool ossimImageChain::insertLeft(ossimConnectableObject* newObj,
                                 ossimConnectableObject* leftOfThisObj)
{
   if(!newObj&&!leftOfThisObj) return false;
   if(!theImageChainList.size())
   {
      return add(newObj);
   }
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator iter = std::find(theImageChainList.begin(), theImageChainList.end(), leftOfThisObj);
   if(iter!=theImageChainList.end())
   {
      if((iter+1)==theImageChainList.end())
      {
         return addLast(newObj);
      }
      else
      {
         ossimConnectableObject::ConnectableObjectList inputList = leftOfThisObj->getInputList();
         
         newObj->connectInputList(inputList);
         
         leftOfThisObj->disconnectAllInputs();
         leftOfThisObj->connectMyInputTo(newObj);
         newObj->changeOwner(this);
         newObj->addListener((ossimConnectableObjectListener*)this);
         theImageChainList.insert(iter+1, newObj);
         // Send an event to any listeners.
         ossimContainerEvent event(this, OSSIM_EVENT_ADD_OBJECT_ID);
         event.setObjectList(newObj);
         fireEvent(event);
         return true;
      }
   }

   return false;
}

bool ossimImageChain::insertLeft(ossimConnectableObject* newObj,
                                  const ossimId& id)
{
   ossimConnectableObject* obj = findObject(id, false);
   if(obj)
   {
      return insertLeft(newObj, obj);
   }

   return false;
}

bool ossimImageChain::replace(ossimConnectableObject* newObj,
                              ossimConnectableObject* oldObj)
{
   ossim_int32 idx = indexOf(oldObj);
   if(idx >= 0)
   {
      ossimConnectableObject::ConnectableObjectList& inputList  = oldObj->getInputList();
      ossimConnectableObject::ConnectableObjectList& outputList = oldObj->getOutputList();
      oldObj->removeListener((ossimConnectableObjectListener*)this);
      oldObj->removeListener(theChildListener);
      oldObj->changeOwner(0);
      theImageChainList[idx] = newObj;
      newObj->connectInputList(inputList);
      newObj->connectOutputList(outputList);
      newObj->changeOwner(this);
      newObj->addListener((ossimConnectableObjectListener*)this);
      if(idx == 0)
      {
         newObj->addListener(theChildListener);
      }
   }
   
   return (idx >= 0);
}

ossimRefPtr<ossimImageData> ossimImageChain::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* inputSource = PTR_CAST(ossimImageSource,
                                             theImageChainList[0].get());
      
      if(inputSource)
      {
         // make sure we initialize in reverse order.
         // some source may depend on the initialization of
         // its inputs
         return inputSource->getTile(tileRect, resLevel);
      }  
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* inputSource = PTR_CAST(ossimImageSource, getInput(0));
         if(inputSource)
         {
            ossimRefPtr<ossimImageData> inputTile = inputSource->getTile(tileRect, resLevel);
//            if(inputTile.valid())
//            {
//               std::cout << *(inputTile.get()) << std::endl;
//            }
            return inputTile.get();
         }
      }
   }
//   std::cout << "RETURNING A BLANK TILE!!!!" << std::endl;
/*
   if(theBlankTile.get())
   {
      theBlankTile->setImageRectangle(tileRect);
   }
   return theBlankTile;
 */
   return 0;
}

ossim_uint32 ossimImageChain::getNumberOfInputBands() const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         return interface->getNumberOfOutputBands();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getNumberOfOutputBands();
         }
      }
   }

   return 0;
}

double ossimImageChain::getNullPixelValue(ossim_uint32 band)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         return interface->getNullPixelValue(band);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getNullPixelValue(band);
         }
      }
   }
   
   return ossim::defaultNull(getOutputScalarType());
}

double ossimImageChain::getMinPixelValue(ossim_uint32 band)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         return interface->getMinPixelValue(band);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getMinPixelValue(band);
         }
      }
   }

   return ossim::defaultMin(getOutputScalarType());
}

double ossimImageChain::getMaxPixelValue(ossim_uint32 band)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                                  theImageChainList[0].get());
      if(inter)
      {
         return inter->getMaxPixelValue(band);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getMaxPixelValue(band);
         }
      }
   }

   return ossim::defaultMax(getOutputScalarType());
}

void ossimImageChain::getOutputBandList(std::vector<ossim_uint32>& bandList) const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                                  theImageChainList[0].get());
      if(inter)
      {
         return inter->getOutputBandList(bandList);
      }
   }
}
   
ossimScalarType ossimImageChain::getOutputScalarType() const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         return interface->getOutputScalarType();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getOutputScalarType();
         }
      }
   }
   
   return OSSIM_SCALAR_UNKNOWN;
}

ossim_uint32 ossimImageChain::getTileWidth()const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         theImageChainList[0].get());
         if(interface)
         {
            return interface->getTileWidth();;
         }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getTileWidth();
         }
      }
   }
   
   return 0;   
}

ossim_uint32 ossimImageChain::getTileHeight()const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         theImageChainList[0].get());
         if(interface)
         {
            return interface->getTileHeight();
         }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getTileHeight();
         }
      }
   }
   
   return 0;   
}
   
ossimIrect ossimImageChain::getBoundingRect(ossim_uint32 resLevel)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {

      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());

      if(interface)
      {
         return interface->getBoundingRect(resLevel);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getBoundingRect();
         }
      }
   }
   ossimDrect rect;
   rect.makeNan();
   
   return rect;
   
}

void ossimImageChain::getValidImageVertices(vector<ossimIpt>& validVertices,
                                            ossimVertexOrdering ordering,
                                            ossim_uint32 resLevel)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface =PTR_CAST(ossimImageSource,
                                                     theImageChainList[0].get());

      if(interface)
      {
         interface->getValidImageVertices(validVertices,
                                          ordering,
                                          resLevel);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            interface->getValidImageVertices(validVertices,
                                             ordering,
                                             resLevel);
         }
      }
   }
}

ossimRefPtr<ossimImageGeometry> ossimImageChain::getImageGeometry()
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource, theImageChainList[0].get());
      if( interface )
      {
         return interface->getImageGeometry();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource, getInput(0));
         if(interface)
         {
            return interface->getImageGeometry();
         }
      }
   }
   return ossimRefPtr<ossimImageGeometry>();
}

void ossimImageChain::getDecimationFactor(ossim_uint32 resLevel,
                                          ossimDpt& result) const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         interface->getDecimationFactor(resLevel,
                                        result);
         return;
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            interface->getDecimationFactor(resLevel, result);
            return;
         }
      }
   }

   result.makeNan();
}

void ossimImageChain::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());
      if(interface)
      {
         interface->getDecimationFactors(decimations);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            interface->getDecimationFactors(decimations);
            return;
         }
      }
   }
}

ossim_uint32 ossimImageChain::getNumberOfDecimationLevels()const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      theImageChainList[0].get());

      if(interface)
      {
         return interface->getNumberOfDecimationLevels();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         getInput(0));
         if(interface)
         {
            return interface->getNumberOfDecimationLevels();
         }
      }
   }

   return 1;
}

bool ossimImageChain::addAllSources(map<ossimId, vector<ossimId> >& idMapping,
                                    const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   static const char* MODULE = "ossimImageChain::addAllSources";
   ossimString copyPrefix = prefix;
   bool result =  ossimImageSource::loadState(kwl, copyPrefix.c_str());

   if(!result)
   {
      return result;
   }
   long index = 0;

//   ossimSource* source = NULL;

   vector<ossimId> inputConnectionIds;
   ossimString regExpression =  ossimString("^(") + copyPrefix + "object[0-9]+.)";
   vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   long numberOfSources = (long)keys.size();//kwl.getNumberOfSubstringKeys(regExpression);

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
         CLOG << "trying to create source with prefix: " << newPrefix
              << std::endl;
      }
      ossimRefPtr<ossimObject> object = ossimObjectFactoryRegistry::instance()->createObject(kwl,
                                                                                 newPrefix.c_str());
      ossimConnectableObject* source = PTR_CAST(ossimConnectableObject, object.get());
      
      if(source)
      {
         // we did find a source so include it in the count
         if(traceDebug())
         {
            CLOG << "Created source with prefix: " << newPrefix << std::endl;
         }
         //if(PTR_CAST(ossimImageSource, source))
         {
            ossimId id = source->getId();
            inputConnectionIds.clear();
            
            findInputConnectionIds(inputConnectionIds,
                                   kwl,
                                   newPrefix);
            if(inputConnectionIds.size() == 0)
            {
               // we will try to do a default connection
               //
               
               if(theImageChainList.size())
               {
                  if(traceDebug())
                  {
                     CLOG << "connecting " << source->getClassName() << " to "
                          << theImageChainList[0]->getClassName() << std::endl;
                  }
                  source->connectMyInputTo(0, theImageChainList[0].get());
               }
            }
            else
            {
               // we remember the connection id's so we can connect this later.
               // this way we make sure all sources were actually
               // allocated.
               //
               idMapping.insert(std::make_pair(id, inputConnectionIds));
            }
            add(source);
         }
      //   else
      //   {
            source = 0;
      //   }
      }
      else
      {
         object = 0;
         source = 0;
      }
      
      ++index;
   }
   if(theImageChainList.size())
   {
     ossimConnectableObject* obj = theImageChainList[(ossim_int32)theImageChainList.size()-1].get();
     if(obj)
     {
        setNumberOfInputs(obj->getNumberOfInputs());
     }
   }
   
   return result;
}

void ossimImageChain::findInputConnectionIds(vector<ossimId>& result,
                                             const ossimKeywordlist& kwl,
                                             const char* prefix)
{
   ossimString copyPrefix = prefix;
   ossim_uint32 idx = 0;
   
   ossimString regExpression =  ossimString("^") + ossimString(prefix) + "input_connection[0-9]+";
   vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   
   ossim_int32 offset = (ossim_int32)(copyPrefix+"input_connection").size();
   ossim_uint32 numberOfKeys = (ossim_uint32)keys.size();
   std::vector<int> theNumberList(numberOfKeys);
   for(idx = 0; idx < theNumberList.size();++idx)
   {
      ossimString numberStr(keys[idx].begin() + offset,
                            keys[idx].end());
      theNumberList[idx] = numberStr.toInt();
   }
   std::sort(theNumberList.begin(), theNumberList.end());
   copyPrefix += ossimString("input_connection");
   for(idx=0;idx < theNumberList.size();++idx)
   {
      const char* lookup = kwl.find(copyPrefix,ossimString::toString(theNumberList[idx]));
      if(lookup)
      {
         long id = ossimString(lookup).toLong();
         result.push_back(ossimId(id));
      }
   }
}
                    

bool ossimImageChain::connectAllSources(const map<ossimId, vector<ossimId> >& idMapping)
{
   if(idMapping.size())
   {
      map<ossimId, vector<ossimId> >::const_iterator iter = idMapping.begin();

      while(iter != idMapping.end())
      {
         ossimConnectableObject* currentSource = findObject((*iter).first);

         if(currentSource)
         {
            long upperBound = (long)(*iter).second.size();
            for(long index = 0; index < upperBound; ++index)
            {
               if((*iter).second[index].getId() > -1)
               {
                  ossimConnectableObject* inputSource = PTR_CAST(ossimConnectableObject, findObject((*iter).second[index]));
                  currentSource->connectMyInputTo(index, inputSource);
                  
               }
               else
               {
                  currentSource->disconnectMyInput((ossim_int32)index);
               }
            }
         }
         else
         {
            cerr << "Could not find " << (*iter).first << " for source: ";
            return false;
         }
         ++iter;
      }
   }

   return true;
}

bool ossimImageChain::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   bool result = true;
   
   result = ossimImageSource::saveState(kwl, prefix);

   if(!result)
   {
      return result;
   }
   ossim_uint32 upper = (ossim_uint32)theImageChainList.size();
   ossim_uint32 counter = 1;

   if (upper)
   {
      ossim_int32 idx = upper-1;
      // start with the tail and go to the head fo the list.
      for(;((idx >= 0)&&result);--idx, ++counter)
      {
         ossimString newPrefix = prefix;
         
         newPrefix += (ossimString("object") +
                       ossimString::toString(counter) +
                       ossimString("."));
         result = theImageChainList[idx]->saveState(kwl, newPrefix.c_str());
      }
   }

   return result;
}

bool ossimImageChain::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   static const char* MODULE = "ossimImageChain::loadState(kwl, prefix)";
   deleteList();

   ossimImageSource::loadState(kwl, prefix);
   
   
   theLoadStateFlag = true;
   bool result = true;
   
   map<ossimId, vector<ossimId> > idMapping;
   result = addAllSources(idMapping, kwl, prefix);
   if(!result)
   {
      CLOG << "problems adding sources" << std::endl;
   }
   result = connectAllSources(idMapping);
   if(!result)
   {
      CLOG << "problems connecting sources" << std::endl;
   }
   
   theLoadStateFlag = false;
   return result;
}


void ossimImageChain::initialize()
{
   static const char* MODULE = "ossimImageChain::initialize()";
   if (traceDebug()) CLOG << " Entered..." << std::endl;
   
   long upper = (ossim_uint32)theImageChainList.size();
   
   for(long index = upper - 1; index >= 0; --index)
   {
      if(traceDebug())
      {
         CLOG << "initilizing source: "
              << theImageChainList[index]->getClassName()
              << std::endl;
      }
      if(theImageChainList[index].valid())
      {
         ossimSource* interface =
            PTR_CAST(ossimSource, theImageChainList[index].get());

         if(interface)
         {
            // make sure we initialize in reverse order.
            // some source may depend on the initialization of
            // its inputs
            interface->initialize();
         }
      }
   }
   if (traceDebug()) CLOG << " Exited..." << std::endl;
}

void ossimImageChain::enableSource()
{
   ossim_int32 upper = static_cast<ossim_int32>(theImageChainList.size());
   ossim_int32 index = 0;
   for(index = upper - 1; index >= 0; --index)
   {
      // make sure we initialize in reverse order.
      // some source may depend on the initialization of
      // its inputs
     ossimSource* source = PTR_CAST(ossimSource, theImageChainList[index].get());
     if(source)
     {
        source->enableSource();
     }
   }
   
   theEnableFlag = true;
}

void ossimImageChain::disableSource()
{
   long upper = (ossim_uint32)theImageChainList.size();
   
   for(long index = upper - 1; index >= 0; --index)
   {
      // make sure we initialize in reverse order.
      // some source may depend on the initialization of
      // its inputs
     ossimSource* source = PTR_CAST(ossimSource, theImageChainList[index].get());
     if(source)
     {
        source->disableSource();
     }
   }
   
   theEnableFlag = false;
}

void ossimImageChain::prepareForRemoval(ossimConnectableObject* connectableObject)
{
   if(connectableObject)
   {
      connectableObject->removeListener((ossimConnectableObjectListener*)this);
      connectableObject->removeListener(theChildListener);
      connectableObject->changeOwner(0);
      connectableObject->disconnect();   
   }
}

bool ossimImageChain::deleteFirst()
{
   if (theImageChainList.size() == 0) return false;

   ossimContainerEvent event(this, OSSIM_EVENT_REMOVE_OBJECT_ID);
   prepareForRemoval(theImageChainList[0].get());
   // Clear any listeners, memory.
   event.setObjectList(theImageChainList[0].get());
   theImageChainList[0] = 0;
   // Remove from the vector.
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator i = theImageChainList.begin();
   theImageChainList.erase(i);
   fireEvent(event);
   return true;
}

bool ossimImageChain::deleteLast()
{
   if (theImageChainList.size() == 0) return false;

   ossimContainerEvent event(this, OSSIM_EVENT_REMOVE_OBJECT_ID);
  // Clear any listeners, memory.
   ossim_uint32 idx = (ossim_uint32)theImageChainList.size() - 1;
   prepareForRemoval(theImageChainList[idx].get());
   event.setObjectList(theImageChainList[idx].get());
   theImageChainList[idx] = 0;
   // Remove from the vector.
   theImageChainList.pop_back();
   fireEvent(event);
   return true; 
}

void ossimImageChain::deleteList()
{
   ossim_uint32 upper = theImageChainList.size();
   ossim_uint32 idx = 0;
   ossimContainerEvent event(this, OSSIM_EVENT_REMOVE_OBJECT_ID);
   for(; idx < upper; ++idx)
   {
      if(theImageChainList[idx].valid())
      {
         event.getObjectList().push_back(theImageChainList[idx].get());
         prepareForRemoval(theImageChainList[idx].get());
         theImageChainList[idx] = 0;
      }
   }
  
   theImageChainList.clear();
   fireEvent(event);
}


void ossimImageChain::disconnectInputEvent(ossimConnectionEvent& event)
{
   if(theImageChainList.size())
   {
      if(event.getObject()==this)
      {
         if(theImageChainList[theImageChainList.size()-1].valid())
         {
            for(ossim_uint32 i = 0; i < event.getNumberOfOldObjects(); ++i)
            {
               theImageChainList[theImageChainList.size()-1]->disconnectMyInput(event.getOldObject(i));
            }
         }
      }
   }
}

void ossimImageChain::disconnectOutputEvent(ossimConnectionEvent& /* event */)
{
}

void ossimImageChain::connectInputEvent(ossimConnectionEvent& event)
{
   if(theImageChainList.size())
   {
      if(event.getObject()==this)
      {
         if(theImageChainList[theImageChainList.size()-1].valid())
         {
            for(ossim_uint32 i = 0; i < event.getNumberOfNewObjects(); ++i)
            {
               ossimConnectableObject* obj = event.getNewObject(i);
               theImageChainList[theImageChainList.size()-1]->connectMyInputTo(findInputIndex(obj),
                                                                               obj,
                                                                               false);
            }
         }
      }
      else if(event.getObject() == theImageChainList[0].get())
      {
         if(!theLoadStateFlag)
         {
//            theInputObjectList = theImageChainList[0]->getInputList();
         }
      }
      initialize();
   }
}

void ossimImageChain::connectOutputEvent(ossimConnectionEvent& /* event */)
{
} 

// void ossimImageChain::propertyEvent(ossimPropertyEvent& event)
// {
//    if(theImageChainList.size())
//    {
//       ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject,
//                                              event.getObject());
      
//       if(obj)
//       {
//          ossimImageSource* interface = findSource(obj->getId());
         
//          if(interface)
//          {
//             ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject,
//                                                    interface.getObject());
//             if(obj)
//             {
               
//             }
//          }
//       }
//    }
// }

void ossimImageChain::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if(!event.getObject()) return;

   if(theImageChainList.size()&&(event.getObject()!=this))
   {
      removeChild(PTR_CAST(ossimConnectableObject,
                           event.getObject()));
   }
}
void ossimImageChain::propagateEventToOutputs(ossimEvent& event)
{
   //if(thePropagateEventFlag) return;

   //thePropagateEventFlag = true;
   if(theImageChainList.size())
   {
      if(theImageChainList[theImageChainList.size()-1].valid())
      {
         theImageChainList[theImageChainList.size()-1]->fireEvent(event);
         theImageChainList[theImageChainList.size()-1]->propagateEventToOutputs(event);
      }
   }
   //ossimConnectableObject::propagateEventToOutputs(event);
  // thePropagateEventFlag = false;
}
void ossimImageChain::propagateEventToInputs(ossimEvent& event)
{
//   if(thePropagateEventFlag) return;

//   thePropagateEventFlag = true;
   if(theImageChainList.size())
   {
      if(theImageChainList[0].valid())
      {
         theImageChainList[0]->fireEvent(event);
         theImageChainList[0]->propagateEventToInputs(event);
      }
   }
//   thePropagateEventFlag = false;
}

ossimConnectableObject* ossimImageChain::operator[](ossim_uint32 index)
{
   return getConnectableObject(index);
}

ossimConnectableObject* ossimImageChain::getConnectableObject(
   ossim_uint32 index)
{
   if(theImageChainList.size() && (index < theImageChainList.size()))
   {
      return theImageChainList[index].get();
   }
   
   return 0; 
}

ossim_int32 ossimImageChain::indexOf(ossimConnectableObject* obj)const
{
   ossim_uint32 idx = 0;
   
   for(idx = 0; idx < theImageChainList.size();++idx)
   {
      if(theImageChainList[idx] == obj)
      {
         return (ossim_int32)idx;
      }
   }
   
   return -1;
}

void ossimImageChain::accept(ossimVisitor& visitor)
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
         ConnectableObjectList::reverse_iterator current = theImageChainList.rbegin();
         while((current != theImageChainList.rend())&&!visitor.stopTraversal())
         {
            ossimRefPtr<ossimConnectableObject> currentObject = (*current);
            if(currentObject.get() && !visitor.hasVisited(currentObject.get())) currentObject->accept(visitor);
            ++current;
         }
      }
      visitor.setVisitorType(currentType);
      ossimConnectableObject::accept(visitor);
   }
}