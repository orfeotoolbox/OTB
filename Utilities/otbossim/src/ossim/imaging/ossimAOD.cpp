//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kathy Minear
//
// Description:
// 
// Class to compute Aerosol Optical Depth (AOD) for with atmospheric
// correction.
// 
//
//*************************************************************************
// $Id: ossimAOD.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <math.h>
#include <ossim/imaging/ossimAOD.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimU8ImageData.h>


RTTI_DEF1(ossimAOD, "ossimAOD", ossimImageSourceFilter)

static ossimTrace traceDebug("ossimAOD:debug");

ossimAOD::ossimAOD(ossimObject* owner)
   :
      ossimImageSourceFilter  (owner),  // base class
      theTile                 (NULL)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();

   // Construction not complete.
}

ossimAOD::ossimAOD(ossimImageSource* inputSource)
   :
      ossimImageSourceFilter  (NULL, inputSource),  // base class
      theTile                 (NULL)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();

   if (inputSource == NULL)
   {
      setErrorStatus();
      cerr << "ossimAOD::ossimAOD ERROR:"
           << "\nNull input source passed to constructor!" << endl;
      return;
   }

   initialize();
}


ossimAOD::ossimAOD(ossimObject* owner,
                                         ossimImageSource* inputSource)
   :
      ossimImageSourceFilter  (owner, inputSource),  // base class     
      theTile                 (NULL)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();

   if (inputSource == NULL)
   {
      setErrorStatus();
      cerr << "ossimAOD::ossimAOD ERROR:"
           << "\nNull input source passed to constructor!" << endl;
      return;
   }

   initialize();
}

ossimAOD::~ossimAOD()
{
}

ossimRefPtr<ossimImageData> ossimAOD::getTile(const ossimIrect& tile_rect,
                                              ossim_uint32 resLevel)
{
   if (!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();
   }
      
   // Fetch tile from pointer from the input source.
   ossimRefPtr<ossimImageData> inputTile = theInputConnection->getTile(tile_rect,
                                                                       resLevel);


   if (!inputTile.valid())  // Just in case...
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimAOD::getTile ERROR:"
         << "\nReceived null pointer to tile from input source!"
         << "\nReturning blank tile."
         << endl;
      return inputTile;
   }

   ossimDataObjectStatus tile_status = inputTile->getDataObjectStatus();

   if ( !theEnableFlag ||
        (tile_status == OSSIM_NULL) ||
        (tile_status == OSSIM_EMPTY) )
   {
      return inputTile;
   }
   
   if (!theTile.valid())
   {
      allocate();
      if (!theTile.valid())
      {
         return inputTile;
      }
   }

   ossim_uint32 w     = tile_rect.width();
   ossim_uint32 h     = tile_rect.height();
   ossim_uint32 tw    = theTile->getWidth();
   ossim_uint32 th    = theTile->getHeight();
   // ossim_uint32 bands = theTile->getNumberOfBands();

   // Set the origin of the output tile.
   theTile->setOrigin(tile_rect.ul());

   if(w*h != tw*th)
   {
      theTile->setWidthHeight(w, h);
      theTile->initialize();
   }
   return theTile;
}

void ossimAOD::initialize()
{
   ossimImageSourceFilter::initialize();
}

void ossimAOD::allocate()
{
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          theInputConnection);
      theTile->initialize();
      setInitializedFlag(true);
      clearErrorStatus();
   }
   else
   {
      setInitializedFlag(false);
      setErrorStatus();
      cerr << "ossimAOD::initialize ERROR:"
           << "\nCannot call method when input connection is NULL!"
           << endl;
   };

   verifyEnabled();
}

bool ossimAOD::loadState(const ossimKeywordlist& /* kwl */, const char* /* prefix */)
{
   static const char MODULE[] = "ossimAOD::loadState()";

   if (traceDebug())  CLOG << "entering..." << endl;

   if (!theTile)
   {
      cerr << MODULE << " ERROR:"
           << "Not initialized..." << endl;
      return false;
   }

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << *this
           << "\nreturning..."
           << endl;
   }
   
   return true;
}


void ossimAOD::verifyEnabled()
{
   // Check all the pointers...
   if ( !theInputConnection    || !theTile )
   {
      disableSource();
      return;
   }

   enableSource();
}

ostream& ossimAOD::print(ostream& os) const
{
   os << "ossimAOD:"
      << "\ntheEnableFlag:  " << (theEnableFlag?"enabled":"disabled")
      << endl;

   return os;
}

ostream& operator<<(ostream& os, const ossimAOD& hr)
{
   return hr.print(os);
}

void ossimAOD::writeTemplate(ostream& /* os */)
{
}

ossimString ossimAOD::getShortName() const
{
   return ossimString("Aerosol Optical Depth");
}
