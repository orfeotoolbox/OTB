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
// $Id: ossimImageChain.h 20316 2011-12-02 15:56:38Z oscarkramer $
#ifndef ossimImageChain_HEADER
#define ossimImageChain_HEADER
#include <vector>
#include <map>
using namespace std;

#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimId.h>
#include <ossim/base/ossimConnectableContainerInterface.h>

class OSSIMDLLEXPORT ossimImageChain : public ossimImageSource,
                                       public ossimConnectableObjectListener,
                                       public ossimConnectableContainerInterface
{
public:
   ossimImageChain();
   virtual ~ossimImageChain();

   ossimConnectableObject::ConnectableObjectList& getChainList(){return theImageChainList;}
   const ossimConnectableObject::ConnectableObjectList& getChainList()const{return theImageChainList;}
   
   /**
    * @param index Index of object to get.
    *
    * @see getNumberOfObjects(false) to get the number of objects inside
    * the container.
    *
    * @return Container's object at that index or NULL if out of range.
    */
   ossimConnectableObject* operator[](ossim_uint32 index);

   /**
    * Same as operator[](ossim_uint32 i)
    * 
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
   
   virtual ossim_int32 indexOf(ossimConnectableObject* obj)const;
   /**
    * Return the first source which is the one that first receives the
    * getTile request
    */
   virtual ossimImageSource* getFirstSource();
   virtual const ossimImageSource* getFirstSource() const;
   
   virtual ossimObject* getFirstObject();

   /**
    * Return the last source which is the one that last receives the
    * getTile request.
    */
   virtual ossimImageSource* getLastSource();
   virtual const ossimImageSource* getLastSource() const;
   
   virtual ossimObject* getLastObject();


   /**
    * Adds it to the start of the chain.  This is the first one to receive
    * the getTile request.  This basically inserts it to the right of the
    * right most filter.
    * Returns true on success, false on error.
    */
   bool addFirst(ossimConnectableObject* obj);

   /**
    * Adds it to the end.  this is the last one to receive the getTile
    * request.  This basically inserts it to the left of the left most filter.
    * Returns true on success, false on error.
    */
   bool addLast(ossimConnectableObject* obj);
   
   /**
    * Deletes the first object.
    * Note this is really most right of the chain.
    * Returns true if deletion occurred, false if the chain is empty.
    */
   bool deleteFirst();

   /**
    * Deletes the last object.
    * Note this is really most left of the chain.
    * Returns true if deletion occurred, false if the chain is empty.
    */
   bool deleteLast();

   /**
    * 
    */
   bool insertRight(ossimConnectableObject* newObj,
                    ossimConnectableObject* rightOfThisObj);

   /**
    *
    */
   bool insertRight(ossimConnectableObject* newObj,
                    const ossimId& id);

   /**
    *
    */
   bool insertLeft(ossimConnectableObject* newObj,
                   const ossimId& id);
   /**
    *
    */
   bool insertLeft(ossimConnectableObject* newObj,
                   ossimConnectableObject* leftOfThisObj);
   
   bool replace(ossimConnectableObject* newObj,
                ossimConnectableObject* oldObj);
   /**
    * Will return true or false if an image source was
    * added to the chain.  It will add and do a connection
    * to the previous source automatically.  This is a
    * chain and the order of insertion matters. It adds it to the beginning
    * of the list.  The last one added is the first one to receive
    * the getTile.  Basically inserts to the right of the rightmost filter
    *
    */
   virtual bool add(ossimConnectableObject* source);

   /**
    * Within the image chain will pass the head of the
    * list.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   /**
    * this call is passed to the head of the list.
    */
   virtual ossim_uint32 getNumberOfInputBands() const;
   
   /**
    * This call is passed to the head of the list.
    */
   virtual ossimScalarType getOutputScalarType() const;

   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const;

   /**
    * This call is passed to the head of the list.
    */
   virtual ossim_uint32 getTileWidth()const;

   /**
    * This call is passed to the head of the list.
    */
   virtual ossim_uint32 getTileHeight()const;
   
   /**
    * Will pass this call to the head of the list.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;
   virtual void getValidImageVertices(vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const;
   
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result) const;
   virtual void getDecimationFactors(vector<ossimDpt>& decimations) const;
   virtual ossim_uint32 getNumberOfDecimationLevels()const;
   
   /**
    * Will save the state of the chain.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;

   /**
    * Will load the state.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);
   
   virtual void initialize();
   virtual void enableSource();
   virtual void disableSource();
   
   bool canConnectMyInputTo(ossim_int32 myInputIndex,
                            const ossimConnectableObject* object)const
      {
         if(theImageChainList.size()&&
            theImageChainList[theImageChainList.size()-1].valid())
         {
            ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject, theImageChainList[theImageChainList.size()-1].get());
            if(obj)
            {
               return obj->canConnectMyInputTo(myInputIndex,
                                               object);
            }
         }
         else if(!theImageChainList.size())
         {
            return true;
         }
         return false;
      }
      
//    bool canConnectMyOutputTo(int32 myOutputIndex,
//                              const ossimConnectableObject* object)const
//       {
//          if(theImageChainList.size()&&theImageChainList[0])
//          {
// 	   ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject, theImageChainList[0]);
// 	   if(obj)
// 	     {
// 	       return obj->canConnectMyOutputTo(myOutputIndex,
// 						object);
// 	     }
//          }
         
//          return false;
//       }
   
   // ________________________CONNECTABLE CONTAINER DEFINTIONS_________________
   //
   
   /*!
    * These methods are now deprecated.  You can achieve the same thing by using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeIdVisitor visitor(tyeId, false, ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimCollectionVisitor::List& collection = visitor.getObjects();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD( virtual ossimConnectableObject::ConnectableObjectList findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                                     bool recurse=true));
   /*!
    * These methods are now deprecated.  You can achieve the same thing by using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeNameVisitor visitor(typeName, false, ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimCollectionVisitor::List& collection = visitor.getObjects();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject::ConnectableObjectList findAllObjectsOfType(const ossimString& className,
                                                                     bool recurse=true));

   /*!
    * These methods are now deprecated.  You can achieve the same thing by using the new visitor design pattern.  If this does not 
    * achieve exactly what you want then you can derive new rules by overriding the virtual visit method in ossimVisitor
    *
    * <pre>
    *    ossimTypeIdVisitor visitor(tyeId, true, ossimVisitor::VISIT_CHILDREN);
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
    *    ossimIdVisitor visitor(tyeId, true, ossimVisitor::VISIT_CHILDREN);
    *    connectableObject->accept(visitor);
    *    ossimRefPtr<ossimConnectableObject> obj = visitor.getObject();
    * </pre>
    */
   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject* findObject(const ossimId& id,
                                              bool recurse=true));

   OSSIM_DEPRECATE_METHOD(virtual ossimConnectableObject* findObject(const ossimConnectableObject* obj,
                                              bool recurse=true));

   /**
    * Will cycle through all sources setting their ids. the idLast wlil
    * be updated
    * so we can recurse into other containers.
    */
   virtual void makeUniqueIds();

   /**
    * Returns the number of objects within this container and all child
    * containers.
    *
    * @param recurse If true and object is a contains containers; all objects
    * will be conter.  If false only the objects of this container will be
    * counted (theImageChainList.size()).
    */
   virtual ossim_uint32 getNumberOfObjects(bool recurse=true)const;

   /**
    * Deprecated! Please use getNumberOfObjects(false).
    *
    * @return The number of objects in this container or
    * (theImageChainList.size()).
    */
   virtual ossim_uint32 getNumberOfSources() const;
   
   /**
    * Will add an object to the container and then set the added objects owner
    * to this.
    */
   virtual bool addChild(ossimConnectableObject* attachableObject);

   /**
    * Will remove the child from the container.  Changes the owner of the
    * child to be NULL;
    */
   virtual bool removeChild(ossimConnectableObject* object);
   virtual ossimConnectableObject* removeChild(const ossimId& id);
   
   virtual void getChildren(vector<ossimConnectableObject*>& children,
                            bool immediateChildrenOnlyFlag);
   //______________END CONNECTABLE CONTAINER INTERFACE____________
   
   virtual void disconnectInputEvent(ossimConnectionEvent& event);
   virtual void disconnectOutputEvent(ossimConnectionEvent& event);
   virtual void connectInputEvent(ossimConnectionEvent& event);
   virtual void connectOutputEvent(ossimConnectionEvent& event);
   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);

   virtual void propagateEventToOutputs(ossimEvent& event);
   virtual void propagateEventToInputs(ossimEvent& event);
 
   virtual void processEvent(ossimEvent& event);

   virtual void accept(ossimVisitor& visitor);
   void deleteList();

   /**
    * These access methods greatly facilitate the implementation of an image chain adaptor class.
    * They shouldn't be accessed directly by any other classes.  These methods really should be 
    * "protected" but the compiler complains if this base class' declarations are "protected".
    */
   virtual ossimConnectableObject::ConnectableObjectList& imageChainList() { return theImageChainList; }
   virtual const ossimConnectableObject::ConnectableObjectList& imageChainList() const { return theImageChainList; }
   
   /**
    * Inserts all of this object's children and inputs into the container provided. Since this is
    * itself a form of container, this method will consolidate this chain with the argument
    * container. Consequently, this chain object will not be represented in the container, but its
    * children will be, with correct input and output connections to external objects. Ownership
    * of children will pass to the argument container.
    * @return Returns TRUE if successful.
    */
   virtual bool fillContainer(ossimConnectableContainer& container);

protected:
   void prepareForRemoval(ossimConnectableObject* connectableObject);
   
  /**
    * This will hold a sequence of image sources.
    * theFirst one in the list will be the head of the
    * list and the last one is the tail.
    */
   ossimConnectableObject::ConnectableObjectList theImageChainList;
   
   ossimRefPtr<ossimImageData>     theBlankTile;
  // mutable bool                    thePropagateEventFlag;
   mutable bool                    theLoadStateFlag;
   /**
    * For dynamic loading to take place we must allocate all objects first and
    * then assign id's later.  We must remember the id's so we can do this.
    * we will create a map that takes the id of the source as a key and a
    * vector of input id's to connect it's inputs to.
    */
   bool addAllSources(map<ossimId, vector<ossimId> >& idMapping,
                      const ossimKeywordlist& kwl,
                      const char* prefix = NULL);
   void findInputConnectionIds(vector<ossimId>& result,
                               const ossimKeywordlist& kwl,
                               const char* prefix=NULL);
   bool connectAllSources(const map<ossimId, vector<ossimId> >& idMapping);
   
   
TYPE_DATA
};
#endif /* #ifndef ossimImageChain_HEADER */
