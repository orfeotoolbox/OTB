//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description: 
//
//*******************************************************************
//  $Id: ossimMultiResLevelHistogram.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimMultiResLevelHistogram_HEADER
#define ossimMultiResLevelHistogram_HEADER
#include <vector>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>

class OSSIMDLLEXPORT ossimMultiResLevelHistogram : public ossimReferenced
{
public:
   ossimMultiResLevelHistogram();
   ossimMultiResLevelHistogram(ossim_uint32 numberOfResLevels);
   ossimMultiResLevelHistogram(const ossimMultiResLevelHistogram& rhs);
   

   ossimRefPtr<ossimHistogram> getHistogram(ossim_uint32 band,
                                            ossim_uint32 resLevel=0);
   const ossimRefPtr<ossimHistogram> getHistogram(ossim_uint32 band,
                                                  ossim_uint32 resLevel=0)const;
   ossim_uint32 getNumberOfResLevels()const;
   ossim_uint32 getNumberOfBands(ossim_uint32 resLevel=0) const;
   void create(ossim_uint32 numberOfResLevels);
   ossimRefPtr<ossimMultiBandHistogram> getMultiBandHistogram(ossim_uint32 resLevel) const;
   ossimRefPtr<ossimMultiResLevelHistogram> createAccumulationLessThanEqual()const;
   ossimRefPtr<ossimMultiResLevelHistogram> createAccumulationGreaterThanEqual()const;

   void setBinCount(double binNumber, double count);
   /*!
    * Will append to the list the passed in histogram.
    */
   virtual void addHistogram(ossimMultiBandHistogram* histo);

   /*!
    * This will create and append a histogram to the
    * list and return a pointer to the new histogram.
    */
   virtual ossimRefPtr<ossimMultiBandHistogram> addHistogram();

   /*!
    * Will set the histogram to the passed in res level.
    * It will delete the histogram at the res level before
    * setting the pointer to the passed in histogram.  if
    * the resLevel is out of range it will return false meaning
    * it was unable to assign the pointer.
    */
   virtual bool setHistogram(ossimRefPtr<ossimMultiBandHistogram> histo, ossim_uint32 resLevel);

   virtual ossimFilename getHistogramFile() const;
   virtual bool importHistogram(const ossimFilename& inputFile);
   virtual bool importHistogram(std::istream& in);

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
protected:
   virtual ~ossimMultiResLevelHistogram();
  class  ossimProprietaryHeaderInformation
   {
   public:
      ossimProprietaryHeaderInformation(){clear();}

      bool parseStream(std::istream& in);

      ossim_uint32 getNumberOfResLevels()
         {
            return theNumberOfResLevels.toUInt32();
         }
      void clear()
         {
            theFileType          = "";
            theVersion           = "";
            theCreatorId         = "";
            theNumberOfResLevels = "";
         }
      ossimString theFileType;
      ossimString theVersion;
      ossimString theCreatorId;
      ossimString theNumberOfResLevels;
   };

   std::vector<ossimRefPtr<ossimMultiBandHistogram> > theHistogramList;
   ossimFilename                    theHistogramFile;
   
   void deleteHistograms();
   bool parseProprietaryHistogram(std::istream& in);
};

#endif
