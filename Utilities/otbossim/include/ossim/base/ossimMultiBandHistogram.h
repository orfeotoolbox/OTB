//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description: 
//
//*******************************************************************
//  $Id: ossimMultiBandHistogram.h 17205 2010-04-24 18:10:01Z dburken $
#ifndef ossimMultiBandHistogram_HEADER
#define ossimMultiBandHistogram_HEADER
#include <vector>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimReferenced.h>

class ossimKeywordlist;
class ossimImageSource;

class OSSIMDLLEXPORT ossimMultiBandHistogram : public ossimReferenced
{
public:
   ossimMultiBandHistogram();
   ossimMultiBandHistogram(const ossimMultiBandHistogram& rhs);
   ossimMultiBandHistogram(ossim_int32 numberOfBands,
                           ossim_int32 numberOfBuckets,
                           float minValue,
                           float maxValue);

   void create(const ossimImageSource* input);
   
   void create(ossim_int32 numberOfBands,
               ossim_int32 numberOfBuckets,
               float minValue,
               float maxValue);
   
   ossim_uint32 getNumberOfBands() const;

   void create(ossim_int32 numberOfBands);
   void setBinCount(double binNumber, double count);
   ossimRefPtr<ossimHistogram> getHistogram(ossim_int32 band);
   const ossimRefPtr<ossimHistogram> getHistogram(ossim_int32 band)const;

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
   virtual ~ossimMultiBandHistogram();
   class  ossimProprietaryHeaderInformation
   {
   public:
      ossimProprietaryHeaderInformation(){clear();}

      bool parseStream(std::istream& in);

      ossim_uint32 getNumberOfBands() const;

      void clear();

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
