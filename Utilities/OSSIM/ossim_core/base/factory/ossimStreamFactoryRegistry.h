//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//
//*******************************************************************
//  $Id: ossimStreamFactoryRegistry.h,v 1.3 2005/11/14 13:47:01 gpotts Exp $
//
#ifndef ossimStreamFactoryRegistry_HEADER
#define ossimStreamFactoryRegistry_HEADER
#include "ossimStreamFactoryBase.h"
#include <vector>

class OSSIM_DLL ossimStreamFactoryRegistry : public ossimStreamFactoryBase
{
public:
   static ossimStreamFactoryRegistry* instance();
   virtual ~ossimStreamFactoryRegistry();
   
   void registerFactory(ossimStreamFactoryBase* factory);
   
   virtual ossimRefPtr<ossimIStream> createNewInputStream(const ossimFilename& file,
	   std::ios::openmode openMode);
   
protected:
   ossimStreamFactoryRegistry();
private:
   ossimStreamFactoryRegistry(const ossimStreamFactoryRegistry&){}
   std::vector<ossimStreamFactoryBase*> theFactoryList;
   static ossimStreamFactoryRegistry* theInstance;
};

#endif
