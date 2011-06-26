#include <ossim/elevation/ossimGeneralRasterElevationDatabase.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimDirectory.h>
#include <sstream>
#include <iomanip>

RTTI_DEF1(ossimGeneralRasterElevationDatabase, "ossimGeneralRasterElevationDatabase", ossimElevationCellDatabase);

double ossimGeneralRasterElevationDatabase::getHeightAboveMSL(const ossimGpt& gpt)
{
   if(!isSourceEnabled()) return ossim::nan();
   ossimRefPtr<ossimElevCellHandler> handler = getOrCreateCellHandler(gpt);

   if(handler.valid())
   {
      return handler->getHeightAboveMSL(gpt); // still need to shift
   }
   return ossim::nan();
}
double ossimGeneralRasterElevationDatabase::getHeightAboveEllipsoid(const ossimGpt& gpt)
{
   double h = getHeightAboveMSL(gpt);
   if(h != ossim::nan())
   {
      h += getOffsetFromEllipsoid(gpt);
   }
   
   return h;
}
bool ossimGeneralRasterElevationDatabase::open(const ossimString& connectionString)
{
   bool result = false;
   ossimFilename file = ossimFilename(connectionString);
   m_connectionString = connectionString;
   result = openGeneralRasterDirectory(file);
   
   return result;
}

bool ossimGeneralRasterElevationDatabase::openGeneralRasterDirectory(const ossimFilename& dir)
{
   m_cellHandler = new ossimGeneralRasterElevHandler;
   bool foundOne = false;
   
   if(dir.exists())
   {
      if(dir.isDir())
      {
         ossimDirectory d(dir);
         
         ossimFilename file;
         ossim_uint32 maxCount = 10; // search at least the first 10 files to see if there are any here
         ossim_uint32 count = 0;
         if(d.getFirst(file))
         {
            do
            {
               ++count;
               ossimString ext = file.ext();
               ext = ext.downcase();
               if(ext == "ras")
               {
                  if(m_cellHandler->open(file, m_memoryMapCellsFlag))
                  {
                     m_meanSpacing = m_cellHandler->getMeanSpacingMeters();
                     foundOne = true;
                  }
               }
            } while(d.getNext(file) &&
                    (foundOne ||
                     (!foundOne && (count < maxCount))));
         }
      }
   }  
   
   if(!foundOne)
   {
      m_cellHandler = 0;
   }
   
   return m_cellHandler.valid();
}

bool ossimGeneralRasterElevationDatabase::pointHasCoverage(const ossimGpt& gpt) const
{
   bool result = false;
   
   if(m_cellHandler.valid())
   {
      result = m_cellHandler->pointHasCoverage(gpt);
   }
   
   return result;
}

void ossimGeneralRasterElevationDatabase::createRelativePath(
   ossimFilename& /* file */, const ossimGpt& /* gpt */)const
{
}

ossimRefPtr<ossimElevCellHandler> ossimGeneralRasterElevationDatabase::createHandler(const ossimGpt&  gpt )
{
   if(pointHasCoverage(gpt))
   {
      return m_cellHandler.get();
   }
   return 0;
}

ossimRefPtr<ossimElevCellHandler> ossimGeneralRasterElevationDatabase::createCell(const ossimGpt& gpt)
{
   if(pointHasCoverage(gpt))
   {
      return m_cellHandler.get();
   }
   return 0;
}


bool ossimGeneralRasterElevationDatabase::loadState(const ossimKeywordlist& kwl, const char* prefix )
{
   bool result = ossimElevationDatabase::loadState(kwl, prefix);
   if(result)
   {
      if(!m_connectionString.empty()&&ossimFilename(m_connectionString).exists())
      {
         result = open(m_connectionString);
      }
      else
      {
         // can't open the connection because it does not exists or empty
         result = false;
      }
   }
   
   return result;
}

bool ossimGeneralRasterElevationDatabase::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   bool result = ossimElevationDatabase::saveState(kwl, prefix);
   
   return result;
}


