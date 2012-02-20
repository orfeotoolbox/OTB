//*************************************************************************************************
//                                            OSSIM
//
// License:  LGPL -- See LICENSE.txt file in the top level directory for more details.
//
// Author: Oscar Kramer
//
//*************************************************************************************************
// $Id$
#ifndef ossimImageChainMtAdaptor_HEADER
#define ossimImageChainMtAdaptor_HEADER

#include <vector>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/parallel/ossimImageHandlerMtAdaptor.h>
#include <ossim/base/ossimId.h>
#include <ossim/base/ossimConnectableContainer.h>

//*************************************************************************************************
//! This class supports multi-threading of image chain getTile() requests and associated chain
//! updating. It accepts an original ossimImageChain object which is then cloned (replicated). 
//! One clone per thread is created, counting the original as clone(0), with all clones
//! sharing a common input handler (if directed) so that disk access is serialized and thread-safe.
//*************************************************************************************************
class OSSIMDLLEXPORT ossimImageChainMtAdaptor : public ossimImageChain
{
   friend class ossimMultiThreadSequencer; //### FOR DEBUG ONLY

public:
   ossimImageChainMtAdaptor();

   //! Full constructor accepts the original image chain to clone. 
   //! @param num_threads  Total number of cloned chains to make available.
   //! 
   ossimImageChainMtAdaptor(ossimImageChain* original, ossim_uint32 num_threads=0);
   ~ossimImageChainMtAdaptor();

   //! Alternate way of specifying number of threads to support. This is the same as the number of 
   //! clones that will be available after replicating the original chain. 
   //! @param num_threads  Total number of cloned chains to make available.
   void setNumberOfThreads(ossim_uint32 num_threads);

   //! Alternate way of specifying the original chain being adapted for multi-threading.
   //! @param original  Image chain to be adapted.
   void setOriginalChain(ossimImageChain* original);

   //! Intercepts the getTile because it should never be called directly. The tile request must go
   //! to the specific chain clone.
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   //! Adapts call to original chain so that all clones are initialized.
   virtual void initialize();

   //! Adapts call to original chain so that clones are replicated after original's loadState.
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=NULL);

   //! Adapts call to original chain so that adapter-specific keywords can be added to KWL.
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=NULL)const;

   //! Adapts the image chain event handler. If the event involves a change to the original image 
   //! chain, then the clones will need to be regenerated here.
   virtual void processEvent(ossimEvent& event);

   //! Returns pointer to a specific clone image chain, or NULL if index exceeds the max available.
   //! The clone at index=0 corresponds to the original, though this detail should be ignored.
   ossimImageSource* getClone(ossim_uint32 index);

   //! Returns the number of clones available. The original chain always counts as the first clone.
   ossim_uint32 getNumberOfClones() const { return (ossim_uint32) m_clones.size(); }

   //! These overriding access methods greatly facilitate the implementation of an image chain
   //! adapter class operating on the member original chain. These methods really should be 
   //! "protected" but the compiler complains if the base class declarations are "protected".
   virtual ossimConnectableObject::ConnectableObjectList& imageChainList();
   virtual const ossimConnectableObject::ConnectableObjectList& imageChainList() const;

   //! Overrides base class implementation in order to make the connection to each clone. This 
   //! connection is typically to the multi-thread sequencer object.
   virtual ossim_int32 connectMyOutputTo(ossimConnectableObject* outputObject,
                                         bool makeInputConnection=true,
                                         bool createEventFlag=true);

protected:
   //! Deletes instances of all replicas from the clone list and leaves only the original.
   void deleteReplicas();

   //! Creates clones of original and pushes them onto the clone list. Returns TRUE if successful.
   bool replicate();

   //! Manages reconnecting shared image handlers to a cloned image chain (identified by index)
   //! after its creation. This is in support of shared image handlers. Returns TRUE if successful.
   bool connectSharedHandlers(ossim_uint32 index);

   //! This is the adaptee image chain.
   ossimRefPtr<ossimImageChain> m_adaptedChain;

   //! List of replicated child chains' first source (the one receiving the getTile call). The
   //! first in the list is always the original (adaptee) source. These are owned by the 
   //! m_chainContainers that are maintained here.
   std::vector< ossimRefPtr<ossimImageSource> > m_clones;
   
   //! The container objects must be kept alive in this vector while their child chains in
   //! m_clones are used. Containers are used because they are the only way of "duping" a complete 
   //! chain with all connections properly done. The containers need to persist in order for the 
   //! chain connections to be maintained (apparent bug in connectable object code). I found it 
   //! impossible to dissociate the chain from its container. As soon as the container disappeared,
   //! the chain's internal connections would be disrupted. (OLK 11/11)
   std::vector< ossimRefPtr<ossimConnectableContainer> > m_chainContainers;

   //! List of handlers that are being shared among all clones (if requested). It is a list since 
   //! the original chain may involve a combiner with multiple image inputs.
   typedef std::vector<ossimRefPtr<ossimImageHandlerMtAdaptor> > SharedHandlerList;
   SharedHandlerList m_sharedHandlers;

   ossim_uint32 m_numThreads;

   bool d_useSharedHandlers;
   bool d_debugEnabled;

};
#endif /* #ifndef ossimImageChain_HEADER */
