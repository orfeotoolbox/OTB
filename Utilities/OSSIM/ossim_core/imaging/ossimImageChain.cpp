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
// $Id: ossimImageChain.cpp,v 1.109 2005/11/22 19:46:46 dburken Exp $
#include <algorithm>
#include <iostream>
#include <iterator>

#include <imaging/ossimImageChain.h>
#include <base/common/ossimCommon.h>
#include <base/context/ossimNotifyContext.h>
#include <imaging/ossimImageData.h>
#include <base/factory/ossimObjectFactoryRegistry.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/common/ossimTrace.h>
#include <base/common/events/ossimEventIds.h>
#include <base/common/events/ossimObjectEvents.h>
#include <base/common/ossimIdManager.h>

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
   virtual void propertyEvent(ossimPropertyEvent& event);

   ossimImageChain* theChain;
};

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

ossimImageChain::ossimImageChain()
   :ossimImageSource(),
    ossimConnectableContainerInterface((ossimObject*)NULL),
    theBlankTile(NULL),
    theLoadStateFlag(false)
{
   ossimConnectableContainerInterface::theBaseObject = this;
   thePropagateEventFlag = false;
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
      theChildListener = NULL;
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
      ossimConnectableObject* lastSource = theImageChainList[ theImageChainList.size() -1];
      if(PTR_CAST(ossimImageSourceInterface, obj)&&lastSource)
      {
         obj->disconnect();
         vector<ossimConnectableObject*> tempIn = getInputList();
         lastSource->disconnectAllInputs();
         lastSource->connectMyInputTo(obj);
         obj->changeOwner(this);
         obj->connectInputList(tempIn);

         tempIn = obj->getInputList();
         theInputListIsFixedFlag = obj->getInputListIsFixedFlag();
         setNumberOfInputs(obj->getNumberOfInputs());
         theImageChainList.push_back(obj);
         
         obj->addListener((ossimConnectableObjectListener*)this);
         return true;
      }
   }
   else
   {
      return add(obj);
   }

   return false;;
}

ossimImageSourceInterface* ossimImageChain::getFirstSource()
{
   if(theImageChainList.size()>0)
   {
      return PTR_CAST(ossimImageSourceInterface,
                      theImageChainList[0]);
   }

   return NULL;
}

ossimObject* ossimImageChain::getFirstObject()
{
   if(theImageChainList.size()>0)
   {
      return PTR_CAST(ossimObject,
                      theImageChainList[0]);
   }

   return NULL;
}

ossimImageSourceInterface* ossimImageChain::getLastSource()
{
   if(theImageChainList.size()>0)
   {
      return PTR_CAST(ossimImageSourceInterface,
                      *(theImageChainList.end()-1));
   }

   return NULL;
}

ossimObject* ossimImageChain::getLastObject()
{
   if(theImageChainList.size()>0)
   {
      return PTR_CAST(ossimObject,
                      *(theImageChainList.end()-1));
   }

   return NULL;
}

ossimConnectableObject* ossimImageChain::findObject(const ossimId& id,
                                                    bool recurse)
{
   vector<ossimConnectableObject*>::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current))
      {
	 if(id == (*current)->getId())
         {
            return (*current);
         }
      }

      ++current;
   }

   current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface,
                                                         (*current));
      
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
                                                    bool recurse)
{
   vector<ossimConnectableObject*>::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if(*current)
      {
	 if(obj == (*current))
         {
            return (*current);
         }
      }

      ++current;
   }

   current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current));
      
      if(child)
      {
         ossimConnectableObject* object = child->findObject(obj, true);

         if(object) return object;
      }
      ++current;
   }
   
   return NULL;
}

// ossimImageSourceInterface* ossimImageChain::findSource(const ossimId& id)
// {
//    ossimImageSourceInterface* result = NULL;
//    for(unsigned long index = 0; index < theImageChainList.size();++index)
//    {
//      ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject, theImageChainList[index]);
//      if(obj)
//        {
// 	 if(id == obj->getId())
// 	   {
//               return PTR_CAST(ossimImageSourceInterface,
//                               theImageChainList[index];
// 	   }
//        }
//    }
//    // now lets search children of type ossimImageChain
//    for(unsigned long index = 0; index < theImageChainList.size();++index)
//    {
//       ossimImageChain* child=PTR_CAST(ossimImageChain, theImageChainList[index]->getObject());
//       if(child)
//       {
//          result = child->findSource(id);
//          if(result)
//          {
//             return result;
//          }
//       }
//    }
   
