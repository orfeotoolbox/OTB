//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimConnectableContainer.h 20316 2011-12-02 15:56:38Z oscarkramer $

#ifndef ossimConnectableContainer_HEADER
#define ossimConnectableContainer_HEADER
#include <map>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimConnectableContainerInterface.h>

class OSSIMDLLEXPORT ossimConnectableContainerChildListener;

class OSSIMDLLEXPORT ossimConnectableContainer : public ossimConnectableObject,
                                  public ossimConnectableContainerInterface,
                                  public ossimConnectableObjectListener
{
public:
   typedef std::map<ossim_int64,
      ossimRefPtr<ossimConnectableObject> > connectablObjectMapType;
   
   ossimConnectableContainer(ossimConnectableObject* owner=0);

   virtual ossimObject* getObject();
   
   virtual const ossimObject* getObject()const;

   /**
    * @param index Index of object to get.
    *
    * @see getNumberOfObjects(false) to get the number of objects inside
    * the container.
    *
    * @return Container's object at that index or NULL if out of range.
    *
    * @note This does not recurse into other containers.
    */
   virtual ossimConnectableObject* getConnectableObject(ossim_uint32 index);

   
   /*!
    * These methods are now deprecated.  You can achieve the same thing by using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeNameVisitor visitor("<put type name here>", false, ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimCollectionVisitor::List& collection = visitor.getObjects();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject::ConnectableObjectList findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                            bool recurse=true));
   
   /*!
    * These methods are now deprecated.  You can achieve the same thing by using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimConnectableTypeIdVisitor visitor(typeId, false, ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimConnectableCollectionVisitor::List& collection = visitor.getObjects();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject::ConnectableObjectList findAllObjectsOfType(const ossimString& className,
                                                            bool recurse=true));

   /*!
    * These methods are now deprecated.  You can achieve the same thing by using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeIdVisitor visitor(typeId, true, ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimRefPtr<ossimConnectableObject> obj = visitor.getObject();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject* findFirstObjectOfType(const RTTItypeid& typeInfo,
                                                 bool recurse=true));
   
   /*!
    * These methods are now deprecated.  You can achieve the same thing by using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeNameVisitor visitor(className, true, ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimRefPtr<ossimConnectableObject> obj = visitor.getObject();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject* findFirstObjectOfType(const ossimString& className,
                                                bool recurse=true));

   /*!
    * These methods are now deprecated.  You can achieve the same thing by using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimIdVisitor visitor(id, ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimRefPtr<ossimConnectableObject> obj = visitor.object();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(ossimConnectableObject* findObject(const ossimId& id,
                                     bool recurse=true));

   /**
    * Please use the new ossimVisitor design pattern to visit objects in the container.
    */
   OSSIM_DEPRECATE_METHOD(ossimConnectableObject* findObject(const ossimConnectableObject* obj,
                                     bool recurse=true));

   /*!
    * Will cycle through all sources setting their ids. the idLast wlil be
    * updated
    * so we can recurse into other containers.
    */
   void makeUniqueIds();

   /*!
    * Returns the number of objects within this container and all child
    * containers.
    */
   ossim_uint32 getNumberOfObjects(bool recurse=true)const;
   
   /*!
    * Will add an object to the container and then set the added objects owner
    * to this.
    */
   virtual bool addChild(ossimConnectableObject* attachableObject);


   virtual bool removeChild(ossimConnectableObject* object);
   
   virtual bool canConnectMyInputTo(ossim_int32 index,
                                    const ossimConnectableObject* obj) const;

   virtual bool canConnectMyOutputTo(ossim_int32 index,
                                     const ossimConnectableObject* obj) const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   //____________________PLACE ALL EVENT HANDLING STUFF HERE_____________
//   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);
//   virtual void propertyEvent(ossimPropertyEvent& event);

//   virtual void disconnectInputEvent(ossimConnectionEvent& event);
//   virtual void connectInputEvent(ossimConnectionEvent& event);

//   virtual void disconnectOutputEvent(ossimConnectionEvent& event);
//   virtual void connectOutputEvent(ossimConnectionEvent& event);
   
   
   virtual void getChildren(std::vector<ossimConnectableObject*>& children,
                            bool immediateChildrenOnlyFlag);
   void deleteAllChildren();
   
   virtual void accept(ossimVisitor& visitor);
//    void propagateEventToOutputs(ossimEvent& event,
//                                 ossimConnectableObject* start);
//    void propagateEventToInputs(ossimEvent& event,
//                                ossimConnectableObject* start);
     
   /*!
    *  Inserts all of this object's children and inputs into the container provided. Since this is
    *  itself a container, this method will consolidate this with the argument container. 
    *  Consequently, this object will not be represented in the argument container but its children
    *  will be. Ownership of children will pass to the argument container.
    *  @return Returns TRUE if successful.
    */
   virtual bool fillContainer(ossimConnectableContainer& container);

protected:
   virtual ~ossimConnectableContainer();
   ossimConnectableContainer(const ossimConnectableContainer& rhs);

   void removeAllListeners();
   bool addAllObjects(std::map<ossimId, std::vector<ossimId> >& idMapping,
                      const ossimKeywordlist& kwl,
                      const char* prefix);
      
   bool connectAllObjects(const std::map<ossimId, std::vector<ossimId> >& idMapping);
   
   void findInputConnectionIds(std::vector<ossimId>& result,
                               const ossimKeywordlist& kwl,
                               const char* prefix);
   
   /*!
    * Every object added must have a unique id.  We will sort them in a easy to
    * query form.  Since the container can have hundreds of objects we will use a
    * more efficient map that allows us to do binary searches of the objects.
    *
    * map<key, value>  The key will be the objectsUnique id and the value
    * is a pointer to the attachable object.
    */
   connectablObjectMapType theObjectMap;

   ossimConnectableContainerChildListener* theChildListener;
TYPE_DATA
};

#endif
