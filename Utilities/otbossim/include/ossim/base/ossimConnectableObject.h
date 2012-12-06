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
// $Id: ossimConnectableObject.h 21852 2012-10-21 20:16:04Z dburken $

#ifndef ossimConnectableObject_HEADER
#define ossimConnectableObject_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimId.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimListenerManager.h>
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/base/ossimRefPtr.h>
#include <vector>

class ossimVisitor;
class ossimConnectableContainer;

class  OSSIMDLLEXPORT  ossimConnectableObject : public ossimObject,
                                                public ossimListenerManager,
                                                public ossimPropertyInterface
{
public:
   typedef std::vector<ossimRefPtr<ossimConnectableObject> > ConnectableObjectList;
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
                          ossim_int32 inputListSize,
                          ossim_int32 outputListSize,
                          bool inputListIsFixedFlag=true,
                          bool outputListIsFixedFlag=true);

   virtual ~ossimConnectableObject();
   /**
    * All connectable objects will have id's.  This allows us to
    * set the id of this object.
    */
   void setId(const ossimId& id);

   /**
    * Will allow us to get this object's id.
    */
   const ossimId& getId()const;
   
   /**
    * Fetches the current owner, most likely a container but not limited to one.
    */
   const ossimObject* getOwner() const;

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
   virtual bool isConnected(
      ossimConnectableObjectDirectionType direction = CONNECTABLE_DIRECTION_INPUT)const;


   /*!
    * These methods are now deprecated.  You can achieve the same thing by
    * using the new visitor design pattern.  If this does not achieve exactly
    * what you want then you can derive new rules by overriding the virtual
    * visit method in ossimVisitor
    *
    * <pre>
    *    ossimIdVisitor visitor(id, false, ossimVisitor::VISIT_CHILDREN |
    *    ossimVisitor::VISIT_INPUTS);
    *    connectableObject->accept(visitor);
    *    ossimRefPtr<ossimConnectableObject> object = visitor.getObject();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject* findConnectableObject(
                             const ossimId& id));

   /*!
    * These methods are now deprecated.  You can achieve the same thing by
    * using the new visitor design pattern.  If this does not achieve exactly
    * what you want then you can derive new rules by overriding the virtual
    * visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeIdVisitor visitor(typeId, true, ossimVisitor::VISIT_CHILDREN |
    *    ossimVisitor::VISIT_INPUTS);
    *    connectableObject->accept(visitor);
    *    ossimRefPtr<ossimConnectableObject> object = visitor.getObject();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject* findObjectOfType(
                             RTTItypeid typeId,
                             ossimConnectableObjectDirectionType directionType,
                             bool recurse = true));

   /*!
    * These methods are now deprecated.  You can achieve the same thing by
    * using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding
    * the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeNameVisitor visitor(typeName, true,
    *    ossimVisitor::VISIT_CHILDREN | ossimVisitor::VISIT_INPUTS);
    *    connectableObject->accept(visitor);
    *    ossimRefPtr<ossimConnectableObject> object = visitor.getObject();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject* findObjectOfType(
                             const ossimString& obj,
                             ossimConnectableObjectDirectionType directionType,
                             bool recurse = true));
   
   /*!
    * These methods are now deprecated.  You can achieve the same thing by
    * using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding
    * the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeNameVisitor visitor(typeName, true,
    *    ossimVisitor::VISIT_CHILDREN | ossimVisitor::VISIT_INPUTS);
    *    connectableObject->accept(visitor);
    *    ossimRefPtr<ossimConnectableObject> object = visitor.getObject();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject* findInputObjectOfType(
                             const ossimString& className));
   
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
    * Should return the first available index to connect to.  The
    * connectMyInputTo that just takes another ossimConnectableObject as input
    * and not an index will call this method.  By default this method will find
    * the first open slot (not null) or append o the list if it's dynamic. If
    * the list is dynamic it calls conConnectMyInputTo(index, object) on an
    * index = to listSize.
    */
   virtual ossim_int32 getMyInputIndexToConnectTo(
      ossimConnectableObject* object)const;
   

   /**
    * Should return the first available index to connect to.  The
    * connectMyOutputTo that just takes another ossimConnectableObject as input\
    * and not an index will call this method.  By default this method will find
    * the first open slot (not null) or append to the list if it's dynamic. If
    * the list is dynamic it calls canConnectMyOutputTo(index, object) on an
    * index = to listSize.
    */
   virtual ossim_int32 getMyOutputIndexToConnectTo(
      ossimConnectableObject* object)const;
   
   /**
    * required to be overriden by derived classes
    */
   virtual bool canConnectMyInputTo(
      ossim_int32 myInputIndex, const ossimConnectableObject* object)const=0;
   
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
   virtual ossimRefPtr<ossimConnectableObject> disconnectMyInput(
      ossim_int32 inputIndex,
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
   virtual void disconnectMyInputs(
      ConnectableObjectList& inputList,
      bool disconnectOutputFlag=true,
      bool createEventFlag=true);
   
   /**
    * Will disconnect the object at the given output index and generate
    * a connection event.  If there is no object at that index then no
    * event is generated and NULL is returned.  The disconnectOutputFlag
    * says do you want this method to disconnect the output pointer to
    * this object.
    */
   virtual ossimRefPtr<ossimConnectableObject> disconnectMyOutput(
      ossim_int32 outputIndex,
      bool disconnectInputFlag=true,
      bool createEventFlag = true);
   
   /**
    * Will disconnect the output object.  It will get the index of
    * the object and call disconnectMyOutput(index, disconnectOutputFlag).
    */
   virtual void disconnectMyOutput(ossimConnectableObject* output,
                                   bool disconnectInputFlag=true,
                                   bool createEventFlag=true);

   virtual void disconnectMyOutputs(
      ConnectableObjectList& outputList,
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
   
   virtual bool connectMyInputTo(
      ConnectableObjectList& inputList,
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
   
   virtual bool connectMyOutputTo(
      ConnectableObjectList& outputList,
      bool makeInputConnection=true,
      bool createEventFlag=true);
   
   /**
    * Will disconnect itself from all inputs and reset to the passed in
    * input list.  It will return true if all objects were successfully
    * added.
    */
   virtual bool connectInputList(
      ConnectableObjectList& inputList);
   
   /**
    * Will disconnect itself from all outputs and reset to the passed in
    * output list.  It will return true if all objects were successfully
    * added.
    */
   virtual bool connectOutputList(
      ConnectableObjectList& outputList);
   
   /**
    * Returns the number of input objects.
    */
   virtual ossim_uint32 getNumberOfInputs()const;

   /**
    * Return the number of output objects
    */
   virtual ossim_uint32 getNumberOfOutputs()const;
   
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

   virtual bool getInputListIsFixedFlag()const;
   
   virtual bool getOutputListIsFixedFlag()const;

   /**
    * Will set the number of outputs.  This will expand the list if the
    * number of outputs is larger than the current number of outputs.
    * Will shrink the list if the current number of outputs is larger than
    * the passed in outputs.
    */
   virtual void setNumberOfOutputs(ossim_int32 numberOfInputs);
   
   const ConnectableObjectList& getInputList()const;

   const ConnectableObjectList& getOutputList()const;

   ConnectableObjectList& getInputList();

   ConnectableObjectList& getOutputList();

   /*!
    * These methods are now deprecated.  You can achieve the same thing by
    * using the new visitor design pattern.  If this does not achieve exactly
    * what you want then you can derive new rules by overriding the virtual
    * visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeNameVisitor visitor("<put type name here>",
    *    false, ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimCollectionVisitor::List& collection = visitor.getObjects();
    * </pre>
    */
    OSSIM_DEPRECATE_METHOD(virtual void findAllObjectsOfType(
                              ConnectableObjectList& result,
                              const RTTItypeid& typeInfo, bool recurse=true) );
    
   /*!
    * These methods are now deprecated.  You can achieve the same thing by
    * using the new visitor design pattern.  If this does not achieve exactly
    * what you want then you can derive new rules by overriding the virtual
    * visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeNameVisitor visitor("<put type name here>", false,
    *    ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimCollectionVisitor::List& collection = visitor.getObjects();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual void findAllObjectsOfType(
                             ConnectableObjectList& result,
                             const ossimString& className,
                             bool recurse=true));

#if 0 /* Deprecated code. Left here until thoroughly debugged. (drb) */
   /*!
    * These methods are now deprecated.  You can achieve the same thing by
    * using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding
    * the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeIdVisitor visitor(<put type id here>, false,
    *    ossimVisitor::VISIT_CHILDREN | ossimVisitor::VISIT_INPUTS);
    *    connectableObject->accept(visitor);
    *    ossimCollectionVisitor::List& collection = visitor.getObjects();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual void findAllInputsOfType(
      ConnectableObjectList& result,
      const RTTItypeid& typeInfo,
      bool propagateToInputs=true,
      bool recurseChildren=false));
   
   /*!
    * These methods are now deprecated.  You can achieve the same thing by
    * using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding
    * the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeNameVisitor visitor("<put type name here>",
    *    false, ossimVisitor::VISIT_CHILDREN | ossimVisitor::VISIT_INPUTS);
    *    connectableObject->accept(visitor);
    *    ossimCollectionVisitor::List& collection = visitor.getObjects();
    * </pre>
    */
   
   OSSIM_DEPRECATE_METHOD(virtual void findAllInputsOfType(
                             ConnectableObjectList& result,
                             const ossimString& className,
                             bool propagateToInputs=true,
                             bool recurseChildren=false));
#endif  /* deprecated code. (drb) */

   virtual void propagateEventToOutputs(ossimEvent& event);
   virtual void propagateEventToInputs(ossimEvent& event);

   /* ------------------- PROPERTY INTERFACE -------------------- */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual void setProperty(const ossimString& name, const ossimString& value);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /* ------------------ PROPERTY INTERFACE END ------------------- */

   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;

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
                                             const char* prefix=0) const;

   /**
    * Inserts this object and all of its children and inputs into the container
    * provided.
    * @return Returns TRUE if successful.
    */
   virtual bool fillContainer(ossimConnectableContainer& container);

   /**
    * Moves the input connection matching id up one in the connection list.
    * @param id The id to move.
    * @return true if action was performed, false if not.
    */
   bool moveInputUp(const ossimId& id);

   /**
    * Moves the input connection matching id down one in the connection list.
    * @param id The id to move.
    * @return true if action was performed, false if not.
    */
   bool moveInputDown(const ossimId& id);

   /**
    * Moves the input connection matching id to the top of the connection list.
    * @param id The id to move.
    * @return true if action was performed, false if not.
    */   
   bool moveInputToTop(const ossimId& id);

   /**
    * Moves the input connection matching id to the bottom of the connection
    * list.
    * @param id The id to move.
    * @return true if action was performed, false if not.
    */     
   bool moveInputToBottom(const ossimId& id);
   
   /**
    * We will add a visitor interface for all connectable objects.
    */
   virtual void accept(ossimVisitor& visitor);
   
protected:

   ossimConnectableObject* findObjectOfType(
      ConnectableObjectList* connectableList,
      ossimVisitor& visitor );
   
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
   ConnectableObjectList theInputObjectList;
   
   /**
    * Holds a list of output objects.
    */
   ConnectableObjectList theOutputObjectList;

private:
TYPE_DATA
};

#endif /* #ifndef ossimConnectableObject_HEADER */
