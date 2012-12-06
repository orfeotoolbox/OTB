//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
// Description: 
//
//*******************************************************************
//  $Id: ossimMultiResLevelHistogram.cpp 20610 2012-02-27 12:19:25Z gpotts $
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
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

ossimMultiResLevelHistogram::ossimMultiResLevelHistogram(ossim_uint32 numberOfResLevels)
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
      theHistogramList[i] = rhs.theHistogramList[i].valid()?
                            new ossimMultiBandHistogram(*rhs.theHistogramList[i]):
                            (ossimMultiBandHistogram*)NULL;
   }
}

ossimMultiResLevelHistogram::~ossimMultiResLevelHistogram()
{
   deleteHistograms();
}

void ossimMultiResLevelHistogram::create(ossim_uint32 numberOfResLevels)
{
   deleteHistograms();

   for(ossim_uint32 idx = 0; idx < numberOfResLevels; ++idx)
   {
      theHistogramList.push_back(new ossimMultiBandHistogram);      
   }
}

ossimRefPtr<ossimMultiResLevelHistogram> ossimMultiResLevelHistogram::createAccumulationLessThanEqual()const
{
   ossimRefPtr<ossimMultiResLevelHistogram> result = 0;

   if(theHistogramList.size() > 0)
   {
      result = new ossimMultiResLevelHistogram((ossim_uint32)theHistogramList.size());

      for(ossim_uint32 idx=0; idx < (ossim_uint32) theHistogramList.size(); ++ idx)
      {
         if(theHistogramList[idx].valid())
         {
            ossimRefPtr<ossimMultiBandHistogram> multiBandAccumulator = theHistogramList[idx]->createAccumulationLessThanEqual();
            result->theHistogramList[idx] = multiBandAccumulator;
         }
         else
         {
            // we will push on a null since someone could
            // turn off a band.  A null accumulation will
            // indicate no histogram data.
            //
            result->theHistogramList[idx] = 0;
         }
      }
   }
   
   return result;
}

void ossimMultiResLevelHistogram::setBinCount(double binNumber, double count)
{
   if(theHistogramList.size() > 0)
   {
      for(ossim_uint32 idx=0; idx < (ossim_uint32) theHistogramList.size(); ++ idx)
      {
         if(theHistogramList[idx].valid())
         {
            theHistogramList[idx]->setBinCount(binNumber, count);
         }
      }
   }   
}

ossimRefPtr<ossimMultiResLevelHistogram> ossimMultiResLevelHistogram::createAccumulationGreaterThanEqual()const
{
   ossimRefPtr<ossimMultiResLevelHistogram> result = NULL;

   if(theHistogramList.size() > 0)
   {
      result = new ossimMultiResLevelHistogram((ossim_uint32)theHistogramList.size());

      for(ossim_uint32 idx=0; idx < (ossim_uint32) theHistogramList.size(); ++ idx)
      {
         if(theHistogramList[idx].valid())
         {
            ossimRefPtr<ossimMultiBandHistogram> multiBandAccumulator = theHistogramList[idx]->createAccumulationGreaterThanEqual();
            result->theHistogramList[idx]=multiBandAccumulator;
         }
         else
         {
            // we will push on a null since someone could
            // turn off a band.  A null accumulation will
            // indicate no histogram data.
            //
            result->theHistogramList[idx] = 0;
         }
      }
      result = 0;
   }
   
   return result;
}

void ossimMultiResLevelHistogram::addHistogram(ossimMultiBandHistogram* histo)
{
   theHistogramList.push_back(histo);
}

ossimRefPtr<ossimMultiBandHistogram> ossimMultiResLevelHistogram::addHistogram()
{
   ossimRefPtr<ossimMultiBandHistogram> result = new ossimMultiBandHistogram;
   theHistogramList.push_back(result);
   return result;
}

bool ossimMultiResLevelHistogram::setHistogram(ossimRefPtr<ossimMultiBandHistogram> histo, 
                                               ossim_uint32 resLevel)
{
   if(resLevel < getNumberOfResLevels())
   {
      theHistogramList[resLevel] = histo;
      return true;
   }
   return false;
}

void ossimMultiResLevelHistogram::deleteHistograms()
{
   for(ossim_uint32 idx = 0; idx < (ossim_uint32)theHistogramList.size(); ++idx)
   {
      if(theHistogramList[idx].valid())
      {
         theHistogramList[idx] = NULL;
      }
   }
   
   theHistogramList.clear();
}

ossimRefPtr<ossimHistogram> ossimMultiResLevelHistogram::getHistogram(ossim_uint32 band,
                                                          ossim_uint32 resLevel)
{
   ossimRefPtr<ossimMultiBandHistogram> temp = getMultiBandHistogram(resLevel);

   if(temp.valid())
   {
      return temp->getHistogram(band);
   }
   
   return (ossimHistogram*)0;
}
const ossimRefPtr<ossimHistogram> ossimMultiResLevelHistogram::getHistogram(ossim_uint32 band,
                                                                            ossim_uint32 resLevel)const
{
   const ossimRefPtr<ossimMultiBandHistogram> temp = getMultiBandHistogram(resLevel);

   if(temp.valid())
   {
      return temp->getHistogram(band);
   }
   
   return (ossimHistogram*)0;
}

