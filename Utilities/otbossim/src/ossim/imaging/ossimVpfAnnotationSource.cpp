#include <algorithm>

#include <ossim/imaging/ossimVpfAnnotationSource.h>
#include <ossim/imaging/ossimVpfAnnotationLibraryInfo.h>
#include <ossim/vec/ossimVpfExtent.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>

static ossimTrace traceDebug("ossimVpfAnnotationSource:debug");

ossimVpfAnnotationSource::ossimVpfAnnotationSource()
   :ossimGeoAnnotationSource()
{
  if(!m_geometry)
  {
     m_geometry = new ossimImageGeometry(0, new ossimEquDistCylProjection);
  }
   ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(m_geometry->getProjection());

   if(mapProj)
   {
      mapProj->setMetersPerPixel(ossimDpt(30, 30));
   }
}

ossimVpfAnnotationSource::~ossimVpfAnnotationSource()
{
   close();
}

bool ossimVpfAnnotationSource::open(const ossimFilename& file)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationSource::open DEBUG: entering..." << std::endl;
   }
   bool result = false;
   
   if(file.file().downcase() == "dht")
   {
      if(openDatabase(file))
      {
         vector<ossimString> libraryNames = theDatabase.getLibraryNames();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "Library names list size = " << libraryNames.size() << std::endl;
         }
         
         for(int idx = 0; idx < (int)libraryNames.size();++idx)
         {
            ossimVpfAnnotationLibraryInfo* info = new ossimVpfAnnotationLibraryInfo;
            info->setName(libraryNames[idx]);
            theLibraryInfo.push_back(info);
            info->setDatabase(&theDatabase);
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "loading library " << libraryNames[idx] << std::endl;
            }
            info->buildLibrary();
            info->getAllFeatures(theFeatureList);
         }
         transformObjects();
         result = true;
      }
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationSource::open DEBUG: leaving..." << std::endl;
   }
   return result;
}

void ossimVpfAnnotationSource::close()
{
   theDatabase.closeDatabase();
   for(int idx = 0; idx < (int)theLibraryInfo.size(); ++idx)
   {
      delete theLibraryInfo[idx];
   }
   
   theLibraryInfo.clear();
}

ossimFilename ossimVpfAnnotationSource::getFilename()const
{
   return theFilename;
}

void ossimVpfAnnotationSource::transformObjects(ossimImageGeometry* geom)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationSource::transformObjects DEBUG: entered..." << std::endl;
   }
   ossimImageGeometry* tempGeom = geom;
   
   if(!tempGeom)
   {
      tempGeom = m_geometry.get();
   }
   
   if(!tempGeom) return;
   for(int idx = 0; idx < (int)theLibraryInfo.size(); ++idx)
   {
      theLibraryInfo[idx]->transform(tempGeom);
   }
   computeBoundingRect();
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimVpfAnnotationSource::transformObjects DEBUG: leaving..." << std::endl;
   }
}

void ossimVpfAnnotationSource::computeBoundingRect()

{
  ossimIrect result;
  result.makeNan();
  for(int i = 0; i < (int)theLibraryInfo.size();++i)
    {
      ossimIrect tempRect = theLibraryInfo[i]->getBoundingProjectedRect();
      if(!tempRect.hasNans())
	{
	  if(result.hasNans())
	    {
	      result = tempRect;
	    }
	  else
	    {
	      result = result.combine(tempRect);
	    }
	}
    }

  theRectangle = result;
}


void ossimVpfAnnotationSource::deleteAllLibraries()
{
  for(int idx = 0; idx < (int)theLibraryInfo.size();++idx)
    {
      delete theLibraryInfo[idx];
    }
  
  theLibraryInfo.clear();
}

void ossimVpfAnnotationSource::getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& features)
{
   features = theFeatureList;
}

void ossimVpfAnnotationSource::setAllFeatures(const std::vector<ossimVpfAnnotationFeatureInfo*>& features)
{
  theFeatureList = features;
  
  computeBoundingRect();
  
}


void ossimVpfAnnotationSource::drawAnnotations(ossimRefPtr<ossimImageData> tile)
{
   theImage->setCurrentImageData(tile);
   if(theImage->getImageData().valid())
   {
      for(ossim_uint32 idx = 0; idx < theFeatureList.size();++idx)
      {
         if(theFeatureList[idx]->getEnabledFlag())
         {
            theFeatureList[idx]->drawAnnotations(theImage.get());
         }
      }
   }  
}

bool ossimVpfAnnotationSource::openDatabase(const ossimFilename& file)
{
  ossimAnnotationSource::deleteAll();
  deleteAllLibraries();
  theFeatureList.clear();
  if(theDatabase.isOpened())
    {
      theDatabase.closeDatabase();
    }

  theFilename = file;
  return theDatabase.openDatabase(file);
}


bool ossimVpfAnnotationSource::saveState(ossimKeywordlist& kwl,
                                         const char* prefix)const
{
  kwl.add(prefix,
	  ossimKeywordNames::FILENAME_KW,
	  theFilename,
	  true);
  for(ossim_uint32 idx = 0; idx < theLibraryInfo.size(); ++idx)
    {
       theLibraryInfo[idx]->saveState(kwl,
 				     (ossimString(prefix) + "library" + ossimString::toString(idx) + ".").c_str());
    }
   return true;
}

bool ossimVpfAnnotationSource::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
  bool result = false;
  deleteAllLibraries();
  const char* filename    = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
  if(filename)
    {
      if(openDatabase(filename))
	{
	  int idx = 0;
	  ossimString regExpression =  ossimString("^(") + ossimString(prefix) + "library[0-9]+.)";
	  vector<ossimString> keys =
	    kwl.getSubstringKeyList( regExpression );
	  std::vector<int> theNumberList(keys.size());
	  int offset = (int)(ossimString(prefix)+"library").size();

	  for(idx = 0; idx < (int)theNumberList.size();++idx)
	    {
	      ossimString numberStr(keys[idx].begin() + offset,
				    keys[idx].end());
	      theNumberList[idx] = numberStr.toInt();
	    }
	  std::sort(theNumberList.begin(), theNumberList.end());

	  for(idx=0;idx < (int)keys.size();++idx)
	    {
	      ossimString newPrefix = ossimString(prefix);
	      newPrefix += ossimString("library");
	      newPrefix += ossimString::toString(theNumberList[idx]);
	      newPrefix += ossimString(".");

	      ossimVpfAnnotationLibraryInfo* info = new ossimVpfAnnotationLibraryInfo;
	      theLibraryInfo.push_back(info);
	      info->setDatabase(&theDatabase);
	      info->loadState(kwl,
			      newPrefix);
	      info->getAllFeatures(theFeatureList);
	    }
	  transformObjects();
	  result = true;
	}
    }
  
  return result;
}

bool ossimVpfAnnotationSource::open()
{
   return open(theFilename);
}

bool ossimVpfAnnotationSource::isOpen()const
{
   return (theDatabase.isOpened());
}
