#ifndef ossimImageShapeFileIndex_HEADER
#define ossimImageShapeFileIndex_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimFilename.h>

class OSSIM_DLL ossimImageShapeFileIndex : public ossimObject
{
public:
   ossimImageShapeFileIndex(const ossimFilename& shapeFilename = "");
   void setFilename(const ossimFilename& shapeFilename);
   
   void add(const ossimFilename& filename,
            const ossim_int32 entry=-1, // default to all entries
            const ossimString& groupName="RASTER");
   /**
    *
    */ 
   void build(ossim_uint32 maxDepth=1)const;
   
protected:
   class ossimImageShapeFileIndexRecord : public ossimReferenced
   {
   public:
      ossimImageShapeFileIndexRecord(const ossimFilename& filename="",
                                     ossim_uint32 entry=0,
                                     const ossimString& groupName="",
                                     ossim_float64 minx=0.0,
                                     ossim_float64 miny=0.0,
                                     ossim_float64 maxx=0.0,
                                     ossim_float64 maxy=0.0)
         :theFilename(filename),
         theEntry(entry),
         theGroupName(groupName),
         theMinx(minx),
         theMiny(miny),
         theMaxx(maxx),
         theMaxy(maxy)
      {
      }

      ossimFilename theFilename;
      ossim_uint32  theEntry;
      ossimString   theGroupName;
      ossim_float64 theMinx;
      ossim_float64 theMiny;
      ossim_float64 theMaxx;
      ossim_float64 theMaxy;
   };

   std::vector<ossimRefPtr<ossimImageShapeFileIndexRecord> > theRecords;

   ossimFilename theFilename;
   ossim_float64 theGlobalMinx;
   ossim_float64 theGlobalMiny;
   ossim_float64 theGlobalMaxx;
   ossim_float64 theGlobalMaxy;
};

#endif