ossim_uint32 ossimMultiResLevelHistogram::getNumberOfResLevels()const
{
   return (ossim_uint32)theHistogramList.size();
}

ossim_uint32 ossimMultiResLevelHistogram::getNumberOfBands(ossim_uint32 resLevel) const
{
   const ossimRefPtr<ossimMultiBandHistogram> h = getMultiBandHistogram(resLevel);
   if (h.valid())
   {
      return h->getNumberOfBands();
   }

   return 0;
}  

ossimRefPtr<ossimMultiBandHistogram> ossimMultiResLevelHistogram::getMultiBandHistogram(ossim_uint32 resLevel) const
{
   if(resLevel < (ossim_uint32)theHistogramList.size())
   {
      return theHistogramList[resLevel];
   }

   return NULL;
}

bool ossimMultiResLevelHistogram::importHistogram(std::istream& in)
{
   if (!in) // Check stream state.
   {
      return false;
   }
   
   ossimString buffer;
   getline(in, buffer);

   if ( in.eof() ) // Zero byte files will hit here.
   {
      return false;
   }

   // check to see if it is a proprietary histogram file
   // 
   if((buffer =="") || (buffer.c_str()[0] != 'F' ||
      buffer.c_str()[1] != 'i'))
   {
      in.seekg(0, ios::beg);
      ossimKeywordlist kwl;
      if (kwl.parseStream(in) == true)
      {
         return loadState(kwl);
      }
      else
      {
         return false;
      }
   }
   
   ossimProprietaryHeaderInformation header;
   in.seekg(0, ios::beg);
   deleteHistograms();
   if(header.parseStream(in))
   {
      ossim_uint32 numberOfResLevels = header.getNumberOfResLevels();
      
      if(numberOfResLevels)
      {
         theHistogramList.resize(numberOfResLevels);

         for(ossim_uint32 counter = 0; counter < (ossim_uint32)theHistogramList.size(); ++counter)
         {
            theHistogramList[counter] = NULL;
         }
         ossimString reslevelBuffer;
         ossimString buffer;
         
         for(ossim_uint32 idx = 0; idx < numberOfResLevels; ++idx)
         {
            getline(in, buffer);
            if(buffer.find("RR Level") != string::npos)
            {
               std::string::size_type offset = buffer.find(":");
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
            ossim_uint32 resLevelIdx = reslevelBuffer.toUInt32();

            if(resLevelIdx < (ossim_uint32)theHistogramList.size())
            {
               if(!theHistogramList[resLevelIdx])
               {
                  ossimRefPtr<ossimMultiBandHistogram> histogram = new ossimMultiBandHistogram;
                  if(histogram->importHistogram(in))
                  {
                     theHistogramList[resLevelIdx] = histogram;
                  }
                  else
                  {
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
   if( file.fileSize() > 0 )
   {
      theHistogramFile = file;
      
      ifstream input(file.c_str());
      return importHistogram(input);
   }
   return false;
}

bool ossimMultiResLevelHistogram::ossimProprietaryHeaderInformation::parseStream(std::istream& in)
{
   ossimString inputLine;

   getline(in, inputLine);  
   if(inputLine.find("File Type") != string::npos)
   {
      std::string::size_type idx = inputLine.find(":");
      if(idx != string::npos)
      {
         theFileType = inputLine.substr(idx+1);
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
      std::string::size_type idx = inputLine.find(":");
      if(idx != string::npos)
      {
         theVersion = inputLine.substr(idx+1);
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
      std::string::size_type idx = inputLine.find(":");
      if(idx != string::npos)
      {
         theCreatorId = inputLine.substr(idx+1);
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
      std::string::size_type idx = inputLine.find(":");
      if(idx != string::npos)
      {
         theNumberOfResLevels = inputLine.substr(idx+1);
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
              static_cast<ossim_uint32>(theHistogramList.size()),
              true);

      for(ossim_uint32 idx = 0; idx < theHistogramList.size(); ++idx)
      {
         ossimString rr_level = ossimString(prefix) + "rr_level";
         rr_level += ossimString::toString(idx)   + ".";
         if(theHistogramList[idx].valid())
         {
            result = theHistogramList[idx]->saveState(kwl, rr_level.c_str());
            
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
         for(ossim_uint32 idx = 0; idx < numberOfResLevels; ++idx)
         {
            ossimRefPtr<ossimMultiBandHistogram> histo = new ossimMultiBandHistogram;

            if (prefix) newPrefix = prefix;
            
            newPrefix += "rr_level";
            newPrefix += ossimString::toString(idx);
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
