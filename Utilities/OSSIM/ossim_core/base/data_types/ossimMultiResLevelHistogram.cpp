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
//  $Id: ossimMultiResLevelHistogram.cpp,v 1.17 2003/09/09 18:33:57 dburken Exp $
#include "ossimMultiResLevelHistogram.h"
#include "ossimMultiBandHistogram.h"
#include "ossimKeyword.h"
#include "ossimKeywordlist.h"
#include "base/common/ossimKeywordNames.h"
#include <fstream>
using namespace std;

static const ossimKeyword NUMBER_OF_RES_LEVELS("number_of_res_levels",
                                               "");

ossimMultiResLevelHistogram::ossimMultiResLevelHistogram()
   :
      theHistogramList(),
      theHistogramFile()
{  
}

ossimMultiResLevelHistogram::ossimMultiResLevelHistogram(long numberOfResLevels)
   :
      theHistogramList(),
      theHistogramFile()
{
   create(numberOfResLevels);
}

ossimMultiResLevelHistogram::ossimMultiResLevelHistogram(const ossimMultiResLevelHistogram& rhs)
   :
      theHistogramList(),
      theHistogramFile()
{
   
   create(rhs.getNumberOfResLevels());
   for(ossim_uint32 i = 0; i < theHistogramList.size(); ++i)
   {
      theHistogramList[0] = rhs.theHistogramList[i]?
                            new ossimMultiBandHistogram(*rhs.theHistogramList[i]):
                            (ossimMultiBandHistogram*)NULL;
   }
}

ossimMultiResLevelHistogram::~ossimMultiResLevelHistogram()
{
   deleteHistograms();
}

void ossimMultiResLevelHistogram::create(long numberOfResLevels)
{
   deleteHistograms();

   for(long index = 0; index < numberOfResLevels; ++index)
   {
      theHistogramList.push_back(new ossimMultiBandHistogram);      
   }
}

ossimMultiResLevelHistogram* ossimMultiResLevelHistogram::createAccumulationLessThanEqual()const
{
   ossimMultiResLevelHistogram* result = NULL;

   if(theHistogramList.size() > 0)
   {
      result = new ossimMultiResLevelHistogram(theHistogramList.size());

      for(long index=0; index < (long) theHistogramList.size(); ++ index)
      {
         if(theHistogramList[index])
         {
            ossimMultiBandHistogram* multiBandAccumulator = theHistogramList[index]->createAccumulationLessThanEqual();
            result->theHistogramList[index] = multiBandAccumulator;
         }
         else
         {
            // we will push on a null since someone could
            // turn off a band.  A null accumulation will
            // indicate no histogram data.
            //
            result->theHistogramList[index] = NULL;
         }
      }
      
      if(result->theHistogramList.size() < 1)
      {
         delete result;
      }
   }
   
   return result;
}

void ossimMultiResLevelHistogram::setBinCount(double binNumber, double count)
{
   if(theHistogramList.size() > 0)
   {
      for(long index=0; index < (long) theHistogramList.size(); ++ index)
      {
         if(theHistogramList[index])
         {
            theHistogramList[index]->setBinCount(binNumber, count);
         }
      }
   }   
}

ossimMultiResLevelHistogram* ossimMultiResLevelHistogram::createAccumulationGreaterThanEqual()const
{
   ossimMultiResLevelHistogram* result = NULL;

   if(theHistogramList.size() > 0)
   {
      result = new ossimMultiResLevelHistogram(theHistogramList.size());

      for(long index=0; index < (long) theHistogramList.size(); ++ index)
      {
         if(theHistogramList[index])
         {
            ossimMultiBandHistogram* multiBandAccumulator = theHistogramList[index]->createAccumulationGreaterThanEqual();
            result->theHistogramList[index]=multiBandAccumulator;
         }
         else
         {
            // we will push on a null since someone could
            // turn off a band.  A null accumulation will
            // indicate no histogram data.
            //
            result->theHistogramList[index] = NULL;
         }
      }
      
      if(result->theHistogramList.size() < 1)
      {
         delete result;
      }
   }
   
   return result;
}

