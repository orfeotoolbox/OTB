//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
//! Intended mainly to provide a mechanism for mutex-locking access to a shared resource during
//! a getTile operation on an ossimImageHandler. This is needed for multi-threaded implementation.
// 
//**************************************************************************************************
//  $Id$
#include <ossim/parallel/ossimImageHandlerMtAdaptor.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/parallel/ossimMtDebug.h>
#include <ossim/base/ossimTimer.h>

RTTI_DEF1(ossimImageHandlerMtAdaptor, "ossimImageHandlerMtAdaptor", ossimImageHandler);

const char* ossimImageHandlerMtAdaptor::ADAPTEE_ID_KW = "adaptee_id";

//**************************************************************************************************
// Constructor
//**************************************************************************************************
ossimImageHandlerMtAdaptor::ossimImageHandlerMtAdaptor(ossimImageHandler* adaptee)
   :  d_getTileT (0),
      m_adaptedHandler (0),
      m_cache (0),
      d_useCache (false),
      d_useFauxTile (false)
{
   //###### DEBUG ############
   ossimMtDebug* mt_debug = ossimMtDebug::instance();
   d_useCache = mt_debug->handlerCacheEnabled;
   d_useFauxTile = mt_debug->handlerUseFauxTile;
   //###### END DEBUG ############

   setAdaptee(adaptee);
}

//**************************************************************************************************
// Destructor
//**************************************************************************************************
ossimImageHandlerMtAdaptor::~ossimImageHandlerMtAdaptor()
{
   m_adaptedHandler = 0;
   m_cache = 0;
}

//**************************************************************************************************
//! Sets the handler being adapted.
//**************************************************************************************************
void ossimImageHandlerMtAdaptor::setAdaptee(ossimImageHandler* handler)
{
   m_adaptedHandler = handler;
   if (handler == NULL)
      return;

   // Fetch the adaptee's output list and make it our own:
   ConnectableObjectList output_list = handler->getOutputList();

   if (d_useCache)
   {
      // Create the cache and connect this adaptor as its output:
      m_cache = new ossimCacheTileSource;
      m_cache->connectMyOutputTo(this, true, false);
      m_cache->changeOwner(this);
      m_cache->connectMyOutputTo(this, true, false);
      handler->disconnectMyOutputs(output_list, true, false);
      handler->connectMyOutputTo(m_cache.get(), true, true);
   }
   else
   {
      handler->disconnectMyOutputs(output_list, true, false);
      handler->connectMyOutputTo(this, true, false);
   }

   // Finally connect the adaptee's outputs to this and fire connection events:
   connectMyOutputTo(output_list, true, true);
   handler->changeOwner(this);

   if (d_useFauxTile)
   {
      d_fauxTile = (ossimImageData*) handler->getTile(ossimIpt(0,0), 0)->dup();
      //d_fauxTile = new ossimImageData(this, 
      //                                handler->getOutputScalarType(), 
      //                                handler->getNumberOfOutputBands(),
      //                                handler->getTileWidth(),
      //                                handler->getTileHeight());
      //d_fauxTile->fill(128.0);
   }
}

//**************************************************************************************************
//! Only an ossimImageHandler is allowed as input here.
//**************************************************************************************************
bool ossimImageHandlerMtAdaptor::canConnectMyInputTo(ossim_int32 inputIndex,
                                                     const ossimConnectableObject* obj) const
{
   const ossimImageHandler* h = dynamic_cast<const ossimImageHandler*>(obj);
   if ((inputIndex == 0) && (h != NULL))
      return true;
   return false;
}


//**************************************************************************************************
//! Intercepts the getTile call intended for the adaptee and sets a mutex lock around the
//! adaptee's getTile call.
//**************************************************************************************************
ossimRefPtr<ossimImageData> 
   ossimImageHandlerMtAdaptor::getTile(const ossimIpt& origin, ossim_uint32 rLevel)
{
   if (!m_adaptedHandler.valid())
      return NULL;

   // Establish tile rect to call overloaded getTile(tile_rect):
   ossim_uint32 h = m_adaptedHandler->getTileHeight();
   ossim_uint32 w = m_adaptedHandler->getTileWidth();
   ossimIpt lr (origin.x + w - 1, origin.y + h - 1);
   ossimIrect tile_rect (origin, lr);

   // Need to unlock to prevent freezing in the called getTile():
   return getTile(tile_rect, rLevel);
}

//**************************************************************************************************
//! Intercepts the getTile call intended for the adaptee and sets a mutex lock around the
//! adaptee's getTile call.
//**************************************************************************************************
ossimRefPtr<ossimImageData>  
   ossimImageHandlerMtAdaptor::getTile(const ossimIrect& tile_rect, ossim_uint32 rLevel)
{
   if (d_useFauxTile)
   {
      ossimRefPtr<ossimImageData> ftile = new ossimImageData(*(d_fauxTile.get()));
      ftile->setOrigin(tile_rect.ul());
      return ftile;
   }

   if (!m_adaptedHandler.valid())
      return NULL;

   // The sole purpose of the adapter is this mutex lock around the actual handler getTile:
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);

   ossimRefPtr<ossimImageData> tile = new ossimImageData();
   ossimRefPtr<ossimImageData> temp_tile = 0;
   double dt = ossimTimer::instance()->time_s();
   if (d_useCache)
      temp_tile = m_cache->getTile(tile_rect, rLevel);
   else
      temp_tile = m_adaptedHandler->getTile(tile_rect, rLevel);
   d_getTileT += ossimTimer::instance()->time_s() - dt;

   // We make our own instance of a tile and copy the adaptee's returned tile to it. This avoids
   // the product tile from changing while being processed up the chain. The adaptee's tile can
   // change as soon as the mutex lock is released:
   
   if (temp_tile.valid())
      *tile = *(temp_tile.get());
   else
      tile = NULL;
   return tile;
}

