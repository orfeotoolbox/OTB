//----------------------------------------------------------------------------
// Copyright (C) 2005 David Burken, all rights reserved.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: David Burken
//
// Description:
//
// Contains class declaration of ossimDtedElevationImageSource.
//
// $Id: ossimDtedElevationImageSource.cpp,v 1.4 2005/02/11 15:07:31 dburken Exp $
//----------------------------------------------------------------------------

#include <vector>
#include <sstream>
using namespace std;

#include <base/common/ossimTrace.h>
#include <base/factory/ossimObjectFactoryRegistry.h>
#include <base/data_types/ossimIrect.h>
#include <elevation/formats/dted_cell/ossimDtedHandler.h>
#include <projections/ossimImageViewProjectionTransform.h>
#include <projections/map_projections/ossimMapProjection.h>
#include <imaging/formats/dted/ossimDtedElevationImageSource.h>
#include <imaging/formats/dted/ossimDtedTileSource.h>
#include <imaging/factory/ossimImageHandlerRegistry.h>
#include <imaging/tile_sources/ossimImageRenderer.h>
#include <imaging/ossimImageData.h>
#include <imaging/ossimImageChain.h>

static ossimTrace traceDebug("ossimDtedElevationImageSource:degug");

ossimDtedElevationImageSource::ossimDtedElevationImageSource()
   :
   theDirectory(),
   theCellExtension("dt2"),
   theResamplerType(ossimDtedElevationImageSource::BILINEAR)
{
}

ossimDtedElevationImageSource::~ossimDtedElevationImageSource()
{
}

ossimRefPtr<ossimImageData> ossimDtedElevationImageSource::getBlock(
   const ossimGrect& rect,
   ossim_uint32 lines,
   ossim_uint32 samples) const
{

   ossimDpt postSpacing;
   getPostSpacing(rect, lines, samples, postSpacing);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDtedElevationImageSource::getBlock DEBUG:"
         << "\nrect:          " << rect
         << "\nlines:         " << lines
         << "\nsamples:       " << samples
         << "\npost spacing:  " << postSpacing
         << "\ntheDirectory:  " << theDirectory
         << endl;
   }

   // Get a list of needed cells.
   vector<ossimFilename> vf;
   findCells(rect, vf);

   // Create a mosaic of them.
   ossimImageChain* mosaic = createMosaic(vf);
   if (!mosaic)
   {
      return NULL;
   }

   // Get the view.
   ossimMapProjection* view = getView(mosaic);
   if (!view)
   {
      delete mosaic;
      return NULL;
   }
   
   // Set the output resolution.
   view->setDecimalDegreesPerPixel(postSpacing);

   // Set the tie point to be the upper left of the requested rect.
   view->setUlGpt(rect.ul());

   // Convert the ground rectangle to the view's image space.
   ossimIrect tileRect;
   computeImageRect(view, rect, tileRect);

   cout << "tileRect:  " << tileRect << endl;
   
   ossimRefPtr<ossimImageData> result = mosaic->getTile(tileRect);

   delete mosaic;
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDtedElevationImageSource::getBlock DEBUG:"
         << *result
         << "\nReturning..."
         << endl;
   }

   return result;
}

void ossimDtedElevationImageSource::setDirectory(const ossimFilename& directory)
{
   theDirectory = directory;
}

void ossimDtedElevationImageSource::getDirectory(ossimFilename& directory) const
{
   directory = theDirectory;
}

