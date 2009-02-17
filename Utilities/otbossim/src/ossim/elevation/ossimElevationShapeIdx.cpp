#include <ossim/elevation/ossimElevationShapeIdx.h>
#include <ossim/vec/shapefil.h>

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
   ossim_SHPHandle shpOpen(const ossimFilename& file, bool overWriteFlag)
      {
         shpClose();
         theShapeFilename = file;
         if(overWriteFlag)
         {
            theShapefileHandle = ossim_SHPCreate(file.c_str(),
                                           ossim_SHPT_POLYGON);
         }
         else
         {
            theShapefileHandle = ossim_SHPOpen(file.c_str(),
                                         "r+b");
            if(!theShapefileHandle)
            {
               theShapefileHandle = ossim_SHPCreate(file.c_str(),
                                              ossim_SHPT_POLYGON);
            }
         }
         dbfOpen(overWriteFlag);
         
         return theShapefileHandle;
      }
   void shpClose()
      {
         if(theShapefileHandle)
         {
            ossim_SHPClose(theShapefileHandle);
            theShapefileHandle = 0;  
         }
      }
   ossim_SHPHandle shpHandle()
      {
         return theShapefileHandle;
      }

   ossim_DBFHandle dbfHandle()
      {
         return theDbfHandle;
      }
   
protected:
   void dbfOpen(bool overWriteFlag)
      {
         dbfClose();

         if(overWriteFlag)
         {
            theDbfHandle = ossim_DBFCreate(theShapeFilename.c_str());
            if(theDbfHandle)
            {
               ossim_DBFAddField(theDbfHandle, "FILENAME", FTString, 256, 0);
               ossim_DBFAddField(theDbfHandle, "MINZ", FTDouble, 32, 17);
               ossim_DBFAddField(theDbfHandle, "MAXZ", FTDouble, 32, 17);
            }
         }
         else
         {
            theDbfHandle = ossim_DBFOpen(theShapeFilename.c_str(), "r+b");
            if(!theDbfHandle)
            {
               theDbfHandle = ossim_DBFCreate(theShapeFilename.c_str());
               if(theDbfHandle)
               {
                  ossim_DBFAddField(theDbfHandle, "FILENAME", FTString, 256, 0);
                  ossim_DBFAddField(theDbfHandle, "MINZ", FTDouble, 32, 17);
                  ossim_DBFAddField(theDbfHandle, "MAXZ", FTDouble, 32, 17);
               }
            }
         }
      }
   void dbfClose()
      {
         if(theDbfHandle)
         {
            ossim_DBFClose(theDbfHandle);
            theDbfHandle = 0;
         }
      }
   ossimFilename theShapeFilename;
   ossim_SHPHandle   theShapefileHandle;
   ossim_DBFHandle   theDbfHandle;
};

ossimElevationShapeIdx::ossimElevationShapeIdx(const ossimFilename& shapeFilename,
                                               bool overWriteFlag)
   :thePrivateData(0)
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

void ossimElevationShapeIdx::setFilename(const ossimFilename& shapeFilename, bool overWriteFlag)
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
   int shapeType = ossim_SHPT_NULL;
   ossim_SHPGetInfo(thePrivateData->shpHandle(),
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
   int iRecord = ossim_DBFGetRecordCount(thePrivateData->dbfHandle());
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
   ossim_SHPObject* obj = ossim_SHPCreateSimpleObject(ossim_SHPT_POLYGON,
                                          nVertices,
                                          x,
                                          y,
                                          0);

   ossim_SHPWriteObject( thePrivateData->shpHandle(),
                   -1,
                   obj );
   ossim_DBFWriteStringAttribute(thePrivateData->dbfHandle(),
                           iRecord,
                           0,
                           filename.c_str());
   ossim_DBFWriteDoubleAttribute(thePrivateData->dbfHandle(),
                           iRecord,
                           0,
                           (double)minz);
   ossim_DBFWriteDoubleAttribute(thePrivateData->dbfHandle(),
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
      
      ossim_SHPTree* tree = ossim_SHPCreateTree(thePrivateData->shpHandle(),
                                    2,
                                    maxDepth,
                                    minBounds,
                                    maxBounds);
      if(tree)
      {
         ossim_SHPWriteTree(tree,
                      qTree.c_str());
         
         ossim_SHPDestroyTree(tree);
      }
   }
}
