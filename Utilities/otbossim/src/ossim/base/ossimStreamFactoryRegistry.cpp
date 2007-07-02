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
//  $Id: ossimStreamFactoryRegistry.cpp 9094 2006-06-13 19:12:40Z dburken $
//
#include <ossim/base/ossimStreamFactoryRegistry.h>
#include <ossim/base/ossimStreamFactory.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimFilename.h>

#include <fstream>
#include <algorithm>

ossimStreamFactoryRegistry* ossimStreamFactoryRegistry::theInstance = 0;

ossimStreamFactoryRegistry::ossimStreamFactoryRegistry()
{
}

ossimStreamFactoryRegistry::~ossimStreamFactoryRegistry()
{
}

ossimStreamFactoryRegistry* ossimStreamFactoryRegistry::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimStreamFactoryRegistry;
      theInstance->registerFactory(ossimStreamFactory::instance());
   }

   return theInstance;
}

ossimRefPtr<ossimIStream> ossimStreamFactoryRegistry::createNewInputStream(const ossimFilename& file,
																		   std::ios::openmode openMode)
{
   ossim_uint32 idx = 0;
   ossimRefPtr<ossimIStream> result = 0;
   for(idx = 0; ((idx < theFactoryList.size())&&(!result)); ++idx)
   {
      result = theFactoryList[idx]->createNewInputStream(file, openMode);
   }

   if(!result)
   {
      result = new ossimIFStream(file.c_str(),
                                 openMode);
//       result = new std::ifstream(file.c_str(),
//                                  openMode);
   }
   
   return result;
}


void ossimStreamFactoryRegistry::registerFactory(ossimStreamFactoryBase* factory)
{
   std::vector<ossimStreamFactoryBase*>::iterator iter = std::find(theFactoryList.begin(),
                                                                  theFactoryList.end(),
                                                                  factory);
   if(iter == theFactoryList.end())
   {
      theFactoryList.push_back(factory);
   }
}
