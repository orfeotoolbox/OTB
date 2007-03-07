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
//  $Id: ossimMultiResLevelHistogram.h,v 1.10 2003/09/03 14:19:30 dburken Exp $
#ifndef ossimMultiResLevelHistogram_HEADER
#define ossimMultiResLevelHistogram_HEADER
#include <vector>
using namespace std;
#include "ossimFilename.h"
#include "ossimKeywordlist.h"

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
