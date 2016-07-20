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

#include "otb/ImageNoise.h"
#include "ossimKeyWordListUtilities.h"
#include <ossim/base/ossimString.h>

namespace ossimplugins
{

static const char PREFIX[]                    = "imageNoise.";
// static const char TIME_UTC[]               = "timeUTC";
static const char UTC_TIME[]                  = "timeUTC";
static const char NOISE_ESTIMATE[]            = "noiseEstimate.";
static const char REFERENCE_POINT[]           = "referencePoint";
static const char VALIDITY_RANGE_MIN[]        = "validityRangeMin";
static const char VALIDITY_RANGE_MAX[]        = "validityRangeMax";
static const char POLYNOMIAL_DEGREE[]         = "polynomialDegree";
static const char COEFFICIENT[]               = "coefficient";
static const char NOISE_ESTIMATE_CONFIDENCE[] = "noiseEstimateConfidence";

ImageNoise::ImageNoise():
     m_timeUTC(),
     m_validityRangeMin(0.),
     m_validityRangeMax(0.),
     m_referencePoint(0.),
     m_polynomialDegree(0),
     m_polynomialCoefficients(0)
{
}

bool ImageNoise::saveState(ossimKeywordlist& kwl, std::string const& prefix) const
{
   const std::string pfx = prefix + PREFIX;
   add(kwl, pfx, UTC_TIME,  m_timeUTC);

   const std::string pfx2 = pfx + NOISE_ESTIMATE;
   add(kwl, pfx2, VALIDITY_RANGE_MIN, m_validityRangeMin);
   add(kwl, pfx2, VALIDITY_RANGE_MAX, m_validityRangeMax);
   add(kwl, pfx2, REFERENCE_POINT,    m_referencePoint);
   add(kwl, pfx2, POLYNOMIAL_DEGREE,  m_polynomialDegree);

   for(unsigned int i=0 ; i<m_polynomialCoefficients.size();i++)
   {
      ossimString iStr = ossimString::toString(i);
      ossimString kw = ossimString(COEFFICIENT) + "["+iStr+ "]";
      add(kwl, pfx2, kw.string(), m_polynomialCoefficients[i]);
   }

   return true;
}

bool ImageNoise::loadState(const ossimKeywordlist& kwl, std::string const& prefix)
{
   static const char MODULE[] = "ImageNoise::loadState";
   bool result = true;
   const std::string pfx = prefix + PREFIX;
   std::string pfx2;
   ossimString s;
   const char* lookup = 0;

#if 1
   get(kwl, pfx, UTC_TIME, m_timeUTC);
#else
   lookup = kwl.find(pfx.c_str(), UTC_TIME);
   if (lookup)

   {

      s = lookup;
      m_timeUTC = s;

   }
   else

   {

      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << UTC_TIME << " in "<<pfx.c_str()<<" path.\n";
      result = false;

   }
#endif
   pfx2 = pfx + NOISE_ESTIMATE;
#if 1
   get(kwl, pfx2, VALIDITY_RANGE_MIN, m_validityRangeMin);
   get(kwl, pfx2, VALIDITY_RANGE_MAX, m_validityRangeMax);
   get(kwl, pfx2, REFERENCE_POINT,    m_referencePoint);
   get(kwl, pfx2, POLYNOMIAL_DEGREE,  m_polynomialDegree);
#else
   lookup = kwl.find(pfx2.c_str(), VALIDITY_RANGE_MIN);
   if (lookup)
   {
      s = lookup;
      m_validityRangeMin = s.toDouble();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << VALIDITY_RANGE_MIN << " in "<<pfx2.c_str()<<" path.\n";
      result = false;
   }
   lookup = kwl.find(pfx2.c_str(), VALIDITY_RANGE_MAX);
   if (lookup)
   {
      s = lookup;
      m_validityRangeMax = s.toDouble();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << VALIDITY_RANGE_MAX << " in "<<pfx2.c_str()<<" path\n";
      result = false;
   }
   lookup = kwl.find(pfx2.c_str(), REFERENCE_POINT);
   if (lookup)
   {
      s = lookup;
      m_referencePoint = s.toDouble();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << REFERENCE_POINT << " in "<<pfx2.c_str()<<" path\n";
      result = false;
   }
   lookup = kwl.find(pfx2.c_str(), POLYNOMIAL_DEGREE);
   if (lookup)
   {
      s = lookup;
      m_polynomialDegree = s.toInt32();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << POLYNOMIAL_DEGREE << " in "<<pfx2.c_str()<<" path\n";
      result = false;
   }
#endif

   for(unsigned int i=0 ; i<m_polynomialDegree+1;i++)
   {
      ossimString iStr = ossimString::toString(i);
      ossimString kw = ossimString(COEFFICIENT) + "["+iStr+ "]";
#if 1
      double coeff;
      get(kwl, pfx2, kw, coeff);
      m_polynomialCoefficients.push_back(coeff);
#else
      lookup = kwl.find(pfx2.c_str(), kw.c_str());
      if (lookup)
      {
         s = lookup;
         m_polynomialCoefficients.push_back( s.toDouble() );
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Keyword not found: " << kw.c_str() << " in "<<pfx2.c_str()<<" path\n";
         result = false;
      }
#endif
   }


#if 0
   pfx += "ImageNoise.";

   const char* lookup = 0;
   ossimString s;
   double d;

   lookup = kwl.find(pfx.c_str(), DATE_JULIAN_KW);
   if (lookup)
   {
      s = lookup;
      d = s.toDouble();
      JulianDate jd(d);
      m_date.set_day0hTU(jd);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << DATE_JULIAN_KW << "\n";

      result = false;
   }

   lookup = kwl.find(pfx.c_str(), DATE_SECOND_KW);
   if (lookup)
   {
      s = lookup;
      d = s.toDouble();
      m_date.set_second(d);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << DATE_SECOND_KW << "\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), DATE_DECIMAL_KW);
   if (lookup)
   {
      s = lookup;
      d = s.toDouble();
      m_date.set_decimal(d);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << DATE_DECIMAL_KW << "\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), POSITION_KW);
   if (lookup)
   {
      std::string ps = lookup;

      ossimDpt3d pt;
      pt.toPoint(ps);

      m_position[0] = pt.x;
      m_position[1] = pt.y;
      m_position[2] = pt.z;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << POSITION_KW << "\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), VELOCITY_KW);
   if (lookup)
   {
      std::string ps = lookup;

      ossimDpt3d pt;
      pt.toPoint(ps);

      m_speed[0] = pt.x;
      m_speed[1] = pt.y;
      m_speed[2] = pt.z;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << VELOCITY_KW << "\n";
      result = false;
   }
#endif
   return result;
}
}
