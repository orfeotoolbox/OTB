//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimConnectableObject.cpp 21850 2012-10-21 20:09:55Z dburken $

#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimIdManager.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimConnectableContainerInterface.h>
#include <ossim/base/ossimConnectableContainer.h>
#include <ossim/base/ossimTextProperty.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimVisitor.h>
#include <algorithm>

RTTI_DEF3(ossimConnectableObject,
          "ossimConnectableObject",
          ossimObject,
          ossimListenerManager,
          ossimPropertyInterface);

const char* CONNECTABLE_INPUT_LIST_FIXED_KW  = "input_list_fixed";
const char* CONNECTABLE_OUTPUT_LIST_FIXED_KW = "output_list_fixed";

ossimConnectableObject::ossimConnectableObject(ossimObject* owner)
:ossimObject(),
ossimListenerManager(),
theInputListIsFixedFlag(false),
theOutputListIsFixedFlag(false)
{
   theId    = ossimIdManager::instance()->generateId();
   theOwner = owner;
}

ossimConnectableObject::ossimConnectableObject(ossimObject* owner,
                                               ossim_int32 inputListSize,
                                               ossim_int32 outputListSize,
                                               bool inputListIsFixedFlag,
                                               bool outputListIsFixedFlag)
:ossimObject(),
ossimListenerManager(),
theInputListIsFixedFlag(inputListIsFixedFlag),
theOutputListIsFixedFlag(outputListIsFixedFlag)
{
   theId    = ossimIdManager::instance()->generateId();
   theOwner = owner;
   
   setNumberOfInputs(inputListSize);
   setNumberOfOutputs(outputListSize);
}

ossimConnectableObject::~ossimConnectableObject()
{
   
   // tell the immediate listeners that we are destructing.
   ossimObjectDestructingEvent event(this);
   
   //    if(theOwner)
   //    {
   //       ossimListenerManager* manager = PTR_CAST(ossimListenerManager, theOwner);
   
   //       if(manager)
   //       {
   //          // notify the owner that you are destructing
   //          //
   //          manager->fireEvent(event);
   //       }
   //    }
   
   // notify all other listeners that you are destructing
   //
   fireEvent(event);
   
 }

void ossimConnectableObject::changeOwner(ossimObject* owner)
{
   theOwner = owner;
}

void ossimConnectableObject::setDescription(const ossimString& description)
{
   theDescription = description;
}

ossimString ossimConnectableObject::getDescription()const
{
   return theDescription;
}

bool ossimConnectableObject::isConnected(ossimConnectableObjectDirectionType direction)const
{
   if(direction & CONNECTABLE_DIRECTION_INPUT)
   {
      if(getNumberOfInputs())
      {
         ConnectableObjectList::const_iterator current = theInputObjectList.begin();
         
         while(current != theInputObjectList.end())
         {
            if(! (*current))
            {
               return false;
            }
            
            ++current;
         }
      }
      else if(!theInputListIsFixedFlag)
      {
         return false;
      }
   }
   
   if(direction & CONNECTABLE_DIRECTION_OUTPUT)
   {
      if(theOutputObjectList.empty())
      {
         return theOutputListIsFixedFlag;
         
      }
      ConnectableObjectList::const_iterator current = theOutputObjectList.begin();
      current = theOutputObjectList.begin();
      while(current != theOutputObjectList.end())
      {
         if(! (*current))
         {
            return false;
         }
         
         ++current;
      }
   }
   
   return true;
}

ossimConnectableObject* ossimConnectableObject::findConnectableObject(const ossimId& id)
{
   ConnectableObjectList::iterator current;
   
   current = theInputObjectList.begin();
   while(current != theInputObjectList.end())
   {
      ossimConnectableObject* temp = (*current).get();
      
      if(temp->getId() == id)
      {
         return temp;
      }
      
      ++current;
   }
   
   // go through the outputs
   current = theOutputObjectList.begin();
   while(current != theOutputObjectList.end())
   {
      ossimConnectableObject* temp = (*current).get();
      
      if(temp->getId() == id)
      {
         return temp;
      }
      
      ++current;
   }
   
   return 0;
}

ossimConnectableObject* ossimConnectableObject::findObjectOfType(
   RTTItypeid typeId,
   ossimConnectableObjectDirectionType directionType,
   bool recurse)
{
   ossimConnectableObject* result = 0;

   if(directionType != CONNECTABLE_DIRECTION_NONE)
   {
      ConnectableObjectList* connectableList = 0;

      if ( CONNECTABLE_DIRECTION_INPUT )
      {
         connectableList = &theInputObjectList;
      }
      else // (directionType == CONNECTABLE_DIRECTION_OUTPUT)
      {
         connectableList = &theOutputObjectList;
      }

      // see if it is in the immediate list
      for(ossim_uint32 index = 0; index < connectableList->size(); ++index)
      {
         if( (*connectableList)[index].valid() )
         {
            if( (*connectableList)[index]->canCastTo( typeId ) )
            {
               result = (*connectableList)[index].get();
               break;
            }
         }
      }

      if ( !result )
      {
         ossimVisitor::VisitorType vType = ossimVisitor::VISIT_NONE;
         
         if ( CONNECTABLE_DIRECTION_INPUT )
         {
            if ( recurse )
            {
               // Cast needed for compiler...
               vType = (ossimVisitor::VisitorType)
                  (ossimVisitor::VISIT_INPUTS|ossimVisitor::VISIT_CHILDREN);
            }
            else
            {
               vType = ossimVisitor::VISIT_INPUTS;
            }
         }
         else // (directionType == CONNECTABLE_DIRECTION_OUTPUT)
         {
            if ( recurse )
            {
               // Cast needed for compiler...
               vType = (ossimVisitor::VisitorType)
                  (ossimVisitor::VISIT_OUTPUTS|ossimVisitor::VISIT_CHILDREN);
            }
            else
            {
               vType = ossimVisitor::VISIT_OUTPUTS;
            }
         }
         
         ossimTypeIdVisitor visitor( typeId,
                                     true, // firstofTypeFlag
                                     vType );


         this->accept( visitor );
         result = dynamic_cast<ossimConnectableObject*>( visitor.getObject(0) );
      }
      
   } // Matches: if(directionType != CONNECTABLE_DIRECTION_NONE)
   
   return result;
   
} // End: findObjectOfType( RTTItypeid ...

ossimConnectableObject* ossimConnectableObject::findObjectOfType(
   const ossimString& className,
   ossimConnectableObjectDirectionType directionType,
   bool recurse )
{
   ossimConnectableObject* result = 0;
   
   if(directionType != CONNECTABLE_DIRECTION_NONE)
   {
      ConnectableObjectList* connectableList = 0;
      
      if ( CONNECTABLE_DIRECTION_INPUT )
      {
         connectableList = &theInputObjectList;
      }
      else // (directionType == CONNECTABLE_DIRECTION_OUTPUT)
      {
         connectableList = &theOutputObjectList;
      }
      
      // see if it is in the immediate list
      for(ossim_uint32 index = 0; index < connectableList->size(); ++index)
      {
         if( (*connectableList)[index].valid() )
         {
            if( (*connectableList)[index]->canCastTo( className ) )
            {
               result = (*connectableList)[index].get();
               break;
            }
         }
      }
      
      if ( !result )
      {
         ossimVisitor::VisitorType vType = ossimVisitor::VISIT_NONE;
         
         if ( CONNECTABLE_DIRECTION_INPUT )
         {
            if ( recurse )
            {
               // Cast needed for compiler...
               vType = (ossimVisitor::VisitorType)
                  (ossimVisitor::VISIT_INPUTS|ossimVisitor::VISIT_CHILDREN);
            }
            else
            {
               vType = ossimVisitor::VISIT_INPUTS;
            }
         }
         else // (directionType == CONNECTABLE_DIRECTION_OUTPUT)
         {
            if ( recurse )
            {
               // Cast needed for compiler...
               vType = (ossimVisitor::VisitorType)
                  (ossimVisitor::VISIT_OUTPUTS|ossimVisitor::VISIT_CHILDREN);
            }
            else
            {
               vType = ossimVisitor::VISIT_OUTPUTS;
            }
         }
         
         ossimTypeNameVisitor visitor( className,
                                       true, // firstofTypeFlag
                                       vType );
         this->accept( visitor );
         result = dynamic_cast<ossimConnectableObject*>( visitor.getObject(0) );
      }
      
   } // Matches: if(directionType != CONNECTABLE_DIRECTION_NONE)
   
   return result;
   
} // End: findObjectOfType( const ossimString& className ...

