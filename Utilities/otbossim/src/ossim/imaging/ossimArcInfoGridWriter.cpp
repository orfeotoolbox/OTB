//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimArcInfoGridWriter.cpp 16081 2009-12-10 20:56:36Z eshirschorn $

#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>

#include <ossim/imaging/ossimArcInfoGridWriter.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimCastTileSourceFilter.h>

#include <ossim/support_data/ossimAigStatistics.h>
#include <ossim/support_data/ossimAigHeader.h>
#include <ossim/support_data/ossimAigBounds.h>
#include <ossim/support_data/ossimAigIndexFileHeader.h>
#include <ossim/support_data/ossimAigDataFileHeader.h>

#include <ossim/imaging/ossimCastTileSourceFilter.h>

static const ossimTrace traceDebug("ossimArcInfoGridWriter:debug");

RTTI_DEF1(ossimArcInfoGridWriter,
          "ossimArcInfoGridWriter",
          ossimImageFileWriter);

//*******************************************************************
// Constructor:
//*******************************************************************
ossimArcInfoGridWriter::ossimArcInfoGridWriter()
   : ossimImageFileWriter(),
     theOutputTileSize(256, 4)
{
}

ossimArcInfoGridWriter::ossimArcInfoGridWriter(ossimImageSource* inputSource,
                                 const ossimFilename& filename)
   :ossimImageFileWriter(filename, inputSource)
{
}

//*******************************************************************
// Destructor
//*******************************************************************
ossimArcInfoGridWriter::~ossimArcInfoGridWriter()
{
   close();
}

bool ossimArcInfoGridWriter::writeFile()
{
   const char* MODULE = "ossimArcInfoGridWriter::writeFile";
   if(!theInputConnection)
   {
      return false;
   }
   
   ossimRefPtr<ossimCastTileSourceFilter> filter = new ossimCastTileSourceFilter;
   filter->setOutputScalarType(OSSIM_FLOAT);
   filter->connectMyInputTo(theInputConnection.get());
   filter->initialize();
   
   open();

   // make sure we have a region of interest
   if(theAreaOfInterest.hasNans())
   {
      theInputConnection->initialize();
      theAreaOfInterest = theInputConnection->getAreaOfInterest();
   }
   else
   {
      theInputConnection->setAreaOfInterest(theAreaOfInterest);
   }
   
   if(theAreaOfInterest.hasNans()) return false;   
   
   theInputConnection->setTileSize(theOutputTileSize);
   theInputConnection->setToStartOfSequence();
   
   bool result = writeBinaryAigHeader()&&
                 writeBinaryAigBounds()&&
                 writeBinaryAigStats();
   if(result)
   {
      ossimFilename indexFile=ossimFilename(theFilename+"/"+"w001001x.adf");
      ossimFilename dataFile=ossimFilename(theFilename+"/"+"w001001.adf");
      
      std::ofstream indexFileStream(indexFile.c_str(),
                               ios::out|ios::binary);
      std::ofstream dataFileStream(dataFile.c_str(),
                              ios::out|ios::binary);
      ossimEndian endian;

      if(indexFileStream&&dataFileStream)
      {
         ossimAigIndexFileHeader aigIndexHeader;
         ossimAigDataFileHeader  aigDataFileHeader;
         
         ossimRefPtr<ossimImageData> currentTile =
            theInputConnection->getNextTile();
         long tileNumber = 0;
         long totalNumberOfTiles = theInputConnection->getNumberOfTiles();
         ossim_int32 tileOffsetShorts = 0;
         ossim_int32 tileSizeShorts   = 0;

         ossimNotify(ossimNotifyLevel_NOTICE)
            << "Tile height  = " << theInputConnection->getTileHeight()
            << std::endl
            << "Tile width   = " << theInputConnection->getTileWidth()
            << std::endl;
         ossim_int32 tileSizeInShorts = 2*(theInputConnection->getTileHeight()*
                                           theInputConnection->getTileWidth());
         // Note: this is in shorts
         //
         aigIndexHeader.theFileSize += (4*totalNumberOfTiles);

         // Note this is in shorts
         //
         
         aigDataFileHeader.theFileSize = 50+
                                         (tileSizeInShorts*totalNumberOfTiles) +
                                         totalNumberOfTiles;
         aigDataFileHeader.theTileSize = tileSizeInShorts;
         
         aigIndexHeader.writeStream(indexFileStream);
         aigDataFileHeader.writeStream(dataFileStream);

         if(traceDebug())
         {
            CLOG << "AIG INDEX HEADER = \n" << aigIndexHeader << std::endl;
            CLOG << "AIG DATA FILE HEADER\n"
                 << "file size = " << aigDataFileHeader.theFileSize
                 << std::endl;
         }
         
         while(currentTile.valid())
         {
            currentTile = filter->applyCast(currentTile);
            
            if(!currentTile->getBuf())
            {
               currentTile->initialize();
            }

            tileOffsetShorts = dataFileStream.tellp()/2;
            tileSizeShorts   = 2*currentTile->getHeight()*currentTile->getWidth();
            ossim_int16 tileSizeShorts16 = 2*currentTile->getHeight()*currentTile->getWidth();
            if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
            {
               endian.swap(tileOffsetShorts);
               endian.swap(tileSizeShorts);
               endian.swap(tileSizeShorts16);
               endian.swap((ossim_float32*)currentTile->getBuf(),
                           currentTile->getWidth()*
                           currentTile->getHeight());
            }
            
            indexFileStream.write((char*)(&tileOffsetShorts), 4);
            indexFileStream.write((char*)(&tileSizeShorts), 4);
            dataFileStream.write((char*)(&tileSizeShorts16),
                                 2);
            dataFileStream.write((char*)(currentTile->getBuf()),
                                 currentTile->getWidth()*currentTile->getHeight()*4);
            
            currentTile = theInputConnection->getNextTile();
            ++tileNumber;
         }
      }
   }
   
   close();
   filter = 0;
   return result;
}

