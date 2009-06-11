
#include <cstdlib>

#include "projection/ossimProjectionFactoryRegistry.h"
#include "projection/ossimProjection.h"
#include <ossim/projection/ossimPcsCodeProjectionFactory.h>
#include "base/ossimKeywordlist.h"
#include "base/ossimFilename.h"

#include "base/ossimTraceManager.h"

#include "ossimPluginProjectionFactory.h"


int main(int argc, char *argv[])
{

  char *filename = argv[1];
  
  ossimTraceManager::instance()->setTracePattern("");
  
  ossimProjectionFactoryRegistry::instance()->unregisterFactory(ossimPcsCodeProjectionFactory::instance());
  ossimProjectionFactoryRegistry::instance()->registerFactory(ossimPluginProjectionFactory::instance());
  ossimProjection *projection = ossimProjectionFactoryRegistry::instance()->createProjection(ossimFilename(filename),0);
  
  if (!projection) {
    std::cout << "OSSIM Instantiate Projection FAILED! " << std::endl;
    return EXIT_FAILURE;
  } else {
    std::cout << "OSSIM Instantiate Projection SUCCESS! " << std::endl;
    ossimKeywordlist geom_kwl;

    // Read OSSIM Keyword List
    bool hasMetaData = projection->saveState(geom_kwl);
    delete projection;

    if (!hasMetaData) {
      std::cout << "OSSIM MetaData not present! " << std::endl;
      return EXIT_FAILURE;
    } else {
      std::cout << "OSSIM MetaData present! " << std::endl;
      std::cout << geom_kwl << std::endl;
    }

  }

  return EXIT_SUCCESS;
}