//    return result;
// }

// ossimImageSourceInterface* ossimImageChain::findFirstSourceOfType(const RTTItypeid& typeInfo)
// {
//    ossimImageSourceInterface* result = NULL;
//    long index = 0;
   
//    for(index = 0; index < (long)theImageChainList.size();++index)
//    {
//       if(typeInfo.can_cast(TYPE_INFO(theImageChainList[index])))
//       {
//          return PTR_CAST(ossimImageSourceInterface,
//                          theImageChainList[index]);
//       }
//    }

//    // if still not found then search children.  It will
//    // see if one of its children is another container of
//    // type ossimImageChain.  If so, then it will call
//    // its method.
//    if(!result)
//    {
//       for(index = 0; index < (long)theImageChainList.size();++index)
//       {
//          ossimImageChain* child=PTR_CAST(ossimImageChain,
//                                          theImageChainList[index]);
         
//          if(child)
//          {
//             result = child->findFirstSourceOfType(typeInfo);
//             if(result)
//             {
//                return result;
//             }
//          }
//       }
//    }
   
//    return result;
// }

ossimConnectableObject* ossimImageChain::findFirstObjectOfType(const RTTItypeid& typeInfo,
                                                               bool recurse)
{
   vector<ossimConnectableObject*>::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current)&&
         (*current)->canCastTo(typeInfo))
      {
         return (*current);
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current));
         
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
   vector<ossimConnectableObject*>::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current)&&
         (*current)->canCastTo(className) )
      {
         return (*current);
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, *current);
         
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
   
   return (ossimConnectableObject*)NULL;   
}

