#include "ossimElevationShapeIdx.h"
#include <vec/shape/shapefil.h>

ossimString ossimElevationShapeIdx::theDefaultName = "elevation_idx.shp";

class ossimElevationShapeIdxPrivate : public ossimReferenced
{
public:
   ossimElevationShapeIdxPrivate()
      :theShapefileHandle(0),
       theDbfHandle(0)
      {
      }
   virtual ~ossimElevationShapeIdxPrivate()
      {
         shpClose();
      }
   SHPHandle shpOpen(const ossimFilename& file, bool overWriteFlag)
      {
         shpClose();
         theShapeFilename = file;
         if(overWriteFlag)
         {
            theShapefileHandle = SHPCreate(file.c_str(),
                                           SHPT_POLYGON);
         }
         else
         {
            theShapefileHandle = SHPOpen(file.c_str(),
                                         "r+b");
            if(!theShapefileHandle)
            {
               theShapefileHandle = SHPCreate(file.c_str(),
                                              SHPT_POLYGON);
            }
         }
         dbfOpen(overWriteFlag);
         
         return theShapefileHandle;
      }
   void shpClose()
      {
         if(theShapefileHandle)
         {
            SHPClose(theShapefileHandle);
            theShapefileHandle = 0;  
         }
      }
   SHPHandle shpHandle()
      {
         return theShapefileHandle;
      }

   DBFHandle dbfHandle()
      {
         return theDbfHandle;
      }
   
protected:
   void dbfOpen(bool overWriteFlag)
      {
         dbfClose();

         if(overWriteFlag)
         {
            theDbfHandle = DBFCreate(theShapeFilename.c_str());
            if(theDbfHandle)
            {
               DBFAddField(theDbfHandle, "FILENAME", FTString, 256, 0);
               DBFAddField(theDbfHandle, "MINZ", FTDouble, 32, 17);
               DBFAddField(theDbfHandle, "MAXZ", FTDouble, 32, 17);
            }
         }
         else
         {
            theDbfHandle = DBFOpen(theShapeFilename.c_str(), "r+b");
            if(!theDbfHandle)
            {
               theDbfHandle = DBFCreate(theShapeFilename.c_str());
               if(theDbfHandle)
               {
                  DBFAddField(theDbfHandle, "FILENAME", FTString, 256, 0);
                  DBFAddField(theDbfHandle, "MINZ", FTDouble, 32, 17);
                  DBFAddField(theDbfHandle, "MAXZ", FTDouble, 32, 17);
               }
            }
         }
      }
   void dbfClose()
      {
         if(theDbfHandle)
         {
            DBFClose(theDbfHandle);
            theDbfHandle = 0;
         }
      }
   ossimFilename theShapeFilename;
   SHPHandle   theShapefileHandle;
   DBFHandle   theDbfHandle;
};

ossimElevationShapeIdx::ossimElevationShapeIdx(const ossimFilename& shapeFilename,
                                               bool overWriteFlag)
{
   setFilename(shapeFilename, overWriteFlag);
}

ossimElevationShapeIdx::~ossimElevationShapeIdx()
{
   if(thePrivateData)
   {
      delete thePrivateData;
      thePrivateData = 0;
   }
}

const ossimString& ossimElevationShapeIdx::defaultName()
{
   return theDefaultName;
}

