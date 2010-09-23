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
// $Id: ossimIgen.h 17815 2010-08-03 13:23:14Z dburken $
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

   virtual void initialize(const ossimKeywordlist& kwl);
   virtual void outputProduct();
   
protected:
   virtual ~ossimIgen();
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
   ossimDrect        theOutputRect;
   bool              theBuildThumbnailFlag;
   ossimIpt          theThumbnailSize;
   long              theNumberOfTilesToBuffer;
   ossimKeywordlist  theKwl;
   bool              theTilingEnabled;
   ossimTiling       theTiling;
   bool              theProgressFlag;
   bool              theStdoutFlag;

};

#endif