std::vector<ossimConnectableObject*> ossimImageChain::findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                                           bool recurse)
{
   std::vector<ossimConnectableObject*> result;
   vector<ossimConnectableObject*>::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current)&&
         (*current)->canCastTo(typeInfo))
      {
         vector<ossimConnectableObject*>::iterator iter = std::find(result.begin(), result.end(), *current);
         if(iter == result.end())
         {
            result.push_back(*current);
         }
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, (*current));
         
         if(child)
         {
            vector<ossimConnectableObject*> temp;
            temp = child->findAllObjectsOfType(typeInfo, recurse);
            for(long index=0; index < (long)temp.size();++index)
            {
               std::vector<ossimConnectableObject*>::iterator iter = std::find(result.begin(), result.end(), temp[index]);
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

std::vector<ossimConnectableObject*> ossimImageChain::findAllObjectsOfType(const ossimString& className,
                                                                           bool recurse)
{
   std::vector<ossimConnectableObject*> result;
   vector<ossimConnectableObject*>::iterator current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      if((*current)&&
         (*current)->canCastTo(className))
      {
         std::vector<ossimConnectableObject*>::iterator iter = std::find(result.begin(), result.end(), (*current));
         if(iter == result.end())
         {
            result.push_back(*current);
         }
      }
      ++current;
   }

   if(recurse)
   {
      current =  theImageChainList.begin();
      while(current != theImageChainList.end())
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, *current);
         
         if(child)
         {
            vector<ossimConnectableObject*> temp;
            temp = child->findAllObjectsOfType(className, recurse);
            for(long index=0; index < (long)temp.size();++index)
            {
               std::vector<ossimConnectableObject*>::iterator iter = std::find(result.begin(), result.end(), temp[index]);
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

// vector<ossimImageSourceInterface*> ossimImageChain::findAllSourcesOfType(const RTTItypeid& typeInfo)
// {
//    vector<ossimImageSourceInterface*> result;
//    long index = 0;

//    for(index = 0; index < (long)theImageChainList.size();++index)
//    {
//       if(typeInfo.can_cast(TYPE_INFO(theImageChainList[index]->getObject())))
//       {
//          result.push_back( theImageChainList[index]);
//       }
//    }
//    // go through other image chains.
//    for(index = 0; index < (long)theImageChainList.size();++index)
//    {
//       ossimImageChain* child=PTR_CAST(ossimImageChain, theImageChainList[index]->getObject());
      
//       if(child)
//       {
//          vector<ossimImageSourceInterface*> temp;
         
//          temp =  child->findAllSourcesOfType(typeInfo);
//          for(long index2=0; index2 < (long)temp.size();index2++)
//          {
//             result.push_back(temp[index2]);
//          }
//       }
//    }   
   
//    return result;
// }

void ossimImageChain::makeUniqueIds()
{
   setId(ossimIdManager::instance()->generateId());
   for(long index = 0; index < (long)theImageChainList.size(); ++index)
   {
      ossimConnectableContainerInterface* container = PTR_CAST(ossimConnectableContainerInterface,
                                                               theImageChainList[index]);
      if(container)
      {
         container->makeUniqueIds();
      }
      else
      {
         if(theImageChainList[index])
         {
            theImageChainList[index]->setId(ossimIdManager::instance()->generateId());
         }
      }
   }
}

ossim_uint32 ossimImageChain::getNumberOfObjects(bool recurse)const
{
   ossim_uint32 result = theImageChainList.size();
   
   if(recurse)
   {
      for(ossim_uint32 i = 0; i < theImageChainList.size(); ++i)
      {
         ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, theImageChainList[i]);
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
   vector<ossimConnectableObject*>::iterator current =  theImageChainList.begin();
   ossim_uint32 i = 0;
   while(current != theImageChainList.end())
   {
      if(*current)
      {
	 if(object == (*current))
         {
            // Send an event to any listeners.
            ossimContainerEvent event((ossimObject*)this,
                                      (ossimObject*)object,
                                      OSSIM_EVENT_REMOVE_OBJECT_ID);
            fireEvent(event);
            object->changeOwner(NULL);

            object->removeListener((ossimConnectableObjectListener*)this);
            object->removeListener((ossimConnectableObjectListener*)theChildListener);

            std::vector<ossimConnectableObject*> input  = object->getInputList();
            std::vector<ossimConnectableObject*> output = object->getOutputList();

            // remeber the old size before removing
            ossim_uint32 chainSize = theImageChainList.size();
            current = theImageChainList.erase(current);
            // fix pointers
            //
            object->disconnect();
            
            if(i == 0) // is it the first in the chain
            {
               theImageChainList[0]->removeListener(theChildListener);
               theImageChainList[0]->addListener(theChildListener);
            }
            else if(i == (chainSize-1)) // is it the last
            {
               if(chainSize > 1)
               {
                  ossimConnectableObject* tempObj = theImageChainList[theImageChainList.size()-1];
                  if(tempObj)
                  {
                     tempObj->connectInputList(input);
                  }
               }
            }
            else // must be an interior
            {
               ossim_uint32 inIndex  = 0;

               for(inIndex = 0; inIndex < input.size(); ++inIndex)
               {
                  if(input[inIndex])
                  {
                     input[inIndex]->disconnectAllOutputs();
                     input[inIndex]->connectOutputList(output);
                  }
               }
            }

            return true;
         }
      }
      ++current;
      ++i;
   }

   current =  theImageChainList.begin();
   
   while(current != theImageChainList.end())
   {
      ossimConnectableContainerInterface* child=PTR_CAST(ossimConnectableContainerInterface, *current);
      
      if(child)
      {
         bool result = child->removeChild(object);
         if(result)
         {
            return result;
         }
      }
      ++current;
   }
   
   return false;
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
      temp.push_back(theImageChainList[i]);
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
   if(PTR_CAST(ossimImageSourceInterface, source))
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
        theImageChainList[0]->connectMyInputTo(theImageChainList[1]);
        result = true;
     }
     else
     {
        theInputObjectList = source->getInputList();
        theInputListIsFixedFlag = source->getInputListIsFixedFlag();
        
        theOutputObjectList      = source->getOutputList();
        theOutputListIsFixedFlag = source->getOutputListIsFixedFlag();
   
        theImageChainList.push_back(source);
        source->addListener((ossimConnectableObjectListener*)this);
        theImageChainList[0]->addListener(theChildListener);
        result = true;
     }
   }

   if (result && source)
   {
      ossimContainerEvent event(this, source, OSSIM_EVENT_ADD_OBJECT_ID);
      fireEvent(event);
   }
   
   return result;
}

bool ossimImageChain::insertRight(ossimConnectableObject* newObj,
                                  ossimConnectableObject* rightOfThisObj)
{
   if(!theImageChainList.size())
   {
      return add(newObj);
   }
   
   if(findObject(rightOfThisObj, false))
   {
      vector<ossimConnectableObject*>::iterator iter = theImageChainList.begin();
      while(iter != theImageChainList.end())
      {
         if( (*iter) == rightOfThisObj)
         {
            break;
         }
         ++iter;
      }
      if(rightOfThisObj == theImageChainList[0])
      {
         return add(newObj);
      }
      else if(PTR_CAST(ossimImageSourceInterface, newObj))
      {
         vector<ossimConnectableObject*> outputList = rightOfThisObj->getOutputList();
         rightOfThisObj->disconnectAllOutputs();

         // Core dump fix.  Connect input prior to outputs. (drb)
         newObj->connectMyInputTo(rightOfThisObj); 
         newObj->connectOutputList(outputList);
         newObj->changeOwner(this);
         newObj->addListener((ossimConnectableObjectListener*)this);
         theImageChainList.insert(iter, newObj);
         if (newObj)
         {
            // Send event to any listeners.
            ossimContainerEvent event(this, newObj, OSSIM_EVENT_ADD_OBJECT_ID);
            fireEvent(event);
         }
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
   if(!theImageChainList.size())
   {
      return add(newObj);
   }
   
   if(findObject(leftOfThisObj, false))
   {
      vector<ossimConnectableObject*>::iterator iter = theImageChainList.begin();

      while(iter != theImageChainList.end())
      {
         if( (*iter) == leftOfThisObj)
         {
            break;
         }
         ++iter;
      }
      if(leftOfThisObj==theImageChainList[theImageChainList.size()-1])
      {
         return addLast(newObj);
      }
      else if(PTR_CAST(ossimImageSourceInterface, newObj))
      {
         vector<ossimConnectableObject*> inputList = leftOfThisObj->getInputList();
         leftOfThisObj->disconnectAllInputs();
         newObj->connectInputList(inputList);
         leftOfThisObj->connectMyInputTo(newObj);
         newObj->changeOwner(this);
         newObj->addListener((ossimConnectableObjectListener*)this);
         theImageChainList.insert(iter+1, newObj);
         if (newObj)
         {
            // Send an event to any listeners.
            ossimContainerEvent event(this, newObj, OSSIM_EVENT_ADD_OBJECT_ID);
            fireEvent(event);
         }
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

ossimRefPtr<ossimImageData> ossimImageChain::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                         theImageChainList[0]);

         if(interface)
         {
            // make sure we initialize in reverse order.
            // some source may depend on the initialization of
            // its inputs
            return interface->getTile(tileRect, resLevel);
         }
      
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                         getInput(0));
         if(interface)
         {
            return interface->getTile(tileRect,
                                      resLevel);
         }
      }
   }

   if(theBlankTile.get())
   {
      theBlankTile->setOrigin(tileRect.ul());
      theBlankTile->setWidthHeight(tileRect.width(),
                                   tileRect.height());
   }
   
   return theBlankTile;
}

ossim_uint32 ossimImageChain::getNumberOfInputBands() const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      theImageChainList[0]);
      if(interface)
      {
         return interface->getNumberOfOutputBands();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
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
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      theImageChainList[0]);
      if(interface)
      {
         return interface->getNullPixelValue(band);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                         getInput(0));
         if(interface)
         {
            return interface->getNullPixelValue(band);
         }
      }
   }
   
   return ossimGetDefaultNull(getOutputScalarType());
}

