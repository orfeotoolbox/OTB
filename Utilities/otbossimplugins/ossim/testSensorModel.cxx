
#include <cstdlib>

#include "imaging/ossimImageHandlerRegistry.h"
#include "imaging/ossimImageHandler.h"
#include "imaging/ossimImageHandlerFactory.h"
#include "imaging/ossimImageHandlerSarFactory.h"
#include "base/ossimKeywordlist.h"
#include "base/ossimFilename.h"

#include "base/ossimTraceManager.h"

#include "ossimPluginReaderFactory.h"

int main(int argc, char * argv[])
{

  char * filename = argv[1];
  
  ossimTraceManager::instance()->setTracePattern("");
  
  ossimImageHandlerRegistry::instance()->unregisterFactory(ossimImageHandlerFactory::instance());
  ossimImageHandlerRegistry::instance()->addFactory(ossimPluginReaderFactory::instance());
  ossimImageHandlerRegistry::instance()->addFactory(ossimImageHandlerSarFactory::instance());
  ossimImageHandler* handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(filename));

  if (!handler)
  {
    std::cout <<"OSSIM Open Image FAILED! " << std::endl;
    return EXIT_FAILURE;
  }

  else
  {
    std::cout <<"OSSIM Open Image SUCCESS! " << std::endl;
    ossimKeywordlist geom_kwl;

    // Read OSSIM Keyword List
    bool hasMetaData = handler->getImageGeometry(geom_kwl);

    if (!hasMetaData)
    {
      std::cout <<"OSSIM MetaData not present! " << std::endl;
      return EXIT_FAILURE;
    }
    else
    {
      std::cout <<"OSSIM MetaData present! " << std::endl;
      std::cout << geom_kwl << std::endl;
    }

  }

  return EXIT_SUCCESS;
}