ossimConnectableObject* ossimConnectableObject::findInputObjectOfType(
   const ossimString& className)
{
   ossimConnectableObject* result = 0;
   
   // See if we are of class type.
   if ( canCastTo( className ) )
   {
      result = this;
   }

   if ( !result )
   {
      ConnectableObjectList* connectableList = &theInputObjectList;

      // see if it is in the immediate list
      for(ossim_uint32 index = 0; index < connectableList->size(); ++index)
      {
         if( (*connectableList)[index].valid() )
         {
            if( (*connectableList)[index]->canCastTo( className ) )
            {
               result = (*connectableList)[index].get();
               break;
            }
         }
      }
      
      if ( !result )
      {
         ossimTypeNameVisitor visitor( className,
                                       true, // firstofTypeFlag
                                       (ossimVisitor::VISIT_INPUTS|
                                        ossimVisitor::VISIT_CHILDREN) );
         this->accept( visitor );
         result = dynamic_cast<ossimConnectableObject*>( visitor.getObject(0) );
      }
   }
   
   return result;
   
} // End: findInputObjectOfType( const ossimString& className )

// Old findObject findInputObject code kept here until debugged to satisfaction.
#if 0 /* drb */

ossimConnectableObject* ossimConnectableObject::findObjectOfType(RTTItypeid typeId,
                                                                 ossimConnectableObjectDirectionType directionType,
                                                                 bool recurse)
{
   ConnectableObjectList* connectableList = &theInputObjectList;
   
   
   if(directionType == CONNECTABLE_DIRECTION_NONE)
   {
      return 0;
   }
   
   if(directionType == CONNECTABLE_DIRECTION_OUTPUT)
   {
      connectableList = &theOutputObjectList;
   }
   // see if it is in the immediate list
   for(ossim_uint32 index = 0; index < connectableList->size(); ++index)
   {
      if((*connectableList)[index].valid())
      {
         if((*connectableList)[index]->canCastTo(typeId))//typeId.can_cast(TYPE_INFO(theInputObjectList[index])))
         {
            return (*connectableList)[index].get();
         }
      }
   }
   
   if(recurse)
   {
      ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                           this);
      if(inter)
      {
         ossimConnectableObject* tempObj = inter->findFirstObjectOfType(typeId);
         if(tempObj)
         {
            return tempObj;
         }
      }
      
      for(ossim_uint32 index = 0; index < connectableList->size(); ++index)
      {
         inter = PTR_CAST(ossimConnectableContainerInterface,
                          (*connectableList)[index].get());
         if(inter)
         {
            ossimConnectableObject* tempObj = inter->findFirstObjectOfType(typeId);
            if(tempObj)
            {
               return tempObj;
            }
         }
         if((*connectableList)[index].valid())
         {
            ossimConnectableObject* result = (*connectableList)[index]->findObjectOfType(typeId,
                                                                                         directionType,
                                                                                         recurse);
            if(result)
            {
               return result;
            }
         }
      }
   }
   
   ossimConnectableObject* result = 0;
   for(ossim_uint32 index = 0; (index < connectableList->size())&&!result; ++index)
   {
      if((*connectableList)[index].valid())
      {
         result = ((*connectableList)[index])->findObjectOfType(typeId,
                                                                directionType,
                                                                recurse);
      }
   }
   
   return result;
}

ossimConnectableObject* ossimConnectableObject::findObjectOfType(
                                                                 const ossimString& className,
                                                                 ossimConnectableObjectDirectionType directionType,
                                                                 bool recurse)
{
   ConnectableObjectList *connectableList = &theInputObjectList;
   
   if(directionType == CONNECTABLE_DIRECTION_NONE)
   {
      return 0;
   }
   
   if(directionType == CONNECTABLE_DIRECTION_OUTPUT)
   {
      connectableList = &theOutputObjectList;
      
   }
   
   // see if it is in the immediate list
   for(ossim_uint32 index = 0; index < connectableList->size(); ++index)
   {
      if((*connectableList)[index].valid())
      {
         if((*connectableList)[index]->canCastTo(className))//typeId.can_cast(TYPE_INFO(theInputObjectList[index])))
         {
            return (*connectableList)[index].get();
         }
      }
   }
   
   if(recurse)
   {
      ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                           this);
      if(inter)
      {
         ossimConnectableObject* tempObj = inter->findFirstObjectOfType(className);
         if(tempObj)
         {
            return tempObj;
         }
      }
      for(ossim_uint32 index = 0; index < connectableList->size(); ++index)
      {
         inter = PTR_CAST(ossimConnectableContainerInterface,
                          (*connectableList)[index].get());
         if(inter)
         {
            ossimConnectableObject* tempObj = inter->findFirstObjectOfType(className);
            if(tempObj)
            {
               return tempObj;
            }
         }
         if((*connectableList)[index].valid())
         {
            ossimConnectableObject* result = (*connectableList)[index]->findObjectOfType(className,
                                                                                         directionType,
                                                                                         recurse);
            if(result)
            {
               return result;
            }
         }
      }
   }
   ossimConnectableObject* result = 0;
   for(ossim_uint32 index = 0; (index < connectableList->size())&&!result; ++index)
   {
      if((*connectableList)[index].valid())
      {
         result = ((*connectableList)[index])->findObjectOfType(className,
                                                                directionType,
                                                                recurse);
      }
   }
   
   return result;
}

ossimConnectableObject* ossimConnectableObject::findInputObjectOfType(
    
                                                                      const ossimString& className)
{
   // See if we are of class type.
   if (canCastTo(className))
   {
      return this;
   }
   ossimConnectableObject* result = 0;
   // If we are a container, look inside for type.
   ossimConnectableContainerInterface* container =
   PTR_CAST(ossimConnectableContainerInterface, this);
   if (container)
   {
      const ossim_uint32 NUMBER_OF_OBJECTS =
      container->getNumberOfObjects(false);
      if (NUMBER_OF_OBJECTS)
      {
         for (ossim_uint32 idx = NUMBER_OF_OBJECTS; idx > 0; --idx) 
         {
            result =
            container->getConnectableObject(idx-1);
            if (result)
            {
               if (result->canCastTo(className))
               {
                  return result;
               }
               else 
               {
                  result = result->findInputObjectOfType(className);
                  if(result)
                  {
                     return result;
                  }
               }
            }
         }
      }
   }
  
   ossim_uint32 inputs = getNumberOfInputs();
   ossim_uint32 inputIdx = 0;
   for(inputIdx = 0; inputIdx < inputs; ++inputIdx)
   {
      ossimConnectableObject* input = getInput(inputIdx);
      if(input)
      {
         result = input->findInputObjectOfType(className);
         if(result)
         {
            return result;
         }
      }
   }
   return result;
}
#endif /* drb */
// End: Old findObject findInputObject code kept here until debugged to satisfaction.