void ossimDtedElevationImageSource::findCells(
   const ossimGrect& rect,
   vector<ossimFilename>& vf) const
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDtedElevationImageSource::findCells entered..." << endl;
   }

   // Clear the vector of filenames passed to us.
   vf.clear();

   if (!theDirectory.isDir())
   {
      return;
   }   

   //---
   // Expand out the rectangle to even degree boundaries.
   // Handle wrapping???
   //---
   
   ossimGrect expandedRect = rect.stretchToEvenBoundary(1.0, 1.0);


   ossim_int32 startLat = static_cast<ossim_int32>(expandedRect.ll().latd());
   ossim_int32 startLon = static_cast<ossim_int32>(expandedRect.ll().lond());
   ossim_int32 stopLat  =
      static_cast<ossim_int32>(expandedRect.ur().latd()-1);
   ossim_int32 stopLon  =
      static_cast<ossim_int32>(expandedRect.ur().lond()-1);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG:"
         << "\nepandedRect:  " << expandedRect
         << "\nstartLat:  " << startLat
         << "\nstartLon:  " << startLon
         << "\nstopLat:   " << stopLat
         << "\nstopLon:   " << stopLon
         
         << endl;
   }

   
   for (ossim_int32 lon = startLon; lon <= stopLon; ++lon)
   {
      // Build up a dted file name.
      ossimString lonBase;
      if (lon < 0)
      {
         lonBase = "w";
      }
      else
      {
         lonBase = "e";
      }

      ossim_int32 tmpLon = abs(lon);
      ostringstream  s1;
      s1<< setfill('0')<<setw(3)<< tmpLon;

      lonBase += s1.str().c_str();
      lonBase += "/";

      for (ossim_int32 lat = startLat; lat <= stopLat; ++lat)
      {
         ossimFilename dtedFile = theDirectory.dirCat(lonBase);
         if (lat >= 0)
         {
            dtedFile += "n";
         }
         else
         {
            dtedFile += "s";
         }
         
         ossim_int32 tmpLat = abs(lat);
         ostringstream  s2;
         s2<< setfill('0')<<setw(2)<<tmpLat;
         dtedFile += s2.str().c_str();
         
         dtedFile.setExtension(theCellExtension);

         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "\nSearching for:  " << dtedFile
               << endl;
         }

         if (dtedFile.exists())
         {
            vf.push_back(dtedFile);
         }
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "Cell list:\n";
      
      vector<ossimFilename>::const_iterator i = vf.begin();
      while (i != vf.end())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << (*i) << "\n";
         ++i;
      }
      ossimNotify(ossimNotifyLevel_DEBUG) << endl;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDtedElevationImageSource::findCells exited..." << endl;
   }
}

void ossimDtedElevationImageSource::resampleCell(
   const ossimGrect& rect,
   const ossimFilename& dtedFile,
   ossimImageData& id,
   const ossimDpt& postSpacing) const
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDtedElevationImageSource::resampleCell DEBUG:  Entered..."
         << endl;
   }

   switch (theResamplerType)
   {
      case NEAREST_NEIGHBOR:
         resampleCellNearestNeighbor(rect, dtedFile, id, postSpacing);
         break;

      case BILINEAR:
         resampleCellBilinear(rect, dtedFile, id, postSpacing);
         break;

      default:
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimDtedElevationImageSource::resampleCell type not handled!"
            << endl;
         break;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDtedElevationImageSource::resampleCell DEBUG:  Exited..."
         << endl;
   }
}

void ossimDtedElevationImageSource::resampleCellBilinear(
   const ossimGrect& rect,
   const ossimFilename& dtedFile,
   ossimImageData& id,
   const ossimDpt& postSpacing) const
{
   cout << dtedFile << endl;
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDtedElevationImageSource::resampleCellBilinear DEBUG:"
         << " Entered..."
         << endl;
   }

   ossimDtedHandler* dh = new ossimDtedHandler(dtedFile);
   if (!dh)
   {
      return;
   }
   if (dh->getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
      delete dh;
      return;
   }

   ossim_float32* buf = id.getFloatBuf();
   if (!buf)
   {
      delete dh;
      return;
   }
   
   // Get the clip rectangle.
   const ossimGrect clipRect = rect.clipToRect(dh->getBoundingGndRect());
   const ossimGpt STOP_GPT = clipRect.lr();
   ossimGpt gpt = clipRect.ul();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "clipRect:  " << clipRect
         << "\nSTOP_GPT:  " << STOP_GPT
         << "\nStarting gpt:       " << gpt
         << endl;
   }

   // Walk in the latitude (line) direction.
   while (gpt.lat >= STOP_GPT.lat)
   {
      // Walk in longitude (sample) direction.
      gpt.lon = clipRect.ul().lon;
      while (gpt.lon <= STOP_GPT.lon)
      {
         ossim_uint32 index = computeIndex(id, rect, gpt, postSpacing);
//         cout << "index:  " << index << endl;
         if (index != OSSIM_UINT_NAN)
         {
            ossim_float32 hgt =
               static_cast<ossim_float32>(dh->getHeightAboveMSL(gpt));
//             cout << "index: " << index
//                  << "\ngpt: " << gpt
//                  << "\nhgt: " << hgt
//                  << endl;
            buf[index] = hgt;
            if (hgt == -32767.0)
            {
               cout << gpt << endl;
            }
            // static_cast<ossim_float32>(dh->getHeightAboveMSL(gpt));
         }
         else
         {
            cout << "nan index for gpt:\n" << gpt << endl;
         }
         
         gpt.lon = gpt.lon + postSpacing.x;
      }
      gpt.lat = gpt.lat - postSpacing.y;
   }
//   cout << "Ending gpt:  " << gpt << endl;

//    // Walk in the latitude direction
//    while (gpt.lat <= STOP_GPT.lat)
//    {
//       while (gpt.lon <= STOP_GPT.lon)
//       {
//          gpt.lon += postSpacing.x;
//       }
//       gpt.lat += postSpacing.y;
//    }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimDtedElevationImageSource::resampleCellBilinear DEBUG:"
         << " Exited..."
         << endl;
   }
   
   delete dh;
}