void ossimMultiResLevelHistogram::addHistogram(ossimMultiBandHistogram* histo)
{
   theHistogramList.push_back(histo);
}

ossimMultiBandHistogram* ossimMultiResLevelHistogram::addHistogram()
{
   ossimMultiBandHistogram* result = new ossimMultiBandHistogram;
   theHistogramList.push_back(result);
   return result;
}

bool ossimMultiResLevelHistogram::setHistogram(ossimMultiBandHistogram* histo, long resLevel)
{
   if( (resLevel >= 0) && (resLevel < getNumberOfResLevels()))
   {
      if(theHistogramList[resLevel])
      {
         delete theHistogramList[resLevel];
      }
      theHistogramList[resLevel] = histo;
      return true;
   }
   return false;
}

void ossimMultiResLevelHistogram::deleteHistograms()
{
   for(long index = 0; index < (long)theHistogramList.size(); ++index)
   {
      if(theHistogramList[index])
      {
         delete theHistogramList[index];
         theHistogramList[index] = NULL;
      }
   }
   
   theHistogramList.clear();
}

ossimHistogram* ossimMultiResLevelHistogram::getHistogram(long band,
                                                          long resLevel)
{
   ossimMultiBandHistogram* temp = getMultiBandHistogram(resLevel);
   if(temp)
   {
      return temp->getHistogram(band);
   }
   return NULL;
}

long ossimMultiResLevelHistogram::getNumberOfResLevels()const
{
   return (long)theHistogramList.size();
}

ossim_uint32 ossimMultiResLevelHistogram::getNumberOfBands(long resLevel) const
{
   const ossimMultiBandHistogram* h = getMultiBandHistogram(resLevel);
   if (h)
   {
      return h->getNumberOfBands();
   }

   return 0;
}  

ossimMultiBandHistogram* ossimMultiResLevelHistogram::getMultiBandHistogram(long resLevel) const
{
   if((resLevel >=0) &&(resLevel < (long)theHistogramList.size()))
   {
      return theHistogramList[resLevel];
   }

   return NULL;
}

bool ossimMultiResLevelHistogram::importHistogram(istream& in)
{
   ossimString buffer;
   getline(in, buffer);
   // check to see if it is a proprietary histogram file
   // 
   if((buffer =="") || (buffer.c_str()[0] != 'F' ||
      buffer.c_str()[1] != 'i'))
   {
      in.seekg(0, ios::beg);
      ossimKeywordlist kwl;
      kwl.parseStream(in);
      return loadState(kwl);

   }
   ossimProprietaryHeaderInformation header;
   in.seekg(0, ios::beg);
   deleteHistograms();
   if(header.parseStream(in))
   {
      long numberOfResLevels = header.getNumberOfResLevels();
      
      if(numberOfResLevels)
      {
         theHistogramList.resize(numberOfResLevels);

         for(long counter = 0; counter < (long)theHistogramList.size(); ++counter)
         {
            theHistogramList[counter] = NULL;
         }
         ossimString reslevelBuffer;
         ossimString buffer;
         
         for(long index = 0; index < numberOfResLevels; ++index)
         {
            getline(in, buffer);
            if(buffer.find("RR Level") != string::npos)
            {
               unsigned long offset = buffer.find(":");
               if(offset != string::npos)
               {
                  reslevelBuffer = buffer.substr(offset+1);
               }
               else
               {
                  deleteHistograms();
                  return false;
               }
            }
            else
            {
               deleteHistograms();
               return false;
            }
            long resLevelIndex = reslevelBuffer.toLong();

            if(resLevelIndex < (long)theHistogramList.size())
            {
               if(!theHistogramList[resLevelIndex])
               {
                  ossimMultiBandHistogram* histogram = new ossimMultiBandHistogram;
                  if(histogram->importHistogram(in))
                  {
                     theHistogramList[resLevelIndex] = histogram;
                  }
                  else
                  {
                     delete histogram;
                     histogram = NULL;
                     
                     deleteHistograms();
                     return false;
                  }
               }
            }
            else
            {
               deleteHistograms();
               return false;
            }
            ossimString skipDot;
            getline(in, skipDot);
         }
      }
      else
      {
         return false;
      }
   }  

   return true;
}