ossim_int32 ossimConnectableObject::findInputIndex(const ossimConnectableObject* object)
{
   ossim_int32 i = 0;
   for(i = 0; i < (ossim_int32)theInputObjectList.size(); ++i)
   {
      if(theInputObjectList[i].get() == object)
      {
         return i;
      }
   }
   
   return -1;
}

ossim_int32 ossimConnectableObject::findInputIndex(const ossimId& id)
{
   ossim_int32 i = 0;
   for(i = 0; i < (ossim_int32)theInputObjectList.size(); ++i)
   {
      if(theInputObjectList[i].valid() &&
         (theInputObjectList[i]->getId() == id))
      {
         return i;
      }
   }
   
   return -1;
}

ossim_int32 ossimConnectableObject::findOutputIndex(const ossimConnectableObject* object)
{
   ossim_int32 i = 0;
   for(i = 0; i < (ossim_int32)theOutputObjectList.size(); ++i)
   {
      if(theOutputObjectList[i] == object)
      {
         return i;
      }
   }
   
   return -1;
}

ossim_int32 ossimConnectableObject::findOutputIndex(const ossimId& id)
{
   ossim_int32 i = 0;
   for(i = 0; i < (ossim_int32)theOutputObjectList.size(); ++i)
   {
      if(theOutputObjectList[i].valid() &&
         (theOutputObjectList[i]->getId() == id))
      {
         return i;
      }
   }
   
   return -1;
}

ossim_int32 ossimConnectableObject::getMyInputIndexToConnectTo(ossimConnectableObject* object)const
{
   ConnectableObjectList::const_iterator current;
   
   ossim_int32 index = 0;
   current = theInputObjectList.begin();
   while(current != theInputObjectList.end())
   {
      if(!(*current))
      {
         if(canConnectMyInputTo(index, object))
         {
            return index;
         }
      }
      ++current;
      ++index;
   }
   if(!theInputListIsFixedFlag)
   {
      if(canConnectMyInputTo((ossim_int32)theInputObjectList.size(),
                             object))
      {
         return (ossim_int32)theInputObjectList.size();
      }
   }
   return -1;
}

ossim_int32 ossimConnectableObject::getMyOutputIndexToConnectTo(ossimConnectableObject* object)const
{
   ConnectableObjectList::const_iterator current;
   
   ossim_int32 index = 0;
   current = theOutputObjectList.begin();
   while(current != theOutputObjectList.end())
   {
      if(!(*current))
      {
         if(canConnectMyOutputTo(index, object))
         {
            return index;
         }
      }
      ++current;
      ++index;
   }
   if(!theOutputListIsFixedFlag)
   {
      if(canConnectMyOutputTo((ossim_int32)theOutputObjectList.size(),
                              object))
      {
         return (ossim_int32)theOutputObjectList.size();
      }
   }
   // default to return the size.  This will force an append.
   //
   return -1;
}

void ossimConnectableObject::disconnect(ossimConnectableObject* object)
{
   if( (object==this) || (object==0) )
   {
      disconnectAllInputs();
      disconnectAllOutputs();
   }
   else
   {
      ossim_int32 index = findInputIndex(object);
      if(index > -1)
      {
         disconnectMyInput(index);
      }
      index = findOutputIndex(object);
      if(index > -1)
      {
         disconnectMyOutput(index, object);
      }
   }
}

void ossimConnectableObject::disconnect(const ossimId& id)
{
   if(id == theId)
   {
      disconnect(this);
   }
   else
   {
      ossimIdVisitor visitor( id,
                              (ossimVisitor::VISIT_CHILDREN |
                               ossimVisitor::VISIT_INPUTS   |
                               ossimVisitor::VISIT_OUTPUTS) );
      accept( visitor );
      disconnect( visitor.getObject() );
   }
}

