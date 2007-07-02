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
//  $Id: ossimStreamFactory.h 9094 2006-06-13 19:12:40Z dburken $
//
#ifndef ossimStreamFactory_HEADER
#define ossimStreamFactory_HEADER
#include <ossim/base/ossimStreamFactoryBase.h>
#include <ossim/base/ossimIoStream.h>

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
