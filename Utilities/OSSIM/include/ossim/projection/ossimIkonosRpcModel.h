//*******************************************************************
//
// License:  See top level LICENSE.txt file.
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
//  $Id: ossimIkonosRpcModel.h 10430 2007-02-07 01:05:22Z dburken $

#ifndef ossimIkonosRpcModel_HEADER
#define ossimIkonosRpcModel_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/projection/ossimRpcModel.h>

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
   /*!
    * STATIC METHOD: writeGeomTemplate(ostream)
    * Writes a template of an ossimIkonosRpcModel geometry file.
    */
   static void writeGeomTemplate(ostream& os);

   bool parseFile(const ossimFilename& file);
   
protected:
   void finishConstruction();
   void parseMetaData(const ossimFilename& metadata);
   void parseRpcData (const ossimFilename& rpcdata);

   bool isNitf(const ossimFilename& filename);
   bool parseTiffFile(const ossimFilename& filename);
   bool parseNitfFile(const ossimFilename& geom_file);
   bool parseHdrData(const ossimFilename& data_file);

   TYPE_DATA
};

#endif