bool ossimArcInfoGridWriter::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   return ossimImageFileWriter::saveState(kwl, prefix);
}

bool ossimArcInfoGridWriter::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   
   return ossimImageFileWriter::loadState(kwl, prefix);
}

bool ossimArcInfoGridWriter::isOpen()const
{
   return (theFilename.isDir() && theFilename.exists());
}

bool ossimArcInfoGridWriter::open()
{
   if(theFilename.isDir()&&
      theFilename.exists())
   {
      return true;
   }
   else
   {
      theFilename = theFilename.path();
      if(theFilename.isDir()&&
         theFilename.exists())
      {
         return true;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimArcInfoGridWriter::open(): Directory "
            << theFilename << " doesn't exist" << std::endl;
      }
      
   }
   return false;   
}

void ossimArcInfoGridWriter::close()
{
}


bool ossimArcInfoGridWriter::writeBinaryAigHeader()
{
   const char* MODULE = "ossimArcInfoGridWriter::writeBinaryAigHeader";
   ossimAigHeader aigHeader;

   ossimFilename file=ossimFilename(theFilename+"/"+"hdr.adf");

   if(isOpen())
   {
      switch(theInputConnection->getOutputScalarType())
      {
         case OSSIM_UCHAR:
         case OSSIM_SSHORT16:
         case OSSIM_USHORT11:
         case OSSIM_USHORT16:
//      {
//         aigHeader.setIntCoverage();
//         break;
//      }
         case OSSIM_DOUBLE:
         case OSSIM_FLOAT:
         case OSSIM_NORMALIZED_FLOAT:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            aigHeader.setFloatCoverage();
            break;
         }
         default:
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimArcInfoGridWriter::writeBinaryAigHeader: "
               << "unkown scalar type" << std::endl;
         }
      }
      
      // need to set pixel size
      //

      // code goes here
      
      // now setting number of tiles per row
      aigHeader.theNumberOfTilesPerRow    = theInputConnection->getNumberOfTilesHorizontal();
      aigHeader.theNumberOfTilesPerColumn = theInputConnection->getNumberOfTilesVertical();
      aigHeader.theWidthOfTileInPixels    = theInputConnection->getTileWidth();
      aigHeader.theHeightOfTileInPixels   = theInputConnection->getTileHeight();
      
      std::ofstream out;
      out.open(file.c_str(), ios::out|ios::binary);

      if(out)
      {
         if(traceDebug())
         {
            CLOG << "AIG_HEADER = \n" << aigHeader << std::endl;
         }
         return aigHeader.writeStream(out);
      }
   }
   
   return false;
}

bool ossimArcInfoGridWriter::writeBinaryAigBounds()
{
   const char* MODULE = "ossimArcInfoGridWriter::writeBinaryAigBounds";
   ossimAigBounds aigBounds;
   

   // setup bounds
   ossimIrect bounds = theInputConnection->getBoundingRect();
   if(!bounds.hasNans())
   {
      aigBounds.ll_x = -.5;
      aigBounds.ll_y = -.5;
      aigBounds.ur_x = bounds.width()-.5;
      aigBounds.ur_y = bounds.height()-.5;
      
      //
      ossimFilename file=ossimFilename(theFilename+"/"+"dblbnd.adf");
      
      if(isOpen())
      {
         std::ofstream out;
         out.open(file.c_str(), ios::out|ios::binary);
         
         if(out)
         {
            if(traceDebug())
            {
               CLOG << "AIG BOUNDS =\n" <<aigBounds << std::endl;
            }
            return aigBounds.writeStream(out);
         }
      }
   }
   
   return false;
}

bool ossimArcInfoGridWriter::writeBinaryAigStats()
{
   ossimAigStatistics aigStats;


   // setup bounds
   aigStats.theMin  = theInputConnection->getMinPixelValue();
   aigStats.theMax  = theInputConnection->getMaxPixelValue();
   aigStats.theMean = (aigStats.theMax-aigStats.theMin)/2.0;

   //
   ossimFilename file=ossimFilename(theFilename+"/"+"sta.adf");

   if(isOpen())
   {
      std::ofstream out;
      out.open(file.c_str(), ios::out|ios::binary);
      
      if(out)
      {
         return aigStats.writeStream(out);
      }
   }

   return false;
}

ossimString ossimArcInfoGridWriter::getExtension() const
{
   return ossimString("aig");
}
