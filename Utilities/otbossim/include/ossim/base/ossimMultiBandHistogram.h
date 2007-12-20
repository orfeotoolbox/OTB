//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description: 
//
//*******************************************************************
//  $Id: ossimMultiBandHistogram.h 11724 2007-09-13 19:28:07Z gpotts $
#ifndef ossimMultiBandHistogram_HEADER
#define ossimMultiBandHistogram_HEADER
#include <vector>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimReferenced.h>

class OSSIMDLLEXPORT ossimHistogram;
class OSSIMDLLEXPORT ossimKeywordlist;

class OSSIMDLLEXPORT ossimMultiBandHistogram : public ossimReferenced
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
   ossimRefPtr<ossimHistogram> getHistogram(long band);
   const ossimRefPtr<ossimHistogram> getHistogram(long band)const;

   ossimRefPtr<ossimMultiBandHistogram> createAccumulationLessThanEqual()const;
   ossimRefPtr<ossimMultiBandHistogram> createAccumulationGreaterThanEqual()const;
   /*!
    * Imports a text file that has histogram data
    */
   virtual bool importHistogram(const ossimFilename& inputFile);
   virtual bool importHistogram(std::istream& in);
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
   virtual bool saveState(ossimRefPtr<ossimXmlNode> xmlNode)const;
   virtual bool loadState(const ossimRefPtr<ossimXmlNode> xmlNode);
   
protected:
   class  ossimProprietaryHeaderInformation
   {
   public:
      ossimProprietaryHeaderInformation(){clear();}

      bool parseStream(std::istream& in);

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
   std::vector<ossimRefPtr<ossimHistogram> > theHistogramList;

   void deleteHistograms();

   bool parseProprietaryHistogram(std::istream& in);
                             
};

#endif
