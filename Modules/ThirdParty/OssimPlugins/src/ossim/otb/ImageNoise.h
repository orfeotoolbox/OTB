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

#ifndef ImageNoise_h
#define ImageNoise_h

#include <ossimPluginConstants.h>
#include <vector>
#include <string>


class ossimKeywordlist;

namespace ossimplugins
{

/**
 * @ingroup ImageNoise
 * @brief This class represents an ImageNoise
 */
class OSSIM_PLUGINS_DLL ImageNoise
{
public:
   /// Constructor
   ImageNoise();

   /**
    * Saves state to a keyword list.
    * @param[in,out] kwl Keyword list to save to.
    * @param[in] prefix added to keys when saved.
    * @return true
    * @throw std::runtime_error if a field cannot be encoded
    */
   bool saveState(ossimKeywordlist& kwl, std::string const& prefix = "") const;

   /**
    * Loads (recreate) the state of the object from a keyword list.
    * @return true
    * @throw std::runtime_error if a field cannot be decoded
    */
   bool loadState (const ossimKeywordlist &kwl, std::string const& prefix = "");

   void set_timeUTC(const std::string& value)
   {
      m_timeUTC = value;
   }

   const std::string & get_timeUTC(void) const
   {
      return m_timeUTC;
   }

   void set_validityRangeMin(double value)
   {
      m_validityRangeMin = value;
   }

   double get_validityRangeMin(void) const
   {
      return m_validityRangeMin;
   }

   void set_validityRangeMax(double value)
   {
      m_validityRangeMax = value;
   }

   double get_validityRangeMax(void) const
   {
      return m_validityRangeMax;
   }

   void set_referencePoint(double value)
   {
      m_referencePoint = value;
   }

   double get_referencePoint(void) const
   {
      return m_referencePoint;
   }

   void set_polynomialDegree(unsigned int value)
   {
      m_polynomialDegree = value;
   }

   unsigned int get_polynomialDegree() const
   {
      return m_polynomialDegree;
   }


   void set_polynomialCoefficients(const std::vector<double>& value)
   {
      m_polynomialCoefficients = value;
   }

   const std::vector<double> & get_polynomialCoefficients(void) const
   {
      return m_polynomialCoefficients;
   }

private:

   /// TimeUTC (Noise/ImageNoise node).
   std::string m_timeUTC;

   /// ValidityRangeMin (Noise/ImageNoise node).
   double m_validityRangeMin;
   /// ValidityRangeMax (Noise/ImageNoise node).
   double m_validityRangeMax;
   /// Reference point (Noise/ImageNoise node).
   double m_referencePoint;

   /// Polynomial degree (Noise/ImageNoise node).
   unsigned int m_polynomialDegree;
   /// Polynomial coefficients (Noise/ImageNoise node).
   std::vector<double> m_polynomialCoefficients;

};
}

#endif
