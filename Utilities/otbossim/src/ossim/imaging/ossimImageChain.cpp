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
// $Id: ossimImageChain.cpp 21850 2012-10-21 20:09:55Z dburken $

#include <ossim/imaging/ossimImageChain.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimConnectableContainer.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimIdManager.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <algorithm>
#include <iostream>
#include <iterator>

static ossimTrace traceDebug("ossimImageChain");


RTTI_DEF3(ossimImageChain, "ossimImageChain", ossimImageSource,
          ossimConnectableObjectListener, ossimConnectableContainerInterface);

void ossimImageChain::processEvent(ossimEvent& event)
{
   ossimConnectableObjectListener::processEvent(event);
   ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject, event.getCurrentObject());
   
   if((ossimConnectableObject*)getFirstSource() == obj)
   {
      if(event.isPropagatingToOutputs())
      {
         ossimConnectableObject::ConnectableObjectList& outputList = getOutputList();
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
}

ossimImageChain::~ossimImageChain()
{
   removeListener((ossimConnectableObjectListener*)this);
   deleteList();
}

bool ossimImageChain::addFirst(ossimConnectableObject* obj)
{
   ossimConnectableObject* rightOfThisObj =
      (ossimConnectableObject*)getFirstObject();

   return insertRight(obj, rightOfThisObj);
}

bool ossimImageChain::addLast(ossimConnectableObject* obj)
{
   if(imageChainList().size() > 0)
   {
      ossimConnectableObject* lastSource = imageChainList()[ imageChainList().size() -1].get();
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
         imageChainList().push_back(obj);
         
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
   if(imageChainList().size()>0)
   {
      return dynamic_cast<ossimImageSource*>(imageChainList()[0].get());
   }

   return 0;
}

const ossimImageSource* ossimImageChain::getFirstSource() const
{
   if(imageChainList().size()>0)
      return dynamic_cast<const ossimImageSource*>(imageChainList()[0].get());

   return 0;
}

ossimObject* ossimImageChain::getFirstObject()
{
   if(imageChainList().size()>0)
   {
      return dynamic_cast<ossimImageSource*>(imageChainList()[0].get());
   }

   return 0;
}

ossimImageSource* ossimImageChain::getLastSource()
{
   if(imageChainList().size()>0)
   {
      return dynamic_cast<ossimImageSource*>((*(imageChainList().end()-1)).get());
   }

   return NULL;
}

const ossimImageSource* ossimImageChain::getLastSource() const
{
   if(imageChainList().size()>0)
      return dynamic_cast<const ossimImageSource*>((*(imageChainList().end()-1)).get());

   return NULL;
}

ossimObject* ossimImageChain::getLastObject()
{
   if(imageChainList().size()>0)
   {
      return dynamic_cast<ossimImageSource*>((*(imageChainList().end()-1)).get());
   }

   return 0;
}

ossimConnectableObject* ossimImageChain::findObject(const ossimId& id,
                                                    bool /* recurse */)
{
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  imageChainList().begin();
   
   while(current != imageChainList().end())
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
   
   current =  imageChainList().begin();
   
   while(current != imageChainList().end())
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
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  imageChainList().begin();
   
   while(current != imageChainList().end())
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
   
   current =  imageChainList().begin();
   
   while(current != imageChainList().end())
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
   vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  imageChainList().begin();
   
   while(current != imageChainList().end())
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
      current =  imageChainList().begin();
      while(current != imageChainList().end())
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
   vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  imageChainList().begin();
   
   while(current != imageChainList().end())
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
      current =  imageChainList().begin();
      while(current != imageChainList().end())
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
   ossimConnectableObject::ConnectableObjectList::iterator current =  imageChainList().begin();
   
   while(current != imageChainList().end())
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
      current =  imageChainList().begin();
      while(current != imageChainList().end())
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
   ossimConnectableObject::ConnectableObjectList::iterator current =  imageChainList().begin();
   
   while(current != imageChainList().end())
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
      current =  imageChainList().begin();
      while(current != imageChainList().end())
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
   for(long index = 0; index < (long)imageChainList().size(); ++index)
   {
      ossimConnectableContainerInterface* container = PTR_CAST(ossimConnectableContainerInterface,
                                                               imageChainList()[index].get());
      if(container)
      {
         container->makeUniqueIds();
      }
      else
      {
         if(imageChainList()[index].valid())
         {
            imageChainList()[index]->setId(ossimIdManager::instance()->generateId());
         }
      }
   }
}

ossim_uint32 ossimImageChain::getNumberOfObjects(bool recurse)const
{
   ossim_uint32 result = (ossim_uint32)imageChainList().size();
   
   if(recurse)
   {
      for(ossim_uint32 i = 0; i < imageChainList().size(); ++i)
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, imageChainList()[i].get());
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
   vector<ossimRefPtr<ossimConnectableObject> >::iterator current =  std::find(imageChainList().begin(), imageChainList().end(), object);
   
   
   if(current!=imageChainList().end())
   {
      result = true;
      object->removeListener((ossimConnectableObjectListener*)this);
      if(current == imageChainList().begin())
      {
         object->removeListener((ossimConnectableObjectListener*)this);
      }
      if(imageChainList().size() == 1)
      {
         object->changeOwner(0);
         current = imageChainList().erase(current);
      }
      else 
      {
         ossimConnectableObject::ConnectableObjectList input  = object->getInputList();
         ossimConnectableObject::ConnectableObjectList output = object->getOutputList();
         object->changeOwner(0);// set the owner to 0
         bool erasingBeginning = (current == imageChainList().begin());
         // bool erasingEnd = (current+1) == imageChainList().end();
         current = imageChainList().erase(current);
         object->disconnect();
         
         if(!imageChainList().empty())
         {
            if(erasingBeginning) // the one that receives the first getTile
            {
               (*imageChainList().begin())->addListener(this);
            }
            
            else if(current==imageChainList().end()) // one that receives the last getTIle
            {
               current = imageChainList().begin()+(imageChainList().size()-1);
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
   ossimIdVisitor visitor( id,
                           (ossimVisitor::VISIT_CHILDREN|ossimVisitor::VISIT_INPUTS ) );
   accept( visitor );
   ossimConnectableObject* obj = visitor.getObject();
   if ( obj )
   {
      removeChild(obj);
   }
   return obj;
}

void ossimImageChain::getChildren(vector<ossimConnectableObject*>& children,
                                  bool immediateChildrenOnlyFlag)
{
   ossim_uint32 i = 0;
   
   vector<ossimConnectableObject*> temp;
   for(i = 0; i < imageChainList().size();++i)
   {
      temp.push_back(imageChainList()[i].get());
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
     if(imageChainList().size() > 0)
     {
        source->disconnectAllOutputs();
        theOutputListIsFixedFlag = source->getOutputListIsFixedFlag();
        imageChainList()[0]->removeListener(this);
        imageChainList().insert(imageChainList().begin(), source);
        imageChainList()[0]->addListener(this);
        source->addListener((ossimConnectableObjectListener*)this);
        imageChainList()[0]->connectMyInputTo(imageChainList()[1].get());
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
        imageChainList().push_back(source);
        source->addListener((ossimConnectableObjectListener*)this);
        source->addListener(this);
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
   if(!imageChainList().size())
   {
      return add(newObj);
   }
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator iter = std::find(imageChainList().begin(), imageChainList().end(), rightOfThisObj);
   
   if(iter!=imageChainList().end())
   {
      if(iter == imageChainList().begin())
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
         imageChainList().insert(iter, newObj);
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

#if 1
   ossimIdVisitor visitor( id, ossimVisitor::VISIT_CHILDREN );
   accept( visitor );
   ossimConnectableObject* obj = visitor.getObject();
   if ( obj )
   {
      return insertRight(newObj, obj);
   }
   return false;
#else
   ossimConnectableObject* obj = findObject(id, false);
   if(obj)
   {
      return insertRight(newObj, obj);
   }

   return false;
#endif
}

bool ossimImageChain::insertLeft(ossimConnectableObject* newObj,
                                 ossimConnectableObject* leftOfThisObj)
{
   if(!newObj&&!leftOfThisObj) return false;
   if(!imageChainList().size())
   {
      return add(newObj);
   }
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator iter = std::find(imageChainList().begin(), imageChainList().end(), leftOfThisObj);
   if(iter!=imageChainList().end())
   {
      if((iter+1)==imageChainList().end())
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
         imageChainList().insert(iter+1, newObj);
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
#if 1
   ossimIdVisitor visitor( id,
                           ossimVisitor::VISIT_CHILDREN|ossimVisitor::VISIT_INPUTS);
   accept( visitor );
   ossimConnectableObject* obj = visitor.getObject();
   if ( obj )
   {
      return insertLeft(newObj, obj);
   }
   return false;
#else
   ossimConnectableObject* obj = findObject(id, false);
   if(obj)
   {
      return insertLeft(newObj, obj);
   }
   return false;
#endif   
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
      oldObj->removeListener(this);
      oldObj->changeOwner(0);
      imageChainList()[idx] = newObj;
      newObj->connectInputList(inputList);
      newObj->connectOutputList(outputList);
      newObj->changeOwner(this);
      newObj->addListener((ossimConnectableObjectListener*)this);
      if(idx == 0)
      {
         newObj->addListener(this);
      }
   }
   
   return (idx >= 0);
}

ossimRefPtr<ossimImageData> ossimImageChain::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* inputSource = PTR_CAST(ossimImageSource,
                                             imageChainList()[0].get());
      
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      imageChainList()[0].get());
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      imageChainList()[0].get());
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      imageChainList()[0].get());
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                                  imageChainList()[0].get());
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
   if( (imageChainList().size() > 0) && isSourceEnabled() )
   {
      ossimRefPtr<const ossimImageSource> inter =
         dynamic_cast<const ossimImageSource*>( imageChainList()[0].get() );
      if( inter.valid() )
      {
         // cout << "cn: " << inter->getClassName() << endl;
         inter->getOutputBandList(bandList);
      }
   }
}
   
ossimScalarType ossimImageChain::getOutputScalarType() const
{
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      imageChainList()[0].get());
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         imageChainList()[0].get());
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
         ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                         imageChainList()[0].get());
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {

      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      imageChainList()[0].get());

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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface =PTR_CAST(ossimImageSource,
                                                     imageChainList()[0].get());

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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource, imageChainList()[0].get());
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      imageChainList()[0].get());
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      imageChainList()[0].get());
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
   if((imageChainList().size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      imageChainList()[0].get());

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
               
               if(imageChainList().size())
               {
                  if(traceDebug())
                  {
                     CLOG << "connecting " << source->getClassName() << " to "
                          << imageChainList()[0]->getClassName() << std::endl;
                  }
                  source->connectMyInputTo(0, imageChainList()[0].get());
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
   if(imageChainList().size())
   {
     ossimConnectableObject* obj = imageChainList()[(ossim_int32)imageChainList().size()-1].get();
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
   // cout << "this->getId(): " << this->getId() << endl;
   
   if(idMapping.size())
   {
      map<ossimId, vector<ossimId> >::const_iterator iter = idMapping.begin();

      
      while(iter != idMapping.end())
      {
         // cout << "(*iter).first): " << (*iter).first << endl;
#if 0
         ossimConnectableObject* currentSource = findObject((*iter).first);
#else
         ossimIdVisitor visitor( (*iter).first,
                                 (ossimVisitor::VISIT_CHILDREN ) );
                                  // ossimVisitor::VISIT_INPUTS ) );
         accept( visitor );
         ossimConnectableObject* currentSource = visitor.getObject();
#endif

         if(currentSource)
         {
            // cout << "currentSource->getClassName: " << currentSource->getClassName() << endl;
            long upperBound = (long)(*iter).second.size();
            for(long index = 0; index < upperBound; ++index)
            {
               //cout << "(*iter).second[index]: " << (*iter).second[index] << endl;
               
               if((*iter).second[index].getId() > -1)
               {
#if 0
                  ossimConnectableObject* inputSource =
                      PTR_CAST(ossimConnectableObject, findObject((*iter).second[index]));
#else
                  visitor.reset();
                  visitor.setId( (*iter).second[index] );
                  accept( visitor );
                  ossimConnectableObject* inputSource = visitor.getObject();
#endif
                  // cout << "inputSource is " << (inputSource?"good...":"null...") << endl;
                  if ( inputSource )
                  {
                     // cout << "inputSource->getClassName(): " << inputSource->getClassName() << endl;
 
                     // Check for connection to self.
                     if ( this != inputSource )
                     {
                        currentSource->connectMyInputTo(index, inputSource);
                     }
                     // else warning???
                  }
               }
               else // -1 id
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

   // abort();
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
   ossim_uint32 upper = (ossim_uint32)imageChainList().size();
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
         result = imageChainList()[idx]->saveState(kwl, newPrefix.c_str());
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
   
   long upper = (ossim_uint32)imageChainList().size();
   
   for(long index = upper - 1; index >= 0; --index)
   {
      if(traceDebug())
      {
         CLOG << "initilizing source: "
              << imageChainList()[index]->getClassName()
              << std::endl;
      }
      if(imageChainList()[index].valid())
      {
         ossimSource* interface =
            PTR_CAST(ossimSource, imageChainList()[index].get());

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
   ossim_int32 upper = static_cast<ossim_int32>(imageChainList().size());
   ossim_int32 index = 0;
   for(index = upper - 1; index >= 0; --index)
   {
      // make sure we initialize in reverse order.
      // some source may depend on the initialization of
      // its inputs
     ossimSource* source = PTR_CAST(ossimSource, imageChainList()[index].get());
     if(source)
     {
        source->enableSource();
     }
   }
   
   theEnableFlag = true;
}

void ossimImageChain::disableSource()
{
   long upper = (ossim_uint32)imageChainList().size();
   
   for(long index = upper - 1; index >= 0; --index)
   {
      // make sure we initialize in reverse order.
      // some source may depend on the initialization of
      // its inputs
     ossimSource* source = PTR_CAST(ossimSource, imageChainList()[index].get());
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
      connectableObject->changeOwner(0);
      connectableObject->disconnect();   
   }
}

bool ossimImageChain::deleteFirst()
{
   if (imageChainList().size() == 0) return false;

   ossimContainerEvent event(this, OSSIM_EVENT_REMOVE_OBJECT_ID);
   prepareForRemoval(imageChainList()[0].get());
   // Clear any listeners, memory.
   event.setObjectList(imageChainList()[0].get());
   imageChainList()[0] = 0;
   // Remove from the vector.
   std::vector<ossimRefPtr<ossimConnectableObject> >::iterator i = imageChainList().begin();
   imageChainList().erase(i);
   fireEvent(event);
   return true;
}

bool ossimImageChain::deleteLast()
{
   if (imageChainList().size() == 0) return false;

   ossimContainerEvent event(this, OSSIM_EVENT_REMOVE_OBJECT_ID);
  // Clear any listeners, memory.
   ossim_uint32 idx = (ossim_uint32)imageChainList().size() - 1;
   prepareForRemoval(imageChainList()[idx].get());
   event.setObjectList(imageChainList()[idx].get());
   imageChainList()[idx] = 0;
   // Remove from the vector.
   imageChainList().pop_back();
   fireEvent(event);
   return true; 
}

void ossimImageChain::deleteList()
{
   ossim_uint32 upper = (ossim_uint32) imageChainList().size();
   ossim_uint32 idx = 0;
   ossimContainerEvent event(this, OSSIM_EVENT_REMOVE_OBJECT_ID);
   for(; idx < upper; ++idx)
   {
      if(imageChainList()[idx].valid())
      {
         event.getObjectList().push_back(imageChainList()[idx].get());
         prepareForRemoval(imageChainList()[idx].get());
         imageChainList()[idx] = 0;
      }
   }
  
   imageChainList().clear();
   fireEvent(event);
}


void ossimImageChain::disconnectInputEvent(ossimConnectionEvent& event)
{
   if(imageChainList().size())
   {
      if(event.getObject()==this)
      {
         if(imageChainList()[imageChainList().size()-1].valid())
         {
            for(ossim_uint32 i = 0; i < event.getNumberOfOldObjects(); ++i)
            {
               imageChainList()[imageChainList().size()-1]->disconnectMyInput(event.getOldObject(i));
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
   if(imageChainList().size())
   {
      if(event.getObject()==this)
      {
         if(imageChainList()[imageChainList().size()-1].valid())
         {
            for(ossim_uint32 i = 0; i < event.getNumberOfNewObjects(); ++i)
            {
               ossimConnectableObject* obj = event.getNewObject(i);
               imageChainList()[imageChainList().size()-1]->connectMyInputTo(findInputIndex(obj),
                                                                               obj,
                                                                               false);
            }
         }
      }
      else if(event.getObject() == imageChainList()[0].get())
      {
         if(!theLoadStateFlag)
         {
//            theInputObjectList = imageChainList()[0]->getInputList();
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
//    if(imageChainList().size())
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

   if(imageChainList().size()&&(event.getObject()!=this))
   {
      removeChild(PTR_CAST(ossimConnectableObject,
                           event.getObject()));
   }
}
void ossimImageChain::propagateEventToOutputs(ossimEvent& event)
{
   //if(thePropagateEventFlag) return;

   //thePropagateEventFlag = true;
   if(imageChainList().size())
   {
      if(imageChainList()[imageChainList().size()-1].valid())
      {
         imageChainList()[imageChainList().size()-1]->fireEvent(event);
         imageChainList()[imageChainList().size()-1]->propagateEventToOutputs(event);
      }
   }
   //ossimConnectableObject::propagateEventToOutputs(event);
  // thePropagateEventFlag = false;
}
void ossimImageChain::propagateEventToInputs(ossimEvent& event)
{
//   if(thePropagateEventFlag) return;

//   thePropagateEventFlag = true;
   if(imageChainList().size())
   {
      if(imageChainList()[0].valid())
      {
         imageChainList()[0]->fireEvent(event);
         imageChainList()[0]->propagateEventToInputs(event);
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
   if(imageChainList().size() && (index < imageChainList().size()))
   {
      return imageChainList()[index].get();
   }
   
   return 0; 
}

ossim_int32 ossimImageChain::indexOf(ossimConnectableObject* obj)const
{
   ossim_uint32 idx = 0;
   
   for(idx = 0; idx < imageChainList().size();++idx)
   {
      if(imageChainList()[idx] == obj)
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
      //---
      // Lets make sure inputs and outputs are turned off for we are traversing all children
      // and we should not have to have that enabled.
      //---
      visitor.turnOffVisitorType(ossimVisitor::VISIT_INPUTS|ossimVisitor::VISIT_OUTPUTS);
      if(visitor.getVisitorType() & ossimVisitor::VISIT_CHILDREN)
      {
         ConnectableObjectList::reverse_iterator current = imageChainList().rbegin();
         while((current != imageChainList().rend())&&!visitor.stopTraversal())
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

//**************************************************************************************************
// Inserts all of its children and inputs into the container provided. Since ossimImageChain is
// itself a form of container, this method will consolidate this chain with the argument
// container. Therefore this chain object will not be represented in the container (but its
// children will be, with correct input and output connections to external objects).
// Returns TRUE if successful.
//**************************************************************************************************
bool ossimImageChain::fillContainer(ossimConnectableContainer& container)
{
   // Grab the first source in the chain and let it fill the container with itself and inputs. This
   // will traverse down the chain and will even pick up external sources that feed this chain:
   ossimRefPtr<ossimConnectableObject> first_source = getFirstSource();
   if (!first_source.valid())
      return false;
   first_source->fillContainer(container);

   // Instead of adding ourselves, make sure my first source is properly connected to my output,
   // thus obviating the need for this image chain (my chain items become part of 'container':
   ConnectableObjectList& obj_list = getOutputList();
   ossimRefPtr<ossimConnectableObject> output_connection = 0;
   while (!obj_list.empty())
   {
      // Always pick off the beginning of the list since it is shrinking with each disconnect:
      output_connection = obj_list[0];
      disconnectMyOutput(output_connection.get(), true, false);
      first_source->connectMyOutputTo(output_connection.get());
   }
   return true;
}