ossimRefPtr<ossimConnectableObject> ossimConnectableObject::disconnectMyInput(ossim_int32 inputIndex,
                                                                  bool disconnectOutputFlag,
                                                                  bool createEventFlag)
{
   if(theInputObjectList.size() == 0)
   {
      return 0;
   }
   
   ConnectableObjectList::iterator current;
   ossimRefPtr<ossimConnectableObject> result = 0;
   
   if( (inputIndex > -1)&&
      (inputIndex < (ossim_int32)theInputObjectList.size()))
   {
      current = (theInputObjectList.begin()+inputIndex);
      result = (*current).get();
      
      if(!theInputListIsFixedFlag)
      {
         current = theInputObjectList.erase(current);
      }
      else
      {
         *current = 0;
      }
      if(createEventFlag&&result.valid())
      {
         ossimConnectionEvent event(this,  // owner of message
                                    OSSIM_EVENT_CONNECTION_DISCONNECT_ID,
                                    0,  // new object
                                    result.get(),// old object
                                    ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
         
         // send event to all listeners.
         //
         fireEvent(event);
      }
      
      if(disconnectOutputFlag&&result.valid())
      {
         result->disconnectMyOutput(this, false, createEventFlag);
      }
   }
   return result;
}


void ossimConnectableObject::disconnectMyInput(ossimConnectableObject* input,
                                               bool disconnectOutputFlag,
                                               bool createEventFlag)
{
   disconnectMyInput(findInputIndex(input),
                     disconnectOutputFlag,
                     createEventFlag);
}

void ossimConnectableObject::disconnectMyInputs(ConnectableObjectList& inputList,
                                                bool disconnectOutputFlag,
                                                bool createEventFlag)
{
   if(theInputObjectList.size() == 0) return;
   ConnectableObjectList oldInputs;
   ConnectableObjectList newInputs;
   ossim_int32 i = 0;
   
   for(i = 0; i < (ossim_int32)inputList.size(); ++i)
   {
      if(findInputIndex(inputList[i].get()) >= 0)
      {
         disconnectMyInput(inputList[i].get(), disconnectOutputFlag, false);
         oldInputs.push_back(inputList[i]);
      }
   }
   if(createEventFlag && oldInputs.size())
   {
      ossimConnectionEvent event(this,
                                 OSSIM_EVENT_CONNECTION_DISCONNECT_ID,
                                 newInputs,
                                 oldInputs,
                                 ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
      fireEvent(event);
   }
}

ossimRefPtr<ossimConnectableObject> ossimConnectableObject::disconnectMyOutput(ossim_int32 outputIndex,
                                                                   bool disconnectInputFlag,
                                                                   bool createEvent)
{
   if(theOutputObjectList.size() == 0)
   {
      return 0;
   }
   
   ConnectableObjectList::iterator current;
   ossimRefPtr<ossimConnectableObject> result = 0;
   if( (outputIndex > -1)&&
      (outputIndex < (ossim_int32)theOutputObjectList.size()))
   {
      current = (theOutputObjectList.begin()+outputIndex);
      result = (*current).get();
      if(!theOutputListIsFixedFlag)
      {
         current = theOutputObjectList.erase(current);
      }
      else
      {
         *current = 0;
      }
      if(createEvent)
      {
         ossimConnectionEvent event(this,  // owner of message
                                    OSSIM_EVENT_CONNECTION_DISCONNECT_ID,
                                    0,  // new object
                                    result.get(),// old object
                                    ossimConnectionEvent::OSSIM_OUTPUT_DIRECTION);
         
         // send event to all listeners.
         //
         fireEvent(event);
      }
      
      if(disconnectInputFlag&&result.valid())
      {
         result->disconnectMyInput(this, false, createEvent);
      }
   }
   return result;
}

void ossimConnectableObject::disconnectMyOutput(ossimConnectableObject* output,
                                                bool disconnectInputFlag,
                                                bool createEventFlag)
{
   disconnectMyOutput(findOutputIndex(output), disconnectInputFlag, createEventFlag);
}

void ossimConnectableObject::disconnectMyOutputs(ConnectableObjectList& outputList,
                                                 bool disconnectInputFlag,
                                                 bool createEventFlag)
{
   if(theOutputObjectList.size() == 0) return;
   
   if(theOutputObjectList.size() == 1)
   {
      disconnectMyOutput((ossim_int32)0);
      return;
   }
   ConnectableObjectList oldOutputs;
   ConnectableObjectList newOutputs;
   ossim_int32 i = 0;
   for(i = 0; i < (ossim_int32)outputList.size(); ++i)
   {
      if(findOutputIndex(outputList[i].get()) >= 0)
      {
         disconnectMyOutput(outputList[i].get(), disconnectInputFlag, false);
         oldOutputs.push_back(outputList[i]);
      }
   }
   if(createEventFlag && oldOutputs.size())
   {
      ossimConnectionEvent event(this,
                                 OSSIM_EVENT_CONNECTION_DISCONNECT_ID,
                                 newOutputs,
                                 oldOutputs,
                                 ossimConnectionEvent::OSSIM_OUTPUT_DIRECTION);
      fireEvent(event);
   }
}

void ossimConnectableObject::disconnectAllInputs()
{
   if(theInputObjectList.size() == 0) return;
   if(theInputObjectList.size() == 1)
   {
      //     ossimConnectableObject* obj = disconnectMyInput((ossim_int32)0, false);
      disconnectMyInput((ossim_int32)0);
      //     if(obj)
      //     {
      //        obj->disconnectMyOutput(this, false);
      //     }
      return;
   }
   ConnectableObjectList::iterator current;
   ConnectableObjectList oldInputs = theInputObjectList;
   ConnectableObjectList newInputs;
   
   
   current = theInputObjectList.begin();
   while(current != theInputObjectList.end())
   {
      if(!theInputListIsFixedFlag)
      {
         current = theInputObjectList.erase(current);
      }
      else
      {
         *current = 0;
         ++current;
      }
   }
   ossimConnectionEvent event(this,
                              OSSIM_EVENT_CONNECTION_DISCONNECT_ID,
                              newInputs,
                              oldInputs,
                              ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
   fireEvent(event);
   
   for(ossim_uint32 index = 0; index < oldInputs.size(); ++index)
   {
      if(oldInputs[index].valid())
      {
         oldInputs[index]->disconnectMyOutput(this, false);
      }
   }
}

void ossimConnectableObject::disconnectAllOutputs()
{
   if(theOutputObjectList.size() == 0) return;
   if(theOutputObjectList.size() == 1)
   {
      //     ossimConnectableObject* obj = disconnectMyOutput((ossim_int32)0, false);
      disconnectMyOutput((ossim_int32)0);
      //     if(obj)
      //     {
      //        obj->disconnectMyInput(this,
      //                               false);
      //     }
      return;
   }
   ConnectableObjectList::iterator current;
   
   ConnectableObjectList oldOutputs = theOutputObjectList;
   ConnectableObjectList newOutputs;
   
   current = theOutputObjectList.begin();
   while(current != theOutputObjectList.end())
   {
      if(!theOutputListIsFixedFlag)
      {
         current = theOutputObjectList.erase(current);
      }
      else
      {
         *current = 0;
         ++current;
      }
   }
   ossimConnectionEvent event(this,
                              OSSIM_EVENT_CONNECTION_DISCONNECT_ID,
                              newOutputs,
                              oldOutputs,
                              ossimConnectionEvent::OSSIM_OUTPUT_DIRECTION);
   fireEvent(event);
   
   for(ossim_uint32 index = 0; index < oldOutputs.size(); ++index)
   {
      if(oldOutputs[index].valid())
      {
         oldOutputs[index]->disconnectMyInput(this, false);
      }
   }
}

ossim_int32 ossimConnectableObject::connectMyInputTo(ossimConnectableObject* object,
                                                     bool makeOutputConnection,
                                                     bool createEventFlag)
{
   if(!object) return false;
   
   ossim_int32 index = findInputIndex(object);
   if(index >= 0) return index;
   
   index = getMyInputIndexToConnectTo(object);
   
   if(index>-1)
   {
      if(index >= (ossim_int32)theInputObjectList.size())
      {
         if(theInputListIsFixedFlag) return -1;
         
         index = (ossim_int32)theInputObjectList.size();
         theInputObjectList.push_back(object);
      }
      else
      {
         if(!theInputObjectList[index])
         {
            theInputObjectList[index] = object;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN) << "Must issue a detach first!! trying to attach object " << object->getClassName()
            << "\n to input index " << index << " in " << getClassName() << "\n";
            
            return -1;
         }
      }
      if(createEventFlag)
      {
         ossimConnectionEvent event(this,  // owner of message
                                    OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                    theInputObjectList[index].get(),  // new object
                                    0,// old object
                                    ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
         // send event to any listener.
         //
         fireEvent(event);
      }
      
      if(makeOutputConnection&&object)
      {
         // make sure we tell the input not to connect
         // back to us or infinite loop
         //
         object->connectMyOutputTo(this, false, createEventFlag);
      }
   }
   else
   {
      return index;
   }
   
   return index;
}

ossim_int32 ossimConnectableObject::connectMyInputTo(ossim_int32 inputIndex,
                                                     ossimConnectableObject* inputObject,
                                                     bool makeOutputConnection,
                                                     bool createEventFlag)
{
   if(!inputObject)
   {
      if(inputIndex < (ossim_int32)theInputObjectList.size())
      {
         ossimConnectableObject* oldObject = theInputObjectList[inputIndex].get();
         if(theInputListIsFixedFlag)
         {
            theInputObjectList[inputIndex] = 0;
         }
         else
         {
            theInputObjectList.erase(theInputObjectList.begin() + inputIndex);
         }
         if(createEventFlag)
         {
            ossimConnectionEvent event(this,  // owner of message
                                       OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                       0,  // new object
                                       oldObject,// old object
                                       ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
            // send event to any listener.
            //
            fireEvent(event);
         }
         return -1;
      }
      return -1;
   }
   
   ossim_int32 index = findInputIndex(inputObject);
   
   if((index >= 0)&&!inputObject) return index;
   if(!inputObject) return -1;
   
   if(canConnectMyInputTo(inputIndex, inputObject))
   {
      if(inputIndex>-1)
      {
         ossimConnectableObject* oldObject=0;
         if(inputIndex >= (ossim_int32)theInputObjectList.size())
         {
            if(theInputListIsFixedFlag) return -1;
            
            inputIndex = (ossim_int32)theInputObjectList.size();
            theInputObjectList.push_back(inputObject);
         }
         else
         {
            oldObject = theInputObjectList[inputIndex].get();
            theInputObjectList[inputIndex] = inputObject;
            if(oldObject)
            {
               oldObject->disconnectMyOutput(this, false);
            }
         }
         
         if(createEventFlag)
         {
            ossimConnectionEvent event(this,  // owner of message
                                       OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                       theInputObjectList[inputIndex].get(),  // new object
                                       oldObject,// old object
                                       ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
            // send event to any listener.
            //
            fireEvent(event);
         }
         
         if(makeOutputConnection&&inputObject)
         {
            // make sure we tell the input not to connect
            // back to us or infinite loop
            //
            inputObject->connectMyOutputTo(this, false, createEventFlag);
         }
      }
      else
      {
         return inputIndex;
      }
      
      return inputIndex;
   }
   
   return -1;
}

bool ossimConnectableObject::connectMyInputTo(ConnectableObjectList& inputList,
                                              bool makeOutputConnection,
                                              bool createEventFlag)
{
   bool result = true;
   ConnectableObjectList oldInputs;
   ConnectableObjectList newInputs;
   
   if(inputList.size() == 1)
   {
      if(inputList[0].valid())
      {
         return (connectMyInputTo(inputList[0].get(), makeOutputConnection, createEventFlag) >= 0);
      }
      else
      {
         if(theInputListIsFixedFlag)
         {
            theInputObjectList[0] = 0;
         }
         else
         {
            theInputObjectList.clear();
         }
         if(createEventFlag)
         {
            ossimConnectionEvent event(this,
                                       OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                       0,
                                       theInputObjectList[0].get(),
                                       ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
            fireEvent(event);
         }
         
         return true;
      }
   }
   ossim_int32 i = 0;
   for(i = 0; i < (ossim_int32)inputList.size(); ++i)
   {
      if(inputList[i].valid())
      {
         if(connectMyInputTo(inputList[i].get(), makeOutputConnection, false)<0)
         {
            result = false;
         }
         else
         {
            newInputs.push_back(inputList[i]);
         }
      }
      else
      {
         newInputs.push_back(0);
      }
   }
   if(createEventFlag)
   {
      ossimConnectionEvent event(this,
                                 OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                 newInputs,
                                 oldInputs,
                                 ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
      fireEvent(event);
   }
   return result;
}

ossim_int32 ossimConnectableObject::connectMyOutputTo(ossimConnectableObject* output,
                                                      bool makeInputConnection,
                                                      bool createEventFlag)
{
   ossim_int32 index = findOutputIndex(output);
   if(index >= 0) return index;
   
   index = getMyOutputIndexToConnectTo(output);
   
   if(index > -1)
   {
      if((index >= (ossim_int32)theOutputObjectList.size())&&
         !theOutputListIsFixedFlag)
      {
         index = (ossim_int32)theOutputObjectList.size();
         theOutputObjectList.push_back(output);
      }
      else
      {
         if(!theOutputObjectList[index])
         {
            theOutputObjectList[index] = output;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN) << "Must issue a disconnect first!! trying to connect object " << output->getClassName()
            << "\n to output index " << index << " in " << getClassName() << "\n";
            
            return -1;
         }
      }
      if(createEventFlag)
      {
         ossimConnectionEvent event(this,  // owner of message
                                    OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                    theOutputObjectList[index].get(),  // new object
                                    0,// old object
                                    ossimConnectionEvent::OSSIM_OUTPUT_DIRECTION);
         // send event to any listener.
         //
         fireEvent(event);
      }
      if(makeInputConnection&&output)
      {
         // tell the output object not to connect back
         // to us since this is already done.
         //
         output->connectMyInputTo(this, false, createEventFlag);
      }
   }
   else
   {
      return index;
   }
   
   return index;
}

bool ossimConnectableObject::connectMyOutputTo(ConnectableObjectList& outputList,
                                               bool makeInputConnection,
                                               bool createEventFlag)
{
   bool result = true;
   ConnectableObjectList oldOutputs;
   ConnectableObjectList newOutputs;
   
   if(outputList.size() == 0)
   {
      disconnectAllOutputs();
      return true;
   }
   if((outputList.size() == 1)&&outputList[0].valid())
   {
      if(outputList[0].valid())
      {
         return (connectMyOutputTo(outputList[0].get(), makeInputConnection, createEventFlag) >= 0);
      }
      else
      {
         disconnectAllOutputs();
      }
   }
   
   ossim_int32 i = 0;
   for(i = 0; i < (ossim_int32)outputList.size(); ++i)
   {
      if((connectMyOutputTo(outputList[i].get(), makeInputConnection, false)<0)&&outputList[i].valid())
      {
         newOutputs.push_back(outputList[i].get());
         result = false;
      }
   }
   if(createEventFlag&&newOutputs.size())
   {
      ossimConnectionEvent event(this,
                                 OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                 newOutputs,
                                 oldOutputs,
                                 ossimConnectionEvent::OSSIM_OUTPUT_DIRECTION);
      fireEvent(event);
   }
   
   return result;
}

ossimConnectableObject* ossimConnectableObject::getInput(ossim_uint32 idx)
{
   if(idx < theInputObjectList.size())
   {
      return theInputObjectList[idx].get();
   }
   
   return 0;
}

const ossimConnectableObject* ossimConnectableObject::getInput(ossim_uint32 idx)const
{
   if( idx < theInputObjectList.size())
   {
      return theInputObjectList[idx].get();
   }
   
   return 0;
}

ossimConnectableObject* ossimConnectableObject::getOutput(ossim_uint32 idx)
{
   if(idx < theOutputObjectList.size())
   {
      return theOutputObjectList[idx].get();
   }
   
   return 0;
}

bool ossimConnectableObject::connectInputList(ConnectableObjectList& inputList)
{
   bool result = true;
   ossim_uint32 i = 0;
   ConnectableObjectList oldInputs = theInputObjectList;
   ConnectableObjectList newInputs;
   ConnectableObjectList::iterator currentInput = inputList.begin();
   ConnectableObjectList tempOld;
   if(theInputObjectList.size())
   {
      for(i = 0; i < theInputObjectList.size(); ++i)
      {
         if(oldInputs[i].valid())
         {
            tempOld.push_back(oldInputs[i].get());
         }
         theInputObjectList[i] = 0;
      }
   }
   if(theInputListIsFixedFlag && (theInputObjectList.size()==0))
   {
      return false;
   }
   
   if(!theInputListIsFixedFlag)
   {
      theInputObjectList.clear();
   }
   if(tempOld.size())
   {
      for(i = 0; i < tempOld.size(); ++ i)
      {
         tempOld[i]->disconnectMyOutput(this, false);
      }
   }
   //   disconnectAllInputs();
   
   if(inputList.size() == 1)
   {
      return (connectMyInputTo(inputList[0].get())>=0);
   }
   i = 0;
   // now connect the new outputs
   //
   currentInput = inputList.begin();
   result = false;
   
   while(currentInput != inputList.end())
   {
      if((*currentInput).valid())
      {
         if(connectMyInputTo((*currentInput).get(), false) >= 0)
         {
            newInputs.push_back((*currentInput).get());
            result = true;
         }
      }
      ++currentInput;
   }
   ossimConnectionEvent event(this,
                              OSSIM_EVENT_CONNECTION_DISCONNECT_ID,
                              newInputs,
                              oldInputs,
                              ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
   
   if(theInputObjectList.size())
   {
      fireEvent(event);
   }
   
   newInputs          = theInputObjectList;
   
   
   
   event = ossimConnectionEvent(this,
                                OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                newInputs,
                                oldInputs,
                                ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
   fireEvent(event);
   
   return result;
}

bool ossimConnectableObject::connectOutputList(ConnectableObjectList& outputList)
{
   bool result = true;
   ossim_int32 index  = 0;
   ConnectableObjectList oldOutputs = theOutputObjectList;
   ConnectableObjectList newOutputs;
   ConnectableObjectList::const_iterator currentOutput = outputList.begin();
   
   disconnectAllOutputs();
   
   if(outputList.size() == 1)
   {
      return (connectMyOutputTo(outputList[0].get())>=0);
   }
   while(currentOutput != outputList.end())
   {
      if(!canConnectMyOutputTo(index, (*currentOutput).get()))
      {
         result = false;
      }
      ++currentOutput;
   }
   
   if(!result)
   {
      return false;
   }
   
   theOutputObjectList = outputList;
   newOutputs = theOutputObjectList;
   
   ossimConnectionEvent event(this,
                              OSSIM_EVENT_CONNECTION_CONNECT_ID,
                              newOutputs,
                              oldOutputs,
                              ossimConnectionEvent::OSSIM_OUTPUT_DIRECTION);
   fireEvent(event);
   
   return result;
}

void ossimConnectableObject::setNumberOfInputs(ossim_int32 numberOfInputs)
{
   if((ossim_int32)theInputObjectList.size() == numberOfInputs)
   {
      return;
   }
   ossim_int32 i = 0;
   if(numberOfInputs < (ossim_int32)theInputObjectList.size())
   {
      ConnectableObjectList v(theInputObjectList.begin(),
                              theInputObjectList.begin()+numberOfInputs);
      ConnectableObjectList disconnectList;
      
      for(i = numberOfInputs;
          i < (ossim_int32)theInputObjectList.size();
          ++i)
      {
         if(theInputObjectList[i].valid())
         {
            disconnectList.push_back(theInputObjectList[i]);
         }
      }
      disconnectMyInputs(disconnectList);
      
      theInputObjectList.clear();
      theInputObjectList = v;
   }
   else
   {
      for(i = (ossim_int32)theInputObjectList.size();
          i < numberOfInputs;
          ++i)
      {
         theInputObjectList.push_back(0);
      }
   }
}

void ossimConnectableObject::setNumberOfOutputs(ossim_int32 numberOfOutputs)
{
   if((ossim_int32)theOutputObjectList.size() == numberOfOutputs)
   {
      return;
   }
   ossim_int32 i = 0;
   if(numberOfOutputs < (ossim_int32)theOutputObjectList.size())
   {
      ConnectableObjectList v(theOutputObjectList.begin(),
                              theOutputObjectList.begin()+numberOfOutputs);
      ConnectableObjectList disconnectList;
      for(i = numberOfOutputs;
          i < (ossim_int32)theOutputObjectList.size();
          ++i)
      {
         if(theOutputObjectList[i].valid())
         {
            disconnectList.push_back(theOutputObjectList[i]);
         }
      }
      disconnectMyOutputs(disconnectList);
      
      theOutputObjectList.clear();
      theOutputObjectList = v;
   }
   else
   {
      for(i = (ossim_int32)theOutputObjectList.size();
          i < numberOfOutputs;
          ++i)
      {
         theOutputObjectList.push_back(0);
      }
   }
}


const ossimConnectableObject* ossimConnectableObject::getOutput(ossim_uint32 idx)const
{
   if(idx < theOutputObjectList.size())
   {
      return theOutputObjectList[idx].get();
   }
   
   return 0;
}

void  ossimConnectableObject::findAllObjectsOfType(ConnectableObjectList& result,
                                                   const RTTItypeid& typeInfo,
                                                   bool recurse)
{
   int j;
   // go through children first
   //
   ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                        this);
   if(inter)
   {
      ConnectableObjectList tempList = inter->findAllObjectsOfType(typeInfo,
                                                                   recurse);
      
      for(j = 0; j < (int)tempList.size(); ++j)
      {
         ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), tempList[j]);
         if(iter == result.end())
         {
            result.push_back(tempList[j].get());
         }
      }
   }
   
}

void ossimConnectableObject::findAllObjectsOfType(ConnectableObjectList& result, 
                                                  const ossimString& className,
                                                  bool recurse)
{
   int j;
   // go through children first
   //
   ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                        this);
   if(inter)
   {
      ConnectableObjectList tempList = inter->findAllObjectsOfType(className,
                                                                   recurse);
      
      for(j = 0; j < (int)tempList.size(); ++j)
      {
         ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), tempList[j]);
         if(iter == result.end())
         {
            result.push_back(tempList[j].get());
         }
      }
   }
}

#if 0
void ossimConnectableObject::findAllInputsOfType(ConnectableObjectList& result,
                                                 const RTTItypeid& typeInfo,
                                                 bool propagateToInputs,
                                                 bool recurseChildren)
{
   int i;
   int j;
   // go through children first
   //
   ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                        this);
   if(inter&&recurseChildren)
   {
      ConnectableObjectList tempList = inter->findAllObjectsOfType(typeInfo,
                                                                   true);
      
      for(j = 0; j < (int)tempList.size(); ++j)
      {
         ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), tempList[j]);
         if(iter == result.end())
         {
            result.push_back(tempList[j].get());
         }
      }
   }
   
   for(i = 0; i < (int)getNumberOfInputs(); ++i)
   {
      ossimConnectableObject* current = getInput(i);
      if(current&&(current->canCastTo(typeInfo)))
      {
         ConnectableObjectList::iterator position = std::find(result.begin(), result.end(), current);
         
         if(position == result.end())
         {
            result.push_back(current);
         }
      }
      inter = PTR_CAST(ossimConnectableContainerInterface, current);
      if(inter)
      {
         ConnectableObjectList tempList = inter->findAllObjectsOfType(typeInfo, true);
         for(j = 0; j < (int)tempList.size(); ++j)
         {
            ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), tempList[j]);
            if(iter == result.end())
            {
               result.push_back(tempList[j]);
            }
         }
      }
      
      if(propagateToInputs&&current)
      {
         current->findAllInputsOfType(result,
                                      typeInfo,
                                      true,
                                      recurseChildren);
      }
   }
}