void ossimDtedElevationImageSource::resampleCellNearestNeighbor(
   const ossimGrect& rect,
   const ossimFilename& dtedFile,
   ossimImageData& id,
   const ossimDpt& requestedPostSpacing) const
{
   // Open the cell.
   ossimImageHandler* dts = new ossimDtedTileSource();
   dts->open(dtedFile);

   if (dts->isOpen() == false)
   {
      delete dts;
      return;
   }

   // Get the post spacing of the cell.
   ossimDpt cellPostSpacing;
   ((ossimDtedTileSource*)dts)->getPostSpacing(cellPostSpacing);

//    // Compute the rlevel to grab from.
//    ossim_uint32 rLevel = computeRLevel(dts->getNumberOfDecimationLevels(),
//                                        requestedPostSpacing,
//                                        cellPostSpacing);

   
}

void ossimDtedElevationImageSource::getPostSpacing(const ossimGrect& rect,
                                                   ossim_uint32 lines,
                                                   ossim_uint32 samples,
                                                   ossimDpt& result) const
{
   result.lat = (rect.ul().lat - rect.ll().lat) / (lines   - 1);
   result.lon = (rect.lr().lon - rect.ll().lon) / (samples - 1);
}

void ossimDtedElevationImageSource::snap(const ossimGrect& rect,
                                         const ossimDpt& postSpacing,
                                         ossimGrect& clipRect) const
{
   ossim_float64 d;

   // upper left latitude
   d = (rect.ul().lat - clipRect.ul().lat) / postSpacing.y;
   if (d != 0.0)
   {
      clipRect.ul().lat = floor(d) * postSpacing.y;
   }

   // upper left longitude
   d = (rect.ul().lon - clipRect.ul().lon) / postSpacing.x;
   if (d != 0.0)
   {
      clipRect.ul().lon = ceil(d) * postSpacing.x;
   }

   // upper right latitude
   d = (rect.ul().lat - clipRect.ur().lat) / postSpacing.y;
   if (d != 0.0)
   {
      clipRect.ur().lat = floor(d) * postSpacing.y;
   }

   // upper right longitude
   d = (rect.ul().lon - clipRect.ur().lon) / postSpacing.x;
   if (d != 0.0)
   {
      clipRect.ul().lon = floor(d) * postSpacing.x;
   }

   // lower right latitude
   d = (rect.ul().lat - clipRect.lr().lat) / postSpacing.y;
   if (d != 0.0)
   {
      clipRect.lr().lat = ceil(d) * postSpacing.y;
   }

   // lower right longitude
   d = (rect.ul().lon - clipRect.lr().lon) / postSpacing.x;
   if (d != 0.0)
   {
      clipRect.ul().lon = floor(d) * postSpacing.x;
   }

   // lower left latitude
   d = (rect.ul().lat - clipRect.ll().lat) / postSpacing.y;
   if (d != 0.0)
   {
      clipRect.ll().lat = ceil(d) * postSpacing.y;
   }

   // lower left longitude
   d = (rect.ul().lon - clipRect.ll().lon) / postSpacing.x;
   if (d != 0.0)
   {
      clipRect.ul().lon = ceil(d) * postSpacing.x;
   }
}

ossim_uint32 ossimDtedElevationImageSource::computeIndex(
   const ossimImageData& id,
   const ossimGrect& idRect,
   const ossimGpt& gpt,
   const ossimDpt& postSpacing) const
{
   if ( ! idRect.pointWithin(gpt) )
   {
      return OSSIM_INT_NAN;
   }

   ossim_uint32 line = static_cast<ossim_uint32>( (idRect.ul().lat - gpt.lat)
                                                  / postSpacing.y );
   ossim_uint32 samp = static_cast<ossim_uint32>( (gpt.lon - idRect.ul().lon)
                                                  / postSpacing.x );
   return ( (line * id.getWidth()) + samp );
}

ossim_uint32 ossimDtedElevationImageSource::computeRLevel(
   ossim_uint32 numberOfRLevels,
   const ossimDpt& requestedPostSpacing,
   const ossimDpt& cellPostSpacing) const
{
   if (numberOfRLevels == 1)
   {
      return 0;
   }

   // Start the check at r1 postspacing.
   ossimDpt rLevelPostSpacing(cellPostSpacing.x * 2.0,
                              cellPostSpacing.y * 2.0);
   ossim_uint32 level = 0;

   for (ossim_uint32 i = 1; i < numberOfRLevels; ++i)
   {
      if ( (rLevelPostSpacing.x > requestedPostSpacing.x) &&
           (rLevelPostSpacing.y > requestedPostSpacing.y) )
      {
         break;
      }
      
      rLevelPostSpacing.x = rLevelPostSpacing.x * 2.0;
      rLevelPostSpacing.y = rLevelPostSpacing.y * 2.0;
      ++level;
   }

   return level;
}

