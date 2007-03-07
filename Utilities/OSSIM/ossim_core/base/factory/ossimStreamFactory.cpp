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
//  $Id: ossimStreamFactory.cpp,v 1.4 2005/11/14 13:47:01 gpotts Exp $
//
#include "ossimStreamFactory.h"
#include <fstream>
#include <ossimConfig.h>
#if OSSIM_HAS_LIBZ
#include <base/common/ossimGzStream.h>
#endif 

ossimStreamFactory* ossimStreamFactory::theInstance = 0;

ossimStreamFactory::ossimStreamFactory()
{
}

ossimStreamFactory::~ossimStreamFactory()
{
}

ossimStreamFactory* ossimStreamFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimStreamFactory;
   }

   return theInstance;
}

ossimRefPtr<ossimIStream> ossimStreamFactory::createNewInputStream(const ossimFilename& file,
																   std::ios::openmode openMode)
{
#if OSSIM_HAS_LIBZ
   ossimFilename copyFile = file;

   if(!copyFile.exists())
   {
      ossimString ext = copyFile.ext();
      copyFile.setExtension("gz");
      if(!copyFile.exists())
      {
         copyFile.setExtension(ext);
         copyFile += ".gz";

         if(!copyFile.exists())
         {
            return false;
         }
      }
   }
   
   std::ifstream in(copyFile.c_str(), std::ios::in|std::ios::binary);

   if(!in) return 0;

   unsigned char buf[2];

   in.read((char*)buf, 2);
   in.close();
   // check for gzip magic number
   //
   if((buf[0] == 0x1F) &&
      (buf[1] == 0x8B))
   {
      return new ossimIgzStream(copyFile.c_str(),
                                openMode);
   }
#endif
   return 0;
}

