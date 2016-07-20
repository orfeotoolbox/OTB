//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef RadarSat2NoiseLevel_h
#define RadarSat2NoiseLevel_h

#include "ossimPluginConstants.h"
#include <ossim/base/ossimConstants.h>
#include <iosfwd>
#include <string>
#include <vector>

class ossimKeywordlist;

namespace ossimplugins
{

/**
 * @ingroup RadarSat2NoiseLevel
 * @brief This class represents an Noise
 */
class OSSIM_PLUGINS_DLL RadarSat2NoiseLevel
{
public:
   /**
    * @brief Constructor
    */
   RadarSat2NoiseLevel();

   /**
    * @brief Method to save object state to a keyword list.
    * @param kwl Keyword list to save to.
    * @param prefix added to keys when saved.
    * @return true on success, false on error.
    */
   bool saveState(ossimKeywordlist& kwl, std::string const& prefix="") const;

   /**
    * @brief Method to the load (recreate) the state of the object from a
    * keyword list. Return true if ok or false on error.
    * @return true if load OK, false on error
    */
   bool loadState (const ossimKeywordlist &kwl, std::string const& prefix="");
   /*!
    * METHOD: print()
    * Fulfills base-class pure virtual. Dumps contents of object to ostream.
    */
   std::ostream& print(std::ostream& out) const;


   void set_incidenceAngleCorrectionName(const std::string&  value)
   {
      m_incidenceAngleCorrectionName = value;
   }
   void set_pixelFirstNoiseValue(ossim_uint32 value)
   {
      m_pixelFirstNoiseValue = value;
   }
   void set_stepSize(ossim_uint32 value)
   {
      m_stepSize = value;
   }
   void set_numberOfNoiseLevelValues(ossim_uint32 value)
   {
      m_numberOfNoiseLevelValues = value;
   }
   void set_noiseLevelValues(const std::vector<ossim_float64>&  value)
   {
      m_noiseLevelValues = value;
   }
   void set_units(const std::string&  value)
   {
      m_units = value;
   }
   void set_offset(const ossim_float64&  value)
   {
      m_offset = value;
   }
   void set_gain(const std::string&  value)
   {
      m_gain = value;
   }

   const std::string & get_incidenceAngleCorrectionName() const
   {
      return m_incidenceAngleCorrectionName;
   }
   const std::string & get_gain() const
   {
      return m_gain;
   }

private:

   /**
    * @brief Incidence Angle correction Name
    */
   std::string                m_incidenceAngleCorrectionName;

   /**
    * @brief pixel First Noise Value
    */
   ossim_uint32               m_pixelFirstNoiseValue;
   /**
    * @brief step Size
    */
   ossim_uint32               m_stepSize;

   /**
    * @brief number Of Noise Level Values
    */
   ossim_uint32               m_numberOfNoiseLevelValues;

   /**
    * @brief noise Level Values
    */
   std::vector<ossim_float64> m_noiseLevelValues;

   /**
    * @brief noise Level units
    */
   std::string                m_units;

   /**
    * @brief offset value
    */
   ossim_float64              m_offset;

   /**
    * @brief lookup table
    */
   std::string                m_gain;

};
}

#endif