double ossimImageChain::getMinPixelValue(ossim_uint32 band)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      theImageChainList[0]);
      if(interface)
      {
         return interface->getMinPixelValue(band);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                         getInput(0));
         if(interface)
         {
            return interface->getMinPixelValue(band);
         }
      }
   }

   return ossimGetDefaultMin(getOutputScalarType());
}

double ossimImageChain::getMaxPixelValue(ossim_uint32 band)const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSourceInterface* inter = PTR_CAST(ossimImageSourceInterface,
                                                  theImageChainList[0]);
      if(inter)
      {
         return inter->getMaxPixelValue(band);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                         getInput(0));
         if(interface)
         {
            return interface->getMaxPixelValue(band);
         }
      }
   }

   return ossimGetDefaultMax(getOutputScalarType());
}

void ossimImageChain::getOutputBandList(std::vector<ossim_uint32>& bandList) const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSourceInterface* inter = PTR_CAST(ossimImageSourceInterface,
                                                  theImageChainList[0]);
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
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      theImageChainList[0]);
      if(interface)
      {
         return interface->getOutputScalarType();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
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
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                         theImageChainList[0]);
         if(interface)
         {
            return interface->getTileWidth();;
         }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
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
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                         theImageChainList[0]);
         if(interface)
         {
            return interface->getTileHeight();
         }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
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

      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      theImageChainList[0]);

      if(interface)
      {
         return interface->getBoundingRect(resLevel);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
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
      ossimImageSourceInterface* interface =PTR_CAST(ossimImageSourceInterface,
                                                     theImageChainList[0]);

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
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
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

bool ossimImageChain::getImageGeometry(ossimKeywordlist& kwl,
                                       const char* prefix)
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      theImageChainList[0]);
      if(interface)
      {
         return interface->getImageGeometry(kwl,
                                            prefix);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                         getInput(0));
         if(interface)
         {
            return interface->getImageGeometry(kwl,
                                               prefix);
         }
      }
      
   }
   return false;
}

