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


#ifndef PlatformPosition_h
#define PlatformPosition_h

#include <vector>
#include <ossimPluginConstants.h>
#include <otb/JSDDateTime.h>

class ossimKeywordlist;

namespace ossimplugins
{


   class Ephemeris;
   class HermiteInterpolator;


/**
 * @ingroup SARModel
 * @brief This class handles the platform position
 */
   class OSSIM_PLUGINS_DLL PlatformPosition
   {
   public:
      /**
       * @brief Constructor
       */
      PlatformPosition();
      /**
       * @brief Destructor
       */
      ~PlatformPosition();
      /**
       * @brief Copy constructor
       */
      PlatformPosition(const PlatformPosition& rhs);

      /**
       * @brief Constructor with initialization
       * @param data Ephemeris
       * @param nbrData Number of Ephemeris in data
       * @remarks All the ephemeris have to be in the same coordinates system
       * @remarks This constructor copy the content of data, so it's the
       * calling function that have to manage the memory of data.
       */
      PlatformPosition(Ephemeris** data, int nbrData);

      /**
       * @brief Affectation operator
       */
      PlatformPosition& operator=(const PlatformPosition& rhs);

      /**
       * @brief This function interpolates its ephemeris to create a new ephemeris at the given date and time
       * @param date Date and time at which the interpolation have to be done
       * @return The ephemeris at the given date, or NULL if an error occurs
       */
      Ephemeris* Interpolate(JSDDateTime date) const;


      /**
       * @brief This function interpolates its ephemeris to create and extract platform's position and speed
       * @param date Date and time at which the interpolation have to be done
       * @return true, or false if an error occurs
       */
      bool getPlatformPositionAtTime(JSDDateTime time, std::vector<double>& position, std::vector<double>& speed);

      PlatformPosition* Clone() const
      {
         return new PlatformPosition(*this);
      };

      void setData(Ephemeris** data, int nbrData);
      Ephemeris* getData(int noData) const;

      int getNbrData() const;
      /**
       * @brief Method to save object state to a keyword list.
       * @param kwl Keyword list to save to.
       * @param prefix added to keys when saved.
       * @return true on success, false on error.
       */
      bool saveState(ossimKeywordlist& kwl,
                     const char* prefix=0) const;

      /**
       * @brief Method to the load (recreate) the state of the object from a
       * keyword list. Return true if ok or false on error.
       * @return true if load OK, false on error
       */
      bool loadState(const ossimKeywordlist &kwl, const char *prefix=0);

   protected:

      /**
       * @brief Internal method to initialize data structures
       */
      void InitData(Ephemeris** data, int nbrData);
      void InitAuxiliaryData();

      /**
       * @brief Number of platform positions
       * @see _data
       */
      int _nbrData;

      /**
       * @brief Platform positions
       */
      Ephemeris** _data;

      /**
       * @brief This function deletes all the contents of the class
       */
      void Clear();

   private:
      double * _t;
      double ** _p;
      double ** _dp;
      HermiteInterpolator ** _interpolator;
   };
}

#endif
