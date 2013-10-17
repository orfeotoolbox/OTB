//*************************************************************************************************
//                                            OSSIM
//
// License:  LGPL -- See LICENSE.txt file in the top level directory for more details.
//
// Author: Oscar Kramer
//
// This class supports multi-threading of image chain getTile() requests and associated chain
// updating. It accepts an original ossimImageChain object which is then cloned (replicated). 
// One replicant per thread is created (counting the original as replicant[0]), with all replicants
// sharing a common input handler (if directed) so that disk access is serialized and thread-safe.
//
//*************************************************************************************************
// $Id$

#include <ossim/parallel/ossimImageChainMtAdaptor.h>
#include <ossim/parallel/ossimMtDebug.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/parallel/ossimImageHandlerMtAdaptor.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <iterator>

static const char* NUM_THREADS_KW         = "ossimImageChainMtAdaptor.num_threads";
static const char* ORIGINAL_SOURCE_ID_KW  = "ossimImageChainMtAdaptor.original_source_id";

//*************************************************************************************************
// Constructor.
//*************************************************************************************************
ossimImageChainMtAdaptor::ossimImageChainMtAdaptor()
:  m_numThreads (0),
   d_useSharedHandlers(false),
   d_debugEnabled(false)
{
   //###### DEBUG ############
   ossimMtDebug* mt_debug = ossimMtDebug::instance();
   d_useSharedHandlers = mt_debug->chainSharedHandlers;
   d_debugEnabled = mt_debug->chainDebugEnabled;
   //###### END DEBUG ############
}

//*************************************************************************************************
// Constructor with original image chain provided. This source becomes the first clone in the list.
//*************************************************************************************************
ossimImageChainMtAdaptor::ossimImageChainMtAdaptor(ossimImageChain* original,
                                                   ossim_uint32 num_threads_req)
:  m_numThreads (0),
   d_useSharedHandlers(true),
   d_debugEnabled(false)
{
   //###### DEBUG ############
   ossimMtDebug* mt_debug = ossimMtDebug::instance();
   d_useSharedHandlers = mt_debug->chainSharedHandlers;
   d_debugEnabled = mt_debug->chainDebugEnabled;
   //###### END DEBUG ############

   setNumberOfThreads(num_threads_req); 
   setOriginalChain(original); 
}

//*************************************************************************************************
// Destructor.
//*************************************************************************************************
ossimImageChainMtAdaptor::~ossimImageChainMtAdaptor()
{
   removeListener((ossimConnectableObjectListener*)this);
   m_clones.clear();
   m_chainContainers.clear();
}

//*************************************************************************************************
//! Alternate way of specifying number of threads to support. This is the same as the number of 
//! clones that will be available after replicating the original chain.
//*************************************************************************************************
void ossimImageChainMtAdaptor::setNumberOfThreads(ossim_uint32 num_threads)
{
   if (m_numThreads == num_threads)
      return;

   // Determine number of cores/threads to set up:
   if (num_threads > 0)
   {
      m_numThreads = num_threads;
   }
   else
   {
      // Look in ossim preferences if arg is provided above.
      ossimString value = ossimPreferences::instance()->findPreference("ossim_threads");
      if ( !value.empty() )
         m_numThreads = value.toUInt32();
   }

   // If there is a valid original chain, we can perform the replication:
   if (!m_chainContainers.empty())
      replicate();
}

//*************************************************************************************************
//! Alternate way of specifying the original chain being adapted for multi-threading.
//*************************************************************************************************
void ossimImageChainMtAdaptor::setOriginalChain(ossimImageChain* original)
{
   if (original == NULL)
      return;

   ossimImageSource* first_source = original->getFirstSource();
   if (first_source == NULL)
      return;

   // Assign the adaptee and put it in the first position of the clones list:
   m_adaptedChain = original;
   m_clones.clear();
   m_clones.push_back(first_source);

   m_chainContainers.clear();
   m_chainContainers.push_back(new ossimConnectableContainer);
   m_adaptedChain->fillContainer(*m_chainContainers[0].get());

   // If we know the number of threads, we can begin replicating:
   if (m_numThreads > 1)
      replicate();
}