void ossimConnectableObject::findAllInputsOfType(ConnectableObjectList& result,
                                                 const ossimString& className,
                                                 bool propagateToInputs,
                                                 bool recurseChildren)
{
   int j;
   // go through children first
   //
   ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                        this);
   if(inter&&recurseChildren)
   {
      ConnectableObjectList tempList = inter->findAllObjectsOfType(className,
                                                                   true);
      
      for(j = 0; j < (int)tempList.size(); ++j)
      {
         ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), tempList[j]);
         if(iter == result.end())
         {
            result.push_back(tempList[j]);
         }
      }
   }
   for(ossim_uint32 i = 0; i < getNumberOfInputs(); ++i)
   {
      ossimConnectableObject* current = getInput(i);
      if(current&&(current->canCastTo(className)))
      {
         ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), current);
         if(iter == result.end())
         {
            result.push_back(current);
         }
      }
      ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                           current);
      if(inter)
      {
         ConnectableObjectList tempList = inter->findAllObjectsOfType(className, true);
         for(j = 0; j < (int)tempList.size(); ++j)
         {
            ConnectableObjectList::iterator iter = std::find(result.begin(), result.end(), tempList[j]);
            if(iter == result.end())
            {
               result.push_back(tempList[j]);
            }
         }
      }
      
      if(propagateToInputs&&current)
      {
         current->findAllInputsOfType(result,
                                      className,
                                      true,
                                      recurseChildren);
      }
   }
}
#endif

