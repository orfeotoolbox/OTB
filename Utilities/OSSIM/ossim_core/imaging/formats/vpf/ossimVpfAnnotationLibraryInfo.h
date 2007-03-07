#ifndef ossimVpfAnnotationLibraryInfo_HEADER
#define ossimVpfAnnotationLibraryInfo_HEADER
#include <map>
#include "base/data_types/ossimString.h"
#include "ossimVpfAnnotationCoverageInfo.h"

class ossimVpfDatabase;
class ossimRgbImage;
class ossimVpfAnnotationFeatureInfo;

class OSSIMDLLEXPORT ossimVpfAnnotationLibraryInfo
{
public:
  ossimVpfAnnotationLibraryInfo(const ossimString& libraryName = "",
				bool enabledFlag=true);

  virtual ~ossimVpfAnnotationLibraryInfo();

  bool getEnabledFlag()const;

  void setEnabledFlag(bool flag);

  const ossimString& getName()const;

  void setName(const ossimString& libraryName);

  void setDatabase(ossimVpfDatabase* database);

  ossimVpfDatabase* getDatabase();

  ossimIrect getBoundingProjectedRect()const;
  
  void transform(ossimProjection* proj);
  void buildLibrary();
  void buildLibrary(const ossimString& coverageName, const ossimString& feature);
  void deleteAllCoverage();
  void drawAnnotations(ossimRgbImage* tile);
  void getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& features);

  virtual bool saveState(ossimKeywordlist& kwl,
			 const char* prefix=0)const;
  virtual bool loadState(const ossimKeywordlist& kwl,
			 const char* prefix=0);
  
protected:
   ossimString theName;
   bool theEnabledFlag;
   ossimVpfDatabase* theDatabase;
   std::vector<ossimVpfAnnotationCoverageInfo*> theCoverageLayerList;
};

#endif
