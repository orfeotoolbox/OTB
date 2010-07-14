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
// Takes in DNs for any number of bands
// Converts DNs to Radiance at the satellite values Lsat
// Converts Lsat to Surface Reflectance values
//
//*************************************************************************
// $Id: ossimAtCorrRemapper.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <cstdlib>
#include <cmath>

#include <ossim/imaging/ossimAtCorrRemapper.h>
#include <ossim/imaging/ossimAtCorrKeywords.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageDataFactory.h>


RTTI_DEF1(ossimAtCorrRemapper, "ossimAtCorrRemapper", ossimImageSourceFilter);

static ossimTrace traceDebug("ossimAtCorrRemapper:debug");

ossimAtCorrRemapper::ossimAtCorrRemapper(ossimObject* owner,
                                         ossimImageSource* inputSource,
                                         const ossimString& sensorType)
      :
      ossimImageSourceFilter  (owner, inputSource),  // base class     
      theTile                 (NULL),
      theSurfaceReflectance   (NULL),
      theUseInterpolationFlag(false),
      theMinPixelValue        (0),  
      theMaxPixelValue        (0),
      theXaArray              (0),
      theXbArray              (0),
      theXcArray              (0),
      theBiasArray            (0),
      theGainArray            (0),
      theCalCoefArray         (0),
      theBandWidthArray       (0),
      theSensorType(sensorType)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();

   initialize();
}

ossimAtCorrRemapper::~ossimAtCorrRemapper()
{
   if (theSurfaceReflectance)
   {
      delete [] theSurfaceReflectance;
      theSurfaceReflectance = NULL;
   }
}

ossimRefPtr<ossimImageData> ossimAtCorrRemapper::getTile(
   const ossimIrect& tile_rect,
   ossim_uint32 resLevel)
{
#if 0
   if (traceDebug())
   {
      cout << "ossimAtCorrRemapper::getTile DEBUG:"
           << "\ntile_rect:  " << tile_rect << endl;
   }
#endif
   
   if (!isInitialized()||!theInputConnection)
   {
       cerr << "ossimAtCorrRemapper::getTile ERROR:"
            << "\nNot initialized!"
            << endl;
       return ossimRefPtr<ossimImageData>();
   }

   if(!theTile.valid())
   {
      initialize();
      if(!theTile)
      {
         return ossimRefPtr<ossimImageData>();
      }
   }
   
   // Fetch tile from pointer from the input source.
   ossimRefPtr<ossimImageData> inputTile = theInputConnection->getTile(tile_rect,
                                                                       resLevel);

   if (!inputTile.valid())  // Just in case...
   {
      return ossimRefPtr<ossimImageData>();
   }

   // Check for remap bypass or empty / null input tile.
   ossimDataObjectStatus tile_status = inputTile->getDataObjectStatus();
   if (!theEnableFlag || tile_status == OSSIM_NULL ||
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
      if(theSurfaceReflectance)
      {
         delete [] theSurfaceReflectance;
         theSurfaceReflectance = NULL;
      }
   }
   
   if(!theSurfaceReflectance)
   {
      ossim_uint32 size  = tw*th*bands;
#if 0
      if (traceDebug())
      {
         cout << "ossimAtCorrRemapper::getTile DEBUG:"
              << "\ntile_rect:    " << tile_rect
              << "\ntile width:   " << tw
              << "\ntile height:  " << th
              << "\nbands:        " << bands
              << "\nBuffer size:  " << size << endl;
      }
#endif
      
      theSurfaceReflectance = new double[size];
   }

   ossim_uint32 buffer_index = 0;
   ossimIpt ul = tile_rect.ul();
   ossimIpt lr = tile_rect.lr();
   const double MP = theTile->getMinNormalizedPix(); // Minimum normalized pix.
   double a, b, c;
   buffer_index = 0;

   cout << setprecision(6);
   for (ossim_uint32 band=0; band < bands; ++band)
   {
      for(ossim_sint32 idxy = ul.y; idxy <= lr.y; ++idxy)
      {
         for(ossim_sint32 idxx = ul.x; idxx <= lr.x; ++idxx)
         {
            double p = inputTile->getPix(buffer_index);
            
            if (p>0.0)
            {
               if(!theUseInterpolationFlag)
               {
                  a = theXaArray[band];
                  b = theXbArray[band];
                  c = theXcArray[band];
               }
               else
               {
                  interpolate(ossimDpt(idxx, idxy),
                              band,
                              a,
                              b,
                              c);
               }
               if(theSensorType == "ls7ms")
               {
                  double radiance_at_satellite
                     = (theGainArray[band] * p) + theBiasArray[band];
                  
                  double y =  (radiance_at_satellite * a) -  b;
                  
                  p = (y / (1.0 + (c * y)) );
               }
               else if(theSensorType == "qbms")
               {
                  double radiance_at_satellite
                     = theCalCoefArray[band] * p / theBandWidthArray[band];
                  
                  double y =  (radiance_at_satellite * a) - b;
                  
                  p = (y / (1.0 + (c * y)) );
               }
               else if(theSensorType == "ikms")
               {
                  
                 
                  double radiance_at_satellite
                     =   p  /((theCalCoefArray[band]/1.0)/ theBandWidthArray[band]);
                  double y =  (radiance_at_satellite * a) -  b;
    
                  p = (y / (1.0 + (c * y)) );
     
               }  
                   
               // Note that "p" should now be normalized between 0.0 and 1.0;
               
               // ***
               // Since it wasn't null to start with clip / clamp between minimum
               // normalized pixel and one(max).
               // ***
               p =  ( p > MP ? ( p < 1.0 ? p : 1.0) : MP );
               
               // Scan the new tile and set the min / max.
               if (p < theMinPixelValue[band])
               {
                  theMinPixelValue[band] = p;
               }
               else if (p > theMaxPixelValue[band])
               {
                  theMaxPixelValue[band] = p;
               }
               
               theSurfaceReflectance[buffer_index] = p;
            }
            else
            {
               theSurfaceReflectance[buffer_index] = 0.0;  // pixel was null...
            }
            
            ++buffer_index;
            
         }  // End of sample loop...
         
      } // End of line loop...
      
   } // End of band loop...

   // Copy the buffer to the output tile at the same time unnormalizing it.
   theTile->copyNormalizedBufferToTile(theSurfaceReflectance);
   
   // Validate the output to set the tile status.
   theTile->validate();
   
   return theTile;
}

