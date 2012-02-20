//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
//**************************************************************************************************
//  $Id$
#ifndef ossimImageHandlerMtAdaptor_HEADER
#define ossimImageHandlerMtAdaptor_HEADER 1

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <OpenThreads/Thread>

//**************************************************************************************************
//! Intended mainly to provide a mechanism for mutex-locking access to a shared resource during
//! a getTile operation on an ossimImageHandler. This is needed for multi-threaded implementation.
//**************************************************************************************************
class OSSIMDLLEXPORT ossimImageHandlerMtAdaptor : public ossimImageHandler
{
public:
   static const char* ADAPTEE_ID_KW;

   ossimImageHandlerMtAdaptor(ossimImageHandler* adaptee = 0);

   //! Sets the handler being adapted.
   void setAdaptee(ossimImageHandler* handler);

   //! Only an ossimImageHandler is allowed as input here.
   virtual bool canConnectMyInputTo(ossim_int32 i, const ossimConnectableObject* obj) const;

   //! Intercepts the getTile call intended for the adaptee and sets a mutex lock around the
   //! adaptee's getTile call.
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIpt& origin, ossim_uint32 resLevel=0);
   
   //! Intercepts the getTile call intended for the adaptee and sets a mutex lock around the
   //! adaptee's getTile call.
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect, ossim_uint32 resLevel=0);

   //! Intercepts the getTile call intended for the adaptee and sets a mutex lock around the
   //! adaptee's getTile call.
   virtual bool getTile(ossimImageData* result, ossim_uint32 resLevel=0);
   
   //! Method to save the state of an object to a keyword list.
   //! Return true if ok or false on error.
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

   //! Method to the load (recreate) the state of an object from a keyword
   //! list.  Return true if ok or false on error.
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   // The following are virtuals in the base class. Implemented here as pass-through to adaptee
   virtual ossim_uint32    getNumberOfInputBands() const;
   virtual bool            isOpen() const;
   virtual bool            open();
   virtual ossim_uint32    getNumberOfLines(ossim_uint32 resLevel = 0) const;
   virtual ossim_uint32    getNumberOfSamples(ossim_uint32 resLevel = 0) const;
   virtual ossim_uint32    getImageTileWidth() const;
   virtual ossim_uint32    getImageTileHeight() const;
   virtual ossimString     getLongName()  const;
   virtual ossimString     getShortName() const;
   virtual void            close();
   virtual ossim_uint32    getNumberOfOutputBands() const;
   virtual ossim_uint32    getNumberOfDecimationLevels() const;
   virtual ossimScalarType getOutputScalarType() const;
   virtual ossim_uint32    getTileWidth() const;
   virtual ossim_uint32    getTileHeight() const;
   virtual ossim_float64   getMinPixelValue(ossim_uint32 band=0)const;
   virtual ossim_float64   getMaxPixelValue(ossim_uint32 band=0)const;
   virtual ossim_float64   getNullPixelValue(ossim_uint32 band=0)const;

   double       d_getTileT;

protected:
   //! Protected destructor forces using reference pointer for instantiation.
   virtual ~ossimImageHandlerMtAdaptor();

   ossimRefPtr<ossimImageHandler>    m_adaptedHandler;
   ossimRefPtr<ossimCacheTileSource> m_cache;
   mutable OpenThreads::Mutex        m_mutex;   

   bool                        d_useCache;
   bool                        d_useFauxTile;
   ossimRefPtr<ossimImageData> d_fauxTile;
   friend class ossimMtDebug;

TYPE_DATA
};

#endif









