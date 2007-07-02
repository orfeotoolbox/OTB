//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageSourceHistogramFilter.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageSourceHistogramFilter_HEADER
#define ossimImageSourceHistogramFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimFilename.h>
class ossimMultiResLevelHistogram;

class OSSIMDLLEXPORT ossimImageSourceHistogramFilter : public ossimImageSourceFilter
{
public:
   ossimImageSourceHistogramFilter();
   ossimImageSourceHistogramFilter(ossimImageSource* inputSource,
                        ossimMultiResLevelHistogram* histogram);
   virtual ~ossimImageSourceHistogramFilter();
   virtual void setHistogram(ossimMultiResLevelHistogram* histogram);
   virtual bool setHistogram(const ossimFilename& filename);
   virtual const ossimFilename& getHistogramFilename()const;
   virtual ossimMultiResLevelHistogram* getHistogram();
   virtual const ossimMultiResLevelHistogram* getHistogram()const;
   
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const;

   virtual void connectInputEvent(ossimConnectionEvent& event);
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = NULL)const;

   virtual std::ostream& print(std::ostream& out) const;

protected:
   long                         theCurrentResLevel;

private:
   ossimMultiResLevelHistogram* theHistogram;
   ossimFilename                theFilename;
   
TYPE_DATA
};

#endif /* #ifndef ossimImageSourceHistogramFilter_HEADER */