void ossimAtCorrRemapper::initialize()
{
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          theInputConnection);
      theTile->initialize();

      if(theSurfaceReflectance)
      {
         delete []theSurfaceReflectance;
         theSurfaceReflectance = NULL;
      }
      
      ossim_uint32 tw    = theTile->getWidth();
      ossim_uint32 th    = theTile->getHeight();
      ossim_uint32 bands = theTile->getNumberOfBands();
      ossim_uint32 size  = tw*th*bands;
      if (traceDebug())
      {
         cout << "ossimAtCorrRemapper::initialize DEBUG:"
              << "\ntile width:   " << tw
              << "\ntile height:  " << th
              << "\nbands:        " << bands
              << "\nBuffer size:  " << size << endl;
      }
      
      theSurfaceReflectance = new double[size];

      setInitializedFlag(true);
      clearErrorStatus();
   }
   else
   {
      setInitializedFlag(false);
      setErrorStatus();
   };

   verifyEnabled();

   if (traceDebug())
   {
      cout << "ossimAtCorrRemapper::initialize DEBUG:"
           << *this
           << endl;
   }

}

bool ossimAtCorrRemapper::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   static const char MODULE[] = "ossimAtCorrRemapper::loadState()";

   if (traceDebug())  CLOG << "entering..." << endl;

   if (!theTile || !theSurfaceReflectance)
   {
      cerr << MODULE << " ERROR:"
           << "Not initialized..." << endl;
      return false;
   }

   ossim_uint32 bands = theTile->getNumberOfBands();

   // Clear out the old values.
   theMinPixelValue.clear();
   theMaxPixelValue.clear();   
   theXaArray.clear();
   theXbArray.clear();
   theXcArray.clear();
   theBiasArray.clear();
   theGainArray.clear();
   theCalCoefArray.clear();
   theBandWidthArray.clear();
 
   
   // Now resize them.

   // Start with arbitrary big number.
   theMinPixelValue.resize(bands, 1.0);

   // Start with arbitrary small number.
   theMaxPixelValue.resize(bands, 0.0);

   
   theXaArray.resize(bands, 1.0);
   theXbArray.resize(bands, 1.0);
   theXcArray.resize(bands, 1.0);
   
   theBiasArray.resize(bands, 0.0);
   theGainArray.resize(bands, 1.0);
   theCalCoefArray.resize(bands);
   theBandWidthArray.resize(bands);
   
   for(ossim_uint32 band = 0; band < bands; ++band)
   {
      const char* lookup = NULL;
      ossimString band_string = ".band";
      band_string += ossimString::toString(band+1);
      
      ossimString kw = AT_CORR_XA_KW;
      kw += band_string;
      lookup = kwl.find(prefix, kw.c_str());
      if (lookup)
      {
         theXaArray[band] = atof(lookup);
      }
      else
      {
         if (traceDebug())
         {
            CLOG << "DEBUG:"
                 << "\nlookup failed for keyword:  " << kw.c_str() << endl;
         }
      }
      
      kw = AT_CORR_XB_KW;
      kw += band_string;
      lookup = kwl.find(prefix, kw.c_str());
      if (lookup)
      {
         theXbArray[band] = atof(lookup);
      }
      else
      {
         if (traceDebug())
         {
            CLOG << "DEBUG:"
                 << "\nlookup failed for keyword:  " << kw.c_str()
                 << endl;
         }
      }
      
      kw = AT_CORR_XC_KW;
      kw += band_string;
      lookup = kwl.find(prefix, kw.c_str());
      if (lookup)
      {
         theXcArray[band] = atof(lookup);
      }
      else
      {
         if (traceDebug())
         {
            CLOG << "DEBUG:"
                 << "\nlookup failed for keyword:  " << kw.c_str()
                 << endl;
         }
      }
      
      if(theSensorType == "ls7ms")
      {
         kw = AT_CORR_BIAS_KW;
         kw += band_string;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            theBiasArray[band] = atof(lookup);
         }
         else
         {
            if (traceDebug())
            {
               CLOG << "DEBUG:"
                    << "\nlookup failed for keyword:  " << kw.c_str()
                    << endl;
            }
         }
         
         kw = AT_CORR_GAIN_KW;
         kw += band_string;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            theGainArray[band] = atof(lookup);
         }
         else
         {
            if (traceDebug())
            {
               CLOG << "DEBUG:"
                    << "\nlookup failed for keyword:  " << kw.c_str()
                    << endl;
            }
         }
      }
      
      if(theSensorType == "qbms")
      {
         kw = AT_CORR_CALCOEF_KW;
         kw += band_string;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            theCalCoefArray[band] = atof(lookup);
         }
         else
         {
            if (traceDebug())
            {
               CLOG << "DEBUG:"
                    << "\nlookup failed for keyword:  " << kw.c_str()
                    << endl;
            }
         }
         
         kw = AT_CORR_BANDWIDTH_KW;
         kw += band_string;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            theBandWidthArray[band] = atof(lookup);
         }
         else
         {
            if (traceDebug())
            {
               CLOG << "DEBUG:"
                    << "\nlookup failed for keyword:  " << kw.c_str()
                    << endl;
            }
         }
      }
      if(theSensorType == "ikms")
      {
         kw = AT_CORR_CALCOEF_KW;
         kw += band_string;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            theCalCoefArray[band] = atof(lookup);
         }
         else
         {
            if (traceDebug())
            {
               CLOG << "DEBUG:"
                    << "\nlookup failed for keyword:  " << kw.c_str()
                    << endl;
            }
         }
         
         kw = AT_CORR_BANDWIDTH_KW;
         kw += band_string;
         lookup = kwl.find(prefix, kw.c_str());
         if (lookup)
         {
            theBandWidthArray[band] = atof(lookup);
         }
         else
         {
            if (traceDebug())
            {
               CLOG << "DEBUG:"
                    << "\nlookup failed for keyword:  " << kw.c_str()
                    << endl;
            }
         }
      }
   }
   
   verifyEnabled();

   if (theEnableFlag)
   {
      //***
      // Call the base class to pick up the enable flag.  Note that this
      // can override the state set from verifyEnabled() method.
      //***
      ossimString pref;
      if (prefix) pref += prefix;
      pref += "atmospheric_correction.";

   }
   
   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << *this
           << "returning..."
           << endl;
   }
   
   return true;
}

