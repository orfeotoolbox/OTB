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

#include <otb/ImageNoise.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>

namespace ossimplugins
{


   static const char PREFIX[]          = "imageNoise.";
   static const char TIME_UTC_KW[] = "timeUTC";
   static const char NOISE_ESTIMATE[] = "noiseEstimate.";
   static const char REFERENCE_POINT[] = "referencePoint";
   static const char VALIDITY_RANGE_MIN[] = "validityRangeMin";
   static const char VALIDITY_RANGE_MAX[] = "validityRangeMax";
   static const char POLYNOMIAL_DEGREE[] = "polynomialDegree";
   static const char COEFFICIENT[] = "coefficient";
   static const char NOISE_ESTIMATE_CONFIDENCE[] = "noiseEstimateConfidence";

   ImageNoise::ImageNoise():
      _timeUTC(),
      _validityRangeMin(0.),
      _validityRangeMax(0.),
      _referencePoint(0.),
      _polynomialDegree(0),
      _polynomialCoefficients(0)
   {
   }

   ImageNoise::~ImageNoise()
   {
   }


   ImageNoise::ImageNoise(const ImageNoise& rhs):
      _timeUTC(rhs._timeUTC),
      _validityRangeMin(rhs._validityRangeMin),
      _validityRangeMax(rhs._validityRangeMax),
      _referencePoint(rhs._referencePoint),
      _polynomialDegree(rhs._polynomialDegree),
      _polynomialCoefficients(rhs._polynomialCoefficients)
   {
   }

   ImageNoise& ImageNoise::operator=(const ImageNoise& rhs)
   {
      _timeUTC = rhs._timeUTC;
      _validityRangeMin = rhs._validityRangeMin;
      _validityRangeMax = rhs._validityRangeMax;
      _referencePoint = rhs._referencePoint;
      _polynomialDegree = rhs._polynomialDegree;
      _polynomialCoefficients = rhs._polynomialCoefficients;
      return *this;
   }

   bool ImageNoise::saveState(ossimKeywordlist& kwl, const char* prefix) const
   {
      std::string pfx;
      std::string pfx2;
      if (prefix)
      {
         pfx = prefix;
      }
      pfx += PREFIX;
      kwl.add(pfx.c_str(), TIME_UTC_KW,  _timeUTC);

      pfx2 = pfx + NOISE_ESTIMATE;
      kwl.add(pfx2.c_str(), VALIDITY_RANGE_MIN,  _validityRangeMin);
      kwl.add(pfx2.c_str(), VALIDITY_RANGE_MAX,  _validityRangeMax);
      kwl.add(pfx2.c_str(), REFERENCE_POINT,  _referencePoint);
      kwl.add(pfx2.c_str(), POLYNOMIAL_DEGREE,  _polynomialDegree);

      for(unsigned int i=0 ; i<_polynomialCoefficients.size();i++)
      {
         ossimString iStr = ossimString::toString(i);
         ossimString kw = ossimString(COEFFICIENT) + "["+iStr+ "]";
         kwl.add(pfx2.c_str(), kw.c_str(), _polynomialCoefficients[i]);
      }

      return true;
   }

   bool ImageNoise::loadState(const ossimKeywordlist& kwl, const char* prefix)
   {
      static const char MODULE[] = "ImageNoise::loadState";
      bool result = true;
      std::string pfx;
      std::string pfx2;
      if (prefix)
      {
         pfx = prefix;
      }
      ossimString s;
      const char* lookup = 0;

      pfx += PREFIX;
      lookup = kwl.find(pfx.c_str(), TIME_UTC_KW);
      if (lookup)
      {
         s = lookup;
         _timeUTC = s;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Keyword not found: " << TIME_UTC_KW << " in "<<pfx.c_str()<<" path.\n";
         result = false;
      }
      pfx2 = pfx + NOISE_ESTIMATE;
      lookup = kwl.find(pfx2.c_str(), VALIDITY_RANGE_MIN);
      if (lookup)
      {
         s = lookup;
         _validityRangeMin = s.toDouble();
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
         _validityRangeMax = s.toDouble();
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
         _referencePoint = s.toDouble();
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
         _polynomialDegree = s.toInt32();
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Keyword not found: " << POLYNOMIAL_DEGREE << " in "<<pfx2.c_str()<<" path\n";
         result = false;
      }

      for(unsigned int i=0 ; i<_polynomialDegree+1;i++)
      {
         ossimString iStr = ossimString::toString(i);
         ossimString kw = ossimString(COEFFICIENT) + "["+iStr+ "]";
         lookup = kwl.find(pfx2.c_str(), kw.c_str());
         if (lookup)
         {
            s = lookup;
            _polynomialCoefficients.push_back( s.toDouble() );
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " Keyword not found: " << kw.c_str() << " in "<<pfx2.c_str()<<" path\n";
            result = false;
         }
      }


/*
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
  _date.set_day0hTU(jd);
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
  _date.set_second(d);
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
  _date.set_decimal(d);
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

  _position[0] = pt.x;
  _position[1] = pt.y;
  _position[2] = pt.z;
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

  _speed[0] = pt.x;
  _speed[1] = pt.y;
  _speed[2] = pt.z;
  }
  else
  {
  ossimNotify(ossimNotifyLevel_WARN)
  << MODULE << " Keyword not found: " << VELOCITY_KW << "\n";
  result = false;
  }
*/
      return result;
   }
}
