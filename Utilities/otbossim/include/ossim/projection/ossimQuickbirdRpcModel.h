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
//  $Id: ossimQuickbirdRpcModel.h 11720 2007-09-12 14:59:30Z gpotts $
#ifndef ossimQuickbirdRpcModel_HEADER
#define ossimQuickbirdRpcModel_HEADER

#include <ossim/projection/ossimRpcModel.h>
#include <ossim/support_data/ossimQuickbirdMetaData.h>

class ossimFilename;
class ossimQuickbirdMetaData;

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
   ~ossimQuickbirdRpcModel();

   virtual ossimObject* dup() const;
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool parseFile(const ossimFilename& file);
   
   
protected:
   //void finishConstruction();
   void parseMetaData(const ossimFilename& metadata);
   void parseRpcData (const ossimFilename& rpcdata);

   bool parseNitfFile(const ossimFilename& file);
   bool parseTiffFile(const ossimFilename& file);
/*
  ossimQuickbirdMetaData* getTheSupportData()
  {
    return theSupportData;
  }
*/
   ossimQuickbirdMetaData* theSupportData;

TYPE_DATA
};

#endif