void ossimAtCorrRemapper::verifyEnabled()
{
   // Check all the pointers...
   if ( !theInputConnection    || !theTile ||
        !theSurfaceReflectance )
   {
      disableSource();
      return;
   }

   ossim_uint32 bands = theTile->getNumberOfBands();
   if ( (theMinPixelValue.size() != bands) ||
        (theMaxPixelValue.size() != bands) ||
        (theXaArray.size()       != bands) ||
        (theXbArray.size()       != bands) ||
        (theXcArray.size()       != bands) ||
        (theBiasArray.size()     != bands) ||
        (theGainArray.size()     != bands) ||
        (theCalCoefArray.size()  != bands) ||
        (theBandWidthArray.size()!= bands))
   {
      disableSource(); 
      return;
   }

   enableSource();
}

ossimString ossimAtCorrRemapper::getShortName() const
{
   return ossimString("Atmospheric Correction Remapper");
}

vector<double> ossimAtCorrRemapper::getNormMinPixelValues() const
{
   return theMinPixelValue;
}

vector<double> ossimAtCorrRemapper::getNormMaxPixelValues() const
{
   return theMaxPixelValue;
}

void ossimAtCorrRemapper::getNormMinPixelValues(vector<double>& v) const
{
   v = theMinPixelValue;
}