bool ossimMultiResLevelHistogram::importHistogram(const ossimFilename& file)
{
   if(file.exists())
   {
      theHistogramFile = file;
      
      ifstream input(file.c_str());

      return importHistogram(input);
   }

   return false;
}

bool ossimMultiResLevelHistogram::ossimProprietaryHeaderInformation::parseStream(istream& in)
{
   ossimString inputLine;

   getline(in, inputLine);  
   if(inputLine.find("File Type") != string::npos)
   {
      unsigned long index = inputLine.find(":");
      if(index != string::npos)
      {
         theFileType = inputLine.substr(index+1);
         theFileType = theFileType.trim();
      }
      else
      {
         return false;
      }

   }
   else
   {
      return false;
   }

   getline(in, inputLine);  
   if(inputLine.find("Version") != string::npos)
   {
      unsigned long index = inputLine.find(":");
      if(index != string::npos)
      {
         theVersion = inputLine.substr(index+1);
         theVersion = theVersion.trim();
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }

   getline(in, inputLine);  
   if(inputLine.find("Creator ID") != string::npos)
   {
      unsigned long index = inputLine.find(":");
      if(index != string::npos)
      {
         theCreatorId = inputLine.substr(index+1);
         theCreatorId = theCreatorId.trim();
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }
   
   getline(in, inputLine);  
   if(inputLine.find("RR Levels") != string::npos)
   {
      unsigned long index = inputLine.find(":");
      if(index != string::npos)
      {
         theNumberOfResLevels = inputLine.substr(index+1);
         theNumberOfResLevels = theNumberOfResLevels.trim();
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }
   
   return true;
}

bool ossimMultiResLevelHistogram::saveState(ossimKeywordlist& kwl,
                                            const char* prefix)const
{
   bool result = true;
   if(theHistogramList.size() > 0)
   {
      ossimString s1 = prefix;
      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              "ossimMultiResLevelHistogram",
              true);
      kwl.add(prefix,
              NUMBER_OF_RES_LEVELS.key(),
              theHistogramList.size(),
              true);

      for(ossim_uint32 index = 0; index < theHistogramList.size(); ++index)
      {
         ossimString rr_level = ossimString(prefix) + "rr_level";
         rr_level += ossimString::toString(index)   + ".";
         if(theHistogramList[index])
         {
            result = theHistogramList[index]->saveState(kwl, rr_level.c_str());
            
            if(!result)
            {
               return result;
            }
         }
      }    
   }

   return result;
}

bool ossimMultiResLevelHistogram::loadState(const ossimKeywordlist& kwl,
                                            const char* prefix)
{
   deleteHistograms();
   const char* number_of_res_levels = kwl.find(prefix, NUMBER_OF_RES_LEVELS.key());

   if(number_of_res_levels)
   {
      ossim_uint32 numberOfResLevels = ossimString(number_of_res_levels).toUInt32();

      if(numberOfResLevels)
      {
         ossimString newPrefix;
         for(ossim_uint32 index = 0; index < numberOfResLevels; ++index)
         {
            ossimMultiBandHistogram* histo = new ossimMultiBandHistogram;

            newPrefix = prefix;
            newPrefix += "rr_level";
            newPrefix += ossimString::toString(index);
            newPrefix += ".";
            
            histo->loadState(kwl, newPrefix.c_str());
            
            theHistogramList.push_back(histo);
         }
      }
   }
   return true;
}

ossimFilename ossimMultiResLevelHistogram::getHistogramFile() const
{
   return theHistogramFile;
}
