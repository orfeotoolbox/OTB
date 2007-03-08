#include <ossim/imaging/ossimImageShapeFileIndex.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimTrace.h>

#include <ossim/vec/shapefil.h>

static ossimTrace traceDebug("ossimImageShapeFileIndex:debug");

ossimImageShapeFileIndex::ossimImageShapeFileIndex(const ossimFilename& shapeFilename)
   :theGlobalMinx(OSSIM_INFINITY),
    theGlobalMiny(OSSIM_INFINITY),
    theGlobalMaxx(-OSSIM_INFINITY),
    theGlobalMaxy(-OSSIM_INFINITY)
{
   setFilename(shapeFilename);
}

void ossimImageShapeFileIndex::setFilename(const ossimFilename& shapeFilename)
{
   theFilename = shapeFilename;
   theFilename = theFilename.setExtension("");
}

void ossimImageShapeFileIndex::add(const ossimFilename& filename,
                                   const ossim_int32 entry, // default to all entries
                                   const ossimString& groupName)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageShapeFileIndex::add: Entered ...." << std::endl;
   }
   ossimRefPtr<ossimImageHandler> h = ossimImageHandlerRegistry::instance()->open(filename);
   ossimRefPtr<ossimProjection> proj;

   if(h.valid())
   {
      std::vector<ossim_uint32> entryList;
      if(entry < 0)
      {
         h->getEntryList(entryList);
      }
      else
      {
         entryList.push_back(static_cast<ossim_uint32>(entry));
      }

      ossim_uint32 idx = 0;
      for(idx = 0; idx < entryList.size(); ++idx)
      {
         ossimKeywordlist kwl;
         if(h->setCurrentEntry(entryList[idx]))
         {
            ossimIrect bounds = h->getBoundingRect();
            
            h->getImageGeometry(kwl);
            proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
            
            if(proj.valid())
            {
               if(traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG) << "Adding file:  " << filename << std::endl
                                                      << "       entry: " << entryList[idx] << std::endl;
               }
               std::vector<ossimDpt> points;
               ossim_float64 minx;
               ossim_float64 miny;
               ossim_float64 maxx;
               ossim_float64 maxy;
               ossimGpt ulgpt;
               ossimGpt urgpt;
               ossimGpt lrgpt;
               ossimGpt llgpt;
               const ossimDatum* datum = ossimDatumFactory::instance()->wgs84();
               proj->lineSampleToWorld(bounds.ul(), ulgpt);
               proj->lineSampleToWorld(bounds.ur(), urgpt);
               proj->lineSampleToWorld(bounds.lr(), lrgpt);
               proj->lineSampleToWorld(bounds.ll(), llgpt);

               ulgpt.changeDatum(datum);
               urgpt.changeDatum(datum);
               lrgpt.changeDatum(datum);
               llgpt.changeDatum(datum);
               
               points.push_back(ossimDpt(ulgpt));
               points.push_back(ossimDpt(urgpt));
               points.push_back(ossimDpt(lrgpt));
               points.push_back(ossimDpt(llgpt));

               ossimDrect rect(points, OSSIM_RIGHT_HANDED);

               minx = rect.ll().x;
               miny = rect.ll().y;
               maxx = rect.ur().x;
               maxy = rect.ur().y;

               if(minx < theGlobalMinx) theGlobalMinx = minx;
               if(maxx > theGlobalMaxx) theGlobalMaxx = maxx;
               if(miny < theGlobalMiny) theGlobalMiny = miny;
               if(maxy > theGlobalMaxy) theGlobalMaxy = maxy;

               theRecords.push_back(new ossimImageShapeFileIndexRecord(filename,
                                                                       entryList[idx],
                                                                       groupName,
                                                                       minx,
                                                                       miny,
                                                                       maxx,
                                                                       maxy));
            }
         }
      }
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageShapeFileIndex::add: Leaving ...." << std::endl;
   }
}

void ossimImageShapeFileIndex::build(ossim_uint32 maxDepth)const
{
   if(theRecords.size())
   {
      SHPHandle hSHP = SHPCreate( theFilename.c_str(), SHPT_POLYGON);
      DBFHandle hDBF = DBFCreate( theFilename.c_str());
      
      DBFAddField(hDBF, "GROUP", FTString, 256, 0);
      DBFAddField(hDBF, "FILENAME", FTString, 256, 0);
      DBFAddField(hDBF, "ENTRY", FTInteger, 10, 0);
      DBFAddField(hDBF, "DATUM", FTString, 16, 0);
      
      double* x = new double[4];
      double* y = new double[4];
      int nVertices = 4;
      ossim_uint32 idx = 0;
      int iRecord = 0;
      for(idx = 0; idx < theRecords.size(); ++idx)
      {
         x[0] = theRecords[idx]->theMinx;
         x[1] = theRecords[idx]->theMaxx;
         x[2] = theRecords[idx]->theMaxx;
         x[3] = theRecords[idx]->theMinx;

         y[0] = theRecords[idx]->theMaxy;
         y[1] = theRecords[idx]->theMaxy;
         y[2] = theRecords[idx]->theMiny;
         y[3] = theRecords[idx]->theMiny;
         
         SHPObject* obj = SHPCreateSimpleObject(SHPT_POLYGON,
                                                nVertices,
                                                x,
                                                y,
                                                0);
         SHPWriteObject( hSHP, -1, obj );
         iRecord = DBFGetRecordCount( hDBF );
         DBFWriteStringAttribute(hDBF, iRecord, 0, theRecords[idx]->theGroupName.c_str());
         DBFWriteStringAttribute(hDBF, iRecord, 1, theRecords[idx]->theFilename.c_str());
         DBFWriteIntegerAttribute(hDBF, iRecord, 2, (int)(theRecords[idx]->theEntry));
         DBFWriteStringAttribute(hDBF, iRecord, 3, "WGE");
         SHPDestroyObject(obj);
      }

      DBFClose(hDBF);
      SHPTree * shpTree = 0;
      double minV[4] = {0.0,0.0,0.0,0.0};
      double maxV[4] = {0.0,0.0,0.0,0.0};
      
      minV[0] = theGlobalMinx;
      minV[1] = theGlobalMiny;
      
      maxV[0] = theGlobalMaxx;
      maxV[1] = theGlobalMaxy;
      if(maxDepth>1)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "building quadtree with " << maxDepth << " levels." << std::endl
                                                << "minx = " << theGlobalMinx << std::endl
                                                << "miny = " << theGlobalMiny << std::endl
                                                << "maxx = " << theGlobalMaxx << std::endl
                                                << "maxy = " << theGlobalMaxy << std::endl;
         }
         shpTree = SHPCreateTree(hSHP, 2, maxDepth, minV, maxV);
      }

      if(shpTree)
      {
         SHPWriteTree(shpTree, (theFilename+".qtree").c_str());
         SHPDestroyTree(shpTree);
      }
      SHPClose(hSHP);
   }
   
}

