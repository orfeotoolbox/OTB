//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
// 
// Contains class declaration for ossimConnectableObject the base class for
// all connectable objects.
//
//*************************************************************************
// $Id: ossimConnectableObject.h,v 1.54 2005/12/27 15:47:49 dburken Exp $

#ifndef ossimConnectableObject_HEADER
#define ossimConnectableObject_HEADER

#include <vector>

#include <base/common/ossimObject.h>
#include <base/common/ossimId.h>
#include <base/common/ossimConstants.h>
#include <base/common/events/ossimListenerManager.h>
#include <base/common/ossimPropertyInterface.h>

/**
 * This class is the base of all connectable objects.  It will manage
 * firing any attachment events (changing, adding and removing, etc.
 * This allows us to perform automatic notification to nodes
 * to the input's or output's.
 *
 * The only methods that the derived classes should override are the
 * canConnect methods.
 */
class  OSSIMDLLEXPORT  ossimConnectableObject : public ossimObject,
                                                public ossimListenerManager,
                                                public ossimPropertyInterface
{
public:
   enum ossimConnectableObjectDirectionType
   {
      CONNECTABLE_DIRECTION_NONE   = 0,
      CONNECTABLE_DIRECTION_INPUT  = 1,
      CONNECTABLE_DIRECTION_OUTPUT = 2
   };
   /**
    * Base constructor of this object.
    */
   ossimConnectableObject(ossimObject* owner=0);
   ossimConnectableObject(ossimObject* owner,
                          long inputListSize,
                          long outputListSize,
                          bool inputListIsFixedFlag=true,
                          bool outputListIsFixedFlag=true);

   virtual ~ossimConnectableObject();

   /**
    * All connectable objects will have id's.  This allows us to
    * set the id of this object.
    */
   void setId(const ossimId& id){theId = id;}

   /**
    * Will allow us to get this object's id.
    */
   const ossimId& getId()const{return theId;}
   
   /**
    * Fetches the current owner, most likely a container but not limited to one.
    */
   const ossimObject* getOwner() const { return theOwner; }

   /**
    * Permits changing the object's owner.
    */
   virtual void changeOwner(ossimObject* owner);

   virtual void setDescription(const ossimString& description);
   virtual ossimString getDescription()const;
   
   /**
    * will check the direction specified to see if all slots are full.
    * If any slot is null then false is returned.
    * If a filter requires options testing they
    * need to override this method.  The argument can be ored together. If you
    * want both checked then pass
    * CONNECTABLE_DIRECTION_INPUT | CONNECTABLE_DIRECTION_OUTPUT
    */
   virtual bool isConnected(ossimConnectableObjectDirectionType direction = CONNECTABLE_DIRECTION_INPUT)const;


   /**
    *
    * Searches the input list and the output list for the
    * passed in id.  Returns NULL if it wasn't found.
    *
    */
   virtual ossimConnectableObject* findConnectableObject(const ossimId& id);

   /**
    * Will search for a given object of the passed in RTTI type.
    */
   virtual ossimConnectableObject* findObjectOfType(RTTItypeid typeId,
                                                    ossimConnectableObjectDirectionType directionType,
                                                    bool recurse = true);

   virtual ossimConnectableObject* findObjectOfType(const ossimString& obj,
                                                    ossimConnectableObjectDirectionType directionType,
                                                    bool recurse = true);

   /**
    * @param className Class name of object to look for.
    * 
    * @return object of the passed in RTTI type or NULL if not found.
    *
    * @note This method does NOT traverse beyond any objects the do not have
    * a fixed input of one connection.
    */
   virtual ossimConnectableObject* findInputObjectOfType(
      const ossimString& className);
   
   /**
    * Return a valid index of the input list if the passed in object
    * is found else return -1.
    */
   virtual ossim_int32 findInputIndex(const ossimConnectableObject* object);

   /**
    * Return a valid index of the input list if the passed id
    * is found else return -1.
    */
   virtual ossim_int32 findInputIndex(const ossimId& id);

   /**
    * Return a valid index of the output list if the passed in object
    * is found else return -1.
    */
   virtual ossim_int32 findOutputIndex(const ossimConnectableObject* object);

   /**
    * Return a valid index of the output list if the passed in object
    * is found else return -1.
    */   
   virtual ossim_int32 findOutputIndex(const ossimId& id);
   
   /**
    * Should return the first available index to connect to.  The connectMyInputTo
    * that just takes another ossimConnectableObject as input and not an index
    * will call this method.  By default this method will find the first open
    * slot (not null) or append o the list if it's dynamic.  if the list is
    * dynamic it calls conConnectMyInputTo(index, object) on an index = to listSize.
    * 
    */
   virtual ossim_int32 getMyInputIndexToConnectTo(ossimConnectableObject* object)const;
   

   /**
    * Should return the first available index to connect to.  The connectMyOutputTo
    * that just takes another ossimConnectableObject as input and not an index
    * will call this method.  By default this method will find the first open
    * slot (not null) or append to the list if it's dynamic.  if the list is
    * dynamic it calls canConnectMyOutputTo(index, object) on an index = to listSize.
    * 
    */
   virtual ossim_int32 getMyOutputIndexToConnectTo(ossimConnectableObject* object)const;
   
   /**
    * required to be overriden by derived classes
    */
   virtual bool canConnectMyInputTo(ossim_int32 myInputIndex,
                                    const ossimConnectableObject* object)const=0;
   
   /**
    * default implementation is to allow anyone to connect to us.
    */
   virtual bool canConnectMyOutputTo(ossim_int32 myOutputIndex,
                                     const ossimConnectableObject* object)const;
   
   /**
    * Will disconnect the object passed in.
    */
   virtual void disconnect(ossimConnectableObject* object=0);

   /**
    * Will disconnect the object passed in.
    */
   virtual void disconnect(const ossimId& id);
   
   /**
    * Will disconnect the object at the given input index and generate
    * a connection event.
    */
   virtual ossimConnectableObject* disconnectMyInput(ossim_int32 inputIndex,
						     bool disconnectOutputFlag=true,
						     bool createEventFlag = true);
  
  /**
   * Finds the index of the passed in input and calls
   * disconnectMyInput(inputIndex, disconnectOutputFlag, createEventFlag);
   */
   virtual void disconnectMyInput(ossimConnectableObject* input,
                                  bool disconnectOutputFlag=true,
                                  bool createEventFlag = true);
   /**
    *
    */
   virtual void disconnectMyInputs(std::vector<ossimConnectableObject*>& inputList,
                                   bool disconnectOutputFlag=true,
                                   bool createEventFlag=true);
   
   /**
    * Will disconnect the object at the given output index and generate
    * a connection event.  If there is no object at that index then no
    * event is generated and NULL is returned.  The disconnectOutputFlag
    * says do you want this method to disconnect the output pointer to
    * this object.
    */
   virtual ossimConnectableObject* disconnectMyOutput(ossim_int32 outputIndex,
						      bool disconnectInputFlag=true,
						      bool createEventFlag = true);
  
   /**
    * Will disconnect the output object.  It will get the index of
    * the object and call disconnectMyOutput(index, disconnectOutputFlag).
    */
   virtual void disconnectMyOutput(ossimConnectableObject* output,
                                   bool disconnectInputFlag=true,
                                   bool createEventFlag=true);

   virtual void disconnectMyOutputs(std::vector<ossimConnectableObject*>& outputList,
                                    bool disconnectOutputFlag=true,
                                    bool createEventFlag=true);
   /**
    * Will disconnect all of the input objects.
    */
   virtual void disconnectAllInputs();

   /**
    * Will disconnect all of the output objects.
    */
   virtual void disconnectAllOutputs();

   /**
    * Will try to connect this objects input to the passed in object.
    * It will return a valid index >= 0 if successful.  Will use the
    * getMyInputIndexToConnectTo method to implement the connection
    */
   virtual ossim_int32 connectMyInputTo(ossimConnectableObject* inputObject,
                                        bool makeOutputConnection=true,
                                        bool createEventFlag=true);

   /**
    * Will connect the specified input to the passed in object
    */
   virtual ossim_int32 connectMyInputTo(ossim_int32 inputIndex,
                                        ossimConnectableObject* inputObject,
                                        bool makeOutputConnection=true,
                                        bool createEventFlag=true);
   
   virtual bool connectMyInputTo(std::vector<ossimConnectableObject*>& inputList,
                                 bool makeOutputConnection=true,
                                 bool createEventFlag = true);   

   /**
    * Will try to connect this objects output to the passed in object.
    * It will return a valid index >= 0 if successful.  It will in turn call
    * the passed in objects connect input command if makeInputConnection
    * is set to true.
    *
    * Will use the getMyOutputIndexToConnectTo to implement this method
    */
   virtual ossim_int32 connectMyOutputTo(ossimConnectableObject* outputObject,
                                         bool makeInputConnection=true,
                                         bool createEventFlag=true);
   
   virtual bool connectMyOutputTo(std::vector<ossimConnectableObject*>& outputList,
                                  bool makeInputConnection=true,
                                  bool createEventFlag=true);
   
   /**
    * Will disconnect itself from all inputs and reset to the passed in
    * input list.  It will return true if all objects were successfully
    * added.
    */
   virtual bool connectInputList(std::vector<ossimConnectableObject*>& inputList);
   
   /**
    * Will disconnect itself from all outputs and reset to the passed in
    * output list.  It will return true if all objects were successfully
    * added.
    */
   virtual bool connectOutputList(std::vector<ossimConnectableObject*>& outputList);
   
   /**
    * Returns the number of input objects.
    */
   virtual ossim_uint32 getNumberOfInputs()const
      {
         return (ossim_uint32)theInputObjectList.size();
      }

   /**
    * Return the number of output objects
    */
   virtual ossim_uint32 getNumberOfOutputs()const
      {
         return (ossim_uint32)theOutputObjectList.size();
      }
   
   /**
    * returns the object at the specified index.
    * if the index is not valid then NULL is
    * returned
    */
   ossimConnectableObject* getInput(ossim_uint32 index=0);   

   /**
    * returns the object at the specified index.
    * if the index is not valid then NULL is
    * returned
    */
   const ossimConnectableObject* getInput(ossim_uint32 index=0)const;

   /**
    * returns the object at the specified index.
    * if the index is not valid then NULL is
    * returned
    */
   ossimConnectableObject* getOutput(ossim_uint32 index=0);

   /**
    * returns the object at the specified index.
    * if the index is not valid then NULL is
    * returned
    */
   const ossimConnectableObject* getOutput(ossim_uint32 index=0)const;
   
   /**
    * Will set the number of inputs.  This will expand the list if the number
    * of inputs is larger than the current number of inputs.  Will shrink
    * the list if the current number of inputs is larger than the passed in
    * inputs.
    */
   virtual void setNumberOfInputs(ossim_int32 numberOfInputs);

   virtual bool getInputListIsFixedFlag()const
      {
         return theInputListIsFixedFlag;
      }

   virtual bool getOutputListIsFixedFlag()const
      {
         return theOutputListIsFixedFlag;
      }
   /**
    * Will set the number of outputs.  This will expand the list if the
    * number of outputs is larger than the current number of outputs.
    * Will shrink the list if the current number of outputs is larger than
    * the passed in outputs.
    */
   virtual void setNumberOfOutputs(ossim_int32 numberOfInputs);
   
   const std::vector<ossimConnectableObject*>& getInputList()const
      {
         return theInputObjectList;
      }
   const std::vector<ossimConnectableObject*>& getOutputList()const
      {
         return theOutputObjectList;
      }
   std::vector<ossimConnectableObject*>& getInputList()
      {
         return theInputObjectList;
      }
   std::vector<ossimConnectableObject*>& getOutputList()
      {
         return theOutputObjectList;
      }

   virtual void findAllInputsOfType(vector<ossimConnectableObject*>& result,
                                    const RTTItypeid& typeInfo,
                                    bool propagateToInputs=true,
                                    bool recurseChildren=false);
   virtual void findAllInputsOfType(vector<ossimConnectableObject*>& result,
                                    const ossimString& className,
                                    bool propagateToInputs=true,
                                    bool recurseChildren=false);
   virtual void findAllOutputsOfType(vector<ossimConnectableObject*>& result,
                                     const ossimString& className,
                                     bool propagateToOutputs=true,
                                    bool recurseChildren=false);
   virtual void findAllOutputsOfType(vector<ossimConnectableObject*>& result,
                                     const RTTItypeid& typeInfo,
                                     bool propagateToOutputs=true,
                                    bool recurseChildren=false);
   
   virtual void propagateEventToOutputs(ossimEvent& event);
   virtual void propagateEventToInputs(ossimEvent& event);

   /* ------------------- PROPERTY INTERFACE -------------------- */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /* ------------------ PROPERTY INTERFACE END ------------------- */

   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = (const char*)NULL);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = (const char*)NULL)const;

   /**
    * Save the state of all inputs to a keyword list.  This will do a
    * recursive call through all inputs and save their state to a
    * keyword list.
    *
    * @param kwl Keyword list to save state to.
    *
    * @param saveThisStateFlag If the true the state of this object will be
    * saved as well as all inputs. Default = true.
    *
    * @param  objectIndex Index to be used for prefix.  So if
    * objectIndex = 1, the first object will have a prefix of: "object1."
    * Default = 1.
    *
    * @param prefix Prefix to tack onto keyword.  If prefix = "myChain." and
    * objectIndex = 1, then the first keyword would be
    * "myChain.object1.some_keyword".
    *
    * @return Returns the next object index.  This is the last index used
    * plus one.  Used internally to keep track of index to use for prefix.
    */
   virtual ossim_uint32 saveStateOfAllInputs(ossimKeywordlist& kwl,
                                             bool saveThisStateFlag=true,
                                             ossim_uint32 objectIndex=1,
                                             const char* prefix=NULL) const;
   
protected:
   ossimId      theId;
   ossimString  theDescription;
   ossimObject* theOwner;

   /**
    * Indicates whether the theInputObjectList is fixed
    */
   bool theInputListIsFixedFlag;

   /**
    * Indicates whether the theOutputObjectList is fixed
    */
   bool theOutputListIsFixedFlag;
   
   /**
    * Holds a list of input objects.
    */
   std::vector<ossimConnectableObject*>     theInputObjectList;
   
   /**
    * Holds a list of output objects.
    */
   std::vector<ossimConnectableObject*>     theOutputObjectList;

private:
TYPE_DATA
};

#endif
