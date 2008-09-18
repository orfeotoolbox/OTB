//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kathy Minear
//
// Description: Takes in DNs for any number of bands
// Converts DNs to Radiance at the satellite values Lsat
// Converts Lsat to Surface Reflectance values
//
//*************************************************************************
// $Id: ossimGammaRemapper.cpp 12980 2008-06-04 00:50:33Z dburken $

#include <cstdlib>
#include <cmath>

#include <ossim/imaging/ossimGammaRemapper.h>
#include <ossim/imaging/ossimAtCorrKeywords.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/imaging/ossimImageDataFactory.h>


RTTI_DEF1(ossimGammaRemapper, "ossimGammaRemapper", ossimImageSourceFilter)

static const double DEFAULT_GAMMA = 1.0;


static ossimTrace traceDebug("ossimGammaRemapper:debug");

ossimGammaRemapper::ossimGammaRemapper(ossimObject* owner)
   :
      ossimImageSourceFilter  (owner),  // base class
      theTile                 (NULL),
      theBuffer               (NULL), 
      theMinPixelValue        (0),
      theMaxPixelValue        (0),
      theGamma                (0),
      theUserDisabledFlag     (false)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();

   // Construction not complete.
}


ossimGammaRemapper::ossimGammaRemapper(ossimImageSource* inputSource)
   :
      ossimImageSourceFilter  (NULL, inputSource),  // base class
      theTile                 (NULL),
      theBuffer               (NULL),
      theMinPixelValue        (0),
      theMaxPixelValue        (0),
      theGamma                (0),
      theUserDisabledFlag     (false)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();

   if (inputSource == NULL)
   {
      setErrorStatus();
      cerr << "ossimGammaRemapper::ossimGammaRemapper ERROR:"
           << "\nNull input source passed to constructor!" << endl;
      return;
   }

   initialize();
}


ossimGammaRemapper::ossimGammaRemapper(ossimObject* owner,
                                         ossimImageSource* inputSource)
   :
      ossimImageSourceFilter  (owner, inputSource),  // base class     
      theTile                 (NULL),
      theBuffer               (NULL),
      theMinPixelValue        (0),
      theMaxPixelValue        (0),
      theGamma                (0)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();

   if (inputSource == NULL)
   {
      setErrorStatus();
      cerr << "ossimGammaRemapper::ossimGammaRemapper ERROR:"
           << "\nNull input source passed to constructor!" << endl;
      return;
   }

   initialize();
}

ossimGammaRemapper::~ossimGammaRemapper()
{
   if (theBuffer)
   {
      delete [] theBuffer;
      theBuffer = NULL;
   }
}

ossimRefPtr<ossimImageData> ossimGammaRemapper::getTile(const ossimIrect& tile_rect,
                                            ossim_uint32 resLevel)
{
   if (!isInitialized())
   {
      cerr << "ossimGammaRemapper::getTile ERROR:"
           << "\nNot initialized!"
           << endl;
      return ossimRefPtr<ossimImageData>();
   }
      
   // Fetch tile from pointer from the input source.
   ossimRefPtr<ossimImageData> inputTile = theInputConnection->getTile(
      tile_rect, resLevel);

   if (!inputTile.valid())  // Just in case...
   {
      cerr << "ossimGammaRemapper::getTile ERROR:"
           << "\nReceived null pointer to tile from input source!"
           << "\nReturning blank tile."
           << endl;
      theTile->makeBlank();
      return theTile;
   }

   // Get its status.
   ossimDataObjectStatus tile_status = inputTile->getDataObjectStatus();
   
   //---
   // Check for remap bypass:
   //---
   if (!theEnableFlag || tile_status == OSSIM_NULL ||!theTile.valid() ||
       tile_status == OSSIM_EMPTY)
   {
      return inputTile;
   }

   ossim_uint32 w     = tile_rect.width();
   ossim_uint32 h     = tile_rect.height();
   ossim_uint32 tw    = theTile->getWidth();
   ossim_uint32 th    = theTile->getHeight();
   ossim_uint32 bands = theTile->getNumberOfBands();

   // Set the origin of the output tile.
   theTile->setOrigin(tile_rect.ul());

   if(w*h != tw*th)
   {
      theTile->setWidthHeight(w, h);
      theTile->initialize();
      if(theBuffer)
      {
         delete [] theBuffer;
         theBuffer = NULL;
      }
   }   
   
   if(!theBuffer)
   {
      theBuffer = new double[w*h*bands];
   }
   
   // Copy the source tile into the buffer at the same time normalizing it.
   inputTile->copyTileToNormalizedBuffer(theBuffer);
   
   // for each band, get the radiance value

   ossim_uint32 buffer_index = 0;
   const double MP = theTile->getMinNormalizedPix(); // Minimum normalized pix.
   const ossim_uint32 PPTB = theTile->getSizePerBand();  // Pixels Per Tile Band

   for (ossim_uint32 band=0; band<bands; ++band)
   {
      for (ossim_uint32 i=0; i<PPTB; ++i)
      {
         double p = theBuffer[buffer_index]; // input pixel
         // double p = getPix(buffer_index);

         if (p)
         {
            // cout<<"p before:  "<<p<<endl;
            // Stretch it...
            p = (p - theMinPixelValue[band]) /
               (theMaxPixelValue[band] -
                theMinPixelValue[band]);
            
            p = pow(p, theGamma[band]);
           
            
            //***
            // Since it wasn't null to start with clip / clamp between minimum
            // normalized pixel and one(max).
            //*** 
            p =  ( p > MP ? ( p < 1.0 ? p : 1.0) : MP );  

            theBuffer[buffer_index] = p;
         }
         else
         {
            theBuffer[buffer_index] = 0.0;
         }
         
         ++buffer_index;
      }
   }
   
   // Copy the buffer to the output tile at the same time unnormalizing it.
   theTile->copyNormalizedBufferToTile(theBuffer);
   
   // Set the status to that of the input tile.
   theTile->setDataObjectStatus(tile_status);

   return theTile;
}