//*************************************************************************************************
// Creates clones of the original and pushes them onto the clone list.
//
// Need to consider the possibility that the original chain is a combiner with multiple image 
// handler inputs, and also the possibility that one handler is wired to multiple input chains
// before the combiner. In order to handle this, we will save the connection information for each 
// input handler encountered so that we can later reproduce it using a shared handler (if requested)
//*************************************************************************************************
bool ossimImageChainMtAdaptor::replicate()
{
   if ((m_clones.empty()) || (m_numThreads == 0))
      return false;

   // Don't need to replicate if only one thread is being requested. This is not an error though:
   if (m_numThreads == 1)
      return true;

   // If the handlers are to be shared, need to isolate them from the original chain and replace
   // them with a "hollow adaptor" (i.e., a handler adaptor without the adaptee set yet:
   m_sharedHandlers.clear();
   if (d_useSharedHandlers)
   {
      if (m_chainContainers.empty())
         return false;

      // Collect all image handlers in original chain for possible sharing with all clones:
      ossimTypeNameVisitor visitor (ossimString("ossimImageHandler"));
      m_chainContainers[0]->accept(visitor);
      ossimRefPtr<ossimImageHandler> handler = 0;
      ossimRefPtr<ossimImageHandlerMtAdaptor> handler_adaptor = 0;
      ossimRefPtr<ossimConnectableObject> output_connection = 0;
      ossim_uint32 handler_idx = 0;

      // Loop over all image handlers found in the original chain. Each handler's connection info
      // is stored and the handler is removed temporarily from the original chain.
      while (1) 
      {
         // Fetch a handler from the chain and wrap it with a handler adaptor:
         handler = visitor.getObjectAs<ossimImageHandler>(handler_idx++);
         if (handler == NULL)
            break; // Only exit point of while loop
         
         handler_adaptor = new ossimImageHandlerMtAdaptor(handler.get());
         m_sharedHandlers.push_back(handler_adaptor);

         // Change ownership:
         m_chainContainers[0]->removeChild(handler.get());
         handler->changeOwner(this);
      } 

      // If no handler was found, we can't continue.
      if (m_sharedHandlers.empty())
         return false;
   }

   // Fetch the state of this and the original chain. This KWL will be used for creating replicas
   // via the loadState. The saveState essentially bootstraps the replication task:
   ossimKeywordlist kwl;
   bool succeeded = saveState(kwl);

   // The original chain may have had the handlers temporarily removed for the saveState in support
   // of shared handlers. If so, need to restore them now:
   if (d_useSharedHandlers)
      succeeded = connectSharedHandlers(0);

   // Finally, initialize THIS chain with the original chain's state. This call will also create
   // the clones:
   if (succeeded)
      succeeded = loadState(kwl);

   return succeeded;
}

//*************************************************************************************************
// Deletes instances of all replicas from the clone list and leaves only the original.
//*************************************************************************************************
void ossimImageChainMtAdaptor::deleteReplicas()
{
   if (m_clones.size() > 1)
   {
      std::vector< ossimRefPtr<ossimImageSource> >::iterator first_copy = m_clones.begin();
      first_copy++;
      m_clones.erase(first_copy, m_clones.end());
   }
   m_numThreads = 1;
}

//*************************************************************************************************
// Saves the state of the original chain along with number of clones present.
//*************************************************************************************************
bool ossimImageChainMtAdaptor::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   if (m_chainContainers.empty())
      return false;

   kwl.add(prefix, NUM_THREADS_KW, m_numThreads);
   kwl.add(prefix, ORIGINAL_SOURCE_ID_KW, m_clones[0]->getId().getId());
   
   bool rtn_state = m_chainContainers[0]->saveState(kwl, prefix);

   if (d_debugEnabled)
      kwl.write("ossimImageChainMtAdaptor.kwl");

   return rtn_state;
}

