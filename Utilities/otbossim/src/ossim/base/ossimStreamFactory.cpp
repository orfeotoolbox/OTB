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
//  $Id: ossimStreamFactory.cpp 18652 2011-01-10 13:30:31Z dburken $
//
#include <ossim/base/ossimStreamFactory.h>
#include <fstream>
#include <ossim/ossimConfig.h>
#include <ossim/base/ossimFilename.h>
#if OSSIM_HAS_LIBZ
#include <ossim/base/ossimGzStream.h>
#endif


ossimStreamFactory* ossimStreamFactory::theInstance = 0;

ossimStreamFactory::ossimStreamFactory()
   : ossimStreamFactoryBase()
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

ossimRefPtr<ossimIFStream> ossimStreamFactory::createNewIFStream(
   const ossimFilename& file,
   std::ios_base::openmode openMode) const
{
   ossimRefPtr<ossimIFStream> result = 0;
   
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
            return result;
         }
      }
   }
   
   std::ifstream in(copyFile.c_str(), std::ios::in|std::ios::binary);

   if(!in) return result;

   unsigned char buf[2];

   in.read((char*)buf, 2);
   in.close();
   // check for gzip magic number
   //
   if((buf[0] == 0x1F) &&
      (buf[1] == 0x8B))
   {
      result = new ossimIgzStream(copyFile.c_str(), openMode);
   }
#endif
   return result;
}

ossimStreamFactory::ossimStreamFactory(const ossimStreamFactory&)
   : ossimStreamFactoryBase()
{}