void ossimConnectableObject::propagateEventToOutputs(ossimEvent& event)
{
   ossim_uint32 i;
   
   for(i = 0; i <getNumberOfOutputs(); ++i)
   {
      ossimConnectableObject* obj = getOutput(i);
      if(obj)
      {
         event.setPropagationType(ossimEvent::PROPAGATION_OUTPUT);
         obj->fireEvent(event);
         obj->propagateEventToOutputs(event);
      }
   }
}

void ossimConnectableObject::propagateEventToInputs(ossimEvent& event)
{
   ossim_uint32 i;
   
   for(i = 0; i <getNumberOfInputs(); ++i)
   {
      ossimConnectableObject* obj = getInput(i);
      if(obj)
      {
         obj->fireEvent(event);
         obj->propagateEventToInputs(event);
      }
   }
}

void ossimConnectableObject::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;
   if(property->getName() == "Description")
   {
      property->valueToString(theDescription);
   }
}

void ossimConnectableObject::setProperty(const ossimString& name, const ossimString& value)
{
   ossimPropertyInterface::setProperty(name, value);
}

ossimRefPtr<ossimProperty> ossimConnectableObject::getProperty(const ossimString& name)const
{
   if(name == "Description")
   {
      return new ossimTextProperty(name, theDescription);
   }
   // "Class name" check for backwards compatibility only.
   else if( (name == "class_name") ||
           (name == "Class name") ) 
   {
      ossimProperty* prop = new ossimTextProperty(name,
                                                  getClassName());
      prop->setReadOnlyFlag(true);
      
      return prop;
   }
   return ossimRefPtr<ossimProperty>();
}

