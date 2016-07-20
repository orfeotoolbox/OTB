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

#include "otb/RadarSat2NoiseLevel.h"
#include "ossimKeyWordListUtilities.h"
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <iomanip>


namespace ossimplugins
{
static const std::string NOISE                              = "referenceNoiseLevel";

static const std::string BETA_NOUGHT                        = "BetaNought";
static const std::string SIGMA_NOUGHT                       = "SigmaNought";
static const std::string GAMMA                              = "Gamma";

static const std::string INCIDENCE_ANGLE_CORRECTION_NAME_KW = "incidenceAngleCorrectionName";
static const std::string PIXEL_FIRST_NOISE_VALUE_KW         = "pixelFirstNoiseValue";
static const std::string STEP_SIZE_KW                       = "stepSize";
static const std::string NUMBER_OF_NOISE_LEVEL_VALUE_KW     = "numberOfNoiseLevelValues";
static const std::string NOISE_LEVEL_VALUES_KW              = "noiseLevelValues";
static const std::string UNITS_KW                           = "units";
static const std::string OFFSET_KW                          = "offset";
static const std::string GAIN_KW                            = "gain";
static const std::string SIZE_KW                            = "size";

RadarSat2NoiseLevel::RadarSat2NoiseLevel():
   m_incidenceAngleCorrectionName("Unknown"),
   m_pixelFirstNoiseValue(0),
   m_stepSize(0),
   m_numberOfNoiseLevelValues(0),
   m_noiseLevelValues(),
   m_units("Unknown"),
   m_offset(0.0),
   m_gain()
   {
   }

bool RadarSat2NoiseLevel::saveState(ossimKeywordlist& kwl, std::string const& prefix) const
{
   std::string pfx = prefix + NOISE;

   if(m_incidenceAngleCorrectionName == "Gamma")
   {
      pfx += '['+ GAMMA +']';
   }
   else if(m_incidenceAngleCorrectionName == "Sigma Nought")
   {
      pfx += '['+ SIGMA_NOUGHT +']';
   }
   else if(m_incidenceAngleCorrectionName == "Beta Nought")
   {
      pfx += '['+ BETA_NOUGHT +']';
   }

   pfx +='.';

   std::string s = pfx + INCIDENCE_ANGLE_CORRECTION_NAME_KW;
   add(kwl, s, m_incidenceAngleCorrectionName);

   s = pfx + PIXEL_FIRST_NOISE_VALUE_KW;
   add(kwl, s, m_pixelFirstNoiseValue);

   s = pfx + STEP_SIZE_KW;
   add(kwl, s, m_stepSize);

   s = pfx + NUMBER_OF_NOISE_LEVEL_VALUE_KW;
   add(kwl, s, m_numberOfNoiseLevelValues);

   s = pfx + UNITS_KW;
   add(kwl, s, m_units);

   for (unsigned int i = 0; i < m_noiseLevelValues.size(); ++i)
   {
      s = pfx + NOISE_LEVEL_VALUES_KW + "[" + ossimString::toString(i).chars() + "]";
      add(kwl, s, m_noiseLevelValues[i]);
   }

   s = pfx + OFFSET_KW;
   add(kwl, s, m_offset);

   s = pfx + GAIN_KW;
   add(kwl, s, m_gain);

   return true;
}

bool RadarSat2NoiseLevel::loadState(const ossimKeywordlist& kwl, std::string const& prefix)
{
   static const char MODULE[] = "RadarSat2NoiseLevel::loadState";

   //bool         result = true;
   ossimString  s;
   const char*  lookup = 0;

   std::string pfx = prefix + NOISE;

#if 0
   if(m_incidenceAngleCorrectionName == "Gamma")
   {
      pfx += "["+ GAMMA +"]";
   }
   else if(m_incidenceAngleCorrectionName == "Sigma Nought")
   {
      pfx += "["+ SIGMA_NOUGHT +"]";
   }
   else if(m_incidenceAngleCorrectionName == "Beta Nought")
   {
      pfx += "["+ BETA_NOUGHT +"]";
   }
#endif

#if 1
   std::string s1 = pfx + "["+ GAMMA +"]";
   std::string v  = kwl.findKey(s1, INCIDENCE_ANGLE_CORRECTION_NAME_KW);
   if (v.empty())
   {
      s1 = pfx + "["+ SIGMA_NOUGHT +"]";
      v  = kwl.findKey(s1, INCIDENCE_ANGLE_CORRECTION_NAME_KW);
      if (v.empty())
      {
         s1 = pfx + "["+ BETA_NOUGHT +"]";
         v  = kwl.findKey(s1, INCIDENCE_ANGLE_CORRECTION_NAME_KW);
         if (v.empty())
         {
            throw std::runtime_error("Cannot find "+INCIDENCE_ANGLE_CORRECTION_NAME_KW+" in KWL");
         }
      }
   }
   m_incidenceAngleCorrectionName = v;
#else
   std::string s1 = pfx + "["+ GAMMA +"]";
   lookup = kwl.find(s1.c_str(), INCIDENCE_ANGLE_CORRECTION_NAME_KW);
   if (!lookup)
   {
      std::string s1 = pfx + "["+ SIGMA_NOUGHT +"]";
      lookup = kwl.find(s1.c_str(), INCIDENCE_ANGLE_CORRECTION_NAME_KW);
      if (!lookup)
      {
         std::string s1 = pfx + "["+ BETA_NOUGHT +"]";
         lookup = kwl.find(s1.c_str(), INCIDENCE_ANGLE_CORRECTION_NAME_KW);
         if (!lookup)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " Keyword not found: " << INCIDENCE_ANGLE_CORRECTION_NAME_KW << "\n";
            return false;
         }
      }
   }

