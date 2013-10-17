//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Class declaration for ossimWavelength.
// See class description below.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimWavelength_HEADER
#define ossimWavelength_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <iostream>
#include <map>
#include <vector>


class ossimEnviHeader;

/**
 * @class ossimWavelenght class.
 *
 * Simple container class encapsulating a std::map of wavelengths(key) and associated
 * bands(value), with convenience methods.  Wavelengths values are 64 bit
 * floating point in nanometers. Bands are zero based
 * 32 bit unsigned integers. 
 */
class OSSIM_DLL ossimWavelength 
{
public:
   
   typedef std::map< ossim_float64, ossim_uint32 > WavelengthMap;
   
   /** @brief default constructor */
   ossimWavelength();

   /** @brief copy constructor */
   ossimWavelength( const ossimWavelength& obj );

   /** @brief assignment operator= */
   const ossimWavelength& operator=( const ossimWavelength& rhs );

   /** @brief virtual destructor */
   ~ossimWavelength();
   
   const ossimWavelength::WavelengthMap& getMap() const;
   
   ossimWavelength::WavelengthMap& getMap();

   /**
    * @brief Initializes map from ENVI header class.
    *
    * This will clear any existing map, look for the keywords
    * "wavelength units" and "wavelength".
    * 
    * @param header to initialize from.
    * @return true on success, false on error.
    */
   bool initialize( const ossimEnviHeader& hdr );

   /**
    * @brief Finds iterator closest to wavelength.
    * @param requestedWavelength Requested wavelength in nanometers.
    * @param thresholdFromCenter in nanometers.
    * @return WavelengthMap::const_iterator if not found will return. 
    */
   WavelengthMap::const_iterator findClosestIterator(
      const ossim_float64& requestedWavelength,
      const ossim_float64& thresholdFromCenter  ) const;
   
   /**
    * @brief Finds index closest to wavelength.
    * @param requestedWavelength Requested wavelength in nanometers.
    * @param thresholdFromCenter in nanometers.
    * @return Closest zero based index to wavelength or -1 if not found. 
    */
   ossim_int32 findClosestIndex(
      const ossim_float64& requestedWavelength,
      const ossim_float64& thresholdFromCenter  ) const;
   
   /**
    * @brief Gets rgb bands if "wavelength" keyword is present.
    * @param bands Initialized by this with zero base rbg band
    * indexes.
    * @return true on success, false if bands not found.
    */   
   bool getRgbBands( std::vector<ossim_uint32>& bands ) const;

   /** @return WavelengthMap::const_iterator of underlying map. */
   WavelengthMap::const_iterator end() const;

   /** @return WavelengthMap::iterator of underlying map. */
   WavelengthMap::iterator end();

private:
   
   WavelengthMap m_map;
};

#endif /* End of "#ifndef ossimWavelength_HEADER" */
