//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Class definition for ossimWavelength.
//
//----------------------------------------------------------------------------

#include <ossim/support_data/ossimWavelength.h>
#include <ossim/base/ossimString.h>
#include <ossim/support_data/ossimEnviHeader.h>
#include <cmath>

ossimWavelength::ossimWavelength()
   : m_map()
{}

ossimWavelength::ossimWavelength( const ossimWavelength& obj )
   : m_map( obj.m_map )
{}

const ossimWavelength& ossimWavelength::operator=( const ossimWavelength& rhs )
{
   if ( this != &rhs )
   {
      m_map = rhs.m_map;
   }
   return *this;   
}

ossimWavelength::~ossimWavelength()
{
}
   
const ossimWavelength::WavelengthMap& ossimWavelength::getMap() const
{
   return m_map;
}
   
ossimWavelength::WavelengthMap& ossimWavelength::getMap()
{
   return m_map;
}

bool ossimWavelength::initialize( const ossimEnviHeader& hdr )
{
   //---
   // Example envi wavelength format:
   // wavelength = { 374.323608,  382.530487,  390.737427 }
   //---
   bool result = false;
   ossimString value;

   // Check the units...
   ossimString key = "wavelength units";
   if ( hdr.getValue( key, value ) )
   {
      if ( value.downcase() == "nanometers" )
      {
         // Check for wavelength key:
         key = "wavelength";
         if ( hdr.getValue( key, value ) )
         {
            if ( value.size() )
            {
               // Split into array.
               value.trim( ossimString("{}") );
               std::vector<ossimString> list;
               value.split( list, ossimString(","), true );

               if ( list.size() )
               {
                  // Initialize the map:
                  
                  std::vector<ossimString>::const_iterator i = list.begin();
                  ossim_uint32 band = 0;
                  ossim_float32 wavelength = 0.0;
            
                  while ( i != list.end() )
                  {
                     wavelength = (*i).toFloat64();
                     m_map.insert( std::make_pair( wavelength, band ) );
                     ++band;
                     ++i;
                  }
               }
            }
         }
      }
   }
   
   return ( m_map.size() ? true : false);
   
} // End: bool ossimWavelength::initialize(const ossimEnviHeader&)

using namespace std;
ossimWavelength::WavelengthMap::const_iterator ossimWavelength::findClosestIterator(
   const ossim_float64& requestedWavelength,
   const ossim_float64& thresholdFromCenter  ) const
{
   WavelengthMap::const_iterator result = m_map.lower_bound( requestedWavelength );
   if ( result != m_map.end() )
   {
      if ( result != m_map.begin() ) //  && (result->first > requestedWavelength) )
      {
         // Somewhere in the middle.
         WavelengthMap::const_iterator lower = result;
         --lower;

         ossim_float64 t = (requestedWavelength - lower->first) / (result->first -lower->first);
         if ( t < 0.5 )
         {
            result = lower;
         }
      }
   }
   else
   {
      --result;
   }
      
   if ( result != m_map.end() )
   {
      // Within threshold check.
      if ( std::fabs( result->first - requestedWavelength) > thresholdFromCenter )
      {
         result = m_map.end();
      }
      
   } // if ( result != m_map.end() )

   return result;
   
} // End: WavelengthMap::const_iterator ossimWavelength::findClosestIterator

ossim_int32 ossimWavelength::findClosestIndex(
   const ossim_float64& requestedWavelength, const ossim_float64& thresholdFromCenter  ) const
{
   ossim_int32 result = -1;
   WavelengthMap::const_iterator i =
      findClosestIterator( requestedWavelength, thresholdFromCenter );
   if ( i != m_map.end() )
   {
      result = (*i).second;
   }
   return result;
   
} // End: ossim_int32 ossimWavelength::findClosestIndex(...

bool ossimWavelength::getRgbBands( std::vector<ossim_uint32>& bands ) const
{
   bool result = false;

   if ( m_map.size() )
   {
      //---
      // Attempt to find bands with closest rgb wavelengths.
      // red:   620 - 750 nm
      // green: 495 - 570 nm
      // blue:  450 - 495 nm
      //---
      //const ossim_float32 RED_WAVELENGTH              = 439.978577; // 442.0;      
      //const ossim_float32 GREEN_WAVELENGTH            = 546.666504; // 546.0;
      //const ossim_float32 BLUE_WAVELENGTH             = 636.941406; // 637.0;
      const ossim_float32 RED_WAVELENGTH              = 685.0;      
      const ossim_float32 GREEN_WAVELENGTH            = 532.5;
      const ossim_float32 BLUE_WAVELENGTH             = 472.5;
      const ossim_float32 RED_THRESHOLD_FROM_CENTER   =  65.0;      
      const ossim_float32 GREEN_THRESHOLD_FROM_CENTER =  37.5;
      const ossim_float32 BLUE_THRESHOLD_FROM_CENTER  =  22.5;

      WavelengthMap::const_iterator r =
         findClosestIterator( RED_WAVELENGTH, RED_THRESHOLD_FROM_CENTER );
      WavelengthMap::const_iterator g =
         findClosestIterator( GREEN_WAVELENGTH, GREEN_THRESHOLD_FROM_CENTER );
      WavelengthMap::const_iterator b =
         findClosestIterator( BLUE_WAVELENGTH, BLUE_THRESHOLD_FROM_CENTER );

      if ( (r != m_map.end()) && (g != m_map.end()) && (b != m_map.end()) )
      {
         bands.resize(3);
         bands[0] = (*r).second;
         bands[1] = (*g).second;
         bands[2] = (*b).second;
         result = true;
      }
   }
   return result;
   
} // bool ossimWavelength::getRgbBands( std::vector<ossim_uint32>& ) const

ossimWavelength::WavelengthMap::const_iterator ossimWavelength::end() const
{
   return m_map.end();
}

ossimWavelength::WavelengthMap::iterator ossimWavelength::end()
{
   return m_map.end();
}