//**************************************************************************************************
//! Intercepts the getTile call intended for the adaptee and sets a mutex lock around the
//! adaptee's getTile call.
//**************************************************************************************************
bool ossimImageHandlerMtAdaptor::getTile(ossimImageData* tile, ossim_uint32 rLevel)
{
   if ((!m_adaptedHandler.valid()) || (tile == NULL))
      return false;

   // The sole purpose of the adapter is this mutex lock around the actual handler getTile:
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);

   // This is effectively a copy of ossimImageSource::getTile(ossimImageData*). It is reimplemented 
   // here to save two additional function calls:
   tile->ref();
   bool status = true;
   ossimIrect tile_rect = tile->getImageRectangle();

   ossimRefPtr<ossimImageData> temp_tile = 0;
   if (d_useCache)
      temp_tile = m_cache->getTile(tile_rect, rLevel);
   else
      temp_tile = m_adaptedHandler->getTile(tile_rect, rLevel);
   
   if (temp_tile.valid())
      *tile = *(temp_tile.get());
   else
      status = false;
   tile->unref();
   
   return status;
}

//**************************************************************************************************
//! Method to save the state of an object to a keyword list.
//! Return true if ok or false on error.
//**************************************************************************************************
bool ossimImageHandlerMtAdaptor::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   if (!m_adaptedHandler.valid())
      return false;
   
   // Skip the ossimImageHandler::saveState() since it is not necessary here:
   ossimImageSource::saveState(kwl, prefix);

   kwl.add(prefix, ADAPTEE_ID_KW, m_adaptedHandler->getId().getId());

   return true;
}

//**************************************************************************************************
//! Method to the load (recreate) the state of an object from a keyword
//! list.  Return true if ok or false on error.
//**************************************************************************************************
bool ossimImageHandlerMtAdaptor::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   m_adaptedHandler = 0;

   // Skip the ossimImageHandler::loadState() since it is not necessary here:
   if (!ossimImageSource::loadState(kwl, prefix))
      return false;

   // The adaptee's ID at least will be in the KWL:
   ossimString value = kwl.find(prefix, ADAPTEE_ID_KW);
   if (value.empty())
      return false;
   
   return true;
}

//**************************************************************************************************
// The following are virtuals in the base class. Implemented here as pass-through to adaptee
//**************************************************************************************************
ossim_uint32 ossimImageHandlerMtAdaptor::getNumberOfInputBands() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getNumberOfInputBands();
   return 0;
}

bool ossimImageHandlerMtAdaptor::isOpen() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->isOpen();
   return false;
}

bool ossimImageHandlerMtAdaptor::open()
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->open();
   return false;
}

ossim_uint32 ossimImageHandlerMtAdaptor::getNumberOfLines(ossim_uint32 resLevel) const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getNumberOfLines(resLevel);
   return 0;
}

ossim_uint32 ossimImageHandlerMtAdaptor::getNumberOfSamples(ossim_uint32 resLevel) const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getNumberOfSamples(resLevel);
   return 0;
}

ossim_uint32 ossimImageHandlerMtAdaptor::getImageTileWidth() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getImageTileWidth();
   return 0;
}

ossim_uint32 ossimImageHandlerMtAdaptor::getImageTileHeight() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getImageTileHeight();
   return 0;
}

ossimString ossimImageHandlerMtAdaptor::getLongName() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getLongName();
   return ossimString();
}

ossimString ossimImageHandlerMtAdaptor::getShortName() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getShortName();
   return ossimString();
}

void ossimImageHandlerMtAdaptor::close()
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->close();
}

ossim_uint32 ossimImageHandlerMtAdaptor::getNumberOfOutputBands() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getNumberOfOutputBands();
   return 0;
}

ossim_uint32 ossimImageHandlerMtAdaptor::getNumberOfDecimationLevels() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getNumberOfDecimationLevels();
   return 0;
}

ossimScalarType ossimImageHandlerMtAdaptor::getOutputScalarType() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getOutputScalarType();
   return OSSIM_SCALAR_UNKNOWN;
}

ossim_uint32 ossimImageHandlerMtAdaptor::getTileWidth() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getTileWidth();
   return 0;
}

ossim_uint32 ossimImageHandlerMtAdaptor::getTileHeight() const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getTileHeight();
   return 0;
}

ossim_float64 ossimImageHandlerMtAdaptor::getMinPixelValue(ossim_uint32 band) const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getMinPixelValue(band);
   return 0.0;
}

ossim_float64 ossimImageHandlerMtAdaptor::getMaxPixelValue(ossim_uint32 band) const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getMaxPixelValue(band);
   return 0.0;
}

ossim_float64 ossimImageHandlerMtAdaptor::getNullPixelValue(ossim_uint32 band) const
{
   if (m_adaptedHandler.valid())
      return m_adaptedHandler->getNullPixelValue(band);
   return 0.0;
}

