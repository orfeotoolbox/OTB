#include <ossim/elevation/ossimGeneralRasterElevFactory.h>
#include <ossim/elevation/ossimGeneralRasterElevHandler.h>
#include <ossim/base/ossimDirectory.h>

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

void ossimGeneralRasterElevFactory::setDirectory(const ossimFilename& directory)
{
   ossimElevSourceFactory::setDirectory(directory);
   theGeneralRasterInfoList.clear();
   ossimRefPtr<ossimGeneralRasterElevHandler>  handler = new ossimGeneralRasterElevHandler;
  
   if(theDirectory.exists())
   {
      if(theDirectory.isDir())
      {
         ossimDirectory dir(theDirectory);
         
         ossimFilename file;
         ossim_uint32 maxCount = 10; // search at least the first 10 files to see if there are any here
         ossim_uint32 count = 0;
         bool foundOne = false;
         if(dir.getFirst(file))
         {
            do
            {
               ++count;
               ossimString ext = file.ext();
               ext = ext.downcase();
               if(ext == "ras")
               {
                  if(handler->open(file))
                  {
                     foundOne = true;
                     addInfo(handler->generalRasterInfo());
                  }
               }
            } while(dir.getNext(file) &&
                    (foundOne ||
                     (!foundOne && (count < maxCount))));
         }
      }
   }  
}

ossimElevSource* ossimGeneralRasterElevFactory::getNewElevSource(const ossimGpt& gpt) const
{
   ossimDpt pt(gpt);
   std::vector<ossimGeneralRasterElevHandler::GeneralRasterInfo>::const_iterator i = theGeneralRasterInfoList.begin();
   while (i != theGeneralRasterInfoList.end())
   {
      if ((*i).theWgs84GroundRect.pointWithin(pt))
      {
         return new ossimGeneralRasterElevHandler((*i));
         
      }
      ++i;
   }
   return 0;
}

void ossimGeneralRasterElevFactory::addInfo(const ossimGeneralRasterElevHandler::GeneralRasterInfo& info)
{
   theGeneralRasterInfoList.push_back(info);
}

void ossimGeneralRasterElevFactory::createIndex()
{
}
