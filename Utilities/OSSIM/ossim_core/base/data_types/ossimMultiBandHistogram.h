//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description: 
//
//*******************************************************************
//  $Id: ossimMultiBandHistogram.h,v 1.11 2003/08/22 20:50:46 dburken Exp $
#ifndef ossimMultiBandHistogram_HEADER
#define ossimMultiBandHistogram_HEADER
#include <vector>
using namespace std;
#include "ossimFilename.h"

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