//*************************************************************************************************
// Fetches the state of the original chain and regenerates the clones. Special handling is required
// when the image handlers are to be shared among all clones.
//*************************************************************************************************
bool ossimImageChainMtAdaptor::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool succeeded;

   // Reset this object:
   deleteReplicas();

   // Fetch this object's data members before moving onto original chain:
   ossimString value = kwl.find(prefix, NUM_THREADS_KW);
   if (value.empty())
      return false;
   m_numThreads = value.toUInt32();
   if (m_numThreads == 0)
      return false;

   // The chain ID needs to be read from KWL:
   ossimId orig_source_id (ossimId::INVALID_ID);
   value = kwl.find(prefix, ORIGINAL_SOURCE_ID_KW);
   if (value.empty())
      return false;
   orig_source_id.setId(value.toInt64());

   // This loadState may be called for the purpose of replicating the existing original, or it can
   // be intended as an adapter to a yet-to-be-instantiated original chain. Check if we already
   // have a valid original chain:
   ossimConnectableObject* candidate = 0;
   ossimImageSource* original_source = 0;
   if (!m_adaptedChain.valid() || m_chainContainers.empty())
   {
      m_chainContainers.clear();
      m_chainContainers.push_back(new ossimConnectableContainer);
      m_chainContainers[0]->loadState(kwl, prefix);

      // Need to instantiate a new original. This is a bootstrap for a full initialization of this
      // object. We'll need to replicate the clones afterwards:
      ossimIdVisitor visitor (orig_source_id);
      m_chainContainers[0]->accept(visitor);
      candidate = visitor.getObject();
      original_source = dynamic_cast<ossimImageSource*>(candidate);
      if (original_source == NULL)
         return false;
      m_clones.push_back(original_source); // original is always in first position of clones list

      // The original "chain" is morphed into a chain with a single child (original first source).
      // This source is the one maintaining the connection to the rest of the sources in the real
      // processing chain:
      m_adaptedChain = new ossimImageChain;
      m_adaptedChain->add(original_source);

      // Now that we have an original chain, Recursive code to replicate clones:
      succeeded = replicate();
      if (!succeeded)
         return false;
   }

   // We may be done:
   if (m_numThreads == 1)
      return true;

   // In preparation for multi-threading jobs, loop to instantiate all clone chains. The container
   // class is used to perform a deep copy of the original chain with all connections established.
   // It would have been cleaner to just use the ossimImageChain::dup() but that method was not
   // traversing the full chain, resulting in missing input sources:
   succeeded = true;
   for (ossim_uint32 i=1; (i<m_numThreads) && succeeded; ++i)
   {
      // Use original container's kwl to dup clone container, and pull out our chain of interest:
      m_chainContainers.push_back(new ossimConnectableContainer);
      m_chainContainers[i]->loadState(kwl, prefix);
      
      // Special handling required if the handlers are being shared. In this case, the handler had
      // been removed from the original chain, so connections need to be identified and made:
      if (d_useSharedHandlers)
      {
         succeeded = connectSharedHandlers(i);
         if (!succeeded)
            return false;
      }

      // Find the first (right-most) source in the chain and store it in the clone list. Need to
      // Modify all IDs
      ossimIdVisitor visitor (orig_source_id);
      m_chainContainers[i]->accept(visitor);
      candidate = visitor.getObject();
      m_chainContainers[i]->makeUniqueIds();
      ossimRefPtr<ossimImageSource> clone_source = dynamic_cast<ossimImageSource*>(candidate);
      if (clone_source == NULL)
         return false;
      m_clones.push_back(clone_source);
   }

   return succeeded;
}

//*************************************************************************************************
//! Adapts call to original chain so that all clones are initialized.
//*************************************************************************************************
void ossimImageChainMtAdaptor::initialize()
{
   for (size_t i=0; i<m_clones.size(); ++i)
      m_clones[i]->initialize();
}

//*************************************************************************************************
// Intercept this getTile because it should never be called directly. The tile request must go
// to the specific chain clone.
//*************************************************************************************************
ossimRefPtr<ossimImageData> ossimImageChainMtAdaptor::getTile(const ossimIrect& tileRect,
                                                                       ossim_uint32 resLevel)
{
   ossimNotify(ossimNotifyLevel_WARN)<<"ossimImageChainMtAdaptor::getTile() -- This method "
      "Should never be called directly. The tile request must go to the specific chain clone. "
      "Returning a tile using the original chain's getTile (not threaded)..."<<endl;
   
   if (!m_adaptedChain.valid())
      return ossimRefPtr<ossimImageData>(0);
      
   return m_adaptedChain->getTile(tileRect, resLevel);
}

