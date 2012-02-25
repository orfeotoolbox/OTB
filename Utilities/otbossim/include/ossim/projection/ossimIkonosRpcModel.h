//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class ossimIkonosRpcModel. This 
//    derived class implements the capability of reading Ikonos RPC support
//    data.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimIkonosRpcModel.h 20606 2012-02-24 12:29:52Z gpotts $

#ifndef ossimIkonosRpcModel_HEADER
#define ossimIkonosRpcModel_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/projection/ossimRpcModel.h>
#include <ossim/support_data/ossimIkonosMetaData.h>

class ossimFilename;

/*!****************************************************************************
 *
 * CLASS:  ossimIkonosRpcModel
 *
 *****************************************************************************/
class OSSIM_DLL ossimIkonosRpcModel : public ossimRpcModel
{
public:
  ossimIkonosRpcModel();
   ossimIkonosRpcModel(const ossimFilename& geom_file);

   ossimIkonosRpcModel(const ossimFilename& metadata,
                       const ossimFilename& rpcdata);

  virtual bool saveState(ossimKeywordlist& kwl,
			 const char* prefix=0)const;

  /**
    * @brief loadState
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
  
   /*!
    * STATIC METHOD: writeGeomTemplate(ostream)
    * Writes a template of an ossimIkonosRpcModel geometry file.
    */
   static void writeGeomTemplate(ostream& os);

   virtual bool parseFile(const ossimFilename& file);
   
protected:
   virtual ~ossimIkonosRpcModel();
   void finishConstruction();
   void parseMetaData(const ossimFilename& metadata);
   void parseRpcData (const ossimFilename& rpcdata);

   bool isNitf(const ossimFilename& filename);
   bool parseTiffFile(const ossimFilename& filename);
/*    bool parseNitfFile(const ossimFilename& geom_file); */
   bool parseHdrData(const ossimFilename& data_file);

   ossimRefPtr<ossimIkonosMetaData> theSupportData;

   TYPE_DATA
};

#endif
