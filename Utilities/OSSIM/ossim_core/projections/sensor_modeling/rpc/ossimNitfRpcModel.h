//*****************************************************************************
// FILE: ossimNitfRpcModel.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class ossimNitfRpcModel. This 
//    derived class implements the capability of reading Nitf RPC support
//    data.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimNitfRpcModel.h,v 1.4 2005/09/09 19:23:58 gpotts Exp $

#ifndef ossimNitfRpcModel_HEADER
#define ossimNitfRpcModel_HEADER

#include "ossimRpcModel.h"

class ossimFilename;

/*!****************************************************************************
 *
 * CLASS:  ossimNitfRpcModel
 *
 *****************************************************************************/
class ossimNitfRpcModel : public ossimRpcModel
{
public:
   ossimNitfRpcModel();
   ossimNitfRpcModel(const ossimNitfRpcModel& rhs);
   ossimNitfRpcModel(const ossimFilename& nitfFile);
   virtual ossimObject* dup() const { return new ossimNitfRpcModel(*this); }
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

private:
   double theBiasError;
   double theRandError;

   TYPE_DATA
};

#endif
