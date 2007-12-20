#ifndef ossimElevationShapeIdx_HEADER
#define ossimElevationShapeIdx_HEADER
#include <exception>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimFilename.h>

class ossimElevationShapeIdxPrivate;

class ossimElevationShapeIdx : public ossimObject
{
public:
   ossimElevationShapeIdx(const ossimFilename& shapeFilename = "",
                          bool overWriteFlag = false);
   virtual ~ossimElevationShapeIdx();
   void setFilename(const ossimFilename& file, bool overWriteFlag=false);

   void add(const ossimFilename& filename,
            ossim_float64 minx,  // decimal degrees
            ossim_float64 miny,  // decimal degrees
            ossim_float64 minz,  // height in meters meters 
            ossim_float64 maxx,  // decimal degrees
            ossim_float64 maxy,  // decimal degrees
            ossim_float64 maxz); // height in meters meters

   void buildQuadTree(int maxDepth=0);
   static const ossimString& defaultName();

protected:
   static ossimString theDefaultName;
   ossimElevationShapeIdxPrivate* thePrivateData;

   ossimFilename theShapeFilename;
   ossim_float64 theGlobalMinx;
   ossim_float64 theGlobalMiny;
   ossim_float64 theGlobalMaxx;
   ossim_float64 theGlobalMaxy;
   
};

#endif