void ossimDtedElevationImageSource::setCellExtension(const ossimString& ext)
{
   theCellExtension = ext;
}

void ossimDtedElevationImageSource::getCellExtension(ossimString& ext) const
{
   ext = theCellExtension;
}

ossimConnectableObject* ossimDtedElevationImageSource::createRawChain(
   const ossimFilename& file) const
{
   ossimImageHandler* ih  = ossimImageHandlerRegistry::instance()->open(file);
   ossimConnectableObject* result = NULL;
   if(ih)
   {
      ossimImageChain* chain = new ossimImageChain;
      chain->add(ih);
      chain->setDescription(file);
      result = chain;
   }

   return result;
}

ossimImageChain* ossimDtedElevationImageSource::createMosaic(
   const std::vector<ossimFilename>& inputs) const
{
   std::vector<ossimConnectableObject*> inputObjects;
   for(int idx = 0; idx < (int)inputs.size();++idx)
   {
      ossimConnectableObject* obj = createRawChain(inputs[idx]);
      if(obj)
      {
         inputObjects.push_back(obj);
      }
   }
   return createMosaic(inputObjects);
}

ossimImageChain* ossimDtedElevationImageSource::createMosaic(
   const std::vector<ossimConnectableObject*>& inputs) const
{
   if(!inputs.size())
   {
      return NULL;
   }

   std::vector<ossimConnectableObject*> inputList;
   ossim_uint32 idx = 0;

   for(idx = 0; idx < inputs.size(); ++idx)
   {
      ossimImageChain* chain = PTR_CAST(ossimImageChain, inputs[idx]);
      if(chain)
      {
         inputList.push_back(chain);
      }
   }
   if(!inputList.size())
   {
      return NULL;
   }
   
   ossimImageChain* chainResult = NULL;
   ossimString defaultDescription="ossimOrthoImageMosaic";;
   ossimKeywordlist kwl;
   
   kwl.add("type",
           "ossimImageChain",
           true);
   
   kwl.add("id",
           0,
           true);
   
   kwl.add("object1.type",
           "ossimOrthoImageMosaic",
           true);
   kwl.add("object1.id",
           1,
           true);
   
   
   kwl.add("object10.type",
           "ossimCacheTileSource",
           true);
   kwl.add("object10.id",
           10,
           true);
   kwl.add("object10.input_connection1",
           1,
           true);
   
   kwl.add("object20.type",
           "ossimImageRenderer",
           true);
   kwl.add("object20.id",
           20,
           true);
   kwl.add("object20.input_connection1",
           10,
           true);
   
    ossimObject* objResult =
       ossimObjectFactoryRegistry::instance()->createObject(kwl);
    chainResult      = PTR_CAST(ossimImageChain, objResult);
    if(chainResult)
    {
	chainResult->makeUniqueIds();
        chainResult->setDescription(defaultDescription);
        for(idx = 0; idx < inputList.size(); ++idx)
        {
           chainResult->connectMyInputTo(inputList[idx]);
        }
    }
    else if(objResult)
    {
	delete objResult;
    }
    
    return chainResult;
}

ossimMapProjection* ossimDtedElevationImageSource::getView(
   ossimConnectableObject* container) const
{
   if (!container)
   {
      cerr << "a..." << endl;
      return NULL;
   }

   // Find the resampler.
   ossimImageRenderer* rs =
      (ossimImageRenderer*)container->
      findObjectOfType("ossimImageRenderer",
                       ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT);

   if (!rs)
   {
      cerr << "b..." << endl;
      return NULL;
   }
   
   const ossimObject* obj = rs->getView();
   if (!obj)
   {
      cerr << "b2..." << endl;
   }

   return PTR_CAST(ossimMapProjection, obj);
}

void ossimDtedElevationImageSource::computeImageRect(
   const ossimMapProjection* view,
   const ossimGrect& grect,
   ossimIrect& irect) const
{
   if (!view)
   {
      return;
   }
   
   ossimDpt dpt;
   view->worldToLineSample(grect.ul(), dpt);
   irect.set_ul(dpt);
   view->worldToLineSample(grect.lr(), dpt);
   irect.set_lr(dpt);
}

void ossimDtedElevationImageSource::setResamplerType(ossimDtedElevationImageSource::ResamplerType type)
{
   theResamplerType = type;
}

ossimDtedElevationImageSource::ResamplerType ossimDtedElevationImageSource::getResamplerType() const
{
   return theResamplerType;
}

