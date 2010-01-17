//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageSourceHistogramFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimImageSourceHistogramFilter_HEADER
#define ossimImageSourceHistogramFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>

class OSSIMDLLEXPORT ossimImageSourceHistogramFilter : public ossimImageSourceFilter
{
public:
   ossimImageSourceHistogramFilter();
   ossimImageSourceHistogramFilter(ossimImageSource* inputSource,
                        ossimRefPtr<ossimMultiResLevelHistogram> histogram);
   virtual void setHistogram(ossimRefPtr<ossimMultiResLevelHistogram> histogram);
   virtual bool setHistogram(const ossimFilename& filename);
   virtual const ossimFilename& getHistogramFilename()const;
   virtual ossimRefPtr<ossimMultiResLevelHistogram> getHistogram();
   virtual const ossimRefPtr<ossimMultiResLevelHistogram> getHistogram()const;
   
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const;

   virtual void connectInputEvent(ossimConnectionEvent& event);
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = NULL)const;

   virtual std::ostream& print(std::ostream& out) const;

protected:
   virtual ~ossimImageSourceHistogramFilter();
   ossim_int32                         theCurrentResLevel;

private:
   ossimRefPtr<ossimMultiResLevelHistogram> theHistogram;
   ossimFilename                theFilename;
   
TYPE_DATA
};

#endif /* #ifndef ossimImageSourceHistogramFilter_HEADER */
