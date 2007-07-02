//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description: 
//
//*******************************************************************
//  $Id: ossimMultiBandHistogram.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimKeywordlist.h>

#include <fstream>
using namespace std;

ossimMultiBandHistogram::ossimMultiBandHistogram()
{  
}

ossimMultiBandHistogram::~ossimMultiBandHistogram()
{
   deleteHistograms();
}

ossimMultiBandHistogram::ossimMultiBandHistogram(const ossimMultiBandHistogram& rhs)
{
   theHistogramList.resize(rhs.theHistogramList.size());

   for(ossim_uint32 i = 0; i < theHistogramList.size(); ++i)
   {
      theHistogramList[i] =  rhs.theHistogramList[i]?
                             new ossimHistogram(*rhs.theHistogramList[i]):
                             (ossimHistogram*)NULL;
   }
}

ossimMultiBandHistogram::ossimMultiBandHistogram(long numberOfBands,
                                                 long numberOfBuckets,
                                                 float minValue,
                                                 float maxValue)
{
   if(numberOfBands > 0)
   {
      create(numberOfBands, numberOfBuckets, minValue, maxValue);
   }
}

void ossimMultiBandHistogram::create(long numberOfBands,
                                                   long numberOfBuckets,
                                                   float minValue,
                                                   float maxValue)
{
   // make sure we clear our internal lists before
   // we start.
   //
   deleteHistograms();

   if(numberOfBands > 0)
   {
      numberOfBuckets = numberOfBuckets>0?numberOfBuckets:1;

      for(long bands = 0; bands < numberOfBands; ++bands)
      {
         theHistogramList.push_back(new ossimHistogram(numberOfBuckets,
                                                       minValue,
                                                       maxValue));
      }
   }
}

void ossimMultiBandHistogram::create(long numberOfBands)
{
   deleteHistograms();
   for(long bands = 0; bands < numberOfBands; ++bands)
   {
      theHistogramList.push_back(new ossimHistogram);
   }
}

ossim_uint32 ossimMultiBandHistogram::getNumberOfBands() const
{
   return theHistogramList.size();
}

ossimHistogram* ossimMultiBandHistogram::getHistogram(long band)
{
   if((band >=0) && (band < (long)theHistogramList.size()))
   {
      return theHistogramList[band];
   }

   return NULL;
}

void ossimMultiBandHistogram::setBinCount(double binNumber, double count)
{
   if(theHistogramList.size() > 0)
   {
      for(long idx = 0; idx < (long)theHistogramList.size(); ++idx)
      {
         if(theHistogramList[idx])
         {
            theHistogramList[idx]->SetCount(binNumber, count);
         }
      }
   }   
}

ossimMultiBandHistogram* ossimMultiBandHistogram::createAccumulationLessThanEqual()const
{
   ossimMultiBandHistogram* result = NULL;

   if(theHistogramList.size() > 0)
   {
      result = new ossimMultiBandHistogram;
      result->theHistogramList.resize(theHistogramList.size());
      
      for(long index = 0; index < (long)theHistogramList.size(); ++index)
      {
         if(theHistogramList[index])
         {
            result->theHistogramList[index] = theHistogramList[index]->CumulativeLessThanEqual();
         }
         else
         {
            result->theHistogramList[index] = NULL;
         }     
      }
   }

   return result;
}

ossimMultiBandHistogram* ossimMultiBandHistogram::createAccumulationGreaterThanEqual()const
{
   ossimMultiBandHistogram* result = NULL;

   if(theHistogramList.size() > 0)
   {
      result = new ossimMultiBandHistogram;
      
      for(long index = 0; index < (long)theHistogramList.size(); ++index)
      {
         if(theHistogramList[index])
         {
            result->theHistogramList[index] = theHistogramList[index]->CumulativeGreaterThanEqual();
         }
         else
         {
            result->theHistogramList[index] = NULL;
         }     
      }
   }

   return result;
}

void ossimMultiBandHistogram::deleteHistograms()
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


bool ossimMultiBandHistogram::importHistogram(istream& in)
{
   ossimProprietaryHeaderInformation header;
   deleteHistograms();
   
   if(header.parseStream(in))
   {
      long numberOfBands = header.getNumberOfBands();
      
      if(numberOfBands)
      {
         theHistogramList.resize(numberOfBands);

         for(long counter = 0; counter < (long)theHistogramList.size(); ++counter)
         {
            theHistogramList[counter] = NULL;
         }
         ossimString bandBuffer;
         ossimString buffer;
         
         for(long index = 0; index < numberOfBands; ++index)
         {
            getline(in, buffer);
            if(buffer.find("Band") != string::npos)
            {
               unsigned long offset = buffer.find(":");
               if(offset != string::npos)
               {
                  bandBuffer = buffer.substr(offset+1);
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
            long bandIndex = bandBuffer.toLong();

            if(bandIndex < (long)theHistogramList.size())
            {
               if(!theHistogramList[bandIndex])
               {
                  ossimHistogram* histogram = new ossimHistogram;
                  if(histogram->importHistogram(in))
                  {
                     theHistogramList[bandIndex] = histogram;
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
         }
      }
      else
      {
         return false;
      }
   }  
   return true;
}

bool ossimMultiBandHistogram::importHistogram(const ossimFilename& file)
{
   if(file.exists())
   {
      ifstream input(file.c_str());

      return importHistogram(input);
   }

   return false;
}

bool ossimMultiBandHistogram::ossimProprietaryHeaderInformation::parseStream(istream& in)
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
   if(inputLine.find("Number of Bands") != string::npos)
   {
      unsigned long index = inputLine.find(":");
      if(index != string::npos)
      {
         theNumberOfBands = inputLine.substr(index+1);
         theNumberOfBands = theNumberOfBands.trim();
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

bool ossimMultiBandHistogram::saveState(ossimKeywordlist& kwl,
                                        const char* prefix)const
{
   kwl.add(prefix,
           "type",
           "ossimMultiBandHistogram",
           true);
   kwl.add(prefix,
           "number_of_bands",
           static_cast<ossim_uint32>(theHistogramList.size()),
           true);
   
   for(ossim_uint32 index = 0; index < theHistogramList.size(); ++index)
   {
      ossimString band = ossimString(prefix) + "band";
      band += (ossimString::toString(index) + ".");
      if(theHistogramList[index])
      {
         ossimString newPrefix = (ossimString(prefix) + ossimString::toString(index) + ".");
         
         theHistogramList[index]->saveState(kwl,
                                            band.c_str());
      }
   }
   
   return true;
}

bool ossimMultiBandHistogram::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   deleteHistograms();
   const char* number_of_bands = kwl.find(prefix, "number_of_bands");
   if(number_of_bands)
   {
      ossim_uint32 numberOfBands = ossimString(number_of_bands).toUInt32();

      if(numberOfBands>0)
      {
         ossimString newPrefix;
         for(ossim_uint32 index = 0; index < numberOfBands; ++index)
         {
            ossimHistogram* histo = new ossimHistogram;

            newPrefix = prefix;
            newPrefix += "band";
            newPrefix += ossimString::toString(index);
            newPrefix += ".";
            
            histo->loadState(kwl, newPrefix.c_str());
            
            theHistogramList.push_back(histo);
         }
      }
   }
   
   return true;
}