void ossimConnectableObject::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back("class_name");
   propertyNames.push_back("Description");
}

bool ossimConnectableObject::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   const char* lookup = kwl.find(prefix,
                                 ossimKeywordNames::ID_KW);
   
   // disconnect(this);
   
   if(lookup)
   {
      theId = ossimId(ossimString(lookup).toLong());
   }
   
   lookup = kwl.find(prefix, CONNECTABLE_INPUT_LIST_FIXED_KW);
   if(lookup)
   {
      theInputListIsFixedFlag = ossimString(lookup).toBool();
   }
   
   lookup = kwl.find(prefix, CONNECTABLE_OUTPUT_LIST_FIXED_KW);
   if(lookup)
   {
      theOutputListIsFixedFlag = ossimString(lookup).toBool();
   }
   
   ossim_int32 numberInputs  = 0;
   ossim_int32 numberOutputs = 0;
   
   ossimString regExpression;
   lookup = kwl.find(prefix, ossimKeywordNames::NUMBER_INPUTS_KW);
   if(lookup)
   {
      numberInputs = ossimString(lookup).toLong();
   }
   else if(!theInputListIsFixedFlag)
   {
      regExpression = ossimString("^(") + ossimString(prefix) + "input_connection[0-9]+)";
      numberInputs = kwl.getNumberOfSubstringKeys(regExpression);
   }
   else
   {
      // if we are fixed then the list should already be set
      numberInputs = (ossim_int32) theInputObjectList.size();
   }

   
   lookup = kwl.find(prefix, ossimKeywordNames::NUMBER_OUTPUTS_KW);
   if(lookup)
   {
      numberOutputs = ossimString(lookup).toLong();
   }
   else if(!theOutputListIsFixedFlag)
   {
      regExpression = ossimString("^(") + ossimString(prefix) + "output_connection[0-9]+)";
      numberOutputs = kwl.getNumberOfSubstringKeys(regExpression);
   }
   else 
   {
      // if we are fixed then the list should already be set
      numberOutputs = (ossim_int32) theOutputObjectList.size();
   }

   lookup = kwl.find(prefix, ossimKeywordNames::DESCRIPTION_KW);
   if (lookup)
   {
      theDescription = lookup;
   }
   
   setNumberOfInputs(numberInputs);
   setNumberOfOutputs(numberOutputs);
   
   return ossimObject::loadState(kwl, prefix);
}

bool ossimConnectableObject::saveState(ossimKeywordlist& kwl,
                                       const char* prefix)const
{
   ossimObject::saveState(kwl, prefix);
   
   kwl.add(prefix,
           ossimKeywordNames::ID_KW,
           theId.getId(),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::DESCRIPTION_KW,
           theDescription,
           true);
   
   kwl.add(prefix,
           CONNECTABLE_INPUT_LIST_FIXED_KW,
           theInputListIsFixedFlag,
           true);
   
   kwl.add(prefix,
           CONNECTABLE_OUTPUT_LIST_FIXED_KW,
           theOutputListIsFixedFlag,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_INPUTS_KW,
           static_cast<ossim_uint32>(theInputObjectList.size()),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_OUTPUTS_KW,
           static_cast<ossim_uint32>(theOutputObjectList.size()),
           true);
   
   ossim_int32 i = 0;
   for(i = 1; i <= (ossim_int32)theInputObjectList.size(); ++i)
   {
      ossimString value = "input_connection" + ossimString::toString(i);
      
      ossim_int32 id;
      
      if(theInputObjectList[i-1].valid())
      {
         id = theInputObjectList[i-1]->getId().getId();
      }
      else
      {
         id = -1;
      }
      kwl.add(prefix,
              value.c_str(),
              id,
              true);
   }
   
   for(i = 1; i <= (ossim_int32)theOutputObjectList.size(); ++i)
   {
      ossimString value = "output_connection" + ossimString::toString(i);
      
      ossim_int32 id;
      
      if(theOutputObjectList[i-1].valid())
      {
         id = theOutputObjectList[i-1]->getId().getId();
      }
      else
      {
         id = -1;
      }
      kwl.add(prefix,
              value.c_str(),
              id,
              true);
   }
   
   return true;
}

ossim_uint32 ossimConnectableObject::saveStateOfAllInputs(ossimKeywordlist& kwl,
                                                          bool              saveThisStateFlag,
                                                          ossim_uint32      objectIndex,
                                                          const char*       prefix) const
{
   ossim_uint32 index = objectIndex;

   const ossim_uint32 NUMBER_OF_INPUTS = getNumberOfInputs();
   if (NUMBER_OF_INPUTS)
   {
      // Save all the inputs.
      for(ossim_uint32 i = 0; i < NUMBER_OF_INPUTS; ++i)
      {
         const ossimConnectableObject* input = getInput(i);
         if(input)
            index = input->saveStateOfAllInputs(kwl, true, index, prefix);
      }
   }

   if (saveThisStateFlag)
   {
      ossimString myPrefix;
      if (prefix)
         myPrefix = prefix;

      myPrefix += "object" + ossimString::toString(index) + ".";

      // Save the state of this object.
      saveState(kwl, myPrefix.c_str());
      ++index;
   }

   return index;
}

bool ossimConnectableObject::fillContainer(ossimConnectableContainer& container)
{
   // Insert inputs into the container:
   bool good_fill = true;
   ossim_uint32 num_inputs = getNumberOfInputs();
   for(ossim_uint32 i=0; (i<num_inputs) && good_fill; ++i)
   {
      ossimConnectableObject* input = getInput(i);
      if (input)
         good_fill = input->fillContainer(container);
   }
   
   // Insert this object and all of its children and inputs into the container provided:
   if (good_fill)
      good_fill = container.addChild(this);

   return good_fill;
}

bool ossimConnectableObject::canConnectMyOutputTo(ossim_int32 myOutputIndex,
                                                  const ossimConnectableObject* /* object */ ) const
{
   if(theOutputListIsFixedFlag)
   {
      return ((myOutputIndex >= 0) &&
              (myOutputIndex < (ossim_int32)theOutputObjectList.size()));
   }
   
   return ((myOutputIndex >= 0) &&
           (myOutputIndex  <= (ossim_int32)theOutputObjectList.size()));
}

