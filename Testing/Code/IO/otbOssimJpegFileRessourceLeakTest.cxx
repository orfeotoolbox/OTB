/*
 * JpegFileDescriptorRessourceLeak.cxx
 *
 *  Created on: 12 mars 2010
 *      Author: jmichel
 */
#include <imaging/ossimImageHandler.h>
#include <imaging/ossimImageHandlerRegistry.h>
#include <base/ossimFilename.h>
#include <stdlib.h>
#include <iostream>

// This test demonstrate that handler->close() does not release the file descriptor for jpeg files.

int otbOssimJpegFileRessourceLeakTest(int argc, char * argv[])
{
  if(argc != 3)
    {
    std::cerr<<"Usage:"<<std::endl;
    std::cerr<<argv[0]<<" infname nbReading"<<std::endl;
    return 1;
    }

  const char * infname = argv[1];
  const unsigned int nbReading = atoi(argv[2]);

  for(unsigned int i = 0; i < nbReading; ++i)
    {
    // Create the handler
    ossimImageHandler * handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(infname));
    if(!handler)
      {
      std::cerr<<"Failed to open image "<<i<<" times."<<std::endl;
      return 1;
      }
    handler->close();
    }
  return 0;
}
