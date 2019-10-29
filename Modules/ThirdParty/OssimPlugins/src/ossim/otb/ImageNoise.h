/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef ImageNoise_h
#define ImageNoise_h

#include <vector>
#include <ossim/base/ossimString.h>
#include <ossimPluginConstants.h>


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
   /**
    * @brief Constructor
    */
   ImageNoise();

   /**
    * @brief Destructor
    */
   virtual ~ImageNoise();

   /**
    * @brief Copy constructor
    */
   ImageNoise(const ImageNoise& rhs);

   /**
    * @brief Affectation operator
    */
   ImageNoise& operator=(const ImageNoise& rhs);

   /**
    * @brief Method to save object state to a keyword list.
    * @param kwl Keyword list to save to.
    * @param prefix added to keys when saved.
    * @return true on success, false on error.
    */
   bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;

   /**
    * @brief Method to the load (recreate) the state of the object from a
    * keyword list. Return true if ok or false on error.
    * @return true if load OK, false on error
    */
   bool loadState (const ossimKeywordlist &kwl, const char *prefix=0);

   void set_timeUTC(const ossimString& value)
   {
      _timeUTC = value;
   }

  const ossimString & get_timeUTC(void) const
  {
    return _timeUTC;
  }

   void set_validityRangeMin(double value)
   {
      _validityRangeMin = value;
   }
  
  double get_validityRangeMin(void) const
  {
    return _validityRangeMin;
  }

   void set_validityRangeMax(double value)
   {
      _validityRangeMax = value;
   }

  double get_validityRangeMax(void) const
  {
    return _validityRangeMax;
  }

   void set_referencePoint(double value)
   {
      _referencePoint = value;
   }

  double get_referencePoint(void) const
  {
    return _referencePoint;
  }
  
   void set_polynomialDegree(unsigned int value)
   {
      _polynomialDegree = value;
   }

  unsigned int get_polynomialDegree() const
  {
    return _polynomialDegree;
  }


   void set_polynomialCoefficients(const std::vector<double>& value)
   {
      _polynomialCoefficients = value;
   }

  const std::vector<double> & get_polynomialCoefficients(void) const
  {
    return _polynomialCoefficients;
  }

protected:

      /**
       * @brief TimeUTC (Noise/ImageNoise node).
       */
      ossimString _timeUTC;

      /**
       * @brief ValidityRangeMin (Noise/ImageNoise node).
       */
      double _validityRangeMin;
      /**
       * @brief ValidityRangeMax (Noise/ImageNoise node).
       */
      double _validityRangeMax;
      /**
       * @brief Reference point (Noise/ImageNoise node).
       */
      double _referencePoint;

      /**
       * @brief Polynomial degree (Noise/ImageNoise node).
       */
      unsigned int _polynomialDegree;
      /**
       * @brief Polynomial coefficients (Noise/ImageNoise node).
       */
      std::vector<double> _polynomialCoefficients;

private:
};
}

#endif