void ossimGammaRemapper::initialize()
{
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          theInputConnection);
      theTile->initialize();

      if(theBuffer)
      {
         delete []theBuffer;
         theBuffer = NULL;
      }
      
      ossim_uint32 tw    = theTile->getWidth();
      ossim_uint32 th    = theTile->getHeight();
      ossim_uint32 bands = theTile->getNumberOfBands();
      
      theBuffer = new double[tw*th*bands];
      memset(theBuffer, '\0', tw*th*bands);

      setInitializedFlag(true);
      clearErrorStatus();
   }
   else
   {
      setInitializedFlag(false);
      setErrorStatus();
      cerr << "ossimGammaRemapper::initialize ERROR:"
           << "\nCannot call method when input connection is NULL!"
           << endl;
   };

   verifyEnabled();
}

void ossimGammaRemapper::setMinMaxPixelValues(const vector<double>& v_min,
                                              const vector<double>& v_max)
{
   theMinPixelValue = v_min;
   theMaxPixelValue = v_max;
   verifyEnabled();
}

void ossimGammaRemapper::verifyEnabled()
{
   // Check all the pointers...
   if ( !theInputConnection || !theTile || !theBuffer )
   {
      disableSource();
      return;
   }

   ossim_uint32 bands = theTile->getNumberOfBands();
   if ( (theMinPixelValue.size() != bands) ||
        (theMaxPixelValue.size() != bands) )
   {
      disableSource(); 
      return;
   }

   if (theUserDisabledFlag == false)
   {
      enableSource();
   }

   if (traceDebug())
   {
      cout << *this << endl;
   }
}

bool ossimGammaRemapper::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   //***
   // Call the base class to pick up the enable flag.  Note that the
   // verifyEnabled flag can override this.
   //***
   ossimString pref;
   if (prefix) pref += prefix;
   pref += "gamma_remapper.";

   if (!theTile)
   {
      cerr << "ossimGammaRemapper::loadState:  ERROR"
           << "Not initialized yet!" << endl;
      return false;
   }

   //---
   // NOTE:
   // base class seems to call initialize which in turn errors if you don't
   // have a connection yet, so check for the enable keyword here...
   // ossimSource::loadState(kwl, pref.c_str());
   //---
   cout << "pref:  " << pref
        << "kw:  " << ossimKeywordNames::ENABLED_KW    << endl;
   
   const char* lookup = kwl.find(pref, ossimKeywordNames::ENABLED_KW);
   if(lookup)
   {
      theEnableFlag = ossimString(lookup).toBool();
      if (theEnableFlag == false)
      {
         // User want filter disabled...
         theUserDisabledFlag = true;
      }
   }

   ossim_uint32 bands = theTile->getNumberOfBands();

   theGamma.clear();
   theGamma.resize(bands, 1.0);

   for(ossim_uint32 band = 0; band < bands; ++band)
   {
      ossimString band_string = ".band";
      band_string += ossimString::toString(band+1);  // Start at one.

      ossimString kw = GAMMA_REMAPPER_GAMMA_KW;
      kw += band_string;
      lookup = kwl.find(prefix, kw.c_str());
      if (lookup)
      {
         theGamma[band] = atof(lookup);
      }
      else
      {
         cout << "MODULE NOTICE:"
              << "\nlookup failed for keyword:  " << kw.c_str()
              << "\nGamma set to " << DEFAULT_GAMMA << " for band:  "
              << (band+1) << endl;
         theGamma[band] = DEFAULT_GAMMA;
      }
   }

   if (traceDebug())
   {
      cout << "ossimGammaRemapper DEBUG:"
           << *this
           << endl;
   }
   
   return true;
}

ostream& ossimGammaRemapper::print(ostream& os) const
{
   os << setprecision(15) << setiosflags(ios::fixed)
      << "ossimGammaRemapper:"
      << "\ntheEnableFlag:  " << (theEnableFlag?"enabled":"disabled")
      << endl;

   ossim_uint32 band = 1;
   vector<double>::const_iterator i = theMinPixelValue.begin();
   while (i != theMinPixelValue.end())
   {
      os << "band[" << band << "] min:  " << (*i) << endl;
      ++i;
      ++band;
   }

   band = 1;
   i = theMaxPixelValue.begin();
   while (i != theMaxPixelValue.end())
   {
      os << "band[" << band << "] max:  " << (*i) << endl;
      ++i;
      ++band;
   }
   
   band = 1;
   i = theGamma.begin();
   while (i != theGamma.end())
   {
      os << "band[" << band << "] gamma:  " << (*i) << endl;
      ++i;
      ++band;
   }

   return os;
}

void ossimGammaRemapper::enableSource()
{
   // Clear the flag...
   theUserDisabledFlag = false;
   ossimSource::enableSource();
}

ossimString ossimGammaRemapper::getShortName() const
{
   return ossimString("Gamma Remapper");
}

ostream& operator<<(ostream& os, const ossimGammaRemapper& hr)
{
   return hr.print(os);
}