bool ossimConnectableObject::moveInputUp(const ossimId& id)
{
   bool result = false;
   
   if (theInputListIsFixedFlag == false)
   {
      if ( theInputObjectList.size() )
      {
         ossim_int32 indexOfId = findInputIndex(id);
         
         if (indexOfId > 0)
         {
            ConnectableObjectList oldInputs =
            theInputObjectList;
            
            // Swap with index above.
            ossimRefPtr<ossimConnectableObject> tmpObj  = theInputObjectList[indexOfId].get();
            theInputObjectList[indexOfId]   = theInputObjectList[indexOfId-1].get();
            theInputObjectList[indexOfId-1] = tmpObj;
            result = true;
            
            ConnectableObjectList newInputs =
            theInputObjectList;
            
            ossimConnectionEvent event(this,
                                       OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                       newInputs,
                                       oldInputs,
                                       ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
            fireEvent(event);
         }
      }
   }
   
   return result;
}

bool ossimConnectableObject::moveInputDown(const ossimId& id)
{
   bool result = false;
   
   if (theInputListIsFixedFlag == false)
   {
      if ( theInputObjectList.size() )
      {
         ossim_int32 indexOfId = findInputIndex(id);
         
         if ( indexOfId <
             static_cast<ossim_int32>(theInputObjectList.size()-1) )
         {
            ConnectableObjectList oldInputs =
            theInputObjectList;
            
            // Swap with index below.
            ossimRefPtr<ossimConnectableObject> tmpObj  = theInputObjectList[indexOfId].get();
            theInputObjectList[indexOfId]   = theInputObjectList[indexOfId+1].get();
            theInputObjectList[indexOfId+1] = tmpObj;
            result = true;
            
            ConnectableObjectList newInputs =
            theInputObjectList;
            
            ossimConnectionEvent event(this,
                                       OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                       newInputs,
                                       oldInputs,
                                       ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
            fireEvent(event);
         }
      }
   }
   
   return result;
}

bool ossimConnectableObject::moveInputToTop(const ossimId& id)
{
   bool result = false;
   
   if (theInputListIsFixedFlag == false)
   {
      if ( theInputObjectList.size() )
      {
         ConnectableObjectList::iterator i =
         theInputObjectList.begin();
         
         while (i != theInputObjectList.end())
         {
            if ( (*i)->getId() == id )
            {
               break;
            }
            ++i;
         }
         
         if ( (i != theInputObjectList.begin()) &&
             (i != theInputObjectList.end()) )
         {
            ConnectableObjectList oldInputs =
            theInputObjectList;
            
            ossimRefPtr<ossimConnectableObject> obj = (*i).get();               
            theInputObjectList.erase(i);
            theInputObjectList.insert(theInputObjectList.begin(), obj.get());
            result = true;
            
            ConnectableObjectList newInputs =
            theInputObjectList;
            
            ossimConnectionEvent event(
                                       this,
                                       OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                       newInputs,
                                       oldInputs,
                                       ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
            fireEvent(event);
         }
      }
   }
   
   return result;
}

bool ossimConnectableObject::moveInputToBottom(const ossimId& id)
{
   bool result = false;
   
   if (theInputListIsFixedFlag == false)
   {
      if ( theInputObjectList.size() )
      {
         ConnectableObjectList::iterator bottom =
         theInputObjectList.end()-1;
         
         // if not bottom already
         if ( (*bottom)->getId() != id ) 
         {
            ConnectableObjectList::iterator i =
            theInputObjectList.begin();
            
            while (i != bottom)
            {
               if ( (*i)->getId() == id )
               {
                  break;
               }
               ++i;
            }
            
            if (i != bottom)
            {
               ConnectableObjectList oldInputs =
               theInputObjectList;
               
               ossimRefPtr<ossimConnectableObject> obj = (*i).get();
               theInputObjectList.erase(i);
               theInputObjectList.push_back(obj);
               result = true;
               
               ConnectableObjectList newInputs =
               theInputObjectList;
               
               ossimConnectionEvent event(
                                          this,
                                          OSSIM_EVENT_CONNECTION_CONNECT_ID,
                                          newInputs,
                                          oldInputs,
                                          ossimConnectionEvent::OSSIM_INPUT_DIRECTION);
               fireEvent(event);
            }
         }
      }
   }
   
   return result;
}

void ossimConnectableObject::accept(ossimVisitor& visitor)
{
   if(!visitor.stopTraversal())
   {
      if(!visitor.hasVisited(this))
      {
         visitor.visit(this);
      }
      
      if(!visitor.stopTraversal())
      {
         
         if(visitor.getVisitorType() & ossimVisitor::VISIT_INPUTS)
         {
            ConnectableObjectList::iterator current = theInputObjectList.begin();
            while(current != theInputObjectList.end())
            {
               if((*current).get()&&!visitor.hasVisited((*current).get())) (*current)->accept(visitor);
               ++current;
            }
         }
         
         if(visitor.getVisitorType() & ossimVisitor::VISIT_OUTPUTS)
         {
            // go through the outputs
            ConnectableObjectList::iterator current = theOutputObjectList.begin();
            while(current != theOutputObjectList.end())
            {
               if((*current).get()&&!visitor.hasVisited((*current).get())) (*current)->accept(visitor);
               ++current;
            }
            ossimConnectableObject* obj = dynamic_cast<ossimConnectableObject*>(theOwner);
            
            if((!getNumberOfOutputs()||!isConnected(CONNECTABLE_DIRECTION_OUTPUT))&&obj)
            {
               ossimVisitor::VisitorType currentType = visitor.getVisitorType();
               // lets make sure inputs and outputs are turned off for we are traversing all children and we should not have
               // to have that enabled
               //
               visitor.turnOffVisitorType(ossimVisitor::VISIT_INPUTS);// |ossimVisitor::VISIT_CHILDREN);
               
               //obj->accept(visitor);
               visitor.setVisitorType(currentType);
              // visitor.turnOffVisitorType(ossimVisitor::VISIT_INPUTS);
               // now go through outputs
               //
               ConnectableObjectList::iterator current = obj->theOutputObjectList.begin();
               while(current != obj->theOutputObjectList.end())
               {
                  if((*current).get()&&!visitor.hasVisited((*current).get())) (*current)->accept(visitor);
                  ++current;
               }
               
               visitor.setVisitorType(currentType);
               
            }
         } 
      }  
   }
}

void ossimConnectableObject::setId(const ossimId& id)
{
   theId = id;
}

const ossimId& ossimConnectableObject::getId()const
{
   return theId;
}

const ossimObject* ossimConnectableObject::getOwner() const
{
   return theOwner;
}

ossim_uint32 ossimConnectableObject::getNumberOfInputs()const
{
   return (ossim_uint32)theInputObjectList.size();
}

ossim_uint32 ossimConnectableObject::getNumberOfOutputs()const
{
   return (ossim_uint32)theOutputObjectList.size();
}

bool ossimConnectableObject::getInputListIsFixedFlag()const
{
   return theInputListIsFixedFlag;
}

bool ossimConnectableObject::getOutputListIsFixedFlag()const
{
   return theOutputListIsFixedFlag;
}

const ossimConnectableObject::ConnectableObjectList& ossimConnectableObject::getInputList()const
{
   return theInputObjectList;
}

const ossimConnectableObject::ConnectableObjectList& ossimConnectableObject::getOutputList()const
{
   return theOutputObjectList;
}

ossimConnectableObject::ConnectableObjectList& ossimConnectableObject::getInputList()
{
   return theInputObjectList;
}

ossimConnectableObject::ConnectableObjectList& ossimConnectableObject::getOutputList()
{
   return theOutputObjectList;
}
