//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimLandsatTopoCorrectionFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimLandsatTopoCorrectionFilter_HEADER
#define ossimLandsatTopoCorrectionFilter_HEADER
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimTopographicCorrectionFilter.h>

class OSSIM_DLL ossimLandsatTopoCorrectionFilter : public ossimTopographicCorrectionFilter
{
public:
   ossimLandsatTopoCorrectionFilter();
   ossimLandsatTopoCorrectionFilter(ossimImageSource* colorSource,
                                    ossimImageSource* elevSource,
                                    const ossimFilename& landsatFileName=ossimFilename(""));
   virtual void initialize();
   virtual void setLandsatHeader(const ossimFilename& header);
   virtual ossimFilename getLandsatHeader()const
      {
         return theLandsatHeader;
      }
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   virtual ~ossimLandsatTopoCorrectionFilter();
   ossimFilename theLandsatHeader;
   ossimFilename findLandsatHeader();
//   virtual void computeC();
TYPE_DATA
};
#endif
