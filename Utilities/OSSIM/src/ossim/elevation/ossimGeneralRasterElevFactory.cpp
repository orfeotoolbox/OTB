#include <ossim/elevation/ossimGeneralRasterElevFactory.h>
#include <ossim/elevation/ossimGeneralRasterElevHandler.h>


RTTI_DEF1(ossimGeneralRasterElevFactory, "ossimGeneralRasterElevFactory", ossimElevSourceFactory)

ossimGeneralRasterElevFactory::ossimGeneralRasterElevFactory()
   :theHandlerReturnedFlag(false)
{
}

ossimGeneralRasterElevFactory::ossimGeneralRasterElevFactory(const ossimFilename& dir)
   :theHandlerReturnedFlag(false)
{
   setDirectory(dir);
}

ossimGeneralRasterElevFactory::~ossimGeneralRasterElevFactory()
{
}

ossimElevSource* ossimGeneralRasterElevFactory::getNewElevSource(const ossimGpt& gpt) const
{
   if(theHandlerReturnedFlag)
   {
      return 0;
   }

   ossimElevSource* source = new  ossimGeneralRasterElevHandler(theDirectory);
   if(source->pointHasCoverage(gpt))
   {
      theHandlerReturnedFlag = true;
   }
   else
   {
      delete source;
      source = 0;
   }
   return source;
}

void ossimGeneralRasterElevFactory::createIndex()
{
}