void ossimElevationShapeIdx::setFilename(const ossimFilename& shapeFilename, bool overWriteFlag)throw(std::exception())
{
   if(thePrivateData)
   {
      delete thePrivateData;
      thePrivateData = 0;
   }
   if(shapeFilename == "") return;
   if(shapeFilename.isDir())
   {
      theShapeFilename = shapeFilename.dirCat(theDefaultName);
   }
   else if(shapeFilename.isFile())
   {
      theShapeFilename = shapeFilename;
      theShapeFilename.setExtension("shp");
   }
   else
   {
      theShapeFilename = shapeFilename.path();
      
      theShapeFilename = theShapeFilename.dirCat(theDefaultName);
      theShapeFilename.setExtension("shp");
   }
   thePrivateData = new ossimElevationShapeIdxPrivate();
   
   if(!thePrivateData->shpOpen(theShapeFilename, overWriteFlag))
   {
      thePrivateData = 0;
      throw(("ossimElevationShapeIdx::setFilename: Unable to setup index file " + theShapeFilename).c_str());
   }
   double minBounds[4];
   double maxBounds[4];
   int nEntities = 0;
   int shapeType = SHPT_NULL;
   SHPGetInfo(thePrivateData->shpHandle(),
              &nEntities,
              &shapeType,
              minBounds,
              maxBounds);
   if(nEntities < 0)
   {
      // set it up for the adds
      //
      theGlobalMinx = 180.0;
      theGlobalMiny = 90.0;
      theGlobalMaxx = -180.0;
      theGlobalMaxy = -90.0;
   }
   else
   {
      theGlobalMinx = minBounds[0];
      theGlobalMiny = minBounds[1];
      theGlobalMaxx = maxBounds[0];
      theGlobalMaxy = maxBounds[1];
   }
}

void ossimElevationShapeIdx::add(const ossimFilename& filename,
                                 ossim_float64 minx, // decimal degrees
                                 ossim_float64 miny, // decimal degrees
                                 ossim_float64 minz, // meters
                                 ossim_float64 maxx, // decimal degrees
                                 ossim_float64 maxy, // decimal degrees
                                 ossim_float64 maxz )// meters
{
   if(!thePrivateData) return;
   int iRecord = DBFGetRecordCount(thePrivateData->dbfHandle());
   double* x = new double[8];
   double* y = new double[8];
   int nVertices = 4;
   x[0] = minx;
   x[1] = maxx;
   x[2] = maxx;
   x[3] = minx;
   
   y[0] = maxy;
   y[1] = maxy;
   y[2] = miny;
   y[3] = miny;
   

   if(minx < theGlobalMinx)
   {
      theGlobalMinx = minx;
   }
   if(miny < theGlobalMiny)
   {
      theGlobalMiny = miny;
   }
   if(maxx > theGlobalMaxx)
   {
      theGlobalMaxx = maxx;
   }
   if(maxy > theGlobalMaxy)
   {
      theGlobalMaxy = maxy;
   }
   SHPObject* obj = SHPCreateSimpleObject(SHPT_POLYGON,
                                          nVertices,
                                          x,
                                          y,
                                          0);

   SHPWriteObject( thePrivateData->shpHandle(),
                   -1,
                   obj );
   DBFWriteStringAttribute(thePrivateData->dbfHandle(),
                           iRecord,
                           0,
                           filename.c_str());
   DBFWriteDoubleAttribute(thePrivateData->dbfHandle(),
                           iRecord,
                           0,
                           (double)minz);
   DBFWriteDoubleAttribute(thePrivateData->dbfHandle(),
                           iRecord,
                           0,
                           (double)maxz);
}

void ossimElevationShapeIdx::buildQuadTree(int maxDepth)
{
   if(!thePrivateData) return;
   
   ossimFilename qTree = theShapeFilename;

   qTree.setExtension("qtree");

   if(thePrivateData->shpHandle())
   {
      double minBounds[4] = {0.0,0.0,0.0,0.0};
      double maxBounds[4] = {0.0,0.0,0.0,0.0};

      minBounds[0] = theGlobalMinx;
      minBounds[1] = theGlobalMiny;
      maxBounds[0] = theGlobalMaxx;
      maxBounds[1] = theGlobalMaxy;
      
      SHPTree* tree = SHPCreateTree(thePrivateData->shpHandle(),
                                    2,
                                    maxDepth,
                                    minBounds,
                                    maxBounds);
      if(tree)
      {
         SHPWriteTree(tree,
                      qTree.c_str());
         
         SHPDestroyTree(tree);
      }
   }
}
