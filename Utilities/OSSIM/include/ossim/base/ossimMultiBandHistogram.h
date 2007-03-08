//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description: 
//
//*******************************************************************
//  $Id: ossimMultiBandHistogram.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimMultiBandHistogram_HEADER
#define ossimMultiBandHistogram_HEADER
#include <vector>
using namespace std;
#include <ossim/base/ossimFilename.h>

class OSSIMDLLEXPORT ossimHistogram;
class OSSIMDLLEXPORT ossimKeywordlist;

class OSSIMDLLEXPORT ossimMultiBandHistogram
{
public:
   ossimMultiBandHistogram();
   ossimMultiBandHistogram(const ossimMultiBandHistogram& rhs);
   ossimMultiBandHistogram(long numberOfBands,
                           long numberOfBuckets,
                           float minValue,
                           float maxValue);
   virtual ~ossimMultiBandHistogram();

   void create(long numberOfBands,
               long numberOfBuckets,
               float minValue,
               float maxValue);
   
   ossim_uint32 getNumberOfBands() const;

   void create(long numberOfBands);
   void setBinCount(double binNumber, double count);
   ossimHistogram* getHistogram(long band);

   ossimMultiBandHistogram* createAccumulationLessThanEqual()const;
   ossimMultiBandHistogram* createAccumulationGreaterThanEqual()const;
   /*!
    * Imports a text file that has histogram data
    */
   virtual bool importHistogram(const ossimFilename& inputFile);
   virtual bool importHistogram(istream& in);
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
protected:
   class  ossimProprietaryHeaderInformation
   {
   public:
      ossimProprietaryHeaderInformation(){clear();}

      bool parseStream(istream& in);

      ossim_uint32 getNumberOfBands()
         {
            return theNumberOfBands.toULong();
         }
      void clear()
         {
            theFileType      = "";
            theVersion       = "";
            theNumberOfBands = "";
         }
      ossimString theFileType;
      ossimString theVersion;
      ossimString theNumberOfBands;
      
   };
   /*!
    * Holds the histograms for each band
    */
   vector<ossimHistogram*> theHistogramList;

   void deleteHistograms();

   bool parseProprietaryHistogram(istream& in);
                             
};

#endif