void ossimAtCorrRemapper::getNormMaxPixelValues(vector<double>& v) const
{
   v = theMaxPixelValue;
}

const ossimString& ossimAtCorrRemapper::getSensorType() const
{
   return theSensorType;
}

void ossimAtCorrRemapper::setSensorType(const ossimString& sensorType)
{
   theSensorType = sensorType;
}

void ossimAtCorrRemapper::interpolate(const ossimDpt& /* pt */,
                                      int band,
                                      double& a,
                                      double& b,
                                      double& c)const
{
   a = theXaArray[band];
   b = theXbArray[band];
   c = theXcArray[band];
}

ostream& ossimAtCorrRemapper::print(ostream& os) const
{
   os << "ossimAtCorrRemapper:"
      << "\ntheEnableFlag:  " << (theEnableFlag?"enabled":"disabled")
      << endl;

   os << setprecision(15) << setiosflags(ios::fixed);

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
   i = theXaArray.begin();
   while (i != theXaArray.end())
   {
      os << "band[" << band << "] xa:  " << (*i) << endl;
      ++i;
      ++band;
   }

   band = 1;
   i = theXbArray.begin();
   while (i != theXbArray.end())
   {
      os << "band[" << band << "] xb:  " << (*i) << endl;
      ++i;
      ++band;
   }

   band = 1;
   i = theXcArray.begin();
   while (i != theXcArray.end())
   {
      os << "band[" << band << "] xc:  " << (*i) << endl;
      ++i;
      ++band;
   }

   if(theSensorType == "ls7ms")
   {
      band = 1;
      i = theBiasArray.begin();
      while (i != theBiasArray.end())
      {
         os << "band[" << band << "] bias:  " << (*i) << endl;
         ++i;
         ++band;
      }
      
      band = 1;
      i = theGainArray.begin();
      while (i != theGainArray.end())
      {
         os << "band[" << band << "] gain:  " << (*i) << endl;
         ++i;
         ++band;
      }
   }
   if(theSensorType == "qbms")
   {
      band = 1;
      i = theCalCoefArray.begin();
      while (i != theCalCoefArray.end())
      {
         os << "band[" << band << "] calcoef:  " << (*i) << endl;
         ++i;
         ++band;
      }

      band = 1;
      i = theBandWidthArray.begin();
      while (i != theBandWidthArray.end())
      {
         os << "band[" << band << "] bandwidth:  " << (*i) << endl;
         ++i;
         ++band;
      }
   }
   if(theSensorType == "ikms")
   {
      band = 1;
      i = theCalCoefArray.begin();
      while (i != theCalCoefArray.end())
      {
         os << "band[" << band << "] calcoef:  " << (*i) << endl;
         ++i;
         ++band;
      }

   }
   return os;
}

ostream& operator<<(ostream& os, const ossimAtCorrRemapper& hr)
{
   return hr.print(os);
}