   m_incidenceAngleCorrectionName = lookup;
#endif

   pfx = s1;

#if 1
   get(kwl, pfx, PIXEL_FIRST_NOISE_VALUE_KW,     m_pixelFirstNoiseValue);
   get(kwl, pfx, STEP_SIZE_KW,                   m_stepSize);
   get(kwl, pfx, NUMBER_OF_NOISE_LEVEL_VALUE_KW, m_numberOfNoiseLevelValues);
   get(kwl, pfx, UNITS_KW,                       m_units);
#else
   lookup = kwl.find(s1.c_str(), PIXEL_FIRST_NOISE_VALUE_KW);
   if (lookup)
   {
      s = lookup;
      m_pixelFirstNoiseValue = s.toUInt32();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << PIXEL_FIRST_NOISE_VALUE_KW << "\n";
      return false;
   }

   lookup = kwl.find(s1.c_str(), STEP_SIZE_KW);
   if (lookup)
   {
      s = lookup;
      m_stepSize = s.toUInt32();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << STEP_SIZE_KW << "\n";
      return false;
   }

   lookup = kwl.find(s1.c_str(), NUMBER_OF_NOISE_LEVEL_VALUE_KW);
   if (lookup)
   {
      s = lookup;
      m_numberOfNoiseLevelValues = s.toUInt32();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << NUMBER_OF_NOISE_LEVEL_VALUE_KW << "\n";
      return false;
   }

   lookup = kwl.find(s1.c_str(), UNITS_KW);
   if (lookup)
   {
      m_units = lookup;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << UNITS_KW << "\n";
      return false;
   }
#endif

   m_noiseLevelValues.clear();
   for (unsigned int i = 0; i < m_numberOfNoiseLevelValues; ++i)
   {
#if 1
      s = pfx + NOISE_LEVEL_VALUES_KW + "[" + ossimString::toString(i) + "]";
      double coeff;
      get(kwl, s, coeff);
      m_noiseLevelValues.push_back( coeff );
#else
      lookup = kwl.find(s.c_str(), "");
      if (lookup)
      {
         s = lookup;
         m_noiseLevelValues.push_back( s.toDouble() );
      }
#endif
   }

   if( m_numberOfNoiseLevelValues != m_noiseLevelValues.size() )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE
         << " Keyword " << NOISE_LEVEL_VALUES_KW
         << " is different with the number of noise level values"
         << std::endl;
      return false;
   }

#if 1
   get(kwl, s1, GAIN_KW, m_gain);
   get(kwl, s1, OFFSET_KW, m_offset);
#else
   lookup = kwl.find(s1.c_str(), GAIN_KW);
   if (lookup)
   {
      m_gain = lookup;;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << GAIN_KW << "\n";
      return false;
   }

   lookup = kwl.find(s1.c_str(), OFFSET_KW);
   if (lookup)
   {
      s = lookup;
      m_offset = s.toFloat64();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << OFFSET_KW << "\n";
      return false;
   }
#endif

   if( m_numberOfNoiseLevelValues != m_noiseLevelValues.size() )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE
         << " Keyword " << NOISE_LEVEL_VALUES_KW
         << " is different with the number of noise level values"
         << std::endl;
      return false;
   }

   return true;
}


std::ostream& RadarSat2NoiseLevel::print(std::ostream& out) const
{
   ossimKeywordlist kwl;
   if (this->saveState(kwl))
   {
      out << setprecision(15) << setiosflags(ios::fixed)
         << "\n RadarSat2NoiseLevel class data members:\n";
      out << kwl;
   }
#if 0
   else
   {
      out.set_state(std::ios_base::badbit);
   }
#endif

   return out;
}

}
