#ifndef ossimVpfAnnotationSource_HEADER
#define ossimVpfAnnotationSource_HEADER
#include "vec/vpf/ossimVpfDatabase.h"
#include "imaging/formats/ossimImageHandler.h"
#include "base/data_types/ossimIrect.h"
#include "imaging/annotation/ossimGeoAnnotationSource.h"
#include "projections/ossimProjection.h"
#include "ossimVpfAnnotationLibraryInfo.h"
class ossimVpfAnnotationFeatureInfo;
class ossimVpfAnnotationSource : public ossimGeoAnnotationSource
{
public:
   ossimVpfAnnotationSource();
   virtual ~ossimVpfAnnotationSource();
   virtual bool open();
   virtual bool open(const ossimFilename& file);
   virtual bool isOpen()const;

   virtual void close();
   virtual ossimFilename getFilename()const;
  virtual void computeBoundingRect();

   virtual void transformObjects(ossimProjection* projection=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);

   virtual void drawAnnotations(ossimRefPtr<ossimImageData> tile);
   virtual void getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& features);
  virtual void setAllFeatures(const std::vector<ossimVpfAnnotationFeatureInfo*>& features);
 
protected:
   ossimVpfDatabase theDatabase;
   ossimFilename    theFilename;
   std::vector<ossimVpfAnnotationLibraryInfo*> theLibraryInfo;
   std::vector<ossimVpfAnnotationFeatureInfo*> theFeatureList;

  void deleteAllLibraries();
  bool openDatabase(const ossimFilename& file);
};

#endif
