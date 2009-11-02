#ifndef ossimVpfAnnotationSource_HEADER
#define ossimVpfAnnotationSource_HEADER
#include <ossim/vec/ossimVpfDatabase.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimGeoAnnotationSource.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/imaging/ossimVpfAnnotationLibraryInfo.h>
class ossimVpfAnnotationFeatureInfo;
class ossimVpfAnnotationSource : public ossimGeoAnnotationSource
{
public:
   ossimVpfAnnotationSource();
   virtual bool open();
   virtual bool open(const ossimFilename& file);
   virtual bool isOpen()const;

   virtual void close();
   virtual ossimFilename getFilename()const;
  virtual void computeBoundingRect();

   virtual void transformObjects(ossimImageGeometry* geom=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);

   virtual void drawAnnotations(ossimRefPtr<ossimImageData> tile);
   virtual void getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& features);
  virtual void setAllFeatures(const std::vector<ossimVpfAnnotationFeatureInfo*>& features);
 
protected:
   virtual ~ossimVpfAnnotationSource();
   
   ossimVpfDatabase theDatabase;
   ossimFilename    theFilename;
   std::vector<ossimVpfAnnotationLibraryInfo*> theLibraryInfo;
   std::vector<ossimVpfAnnotationFeatureInfo*> theFeatureList;

  void deleteAllLibraries();
  bool openDatabase(const ossimFilename& file);
};

#endif