void ossimImageChain::getDecimationFactor(ossim_uint32 resLevel,
                                          ossimDpt& result) const
{
   if((theImageChainList.size() > 0)&&(isSourceEnabled()))
   {
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      theImageChainList[0]);
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
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
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
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      theImageChainList[0]);
      if(interface)
      {
         interface->getDecimationFactors(decimations);
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
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
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      theImageChainList[0]);

      if(interface)
      {
         return interface->getNumberOfDecimationLevels();
      }
   }
   else
   {
      if(getInput(0))
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
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
   long numberOfSources = keys.size();//kwl.getNumberOfSubstringKeys(regExpression);

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
         CLOG << "trying to create source with prefix: " << newPrefix
              << std::endl;
      }
      ossimObject* object = ossimObjectFactoryRegistry::instance()->createObject(kwl,
                                                                                 newPrefix.c_str());
      ossimConnectableObject* source = PTR_CAST(ossimConnectableObject, object);
      
      if(source)
      {
         // we did find a source so include it in the count
         if(traceDebug())
         {
            CLOG << "Created source with prefix: " << newPrefix << std::endl;
         }
         if(PTR_CAST(ossimImageSourceInterface, source))
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
                  source->connectMyInputTo(0, theImageChainList[0]);
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
         else
         {
            delete source;
            source = NULL;
         }
      }
      else
      {
         delete object;
         object = NULL;
         source = NULL;
      }
      
      ++index;
   }
   if(theImageChainList.size())
   {
     ossimConnectableObject* obj = theImageChainList[(long)theImageChainList.size()-1];
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
   
   ossim_int32 offset = (copyPrefix+"input_connection").size();
   ossim_uint32 numberOfKeys = keys.size();
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
            long upperBound = (*iter).second.size();
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
   long upper = theImageChainList.size();
   long counter = 1;

   // start with the tail and go to the head fo the list.
   for(long index = upper - 1; ((index >= 0)&&result); --index, ++counter)
   {
      ossimString newPrefix = prefix;

      newPrefix += (ossimString("object") +
                    ossimString::toString(counter) +
                    ossimString("."));
      result = theImageChainList[index]->saveState(kwl, newPrefix.c_str());
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
   
   long upper = theImageChainList.size();
   
   for(long index = upper - 1; index >= 0; --index)
   {
      if(traceDebug())
      {
         CLOG << "initilizing source: "
              << theImageChainList[index]->getClassName()
              << std::endl;
      }
      if(theImageChainList[index])
      {
         ossimImageSourceInterface* interface =
            PTR_CAST(ossimImageSourceInterface, theImageChainList[index]);

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
   long upper = theImageChainList.size();
   
   for(long index = upper - 1; index >= 0; --index)
   {
      // make sure we initialize in reverse order.
      // some source may depend on the initialization of
      // its inputs
     ossimSource* source = PTR_CAST(ossimSource, theImageChainList[index]);
     if(source)
     {
        source->enableSource();
     }
   }
   
   theEnableFlag = true;
}

void ossimImageChain::disableSource()
{
   long upper = theImageChainList.size();
   
   for(long index = upper - 1; index >= 0; --index)
   {
      // make sure we initialize in reverse order.
      // some source may depend on the initialization of
      // its inputs
     ossimSource* source = PTR_CAST(ossimSource, theImageChainList[index]);
     if(source)
     {
        source->disableSource();
     }
   }
   
   theEnableFlag = false;
}


vector<ossimConnectableObject*>& ossimImageChain::getChildren()
{
   return theImageChainList;
}

bool ossimImageChain::deleteFirst()
{
   if (theImageChainList.size() == 0) return false;

   // Clear any listeners, memory.
   ossim_uint32 index = 0;
   theImageChainList[index]->
      removeListener((ossimConnectableObjectListener*)this);
   theImageChainList[index]->removeListener(theChildListener);
   theImageChainList[index]->changeOwner(NULL);
   delete theImageChainList[index];
   
   // Remove from the vector.
   vector<ossimConnectableObject*>::iterator i = theImageChainList.begin();
   theImageChainList.erase(i);
   return true;
}

bool ossimImageChain::deleteLast()
{
   if (theImageChainList.size() == 0) return false;

   // Clear any listeners, memory.
   ossim_uint32 index = theImageChainList.size() - 1;
   theImageChainList[index]->
      removeListener((ossimConnectableObjectListener*)this);
   theImageChainList[index]->removeListener(theChildListener);
   theImageChainList[index]->changeOwner(NULL);
   delete theImageChainList[index];
   
   // Remove from the vector.
   theImageChainList.pop_back();
   return true; 
}

void ossimImageChain::deleteList()
{
   long upper = theImageChainList.size();

   
   for(long index = 0; index < upper; ++index)
   {
      theImageChainList[index]->removeListener((ossimConnectableObjectListener*)this);
      theImageChainList[index]->removeListener(theChildListener);
      theImageChainList[index]->changeOwner(NULL);
      delete theImageChainList[index];
   }

   theImageChainList.clear();
}

void ossimImageChain::disconnectInputEvent(ossimConnectionEvent& event)
{
   if(theImageChainList.size())
   {
      if(event.getObject()==this)
      {
         if(theImageChainList[theImageChainList.size()-1])
         {
            for(ossim_uint32 i = 0; i < event.getNumberOfOldObjects(); ++i)
            {
               theImageChainList[theImageChainList.size()-1]->disconnectMyInput(event.getOldObject(i));
            }
         }
      }
   }
}

void ossimImageChain::disconnectOutputEvent(ossimConnectionEvent& event)
{
}

void ossimImageChain::connectInputEvent(ossimConnectionEvent& event)
{
   if(theImageChainList.size())
   {
      if(event.getObject()==this)
      {
         if(theImageChainList[theImageChainList.size()-1])
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
      else if(event.getObject() == theImageChainList[0])
      {
         if(!theLoadStateFlag)
         {
//            theInputObjectList = theImageChainList[0]->getInputList();
         }
      }
      initialize();
   }
}

void ossimImageChain::connectOutputEvent(ossimConnectionEvent& event)
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
//          ossimImageSourceInterface* interface = findSource(obj->getId());
         
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
   if(thePropagateEventFlag) return;

   thePropagateEventFlag = true;
   if(theImageChainList.size())
   {
      if(theImageChainList[theImageChainList.size()-1])
      {
         theImageChainList[theImageChainList.size()-1]->fireEvent(event);
         theImageChainList[theImageChainList.size()-1]->propagateEventToOutputs(event);
      }
   }
   ossimConnectableObject::propagateEventToOutputs(event);
   thePropagateEventFlag = false;
}

void ossimImageChain::propagateEventToInputs(ossimEvent& event)
{
   if(thePropagateEventFlag) return;

   thePropagateEventFlag = true;
   if(theImageChainList.size())
   {
      if(theImageChainList[0])
      {
         theImageChainList[0]->fireEvent(event);
         theImageChainList[0]->propagateEventToOutputs(event);
      }
   }
   thePropagateEventFlag = false;
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
      return theImageChainList[index];
   }
   
   return (ossimConnectableObject*)NULL; 
}
