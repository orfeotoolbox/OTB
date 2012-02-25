//*****************************************************************************
// FILE: ossimQuickbirdRpcModel.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION: Contains declaration of class ossimQuickbirdRpcModel. This 
//    derived class implements the capability of reading Quickbird RPC support
//    data.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimQuickbirdRpcModel.h 20606 2012-02-24 12:29:52Z gpotts $
#ifndef ossimQuickbirdRpcModel_HEADER
#define ossimQuickbirdRpcModel_HEADER

#include <ossim/projection/ossimRpcModel.h>
#include <ossim/support_data/ossimQuickbirdMetaData.h>

class ossimFilename;
class ossimQuickbirdMetaData;
class ossimQbTileFilesHandler;

/*!****************************************************************************
 *
 * CLASS:  ossimQuickbirdRpcModel
 *
 *****************************************************************************/
class ossimQuickbirdRpcModel : public ossimRpcModel
{
public:
   ossimQuickbirdRpcModel();
   ossimQuickbirdRpcModel(const ossimQuickbirdRpcModel& rhs);

   //! Initializes
   ossimQuickbirdRpcModel(const ossimQbTileFilesHandler* handler);
   ~ossimQuickbirdRpcModel();

   virtual ossimObject* dup() const;
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool parseFile(const ossimFilename& file);
   
   void setSupportData(ossimQuickbirdMetaData* supportData)
   {
      theSupportData = supportData;
   }
   ossimQuickbirdMetaData* getSupportData()
   {
      return theSupportData.get();
   }
   const ossimQuickbirdMetaData* getSupportData()const
   {
      return theSupportData.get();
   }
protected:
   bool parseNitfFile(const ossimFilename& file);
   bool parseTiffFile(const ossimFilename& file);
   bool parseMetaData(const ossimFilename& file);
   bool parseRpcData (const ossimFilename& file);
   bool parseTileData(const ossimFilename& file);

   //! Given an initial filename with case-agnostic extension, this method searches first for an
   //! image-specific instance of that file (i.e., with _R*C* in the filename) before considering
   //! the mosaic-global support file (_R*C* removed). If a file is found, the argument is modified 
   //! to match the actual filename and TRUE is returned. Otherwise, argument filename is left 
   //! unchanged and FALSE is returned.
   bool findSupportFile(ossimFilename& file) const;

void finishConstruction();

   ossimRefPtr<ossimQuickbirdMetaData> theSupportData;

TYPE_DATA
};

#endif
