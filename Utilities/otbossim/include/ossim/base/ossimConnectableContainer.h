//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimConnectableContainer.h 13017 2008-06-10 16:09:17Z dburken $

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
      ossimConnectableObject*> connectablObjectMapType;
   
   ossimConnectableContainer(ossimConnectableObject* owner=0);
   virtual ~ossimConnectableContainer();

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
    * Will find all objects of the past in type.  Use the RTTI type info.
    * An optional recurse flag will say if there is another container then
    * recurse it to find the type you are looking for else it just looks
    * within its immediate children.
    * 
    * Example: passing STATIC_TYPE_INFO(ossimImageRenderer) as an argument will
    *          look for all ossimImageRenderer's and return the list.
    */
   virtual std::vector<ossimConnectableObject*> findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                            bool recurse=true);
   virtual std::vector<ossimConnectableObject*> findAllObjectsOfType(const ossimString& className,
                                                            bool recurse=true);

   /*!
    * Will find the firt object of the past in type.  Use the RTTI type info.
    * An optional recurse flag will say if there is another container then
    * recurse it to find the type you are looking for else it just looks
    * within its immediate children.
    * 
    * Example: passing STATIC_TYPE_INFO(ossimImageRenderer) as an argument will
    *          look for the first ossimImageRenderer and return that object.
    */
   virtual ossimConnectableObject* findFirstObjectOfType(const RTTItypeid& typeInfo,
                                                 bool recurse=true);
   
   virtual ossimConnectableObject* findFirstObjectOfType(const ossimString& className,
                                                bool recurse=true);

   /*!
    * will search for the object given an id.  If recurse is true it will
    * recurse
    * to other containers.
    */ 
   ossimConnectableObject* findObject(const ossimId& id,
                                     bool recurse=true);

   ossimConnectableObject* findObject(const ossimConnectableObject* obj,
                                     bool recurse=true);

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
   
//    void propagateEventToOutputs(ossimEvent& event,
//                                 ossimConnectableObject* start);
//    void propagateEventToInputs(ossimEvent& event,
//                                ossimConnectableObject* start);
     
protected:
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
