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

class ossimFilename;

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
   
   virtual ossimObject* dup() const;
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool parseFile(const ossimFilename& file);
   
   
protected:

   bool parseNitfFile(const ossimFilename& file);
   bool parseTiffFile(const ossimFilename& file);
   
   
TYPE_DATA
};

#endif