//*************************************************************************************************
// Manages reconnecting shared image handlers to an image chain after its creation.
// This is in support of shared image handlers. Returns TRUE if successful.
//*************************************************************************************************
bool ossimImageChainMtAdaptor::connectSharedHandlers(ossim_uint32 chain_index)
{
   if ((size_t)chain_index >= m_chainContainers.size())
      return false;

   // Loop over each adapted handler in our shared handler list:
   SharedHandlerList::iterator handler = m_sharedHandlers.begin();
   while (handler != m_sharedHandlers.end())
   {
      // Fetch all objects connected to this adapted handler. The list will point to objects in
      // m_chainContainers[0], the original chain. The new chain's objects share the same ID's as
      // the original chain for the moment. So we can search for the output connection 
      // in the new chain using the ID of the corresponding object in the original chain:
      ConnectableObjectList handler_connections = (*handler)->getOutputList();
      ConnectableObjectList::iterator output_connection = handler_connections.begin();
      while (output_connection != handler_connections.end())
      {
         ossimId obj_id = (*output_connection)->getId();
         ossimIdVisitor visitor (obj_id);
         m_chainContainers[chain_index]->accept(visitor);

         // Get the pointer to the actual output object that needs to be connected to the shared
         // handler:
         ossimConnectableObject* output_obj = visitor.getObject();
         if (output_obj == NULL)
            return false; // Should never happen
         output_obj->connectMyInputTo((*handler).get(), true, true);

         output_connection++;
      }
      handler++;
   }
   return true;
}


//*************************************************************************************************
// Adapts base class method for accessing connectables in the original chain.
//*************************************************************************************************
ossimConnectableObject::ConnectableObjectList& ossimImageChainMtAdaptor::imageChainList()
{
   // If there is no original chain defined, then just return our own blank list:
   if (!m_adaptedChain.valid())
      return theImageChainList;

   return m_adaptedChain->imageChainList();
}

//*************************************************************************************************
// Adapts base class method for accessing connectables in the original chain.
//*************************************************************************************************
const ossimConnectableObject::ConnectableObjectList& 
   ossimImageChainMtAdaptor::imageChainList()const
{
   // If there is no original chain defined, then just return our own blank list:
   if (!m_adaptedChain.valid())
      return theImageChainList;

   return m_adaptedChain->imageChainList();
}

//*************************************************************************************************
//! Adapts the image chain event handler. If the event involves a change to the original image 
//! chain, then the clones will need to be regenerated here.
//*************************************************************************************************
void ossimImageChainMtAdaptor::processEvent(ossimEvent& /* event */)
{
   ossimNotify(ossimNotifyLevel_WARN)<<"ossimImageChainMtAdaptor::processEvent() -- "
      "NOT YET IMPLEMENTED"<<endl;
}

//*************************************************************************************************
ossimImageSource* ossimImageChainMtAdaptor::getClone(ossim_uint32 index)
{
   if (index < (ossim_uint32) m_clones.size())
      return m_clones[index].get();
   
   return 0;
}

//*************************************************************************************************
//! Overrides base class implementation in order to make the connection to each clone. This 
//! connection is typically to the multi-thread sequencer object.
//*************************************************************************************************
ossim_int32 ossimImageChainMtAdaptor::connectMyOutputTo(ossimConnectableObject* outputObject,
                                                        bool makeInputConnection,
                                                        bool createEventFlag)
{
   // Make output connection for each clone source:
   std::vector< ossimRefPtr<ossimImageSource> >::iterator clone_source = m_clones.begin();
   while (clone_source != m_clones.end())
   {
      (*clone_source)->connectMyOutputTo(outputObject, false, false);
      clone_source++;
   }

   // Now make master connection including making input connection on outputObject and firing event:
   return ossimConnectableObject::connectMyOutputTo(outputObject, 
                                                    makeInputConnection, 
                                                    createEventFlag);
}
