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
// $Id: ossimIgen.h 19911 2011-08-08 14:52:27Z gpotts $
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
class ossimMapProjection;
class ossimImageViewTransform;
class ossimImageSource;
class ossimImageMpiMWriterSequenceConnection;
class ossimImageMpiSWriterSequenceConnection;
class ossimObject;
class ossimArgumentParser;

class OSSIM_DLL ossimIgen : public ossimReferenced
{
public:
   ossimIgen();
   virtual ~ossimIgen();

   virtual void initialize(const ossimKeywordlist& kwl);
   virtual void outputProduct();
   
protected:
   void initializeAttributes();
   void slaveSetup();
   bool loadProductSpec();
   void setView();
   void initThumbnailProjection();
   void initializeChain();
   bool writeToFile(ossimImageFileWriter* writer);

   ossimRefPtr<ossimConnectableContainer> theContainer;
   ossimRefPtr<ossimMapProjection>  theProductProjection;
   ossimRefPtr<ossimImageChain>  theProductChain;
   ossimRefPtr<ossimTiling>      theTiling;
   ossimDrect        theOutputRect;
   bool              theBuildThumbnailFlag;
   ossimIpt          theThumbnailSize;
   long              theNumberOfTilesToBuffer;
   ossimKeywordlist  theKwl;
   bool              theTilingEnabled;
   bool              theProgressFlag;
   bool              theStdoutFlag;

};

#endif
