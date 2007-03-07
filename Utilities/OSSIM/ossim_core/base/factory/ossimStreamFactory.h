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
//  $Id: ossimStreamFactory.h,v 1.3 2005/11/14 13:47:01 gpotts Exp $
//
#ifndef ossimStreamFactory_HEADER
#define ossimStreamFactory_HEADER
#include "ossimStreamFactoryBase.h"

class OSSIM_DLL ossimStreamFactory : public ossimStreamFactoryBase
{
public:
   static ossimStreamFactory* instance();
   virtual ~ossimStreamFactory();
 
   virtual ossimRefPtr<ossimIStream> createNewInputStream(const ossimFilename& file,
	   std::ios::openmode openMode);

   
protected:
   ossimStreamFactory();
   ossimStreamFactory(const ossimStreamFactory&){}
   static ossimStreamFactory* theInstance;
   
};

#endif
