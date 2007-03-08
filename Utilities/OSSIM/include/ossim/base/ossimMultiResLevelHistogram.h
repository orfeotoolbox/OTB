//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description: 
//
//*******************************************************************
//  $Id: ossimMultiResLevelHistogram.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimMultiResLevelHistogram_HEADER
#define ossimMultiResLevelHistogram_HEADER
#include <vector>
using namespace std;
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>

class OSSIMDLLEXPORT ossimMultiBandHistogram;
class OSSIMDLLEXPORT ossimHistogram;

class OSSIMDLLEXPORT ossimMultiResLevelHistogram
{
public:
   ossimMultiResLevelHistogram();
   ossimMultiResLevelHistogram(long numberOfResLevels);
   ossimMultiResLevelHistogram(const ossimMultiResLevelHistogram& rhs);
   
   virtual ~ossimMultiResLevelHistogram();

   ossimHistogram* getHistogram(long band,
                                long resLevel=0);
   long getNumberOfResLevels()const;
   ossim_uint32 getNumberOfBands(long resLevel=0) const;
   void create(long numberOfResLevels);
   ossimMultiBandHistogram* getMultiBandHistogram(long resLevel) const;
   ossimMultiResLevelHistogram* createAccumulationLessThanEqual()const;
   ossimMultiResLevelHistogram* createAccumulationGreaterThanEqual()const;

   void setBinCount(double binNumber, double count);
   /*!
    * Will append to the list the passed in histogram.
    */
   virtual void addHistogram(ossimMultiBandHistogram* histo);

   /*!
    * This will create and append a histogram to the
    * list and return a pointer to the new histogram.
    */
   virtual ossimMultiBandHistogram* addHistogram();

   /*!
    * Will set the histogram to the passed in res level.
    * It will delete the histogram at the res level before
    * setting the pointer to the passed in histogram.  if
    * the resLevel is out of range it will return false meaning
    * it was unable to assign the pointer.
    */
   virtual bool setHistogram(ossimMultiBandHistogram* histo, long resLevel);

   virtual ossimFilename getHistogramFile() const;
   virtual bool importHistogram(const ossimFilename& inputFile);
   virtual bool importHistogram(istream& in);

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
protected:
   class  ossimProprietaryHeaderInformation
   {
   public:
      ossimProprietaryHeaderInformation(){clear();}

      bool parseStream(istream& in);

      long getNumberOfResLevels()
         {
            return theNumberOfResLevels.toLong();
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

   vector<ossimMultiBandHistogram*> theHistogramList;
   ossimFilename                    theHistogramFile;
   
   void deleteHistograms();
   bool parseProprietaryHistogram(istream& in);
};

#endif
