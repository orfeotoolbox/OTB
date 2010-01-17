//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*******************************************************************
//  $Id: ossimStreamFactoryRegistry.cpp 11177 2007-06-07 19:47:04Z dburken $
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
      theInstance = new ossimStreamFactoryRegistry();
      theInstance->registerFactory(ossimStreamFactory::instance());
   }

   return theInstance;
}

ossimRefPtr<ossimIFStream>
ossimStreamFactoryRegistry::createNewIFStream(
   const ossimFilename& file,
   std::ios_base::openmode openMode) const
{
   ossim_uint32 idx = 0;
   ossimRefPtr<ossimIFStream> result = 0;
   for(idx = 0; ((idx < theFactoryList.size())&&(!result)); ++idx)
   {
      result = theFactoryList[idx]->createNewIFStream(file, openMode);
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

ossimStreamFactoryRegistry::ossimStreamFactoryRegistry(const ossimStreamFactoryRegistry&)
{}
