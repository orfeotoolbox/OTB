#ifndef ossimMtDebug_HEADER
#define ossimMtDebug_HEADER

//*************************************************************************************************
//! For debugging purposes. To be removed with final release:
//*************************************************************************************************
class OSSIMDLLEXPORT ossimMtDebug
{
   friend class ossimMultiThreadSequencer;
   friend class ossimImageChainMtAdaptor;
   friend class ossimImageHandlerMtAdaptor;

public:
   ossimMtDebug() 
      : handlerCacheEnabled (false), 
      handlerUseFauxTile (false),
      chainDebugEnabled (false),
      chainSharedHandlers (false),
      seqDebugEnabled (false),
      seqTimedBlocksDt (0),
      seqMetricsEnabled (false),
      maxTileCacheSize (0) { m_instance = this; }

   static ossimMtDebug* instance()
   { 
      if (m_instance == NULL) 
         m_instance = new ossimMtDebug; 
      return m_instance; 
   }
   
   bool handlerCacheEnabled;
   bool handlerUseFauxTile;
   bool chainDebugEnabled;
   bool chainSharedHandlers;
   bool seqDebugEnabled;
   ossim_uint32 seqTimedBlocksDt;
   bool seqMetricsEnabled;
   ossim_uint32 maxTileCacheSize;

private:
   static ossimMtDebug* m_instance;
};

#endif
