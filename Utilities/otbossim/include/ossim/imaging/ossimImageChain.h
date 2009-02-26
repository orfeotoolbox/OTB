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
// $Id: ossimImageChain.h 13475 2008-08-22 14:21:54Z gpotts $
#ifndef ossimImageChain_HEADER
#define ossimImageChain_HEADER
#include <vector>
#include <map>
using namespace std;

#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimId.h>
#include <ossim/base/ossimConnectableContainerInterface.h>

class ossimImageChainChildListener;

class OSSIMDLLEXPORT ossimImageChain : public ossimImageSource,
                                       public ossimConnectableObjectListener,
                                       public ossimConnectableContainerInterface
{
public:
   ossimImageChain();
   virtual ~ossimImageChain();

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
   virtual ossimObject* getFirstObject();

   /**
    * Return the last source which is the one that last receives the
    * getTile request.
    */
   virtual ossimImageSource* getLastSource();
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
   
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=NULL);

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
   vector<ossimConnectableObject*>& getChildren();
   
   bool canConnectMyInputTo(ossim_int32 myInputIndex,
                            const ossimConnectableObject* object)const
      {
         if(theImageChainList.size()&&
            theImageChainList[theImageChainList.size()-1])
         {
            ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject, theImageChainList[theImageChainList.size()-1]);
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
   
   /**
    * Will find all objects of the past in type.  Use the RTTI type info.  An optional
    * recurse flag will say if there is another container then recurse it to
    * find the type you are looking for else it just looks within its immediate
    * children
    * 
    * Example: passing STATIC_TYPE_INFO(ossimImageRenderer) as an argument will
    *          look for all ossimImageRenderer's and return the list.
    */
   virtual std::vector<ossimConnectableObject*> findAllObjectsOfType(const RTTItypeid& typeInfo,
                                                                     bool recurse=true);
   virtual std::vector<ossimConnectableObject*> findAllObjectsOfType(const ossimString& className,
                                                                     bool recurse=true);

   /**
    * Will find the firt object of the past in type.  Use the RTTI type info.An optional
    * recurse flag will say if there is another container then recurse it to
    * find the type you are looking for else it just looks within its immediate
    * children
    * 
    * Example: passing STATIC_TYPE_INFO(ossimImageRenderer) as an argument will
    *          look for the first ossimImageRenderer and return that object.
    */
   virtual ossimConnectableObject* findFirstObjectOfType(const RTTItypeid& typeInfo,
                                                         bool recurse=true);
   virtual ossimConnectableObject* findFirstObjectOfType(const ossimString& className,
                                                         bool recurse=true);

   /**
    * will search for the object given an id.  If recurse is true it will
    * recurse to other containers.
    */ 
   virtual ossimConnectableObject* findObject(const ossimId& id,
                                              bool recurse=true);

   virtual ossimConnectableObject* findObject(const ossimConnectableObject* obj,
                                              bool recurse=true);

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
   
protected:
   friend class ossimImageChainChildListener;
   /**
    * This will hold a sequence of image sources.
    * theFirst one in the list will be the head of the
    * list and the last one is the tail.
    */
   vector<ossimConnectableObject*> theImageChainList;
   
   ossimRefPtr<ossimImageData>     theBlankTile;
   ossimImageChainChildListener*   theChildListener;
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
   
   void deleteList();
   
TYPE_DATA
};
#endif /* #ifndef ossimImageChain_HEADER */
