//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Class declaration for image generator.
//
//*************************************************************************
// $Id: ossimIgen.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimIgen_HEADER
#define ossimIgen_HEADER

#include <iostream>
#include <list>
#include <fstream>

#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimConnectableContainer.h>
#include <ossim/imaging/ossimTiling.h>

class ossimImageChain;
class ossimImageFileWriter;
class ossimProjection;
class ossimImageViewTransform;
class ossimImageSource;
class ossimImageMpiMWriterSequenceConnection;
class ossimImageMpiSWriterSequenceConnection;
class ossimObject;
class ossimArgumentParser;

class OSSIM_DLL ossimIgen
{
public:
   ossimIgen();
   virtual ~ossimIgen();

   virtual void initialize(const ossimKeywordlist& kwl);
   virtual void outputProduct();
   
protected:
   ossimConnectableContainer theContainer;

   ossimProjection*  theProductProjection;
   ossimDrect        theOutputRect;
   bool              theBuildThumbnailFlag;
   ossimIpt          theThumbnailResolution;
   long              theNumberOfTilesToBuffer;
   ossimKeywordlist  theKwl;
   bool              theTilingEnabled;
   ossimTiling       theTiling;
   bool              theProgressFlag;
   
   /**
    * Initializes attributes of class.
    *
    * Keywords searched for by method:
    * 
    * igen.output_progress: true (bool)
    *
    * igen.thumbnail: true (bool)
    *
    * igen.thumbnail_res: 128 128 (x_res, y_res)
    *
    * (number of buffers for slave nodes to process before waiting)
    * igen.slave_tile_buffers: 2
    *
    * igen.tiling.type: ?
    *
    * igen.tiling: ?
    */
   void initializeAttributes();
   void slaveSetup();

   bool loadProduct();
   
   void deleteAttributes();
   void setView(ossimConnectableObject* obj, ossimObject* viewObj);
   ossimProjection* createImageProjection(const ossimKeywordlist& kwl);
   virtual ossimProjection* buildProductProjection(const ossimKeywordlist& kwl,
                                                   const char* prefix="product.");
   bool getOutputRect(const ossimKeywordlist& kwl,
                      ossimDrect& rect);
   void buildThumbnail();
   void outputImageGeometry(const char* filename);
   ossimImageChain* getChain();
};

#endif
